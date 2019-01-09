/*
 * netifd - network interface daemon
 * Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 * Copyright (C) 2013 Jo-Philipp Wich <jow@openwrt.org>
 * Copyright (C) 2017 Matthias Schiffer <mschiffer@universe-factory.net>
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
#include <net/ethernet.h>

#ifdef linux
#include <netinet/ether.h>
#endif

#include "netifd.h"
#include "device.h"
#include "interface.h"
#include "system.h"

enum {
	VETH_ATTR_MACADDR,
	VETH_ATTR_PEER_NAME,
	VETH_ATTR_PEER_MACADDR,
	__VETH_ATTR_MAX
};

static const struct blobmsg_policy veth_attrs[__VETH_ATTR_MAX] = {
	[VETH_ATTR_MACADDR] = { "macaddr", BLOBMSG_TYPE_STRING },
	[VETH_ATTR_PEER_NAME]  = { "peer_name", BLOBMSG_TYPE_STRING },
	[VETH_ATTR_PEER_MACADDR] = { "peer_macaddr", BLOBMSG_TYPE_STRING },
};

static const struct uci_blob_param_list veth_attr_list = {
	.n_params = __VETH_ATTR_MAX,
	.params = veth_attrs,

	.n_next = 1,
	.next = { &device_attr_list },
};

struct veth {
	struct device dev;

	device_state_cb set_state;

	struct blob_attr *config_data;
	struct veth_config config;
};

static int
veth_set_down(struct veth *veth)
{
	veth->set_state(&veth->dev, false);
	system_veth_del(&veth->dev);

	return 0;
}

static int
veth_set_up(struct veth *veth)
{
	int ret;

	ret = system_veth_add(&veth->dev, &veth->config);
	if (ret < 0)
		return ret;

	ret = veth->set_state(&veth->dev, true);
	if (ret)
		goto delete;

	return 0;

delete:
	system_veth_del(&veth->dev);
	return ret;
}

static int
veth_set_state(struct device *dev, bool up)
{
	struct veth *veth;

	D(SYSTEM, "veth_set_state(%s, %u)\n", dev->ifname, up);

	veth = container_of(dev, struct veth, dev);
	if (up)
		return veth_set_up(veth);
	else
		return veth_set_down(veth);
}

static void
veth_free(struct device *dev)
{
	struct veth *veth;

	veth = container_of(dev, struct veth, dev);
	free(veth->config_data);
	free(veth);
}

static void
veth_dump_info(struct device *dev, struct blob_buf *b)
{
	struct veth *veth;

	veth = container_of(dev, struct veth, dev);
	if (veth->config.flags & VETH_OPT_PEER_NAME)
		blobmsg_add_string(b, "peer", veth->config.peer_name);
	system_if_dump_info(dev, b);
}

static void
veth_config_init(struct device *dev)
{
	device_set_present(dev, true);
}

static void
veth_apply_settings(struct veth *veth, struct blob_attr **tb)
{
	struct veth_config *cfg = &veth->config;
	struct blob_attr *cur;
	struct ether_addr *ea;

	cfg->flags = 0;

	if ((cur = tb[VETH_ATTR_MACADDR]))
	{
		ea = ether_aton(blobmsg_data(cur));
		if (ea) {
			memcpy(cfg->macaddr, ea, 6);
			cfg->flags |= VETH_OPT_MACADDR;
		}
	}

	if ((cur = tb[VETH_ATTR_PEER_NAME]))
	{
		strncpy(cfg->peer_name, blobmsg_get_string(cur), sizeof(cfg->peer_name)-1);
		cfg->flags |= VETH_OPT_PEER_NAME;
	}

	if ((cur = tb[VETH_ATTR_PEER_MACADDR]))
	{
		ea = ether_aton(blobmsg_data(cur));
		if (ea) {
			memcpy(cfg->peer_macaddr, ea, 6);
			cfg->flags |= VETH_OPT_PEER_MACADDR;
		}
	}
}

static enum dev_change_type
veth_reload(struct device *dev, struct blob_attr *attr)
{
	struct blob_attr *tb_dev[__DEV_ATTR_MAX];
	struct blob_attr *tb_mv[__VETH_ATTR_MAX];
	enum dev_change_type ret = DEV_CONFIG_APPLIED;
	struct veth *veth;

	veth = container_of(dev, struct veth, dev);
	attr = blob_memdup(attr);

	blobmsg_parse(device_attr_list.params, __DEV_ATTR_MAX, tb_dev,
		blob_data(attr), blob_len(attr));
	blobmsg_parse(veth_attrs, __VETH_ATTR_MAX, tb_mv,
		blob_data(attr), blob_len(attr));

	device_init_settings(dev, tb_dev);
	veth_apply_settings(veth, tb_mv);

	if (veth->config_data) {
		struct blob_attr *otb_dev[__DEV_ATTR_MAX];
		struct blob_attr *otb_mv[__VETH_ATTR_MAX];

		blobmsg_parse(device_attr_list.params, __DEV_ATTR_MAX, otb_dev,
			blob_data(veth->config_data), blob_len(veth->config_data));

		if (uci_blob_diff(tb_dev, otb_dev, &device_attr_list, NULL))
		    ret = DEV_CONFIG_RESTART;

		blobmsg_parse(veth_attrs, __VETH_ATTR_MAX, otb_mv,
			blob_data(veth->config_data), blob_len(veth->config_data));

		if (uci_blob_diff(tb_mv, otb_mv, &veth_attr_list, NULL))
		    ret = DEV_CONFIG_RESTART;

		veth_config_init(dev);
	}

	free(veth->config_data);
	veth->config_data = attr;
	return ret;
}

static struct device *
veth_create(const char *name, struct device_type *devtype,
	struct blob_attr *attr)
{
	struct veth *veth;
	struct device *dev = NULL;

	veth = calloc(1, sizeof(*veth));
	if (!veth)
		return NULL;

	dev = &veth->dev;
	device_init(dev, devtype, name);
	dev->config_pending = true;

	veth->set_state = dev->set_state;
	dev->set_state = veth_set_state;

	dev->hotplug_ops = NULL;

	veth_reload(dev, attr);

	return dev;
}

static struct device_type veth_device_type = {
	.name = "veth",
	.config_params = &veth_attr_list,
	.create = veth_create,
	.config_init = veth_config_init,
	.reload = veth_reload,
	.free = veth_free,
	.dump_info = veth_dump_info,
};

static void __init veth_device_type_init(void)
{
	device_type_add(&veth_device_type);
}
