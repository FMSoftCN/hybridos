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
#ifndef __NETIFD_HANDLER_H
#define __NETIFD_HANDLER_H

#include <libubox/blobmsg_json.h>
#include <json-c/json.h>
#include "config.h"

typedef void (*script_dump_cb)(const char *script, const char *name, json_object *obj);

static inline json_object *
json_check_type(json_object *obj, json_type type)
{
	if (!obj)
		return NULL;

	if (json_object_get_type(obj) != type)
		return NULL;

	return obj;
}

static inline json_object *
json_get_field(json_object *obj, const char *name, json_type type)
{
	return json_object_object_get_ex(obj, name, &obj) ?
			json_check_type(obj, type) : NULL;
}

int netifd_open_subdir(const char *name);
void netifd_init_script_handlers(int dir_fd, script_dump_cb cb);
char *netifd_handler_parse_config(struct uci_blob_param_list *config, json_object *obj);

#endif
