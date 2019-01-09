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
#ifndef __NETIFD_H
#define __NETIFD_H

#include <sys/socket.h>
#include <net/if.h>

#include <stdbool.h>
#include <stdio.h>

#include <libubox/uloop.h>
#include <libubox/ustream.h>
#include <libubox/utils.h>

#include <libubus.h>

#include "utils.h"

#ifdef DUMMY_MODE
#define DEFAULT_MAIN_PATH	"./examples"
#define DEFAULT_CONFIG_PATH	"./config"
#define DEFAULT_HOTPLUG_PATH	"./examples/hotplug-cmd"
#define DEFAULT_RESOLV_CONF	"./tmp/resolv.conf"
#else
#define DEFAULT_MAIN_PATH	"/lib/netifd"
#define DEFAULT_CONFIG_PATH	NULL /* use the default set in libuci */
#define DEFAULT_HOTPLUG_PATH	"/sbin/hotplug-call"
#define DEFAULT_RESOLV_CONF	"/tmp/resolv.conf.auto"
#endif

extern const char *resolv_conf;
extern char *hotplug_cmd_path;
extern unsigned int debug_mask;

enum {
	L_CRIT,
	L_WARNING,
	L_NOTICE,
	L_INFO,
	L_DEBUG
};

enum {
	DEBUG_SYSTEM	= 0,
	DEBUG_DEVICE	= 1,
	DEBUG_INTERFACE	= 2,
	DEBUG_WIRELESS	= 3,
};

#ifdef DEBUG
#define DPRINTF(format, ...) fprintf(stderr, "%s(%d): " format, __func__, __LINE__, ## __VA_ARGS__)
#define D(level, format, ...) do { \
		if (debug_mask & (1 << (DEBUG_ ## level))) \
				DPRINTF(format, ##__VA_ARGS__); \
	} while (0)
#else
#define DPRINTF(format, ...) no_debug(0, format, ## __VA_ARGS__)
#define D(level, format, ...) no_debug(DEBUG_ ## level, format, ## __VA_ARGS__)
#endif

#define LOG_BUF_SIZE	256

static inline void no_debug(int level, const char *fmt, ...)
{
}

struct netifd_process {
	struct list_head list;
	struct uloop_process uloop;
	void (*cb)(struct netifd_process *, int ret);
	int dir_fd;

	struct ustream_fd log;
	const char *log_prefix;
	bool log_overflow;
};

void netifd_log_message(int priority, const char *format, ...);

int netifd_start_process(const char **argv, char **env, struct netifd_process *proc);
void netifd_kill_process(struct netifd_process *proc);

struct device;
struct interface;

extern const char *main_path;
extern const char *config_path;
void netifd_restart(void);
int netifd_reload(void);

#endif
