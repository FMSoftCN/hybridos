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
#include <errno.h>

#include "netifd.h"
#include "device.h"
#include "interface.h"
#include "system.h"

enum {
	BRIDGE_ATTR_IFNAME,
	BRIDGE_ATTR_STP,
	BRIDGE_ATTR_FORWARD_DELAY,
	BRIDGE_ATTR_PRIORITY,
	BRIDGE_ATTR_IGMP_SNOOP,
	BRIDGE_ATTR_AGEING_TIME,
	BRIDGE_ATTR_HELLO_TIME,
	BRIDGE_ATTR_MAX_AGE,
	BRIDGE_ATTR_BRIDGE_EMPTY,
	BRIDGE_ATTR_MULTICAST_QUERIER,
	BRIDGE_ATTR_HASH_MAX,
	BRIDGE_ATTR_ROBUSTNESS,
	BRIDGE_ATTR_QUERY_INTERVAL,
	BRIDGE_ATTR_QUERY_RESPONSE_INTERVAL,
	BRIDGE_ATTR_LAST_MEMBER_INTERVAL,
	__BRIDGE_ATTR_MAX
};

static const struct blobmsg_policy bridge_attrs[__BRIDGE_ATTR_MAX] = {
	[BRIDGE_ATTR_IFNAME] = { "ifname", BLOBMSG_TYPE_ARRAY },
	[BRIDGE_ATTR_STP] = { "stp", BLOBMSG_TYPE_BOOL },
	[BRIDGE_ATTR_FORWARD_DELAY] = { "forward_delay", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_PRIORITY] = { "priority", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_AGEING_TIME] = { "ageing_time", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_HELLO_TIME] = { "hello_time", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_MAX_AGE] = { "max_age", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_IGMP_SNOOP] = { "igmp_snooping", BLOBMSG_TYPE_BOOL },
	[BRIDGE_ATTR_BRIDGE_EMPTY] = { "bridge_empty", BLOBMSG_TYPE_BOOL },
	[BRIDGE_ATTR_MULTICAST_QUERIER] = { "multicast_querier", BLOBMSG_TYPE_BOOL },
	[BRIDGE_ATTR_HASH_MAX] = { "hash_max", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_ROBUSTNESS] = { "robustness", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_QUERY_INTERVAL] = { "query_interval", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_QUERY_RESPONSE_INTERVAL] = { "query_response_interval", BLOBMSG_TYPE_INT32 },
	[BRIDGE_ATTR_LAST_MEMBER_INTERVAL] = { "last_member_interval", BLOBMSG_TYPE_INT32 },
};

static const struct uci_blob_param_info bridge_attr_info[__BRIDGE_ATTR_MAX] = {
	[BRIDGE_ATTR_IFNAME] = { .type = BLOBMSG_TYPE_STRING },
};

static const struct uci_blob_param_list bridge_attr_list = {
	.n_params = __BRIDGE_ATTR_MAX,
	.params = bridge_attrs,
	.info = bridge_attr_info,

	.n_next = 1,
	.next = { &device_attr_list },
};

static struct device *bridge_create(const char *name, struct device_type *devtype,
	struct blob_attr *attr);
static void bridge_config_init(struct device *dev);
static void bridge_free(struct device *dev);
static void bridge_dump_info(struct device *dev, struct blob_buf *b);
enum dev_change_type
bridge_reload(struct device *dev, struct blob_attr *attr);

static struct device_type bridge_device_type = {
	.name = "bridge",
	.config_params = &bridge_attr_list,

	.bridge_capability = true,
	.name_prefix = "br",

	.create = bridge_create,
	.config_init = bridge_config_init,
	.reload = bridge_reload,
	.free = bridge_free,
	.dump_info = bridge_dump_info,
};

struct bridge_state {
	struct device dev;
	device_state_cb set_state;

	struct blob_attr *config_data;
	struct bridge_config config;
	struct blob_attr *ifnames;
	bool active;
	bool force_active;

	struct uloop_timeout retry;
	struct bridge_member *primary_port;
	struct vlist_tree members;
	int n_present;
	int n_failed;
};

struct bridge_member {
	struct vlist_node node;
	struct bridge_state *bst;
	struct device_user dev;
	bool present;
	char name[];
};

static void
bridge_reset_primary(struct bridge_state *bst)
{
	struct bridge_member *bm;

	if (!bst->primary_port &&
	    (bst->dev.settings.flags & DEV_OPT_MACADDR))
		return;

	bst->primary_port = NULL;
	bst->dev.settings.flags &= ~DEV_OPT_MACADDR;
	vlist_for_each_element(&bst->members, bm, node) {
		uint8_t *macaddr;

		if (!bm->present)
			continue;

		bst->primary_port = bm;
		if (bm->dev.dev->settings.flags & DEV_OPT_MACADDR)
			macaddr = bm->dev.dev->settings.macaddr;
		else
			macaddr = bm->dev.dev->orig_settings.macaddr;
		memcpy(bst->dev.settings.macaddr, macaddr, 6);
		bst->dev.settings.flags |= DEV_OPT_MACADDR;
		return;
	}
}

static int
bridge_disable_member(struct bridge_member *bm)
{
	struct bridge_state *bst = bm->bst;

	if (!bm->present)
		return 0;

	system_bridge_delif(&bst->dev, bm->dev.dev);
	device_release(&bm->dev);

	device_broadcast_event(&bst->dev, DEV_EVENT_TOPO_CHANGE);

	return 0;
}

static int
bridge_enable_interface(struct bridge_state *bst)
{
	int ret;

	if (bst->active)
		return 0;

	ret = system_bridge_addbr(&bst->dev, &bst->config);
	if (ret < 0)
		return ret;

	bst->active = true;
	return 0;
}

static void
bridge_disable_interface(struct bridge_state *bst)
{
	if (!bst->active)
		return;

	system_bridge_delbr(&bst->dev);
	bst->active = false;
}

static int
bridge_enable_member(struct bridge_member *bm)
{
	struct bridge_state *bst = bm->bst;
	int ret;

	if (!bm->present)
		return 0;

	ret = bridge_enable_interface(bst);
	if (ret)
		goto error;

	/* Disable IPv6 for bridge members */
	if (!(bm->dev.dev->settings.flags & DEV_OPT_IPV6)) {
		bm->dev.dev->settings.ipv6 = 0;
		bm->dev.dev->settings.flags |= DEV_OPT_IPV6;
	}

	ret = device_claim(&bm->dev);
	if (ret < 0)
		goto error;

	ret = system_bridge_addif(&bst->dev, bm->dev.dev);
	if (ret < 0) {
		D(DEVICE, "Bridge device %s could not be added\n", bm->dev.dev->ifname);
		goto error;
	}

	device_set_present(&bst->dev, true);
	device_broadcast_event(&bst->dev, DEV_EVENT_TOPO_CHANGE);

	return 0;

error:
	bst->n_failed++;
	bm->present = false;
	bst->n_present--;
	device_release(&bm->dev);

	return ret;
}

static void
bridge_remove_member(struct bridge_member *bm)
{
	struct bridge_state *bst = bm->bst;

	if (!bm->present)
		return;

	if (bst->dev.active)
		bridge_disable_member(bm);

	bm->present = false;
	bm->bst->n_present--;

	if (bm == bst->primary_port)
		bridge_reset_primary(bst);

	if (bst->config.bridge_empty)
		return;

	bst->force_active = false;
	if (bst->n_present == 0)
		device_set_present(&bst->dev, false);
}

static void
bridge_free_member(struct bridge_member *bm)
{
	struct device *dev = bm->dev.dev;

	bridge_remove_member(bm);
	device_remove_user(&bm->dev);

	/*
	 * When reloading the config and moving a device from one bridge to
	 * another, the other bridge may have tried to claim this device
	 * before it was removed here.
	 * Ensure that claiming the device is retried by toggling its present
	 * state
	 */
	if (dev->present) {
		device_set_present(dev, false);
		device_set_present(dev, true);
	}

	free(bm);
}

static void
bridge_check_retry(struct bridge_state *bst)
{
	if (!bst->n_failed)
		return;

	uloop_timeout_set(&bst->retry, 100);
}

static void
bridge_member_cb(struct device_user *dev, enum device_event ev)
{
	struct bridge_member *bm = container_of(dev, struct bridge_member, dev);
	struct bridge_state *bst = bm->bst;

	switch (ev) {
	case DEV_EVENT_ADD:
		assert(!bm->present);

		bm->present = true;
		bst->n_present++;

		if (bst->n_present == 1)
			device_set_present(&bst->dev, true);
		if (bst->dev.active && !bridge_enable_member(bm)) {
			/*
			 * Adding a bridge member can overwrite the bridge mtu
			 * in the kernel, apply the bridge settings in case the
			 * bridge mtu is set
			 */
			system_if_apply_settings(&bst->dev, &bst->dev.settings,
						 DEV_OPT_MTU | DEV_OPT_MTU6);
		}

		break;
	case DEV_EVENT_REMOVE:
		if (dev->hotplug) {
			vlist_delete(&bst->members, &bm->node);
			return;
		}

		if (bm->present)
			bridge_remove_member(bm);

		break;
	default:
		return;
	}
}

static int
bridge_set_down(struct bridge_state *bst)
{
	struct bridge_member *bm;

	bst->set_state(&bst->dev, false);

	vlist_for_each_element(&bst->members, bm, node)
		bridge_disable_member(bm);

	bridge_disable_interface(bst);

	return 0;
}

static int
bridge_set_up(struct bridge_state *bst)
{
	struct bridge_member *bm;
	int ret;

	if (!bst->n_present) {
		if (!bst->force_active)
			return -ENOENT;

		ret = bridge_enable_interface(bst);
		if (ret)
			return ret;
	}

	bst->n_failed = 0;
	vlist_for_each_element(&bst->members, bm, node)
		bridge_enable_member(bm);
	bridge_check_retry(bst);

	if (!bst->force_active && !bst->n_present) {
		/* initialization of all member interfaces failed */
		bridge_disable_interface(bst);
		device_set_present(&bst->dev, false);
		return -ENOENT;
	}

	bridge_reset_primary(bst);
	ret = bst->set_state(&bst->dev, true);
	if (ret < 0)
		bridge_set_down(bst);

	return ret;
}

static int
bridge_set_state(struct device *dev, bool up)
{
	struct bridge_state *bst;

	bst = container_of(dev, struct bridge_state, dev);

	if (up)
		return bridge_set_up(bst);
	else
		return bridge_set_down(bst);
}

static struct bridge_member *
bridge_create_member(struct bridge_state *bst, const char *name,
		     struct device *dev, bool hotplug)
{
	struct bridge_member *bm;

	bm = calloc(1, sizeof(*bm) + strlen(name) + 1);
	if (!bm)
		return NULL;

	bm->bst = bst;
	bm->dev.cb = bridge_member_cb;
	bm->dev.hotplug = hotplug;
	strcpy(bm->name, name);
	bm->dev.dev = dev;
	vlist_add(&bst->members, &bm->node, bm->name);
	// Need to look up the bridge member again as the above
	// created pointer will be freed in case the bridge member
	// already existed
	bm = vlist_find(&bst->members, name, bm, node);
	if (hotplug && bm)
		bm->node.version = -1;

	return bm;
}

static void
bridge_member_update(struct vlist_tree *tree, struct vlist_node *node_new,
		     struct vlist_node *node_old)
{
	struct bridge_member *bm;
	struct device *dev;

	if (node_new) {
		bm = container_of(node_new, struct bridge_member, node);

		if (node_old) {
			free(bm);
			return;
		}

		dev = bm->dev.dev;
		bm->dev.dev = NULL;
		device_add_user(&bm->dev, dev);
	}


	if (node_old) {
		bm = container_of(node_old, struct bridge_member, node);
		bridge_free_member(bm);
	}
}


static void
bridge_add_member(struct bridge_state *bst, const char *name)
{
	struct device *dev;

	dev = device_get(name, true);
	if (!dev)
		return;

	bridge_create_member(bst, name, dev, false);
}

static int
bridge_hotplug_add(struct device *dev, struct device *member)
{
	struct bridge_state *bst = container_of(dev, struct bridge_state, dev);

	bridge_create_member(bst, member->ifname, member, true);

	return 0;
}

static int
bridge_hotplug_del(struct device *dev, struct device *member)
{
	struct bridge_state *bst = container_of(dev, struct bridge_state, dev);
	struct bridge_member *bm;

	bm = vlist_find(&bst->members, member->ifname, bm, node);
	if (!bm)
		return UBUS_STATUS_NOT_FOUND;

	vlist_delete(&bst->members, &bm->node);
	return 0;
}

static int
bridge_hotplug_prepare(struct device *dev)
{
	struct bridge_state *bst;

	bst = container_of(dev, struct bridge_state, dev);
	bst->force_active = true;
	device_set_present(&bst->dev, true);

	return 0;
}

static const struct device_hotplug_ops bridge_ops = {
	.prepare = bridge_hotplug_prepare,
	.add = bridge_hotplug_add,
	.del = bridge_hotplug_del
};

static void
bridge_free(struct device *dev)
{
	struct bridge_state *bst;

	bst = container_of(dev, struct bridge_state, dev);
	vlist_flush_all(&bst->members);
	free(bst->config_data);
	free(bst);
}

static void
bridge_dump_info(struct device *dev, struct blob_buf *b)
{
	struct bridge_state *bst;
	struct bridge_member *bm;
	void *list;

	bst = container_of(dev, struct bridge_state, dev);

	system_if_dump_info(dev, b);
	list = blobmsg_open_array(b, "bridge-members");

	vlist_for_each_element(&bst->members, bm, node) {
		if (bm->dev.dev->hidden)
			continue;

		blobmsg_add_string(b, NULL, bm->dev.dev->ifname);
	}

	blobmsg_close_array(b, list);
}

static void
bridge_config_init(struct device *dev)
{
	struct bridge_state *bst;
	struct blob_attr *cur;
	int rem;

	bst = container_of(dev, struct bridge_state, dev);

	if (bst->config.bridge_empty) {
		bst->force_active = true;
		device_set_present(&bst->dev, true);
	}

	bst->n_failed = 0;
	vlist_update(&bst->members);
	if (bst->ifnames) {
		blobmsg_for_each_attr(cur, bst->ifnames, rem) {
			bridge_add_member(bst, blobmsg_data(cur));
		}
	}
	vlist_flush(&bst->members);
	bridge_check_retry(bst);
}

static void
bridge_apply_settings(struct bridge_state *bst, struct blob_attr **tb)
{
	struct bridge_config *cfg = &bst->config;
	struct blob_attr *cur;

	/* defaults */
	cfg->stp = false;
	cfg->forward_delay = 2;
	cfg->robustness = 2;
	cfg->query_interval = 12500;
	cfg->query_response_interval = 1000;
	cfg->last_member_interval = 100;
	cfg->hash_max = 512;
	cfg->bridge_empty = false;
	cfg->priority = 0x7FFF;

	if ((cur = tb[BRIDGE_ATTR_STP]))
		cfg->stp = blobmsg_get_bool(cur);

	if ((cur = tb[BRIDGE_ATTR_FORWARD_DELAY]))
		cfg->forward_delay = blobmsg_get_u32(cur);

	if ((cur = tb[BRIDGE_ATTR_PRIORITY]))
		cfg->priority = blobmsg_get_u32(cur);

	if ((cur = tb[BRIDGE_ATTR_IGMP_SNOOP]))
		cfg->multicast_querier = cfg->igmp_snoop = blobmsg_get_bool(cur);

	if ((cur = tb[BRIDGE_ATTR_MULTICAST_QUERIER]))
		cfg->multicast_querier = blobmsg_get_bool(cur);

	if ((cur = tb[BRIDGE_ATTR_HASH_MAX]))
		cfg->hash_max = blobmsg_get_u32(cur);

	if ((cur = tb[BRIDGE_ATTR_ROBUSTNESS])) {
		cfg->robustness = blobmsg_get_u32(cur);
		cfg->flags |= BRIDGE_OPT_ROBUSTNESS;
	}

	if ((cur = tb[BRIDGE_ATTR_QUERY_INTERVAL])) {
		cfg->query_interval = blobmsg_get_u32(cur);
		cfg->flags |= BRIDGE_OPT_QUERY_INTERVAL;
	}

	if ((cur = tb[BRIDGE_ATTR_QUERY_RESPONSE_INTERVAL])) {
		cfg->query_response_interval = blobmsg_get_u32(cur);
		cfg->flags |= BRIDGE_OPT_QUERY_RESPONSE_INTERVAL;
	}

	if ((cur = tb[BRIDGE_ATTR_LAST_MEMBER_INTERVAL])) {
		cfg->last_member_interval = blobmsg_get_u32(cur);
		cfg->flags |= BRIDGE_OPT_LAST_MEMBER_INTERVAL;
	}

	if ((cur = tb[BRIDGE_ATTR_AGEING_TIME])) {
		cfg->ageing_time = blobmsg_get_u32(cur);
		cfg->flags |= BRIDGE_OPT_AGEING_TIME;
	}

	if ((cur = tb[BRIDGE_ATTR_HELLO_TIME])) {
		cfg->hello_time = blobmsg_get_u32(cur);
		cfg->flags |= BRIDGE_OPT_HELLO_TIME;
	}

	if ((cur = tb[BRIDGE_ATTR_MAX_AGE])) {
		cfg->max_age = blobmsg_get_u32(cur);
		cfg->flags |= BRIDGE_OPT_MAX_AGE;
	}

	if ((cur = tb[BRIDGE_ATTR_BRIDGE_EMPTY]))
		cfg->bridge_empty = blobmsg_get_bool(cur);
}

enum dev_change_type
bridge_reload(struct device *dev, struct blob_attr *attr)
{
	struct blob_attr *tb_dev[__DEV_ATTR_MAX];
	struct blob_attr *tb_br[__BRIDGE_ATTR_MAX];
	enum dev_change_type ret = DEV_CONFIG_APPLIED;
	unsigned long diff;
	struct bridge_state *bst;

	BUILD_BUG_ON(sizeof(diff) < __BRIDGE_ATTR_MAX / 8);
	BUILD_BUG_ON(sizeof(diff) < __DEV_ATTR_MAX / 8);

	bst = container_of(dev, struct bridge_state, dev);
	attr = blob_memdup(attr);

	blobmsg_parse(device_attr_list.params, __DEV_ATTR_MAX, tb_dev,
		blob_data(attr), blob_len(attr));
	blobmsg_parse(bridge_attrs, __BRIDGE_ATTR_MAX, tb_br,
		blob_data(attr), blob_len(attr));

	if (tb_dev[DEV_ATTR_MACADDR])
		bst->primary_port = NULL;

	bst->ifnames = tb_br[BRIDGE_ATTR_IFNAME];
	device_init_settings(dev, tb_dev);
	bridge_apply_settings(bst, tb_br);

	if (bst->config_data) {
		struct blob_attr *otb_dev[__DEV_ATTR_MAX];
		struct blob_attr *otb_br[__BRIDGE_ATTR_MAX];

		blobmsg_parse(device_attr_list.params, __DEV_ATTR_MAX, otb_dev,
			blob_data(bst->config_data), blob_len(bst->config_data));

		diff = 0;
		uci_blob_diff(tb_dev, otb_dev, &device_attr_list, &diff);
		if (diff)
		    ret = DEV_CONFIG_RESTART;

		blobmsg_parse(bridge_attrs, __BRIDGE_ATTR_MAX, otb_br,
			blob_data(bst->config_data), blob_len(bst->config_data));

		diff = 0;
		uci_blob_diff(tb_br, otb_br, &bridge_attr_list, &diff);
		if (diff & ~(1 << BRIDGE_ATTR_IFNAME))
		    ret = DEV_CONFIG_RESTART;

		bridge_config_init(dev);
	}

	free(bst->config_data);
	bst->config_data = attr;
	return ret;
}

static void
bridge_retry_members(struct uloop_timeout *timeout)
{
	struct bridge_state *bst = container_of(timeout, struct bridge_state, retry);
	struct bridge_member *bm;

	bst->n_failed = 0;
	vlist_for_each_element(&bst->members, bm, node) {
		if (bm->present)
			continue;

		if (!bm->dev.dev->present)
			continue;

		bm->present = true;
		bst->n_present++;
		bridge_enable_member(bm);
	}
}

static struct device *
bridge_create(const char *name, struct device_type *devtype,
	struct blob_attr *attr)
{
	struct bridge_state *bst;
	struct device *dev = NULL;

	bst = calloc(1, sizeof(*bst));
	if (!bst)
		return NULL;

	dev = &bst->dev;
	device_init(dev, devtype, name);
	dev->config_pending = true;
	bst->retry.cb = bridge_retry_members;

	bst->set_state = dev->set_state;
	dev->set_state = bridge_set_state;

	dev->hotplug_ops = &bridge_ops;

	vlist_init(&bst->members, avl_strcmp, bridge_member_update);
	bst->members.keep_old = true;
	bridge_reload(dev, attr);

	return dev;
}

static void __init bridge_device_type_init(void)
{
	device_type_add(&bridge_device_type);
}
