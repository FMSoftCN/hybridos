/*
 * netifd - network interface daemon
 * Copyright (C) 2013 Felix Fietkau <nbd@openwrt.org>
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
#ifndef __NETIFD_WIRELESS_H
#define __NETIFD_WIRELESS_H

#include <libubox/utils.h>
#include "interface.h"

extern struct vlist_tree wireless_devices;
extern struct avl_tree wireless_drivers;

struct wireless_driver {
	struct avl_node node;

	const char *name;
	const char *script;

	struct {
		char *buf;
		struct uci_blob_param_list *config;
	} device, interface;
};

struct wireless_device {
	struct vlist_node node;

	struct wireless_driver *drv;
	struct vlist_tree interfaces;
	char *name;

	struct netifd_process script_task;
	struct uloop_timeout timeout;
	struct uloop_timeout poll;

	struct list_head script_proc;
	struct uloop_fd script_proc_fd;
	struct uloop_timeout script_check;

	struct ubus_request_data *kill_request;

	struct blob_attr *prev_config;
	struct blob_attr *config;
	struct blob_attr *data;

	bool autostart;
	bool disabled;
	bool retry_setup_failed;

	enum interface_state state;
	enum interface_config_state config_state;
	bool cancel;
	int retry;

	int vif_idx;
};

struct wireless_interface {
	struct vlist_node node;
	const char *section;
	char *name;

	struct wireless_device *wdev;

	struct blob_attr *config;
	struct blob_attr *data;

	const char *ifname;
	struct blob_attr *network;
	bool isolate;
	bool ap_mode;
};

struct wireless_process {
	struct list_head list;

	const char *exe;
	int pid;

	bool required;
};

void wireless_device_create(struct wireless_driver *drv, const char *name, struct blob_attr *data);
void wireless_device_set_up(struct wireless_device *wdev);
void wireless_device_set_down(struct wireless_device *wdev);
void wireless_device_status(struct wireless_device *wdev, struct blob_buf *b);
void wireless_device_get_validate(struct wireless_device *wdev, struct blob_buf *b);
void wireless_interface_create(struct wireless_device *wdev, struct blob_attr *data, const char *section);
int wireless_device_notify(struct wireless_device *wdev, struct blob_attr *data,
			   struct ubus_request_data *req);

void wireless_start_pending(void);
void wireless_init(void);

#endif
