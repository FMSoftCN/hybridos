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
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#ifdef __APPLE__
#include <libproc.h>
#endif

void
__vlist_simple_init(struct vlist_simple_tree *tree, int offset)
{
	INIT_LIST_HEAD(&tree->list);
	tree->version = 1;
	tree->head_offset = offset;
}

void
vlist_simple_delete(struct vlist_simple_tree *tree, struct vlist_simple_node *node)
{
	char *ptr;

	list_del(&node->list);
	ptr = (char *) node - tree->head_offset;
	free(ptr);
}

void
vlist_simple_flush(struct vlist_simple_tree *tree)
{
	struct vlist_simple_node *n, *tmp;

	list_for_each_entry_safe(n, tmp, &tree->list, list) {
		if ((n->version == tree->version || n->version == -1) &&
		    tree->version != -1)
			continue;

		vlist_simple_delete(tree, n);
	}
}

void
vlist_simple_replace(struct vlist_simple_tree *dest, struct vlist_simple_tree *old)
{
	struct vlist_simple_node *n, *tmp;

	vlist_simple_update(dest);
	list_for_each_entry_safe(n, tmp, &old->list, list) {
		list_del(&n->list);
		vlist_simple_add(dest, n);
	}
	vlist_simple_flush(dest);
}

void
vlist_simple_flush_all(struct vlist_simple_tree *tree)
{
	tree->version = -1;
	vlist_simple_flush(tree);
}

unsigned int
parse_netmask_string(const char *str, bool v6)
{
	struct in_addr addr;
	unsigned int ret;
	char *err = NULL;

	if (!strchr(str, '.')) {
		ret = strtoul(str, &err, 0);
		if (err && *err)
			goto error;

		return ret;
	}

	if (v6)
		goto error;

	if (inet_aton(str, &addr) != 1)
		goto error;

	return 32 - fls(~(ntohl(addr.s_addr)));

error:
	return ~0;
}

bool
split_netmask(char *str, unsigned int *netmask, bool v6)
{
	char *delim = strchr(str, '/');

	if (delim) {
		*(delim++) = 0;

		*netmask = parse_netmask_string(delim, v6);
	}
	return true;
}

int
parse_ip_and_netmask(int af, const char *str, void *addr, unsigned int *netmask)
{
	char *astr = alloca(strlen(str) + 1);

	strcpy(astr, str);
	if (!split_netmask(astr, netmask, af == AF_INET6))
		return 0;

	if (af == AF_INET6) {
		if (*netmask > 128)
			return 0;
	} else {
		if (*netmask > 32)
			return 0;
	}

	return inet_pton(af, astr, addr);
}

char *
format_macaddr(uint8_t *mac)
{
	static char str[sizeof("ff:ff:ff:ff:ff:ff ")];

	snprintf(str, sizeof(str), "%02x:%02x:%02x:%02x:%02x:%02x",
		 mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	return str;
}

uint32_t
crc32_file(FILE *fp)
{
	static uint32_t *crcvals = NULL;
	if (!crcvals) {
		crcvals = malloc(sizeof(*crcvals) * 256);

		for (size_t i = 0; i < 256; ++i) {
			uint32_t c = i;
			for (size_t j = 0; j < 8; ++j)
				c = (c & 1) ? (0xEDB88320 ^ (c >> 1)) : (c >> 1);
			crcvals[i] = c;
		}
	}

	uint8_t buf[1024];
	size_t len;
	uint32_t c = 0xFFFFFFFF;

	do {
		len = fread(buf, 1, sizeof(buf), fp);
		for (size_t i = 0; i < len; ++i)
			c = crcvals[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
	} while (len == sizeof(buf));

	return c ^ 0xFFFFFFFF;
}

bool check_pid_path(int pid, const char *exe)
{
	int proc_exe_len;
	int exe_len = strlen(exe);

#ifdef __APPLE__
	char proc_exe_buf[PROC_PIDPATHINFO_SIZE];

	proc_exe_len = proc_pidpath(pid, proc_exe_buf, sizeof(proc_exe_buf));
#else
	char proc_exe[32];
	char *proc_exe_buf = alloca(exe_len);

	sprintf(proc_exe, "/proc/%d/exe", pid);
	proc_exe_len = readlink(proc_exe, proc_exe_buf, exe_len);
#endif

	if (proc_exe_len != exe_len)
		return false;

	return !memcmp(exe, proc_exe_buf, exe_len);
}

static const char * const uci_validate_name[__BLOBMSG_TYPE_LAST] = {
	[BLOBMSG_TYPE_STRING] = "string",
	[BLOBMSG_TYPE_ARRAY] = "list(string)",
	[BLOBMSG_TYPE_INT32] = "uinteger",
	[BLOBMSG_TYPE_BOOL] = "bool",
};

const char*
uci_get_validate_string(const struct uci_blob_param_list *p, int i)
{
	if (p->validate[i])
		return p->validate[i];

	else if (uci_validate_name[p->params[i].type])
		return uci_validate_name[p->params[i].type];

	return p->validate[BLOBMSG_TYPE_STRING];
}
