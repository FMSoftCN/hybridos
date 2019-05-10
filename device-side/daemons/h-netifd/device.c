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
#include <stdio.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <net/ethernet.h>

#ifdef linux
#include <netinet/ether.h>
#endif

#include <libubox/list.h>

#include "netifd.h"
#include "system.h"
#include "config.h"

static struct list_head devtypes = LIST_HEAD_INIT(devtypes);
static struct avl_tree devices;

static const struct blobmsg_policy dev_attrs[__DEV_ATTR_MAX] = {
	[DEV_ATTR_TYPE] = { .name = "type", .type = BLOBMSG_TYPE_STRING },
	[DEV_ATTR_MTU] = { .name = "mtu", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_MTU6] = { .name = "mtu6", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_MACADDR] = { .name = "macaddr", .type = BLOBMSG_TYPE_STRING },
	[DEV_ATTR_TXQUEUELEN] = { .name = "txqueuelen", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_ENABLED] = { .name = "enabled", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_IPV6] = { .name = "ipv6", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_PROMISC] = { .name = "promisc", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_RPFILTER] = { .name = "rpfilter", .type = BLOBMSG_TYPE_STRING },
	[DEV_ATTR_ACCEPTLOCAL] = { .name = "acceptlocal", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_IGMPVERSION] = { .name = "igmpversion", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_MLDVERSION] = { .name = "mldversion", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_NEIGHREACHABLETIME] = { .name = "neighreachabletime", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_NEIGHGCSTALETIME] = { .name = "neighgcstaletime", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_DADTRANSMITS] = { .name = "dadtransmits", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_MULTICAST_TO_UNICAST] = { .name = "multicast_to_unicast", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_MULTICAST_ROUTER] = { .name = "multicast_router", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_MULTICAST_FAST_LEAVE] = { .name = "multicast_fast_leave", . type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_MULTICAST] = { .name ="multicast", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_LEARNING] = { .name ="learning", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_UNICAST_FLOOD] = { .name ="unicast_flood", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_SENDREDIRECTS] = { .name = "sendredirects", .type = BLOBMSG_TYPE_BOOL },
	[DEV_ATTR_NEIGHLOCKTIME] = { .name = "neighlocktime", .type = BLOBMSG_TYPE_INT32 },
	[DEV_ATTR_ISOLATE] = { .name = "isolate", .type = BLOBMSG_TYPE_BOOL },
};

const struct uci_blob_param_list device_attr_list = {
	.n_params = __DEV_ATTR_MAX,
	.params = dev_attrs,
};

static int __devlock = 0;

int device_type_add(struct device_type *devtype)
{
	if (device_type_get(devtype->name)) {
		netifd_log_message(L_WARNING, "Device handler '%s' already exists\n",
				   devtype->name);
		return 1;
	}

	netifd_log_message(L_NOTICE, "Added device handler type: %s\n",
		devtype->name);

	list_add(&devtype->list, &devtypes);
	return 0;
}

/* Retrieve the device type for the given name. If 'bridge' is true, the type
 * must have bridge capabilities
 */
struct device_type *
device_type_get(const char *tname)
{
	struct device_type *cur;

	list_for_each_entry(cur, &devtypes, list)
		if (!strcmp(cur->name, tname))
			return cur;

	return NULL;
}

void device_lock(void)
{
	__devlock++;
}

void device_unlock(void)
{
	__devlock--;
	if (!__devlock)
		device_free_unused(NULL);
}

static int set_device_state(struct device *dev, bool state)
{
	if (state) {
		/* Get ifindex for all devices being enabled so a valid  */
		/* ifindex is in place avoiding possible race conditions */
		device_set_ifindex(dev, system_if_resolve(dev));
		if (!dev->ifindex)
			return -1;

		system_if_up(dev);
	}
	else
		system_if_down(dev);

	return 0;
}

static int
simple_device_set_state(struct device *dev, bool state)
{
	struct device *pdev;
	int ret = 0;

	pdev = dev->parent.dev;
	if (state && !pdev) {
		pdev = system_if_get_parent(dev);
		if (pdev)
			device_add_user(&dev->parent, pdev);
	}

	if (pdev) {
		if (state)
			ret = device_claim(&dev->parent);
		else
			device_release(&dev->parent);

		if (ret < 0)
			return ret;
	}
	return set_device_state(dev, state);
}

static struct device *
simple_device_create(const char *name, struct device_type *devtype,
		     struct blob_attr *attr)
{
	struct blob_attr *tb[__DEV_ATTR_MAX];
	struct device *dev = NULL;

	/* device type is unused for simple devices */
	devtype = NULL;

	blobmsg_parse(dev_attrs, __DEV_ATTR_MAX, tb, blob_data(attr), blob_len(attr));
	dev = device_get(name, true);
	if (!dev)
		return NULL;

	dev->set_state = simple_device_set_state;
	device_init_settings(dev, tb);

	return dev;
}

static void simple_device_free(struct device *dev)
{
	if (dev->parent.dev)
		device_remove_user(&dev->parent);
	free(dev);
}

struct device_type simple_device_type = {
	.name = "Network device",
	.config_params = &device_attr_list,

	.create = simple_device_create,
	.check_state = system_if_check,
	.free = simple_device_free,
};

void
device_merge_settings(struct device *dev, struct device_settings *n)
{
	struct device_settings *os = &dev->orig_settings;
	struct device_settings *s = &dev->settings;

	memset(n, 0, sizeof(*n));
	n->mtu = s->flags & DEV_OPT_MTU ? s->mtu : os->mtu;
	n->mtu6 = s->flags & DEV_OPT_MTU6 ? s->mtu6 : os->mtu6;
	n->txqueuelen = s->flags & DEV_OPT_TXQUEUELEN ?
		s->txqueuelen : os->txqueuelen;
	memcpy(n->macaddr,
		(s->flags & DEV_OPT_MACADDR ? s->macaddr : os->macaddr),
		sizeof(n->macaddr));
	n->ipv6 = s->flags & DEV_OPT_IPV6 ? s->ipv6 : os->ipv6;
	n->promisc = s->flags & DEV_OPT_PROMISC ? s->promisc : os->promisc;
	n->rpfilter = s->flags & DEV_OPT_RPFILTER ? s->rpfilter : os->rpfilter;
	n->acceptlocal = s->flags & DEV_OPT_ACCEPTLOCAL ? s->acceptlocal : os->acceptlocal;
	n->igmpversion = s->flags & DEV_OPT_IGMPVERSION ? s->igmpversion : os->igmpversion;
	n->mldversion = s->flags & DEV_OPT_MLDVERSION ? s->mldversion : os->mldversion;
	n->neigh4reachabletime = s->flags & DEV_OPT_NEIGHREACHABLETIME ?
		s->neigh4reachabletime : os->neigh4reachabletime;
	n->neigh6reachabletime = s->flags & DEV_OPT_NEIGHREACHABLETIME ?
		s->neigh6reachabletime : os->neigh6reachabletime;
	n->neigh4gcstaletime = s->flags & DEV_OPT_NEIGHGCSTALETIME ?
		s->neigh4gcstaletime : os->neigh4gcstaletime;
	n->neigh6gcstaletime = s->flags & DEV_OPT_NEIGHGCSTALETIME ?
		s->neigh6gcstaletime : os->neigh6gcstaletime;
	n->neigh4locktime = s->flags & DEV_OPT_NEIGHLOCKTIME ?
		s->neigh4locktime : os->neigh4locktime;
	n->dadtransmits = s->flags & DEV_OPT_DADTRANSMITS ?
		s->dadtransmits : os->dadtransmits;
	n->multicast = s->flags & DEV_OPT_MULTICAST ?
		s->multicast : os->multicast;
	n->multicast_to_unicast = s->multicast_to_unicast;
	n->multicast_router = s->multicast_router;
	n->multicast_fast_leave = s->multicast_fast_leave;
	n->learning = s->learning;
	n->unicast_flood = s->unicast_flood;
	n->sendredirects = s->flags & DEV_OPT_SENDREDIRECTS ?
		s->sendredirects : os->sendredirects;
	n->flags = s->flags | os->flags | os->valid_flags;
}

void
device_init_settings(struct device *dev, struct blob_attr **tb)
{
	struct device_settings *s = &dev->settings;
	struct blob_attr *cur;
	struct ether_addr *ea;
	bool disabled = false;

	s->flags = 0;
	if ((cur = tb[DEV_ATTR_ENABLED]))
		disabled = !blobmsg_get_bool(cur);

	if ((cur = tb[DEV_ATTR_MTU]) && blobmsg_get_u32(cur) >= 68) {
		s->mtu = blobmsg_get_u32(cur);
		s->flags |= DEV_OPT_MTU;
	}

	if ((cur = tb[DEV_ATTR_MTU6]) && blobmsg_get_u32(cur) >= 1280) {
		s->mtu6 = blobmsg_get_u32(cur);
		s->flags |= DEV_OPT_MTU6;
	}

	if ((cur = tb[DEV_ATTR_TXQUEUELEN])) {
		s->txqueuelen = blobmsg_get_u32(cur);
		s->flags |= DEV_OPT_TXQUEUELEN;
	}

	if ((cur = tb[DEV_ATTR_MACADDR])) {
		ea = ether_aton(blobmsg_data(cur));
		if (ea) {
			memcpy(s->macaddr, ea, 6);
			s->flags |= DEV_OPT_MACADDR;
		}
	}

	if ((cur = tb[DEV_ATTR_IPV6])) {
		s->ipv6 = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_IPV6;
	}

	if ((cur = tb[DEV_ATTR_PROMISC])) {
		s->promisc = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_PROMISC;
	}

	if ((cur = tb[DEV_ATTR_RPFILTER])) {
		if (system_resolve_rpfilter(blobmsg_data(cur), &s->rpfilter))
			s->flags |= DEV_OPT_RPFILTER;
		else
			DPRINTF("Failed to resolve rpfilter: %s\n", (char *) blobmsg_data(cur));
	}

	if ((cur = tb[DEV_ATTR_ACCEPTLOCAL])) {
		s->acceptlocal = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_ACCEPTLOCAL;
	}

	if ((cur = tb[DEV_ATTR_IGMPVERSION])) {
		s->igmpversion = blobmsg_get_u32(cur);
		if (s->igmpversion >= 1 && s->igmpversion <= 3)
			s->flags |= DEV_OPT_IGMPVERSION;
		else
			DPRINTF("Failed to resolve igmpversion: %d\n", blobmsg_get_u32(cur));
	}

	if ((cur = tb[DEV_ATTR_MLDVERSION])) {
		s->mldversion = blobmsg_get_u32(cur);
		if (s->mldversion >= 1 && s->mldversion <= 2)
			s->flags |= DEV_OPT_MLDVERSION;
		else
			DPRINTF("Failed to resolve mldversion: %d\n", blobmsg_get_u32(cur));
	}

	if ((cur = tb[DEV_ATTR_NEIGHREACHABLETIME])) {
		s->neigh6reachabletime = s->neigh4reachabletime = blobmsg_get_u32(cur);
		s->flags |= DEV_OPT_NEIGHREACHABLETIME;
	}

	if ((cur = tb[DEV_ATTR_NEIGHGCSTALETIME])) {
		s->neigh6gcstaletime = s->neigh4gcstaletime = blobmsg_get_u32(cur);
		s->flags |= DEV_OPT_NEIGHGCSTALETIME;
	}

	if ((cur = tb[DEV_ATTR_NEIGHLOCKTIME])) {
		s->neigh4locktime = blobmsg_get_u32(cur);
		s->flags |= DEV_OPT_NEIGHLOCKTIME;
	}

	if ((cur = tb[DEV_ATTR_DADTRANSMITS])) {
		s->dadtransmits = blobmsg_get_u32(cur);
		s->flags |= DEV_OPT_DADTRANSMITS;
	}

	if ((cur = tb[DEV_ATTR_MULTICAST_TO_UNICAST])) {
		s->multicast_to_unicast = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_MULTICAST_TO_UNICAST;
	}

	if ((cur = tb[DEV_ATTR_MULTICAST_ROUTER])) {
		s->multicast_router = blobmsg_get_u32(cur);
		if (s->multicast_router <= 2)
			s->flags |= DEV_OPT_MULTICAST_ROUTER;
		else
			DPRINTF("Invalid value: %d - (Use 0: never, 1: learn, 2: always)\n", blobmsg_get_u32(cur));
	}

	if ((cur = tb[DEV_ATTR_MULTICAST_FAST_LEAVE])) {
		s->multicast_fast_leave = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_MULTICAST_FAST_LEAVE;
	}

	if ((cur = tb[DEV_ATTR_MULTICAST])) {
		s->multicast = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_MULTICAST;
	}

	if ((cur = tb[DEV_ATTR_LEARNING])) {
		s->learning = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_LEARNING;
	}

	if ((cur = tb[DEV_ATTR_UNICAST_FLOOD])) {
		s->unicast_flood = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_UNICAST_FLOOD;
	}

	if ((cur = tb[DEV_ATTR_SENDREDIRECTS])) {
		s->sendredirects = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_SENDREDIRECTS;
	}

	if ((cur = tb[DEV_ATTR_ISOLATE])) {
		s->isolate = blobmsg_get_bool(cur);
		s->flags |= DEV_OPT_ISOLATE;
	}

	device_set_disabled(dev, disabled);
}

static void __init dev_init(void)
{
	avl_init(&devices, avl_strcmp, true, NULL);
}

static int device_broadcast_cb(void *ctx, struct safe_list *list)
{
	struct device_user *dep = container_of(list, struct device_user, list);
	int *ev = ctx;

	/* device might have been removed by an earlier callback */
	if (!dep->dev)
		return 0;

	if (dep->cb)
		dep->cb(dep, *ev);
	return 0;
}

void device_broadcast_event(struct device *dev, enum device_event ev)
{
	int dev_ev = ev;

	safe_list_for_each(&dev->aliases, device_broadcast_cb, &dev_ev);
	safe_list_for_each(&dev->users, device_broadcast_cb, &dev_ev);
}

int device_claim(struct device_user *dep)
{
	struct device *dev = dep->dev;
	int ret = 0;

	if (dep->claimed)
		return 0;

	if (!dev)
		return -1;

	dep->claimed = true;
	D(DEVICE, "Claim %s %s, new active count: %d\n", dev->type->name, dev->ifname, dev->active + 1);
	if (++dev->active != 1)
		return 0;

	device_broadcast_event(dev, DEV_EVENT_SETUP);
	if (dev->external) {
		/* Get ifindex for external claimed devices so a valid   */
		/* ifindex is in place avoiding possible race conditions */
		device_set_ifindex(dev, system_if_resolve(dev));
		if (!dev->ifindex)
			ret = -1;

		system_if_get_settings(dev, &dev->orig_settings);
	} else
		ret = dev->set_state(dev, true);

	if (ret == 0)
		device_broadcast_event(dev, DEV_EVENT_UP);
	else {
		D(DEVICE, "claim %s %s failed: %d\n", dev->type->name, dev->ifname, ret);
		dev->active = 0;
		dep->claimed = false;
	}

	return ret;
}

void device_release(struct device_user *dep)
{
	struct device *dev = dep->dev;

	if (!dep->claimed)
		return;

	dep->claimed = false;
	dev->active--;
	D(DEVICE, "Release %s %s, new active count: %d\n", dev->type->name, dev->ifname, dev->active);
	assert(dev->active >= 0);

	if (dev->active)
		return;

	device_broadcast_event(dev, DEV_EVENT_TEARDOWN);
	if (!dev->external)
		dev->set_state(dev, false);

	if (dev->active)
		return;

	device_broadcast_event(dev, DEV_EVENT_DOWN);
}

int device_check_state(struct device *dev)
{
	if (!dev->type->check_state)
		return simple_device_type.check_state(dev);

	return dev->type->check_state(dev);
}

void device_init_virtual(struct device *dev, struct device_type *type, const char *name)
{
	assert(dev);
	assert(type);

	D(DEVICE, "Initialize device '%s'\n", name ? name : "");
	INIT_SAFE_LIST(&dev->users);
	INIT_SAFE_LIST(&dev->aliases);
	dev->type = type;

	if (name)
		device_set_ifname(dev, name);

	if (!dev->set_state)
		dev->set_state = set_device_state;
}

int device_init(struct device *dev, struct device_type *type, const char *ifname)
{
	int ret;

	device_init_virtual(dev, type, ifname);

	dev->avl.key = dev->ifname;

	ret = avl_insert(&devices, &dev->avl);
	if (ret < 0)
		return ret;

	system_if_clear_state(dev);
	device_check_state(dev);

	return 0;
}

static struct device *
device_create_default(const char *name, bool external)
{
	struct device *dev;

	if (!external && system_if_force_external(name))
		return NULL;

	D(DEVICE, "Create simple device '%s'\n", name);
	dev = calloc(1, sizeof(*dev));
	if (!dev)
		return NULL;

	dev->external = external;
	dev->set_state = simple_device_set_state;
	device_init(dev, &simple_device_type, name);
	dev->default_config = true;
	if (external)
		system_if_apply_settings(dev, &dev->settings, dev->settings.flags);
	return dev;
}

struct device *
device_find(const char *name)
{
	struct device *dev;

	return avl_find_element(&devices, name, dev, avl);
}

struct device *
device_get(const char *name, int create)
{
	struct device *dev;

	if (strchr(name, '.'))
		return get_vlan_device_chain(name, create);

	if (name[0] == '@')
		return device_alias_get(name + 1);

	dev = avl_find_element(&devices, name, dev, avl);
	if (dev) {
		if (create > 1 && !dev->external) {
			system_if_apply_settings(dev, &dev->settings, dev->settings.flags);
			dev->external = true;
			device_set_present(dev, true);
		}
		return dev;
	}

	if (!create)
		return NULL;

	return device_create_default(name, create > 1);
}

static void
device_delete(struct device *dev)
{
	if (!dev->avl.key)
		return;

	D(DEVICE, "Delete device '%s' from list\n", dev->ifname);
	avl_delete(&devices, &dev->avl);
	dev->avl.key = NULL;
}

static int device_cleanup_cb(void *ctx, struct safe_list *list)
{
	struct device_user *dep = container_of(list, struct device_user, list);
	if (dep->cb)
		dep->cb(dep, DEV_EVENT_REMOVE);

	device_release(dep);
	return 0;
}

void device_cleanup(struct device *dev)
{
	D(DEVICE, "Clean up device '%s'\n", dev->ifname);
	safe_list_for_each(&dev->users, device_cleanup_cb, NULL);
	safe_list_for_each(&dev->aliases, device_cleanup_cb, NULL);
	device_delete(dev);
}

static void __device_set_present(struct device *dev, bool state)
{
	if (dev->present == state)
		return;

	dev->present = state;
	device_broadcast_event(dev, state ? DEV_EVENT_ADD : DEV_EVENT_REMOVE);
}

void
device_refresh_present(struct device *dev)
{
	bool state = dev->sys_present;

	if (dev->disabled || dev->deferred)
		state = false;

	__device_set_present(dev, state);
}

void device_set_present(struct device *dev, bool state)
{
	if (dev->sys_present == state)
		return;

	D(DEVICE, "%s '%s' %s present\n", dev->type->name, dev->ifname, state ? "is now" : "is no longer" );
	dev->sys_present = state;
	device_refresh_present(dev);
}

void device_set_link(struct device *dev, bool state)
{
	if (dev->link_active == state)
		return;

	netifd_log_message(L_NOTICE, "%s '%s' link is %s\n", dev->type->name, dev->ifname, state ? "up" : "down" );

	dev->link_active = state;
	device_broadcast_event(dev, state ? DEV_EVENT_LINK_UP : DEV_EVENT_LINK_DOWN);
}

void device_set_ifindex(struct device *dev, int ifindex)
{
	if (dev->ifindex == ifindex)
		return;

	dev->ifindex = ifindex;
	device_broadcast_event(dev, DEV_EVENT_UPDATE_IFINDEX);
}

int device_set_ifname(struct device *dev, const char *name)
{
	int ret = 0;

	if (!strcmp(dev->ifname, name))
		return 0;

	if (dev->avl.key)
		avl_delete(&devices, &dev->avl);

	strncpy(dev->ifname, name, IFNAMSIZ);

	if (dev->avl.key)
		ret = avl_insert(&devices, &dev->avl);

	if (ret == 0)
		device_broadcast_event(dev, DEV_EVENT_UPDATE_IFNAME);

	return ret;
}

static int device_refcount(struct device *dev)
{
	struct list_head *list;
	int count = 0;

	list_for_each(list, &dev->users.list)
		count++;

	list_for_each(list, &dev->aliases.list)
		count++;

	return count;
}

static void
__device_add_user(struct device_user *dep, struct device *dev)
{
	struct safe_list *head;

	dep->dev = dev;

	if (dep->alias)
		head = &dev->aliases;
	else
		head = &dev->users;

	safe_list_add(&dep->list, head);
	D(DEVICE, "Add user for device '%s', refcount=%d\n", dev->ifname, device_refcount(dev));

	if (dep->cb && dev->present) {
		dep->cb(dep, DEV_EVENT_ADD);
		if (dev->active)
			dep->cb(dep, DEV_EVENT_UP);

		if (dev->link_active)
			dep->cb(dep, DEV_EVENT_LINK_UP);
	}
}

void device_add_user(struct device_user *dep, struct device *dev)
{
	if (dep->dev == dev)
		return;

	if (dep->dev)
		device_remove_user(dep);

	if (!dev)
		return;

	__device_add_user(dep, dev);
}

void
device_free(struct device *dev)
{
	__devlock++;
	free(dev->config);
	device_cleanup(dev);
	dev->type->free(dev);
	__devlock--;
}

static void
__device_free_unused(struct device *dev)
{
	if (!safe_list_empty(&dev->users) ||
		!safe_list_empty(&dev->aliases) ||
	    dev->current_config || __devlock)
		return;

	device_free(dev);
}

void device_remove_user(struct device_user *dep)
{
	struct device *dev = dep->dev;

	if (!dep->dev)
		return;

	dep->hotplug = false;
	if (dep->claimed)
		device_release(dep);

	safe_list_del(&dep->list);
	dep->dev = NULL;
	D(DEVICE, "Remove user for device '%s', refcount=%d\n", dev->ifname, device_refcount(dev));
	__device_free_unused(dev);
}

void
device_free_unused(struct device *dev)
{
	struct device *tmp;

	if (dev)
		return __device_free_unused(dev);

	avl_for_each_element_safe(&devices, dev, avl, tmp)
		__device_free_unused(dev);
}

void
device_init_pending(void)
{
	struct device *dev, *tmp;

	avl_for_each_element_safe(&devices, dev, avl, tmp) {
		if (!dev->config_pending)
			continue;

		dev->type->config_init(dev);
		dev->config_pending = false;
	}
}

static enum dev_change_type
device_set_config(struct device *dev, struct device_type *type,
		  struct blob_attr *attr)
{
	struct blob_attr *tb[__DEV_ATTR_MAX];
	const struct uci_blob_param_list *cfg = type->config_params;

	if (type != dev->type)
		return DEV_CONFIG_RECREATE;

	if (dev->type->reload)
		return dev->type->reload(dev, attr);

	if (uci_blob_check_equal(dev->config, attr, cfg))
		return DEV_CONFIG_NO_CHANGE;

	if (cfg == &device_attr_list) {
		memset(tb, 0, sizeof(tb));

		if (attr)
			blobmsg_parse(dev_attrs, __DEV_ATTR_MAX, tb,
				blob_data(attr), blob_len(attr));

		device_init_settings(dev, tb);
		return DEV_CONFIG_RESTART;
	} else
		return DEV_CONFIG_RECREATE;
}

enum dev_change_type
device_apply_config(struct device *dev, struct device_type *type,
		    struct blob_attr *config)
{
	enum dev_change_type change;

	change = device_set_config(dev, type, config);
	if (dev->external) {
		system_if_apply_settings(dev, &dev->settings, dev->settings.flags);
		change = DEV_CONFIG_APPLIED;
	}

	switch (change) {
		case DEV_CONFIG_RESTART:
		case DEV_CONFIG_APPLIED:
			D(DEVICE, "Device '%s': config applied\n", dev->ifname);
			config = blob_memdup(config);
			free(dev->config);
			dev->config = config;
			if (change == DEV_CONFIG_RESTART && dev->present) {
				int ret = 0;

				device_set_present(dev, false);
				if (dev->active && !dev->external) {
					ret = dev->set_state(dev, false);
					if (!ret)
						ret = dev->set_state(dev, true);
				}
				if (!ret)
					device_set_present(dev, true);
			}
			break;
		case DEV_CONFIG_NO_CHANGE:
			D(DEVICE, "Device '%s': no configuration change\n", dev->ifname);
			break;
		case DEV_CONFIG_RECREATE:
			break;
	}

	return change;
}

static void
device_replace(struct device *dev, struct device *odev)
{
	struct device_user *dep, *tmp;

	__devlock++;
	if (odev->present)
		device_set_present(odev, false);

	list_for_each_entry_safe(dep, tmp, &odev->users.list, list.list) {
		device_release(dep);
		safe_list_del(&dep->list);
		__device_add_user(dep, dev);
	}
	__devlock--;

	device_free(odev);
}

void
device_reset_config(void)
{
	struct device *dev;

	avl_for_each_element(&devices, dev, avl)
		dev->current_config = false;
}

void
device_reset_old(void)
{
	struct device *dev, *tmp, *ndev;

	avl_for_each_element_safe(&devices, dev, avl, tmp) {
		if (dev->current_config || dev->default_config)
			continue;

		if (dev->type != &simple_device_type)
			continue;

		ndev = device_create_default(dev->ifname, dev->external);
		if (!ndev)
			continue;

		device_replace(ndev, dev);
	}
}

struct device *
device_create(const char *name, struct device_type *type,
	      struct blob_attr *config)
{
	struct device *odev = NULL, *dev;
	enum dev_change_type change;

	odev = device_find(name);
	if (odev) {
		odev->current_config = true;
		change = device_apply_config(odev, type, config);
		switch (change) {
		case DEV_CONFIG_RECREATE:
			D(DEVICE, "Device '%s': recreate device\n", odev->ifname);
			device_delete(odev);
			break;
		default:
			return odev;
		}
	} else
		D(DEVICE, "Create new device '%s' (%s)\n", name, type->name);

	config = blob_memdup(config);
	if (!config)
		return NULL;

	dev = type->create(name, type, config);
	if (!dev)
		return NULL;

	dev->current_config = true;
	dev->config = config;
	if (odev)
		device_replace(dev, odev);

	if (!config_init && dev->config_pending) {
		type->config_init(dev);
		dev->config_pending = false;
	}

	return dev;
}

void
device_dump_status(struct blob_buf *b, struct device *dev)
{
	struct device_settings st;
	void *c, *s;

	if (!dev) {
		avl_for_each_element(&devices, dev, avl) {
			if (!dev->present)
				continue;
			c = blobmsg_open_table(b, dev->ifname);
			device_dump_status(b, dev);
			blobmsg_close_table(b, c);
		}

		return;
	}

	blobmsg_add_u8(b, "external", dev->external);
	blobmsg_add_u8(b, "present", dev->present);
	blobmsg_add_string(b, "type", dev->type->name);

	if (!dev->present)
		return;

	blobmsg_add_u8(b, "up", !!dev->active);
	blobmsg_add_u8(b, "carrier", !!dev->link_active);

	if (dev->type->dump_info)
		dev->type->dump_info(dev, b);
	else
		system_if_dump_info(dev, b);

	if (dev->active) {
		device_merge_settings(dev, &st);
		if (st.flags & DEV_OPT_MTU)
			blobmsg_add_u32(b, "mtu", st.mtu);
		if (st.flags & DEV_OPT_MTU6)
			blobmsg_add_u32(b, "mtu6", st.mtu6);
		if (st.flags & DEV_OPT_MACADDR)
			blobmsg_add_string(b, "macaddr", format_macaddr(st.macaddr));
		if (st.flags & DEV_OPT_TXQUEUELEN)
			blobmsg_add_u32(b, "txqueuelen", st.txqueuelen);
		if (st.flags & DEV_OPT_IPV6)
			blobmsg_add_u8(b, "ipv6", st.ipv6);
		if (st.flags & DEV_OPT_PROMISC)
			blobmsg_add_u8(b, "promisc", st.promisc);
		if (st.flags & DEV_OPT_RPFILTER)
			blobmsg_add_u32(b, "rpfilter", st.rpfilter);
		if (st.flags & DEV_OPT_ACCEPTLOCAL)
			blobmsg_add_u8(b, "acceptlocal", st.acceptlocal);
		if (st.flags & DEV_OPT_IGMPVERSION)
			blobmsg_add_u32(b, "igmpversion", st.igmpversion);
		if (st.flags & DEV_OPT_MLDVERSION)
			blobmsg_add_u32(b, "mldversion", st.mldversion);
		if (st.flags & DEV_OPT_NEIGHREACHABLETIME) {
			blobmsg_add_u32(b, "neigh4reachabletime", st.neigh4reachabletime);
			blobmsg_add_u32(b, "neigh6reachabletime", st.neigh6reachabletime);
		}
		if (st.flags & DEV_OPT_NEIGHGCSTALETIME) {
			blobmsg_add_u32(b, "neigh4gcstaletime", st.neigh4gcstaletime);
			blobmsg_add_u32(b, "neigh6gcstaletime", st.neigh6gcstaletime);
		}
		if (st.flags & DEV_OPT_NEIGHLOCKTIME)
			blobmsg_add_u32(b, "neigh4locktime", st.neigh4locktime);
		if (st.flags & DEV_OPT_DADTRANSMITS)
			blobmsg_add_u32(b, "dadtransmits", st.dadtransmits);
		if (st.flags & DEV_OPT_MULTICAST_TO_UNICAST)
			blobmsg_add_u8(b, "multicast_to_unicast", st.multicast_to_unicast);
		if (st.flags & DEV_OPT_MULTICAST_ROUTER)
			blobmsg_add_u32(b, "multicast_router", st.multicast_router);
		if (st.flags & DEV_OPT_MULTICAST_FAST_LEAVE)
			blobmsg_add_u8(b, "multicast_fast_leave", st.multicast_fast_leave);
		if (st.flags & DEV_OPT_MULTICAST)
			blobmsg_add_u8(b, "multicast", st.multicast);
		if (st.flags & DEV_OPT_LEARNING)
			blobmsg_add_u8(b, "learning", st.learning);
		if (st.flags & DEV_OPT_UNICAST_FLOOD)
			blobmsg_add_u8(b, "unicast_flood", st.unicast_flood);
		if (st.flags & DEV_OPT_SENDREDIRECTS)
			blobmsg_add_u8(b, "sendredirects", st.sendredirects);
	}

	s = blobmsg_open_table(b, "statistics");
	if (dev->type->dump_stats)
		dev->type->dump_stats(dev, b);
	else
		system_if_dump_stats(dev, b);
	blobmsg_close_table(b, s);
}

static void __init simple_device_type_init(void)
{
	device_type_add(&simple_device_type);
}
