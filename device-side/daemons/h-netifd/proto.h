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
#ifndef __NETIFD_PROTO_H
#define __NETIFD_PROTO_H

struct interface;
struct interface_proto_state;
struct proto_handler;

enum interface_proto_event {
	IFPEV_UP,
	IFPEV_DOWN,
	IFPEV_LINK_LOST,
	IFPEV_RENEW,
};

enum interface_proto_cmd {
	PROTO_CMD_SETUP,
	PROTO_CMD_TEARDOWN,
	PROTO_CMD_RENEW,
};

enum {
	PROTO_FLAG_IMMEDIATE = (1 << 0),
	PROTO_FLAG_NODEV = (1 << 1),
	PROTO_FLAG_INIT_AVAILABLE = (1 << 2),
	PROTO_FLAG_RENEW_AVAILABLE = (1 << 3),
	PROTO_FLAG_FORCE_LINK_DEFAULT = (1 << 4),
	PROTO_FLAG_LASTERROR = (1 << 5),
	PROTO_FLAG_TEARDOWN_ON_L3_LINK_DOWN = (1 << 6),
	PROTO_FLAG_NO_TASK = (1 << 7),
};

struct interface_proto_state {
	const struct proto_handler *handler;
	struct interface *iface;

	/* filled in by the protocol user */
	void (*proto_event)(struct interface_proto_state *, enum interface_proto_event ev);

	/* filled in by the protocol handler */
	int (*notify)(struct interface_proto_state *, struct blob_attr *data);
	int (*cb)(struct interface_proto_state *, enum interface_proto_cmd cmd, bool force);
	void (*free)(struct interface_proto_state *);
};


struct proto_handler {
	struct avl_node avl;

	unsigned int flags;

	const char *name;
	const struct uci_blob_param_list *config_params;

	struct interface_proto_state *(*attach)(const struct proto_handler *h,
		struct interface *iface, struct blob_attr *attr);
};

extern const struct uci_blob_param_list proto_ip_attr;

void add_proto_handler(struct proto_handler *p);
void proto_init_interface(struct interface *iface, struct blob_attr *attr);
void proto_attach_interface(struct interface *iface, const char *proto_name);
int interface_proto_event(struct interface_proto_state *proto,
			  enum interface_proto_cmd cmd, bool force);

unsigned int parse_netmask_string(const char *str, bool v6);
int proto_apply_static_ip_settings(struct interface *iface, struct blob_attr *attr);
int proto_apply_ip_settings(struct interface *iface, struct blob_attr *attr, bool ext);
void proto_dump_handlers(struct blob_buf *b);
void proto_shell_init(void);

#endif
