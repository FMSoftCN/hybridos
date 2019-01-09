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
#ifndef __NETIFD_INTERFACE_H
#define __NETIFD_INTERFACE_H

#include "device.h"
#include "config.h"

struct interface;
struct interface_proto_state;

enum interface_event {
	IFEV_DOWN,
	IFEV_UP,
	IFEV_UP_FAILED,
	IFEV_UPDATE,
	IFEV_FREE,
	IFEV_RELOAD,
	IFEV_LINK_UP,
	/* send when a new interface created. This is before proto handlers has been attached. */
	IFEV_CREATE,
};

enum interface_state {
	IFS_SETUP,
	IFS_UP,
	IFS_TEARDOWN,
	IFS_DOWN,
};

enum interface_config_state {
	IFC_NORMAL,
	IFC_RELOAD,
	IFC_REMOVE
};

enum interface_id_selection_type {
	IFID_FIXED,
	IFID_RANDOM,
	IFID_EUI64
};

enum interface_update_flags {
	IUF_ADDRESS	= (1 << 0),
	IUF_ROUTE	= (1 << 1),
	IUF_PREFIX	= (1 << 2),
	IUF_DATA	= (1 << 3),
};

struct interface_error {
	struct list_head list;

	const char *subsystem;
	const char *code;
	const char *data[];
};

struct interface_user {
	struct list_head list;
	struct interface *iface;
	void (*cb)(struct interface_user *dep, struct interface *iface, enum interface_event ev);
};

struct interface_ip_settings {
	struct interface *iface;
	bool enabled;
	bool no_defaultroute;
	bool no_dns;
	bool no_delegation;

	struct vlist_tree addr;
	struct vlist_tree route;
	struct vlist_tree prefix;

	struct vlist_simple_tree dns_servers;
	struct vlist_simple_tree dns_search;
};

struct interface_data {
	struct avl_node node;
	struct blob_attr data[];
};

struct interface_assignment_class {
	struct list_head head;
	char name[];
};

/*
 * interface configuration
 */
struct interface {
	struct vlist_node node;
	struct list_head hotplug_list;
	enum interface_event hotplug_ev;

	const char *name;
	const char *ifname;

	bool available;
	bool autostart;
	bool config_autostart;
	bool device_config;
	bool enabled;
	bool link_state;
	bool force_link;
	bool dynamic;
	bool policy_rules_set;
	bool link_up_event;

	time_t start_time;
	enum interface_state state;
	enum interface_config_state config_state;
	enum interface_update_flags updated;

	struct list_head users;

	/* for alias interface */
	const char *parent_ifname;
	struct interface_user parent_iface;

	/* main interface that the interface is bound to */
	struct device_user main_dev;
	struct device_user ext_dev;

	/* interface that layer 3 communication will go through */
	struct device_user l3_dev;

	struct blob_attr *config;

	/* primary protocol state */
	const struct proto_handler *proto_handler;
	struct interface_proto_state *proto;

	struct interface_ip_settings proto_ip;
	struct interface_ip_settings config_ip;
	struct vlist_tree host_routes;

	int metric;
	int dns_metric;
	unsigned int ip4table;
	unsigned int ip6table;

	/* IPv6 assignment parameters */
	enum interface_id_selection_type assignment_iface_id_selection;
	struct in6_addr assignment_fixed_iface_id;
	uint8_t assignment_length;
	int32_t assignment_hint;
	struct list_head assignment_classes;
	int assignment_weight;

	/* errors/warnings while trying to bring up the interface */
	struct list_head errors;

	/* extra data provided by protocol handlers or modules */
	struct avl_tree data;

	struct uloop_timeout remove_timer;
	struct ubus_object ubus;
};


extern struct vlist_tree interfaces;
extern const struct uci_blob_param_list interface_attr_list;

struct interface *interface_alloc(const char *name, struct blob_attr *config);

void interface_set_dynamic(struct interface *iface);

void interface_add(struct interface *iface, struct blob_attr *config);
bool interface_add_alias(struct interface *iface, struct blob_attr *config);

void interface_set_proto_state(struct interface *iface, struct interface_proto_state *state);

void interface_set_available(struct interface *iface, bool new_state);
int interface_set_up(struct interface *iface);
int interface_set_down(struct interface *iface);
void __interface_set_down(struct interface *iface, bool force);
int interface_renew(struct interface *iface);

void interface_set_main_dev(struct interface *iface, struct device *dev);
void interface_set_l3_dev(struct interface *iface, struct device *dev);

void interface_add_user(struct interface_user *dep, struct interface *iface);
void interface_remove_user(struct interface_user *dep);

int interface_remove_link(struct interface *iface, struct device *dev);
int interface_handle_link(struct interface *iface, const char *name, bool add, bool link_ext);

void interface_add_error(struct interface *iface, const char *subsystem,
			 const char *code, const char **data, int n_data);

int interface_add_data(struct interface *iface, const struct blob_attr *data);
int interface_parse_data(struct interface *iface, const struct blob_attr *attr);

void interface_update_start(struct interface *iface, const bool keep_old);
void interface_update_complete(struct interface *iface);

void interface_start_pending(void);

#endif
