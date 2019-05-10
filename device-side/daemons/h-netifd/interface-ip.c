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
#include "device.h"
#include "interface.h"
#include "interface-ip.h"
#include "proto.h"
#include "ubus.h"
#include "system.h"

enum {
	ROUTE_INTERFACE,
	ROUTE_TARGET,
	ROUTE_MASK,
	ROUTE_GATEWAY,
	ROUTE_METRIC,
	ROUTE_MTU,
	ROUTE_VALID,
	ROUTE_TABLE,
	ROUTE_SOURCE,
	ROUTE_ONLINK,
	ROUTE_TYPE,
	ROUTE_PROTO,
	__ROUTE_MAX
};

static const struct blobmsg_policy route_attr[__ROUTE_MAX] = {
	[ROUTE_INTERFACE] = { .name = "interface", .type = BLOBMSG_TYPE_STRING },
	[ROUTE_TARGET] = { .name = "target", .type = BLOBMSG_TYPE_STRING },
	[ROUTE_MASK] = { .name = "netmask", .type = BLOBMSG_TYPE_STRING },
	[ROUTE_GATEWAY] = { .name = "gateway", .type = BLOBMSG_TYPE_STRING },
	[ROUTE_METRIC] = { .name = "metric", .type = BLOBMSG_TYPE_INT32 },
	[ROUTE_MTU] = { .name = "mtu", .type = BLOBMSG_TYPE_INT32 },
	[ROUTE_TABLE] = { .name = "table", .type = BLOBMSG_TYPE_STRING },
	[ROUTE_VALID] = { .name = "valid", .type = BLOBMSG_TYPE_INT32 },
	[ROUTE_SOURCE] = { .name = "source", .type = BLOBMSG_TYPE_STRING },
	[ROUTE_ONLINK] = { .name = "onlink", .type = BLOBMSG_TYPE_BOOL },
	[ROUTE_TYPE] = { .name = "type", .type = BLOBMSG_TYPE_STRING },
	[ROUTE_PROTO] = { .name = "proto", .type = BLOBMSG_TYPE_STRING },
};

const struct uci_blob_param_list route_attr_list = {
	.n_params = __ROUTE_MAX,
	.params = route_attr,
};


struct list_head prefixes = LIST_HEAD_INIT(prefixes);
static struct device_prefix *ula_prefix = NULL;
static struct uloop_timeout valid_until_timeout;


static void
clear_if_addr(union if_addr *a, int mask)
{
	int m_bytes = (mask + 7) / 8;
	uint8_t m_clear = (1 << (m_bytes * 8 - mask)) - 1;
	uint8_t *p = (uint8_t *) a;

	if (m_bytes < sizeof(*a))
		memset(p + m_bytes, 0, sizeof(*a) - m_bytes);

	p[m_bytes - 1] &= ~m_clear;
}

static bool
match_if_addr(union if_addr *a1, union if_addr *a2, int mask)
{
	union if_addr *p1, *p2;

	p1 = alloca(sizeof(*a1));
	p2 = alloca(sizeof(*a2));

	memcpy(p1, a1, sizeof(*a1));
	clear_if_addr(p1, mask);
	memcpy(p2, a2, sizeof(*a2));
	clear_if_addr(p2, mask);

	return !memcmp(p1, p2, sizeof(*p1));
}

static int set_ip_source_policy(bool add, bool v6, unsigned int priority,
		const union if_addr *addr, uint8_t mask, unsigned int table,
		struct interface *in_iface, const char *action, bool src)
{
	struct iprule rule = {
		.flags = IPRULE_PRIORITY,
		.priority = priority
	};

	if (addr) {
		if (src) {
			rule.flags |= IPRULE_SRC;
			rule.src_addr = *addr;
			rule.src_mask = mask;
		} else {
			rule.flags |= IPRULE_DEST;
			rule.dest_addr = *addr;
			rule.dest_mask = mask;
		}
	}

	if (table) {
		rule.flags |= IPRULE_LOOKUP;
		rule.lookup = table;

		if (!rule.lookup)
			return 0;
	} else if (action) {
		rule.flags |= IPRULE_ACTION;
		system_resolve_iprule_action(action, &rule.action);
	}

	if (in_iface && in_iface->l3_dev.dev) {
		rule.flags |= IPRULE_IN;
		strcpy(rule.in_dev, in_iface->l3_dev.dev->ifname);
	}

	rule.flags |= (v6) ? IPRULE_INET6 : IPRULE_INET4;

	return (add) ? system_add_iprule(&rule) : system_del_iprule(&rule);
}

static int set_ip_lo_policy(bool add, bool v6, struct interface *iface)
{
	struct iprule rule = {
		.flags = IPRULE_IN | IPRULE_LOOKUP | IPRULE_PRIORITY,
		.priority = IPRULE_PRIORITY_NW + iface->l3_dev.dev->ifindex,
		.lookup = (v6) ? iface->ip6table : iface->ip4table,
		.in_dev = "lo"
	};

	if (!rule.lookup)
		return 0;

	rule.flags |= (v6) ? IPRULE_INET6 : IPRULE_INET4;

	return (add) ? system_add_iprule(&rule) : system_del_iprule(&rule);
}

static bool
__find_ip_addr_target(struct interface_ip_settings *ip, union if_addr *a, bool v6)
{
	struct device_addr *addr;

	vlist_for_each_element(&ip->addr, addr, node) {
		if (!addr->enabled)
			continue;

		if (v6 != ((addr->flags & DEVADDR_FAMILY) == DEVADDR_INET6))
			continue;

		// Handle offlink addresses correctly
		unsigned int mask = addr->mask;
		if ((addr->flags & DEVADDR_FAMILY) == DEVADDR_INET6 &&
				(addr->flags & DEVADDR_OFFLINK))
			mask = 128;

		if (!match_if_addr(&addr->addr, a, mask))
			continue;

		return true;
	}

	return false;
}

static void
__find_ip_route_target(struct interface_ip_settings *ip, union if_addr *a,
		       bool v6, struct device_route **res)
{
	struct device_route *route;

	vlist_for_each_element(&ip->route, route, node) {
		if (!route->enabled)
			continue;

		if (v6 != ((route->flags & DEVADDR_FAMILY) == DEVADDR_INET6))
			continue;

		if (!match_if_addr(&route->addr, a, route->mask))
			continue;

		if (route->flags & DEVROUTE_TABLE)
			continue;

		if (!*res || route->mask > (*res)->mask)
			*res = route;
	}
}

static bool
interface_ip_find_addr_target(struct interface *iface, union if_addr *a, bool v6)
{
	return __find_ip_addr_target(&iface->proto_ip, a, v6) ||
	       __find_ip_addr_target(&iface->config_ip, a, v6);
}

static void
interface_ip_find_route_target(struct interface *iface, union if_addr *a,
			       bool v6, struct device_route **route)
{
	__find_ip_route_target(&iface->proto_ip, a, v6, route);
	__find_ip_route_target(&iface->config_ip, a, v6, route);
}

struct interface *
interface_ip_add_target_route(union if_addr *addr, bool v6, struct interface *iface)
{
	struct device_route *route, *r_next = NULL;
	bool defaultroute_target = false;
	union if_addr addr_zero;
	int addrsize = v6 ? sizeof(addr->in6) : sizeof(addr->in);

	memset(&addr_zero, 0, sizeof(addr_zero));
	if (memcmp(&addr_zero, addr, addrsize) == 0)
		defaultroute_target = true;

	if (iface) {
		/* look for locally addressable target first */
		if (interface_ip_find_addr_target(iface, addr, v6))
			return iface;

		/* do not stop at the first route, let the lookup compare
		 * masks to find the best match */
		interface_ip_find_route_target(iface, addr, v6, &r_next);
	} else {
		vlist_for_each_element(&interfaces, iface, node) {
			/* look for locally addressable target first */
			if (interface_ip_find_addr_target(iface, addr, v6))
				return iface;

			/* do not stop at the first route, let the lookup compare
			 * masks to find the best match */
			interface_ip_find_route_target(iface, addr, v6, &r_next);
		}
	}

	if (!r_next)
		return NULL;

	iface = r_next->iface;
	if (defaultroute_target)
		return iface;

	route = calloc(1, sizeof(*route));
	if (!route)
		return NULL;

	route->flags = v6 ? DEVADDR_INET6 : DEVADDR_INET4;
	route->mask = v6 ? 128 : 32;
	memcpy(&route->addr, addr, addrsize);
	memcpy(&route->nexthop, &r_next->nexthop, sizeof(route->nexthop));
	route->mtu = r_next->mtu;
	route->metric = r_next->metric;
	route->table = r_next->table;
	route->iface = iface;
	vlist_add(&iface->host_routes, &route->node, route);

	return iface;
}

static void
interface_set_route_info(struct interface *iface, struct device_route *route)
{
	bool v6 = ((route->flags & DEVADDR_FAMILY) == DEVADDR_INET6);

	if (!iface)
		return;

	if (!(route->flags & DEVROUTE_METRIC))
		route->metric = iface->metric;

	if (!(route->flags & DEVROUTE_TABLE)) {
		route->table = (v6) ? iface->ip6table : iface->ip4table;
		if (route->table)
			route->flags |= DEVROUTE_SRCTABLE;
	}
}

void
interface_ip_add_route(struct interface *iface, struct blob_attr *attr, bool v6)
{
	struct interface_ip_settings *ip;
	struct blob_attr *tb[__ROUTE_MAX], *cur;
	struct device_route *route;
	int af = v6 ? AF_INET6 : AF_INET;

	blobmsg_parse(route_attr, __ROUTE_MAX, tb, blobmsg_data(attr), blobmsg_data_len(attr));

	if (!iface) {
		if ((cur = tb[ROUTE_INTERFACE]) == NULL)
			return;

		iface = vlist_find(&interfaces, blobmsg_data(cur), iface, node);
		if (!iface)
			return;

		ip = &iface->config_ip;
	} else {
		ip = &iface->proto_ip;
	}

	route = calloc(1, sizeof(*route));
	if (!route)
		return;

	route->flags = v6 ? DEVADDR_INET6 : DEVADDR_INET4;
	route->mask = v6 ? 128 : 32;
	if ((cur = tb[ROUTE_MASK]) != NULL) {
		route->mask = parse_netmask_string(blobmsg_data(cur), v6);
		if (route->mask > (v6 ? 128 : 32))
			goto error;
	}

	if ((cur = tb[ROUTE_TARGET]) != NULL) {
		if (!parse_ip_and_netmask(af, blobmsg_data(cur), &route->addr, &route->mask)) {
			DPRINTF("Failed to parse route target: %s\n", (char *) blobmsg_data(cur));
			goto error;
		}
	}

	if ((cur = tb[ROUTE_GATEWAY]) != NULL) {
		if (!inet_pton(af, blobmsg_data(cur), &route->nexthop)) {
			DPRINTF("Failed to parse route gateway: %s\n", (char *) blobmsg_data(cur));
			goto error;
		}
	}

	if ((cur = tb[ROUTE_METRIC]) != NULL) {
		route->metric = blobmsg_get_u32(cur);
		route->flags |= DEVROUTE_METRIC;
	}

	if ((cur = tb[ROUTE_MTU]) != NULL) {
		route->mtu = blobmsg_get_u32(cur);
		route->flags |= DEVROUTE_MTU;
	}

	// Use source-based routing
	if ((cur = tb[ROUTE_SOURCE]) != NULL) {
		char *saveptr, *source = alloca(blobmsg_data_len(cur));
		memcpy(source, blobmsg_data(cur), blobmsg_data_len(cur));

		const char *addr = strtok_r(source, "/", &saveptr);
		const char *mask = strtok_r(NULL, "/", &saveptr);

		if (!addr || inet_pton(af, addr, &route->source) < 1) {
			DPRINTF("Failed to parse route source: %s\n", addr ? addr : "NULL");
			goto error;
		}

		route->sourcemask = (mask) ? atoi(mask) : ((af == AF_INET6) ? 128 : 32);
	}

	if ((cur = tb[ROUTE_ONLINK]) != NULL && blobmsg_get_bool(cur))
		route->flags |= DEVROUTE_ONLINK;

	if ((cur = tb[ROUTE_TABLE]) != NULL) {
		if (!system_resolve_rt_table(blobmsg_data(cur), &route->table)) {
			DPRINTF("Failed to resolve routing table: %s\n", (char *) blobmsg_data(cur));
			goto error;
		}

		/* only set the table flag if not using the main (default) table */
		if (system_is_default_rt_table(route->table))
			route->table = 0;

		if (route->table)
			route->flags |= DEVROUTE_TABLE;
	}

	if ((cur = tb[ROUTE_VALID]) != NULL) {
		int64_t valid = blobmsg_get_u32(cur);
		int64_t valid_until = valid + (int64_t)system_get_rtime();
		if (valid_until <= LONG_MAX && valid != 0xffffffffLL) // Catch overflow
			route->valid_until = valid_until;
	}

	if ((cur = tb[ROUTE_TYPE]) != NULL) {
		if (!system_resolve_rt_type(blobmsg_data(cur), &route->type)) {
			DPRINTF("Failed to resolve routing type: %s\n", (char *) blobmsg_data(cur));
			goto error;
		}
		route->flags |= DEVROUTE_TYPE;
	}

	if ((cur = tb[ROUTE_PROTO]) != NULL) {
		if (!system_resolve_rt_proto(blobmsg_data(cur), &route->proto)) {
			DPRINTF("Failed to resolve proto type: %s\n", (char *) blobmsg_data(cur));
			goto error;
		}
		route->flags |= DEVROUTE_PROTO;
	}

	interface_set_route_info(iface, route);
	vlist_add(&ip->route, &route->node, route);
	return;

error:
	free(route);
}

static int
addr_cmp(const void *k1, const void *k2, void *ptr)
{
	return memcmp(k1, k2, sizeof(struct device_addr) -
		      offsetof(struct device_addr, flags));
}

static int
route_cmp(const void *k1, const void *k2, void *ptr)
{
	const struct device_route *r1 = k1, *r2 = k2;

	if (r1->mask != r2->mask)
		return r2->mask - r1->mask;

	if (r1->metric != r2->metric)
		return r1->metric - r2->metric;

	if (r1->flags != r2->flags)
		return r2->flags - r1->flags;

	if (r1->sourcemask != r2->sourcemask)
		return r1->sourcemask - r2->sourcemask;

	if (r1->table != r2->table)
		return r1->table - r2->table;

	int maskcmp = memcmp(&r1->source, &r2->source, sizeof(r1->source));
	if (maskcmp)
		return maskcmp;

	return memcmp(&r1->addr, &r2->addr, sizeof(r1->addr));
}

static int
prefix_cmp(const void *k1, const void *k2, void *ptr)
{
	return memcmp(k1, k2, offsetof(struct device_prefix, pclass) -
			offsetof(struct device_prefix, addr));
}

static void
interface_handle_subnet_route(struct interface *iface, struct device_addr *addr, bool add)
{
	struct device *dev = iface->l3_dev.dev;
	struct device_route *r = &addr->subnet;

	if (addr->flags & DEVADDR_OFFLINK)
		return;

	if (!add) {
		if (!addr->subnet.iface)
			return;

		system_del_route(dev, r);
		memset(r, 0, sizeof(*r));
		return;
	}

	r->iface = iface;
	r->flags = addr->flags;
	r->mask = addr->mask;
	memcpy(&r->addr, &addr->addr, sizeof(r->addr));
	clear_if_addr(&r->addr, r->mask);

	if (!system_resolve_rt_proto("kernel", &r->proto))
		return;

	r->flags |= DEVROUTE_PROTO;
	system_del_route(dev, r);

	r->flags &= ~DEVROUTE_PROTO;
	interface_set_route_info(iface, r);

	system_add_route(dev, r);
}

static void
interface_add_addr_rules(struct device_addr *addr, bool enabled)
{
	bool v6 = (addr->flags & DEVADDR_FAMILY) == DEVADDR_INET6;

	set_ip_source_policy(enabled, v6, IPRULE_PRIORITY_ADDR, &addr->addr,
			     (v6) ? 128 : 32, addr->policy_table, NULL, NULL,
			     true);
	set_ip_source_policy(enabled, v6, IPRULE_PRIORITY_ADDR_MASK,
			     &addr->addr, addr->mask, addr->policy_table, NULL,
			     NULL, false);
}

static void
interface_update_proto_addr(struct vlist_tree *tree,
			    struct vlist_node *node_new,
			    struct vlist_node *node_old)
{
	struct interface_ip_settings *ip;
	struct interface *iface;
	struct device *dev;
	struct device_addr *a_new = NULL, *a_old = NULL;
	bool replace = false;
	bool keep = false;
	bool v6 = false;

	ip = container_of(tree, struct interface_ip_settings, addr);
	iface = ip->iface;
	dev = iface->l3_dev.dev;

	if (!node_new || !node_old)
		iface->updated |= IUF_ADDRESS;

	if (node_new) {
		a_new = container_of(node_new, struct device_addr, node);

		if ((a_new->flags & DEVADDR_FAMILY) == DEVADDR_INET4 &&
		    !a_new->broadcast) {

			/* /31 and /32 addressing need 255.255.255.255
			 * as broadcast address. */
			if (a_new->mask >= 31) {
				a_new->broadcast = (uint32_t) ~0;
			} else {
				uint32_t mask = ~0;
				uint32_t *a = (uint32_t *) &a_new->addr;

				mask >>= a_new->mask;
				a_new->broadcast = *a | htonl(mask);
			}
		}
	}

	if (node_old)
		a_old = container_of(node_old, struct device_addr, node);

	if (a_new && a_old) {
		keep = true;

		if (a_old->flags != a_new->flags || a_old->failed)
			keep = false;

		if (a_old->valid_until != a_new->valid_until ||
				a_old->preferred_until != a_new->preferred_until)
			replace = true;

		if (((a_new->flags & DEVADDR_FAMILY) == DEVADDR_INET4) &&
		     (a_new->broadcast != a_old->broadcast ||
		      a_new->point_to_point != a_old->point_to_point))
			keep = false;
	}

	if (node_old) {
		if (a_old->enabled && !keep) {
			//This is needed for source routing to work correctly. If a device
			//has two connections to a network using the same subnet, adding
			//only the network-rule will cause packets to be routed through the
			//first matching network (source IP matches both masks).
			if (a_old->policy_table)
				interface_add_addr_rules(a_old, false);

			if (!(a_old->flags & DEVADDR_EXTERNAL)) {
				interface_handle_subnet_route(iface, a_old, false);
				system_del_address(dev, a_old);
			}
		}
		free(a_old->pclass);
		free(a_old);
	}

	if (node_new) {
		a_new->enabled = true;

		if ((a_new->flags & DEVADDR_FAMILY) == DEVADDR_INET6)
				v6 = true;

		a_new->policy_table = (v6) ? iface->ip6table : iface->ip4table;

		if (!keep || replace) {
			if (!(a_new->flags & DEVADDR_EXTERNAL)) {
				if (system_add_address(dev, a_new))
					a_new->failed = true;

				if (iface->metric || a_new->policy_table)
					interface_handle_subnet_route(iface, a_new, true);
			}

			if (!keep) {
				if (a_new->policy_table)
					interface_add_addr_rules(a_new, true);
			}
		}
	}
}

static bool
enable_route(struct interface_ip_settings *ip, struct device_route *route)
{
	if (ip->no_defaultroute && !route->mask)
		return false;

	return ip->enabled;
}

static void
interface_update_proto_route(struct vlist_tree *tree,
			     struct vlist_node *node_new,
			     struct vlist_node *node_old)
{
	struct interface_ip_settings *ip;
	struct interface *iface;
	struct device *dev;
	struct device_route *route_old, *route_new;
	bool keep = false;

	ip = container_of(tree, struct interface_ip_settings, route);
	iface = ip->iface;
	dev = iface->l3_dev.dev;

	if (!node_new || !node_old)
		iface->updated |= IUF_ROUTE;

	route_old = container_of(node_old, struct device_route, node);
	route_new = container_of(node_new, struct device_route, node);

	if (node_old && node_new)
		keep = !memcmp(&route_old->nexthop, &route_new->nexthop, sizeof(route_old->nexthop)) &&
			(route_old->mtu == route_new->mtu) && (route_old->type == route_new->type) &&
			(route_old->proto == route_new->proto) && !route_old->failed;

	if (node_old) {
		if (!(route_old->flags & DEVADDR_EXTERNAL) && route_old->enabled && !keep)
			system_del_route(dev, route_old);

		free(route_old);
	}

	if (node_new) {
		bool _enabled = enable_route(ip, route_new);

		if (!(route_new->flags & DEVADDR_EXTERNAL) && !keep && _enabled)
			if (system_add_route(dev, route_new))
				route_new->failed = true;

		route_new->iface = iface;
		route_new->enabled = _enabled;
	}
}

static void
interface_update_host_route(struct vlist_tree *tree,
			     struct vlist_node *node_new,
			     struct vlist_node *node_old)
{
	struct interface *iface;
	struct device *dev;
	struct device_route *route_old, *route_new;

	iface = container_of(tree, struct interface, host_routes);
	dev = iface->l3_dev.dev;

	route_old = container_of(node_old, struct device_route, node);
	route_new = container_of(node_new, struct device_route, node);

	if (node_old) {
		system_del_route(dev, route_old);
		free(route_old);
	}

	if (node_new) {
		if (system_add_route(dev, route_new))
			route_new->failed = true;
	}
}

static void
random_ifaceid(struct in6_addr *addr)
{
	static bool initialized = false;
	struct timeval t;

	if (!initialized) {
		long int seed = 0;
		gettimeofday(&t, NULL);
		seed = t.tv_sec ^ t.tv_usec ^ getpid();
		srand48(seed);
		initialized = true;
	}
	addr->s6_addr32[2] = (uint32_t)mrand48();
	addr->s6_addr32[3] = (uint32_t)mrand48();
}

static bool
eui64_ifaceid(struct interface *iface, struct in6_addr *addr)
{
	struct device_settings st;

	device_merge_settings(iface->l3_dev.dev, &st);

	if (!(st.flags & DEV_OPT_MACADDR))
		return false;

	/* get mac address */
	uint8_t *ifaceid = addr->s6_addr + 8;
	memcpy(ifaceid, st.macaddr, 3);
	memcpy(ifaceid + 5, st.macaddr + 3, 3);
	ifaceid[3] = 0xff;
	ifaceid[4] = 0xfe;
	ifaceid[0] ^= 0x02;

	return true;
}

static bool
generate_ifaceid(struct interface *iface, struct in6_addr *addr)
{
	bool ret = true;

	/* generate new iface id */
	switch (iface->assignment_iface_id_selection) {
	case IFID_FIXED:
		/* fixed */
		/* copy host part from assignment_fixed_iface_id */
		memcpy(addr->s6_addr + 8, iface->assignment_fixed_iface_id.s6_addr + 8, 8);
		break;
	case IFID_RANDOM:
		/* randomize last 64 bits */
		random_ifaceid(addr);
		break;
	case IFID_EUI64:
		/* eui64 */
		ret = eui64_ifaceid(iface, addr);
		break;
	default:
		ret = false;
		break;
	}
	return ret;
}

static void
interface_set_prefix_address(struct device_prefix_assignment *assignment,
		const struct device_prefix *prefix, struct interface *iface, bool add)
{
	const struct interface *uplink = prefix->iface;
	if (!iface->l3_dev.dev)
		return;

	struct device *l3_downlink = iface->l3_dev.dev;

	struct device_addr addr;
	struct device_route route;
	memset(&addr, 0, sizeof(addr));
	memset(&route, 0, sizeof(route));

	addr.addr.in6 = assignment->addr;
	addr.mask = assignment->length;
	addr.flags = DEVADDR_INET6 | DEVADDR_OFFLINK;
	addr.preferred_until = prefix->preferred_until;
	addr.valid_until = prefix->valid_until;

	route.flags = DEVADDR_INET6;
	route.mask = addr.mask < 64 ? 64 : addr.mask;
	route.addr = addr.addr;

	if (!add && assignment->enabled) {
		time_t now = system_get_rtime();

		addr.preferred_until = now;
		if (!addr.valid_until || addr.valid_until - now > 7200)
			addr.valid_until = now + 7200;

		if (iface->ip6table)
			set_ip_source_policy(false, true, IPRULE_PRIORITY_ADDR_MASK, &addr.addr,
					addr.mask < 64 ? 64 : addr.mask, iface->ip6table, NULL, NULL, false);

		if (prefix->iface) {
			if (prefix->iface->ip6table)
				set_ip_source_policy(false, true, IPRULE_PRIORITY_NW, &addr.addr,
						addr.mask, prefix->iface->ip6table, iface, NULL, true);

			set_ip_source_policy(false, true, IPRULE_PRIORITY_REJECT, &addr.addr,
							addr.mask, 0, iface, "unreachable", true);
		}

		clear_if_addr(&route.addr, route.mask);
		interface_set_route_info(iface, &route);

		system_del_route(l3_downlink, &route);
		system_add_address(l3_downlink, &addr);

		assignment->addr = in6addr_any;
		assignment->enabled = false;
	} else if (add && (iface->state == IFS_UP || iface->state == IFS_SETUP)) {
		if (IN6_IS_ADDR_UNSPECIFIED(&addr.addr.in6)) {
			addr.addr.in6 = prefix->addr;
			addr.addr.in6.s6_addr32[1] |= htonl(assignment->assigned);
			if (!generate_ifaceid(iface, &addr.addr.in6))
				return;

			assignment->addr = addr.addr.in6;
			route.addr = addr.addr;
		}

		if (system_add_address(l3_downlink, &addr))
			return;

		if (!assignment->enabled) {
			if (iface->ip6table)
				set_ip_source_policy(true, true, IPRULE_PRIORITY_ADDR_MASK, &addr.addr,
						addr.mask < 64 ? 64 : addr.mask, iface->ip6table, NULL, NULL, false);

			if (prefix->iface) {
				set_ip_source_policy(true, true, IPRULE_PRIORITY_REJECT, &addr.addr,
						addr.mask, 0, iface, "unreachable", true);

				if (prefix->iface->ip6table)
					set_ip_source_policy(true, true, IPRULE_PRIORITY_NW, &addr.addr,
							addr.mask, prefix->iface->ip6table, iface, NULL, true);
			}
		}

		clear_if_addr(&route.addr, route.mask);
		interface_set_route_info(iface, &route);

		system_add_route(l3_downlink, &route);

		if (uplink && uplink->l3_dev.dev && !(l3_downlink->settings.flags & DEV_OPT_MTU6)) {
			int mtu = system_update_ipv6_mtu(uplink->l3_dev.dev, 0);
			int mtu_old = system_update_ipv6_mtu(l3_downlink, 0);

			if (mtu > 0 && mtu_old > mtu)
				system_update_ipv6_mtu(l3_downlink, mtu);
		}

		assignment->enabled = true;
	}
}

static bool interface_prefix_assign(struct list_head *list,
		struct device_prefix_assignment *assign)
{
	int32_t current = 0, asize = (1 << (64 - assign->length)) - 1;
	struct device_prefix_assignment *c;
	list_for_each_entry(c, list, head) {
		if (assign->assigned != -1) {
			if (assign->assigned >= current && assign->assigned + asize < c->assigned) {
				list_add_tail(&assign->head, &c->head);
				return true;
			}
		} else if (assign->assigned == -1) {
			current = (current + asize) & (~asize);
			if (current + asize < c->assigned) {
				assign->assigned = current;
				list_add_tail(&assign->head, &c->head);
				return true;
			}
		}
		current = (c->assigned + (1 << (64 - c->length)));
	}
	return false;
}

/*
 * Sorting of assignment entries:
 * Primary on assignment length: smallest assignment first
 * Secondary on assignment weight: highest weight first
 * Finally alphabetical order of interface names
 */
static int prefix_assignment_cmp(const void *k1, const void *k2, void *ptr)
{
	const struct device_prefix_assignment *a1 = k1, *a2 = k2;

	if (a1->length != a2->length)
		return a1->length - a2->length;

	if (a1->weight != a2->weight)
		return a2->weight - a1->weight;

	return strcmp(a1->name, a2->name);
}

static void interface_update_prefix_assignments(struct device_prefix *prefix, bool setup)
{
	struct device_prefix_assignment *c;
	struct interface *iface;

	// Delete all assignments
	while (!list_empty(&prefix->assignments)) {
		c = list_first_entry(&prefix->assignments,
				struct device_prefix_assignment, head);
		if ((iface = vlist_find(&interfaces, c->name, iface, node)))
			interface_set_prefix_address(c, prefix, iface, false);
		list_del(&c->head);
		free(c);
	}

	if (!setup)
		return;

	// End-of-assignment sentinel
	c = malloc(sizeof(*c) + 1);
	if (!c)
		return;

	c->assigned = 1 << (64 - prefix->length);
	c->length = 64;
	c->name[0] = 0;
	c->addr = in6addr_any;
	list_add(&c->head, &prefix->assignments);

	// Excluded prefix
	if (prefix->excl_length > 0) {
		const char name[] = "!excluded";
		c = malloc(sizeof(*c) + sizeof(name));
		if (c) {
			c->assigned = ntohl(prefix->excl_addr.s6_addr32[1]) &
					((1 << (64 - prefix->length)) - 1);
			c->length = prefix->excl_length;
			c->addr = in6addr_any;
			memcpy(c->name, name, sizeof(name));
			list_add(&c->head, &prefix->assignments);
		}
	}

	bool assigned_any = false;
	struct {
		struct avl_node node;
	} *entry, *n_entry;
	struct avl_tree assign_later;

	avl_init(&assign_later, prefix_assignment_cmp, false, NULL);

	vlist_for_each_element(&interfaces, iface, node) {
		if (iface->assignment_length < 48 ||
				iface->assignment_length > 64)
			continue;

		// Test whether there is a matching class
		if (!list_empty(&iface->assignment_classes)) {
			bool found = false;

			struct interface_assignment_class *c;
			list_for_each_entry(c, &iface->assignment_classes, head) {
				if (!strcmp(c->name, prefix->pclass)) {
					found = true;
					break;
				}
			}

			if (!found)
				continue;
		}

		size_t namelen = strlen(iface->name) + 1;
		c = malloc(sizeof(*c) + namelen);
		if (!c)
			continue;

		c->length = iface->assignment_length;
		c->assigned = iface->assignment_hint;
		c->weight = iface->assignment_weight;
		c->addr = in6addr_any;
		c->enabled = false;
		memcpy(c->name, iface->name, namelen);

		// First process all custom assignments, put all others in later-list
		if (c->assigned == -1 || !interface_prefix_assign(&prefix->assignments, c)) {
			if (c->assigned != -1) {
				c->assigned = -1;
				netifd_log_message(L_WARNING, "Failed to assign requested subprefix "
						"of size %hhu for %s, trying other\n", c->length, c->name);
			}

			entry = calloc(1, sizeof(*entry));
			if (!entry) {
				free(c);
				continue;
			}

			entry->node.key = c;
			avl_insert(&assign_later, &entry->node);
		}

		if (c->assigned != -1)
			assigned_any = true;
	}

	/* Then try to assign all other + failed custom assignments */
	avl_for_each_element_safe(&assign_later, entry, node, n_entry) {
		bool assigned = false;

		c = (struct device_prefix_assignment *)entry->node.key;
		avl_delete(&assign_later, &entry->node);

		do {
			assigned = interface_prefix_assign(&prefix->assignments, c);
		} while (!assigned && ++c->length <= 64);

		if (!assigned) {
			netifd_log_message(L_WARNING, "Failed to assign subprefix "
					"of size %hhu for %s\n", c->length, c->name);
			free(c);
		} else
			assigned_any = true;

		free(entry);
	}

	list_for_each_entry(c, &prefix->assignments, head)
		if ((iface = vlist_find(&interfaces, c->name, iface, node)))
			interface_set_prefix_address(c, prefix, iface, true);

	if (!assigned_any)
		netifd_log_message(L_WARNING, "You have delegated IPv6-prefixes but haven't assigned them "
				"to any interface. Did you forget to set option ip6assign on your lan-interfaces?");
}


void interface_refresh_assignments(bool hint)
{
	static bool refresh = false;
	if (!hint && refresh) {
		struct device_prefix *p;
		list_for_each_entry(p, &prefixes, head)
			interface_update_prefix_assignments(p, true);
	}
	refresh = hint;
}


static void
interface_update_prefix(struct vlist_tree *tree,
			     struct vlist_node *node_new,
			     struct vlist_node *node_old)
{
	struct device_prefix *prefix_old, *prefix_new;
	prefix_old = container_of(node_old, struct device_prefix, node);
	prefix_new = container_of(node_new, struct device_prefix, node);

	struct interface_ip_settings *ip = container_of(tree, struct interface_ip_settings, prefix);
	if (tree && (!node_new || !node_old))
		ip->iface->updated |= IUF_PREFIX;

	struct device_route route;
	memset(&route, 0, sizeof(route));
	route.flags = DEVADDR_INET6;
	route.metric = INT32_MAX;
	route.mask = (node_new) ? prefix_new->length : prefix_old->length;
	route.addr.in6 = (node_new) ? prefix_new->addr : prefix_old->addr;


	struct device_prefix_assignment *c;
	struct interface *iface;

	if (node_old && node_new) {
		// Move assignments and refresh addresses to update valid times
		list_splice(&prefix_old->assignments, &prefix_new->assignments);

		list_for_each_entry(c, &prefix_new->assignments, head)
			if ((iface = vlist_find(&interfaces, c->name, iface, node)))
				interface_set_prefix_address(c, prefix_new, iface, true);

		if (prefix_new->preferred_until != prefix_old->preferred_until ||
				prefix_new->valid_until != prefix_old->valid_until)
			ip->iface->updated |= IUF_PREFIX;
	} else if (node_new) {
		// Set null-route to avoid routing loops
		system_add_route(NULL, &route);

		if (!prefix_new->iface || !prefix_new->iface->proto_ip.no_delegation)
			interface_update_prefix_assignments(prefix_new, true);
	} else if (node_old) {
		// Remove null-route
		interface_update_prefix_assignments(prefix_old, false);
		system_del_route(NULL, &route);
	}

	if (node_old) {
		if (prefix_old->head.next)
			list_del(&prefix_old->head);
		free(prefix_old);
	}

	if (node_new && (!prefix_new->iface || !prefix_new->iface->proto_ip.no_delegation))
		list_add(&prefix_new->head, &prefixes);

}

struct device_prefix*
interface_ip_add_device_prefix(struct interface *iface, struct in6_addr *addr,
		uint8_t length, time_t valid_until, time_t preferred_until,
		struct in6_addr *excl_addr, uint8_t excl_length, const char *pclass)
{
	if (!pclass)
		pclass = (iface) ? iface->name : "local";

	struct device_prefix *prefix = calloc(1, sizeof(*prefix) + strlen(pclass) + 1);
	if (!prefix)
		return NULL;

	prefix->length = length;
	prefix->addr = *addr;
	prefix->preferred_until = preferred_until;
	prefix->valid_until = valid_until;
	prefix->iface = iface;
	INIT_LIST_HEAD(&prefix->assignments);

	if (excl_addr) {
		prefix->excl_addr = *excl_addr;
		prefix->excl_length = excl_length;
	}

	strcpy(prefix->pclass, pclass);

	if (iface)
		vlist_add(&iface->proto_ip.prefix, &prefix->node, &prefix->addr);
	else
		interface_update_prefix(NULL, &prefix->node, NULL);

	return prefix;
}

void
interface_ip_set_ula_prefix(const char *prefix)
{
	char buf[INET6_ADDRSTRLEN + 4] = {0}, *saveptr;
	if (prefix)
		strncpy(buf, prefix, sizeof(buf) - 1);
	char *prefixaddr = strtok_r(buf, "/", &saveptr);

	struct in6_addr addr;
	if (!prefixaddr || inet_pton(AF_INET6, prefixaddr, &addr) < 1) {
		if (ula_prefix) {
			interface_update_prefix(NULL, NULL, &ula_prefix->node);
			ula_prefix = NULL;
		}
		return;
	}

	int length;
	char *prefixlen = strtok_r(NULL, ",", &saveptr);
	if (!prefixlen || (length = atoi(prefixlen)) < 1 || length > 64)
		return;

	if (!ula_prefix || !IN6_ARE_ADDR_EQUAL(&addr, &ula_prefix->addr) ||
			ula_prefix->length != length) {
		if (ula_prefix)
			interface_update_prefix(NULL, NULL, &ula_prefix->node);

		ula_prefix = interface_ip_add_device_prefix(NULL, &addr, length,
				0, 0, NULL, 0, NULL);
	}
}

void
interface_add_dns_server(struct interface_ip_settings *ip, const char *str)
{
	struct dns_server *s;

	s = calloc(1, sizeof(*s));
	if (!s)
		return;

	s->af = AF_INET;
	if (inet_pton(s->af, str, &s->addr.in))
		goto add;

	s->af = AF_INET6;
	if (inet_pton(s->af, str, &s->addr.in))
		goto add;

	free(s);
	return;

add:
	D(INTERFACE, "Add IPv%c DNS server: %s\n",
	  s->af == AF_INET6 ? '6' : '4', str);
	vlist_simple_add(&ip->dns_servers, &s->node);
}

void
interface_add_dns_server_list(struct interface_ip_settings *ip, struct blob_attr *list)
{
	struct blob_attr *cur;
	int rem;

	blobmsg_for_each_attr(cur, list, rem) {
		if (blobmsg_type(cur) != BLOBMSG_TYPE_STRING)
			continue;

		if (!blobmsg_check_attr(cur, NULL))
			continue;

		interface_add_dns_server(ip, blobmsg_data(cur));
	}
}

static void
interface_add_dns_search_domain(struct interface_ip_settings *ip, const char *str)
{
	struct dns_search_domain *s;
	int len = strlen(str);

	s = calloc(1, sizeof(*s) + len + 1);
	if (!s)
		return;

	D(INTERFACE, "Add DNS search domain: %s\n", str);
	memcpy(s->name, str, len);
	vlist_simple_add(&ip->dns_search, &s->node);
}

void
interface_add_dns_search_list(struct interface_ip_settings *ip, struct blob_attr *list)
{
	struct blob_attr *cur;
	int rem;

	blobmsg_for_each_attr(cur, list, rem) {
		if (blobmsg_type(cur) != BLOBMSG_TYPE_STRING)
			continue;

		if (!blobmsg_check_attr(cur, NULL))
			continue;

		interface_add_dns_search_domain(ip, blobmsg_data(cur));
	}
}

static void
write_resolv_conf_entries(FILE *f, struct interface_ip_settings *ip, const char *dev)
{
	struct dns_server *s;
	struct dns_search_domain *d;
	const char *str;
	char buf[INET6_ADDRSTRLEN];

	vlist_simple_for_each_element(&ip->dns_servers, s, node) {
		str = inet_ntop(s->af, &s->addr, buf, sizeof(buf));
		if (!str)
			continue;

		if (s->af == AF_INET6 && IN6_IS_ADDR_LINKLOCAL(&s->addr.in6))
			fprintf(f, "nameserver %s%%%s\n", str, dev);
		else
			fprintf(f, "nameserver %s\n", str);
	}

	vlist_simple_for_each_element(&ip->dns_search, d, node) {
		fprintf(f, "search %s\n", d->name);
	}
}

/* Sorting of interface resolver entries :               */
/* Primary on interface dns_metric : lowest metric first */
/* Secondary on interface metric : lowest metric first   */
/* Finally alphabetical order of interface names         */
static int resolv_conf_iface_cmp(const void *k1, const void *k2, void *ptr)
{
	const struct interface *iface1 = k1, *iface2 = k2;

	if (iface1->dns_metric != iface2->dns_metric)
		return iface1->dns_metric - iface2->dns_metric;

	if (iface1->metric != iface2->metric)
		return iface1->metric - iface2->metric;

	return strcmp(iface1->name, iface2->name);
}

static void
__interface_write_dns_entries(FILE *f)
{
	struct interface *iface;
	struct {
		struct avl_node node;
	} *entry, *n_entry;
	struct avl_tree resolv_conf_iface_entries;

	avl_init(&resolv_conf_iface_entries, resolv_conf_iface_cmp, false, NULL);

	vlist_for_each_element(&interfaces, iface, node) {
		if (iface->state != IFS_UP)
			continue;

		if (vlist_simple_empty(&iface->proto_ip.dns_search) &&
		    vlist_simple_empty(&iface->proto_ip.dns_servers) &&
		    vlist_simple_empty(&iface->config_ip.dns_search) &&
		    vlist_simple_empty(&iface->config_ip.dns_servers))
			continue;

		entry = calloc(1, sizeof(*entry));
		if (!entry)
			continue;

		entry->node.key = iface;
		avl_insert(&resolv_conf_iface_entries, &entry->node);
	}

	avl_for_each_element(&resolv_conf_iface_entries, entry, node) {
		iface = (struct interface *)entry->node.key;
		struct device *dev = iface->l3_dev.dev;

		fprintf(f, "# Interface %s\n", iface->name);

		write_resolv_conf_entries(f, &iface->config_ip, dev->ifname);

		if (!iface->proto_ip.no_dns)
			write_resolv_conf_entries(f, &iface->proto_ip, dev->ifname);
	}

	avl_remove_all_elements(&resolv_conf_iface_entries, entry, node, n_entry)
		free(entry);
}

void
interface_write_resolv_conf(void)
{
	char *path = alloca(strlen(resolv_conf) + 5);
	FILE *f;
	uint32_t crcold, crcnew;

	sprintf(path, "%s.tmp", resolv_conf);
	unlink(path);
	f = fopen(path, "w+");
	if (!f) {
		D(INTERFACE, "Failed to open %s for writing\n", path);
		return;
	}

	__interface_write_dns_entries(f);

	fflush(f);
	rewind(f);
	crcnew = crc32_file(f);
	fclose(f);

	crcold = crcnew + 1;
	f = fopen(resolv_conf, "r");
	if (f) {
		crcold = crc32_file(f);
		fclose(f);
	}

	if (crcold == crcnew) {
		unlink(path);
	} else if (rename(path, resolv_conf) < 0) {
		D(INTERFACE, "Failed to replace %s\n", resolv_conf);
		unlink(path);
	}
}

void interface_ip_set_enabled(struct interface_ip_settings *ip, bool enabled)
{
	struct device_addr *addr;
	struct device_route *route;
	struct device *dev;
	struct interface *iface;

	ip->enabled = enabled;
	iface = ip->iface;
	dev = iface->l3_dev.dev;
	if (!dev)
		return;

	vlist_for_each_element(&ip->addr, addr, node) {
		bool v6 = ((addr->flags & DEVADDR_FAMILY) == DEVADDR_INET6) ? true : false;

		if (addr->flags & DEVADDR_EXTERNAL)
			continue;

		if (addr->enabled == enabled)
			continue;

		if (enabled) {
			system_add_address(dev, addr);

			addr->policy_table = (v6) ? iface->ip6table : iface->ip4table;
			if (iface->metric || addr->policy_table)
				interface_handle_subnet_route(iface, addr, true);

			if (addr->policy_table)
				interface_add_addr_rules(addr, true);
		} else {
			interface_handle_subnet_route(iface, addr, false);
			system_del_address(dev, addr);

			if (addr->policy_table)
				interface_add_addr_rules(addr, false);
		}
		addr->enabled = enabled;
	}

	vlist_for_each_element(&ip->route, route, node) {
		bool _enabled = enabled;

		if (route->flags & DEVADDR_EXTERNAL)
			continue;

		if (!enable_route(ip, route))
			_enabled = false;

		if (route->enabled == _enabled)
			continue;

		if (_enabled) {
			interface_set_route_info(ip->iface, route);

			if (system_add_route(dev, route))
				route->failed = true;
		} else
			system_del_route(dev, route);
		route->enabled = _enabled;
	}

	struct device_prefix *c;
	struct device_prefix_assignment *a;
	list_for_each_entry(c, &prefixes, head)
		list_for_each_entry(a, &c->assignments, head)
			if (!strcmp(a->name, ip->iface->name))
				interface_set_prefix_address(a, c, ip->iface, enabled);

	if (ip->iface->policy_rules_set != enabled &&
	    ip->iface->l3_dev.dev) {
		set_ip_lo_policy(enabled, true, ip->iface);
		set_ip_lo_policy(enabled, false, ip->iface);

		set_ip_source_policy(enabled, true, IPRULE_PRIORITY_REJECT + ip->iface->l3_dev.dev->ifindex,
			NULL, 0, 0, ip->iface, "failed_policy", true);
		ip->iface->policy_rules_set = enabled;
	}
}

void
interface_ip_update_start(struct interface_ip_settings *ip)
{
	if (ip != &ip->iface->config_ip) {
		vlist_simple_update(&ip->dns_servers);
		vlist_simple_update(&ip->dns_search);
	}
	vlist_update(&ip->route);
	vlist_update(&ip->addr);
	vlist_update(&ip->prefix);
}

void
interface_ip_update_complete(struct interface_ip_settings *ip)
{
	vlist_simple_flush(&ip->dns_servers);
	vlist_simple_flush(&ip->dns_search);
	vlist_flush(&ip->route);
	vlist_flush(&ip->addr);
	vlist_flush(&ip->prefix);
	interface_write_resolv_conf();
}

void
interface_ip_flush(struct interface_ip_settings *ip)
{
	if (ip == &ip->iface->proto_ip)
		vlist_flush_all(&ip->iface->host_routes);
	vlist_simple_flush_all(&ip->dns_servers);
	vlist_simple_flush_all(&ip->dns_search);
	vlist_flush_all(&ip->route);
	vlist_flush_all(&ip->addr);
	vlist_flush_all(&ip->prefix);
}

static void
__interface_ip_init(struct interface_ip_settings *ip, struct interface *iface)
{
	ip->iface = iface;
	ip->enabled = true;
	vlist_simple_init(&ip->dns_search, struct dns_search_domain, node);
	vlist_simple_init(&ip->dns_servers, struct dns_server, node);
	vlist_init(&ip->route, route_cmp, interface_update_proto_route);
	vlist_init(&ip->addr, addr_cmp, interface_update_proto_addr);
	vlist_init(&ip->prefix, prefix_cmp, interface_update_prefix);
}

void
interface_ip_init(struct interface *iface)
{
	__interface_ip_init(&iface->proto_ip, iface);
	__interface_ip_init(&iface->config_ip, iface);
	vlist_init(&iface->host_routes, route_cmp, interface_update_host_route);
}

static void
interface_ip_valid_until_handler(struct uloop_timeout *t)
{
	time_t now = system_get_rtime();
	struct interface *iface;
	vlist_for_each_element(&interfaces, iface, node) {
		if (iface->state != IFS_UP)
			continue;

		struct device_addr *addr, *addrp;
		struct device_route *route, *routep;
		struct device_prefix *pref, *prefp;

		vlist_for_each_element_safe(&iface->proto_ip.addr, addr, node, addrp)
			if (addr->valid_until && addr->valid_until < now)
				vlist_delete(&iface->proto_ip.addr, &addr->node);

		vlist_for_each_element_safe(&iface->proto_ip.route, route, node, routep)
			if (route->valid_until && route->valid_until < now)
				vlist_delete(&iface->proto_ip.route, &route->node);

		vlist_for_each_element_safe(&iface->proto_ip.prefix, pref, node, prefp)
			if (pref->valid_until && pref->valid_until < now)
				vlist_delete(&iface->proto_ip.prefix, &pref->node);

	}

	uloop_timeout_set(t, 1000);
}

static void __init
interface_ip_init_worker(void)
{
	valid_until_timeout.cb = interface_ip_valid_until_handler;
	uloop_timeout_set(&valid_until_timeout, 1000);
}
