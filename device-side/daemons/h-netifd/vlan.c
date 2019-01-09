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

#include "netifd.h"
#include "system.h"

struct vlan_device {
	struct device dev;
	struct device_user dep;

	device_state_cb set_state;
	int id;
};

static void free_vlan_if(struct device *iface)
{
	struct vlan_device *vldev;

	vldev = container_of(iface, struct vlan_device, dev);
	device_remove_user(&vldev->dep);
	device_cleanup(&vldev->dev);
	free(vldev);
}

static int vlan_set_device_state(struct device *dev, bool up)
{
	struct vlan_device *vldev;
	int ret = 0;

	vldev = container_of(dev, struct vlan_device, dev);
	if (!up) {
		vldev->set_state(dev, false);
		system_vlan_del(dev);
		device_release(&vldev->dep);
		return 0;
	}

	ret = device_claim(&vldev->dep);
	if (ret < 0)
		return ret;

	system_vlan_add(vldev->dep.dev, vldev->id);
	ret = vldev->set_state(dev, true);
	if (ret)
		device_release(&vldev->dep);

	return ret;
}

static void vlan_dev_set_name(struct vlan_device *vldev, struct device *dev)
{
	char *name;

	name = alloca(strlen(dev->ifname) + sizeof(".2147483647\0"));
	vldev->dev.hidden = dev->hidden;
	sprintf(name, "%s.%d", dev->ifname, vldev->id);

	device_set_ifname(&vldev->dev, name);
}

static void vlan_dev_cb(struct device_user *dep, enum device_event ev)
{
	struct vlan_device *vldev;

	vldev = container_of(dep, struct vlan_device, dep);
	switch(ev) {
	case DEV_EVENT_ADD:
		device_set_present(&vldev->dev, true);
		break;
	case DEV_EVENT_REMOVE:
		device_set_present(&vldev->dev, false);
		break;
	case DEV_EVENT_UPDATE_IFNAME:
		vlan_dev_set_name(vldev, dep->dev);
		break;
	case DEV_EVENT_TOPO_CHANGE:
		/* Propagate topo changes */
		device_broadcast_event(&vldev->dev, DEV_EVENT_TOPO_CHANGE);
		break;
	default:
		break;
	}
}

static struct device *get_vlan_device(struct device *dev, int id, bool create)
{
	static struct device_type vlan_type = {
		.name = "VLAN",
		.config_params = &device_attr_list,
		.free = free_vlan_if,
	};
	struct vlan_device *vldev;
	struct device_user *dep;

	if (strlen(dev->ifname) > (IFNAMSIZ - 6))
		return NULL;

	/* look for an existing interface before creating a new one */
	list_for_each_entry(dep, &dev->users.list, list.list) {
		if (dep->cb != vlan_dev_cb)
			continue;

		vldev = container_of(dep, struct vlan_device, dep);
		if (vldev->id != id)
			continue;

		return &vldev->dev;
	}

	if (!create)
		return NULL;

	D(DEVICE, "Create vlan device '%s.%d'\n", dev->ifname, id);

	vldev = calloc(1, sizeof(*vldev));
	if (!vldev)
		return NULL;

	vldev->id = id;

	device_init(&vldev->dev, &vlan_type, NULL);

	vlan_dev_set_name(vldev, dev);
	vldev->dev.default_config = true;

	vldev->set_state = vldev->dev.set_state;
	vldev->dev.set_state = vlan_set_device_state;

	vldev->dep.cb = vlan_dev_cb;
	device_add_user(&vldev->dep, dev);

	return &vldev->dev;
}

static char *split_vlan(char *s)
{
	s = strchr(s, '.');
	if (!s)
		goto out;

	*s = 0;
	s++;

out:
	return s;
}

struct device *get_vlan_device_chain(const char *ifname, bool create)
{
	struct device *dev = NULL;
	char *buf, *s, *next, *err = NULL;
	int id;

	buf = strdup(ifname);
	if (!buf)
		return NULL;

	s = split_vlan(buf);
	dev = device_get(buf, create);
	if (!dev)
		goto error;

	do {
		next = split_vlan(s);
		id = strtoul(s, &err, 10);
		if (err && *err)
			goto error;

		dev = get_vlan_device(dev, id, create);
		if (!dev)
			goto error;

		s = next;
		if (!s)
			goto out;
	} while (1);

error:
	dev = NULL;
out:
	free(buf);
	return dev;
}
