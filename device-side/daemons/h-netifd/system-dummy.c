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
#include <sys/time.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#ifndef DEBUG
#define DEBUG
#endif

#include "netifd.h"
#include "device.h"
#include "system.h"

int system_init(void)
{
	return 0;
}

int system_bridge_addbr(struct device *bridge, struct bridge_config *cfg)
{
	D(SYSTEM, "brctl addbr %s\n", bridge->ifname);
	return 0;
}

int system_bridge_delbr(struct device *bridge)
{
	D(SYSTEM, "brctl delbr %s\n", bridge->ifname);
	return 0;
}

int system_bridge_addif(struct device *bridge, struct device *dev)
{
	D(SYSTEM, "brctl addif %s %s\n", bridge->ifname, dev->ifname);
	return 0;
}

int system_bridge_delif(struct device *bridge, struct device *dev)
{
	D(SYSTEM, "brctl delif %s %s\n", bridge->ifname, dev->ifname);
	return 0;
}

int system_vlan_add(struct device *dev, int id)
{
	D(SYSTEM, "vconfig add %s %d\n", dev->ifname, id);
	return 0;
}

int system_vlan_del(struct device *dev)
{
	D(SYSTEM, "vconfig rem %s\n", dev->ifname);
	return 0;
}

bool system_if_force_external(const char *ifname)
{
	return false;
}

int system_if_up(struct device *dev)
{
	D(SYSTEM, "ifconfig %s up\n", dev->ifname);
	return 0;
}

int system_if_down(struct device *dev)
{
	D(SYSTEM, "ifconfig %s down\n", dev->ifname);
	return 0;
}

void system_if_get_settings(struct device *dev, struct device_settings *s)
{
}

void system_if_clear_state(struct device *dev)
{
}

int system_if_check(struct device *dev)
{
	dev->ifindex = 0;
	device_set_present(dev, true);
	device_set_link(dev, true);

	return 0;
}

int system_if_resolve(struct device *dev)
{
	return 0;
}

struct device *
system_if_get_parent(struct device *dev)
{
	return NULL;
}

int
system_if_dump_info(struct device *dev, struct blob_buf *b)
{
	blobmsg_add_u8(b, "link", dev->present);
	return 0;
}

int
system_if_dump_stats(struct device *dev, struct blob_buf *b)
{
	return 0;
}

void
system_if_apply_settings(struct device *dev, struct device_settings *s, unsigned int apply_mask)
{
}

static int system_address_msg(struct device *dev, struct device_addr *addr, const char *type)
{
	char ipaddr[64];
	int af = system_get_addr_family(addr->flags);

	D(SYSTEM, "ifconfig %s %s %s/%u\n",
		dev->ifname, type, inet_ntop(af, &addr->addr.in, ipaddr, sizeof(ipaddr)),
		addr->mask);

	return 0;
}

int system_add_address(struct device *dev, struct device_addr *addr)
{
	return system_address_msg(dev, addr, "add");
}

int system_del_address(struct device *dev, struct device_addr *addr)
{
	return system_address_msg(dev, addr, "del");
}

static int system_route_msg(struct device *dev, struct device_route *route, const char *type)
{
	char addr[64], gw[64] = " gw ", devstr[64] = "";
	int af = system_get_addr_family(route->flags);
	int alen = system_get_addr_len(route->flags);
	static uint32_t zero_addr[4];

	if ((route->flags & DEVADDR_FAMILY) != DEVADDR_INET4)
		return -1;

	if (!route->mask)
		sprintf(addr, "default");
	else
		inet_ntop(af, &route->addr.in, addr, sizeof(addr));

	if (memcmp(&route->nexthop.in, (void *) zero_addr, alen) != 0)
		inet_ntop(af, &route->nexthop.in, gw + 4, sizeof(gw) - 4);
	else
		gw[0] = 0;

	if (dev)
		sprintf(devstr, " dev %s", dev->ifname);

	if (route->metric > 0)
		sprintf(devstr, " metric %d", route->metric);

	D(SYSTEM, "route %s %s%s%s\n", type, addr, gw, devstr);
	return 0;
}

int system_add_route(struct device *dev, struct device_route *route)
{
	return system_route_msg(dev, route, "add");
}

int system_del_route(struct device *dev, struct device_route *route)
{
	return system_route_msg(dev, route, "del");
}

int system_flush_routes(void)
{
	return 0;
}

bool system_resolve_rt_type(const char *type, unsigned int *id)
{
	*id = 0;
	return true;
}

bool system_resolve_rt_proto(const char *type, unsigned int *id)
{
	*id = 0;
	return true;
}

bool system_resolve_rt_table(const char *name, unsigned int *id)
{
	*id = 0;
	return true;
}

bool system_is_default_rt_table(unsigned int id)
{
	return true;
}

bool system_resolve_rpfilter(const char *filter, unsigned int *id)
{
	*id = 0;
	return true;
}

int system_add_iprule(struct iprule *rule)
{
	return 0;
}

int system_del_iprule(struct iprule *rule)
{
	return 0;
}

int system_flush_iprules(void)
{
	return 0;
}

bool system_resolve_iprule_action(const char *action, unsigned int *id)
{
	*id = 0;
	return true;
}

time_t system_get_rtime(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) == 0)
		return tv.tv_sec;

	return 0;
}

int system_del_ip_tunnel(const char *name, struct blob_attr *attr)
{
	return 0;
}

int system_add_ip_tunnel(const char *name, struct blob_attr *attr)
{
	return 0;
}

int system_update_ipv6_mtu(struct device *dev, int mtu)
{
	return 0;
}

int system_macvlan_add(struct device *macvlan, struct device *dev, struct macvlan_config *cfg)
{
	return 0;
}

int system_macvlan_del(struct device *macvlan)
{
	return 0;
}

int system_veth_add(struct device *veth, struct veth_config *cfg)
{
	return 0;
}

int system_veth_del(struct device *veth)
{
	return 0;
}

int system_vlandev_add(struct device *vlandev, struct device *dev, struct vlandev_config *cfg)
{
	return 0;
}

int system_vlandev_del(struct device *vlandev)
{
	return 0;
}
