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
#ifndef __NETIFD_UTILS_H
#define __NETIFD_UTILS_H

#include <unistd.h>
#include <stdio.h>
#include <uci_blob.h>
#include <libubox/list.h>
#include <libubox/avl.h>
#include <libubox/avl-cmp.h>
#include <libubox/blobmsg.h>
#include <libubox/vlist.h>
#include <libubox/utils.h>

static inline bool blobmsg_get_bool_default(struct blob_attr *attr, bool val)
{
	if (!attr)
		return val;

	return blobmsg_get_bool(attr);
}

#define __init __attribute__((constructor))

struct vlist_simple_tree {
	struct list_head list;
	int head_offset;
	int version;
};

struct vlist_simple_node {
	struct list_head list;
	int version;
};

#define vlist_for_each_element_safe(tree, element, node_member, ptr) \
        avl_for_each_element_safe(&(tree)->avl, element, node_member.avl, ptr)

#define vlist_simple_init(tree, node, member) \
	__vlist_simple_init(tree, offsetof(node, member))

void __vlist_simple_init(struct vlist_simple_tree *tree, int offset);
void vlist_simple_delete(struct vlist_simple_tree *tree, struct vlist_simple_node *node);
void vlist_simple_flush(struct vlist_simple_tree *tree);
void vlist_simple_flush_all(struct vlist_simple_tree *tree);
void vlist_simple_replace(struct vlist_simple_tree *dest, struct vlist_simple_tree *old);

static inline void vlist_simple_update(struct vlist_simple_tree *tree)
{
	tree->version++;
}

static inline void vlist_simple_add(struct vlist_simple_tree *tree, struct vlist_simple_node *node)
{
	node->version = tree->version;
	list_add_tail(&node->list, &tree->list);
}

#define vlist_simple_for_each_element(tree, element, node_member) \
	list_for_each_entry(element, &(tree)->list, node_member.list)

#define vlist_simple_empty(tree) \
	list_empty(&(tree)->list)


#ifdef __linux__
static inline int fls(int x)
{
    int r = 32;

    if (!x)
        return 0;
    if (!(x & 0xffff0000u)) {
        x <<= 16;
        r -= 16;
    }
    if (!(x & 0xff000000u)) {
        x <<= 8;
        r -= 8;
    }
    if (!(x & 0xf0000000u)) {
        x <<= 4;
        r -= 4;
    }
    if (!(x & 0xc0000000u)) {
        x <<= 2;
        r -= 2;
    }
    if (!(x & 0x80000000u))
        r -= 1;
    return r;
}
#endif

unsigned int parse_netmask_string(const char *str, bool v6);
bool split_netmask(char *str, unsigned int *netmask, bool v6);
int parse_ip_and_netmask(int af, const char *str, void *addr, unsigned int *netmask);
bool check_pid_path(int pid, const char *exe);

char * format_macaddr(uint8_t *mac);

uint32_t crc32_file(FILE *fp);

const char * uci_get_validate_string(const struct uci_blob_param_list *p, int i);

#ifdef __APPLE__
#define s6_addr32	__u6_addr.__u6_addr32
#endif

#endif
