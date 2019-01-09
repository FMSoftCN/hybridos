/*
 * netifd - network interface daemon
 * Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <stdarg.h>
#include <syslog.h>

#include "netifd.h"
#include "ubus.h"
#include "config.h"
#include "system.h"
#include "interface.h"
#include "wireless.h"
#include "proto.h"

unsigned int debug_mask = 0;
const char *main_path = DEFAULT_MAIN_PATH;
const char *config_path = DEFAULT_CONFIG_PATH;
const char *resolv_conf = DEFAULT_RESOLV_CONF;
static char **global_argv;

static struct list_head process_list = LIST_HEAD_INIT(process_list);

#define DEFAULT_LOG_LEVEL L_NOTICE

static int log_level = DEFAULT_LOG_LEVEL;
static const int log_class[] = {
	[L_CRIT] = LOG_CRIT,
	[L_WARNING] = LOG_WARNING,
	[L_NOTICE] = LOG_NOTICE,
	[L_INFO] = LOG_INFO,
	[L_DEBUG] = LOG_DEBUG
};

#ifdef DUMMY_MODE
#define use_syslog false
#else
static bool use_syslog = true;
#endif


static void
netifd_delete_process(struct netifd_process *proc)
{
	list_del(&proc->list);
	ustream_free(&proc->log.stream);
	close(proc->log.fd.fd);
}

void
netifd_log_message(int priority, const char *format, ...)
{
	va_list vl;

	if (priority > log_level)
		return;

	va_start(vl, format);
	if (use_syslog)
		vsyslog(log_class[priority], format, vl);
	else
		vfprintf(stderr, format, vl);
	va_end(vl);
}

static void
netifd_process_log_read_cb(struct ustream *s, int bytes)
{
	struct netifd_process *proc;
	const char *log_prefix;
	char *data;
	int len = 0;

	proc = container_of(s, struct netifd_process, log.stream);
	log_prefix = proc->log_prefix;
	if (!log_prefix)
		log_prefix = "process";

	do {
		char *newline;

		data = ustream_get_read_buf(s, &len);
		if (!len)
			break;

		newline = strchr(data, '\n');

		if (proc->log_overflow) {
			if (newline) {
				len = newline + 1 - data;
				proc->log_overflow = false;
			}
		} else if (newline) {
			*newline = 0;
			len = newline + 1 - data;
			netifd_log_message(L_NOTICE, "%s (%d): %s\n",
				log_prefix, proc->uloop.pid, data);
		} else if (len == s->r.buffer_len) {
			netifd_log_message(L_NOTICE, "%s (%d): %s [...]\n",
				log_prefix, proc->uloop.pid, data);
			proc->log_overflow = true;
		} else
			break;

		ustream_consume(s, len);
	} while (1);
}

static void
netifd_process_cb(struct uloop_process *proc, int ret)
{
	struct netifd_process *np;
	np = container_of(proc, struct netifd_process, uloop);

	while (ustream_poll(&np->log.stream));
	netifd_delete_process(np);
	return np->cb(np, ret);
}

int
netifd_start_process(const char **argv, char **env, struct netifd_process *proc)
{
	int pfds[2];
	int pid;

	netifd_kill_process(proc);

	if (pipe(pfds) < 0)
		return -1;

	if ((pid = fork()) < 0)
		goto error;

	if (!pid) {
		int i;

		if (env) {
			while (*env) {
				putenv(*env);
				env++;
			}
		}
		if (proc->dir_fd >= 0)
			if (fchdir(proc->dir_fd)) {}

		close(pfds[0]);

		for (i = 0; i <= 2; i++) {
			if (pfds[1] == i)
				continue;

			dup2(pfds[1], i);
		}

		if (pfds[1] > 2)
			close(pfds[1]);

		execvp(argv[0], (char **) argv);
		exit(127);
	}

	close(pfds[1]);
	proc->uloop.cb = netifd_process_cb;
	proc->uloop.pid = pid;
	uloop_process_add(&proc->uloop);
	list_add_tail(&proc->list, &process_list);

	system_fd_set_cloexec(pfds[0]);
	proc->log.stream.string_data = true;
	proc->log.stream.notify_read = netifd_process_log_read_cb;
	ustream_fd_init(&proc->log, pfds[0]);

	return 0;

error:
	close(pfds[0]);
	close(pfds[1]);
	return -1;
}

void
netifd_kill_process(struct netifd_process *proc)
{
	if (!proc->uloop.pending)
		return;

	kill(proc->uloop.pid, SIGKILL);
	uloop_process_delete(&proc->uloop);
	netifd_delete_process(proc);
}

static void netifd_do_restart(struct uloop_timeout *timeout)
{
	execvp(global_argv[0], global_argv);
}

int netifd_reload(void)
{
	return config_init_all();
}

void netifd_restart(void)
{
	static struct uloop_timeout main_timer = {
		.cb = netifd_do_restart
	};

	interface_set_down(NULL);
	uloop_timeout_set(&main_timer, 1000);
}

static int usage(const char *progname)
{
	fprintf(stderr, "Usage: %s [options]\n"
		"Options:\n"
		" -d <mask>:		Mask for debug messages\n"
		" -s <path>:		Path to the ubus socket\n"
		" -p <path>:		Path to netifd addons (default: %s)\n"
		" -c <path>:		Path to UCI configuration\n"
		" -h <path>:		Path to the hotplug script\n"
		" -r <path>:		Path to resolv.conf\n"
		" -l <level>:		Log output level (default: %d)\n"
		" -S:			Use stderr instead of syslog for log messages\n"
		"			(default: "DEFAULT_HOTPLUG_PATH")\n"
		"\n", progname, main_path, DEFAULT_LOG_LEVEL);

	return 1;
}

static void
netifd_handle_signal(int signo)
{
	uloop_end();
}

static void
netifd_setup_signals(void)
{
	struct sigaction s;

	memset(&s, 0, sizeof(s));
	s.sa_handler = netifd_handle_signal;
	s.sa_flags = 0;
	sigaction(SIGINT, &s, NULL);
	sigaction(SIGTERM, &s, NULL);
	sigaction(SIGUSR1, &s, NULL);
	sigaction(SIGUSR2, &s, NULL);

	s.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &s, NULL);
}

static void
netifd_kill_processes(void)
{
	struct netifd_process *proc, *tmp;

	list_for_each_entry_safe(proc, tmp, &process_list, list)
		netifd_kill_process(proc);
}

int main(int argc, char **argv)
{
	const char *socket = NULL;
	int ch;

	global_argv = argv;

	while ((ch = getopt(argc, argv, "d:s:p:c:h:r:l:S")) != -1) {
		switch(ch) {
		case 'd':
			debug_mask = strtoul(optarg, NULL, 0);
			break;
		case 's':
			socket = optarg;
			break;
		case 'p':
			main_path = optarg;
			break;
		case 'c':
			config_path = optarg;
			break;
		case 'h':
			hotplug_cmd_path = optarg;
			break;
		case 'r':
			resolv_conf = optarg;
			break;
		case 'l':
			log_level = atoi(optarg);
			if (log_level >= ARRAY_SIZE(log_class))
				log_level = ARRAY_SIZE(log_class) - 1;
			break;
#ifndef DUMMY_MODE
		case 'S':
			use_syslog = false;
			break;
#endif
		default:
			return usage(argv[0]);
		}
	}

	if (use_syslog)
		openlog("netifd", 0, LOG_DAEMON);

	netifd_setup_signals();
	if (netifd_ubus_init(socket) < 0) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return 1;
	}

	proto_shell_init();
	wireless_init();

	if (system_init()) {
		fprintf(stderr, "Failed to initialize system control\n");
		return 1;
	}

	config_init_all();

	uloop_run();
	netifd_kill_processes();

	netifd_ubus_done();

	if (use_syslog)
		closelog();

	return 0;
}
