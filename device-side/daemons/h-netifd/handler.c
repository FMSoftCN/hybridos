/*
 * netifd - network interface daemon
 * Copyright (C) 2012-2013 Felix Fietkau <nbd@openwrt.org>
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

#define _GNU_SOURCE
#include <glob.h>
#include <fcntl.h>
#include <stdio.h>

#include "netifd.h"
#include "system.h"
#include "handler.h"

static int
netifd_dir_push(int fd)
{
	int prev_fd = open(".", O_RDONLY | O_DIRECTORY);
	system_fd_set_cloexec(prev_fd);
	if (fd >= 0)
		if (fchdir(fd)) {}
	return prev_fd;
}

static void
netifd_dir_pop(int prev_fd)
{
	if (prev_fd < 0)
		return;

	if (fchdir(prev_fd)) {}
	close(prev_fd);
}

int netifd_open_subdir(const char *name)
{
	int prev_dir;
	int ret = -1;

	prev_dir = netifd_dir_push(-1);
	if (chdir(main_path)) {
		perror("chdir(main path)");
		goto out;
	}

	ret = open(name, O_RDONLY | O_DIRECTORY);
	if (ret >= 0)
		system_fd_set_cloexec(ret);

out:
	netifd_dir_pop(prev_dir);
	return ret;
}

static void
netifd_init_script_handler(const char *script, json_object *obj, script_dump_cb cb)
{
	json_object *tmp;
	const char *name;

	if (!json_check_type(obj, json_type_object))
		return;

	tmp = json_get_field(obj, "name", json_type_string);
	if (!tmp)
		return;

	name = json_object_get_string(tmp);
	cb(script, name, obj);
}

static void
netifd_parse_script_handler(const char *name, script_dump_cb cb)
{
	struct json_tokener *tok = NULL;
	json_object *obj;
	static char buf[512];
	char *start, *cmd;
	FILE *f;
	int len;

#define DUMP_SUFFIX	" '' dump"

	cmd = alloca(strlen(name) + 1 + sizeof(DUMP_SUFFIX));
	sprintf(cmd, "%s" DUMP_SUFFIX, name);

	f = popen(cmd, "r");
	if (!f)
		return;

	do {
		start = fgets(buf, sizeof(buf), f);
		if (!start)
			continue;

		len = strlen(start);

		if (!tok)
			tok = json_tokener_new();

		obj = json_tokener_parse_ex(tok, start, len);
		if (obj) {
			netifd_init_script_handler(name, obj, cb);
			json_object_put(obj);
			json_tokener_free(tok);
			tok = NULL;
		} else if (start[len - 1] == '\n') {
			json_tokener_free(tok);
			tok = NULL;
		}
	} while (!feof(f) && !ferror(f));

	if (tok)
		json_tokener_free(tok);

	pclose(f);
}

void netifd_init_script_handlers(int dir_fd, script_dump_cb cb)
{
	glob_t g;
	int i, prev_fd;

	prev_fd = netifd_dir_push(dir_fd);
	if (glob("./*.sh", 0, NULL, &g)) {
		netifd_dir_pop(prev_fd);
		return;
	}

	for (i = 0; i < g.gl_pathc; i++)
		netifd_parse_script_handler(g.gl_pathv[i], cb);
	netifd_dir_pop(prev_fd);

	globfree(&g);
}

char *
netifd_handler_parse_config(struct uci_blob_param_list *config, json_object *obj)
{
	struct blobmsg_policy *attrs;
	char *str_buf, *str_cur;
	char const **validate;
	int str_len = 0;
	int i;

	config->n_params = json_object_array_length(obj);
	attrs = calloc(1, sizeof(*attrs) * config->n_params);
	if (!attrs)
		return NULL;

	validate = calloc(1, sizeof(char*) * config->n_params);
	if (!validate)
		goto error;

	config->params = attrs;
	config->validate = validate;
	for (i = 0; i < config->n_params; i++) {
		json_object *cur, *name, *type;

		cur = json_check_type(json_object_array_get_idx(obj, i), json_type_array);
		if (!cur)
			goto error;

		name = json_check_type(json_object_array_get_idx(cur, 0), json_type_string);
		if (!name)
			goto error;

		type = json_check_type(json_object_array_get_idx(cur, 1), json_type_int);
		if (!type)
			goto error;

		attrs[i].name = json_object_get_string(name);
		attrs[i].type = json_object_get_int(type);
		if (attrs[i].type > BLOBMSG_TYPE_LAST)
			goto error;

		str_len += strlen(attrs[i].name) + 1;
	}

	str_buf = malloc(str_len);
	if (!str_buf)
		goto error;

	str_cur = str_buf;
	for (i = 0; i < config->n_params; i++) {
		const char *name = attrs[i].name;
		char *delim;

		attrs[i].name = str_cur;
		str_cur += sprintf(str_cur, "%s", name) + 1;
		delim = strchr(attrs[i].name, ':');
		if (delim) {
			*delim = '\0';
			validate[i] = ++delim;
		} else {
			validate[i] = NULL;
		}
	}

	return str_buf;

error:
	free(attrs);
	if (validate)
		free(validate);
	config->n_params = 0;
	return NULL;
}
