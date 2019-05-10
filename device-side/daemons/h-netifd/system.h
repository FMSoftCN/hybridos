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
#ifndef __NETIFD_SYSTEM_H
#define __NETIFD_SYSTEM_H

#include <net/if.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "device.h"
#include "interface-ip.h"
#include "iprule.h"

enum tunnel_param {
	TUNNEL_ATTR_TYPE,
	TUNNEL_ATTR_REMOTE,
	TUNNEL_ATTR_LOCAL,
	TUNNEL_ATTR_MTU,
	TUNNEL_ATTR_DF,
	TUNNEL_ATTR_TTL,
	TUNNEL_ATTR_TOS,
	TUNNEL_ATTR_LINK,
	TUNNEL_ATTR_DATA,
	__TUNNEL_ATTR_MAX
};

extern const struct uci_blob_param_list tunnel_attr_list;

enum vxlan_data {
	VXLAN_DATA_ATTR_ID,
	VXLAN_DATA_ATTR_PORT,
	VXLAN_DATA_ATTR_MACADDR,
	VXLAN_DATA_ATTR_RXCSUM,
	VXLAN_DATA_ATTR_TXCSUM,
	__VXLAN_DATA_ATTR_MAX
};

enum gre_data {
	GRE_DATA_IKEY,
	GRE_DATA_OKEY,
	GRE_DATA_ICSUM,
	GRE_DATA_OCSUM,
	GRE_DATA_ISEQNO,
	GRE_DATA_OSEQNO,
	__GRE_DATA_ATTR_MAX
};

enum vti_data {
	VTI_DATA_IKEY,
	VTI_DATA_OKEY,
	__VTI_DATA_ATTR_MAX
};

enum sixrd_data {
	SIXRD_DATA_PREFIX,
	SIXRD_DATA_RELAY_PREFIX,
	__SIXRD_DATA_ATTR_MAX
};

enum ipip6_data {
	IPIP6_DATA_ENCAPLIMIT,
	IPIP6_DATA_FMRS,
	__IPIP6_DATA_ATTR_MAX
};

enum fmr_data {
	FMR_DATA_PREFIX6,
	FMR_DATA_PREFIX4,
	FMR_DATA_EALEN,
	FMR_DATA_OFFSET,
	__FMR_DATA_ATTR_MAX
};

extern const struct uci_blob_param_list vxlan_data_attr_list;
extern const struct uci_blob_param_list gre_data_attr_list;
extern const struct uci_blob_param_list vti_data_attr_list;
extern const struct uci_blob_param_list sixrd_data_attr_list;
extern const struct uci_blob_param_list ipip6_data_attr_list;
extern const struct uci_blob_param_list fmr_data_attr_list;

enum bridge_opt {
	/* stp and forward delay always set */
	BRIDGE_OPT_AGEING_TIME		    = (1 << 0),
	BRIDGE_OPT_HELLO_TIME		    = (1 << 1),
	BRIDGE_OPT_MAX_AGE		    = (1 << 2),
	BRIDGE_OPT_ROBUSTNESS		    = (1 << 3),
	BRIDGE_OPT_QUERY_INTERVAL	    = (1 << 4),
	BRIDGE_OPT_QUERY_RESPONSE_INTERVAL  = (1 << 5),
	BRIDGE_OPT_LAST_MEMBER_INTERVAL	    = (1 << 6),
};

struct bridge_config {
	enum bridge_opt flags;
	bool stp;

	bool igmp_snoop;
	bool multicast_querier;
	int robustness;
	int query_interval;
	int query_response_interval;
	int last_member_interval;

	unsigned short priority;
	int forward_delay;
	bool bridge_empty;

	int ageing_time;
	int hello_time;
	int max_age;
	int hash_max;
};

enum macvlan_opt {
	MACVLAN_OPT_MACADDR = (1 << 0),
};

struct macvlan_config {
	const char *mode;

	enum macvlan_opt flags;
	unsigned char macaddr[6];
};

enum veth_opt {
	VETH_OPT_MACADDR = (1 << 0),
	VETH_OPT_PEER_NAME = (1 << 1),
	VETH_OPT_PEER_MACADDR = (1 << 2),
};

struct veth_config {
	enum veth_opt flags;

	unsigned char macaddr[6];
	char peer_name[IFNAMSIZ];
	unsigned char peer_macaddr[6];
};

enum vlan_proto {
	VLAN_PROTO_8021Q = 0x8100,
	VLAN_PROTO_8021AD = 0x88A8
};

struct vlandev_config {
	enum vlan_proto proto;
	uint16_t vid;
};

static inline int system_get_addr_family(unsigned int flags)
{
	if ((flags & DEVADDR_FAMILY) == DEVADDR_INET6)
		return AF_INET6;
	else
		return AF_INET;
}

static inline int system_get_addr_len(unsigned int flags)
{
	if ((flags & DEVADDR_FAMILY) != DEVADDR_INET6)
		return sizeof(struct in_addr);
	else
		return sizeof(struct in6_addr);
}

int system_init(void);

int system_bridge_addbr(struct device *bridge, struct bridge_config *cfg);
int system_bridge_delbr(struct device *bridge);
int system_bridge_addif(struct device *bridge, struct device *dev);
int system_bridge_delif(struct device *bridge, struct device *dev);

int system_macvlan_add(struct device *macvlan, struct device *dev, struct macvlan_config *cfg);
int system_macvlan_del(struct device *macvlan);

int system_veth_add(struct device *veth, struct veth_config *cfg);
int system_veth_del(struct device *veth);

int system_vlan_add(struct device *dev, int id);
int system_vlan_del(struct device *dev);

int system_vlandev_add(struct device *vlandev, struct device *dev, struct vlandev_config *cfg);
int system_vlandev_del(struct device *vlandev);

void system_if_get_settings(struct device *dev, struct device_settings *s);
void system_if_clear_state(struct device *dev);
int system_if_up(struct device *dev);
int system_if_down(struct device *dev);
int system_if_check(struct device *dev);
int system_if_resolve(struct device *dev);

int system_if_dump_info(struct device *dev, struct blob_buf *b);
int system_if_dump_stats(struct device *dev, struct blob_buf *b);
struct device *system_if_get_parent(struct device *dev);
bool system_if_force_external(const char *ifname);
void system_if_apply_settings(struct device *dev, struct device_settings *s,
			      unsigned int apply_mask);

int system_add_address(struct device *dev, struct device_addr *addr);
int system_del_address(struct device *dev, struct device_addr *addr);

int system_add_route(struct device *dev, struct device_route *route);
int system_del_route(struct device *dev, struct device_route *route);
int system_flush_routes(void);

bool system_resolve_rt_type(const char *type, unsigned int *id);
bool system_resolve_rt_proto(const char *type, unsigned int *id);
bool system_resolve_rt_table(const char *name, unsigned int *id);
bool system_is_default_rt_table(unsigned int id);
bool system_resolve_rpfilter(const char *filter, unsigned int *id);

int system_del_ip_tunnel(const char *name, struct blob_attr *attr);
int system_add_ip_tunnel(const char *name, struct blob_attr *attr);

int system_add_iprule(struct iprule *rule);
int system_del_iprule(struct iprule *rule);
int system_flush_iprules(void);

bool system_resolve_iprule_action(const char *action, unsigned int *id);

time_t system_get_rtime(void);

void system_fd_set_cloexec(int fd);

int system_update_ipv6_mtu(struct device *dev, int mtu);

#endif
