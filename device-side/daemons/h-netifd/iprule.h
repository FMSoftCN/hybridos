/*
 * netifd - network interface daemon
 * Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2013 Jo-Philipp Wich <jow@openwrt.org>
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
#ifndef __IPRULE_H
#define __IPRULE_H

#include "interface-ip.h"

#define IPRULE_PRIORITY_ADDR		10000
#define IPRULE_PRIORITY_ADDR_MASK	20000
#define IPRULE_PRIORITY_NW		90000
#define IPRULE_PRIORITY_REJECT		4200000000

enum iprule_flags {
	/* address family for rule */
	IPRULE_INET4		= (0 << 0),
	IPRULE_INET6		= (1 << 0),
	IPRULE_FAMILY		= IPRULE_INET4 | IPRULE_INET6,

	/* rule specifies input device */
	IPRULE_IN		= (1 << 2),

	/* rule specifies output device */
	IPRULE_OUT		= (1 << 3),

	/* rule specifies src */
	IPRULE_SRC		= (1 << 4),

	/* rule specifies dest */
	IPRULE_DEST		= (1 << 5),

	/* rule specifies priority */
	IPRULE_PRIORITY		= (1 << 6),

	/* rule specifies diffserv/tos */
	IPRULE_TOS		= (1 << 7),

	/* rule specifies fwmark */
	IPRULE_FWMARK		= (1 << 8),

	/* rule specifies fwmask */
	IPRULE_FWMASK		= (1 << 9),

	/* rule performs table lookup */
	IPRULE_LOOKUP		= (1 << 10),

	/* rule performs routing action */
	IPRULE_ACTION		= (1 << 11),

	/* rule is a goto */
	IPRULE_GOTO		= (1 << 12),

	/* rule suppresses results by prefix length */
	IPRULE_SUP_PREFIXLEN	= (1 << 13),
};

struct iprule {
	struct vlist_node node;
	unsigned int order;

	/* everything below is used as avl tree key */
	enum iprule_flags flags;

	bool invert;

	/* uci interface name */
	char *in_iface;
	char *out_iface;

	/* to receive interface events */
	struct interface_user in_iface_user;
	struct interface_user out_iface_user;

	/* device name */
	char in_dev[IFNAMSIZ + 1];
	char out_dev[IFNAMSIZ + 1];

	unsigned int src_mask;
	union if_addr src_addr;

	unsigned int dest_mask;
	union if_addr dest_addr;

	unsigned int priority;
	unsigned int tos;

	unsigned int fwmark;
	unsigned int fwmask;

	unsigned int lookup;
	unsigned int sup_prefixlen;
	unsigned int action;
	unsigned int gotoid;
};

extern struct vlist_tree iprules;
extern const struct uci_blob_param_list rule_attr_list;

void iprule_add(struct blob_attr *attr, bool v6);
void iprule_update_start(void);
void iprule_update_complete(void);

#endif
