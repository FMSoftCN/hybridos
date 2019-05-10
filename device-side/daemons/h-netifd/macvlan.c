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
	MACVLAN_ATTR_IFNAME,
	MACVLAN_ATTR_MACADDR,
	MACVLAN_ATTR_MODE,
	__MACVLAN_ATTR_MAX
};

static const struct blobmsg_policy macvlan_attrs[__MACVLAN_ATTR_MAX] = {
	[MACVLAN_ATTR_IFNAME]  = { "ifname", BLOBMSG_TYPE_STRING },
	[MACVLAN_ATTR_MACADDR] = { "macaddr", BLOBMSG_TYPE_STRING },
	[MACVLAN_ATTR_MODE] = { "mode", BLOBMSG_TYPE_STRING },
};

static const struct uci_blob_param_list macvlan_attr_list = {
	.n_params = __MACVLAN_ATTR_MAX,
	.params = macvlan_attrs,

	.n_next = 1,
	.next = { &device_attr_list },
};

struct macvlan_device {
	struct device dev;
	struct device_user parent;

	device_state_cb set_state;

	struct blob_attr *config_data;
	struct blob_attr *ifname;
	struct macvlan_config config;
};

static void
macvlan_base_cb(struct device_user *dev, enum device_event ev)
{
	struct macvlan_device *mvdev = container_of(dev, struct macvlan_device, parent);

	switch (ev) {
	case DEV_EVENT_ADD:
		device_set_present(&mvdev->dev, true);
		break;
	case DEV_EVENT_REMOVE:
		device_set_present(&mvdev->dev, false);
		break;
	default:
		return;
	}
}

static int
macvlan_set_down(struct macvlan_device *mvdev)
{
	mvdev->set_state(&mvdev->dev, false);
	system_macvlan_del(&mvdev->dev);
	device_release(&mvdev->parent);

	return 0;
}

static int
macvlan_set_up(struct macvlan_device *mvdev)
{
	int ret;

	ret = device_claim(&mvdev->parent);
	if (ret < 0)
		return ret;

	ret = system_macvlan_add(&mvdev->dev, mvdev->parent.dev, &mvdev->config);
	if (ret < 0)
		goto release;

	ret = mvdev->set_state(&mvdev->dev, true);
	if (ret)
		goto delete;

	return 0;

delete:
	system_macvlan_del(&mvdev->dev);
release:
	device_release(&mvdev->parent);
	return ret;
}

static int
macvlan_set_state(struct device *dev, bool up)
{
	struct macvlan_device *mvdev;

	D(SYSTEM, "macvlan_set_state(%s, %u)\n", dev->ifname, up);

	mvdev = container_of(dev, struct macvlan_device, dev);
	if (up)
		return macvlan_set_up(mvdev);
	else
		return macvlan_set_down(mvdev);
}

static void
macvlan_free(struct device *dev)
{
	struct macvlan_device *mvdev;

	mvdev = container_of(dev, struct macvlan_device, dev);
	device_remove_user(&mvdev->parent);
	free(mvdev->config_data);
	free(mvdev);
}

static void
macvlan_dump_info(struct device *dev, struct blob_buf *b)
{
	struct macvlan_device *mvdev;

	mvdev = container_of(dev, struct macvlan_device, dev);
	blobmsg_add_string(b, "parent", mvdev->parent.dev->ifname);
	system_if_dump_info(dev, b);
}

static void
macvlan_config_init(struct device *dev)
{
	struct macvlan_device *mvdev;
	struct device *basedev = NULL;

	mvdev = container_of(dev, struct macvlan_device, dev);
	if (mvdev->ifname)
		basedev = device_get(blobmsg_data(mvdev->ifname), true);

	device_add_user(&mvdev->parent, basedev);
}

static void
macvlan_apply_settings(struct macvlan_device *mvdev, struct blob_attr **tb)
{
	struct macvlan_config *cfg = &mvdev->config;
	struct blob_attr *cur;
	struct ether_addr *ea;

	cfg->flags = 0;
	cfg->mode = NULL;

	if ((cur = tb[MACVLAN_ATTR_MACADDR])) {
		ea = ether_aton(blobmsg_data(cur));
		if (ea) {
			memcpy(cfg->macaddr, ea, 6);
			cfg->flags |= MACVLAN_OPT_MACADDR;
		}
	}

	if ((cur = tb[MACVLAN_ATTR_MODE]))
		cfg->mode = blobmsg_data(cur);
}

static enum dev_change_type
macvlan_reload(struct device *dev, struct blob_attr *attr)
{
	struct blob_attr *tb_dev[__DEV_ATTR_MAX];
	struct blob_attr *tb_mv[__MACVLAN_ATTR_MAX];
	enum dev_change_type ret = DEV_CONFIG_APPLIED;
	struct macvlan_device *mvdev;

	mvdev = container_of(dev, struct macvlan_device, dev);
	attr = blob_memdup(attr);

	blobmsg_parse(device_attr_list.params, __DEV_ATTR_MAX, tb_dev,
		blob_data(attr), blob_len(attr));
	blobmsg_parse(macvlan_attrs, __MACVLAN_ATTR_MAX, tb_mv,
		blob_data(attr), blob_len(attr));

	device_init_settings(dev, tb_dev);
	macvlan_apply_settings(mvdev, tb_mv);
	mvdev->ifname = tb_mv[MACVLAN_ATTR_IFNAME];

	if (mvdev->config_data) {
		struct blob_attr *otb_dev[__DEV_ATTR_MAX];
		struct blob_attr *otb_mv[__MACVLAN_ATTR_MAX];

		blobmsg_parse(device_attr_list.params, __DEV_ATTR_MAX, otb_dev,
			blob_data(mvdev->config_data), blob_len(mvdev->config_data));

		if (uci_blob_diff(tb_dev, otb_dev, &device_attr_list, NULL))
		    ret = DEV_CONFIG_RESTART;

		blobmsg_parse(macvlan_attrs, __MACVLAN_ATTR_MAX, otb_mv,
			blob_data(mvdev->config_data), blob_len(mvdev->config_data));

		if (uci_blob_diff(tb_mv, otb_mv, &macvlan_attr_list, NULL))
		    ret = DEV_CONFIG_RESTART;

		macvlan_config_init(dev);
	}

	free(mvdev->config_data);
	mvdev->config_data = attr;
	return ret;
}

static struct device *
macvlan_create(const char *name, struct device_type *devtype,
	struct blob_attr *attr)
{
	struct macvlan_device *mvdev;
	struct device *dev = NULL;

	mvdev = calloc(1, sizeof(*mvdev));
	if (!mvdev)
		return NULL;

	dev = &mvdev->dev;
	device_init(dev, devtype, name);
	dev->config_pending = true;

	mvdev->set_state = dev->set_state;
	dev->set_state = macvlan_set_state;

	dev->hotplug_ops = NULL;
	mvdev->parent.cb = macvlan_base_cb;

	macvlan_reload(dev, attr);

	return dev;
}

static struct device_type macvlan_device_type = {
	.name = "macvlan",
	.config_params = &macvlan_attr_list,
	.create = macvlan_create,
	.config_init = macvlan_config_init,
	.reload = macvlan_reload,
	.free = macvlan_free,
	.dump_info = macvlan_dump_info,
};

static void __init macvlan_device_type_init(void)
{
	device_type_add(&macvlan_device_type);
}
