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
#ifndef __INTERFACE_IP_H
#define __INTERFACE_IP_H

#include "interface.h"

enum device_addr_flags {
	/* address family for routes and addresses */
	DEVADDR_INET4		= (0 << 0),
	DEVADDR_INET6		= (1 << 0),
	DEVADDR_FAMILY		= DEVADDR_INET4 | DEVADDR_INET6,

	/* externally added address */
	DEVADDR_EXTERNAL	= (1 << 2),

	/* route overrides the default interface metric */
	DEVROUTE_METRIC		= (1 << 3),

	/* route overrides the default interface mtu */
	DEVROUTE_MTU		= (1 << 4),

	/* route overrides the default proto type */
	DEVROUTE_PROTO		= (1 << 5),

	/* address is off-link (no subnet-route) */
	DEVADDR_OFFLINK		= (1 << 6),

	/* route resides in different table */
	DEVROUTE_TABLE		= (1 << 7),

	/* route resides in default source-route table */
	DEVROUTE_SRCTABLE	= (1 << 8),

	/* route is on-link */
	DEVROUTE_ONLINK		= (1 << 9),

	/* route overrides the default route type */
	DEVROUTE_TYPE		= (1 << 10),
};

union if_addr {
	struct in_addr in;
	struct in6_addr in6;
};

struct device_prefix_assignment {
	struct list_head head;
	int32_t assigned;
	uint8_t length;
	int weight;
	struct in6_addr addr;
	bool enabled;
	char name[];
};

struct device_prefix {
	struct vlist_node node;
	struct list_head head;
	struct list_head assignments;
	struct interface *iface;
	time_t valid_until;
	time_t preferred_until;

	struct in6_addr excl_addr;
	uint8_t excl_length;

	struct in6_addr addr;
	uint8_t length;

	char pclass[];
};

struct device_route {
	struct vlist_node node;
	struct interface *iface;

	bool enabled;
	bool keep;
	bool failed;

	union if_addr nexthop;
	int mtu;
	unsigned int type;
	unsigned int proto;
	time_t valid_until;

	/* must be last */
	enum device_addr_flags flags;
	int metric; // there can be multiple routes to the same target
	unsigned int table;
	unsigned int mask;
	unsigned int sourcemask;
	union if_addr addr;
	union if_addr source;
};

struct device_addr {
	struct vlist_node node;
	bool enabled;
	bool failed;
	unsigned int policy_table;

	struct device_route subnet;

	/* ipv4 only */
	uint32_t broadcast;
	uint32_t point_to_point;

	/* ipv6 only */
	time_t valid_until;
	time_t preferred_until;
	char *pclass;

	/* must be last */
	enum device_addr_flags flags;
	unsigned int mask;
	union if_addr addr;
};

struct device_source_table {
	struct list_head head;
	uint32_t table;
	uint16_t refcount;
	uint8_t v6;
	uint8_t mask;
	union if_addr addr;
};

struct dns_server {
	struct vlist_simple_node node;
	int af;
	union if_addr addr;
};

struct dns_search_domain {
	struct vlist_simple_node node;
	char name[];
};

extern const struct uci_blob_param_list route_attr_list;
extern struct list_head prefixes;

void interface_ip_init(struct interface *iface);
void interface_add_dns_server(struct interface_ip_settings *ip, const char *str);
void interface_add_dns_server_list(struct interface_ip_settings *ip, struct blob_attr *list);
void interface_add_dns_search_list(struct interface_ip_settings *ip, struct blob_attr *list);
void interface_write_resolv_conf(void);

void interface_ip_add_route(struct interface *iface, struct blob_attr *attr, bool v6);

void interface_ip_update_start(struct interface_ip_settings *ip);
void interface_ip_update_complete(struct interface_ip_settings *ip);
void interface_ip_flush(struct interface_ip_settings *ip);
void interface_ip_set_enabled(struct interface_ip_settings *ip, bool enabled);
void interface_ip_update_metric(struct interface_ip_settings *ip, int metric);

struct interface *interface_ip_add_target_route(union if_addr *addr, bool v6, struct interface *iface);

struct device_prefix* interface_ip_add_device_prefix(struct interface *iface,
		struct in6_addr *addr, uint8_t length, time_t valid_until, time_t preferred_until,
		struct in6_addr *excl_addr, uint8_t excl_length, const char *pclass);
void interface_ip_set_ula_prefix(const char *prefix);
void interface_refresh_assignments(bool hint);

#endif
