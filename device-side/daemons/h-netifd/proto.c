/*
 * netifd - network interface daemon
 * Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2012 Steven Barth <steven@midlink.org>
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "netifd.h"
#include "system.h"
#include "interface.h"
#include "interface-ip.h"
#include "proto.h"

static struct avl_tree handlers;

enum {
	OPT_IPADDR,
	OPT_IP6ADDR,
	OPT_NETMASK,
	OPT_BROADCAST,
	OPT_PTPADDR,
	OPT_GATEWAY,
	OPT_IP6GW,
	OPT_IP6PREFIX,
	OPT_IP6DEPRECATED,
	__OPT_MAX,
};

static const struct blobmsg_policy proto_ip_attributes[__OPT_MAX] = {
	[OPT_IPADDR] = { .name = "ipaddr", .type = BLOBMSG_TYPE_ARRAY },
	[OPT_IP6ADDR] = { .name = "ip6addr", .type = BLOBMSG_TYPE_ARRAY },
	[OPT_NETMASK] = { .name = "netmask", .type = BLOBMSG_TYPE_STRING },
	[OPT_BROADCAST] = { .name = "broadcast", .type = BLOBMSG_TYPE_STRING },
	[OPT_PTPADDR] = { .name = "ptpaddr", .type = BLOBMSG_TYPE_STRING },
	[OPT_GATEWAY] = { .name = "gateway", .type = BLOBMSG_TYPE_STRING },
	[OPT_IP6GW] = { .name = "ip6gw", .type = BLOBMSG_TYPE_STRING },
	[OPT_IP6PREFIX] = { .name = "ip6prefix", .type = BLOBMSG_TYPE_ARRAY },
	[OPT_IP6DEPRECATED] = { .name = "ip6deprecated", .type = BLOBMSG_TYPE_BOOL },
};

static const struct uci_blob_param_info proto_ip_attr_info[__OPT_MAX] = {
	[OPT_IPADDR] = { .type = BLOBMSG_TYPE_STRING },
	[OPT_IP6ADDR] = { .type = BLOBMSG_TYPE_STRING },
	[OPT_IP6PREFIX] = { .type = BLOBMSG_TYPE_STRING },
};

static const char * const proto_ip_validate[__OPT_MAX] = {
	[OPT_IPADDR] = "ip4addr",
	[OPT_IP6ADDR] = "ip6addr",
	[OPT_NETMASK] = "netmask",
	[OPT_BROADCAST] = "ipaddr",
	[OPT_PTPADDR] = "ip4addr",
	[OPT_GATEWAY] = "ip4addr",
	[OPT_IP6GW] = "ip6addr",
	[OPT_IP6PREFIX] = "ip6addr",
};

const struct uci_blob_param_list proto_ip_attr = {
	.n_params = __OPT_MAX,
	.params = proto_ip_attributes,
	.validate = proto_ip_validate,
	.info = proto_ip_attr_info,
};

enum {
	ADDR_IPADDR,
	ADDR_MASK,
	ADDR_BROADCAST,
	ADDR_PTP,
	ADDR_PREFERRED,
	ADDR_VALID,
	ADDR_OFFLINK,
	ADDR_CLASS,
	__ADDR_MAX
};

static const struct blobmsg_policy proto_ip_addr[__ADDR_MAX] = {
	[ADDR_IPADDR] = { .name = "ipaddr", .type = BLOBMSG_TYPE_STRING },
	[ADDR_MASK] = { .name = "mask", .type = BLOBMSG_TYPE_STRING },
	[ADDR_BROADCAST] = { .name = "broadcast", .type = BLOBMSG_TYPE_STRING },
	[ADDR_PTP] = { .name = "ptp", .type = BLOBMSG_TYPE_STRING },
	[ADDR_PREFERRED] = { .name = "preferred", .type = BLOBMSG_TYPE_INT32 },
	[ADDR_VALID] = { .name = "valid", .type = BLOBMSG_TYPE_INT32 },
	[ADDR_OFFLINK] = { .name = "offlink", .type = BLOBMSG_TYPE_BOOL },
	[ADDR_CLASS] = { .name = "class", .type = BLOBMSG_TYPE_STRING },
};

static struct device_addr *
alloc_device_addr(bool v6, bool ext)
{
	struct device_addr *addr;

	addr = calloc(1, sizeof(*addr));
	if (!addr)
		return NULL;

	addr->flags = v6 ? DEVADDR_INET6 : DEVADDR_INET4;
	if (ext)
		addr->flags |= DEVADDR_EXTERNAL;

	return addr;
}

static bool
parse_addr(struct interface *iface, const char *str, bool v6, int mask,
	   bool ext, uint32_t broadcast, uint32_t ptp, bool deprecated)
{
	struct device_addr *addr;
	int af = v6 ? AF_INET6 : AF_INET;

	addr = alloc_device_addr(v6, ext);
	if (!addr)
		return false;

	addr->mask = mask;
	if (!parse_ip_and_netmask(af, str, &addr->addr, &addr->mask))
		goto error;

	if (!v6) {
		if (IN_EXPERIMENTAL(ntohl(addr->addr.in.s_addr)))
			goto error;

	} else if (IN6_IS_ADDR_MULTICAST(&addr->addr.in6))
		goto error;

	if (broadcast)
		addr->broadcast = broadcast;

	if (ptp)
		addr->point_to_point = ptp;

	if (deprecated)
		addr->preferred_until = system_get_rtime();

	vlist_add(&iface->proto_ip.addr, &addr->node, &addr->flags);
	return true;

error:
	interface_add_error(iface, "proto", "INVALID_ADDRESS", &str, 1);
	free(addr);

	return false;
}

static int
parse_static_address_option(struct interface *iface, struct blob_attr *attr,
			    bool v6, int netmask, bool ext, uint32_t broadcast,
			    uint32_t ptp, bool deprecated)
{
	struct blob_attr *cur;
	int n_addr = 0;
	int rem;

	blobmsg_for_each_attr(cur, attr, rem) {
		if (blobmsg_type(cur) != BLOBMSG_TYPE_STRING)
			return -1;

		n_addr++;
		if (!parse_addr(iface, blobmsg_data(cur), v6, netmask, ext,
				broadcast, ptp, deprecated))
			return -1;
	}

	return n_addr;
}

static struct device_addr *
parse_address_item(struct blob_attr *attr, bool v6, bool ext)
{
	struct device_addr *addr;
	struct blob_attr *tb[__ADDR_MAX];
	struct blob_attr *cur;

	if (blobmsg_type(attr) != BLOBMSG_TYPE_TABLE)
		return NULL;

	addr = alloc_device_addr(v6, ext);
	if (!addr)
		return NULL;

	blobmsg_parse(proto_ip_addr, __ADDR_MAX, tb, blobmsg_data(attr), blobmsg_data_len(attr));

	addr->mask = v6 ? 128 : 32;
	if ((cur = tb[ADDR_MASK])) {
		unsigned int new_mask;

		new_mask = parse_netmask_string(blobmsg_data(cur), v6);
		if (new_mask > addr->mask)
			goto error;

		addr->mask = new_mask;
	}

	cur = tb[ADDR_IPADDR];
	if (!cur)
		goto error;

	if (!inet_pton(v6 ? AF_INET6 : AF_INET, blobmsg_data(cur), &addr->addr))
		goto error;

	if ((cur = tb[ADDR_OFFLINK]) && blobmsg_get_bool(cur))
		addr->flags |= DEVADDR_OFFLINK;

	if (!v6) {
		if ((cur = tb[ADDR_BROADCAST]) &&
		    !inet_pton(AF_INET, blobmsg_data(cur), &addr->broadcast))
			goto error;
		if ((cur = tb[ADDR_PTP]) &&
		    !inet_pton(AF_INET, blobmsg_data(cur), &addr->point_to_point))
			goto error;
	} else {
		time_t now = system_get_rtime();
		if ((cur = tb[ADDR_PREFERRED])) {
			int64_t preferred = blobmsg_get_u32(cur);
			int64_t preferred_until = preferred + (int64_t)now;
			if (preferred_until <= LONG_MAX && preferred != 0xffffffffLL)
				addr->preferred_until = preferred_until;
		}

		if ((cur = tb[ADDR_VALID])) {
			int64_t valid = blobmsg_get_u32(cur);
			int64_t valid_until = valid + (int64_t)now;
			if (valid_until <= LONG_MAX && valid != 0xffffffffLL)
				addr->valid_until = valid_until;

		}

		if (addr->valid_until) {
			if (!addr->preferred_until)
				addr->preferred_until = addr->valid_until;
			else if (addr->preferred_until > addr->valid_until)
				goto error;
		}

		if ((cur = tb[ADDR_CLASS]))
			addr->pclass = strdup(blobmsg_get_string(cur));
	}

	return addr;

error:
	free(addr);
	return NULL;
}

static int
parse_address_list(struct interface *iface, struct blob_attr *attr, bool v6,
		   bool ext)
{
	struct device_addr *addr;
	struct blob_attr *cur;
	int n_addr = 0;
	int rem;

	blobmsg_for_each_attr(cur, attr, rem) {
		addr = parse_address_item(cur, v6, ext);
		if (!addr)
			return -1;

		n_addr++;
		vlist_add(&iface->proto_ip.addr, &addr->node, &addr->flags);
	}

	return n_addr;
}

static bool
parse_gateway_option(struct interface *iface, struct blob_attr *attr, bool v6)
{
	struct device_route *route;
	const char *str = blobmsg_data(attr);
	int af = v6 ? AF_INET6 : AF_INET;

	route = calloc(1, sizeof(*route));
	if (!route)
		return NULL;

	if (!inet_pton(af, str, &route->nexthop)) {
		interface_add_error(iface, "proto", "INVALID_GATEWAY", &str, 1);
		free(route);
		return false;
	}

	route->mask = 0;
	route->flags = (v6 ? DEVADDR_INET6 : DEVADDR_INET4);
	route->metric = iface->metric;

	unsigned int table = (v6) ? iface->ip6table : iface->ip4table;
	if (table) {
		route->table = table;
		route->flags |= DEVROUTE_SRCTABLE;
	}

	vlist_add(&iface->proto_ip.route, &route->node, route);

	return true;
}

static bool
parse_prefix_option(struct interface *iface, const char *str, size_t len)
{
	char buf[128] = {0}, *saveptr;
	if (len >= sizeof(buf))
		return false;

	memcpy(buf, str, len);
	char *addrstr = strtok_r(buf, "/", &saveptr);
	if (!addrstr)
		return false;

	char *lengthstr = strtok_r(NULL, ",", &saveptr);
	if (!lengthstr)
		return false;

	char *prefstr = strtok_r(NULL, ",", &saveptr);
	char *validstr = (!prefstr) ? NULL : strtok_r(NULL, ",", &saveptr);
	char *addstr = (!validstr) ? NULL : strtok_r(NULL, ",", &saveptr);
	const char *pclass = NULL;

	int64_t pref = (!prefstr) ? 0 : strtoul(prefstr, NULL, 10);
	int64_t valid = (!validstr) ? 0 : strtoul(validstr, NULL, 10);

	uint8_t length = strtoul(lengthstr, NULL, 10), excl_length = 0;
	if (length < 1 || length > 64)
		return false;

	struct in6_addr addr, excluded, *excludedp = NULL;
	if (inet_pton(AF_INET6, addrstr, &addr) < 1)
		return false;

	for (; addstr; addstr = strtok_r(NULL, ",", &saveptr)) {
		char *key = NULL, *val = NULL, *addsaveptr;
		if (!(key = strtok_r(addstr, "=", &addsaveptr)) ||
				!(val = strtok_r(NULL, ",", &addsaveptr)))
			continue;

		if (!strcmp(key, "excluded")) {
			char *sep = strchr(val, '/');
			if (!sep)
				return false;

			*sep = 0;
			excl_length = atoi(sep + 1);

			if (inet_pton(AF_INET6, val, &excluded) < 1)
				return false;

			excludedp = &excluded;
		} else if (!strcmp(key, "class")) {
			pclass = val;
		}

	}




	int64_t now = system_get_rtime();
	time_t preferred_until = 0;
	if (prefstr && pref != 0xffffffffLL && pref + now <= LONG_MAX)
		preferred_until = pref + now;

	time_t valid_until = 0;
	if (validstr && valid != 0xffffffffLL && valid + now <= LONG_MAX)
		valid_until = valid + now;

	interface_ip_add_device_prefix(iface, &addr, length,
			valid_until, preferred_until,
			excludedp, excl_length, pclass);
	return true;
}

static int
parse_prefix_list(struct interface *iface, struct blob_attr *attr)
{
	struct blob_attr *cur;
	int n_addr = 0;
	int rem;

	blobmsg_for_each_attr(cur, attr, rem) {
		if (blobmsg_type(cur) != BLOBMSG_TYPE_STRING)
			return -1;

		n_addr++;
		if (!parse_prefix_option(iface, blobmsg_data(cur),
				blobmsg_data_len(cur)))
			return -1;
	}

	return n_addr;
}

int
proto_apply_static_ip_settings(struct interface *iface, struct blob_attr *attr)
{
	struct blob_attr *tb[__OPT_MAX];
	struct blob_attr *cur;
	const char *error;
	unsigned int netmask = 32;
	bool ip6deprecated;
	int n_v4 = 0, n_v6 = 0;
	struct in_addr bcast = {}, ptp = {};

	blobmsg_parse(proto_ip_attributes, __OPT_MAX, tb, blob_data(attr), blob_len(attr));

	if ((cur = tb[OPT_NETMASK])) {
		netmask = parse_netmask_string(blobmsg_data(cur), false);
		if (netmask > 32) {
			error = "INVALID_NETMASK";
			goto error;
		}
	}

	if ((cur = tb[OPT_BROADCAST])) {
		if (!inet_pton(AF_INET, blobmsg_data(cur), &bcast)) {
			error = "INVALID_BROADCAST";
			goto error;
		}
	}

	if ((cur = tb[OPT_PTPADDR])) {
		if (!inet_pton(AF_INET, blobmsg_data(cur), &ptp)) {
			error = "INVALID_PTPADDR";
			goto error;
		}
	}

	ip6deprecated = blobmsg_get_bool_default(tb[OPT_IP6DEPRECATED], false);

	if ((cur = tb[OPT_IPADDR]))
		n_v4 = parse_static_address_option(iface, cur, false,
			netmask, false, bcast.s_addr, ptp.s_addr, false);

	if ((cur = tb[OPT_IP6ADDR]))
		n_v6 = parse_static_address_option(iface, cur, true,
			128, false, 0, 0, ip6deprecated);

	if ((cur = tb[OPT_IP6PREFIX]))
		if (parse_prefix_list(iface, cur) < 0)
			goto out;

	if (n_v4 < 0 || n_v6 < 0)
		goto out;

	if ((cur = tb[OPT_GATEWAY])) {
		if (n_v4 && !parse_gateway_option(iface, cur, false))
			goto out;
	}

	if ((cur = tb[OPT_IP6GW])) {
		if (n_v6 && !parse_gateway_option(iface, cur, true))
			goto out;
	}

	return 0;

error:
	interface_add_error(iface, "proto", error, NULL, 0);
out:
	return -1;
}

int
proto_apply_ip_settings(struct interface *iface, struct blob_attr *attr, bool ext)
{
	struct blob_attr *tb[__OPT_MAX];
	struct blob_attr *cur;
	int n_v4 = 0, n_v6 = 0;

	blobmsg_parse(proto_ip_attributes, __OPT_MAX, tb, blob_data(attr), blob_len(attr));

	if ((cur = tb[OPT_IPADDR]))
		n_v4 = parse_address_list(iface, cur, false, ext);

	if ((cur = tb[OPT_IP6ADDR]))
		n_v6 = parse_address_list(iface, cur, true, ext);

	if ((cur = tb[OPT_IP6PREFIX]))
		if (parse_prefix_list(iface, cur) < 0)
			goto out;

	if (n_v4 < 0 || n_v6 < 0)
		goto out;

	if ((cur = tb[OPT_GATEWAY])) {
		if (n_v4 && !parse_gateway_option(iface, cur, false))
			goto out;
	}

	if ((cur = tb[OPT_IP6GW])) {
		if (n_v6 && !parse_gateway_option(iface, cur, true))
			goto out;
	}

	return 0;

out:
	return -1;
}

void add_proto_handler(struct proto_handler *p)
{
	if (!handlers.comp)
		avl_init(&handlers, avl_strcmp, false, NULL);

	if (p->avl.key)
		return;

	p->avl.key = p->name;
	avl_insert(&handlers, &p->avl);
}

static void
default_proto_free(struct interface_proto_state *proto)
{
	free(proto);
}

static int
invalid_proto_handler(struct interface_proto_state *proto,
		      enum interface_proto_cmd cmd, bool force)
{
	return -1;
}

static int
no_proto_handler(struct interface_proto_state *proto,
		 enum interface_proto_cmd cmd, bool force)
{
	return 0;
}

static struct interface_proto_state *
default_proto_attach(const struct proto_handler *h,
		     struct interface *iface, struct blob_attr *attr)
{
	struct interface_proto_state *proto;

	proto = calloc(1, sizeof(*proto));
	if (!proto)
		return NULL;

	proto->free = default_proto_free;
	proto->cb = no_proto_handler;

	return proto;
}

static const struct proto_handler no_proto = {
	.name = "none",
	.flags = PROTO_FLAG_IMMEDIATE,
	.attach = default_proto_attach,
};

static const struct proto_handler *
get_proto_handler(const char *name)
{
	struct proto_handler *proto;

	if (!strcmp(name, "none"))
	    return &no_proto;

	if (!handlers.comp)
		return NULL;

	return avl_find_element(&handlers, name, proto, avl);
}

void
proto_dump_handlers(struct blob_buf *b)
{
	struct proto_handler *p;
	void *c;

	avl_for_each_element(&handlers, p, avl) {
		void *v;

		c = blobmsg_open_table(b, p->name);
		if (p->config_params && p->config_params->validate) {
			int i;

			v = blobmsg_open_table(b, "validate");
			for (i = 0; i < p->config_params->n_params; i++)
				blobmsg_add_string(b, p->config_params->params[i].name, uci_get_validate_string(p->config_params, i));
			blobmsg_close_table(b, v);
		}
		blobmsg_add_u8(b, "immediate", !!(p->flags & PROTO_FLAG_IMMEDIATE));
		blobmsg_add_u8(b, "no_device", !!(p->flags & PROTO_FLAG_NODEV));
		blobmsg_add_u8(b, "init_available", !!(p->flags & PROTO_FLAG_INIT_AVAILABLE));
		blobmsg_add_u8(b, "renew_available", !!(p->flags & PROTO_FLAG_RENEW_AVAILABLE));
		blobmsg_add_u8(b, "force_link_default", !!(p->flags & PROTO_FLAG_FORCE_LINK_DEFAULT));
		blobmsg_add_u8(b, "last_error", !!(p->flags & PROTO_FLAG_LASTERROR));
		blobmsg_add_u8(b, "teardown_on_l3_link_down", !!(p->flags & PROTO_FLAG_TEARDOWN_ON_L3_LINK_DOWN));
		blobmsg_add_u8(b, "no_task", !!(p->flags & PROTO_FLAG_NO_TASK));
		blobmsg_close_table(b, c);
	}
}

void
proto_init_interface(struct interface *iface, struct blob_attr *attr)
{
	const struct proto_handler *proto = iface->proto_handler;
	struct interface_proto_state *state = NULL;

	if (!proto)
		proto = &no_proto;

	state = proto->attach(proto, iface, attr);
	if (!state) {
		state = no_proto.attach(&no_proto, iface, attr);
		state->cb = invalid_proto_handler;
	}

	state->handler = proto;
	interface_set_proto_state(iface, state);
}

void
proto_attach_interface(struct interface *iface, const char *proto_name)
{
	const struct proto_handler *proto = &no_proto;
	const char *error = NULL;

	if (proto_name) {
		proto = get_proto_handler(proto_name);
		if (!proto) {
			error = "INVALID_PROTO";
			proto = &no_proto;
		}
	}

	iface->proto_handler = proto;

	if (error)
		interface_add_error(iface, "proto", error, NULL, 0);
}

int
interface_proto_event(struct interface_proto_state *proto,
		      enum interface_proto_cmd cmd, bool force)
{
	enum interface_proto_event ev;
	int ret;

	ret = proto->cb(proto, cmd, force);
	if (ret || !(proto->handler->flags & PROTO_FLAG_IMMEDIATE))
		goto out;

	switch(cmd) {
	case PROTO_CMD_SETUP:
		ev = IFPEV_UP;
		break;
	case PROTO_CMD_TEARDOWN:
		ev = IFPEV_DOWN;
		break;
	case PROTO_CMD_RENEW:
		ev = IFPEV_RENEW;
		break;
	default:
		return -EINVAL;
	}
	proto->proto_event(proto, ev);

out:
	return ret;
}
