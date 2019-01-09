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
#include "netifd.h"
#include "device.h"
#include "config.h"
#include "system.h"

struct tunnel {
	struct device dev;
	device_state_cb set_state;
};

static int
tunnel_set_state(struct device *dev, bool up)
{
	struct tunnel *tun = container_of(dev, struct tunnel, dev);
	int ret;

	if (up) {
		ret = system_add_ip_tunnel(dev->ifname, dev->config);
		if (ret != 0)
			return ret;
	}

	ret = tun->set_state(dev, up);
	if (ret || !up)
		system_del_ip_tunnel(dev->ifname, dev->config);

	return ret;
}

static enum dev_change_type
tunnel_reload(struct device *dev, struct blob_attr *attr)
{
	struct blob_attr *tb_dev[__DEV_ATTR_MAX];
	const struct uci_blob_param_list *cfg = dev->type->config_params;

	if (uci_blob_check_equal(dev->config, attr, cfg))
		return DEV_CONFIG_NO_CHANGE;

	memset(tb_dev, 0, sizeof(tb_dev));

	if (attr)
		blobmsg_parse(device_attr_list.params, __DEV_ATTR_MAX, tb_dev,
			blob_data(attr), blob_len(attr));

	device_init_settings(dev, tb_dev);

	return DEV_CONFIG_RESTART;
}

static struct device *
tunnel_create(const char *name, struct device_type *devtype,
	struct blob_attr *attr)
{
	struct tunnel *tun;
	struct device *dev;

	tun = calloc(1, sizeof(*tun));
	if (!tun)
		return NULL;

	dev = &tun->dev;
	device_init(dev, devtype, name);
	tun->set_state = dev->set_state;
	dev->set_state = tunnel_set_state;
	device_apply_config(dev, devtype, attr);
	device_set_present(dev, true);

	return dev;
}

static void
tunnel_free(struct device *dev)
{
	struct tunnel *tun = container_of(dev, struct tunnel, dev);

	free(tun);
}

struct device_type tunnel_device_type = {
	.name = "tunnel",
	.config_params = &tunnel_attr_list,
	.reload = tunnel_reload,
	.create = tunnel_create,
	.free = tunnel_free,
};

static void __init tunnel_device_type_init(void)
{
	device_type_add(&tunnel_device_type);
}
