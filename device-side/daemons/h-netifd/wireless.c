/*
 * netifd - network interface daemon
 * Copyright (C) 2013 Felix Fietkau <nbd@openwrt.org>
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
#include <signal.h>
#include "netifd.h"
#include "wireless.h"
#include "handler.h"
#include "ubus.h"

#define WIRELESS_SETUP_RETRY	3

struct vlist_tree wireless_devices;
struct avl_tree wireless_drivers;
static struct blob_buf b;
static int drv_fd;

static const struct blobmsg_policy wdev_policy =
	{ .name = "disabled", .type = BLOBMSG_TYPE_BOOL };

static const struct uci_blob_param_list wdev_param = {
	.n_params = 1,
	.params = &wdev_policy,
};

enum {
	VIF_ATTR_DISABLED,
	VIF_ATTR_NETWORK,
	VIF_ATTR_ISOLATE,
	VIF_ATTR_MODE,
	__VIF_ATTR_MAX,
};

static const struct blobmsg_policy vif_policy[__VIF_ATTR_MAX] = {
	[VIF_ATTR_DISABLED] = { .name = "disabled", .type = BLOBMSG_TYPE_BOOL },
	[VIF_ATTR_NETWORK] = { .name = "network", .type = BLOBMSG_TYPE_ARRAY },
	[VIF_ATTR_ISOLATE] = { .name = "isolate", .type = BLOBMSG_TYPE_BOOL },
	[VIF_ATTR_MODE] = { .name = "mode", .type = BLOBMSG_TYPE_STRING },
};

static const struct uci_blob_param_list vif_param = {
	.n_params = ARRAY_SIZE(vif_policy),
	.params = vif_policy,
};

static void
put_container(struct blob_buf *buf, struct blob_attr *attr, const char *name)
{
	void *c = blobmsg_open_table(buf, name);
	blob_put_raw(buf, blob_data(attr), blob_len(attr));
	blobmsg_close_table(buf, c);
}

static void
vif_config_add_bridge(struct blob_buf *buf, struct blob_attr *networks, bool prepare)
{
	struct interface *iface;
	struct device *dev = NULL;
	struct blob_attr *cur;
	const char *network;
	int rem;

	if (!networks)
		return;

	blobmsg_for_each_attr(cur, networks, rem) {
		network = blobmsg_data(cur);

		iface = vlist_find(&interfaces, network, iface, node);
		if (!iface)
			continue;

		dev = iface->main_dev.dev;
		if (!dev)
			return;

		if (!dev->type->bridge_capability)
			return;
	}

	if (!dev)
		return;

	if (dev->hotplug_ops && dev->hotplug_ops->prepare)
		dev->hotplug_ops->prepare(dev);

	blobmsg_add_string(buf, "bridge", dev->ifname);

	if (dev->settings.flags & DEV_OPT_MULTICAST_TO_UNICAST)
		blobmsg_add_u8(buf, "multicast_to_unicast",
			       dev->settings.multicast_to_unicast);
}

static void
prepare_config(struct wireless_device *wdev, struct blob_buf *buf, bool up)
{
	struct wireless_interface *vif;
	void *l, *i;

	blob_buf_init(&b, 0);
	put_container(&b, wdev->config, "config");
	if (wdev->data)
		blobmsg_add_blob(&b, wdev->data);

	l = blobmsg_open_table(&b, "interfaces");
	vlist_for_each_element(&wdev->interfaces, vif, node) {
		i = blobmsg_open_table(&b, vif->name);
		vif_config_add_bridge(&b, vif->network, up);
		put_container(&b, vif->config, "config");
		if (vif->data)
			blobmsg_add_blob(&b, vif->data);
		blobmsg_close_table(&b, i);
	}
	blobmsg_close_table(&b, l);
}

static bool
wireless_process_check(struct wireless_process *proc)
{
	return check_pid_path(proc->pid, proc->exe);
}

static void
wireless_complete_kill_request(struct wireless_device *wdev)
{
	if (!wdev->kill_request)
		return;

	ubus_complete_deferred_request(ubus_ctx, wdev->kill_request, 0);
	free(wdev->kill_request);
	wdev->kill_request = NULL;
}

static void
wireless_process_free(struct wireless_device *wdev, struct wireless_process *proc)
{
	D(WIRELESS, "Wireless device '%s' free pid %d\n", wdev->name, proc->pid);
	list_del(&proc->list);
	free(proc);

	if (list_empty(&wdev->script_proc))
		wireless_complete_kill_request(wdev);
}

static void
wireless_close_script_proc_fd(struct wireless_device *wdev)
{
	if (wdev->script_proc_fd.fd < 0)
		return;

	uloop_fd_delete(&wdev->script_proc_fd);
	close(wdev->script_proc_fd.fd);
	wdev->script_proc_fd.fd = -1;
}

static void
wireless_process_kill_all(struct wireless_device *wdev, int signal, bool free)
{
	struct wireless_process *proc, *tmp;

	list_for_each_entry_safe(proc, tmp, &wdev->script_proc, list) {
		bool check = wireless_process_check(proc);

		if (check) {
			D(WIRELESS, "Wireless device '%s' kill pid %d\n", wdev->name, proc->pid);
			kill(proc->pid, signal);
		}

		if (free || !check)
			wireless_process_free(wdev, proc);
	}

	if (free)
		wireless_close_script_proc_fd(wdev);
}

static void
wireless_device_free_state(struct wireless_device *wdev)
{
	struct wireless_interface *vif;

	uloop_timeout_cancel(&wdev->script_check);
	uloop_timeout_cancel(&wdev->timeout);
	wireless_complete_kill_request(wdev);
	free(wdev->data);
	wdev->data = NULL;
	vlist_for_each_element(&wdev->interfaces, vif, node) {
		free(vif->data);
		vif->data = NULL;
		vif->ifname = NULL;
	}
}

static void wireless_interface_handle_link(struct wireless_interface *vif, bool up)
{
	struct interface *iface;
	struct blob_attr *cur;
	const char *network;
	int rem;

	if (!vif->network || !vif->ifname)
		return;

	if (up) {
		struct device *dev = device_get(vif->ifname, 2);
		if (dev) {
			dev->wireless_isolate = vif->isolate;
			dev->wireless = true;
			dev->wireless_ap = vif->ap_mode;
		}
	}

	blobmsg_for_each_attr(cur, vif->network, rem) {
		network = blobmsg_data(cur);

		iface = vlist_find(&interfaces, network, iface, node);
		if (!iface)
			continue;

		interface_handle_link(iface, vif->ifname, up, true);
	}
}

static void
wireless_device_setup_cancel(struct wireless_device *wdev)
{
	if (wdev->cancel)
		return;

	D(WIRELESS, "Cancel wireless device '%s' setup\n", wdev->name);
	wdev->cancel = true;
	uloop_timeout_set(&wdev->timeout, 10 * 1000);
}

static void
wireless_device_run_handler(struct wireless_device *wdev, bool up)
{
	const char *action = up ? "setup" : "teardown";
	const char *argv[6];
	char *config;
	int i = 0;
	int fds[2] = { -1, -1 };

	D(WIRELESS, "Wireless device '%s' run %s handler\n", wdev->name, action);
	if (!up && wdev->prev_config) {
		config = blobmsg_format_json(wdev->prev_config, true);
		free(wdev->prev_config);
		wdev->prev_config = NULL;
	} else {
		prepare_config(wdev, &b, up);
		config = blobmsg_format_json(b.head, true);
	}

	argv[i++] = wdev->drv->script;
	argv[i++] = wdev->drv->name;
	argv[i++] = action;
	argv[i++] = wdev->name;
	argv[i++] = config;
	argv[i] = NULL;

	if (up && pipe(fds) == 0) {
		wdev->script_proc_fd.fd = fds[0];
		uloop_fd_add(&wdev->script_proc_fd,
			     ULOOP_READ | ULOOP_EDGE_TRIGGER);
	}

	netifd_start_process(argv, NULL, &wdev->script_task);

	if (fds[1] >= 0)
		close(fds[1]);

	free(config);
}

static void
__wireless_device_set_up(struct wireless_device *wdev)
{
	if (wdev->disabled)
		return;

	if (wdev->retry_setup_failed)
		return;

	if (!wdev->autostart)
		return;

	if (wdev->state != IFS_DOWN || config_init)
		return;

	free(wdev->prev_config);
	wdev->prev_config = NULL;
	wdev->state = IFS_SETUP;
	wireless_device_run_handler(wdev, true);
}

static void
wireless_device_free(struct wireless_device *wdev)
{
	vlist_flush_all(&wdev->interfaces);
	avl_delete(&wireless_devices.avl, &wdev->node.avl);
	free(wdev->config);
	free(wdev->prev_config);
	free(wdev);
}

static void
wdev_handle_config_change(struct wireless_device *wdev)
{
	enum interface_config_state state = wdev->config_state;

	switch(state) {
	case IFC_NORMAL:
	case IFC_RELOAD:
		__wireless_device_set_up(wdev);

		wdev->config_state = IFC_NORMAL;
		break;
	case IFC_REMOVE:
		wireless_device_free(wdev);
		break;
	}
}

static void
wireless_device_mark_down(struct wireless_device *wdev)
{
	struct wireless_interface *vif;

	D(WIRELESS, "Wireless device '%s' is now down\n", wdev->name);

	vlist_for_each_element(&wdev->interfaces, vif, node)
		wireless_interface_handle_link(vif, false);

	wireless_process_kill_all(wdev, SIGTERM, true);

	wdev->cancel = false;
	wdev->state = IFS_DOWN;
	wireless_device_free_state(wdev);
	wdev_handle_config_change(wdev);
}

static void
wireless_device_setup_timeout(struct uloop_timeout *timeout)
{
	struct wireless_device *wdev = container_of(timeout, struct wireless_device, timeout);

	netifd_kill_process(&wdev->script_task);
	wdev->script_task.cb(&wdev->script_task, -1);
	wireless_device_mark_down(wdev);
}

void
wireless_device_set_up(struct wireless_device *wdev)
{
	wdev->retry = WIRELESS_SETUP_RETRY;
	wdev->autostart = true;
	__wireless_device_set_up(wdev);
}

static void
__wireless_device_set_down(struct wireless_device *wdev)
{
	if (wdev->state == IFS_TEARDOWN || wdev->state == IFS_DOWN)
		return;

	if (wdev->script_task.uloop.pending) {
		wireless_device_setup_cancel(wdev);
		return;
	}

	wdev->state = IFS_TEARDOWN;
	wireless_device_run_handler(wdev, false);
}

static void
wireless_device_mark_up(struct wireless_device *wdev)
{
	struct wireless_interface *vif;

	if (wdev->cancel) {
		wdev->cancel = false;
		__wireless_device_set_down(wdev);
		return;
	}

	D(WIRELESS, "Wireless device '%s' is now up\n", wdev->name);
	wdev->state = IFS_UP;
	vlist_for_each_element(&wdev->interfaces, vif, node)
		wireless_interface_handle_link(vif, true);
}

static void
wireless_device_retry_setup(struct wireless_device *wdev)
{
	if (wdev->state == IFS_TEARDOWN || wdev->state == IFS_DOWN || wdev->cancel)
		return;

	if (--wdev->retry < 0)
		wdev->retry_setup_failed = true;

	__wireless_device_set_down(wdev);
}

static void
wireless_device_script_task_cb(struct netifd_process *proc, int ret)
{
	struct wireless_device *wdev = container_of(proc, struct wireless_device, script_task);

	switch (wdev->state) {
	case IFS_SETUP:
		wireless_device_retry_setup(wdev);
		break;
	case IFS_TEARDOWN:
		wireless_device_mark_down(wdev);
		break;
	default:
		break;
	}
}

void
wireless_device_set_down(struct wireless_device *wdev)
{
	wdev->retry_setup_failed = false;
	wdev->autostart = false;
	__wireless_device_set_down(wdev);
}

static void
wdev_set_config_state(struct wireless_device *wdev, enum interface_config_state s)
{
	if (wdev->config_state != IFC_NORMAL)
		return;

	wdev->config_state = s;
	if (wdev->state == IFS_DOWN)
		wdev_handle_config_change(wdev);
	else
		__wireless_device_set_down(wdev);
}

static void
wdev_prepare_prev_config(struct wireless_device *wdev)
{
	if (wdev->prev_config)
		return;

	prepare_config(wdev, &b, false);
	wdev->prev_config = blob_memdup(b.head);
}

static void
wdev_change_config(struct wireless_device *wdev, struct wireless_device *wd_new)
{
	struct blob_attr *new_config = wd_new->config;
	bool disabled = wd_new->disabled;

	free(wd_new);

	wdev_prepare_prev_config(wdev);
	if (blob_attr_equal(wdev->config, new_config) && wdev->disabled == disabled)
		return;

	D(WIRELESS, "Update configuration of wireless device '%s'\n", wdev->name);
	free(wdev->config);
	wdev->config = blob_memdup(new_config);
	wdev->disabled = disabled;
	wdev->retry_setup_failed = false;
	wdev_set_config_state(wdev, IFC_RELOAD);
}

static void
wdev_create(struct wireless_device *wdev)
{
	wdev->retry = WIRELESS_SETUP_RETRY;
	wdev->config = blob_memdup(wdev->config);
}

static void
wdev_update(struct vlist_tree *tree, struct vlist_node *node_new,
	    struct vlist_node *node_old)
{
	struct wireless_device *wd_old = container_of(node_old, struct wireless_device, node);
	struct wireless_device *wd_new = container_of(node_new, struct wireless_device, node);

	if (wd_old && wd_new) {
		wdev_change_config(wd_old, wd_new);
	} else if (wd_old) {
		D(WIRELESS, "Delete wireless device '%s'\n", wd_old->name);
		wdev_set_config_state(wd_old, IFC_REMOVE);
	} else if (wd_new) {
		D(WIRELESS, "Create wireless device '%s'\n", wd_new->name);
		wdev_create(wd_new);
	}
}

static void
wireless_add_handler(const char *script, const char *name, json_object *obj)
{
	struct wireless_driver *drv;
	char *name_str, *script_str;
	json_object *dev_config_obj, *iface_config_obj;
	struct uci_blob_param_list *dev_config, *iface_config;

	dev_config_obj = json_get_field(obj, "device", json_type_array);
	iface_config_obj = json_get_field(obj, "iface", json_type_array);

	if (!dev_config_obj || !iface_config_obj)
		return;

	drv = calloc_a(sizeof(*drv),
		&dev_config, sizeof(*dev_config) + sizeof(void *),
		&iface_config, sizeof(*iface_config) + sizeof(void *),
		&name_str, strlen(name) + 1,
		&script_str, strlen(script) + 1);

	drv->name = strcpy(name_str, name);
	drv->script = strcpy(script_str, script);

	dev_config->n_next = 1;
	dev_config->next[0] = &wdev_param;
	drv->device.config = dev_config;

	iface_config->n_next = 1;
	iface_config->next[0] = &vif_param;
	drv->interface.config = iface_config;

	drv->device.buf = netifd_handler_parse_config(drv->device.config, dev_config_obj);
	drv->interface.buf = netifd_handler_parse_config(drv->interface.config, iface_config_obj);

	drv->node.key = drv->name;
	avl_insert(&wireless_drivers, &drv->node);
	D(WIRELESS, "Add handler for script %s: %s\n", script, name);
}

void wireless_init(void)
{
	vlist_init(&wireless_devices, avl_strcmp, wdev_update);
	wireless_devices.keep_old = true;
	wireless_devices.no_delete = true;

	avl_init(&wireless_drivers, avl_strcmp, false, NULL);
	drv_fd = netifd_open_subdir("wireless");
	if (drv_fd < 0)
		return;

	netifd_init_script_handlers(drv_fd, wireless_add_handler);
}

static void
wireless_interface_init_config(struct wireless_interface *vif)
{
	struct blob_attr *tb[__VIF_ATTR_MAX];
	struct blob_attr *cur;

	vif->network = NULL;
	blobmsg_parse(vif_policy, __VIF_ATTR_MAX, tb, blob_data(vif->config), blob_len(vif->config));

	if ((cur = tb[VIF_ATTR_NETWORK]))
		vif->network = cur;

	cur = tb[VIF_ATTR_ISOLATE];
	if (cur)
		vif->isolate = blobmsg_get_bool(cur);

	cur = tb[VIF_ATTR_MODE];
	if (cur)
		vif->ap_mode = !strcmp(blobmsg_get_string(cur), "ap");
}

static void
vif_update(struct vlist_tree *tree, struct vlist_node *node_new,
	   struct vlist_node *node_old)
{
	struct wireless_interface *vif_old = container_of(node_old, struct wireless_interface, node);
	struct wireless_interface *vif_new = container_of(node_new, struct wireless_interface, node);
	struct wireless_device *wdev;

	if (vif_old)
		wdev = vif_old->wdev;
	else
		wdev = vif_new->wdev;

	if (vif_old && vif_new) {
		free((void *) vif_old->section);
		vif_old->section = strdup(vif_new->section);
		if (blob_attr_equal(vif_old->config, vif_new->config)) {
			free(vif_new);
			return;
		}

		D(WIRELESS, "Update wireless interface %s on device %s\n", vif_new->name, wdev->name);
		wireless_interface_handle_link(vif_old, false);
		free(vif_old->config);
		vif_old->config = blob_memdup(vif_new->config);
		vif_old->isolate = vif_new->isolate;
		vif_old->ap_mode = vif_new->ap_mode;
		wireless_interface_init_config(vif_old);
		free(vif_new);
	} else if (vif_new) {
		D(WIRELESS, "Create new wireless interface %s on device %s\n", vif_new->name, wdev->name);
		vif_new->section = strdup(vif_new->section);
		vif_new->config = blob_memdup(vif_new->config);
		wireless_interface_init_config(vif_new);
	} else if (vif_old) {
		D(WIRELESS, "Delete wireless interface %s on device %s\n", vif_old->name, wdev->name);
		wireless_interface_handle_link(vif_old, false);
		free((void *) vif_old->section);
		free(vif_old->config);
		free(vif_old);
	}

	wdev_set_config_state(wdev, IFC_RELOAD);
}

static void
wireless_proc_poll_fd(struct uloop_fd *fd, unsigned int events)
{
	struct wireless_device *wdev = container_of(fd, struct wireless_device, script_proc_fd);
	char buf[128];

	while (1) {
		int b = read(fd->fd, buf, sizeof(buf));
		if (b < 0) {
			if (errno == EINTR)
				continue;

			if (errno == EAGAIN)
				return;

			goto done;
		}

		if (!b)
			goto done;
	}

done:
	uloop_timeout_set(&wdev->script_check, 0);
	wireless_close_script_proc_fd(wdev);
}

static void
wireless_device_check_script_tasks(struct uloop_timeout *timeout)
{
	struct wireless_device *wdev = container_of(timeout, struct wireless_device, script_check);
	struct wireless_process *proc, *tmp;
	bool restart = false;

	list_for_each_entry_safe(proc, tmp, &wdev->script_proc, list) {
		if (wireless_process_check(proc))
			continue;

		D(WIRELESS, "Wireless device '%s' pid %d has terminated\n", wdev->name, proc->pid);
		if (proc->required)
			restart = true;

		wireless_process_free(wdev, proc);
	}

	if (restart)
		wireless_device_retry_setup(wdev);
	else
		uloop_timeout_set(&wdev->script_check, 1000);
}

void
wireless_device_create(struct wireless_driver *drv, const char *name, struct blob_attr *data)
{
	struct wireless_device *wdev;
	char *name_buf;
	struct blob_attr *disabled;

	blobmsg_parse(&wdev_policy, 1, &disabled, blob_data(data), blob_len(data));

	wdev = calloc_a(sizeof(*wdev), &name_buf, strlen(name) + 1);
	if (disabled && blobmsg_get_bool(disabled))
		wdev->disabled = true;
	wdev->drv = drv;
	wdev->state = IFS_DOWN;
	wdev->config_state = IFC_NORMAL;
	wdev->name = strcpy(name_buf, name);
	wdev->config = data;
	wdev->retry_setup_failed = false;
	wdev->autostart = true;
	INIT_LIST_HEAD(&wdev->script_proc);
	vlist_init(&wdev->interfaces, avl_strcmp, vif_update);
	wdev->interfaces.keep_old = true;

	wdev->timeout.cb = wireless_device_setup_timeout;
	wdev->script_task.cb = wireless_device_script_task_cb;
	wdev->script_task.dir_fd = drv_fd;
	wdev->script_task.log_prefix = wdev->name;

	wdev->script_proc_fd.fd = -1;
	wdev->script_proc_fd.cb = wireless_proc_poll_fd;

	wdev->script_check.cb = wireless_device_check_script_tasks;

	vlist_add(&wireless_devices, &wdev->node, wdev->name);
}

void wireless_interface_create(struct wireless_device *wdev, struct blob_attr *data, const char *section)
{
	struct wireless_interface *vif;
	struct blob_attr *tb[__VIF_ATTR_MAX];
	struct blob_attr *cur;
	char *name_buf;
	char name[8];

	blobmsg_parse(vif_policy, __VIF_ATTR_MAX, tb, blob_data(data), blob_len(data));

	cur = tb[VIF_ATTR_DISABLED];
	if (cur && blobmsg_get_bool(cur))
		return;

	sprintf(name, "%d", wdev->vif_idx++);

	vif = calloc_a(sizeof(*vif),
		       &name_buf, strlen(name) + 1);
	vif->name = strcpy(name_buf, name);
	vif->wdev = wdev;
	vif->config = data;
	vif->section = section;
	vif->isolate = false;

	vlist_add(&wdev->interfaces, &vif->node, vif->name);
}

static void
wireless_interface_status(struct wireless_interface *iface, struct blob_buf *b)
{
	void *i;

	i = blobmsg_open_table(b, NULL);
	if (iface->section)
		blobmsg_add_string(b, "section", iface->section);
	if (iface->ifname)
		blobmsg_add_string(b, "ifname", iface->ifname);
	put_container(b, iface->config, "config");
	blobmsg_close_table(b, i);
}

void
wireless_device_status(struct wireless_device *wdev, struct blob_buf *b)
{
	struct wireless_interface *iface;
	void *c, *i;

	c = blobmsg_open_table(b, wdev->name);
	blobmsg_add_u8(b, "up", wdev->state == IFS_UP);
	blobmsg_add_u8(b, "pending", wdev->state == IFS_SETUP || wdev->state == IFS_TEARDOWN);
	blobmsg_add_u8(b, "autostart", wdev->autostart);
	blobmsg_add_u8(b, "disabled", wdev->disabled);
	blobmsg_add_u8(b, "retry_setup_failed", wdev->retry_setup_failed);
	put_container(b, wdev->config, "config");

	i = blobmsg_open_array(b, "interfaces");
	vlist_for_each_element(&wdev->interfaces, iface, node)
		wireless_interface_status(iface, b);
	blobmsg_close_array(b, i);
	blobmsg_close_table(b, c);
}

void
wireless_device_get_validate(struct wireless_device *wdev, struct blob_buf *b)
{
	struct uci_blob_param_list *p;
	void *c, *d;
	int i;

	c = blobmsg_open_table(b, wdev->name);

	d = blobmsg_open_table(b, "device");
	p = wdev->drv->device.config;
	for (i = 0; i < p->n_params; i++)
		blobmsg_add_string(b, p->params[i].name, uci_get_validate_string(p, i));
	blobmsg_close_table(b, d);

	d = blobmsg_open_table(b, "interface");
	p = wdev->drv->interface.config;
	for (i = 0; i < p->n_params; i++)
		blobmsg_add_string(b, p->params[i].name, uci_get_validate_string(p, i));
	blobmsg_close_table(b, d);

	blobmsg_close_table(b, c);
}

static void
wireless_interface_set_data(struct wireless_interface *vif)
{
	enum {
		VIF_DATA_IFNAME,
		__VIF_DATA_MAX,
	};
	static const struct blobmsg_policy data_policy[__VIF_DATA_MAX] = {
		[VIF_DATA_IFNAME] = { .name = "ifname", .type = BLOBMSG_TYPE_STRING },
	};
	struct blob_attr *tb[__VIF_DATA_MAX];
	struct blob_attr *cur;

	blobmsg_parse(data_policy, __VIF_DATA_MAX, tb,
		      blobmsg_data(vif->data), blobmsg_data_len(vif->data));

	if ((cur = tb[VIF_DATA_IFNAME]))
		vif->ifname = blobmsg_data(cur);
}

static int
wireless_device_add_process(struct wireless_device *wdev, struct blob_attr *data)
{
	enum {
		PROC_ATTR_PID,
		PROC_ATTR_EXE,
		PROC_ATTR_REQUIRED,
		__PROC_ATTR_MAX
	};
	static const struct blobmsg_policy proc_policy[__PROC_ATTR_MAX] = {
		[PROC_ATTR_PID] = { .name = "pid", .type = BLOBMSG_TYPE_INT32 },
		[PROC_ATTR_EXE] = { .name = "exe", .type = BLOBMSG_TYPE_STRING },
		[PROC_ATTR_REQUIRED] = { .name = "required", .type = BLOBMSG_TYPE_BOOL },
	};
	struct blob_attr *tb[__PROC_ATTR_MAX];
	struct wireless_process *proc;
	char *name;
	int pid;

	if (!data)
		return UBUS_STATUS_INVALID_ARGUMENT;

	blobmsg_parse(proc_policy, __PROC_ATTR_MAX, tb, blobmsg_data(data), blobmsg_data_len(data));
	if (!tb[PROC_ATTR_PID] || !tb[PROC_ATTR_EXE])
		return UBUS_STATUS_INVALID_ARGUMENT;

	pid = blobmsg_get_u32(tb[PROC_ATTR_PID]);
	if (pid < 2)
		return UBUS_STATUS_INVALID_ARGUMENT;

	proc = calloc_a(sizeof(*proc),
		&name, strlen(blobmsg_data(tb[PROC_ATTR_EXE])) + 1);

	proc->pid = pid;
	proc->exe = strcpy(name, blobmsg_data(tb[PROC_ATTR_EXE]));

	if (tb[PROC_ATTR_REQUIRED])
		proc->required = blobmsg_get_bool(tb[PROC_ATTR_REQUIRED]);

	D(WIRELESS, "Wireless device '%s' add pid %d\n", wdev->name, proc->pid);
	list_add(&proc->list, &wdev->script_proc);
	uloop_timeout_set(&wdev->script_check, 0);

	return 0;
}

static int
wireless_device_process_kill_all(struct wireless_device *wdev, struct blob_attr *data,
				 struct ubus_request_data *req)
{
	enum {
		KILL_ATTR_SIGNAL,
		KILL_ATTR_IMMEDIATE,
		__KILL_ATTR_MAX
	};
	static const struct blobmsg_policy kill_policy[__KILL_ATTR_MAX] = {
		[KILL_ATTR_SIGNAL] = { .name = "signal", .type = BLOBMSG_TYPE_INT32 },
		[KILL_ATTR_IMMEDIATE] = { .name = "immediate", .type = BLOBMSG_TYPE_BOOL },
	};
	struct blob_attr *tb[__KILL_ATTR_MAX];
	struct blob_attr *cur;
	bool immediate = false;
	int signal = SIGTERM;

	blobmsg_parse(kill_policy, __KILL_ATTR_MAX, tb, blobmsg_data(data), blobmsg_data_len(data));

	if ((cur = tb[KILL_ATTR_SIGNAL]))
		signal = blobmsg_get_u32(cur);

	if ((cur = tb[KILL_ATTR_IMMEDIATE]))
		immediate = blobmsg_get_bool(cur);

	if (wdev->state != IFS_TEARDOWN || wdev->kill_request)
		return UBUS_STATUS_PERMISSION_DENIED;

	wireless_process_kill_all(wdev, signal, immediate);

	if (list_empty(&wdev->script_proc))
		return 0;

	wdev->kill_request = calloc(1, sizeof(*wdev->kill_request));
	ubus_defer_request(ubus_ctx, req, wdev->kill_request);

	return 0;
}

static int
wireless_device_set_retry(struct wireless_device *wdev, struct blob_attr *data)
{
	static const struct blobmsg_policy retry_policy = {
		.name = "retry", .type = BLOBMSG_TYPE_INT32
	};
	struct blob_attr *val;

	blobmsg_parse(&retry_policy, 1, &val, blobmsg_data(data), blobmsg_data_len(data));
	if (!val)
		return UBUS_STATUS_INVALID_ARGUMENT;

	wdev->retry = blobmsg_get_u32(val);
	return 0;
}

enum {
	NOTIFY_CMD_UP = 0,
	NOTIFY_CMD_SET_DATA = 1,
	NOTIFY_CMD_PROCESS_ADD = 2,
	NOTIFY_CMD_PROCESS_KILL_ALL = 3,
	NOTIFY_CMD_SET_RETRY = 4,
};

int
wireless_device_notify(struct wireless_device *wdev, struct blob_attr *data,
		       struct ubus_request_data *req)
{
	enum {
		NOTIFY_ATTR_COMMAND,
		NOTIFY_ATTR_VIF,
		NOTIFY_ATTR_DATA,
		__NOTIFY_MAX,
	};
	static const struct blobmsg_policy notify_policy[__NOTIFY_MAX] = {
		[NOTIFY_ATTR_COMMAND] = { .name = "command", .type = BLOBMSG_TYPE_INT32 },
		[NOTIFY_ATTR_VIF] = { .name = "interface", .type = BLOBMSG_TYPE_STRING },
		[NOTIFY_ATTR_DATA] = { .name = "data", .type = BLOBMSG_TYPE_TABLE },
	};
	struct wireless_interface *vif = NULL;
	struct blob_attr *tb[__NOTIFY_MAX];
	struct blob_attr *cur, **pdata;

	blobmsg_parse(notify_policy, __NOTIFY_MAX, tb, blob_data(data), blob_len(data));

	if (!tb[NOTIFY_ATTR_COMMAND])
		return UBUS_STATUS_INVALID_ARGUMENT;

	if ((cur = tb[NOTIFY_ATTR_VIF]) != NULL) {
		vif = vlist_find(&wdev->interfaces, blobmsg_data(cur), vif, node);
		if (!vif)
			return UBUS_STATUS_NOT_FOUND;
	}

	cur = tb[NOTIFY_ATTR_DATA];
	if (!cur)
		return UBUS_STATUS_INVALID_ARGUMENT;

	switch (blobmsg_get_u32(tb[NOTIFY_ATTR_COMMAND])) {
	case NOTIFY_CMD_UP:
		if (vif)
			return UBUS_STATUS_INVALID_ARGUMENT;

		if (wdev->state != IFS_SETUP)
			return UBUS_STATUS_PERMISSION_DENIED;

		wireless_device_mark_up(wdev);
		break;
	case NOTIFY_CMD_SET_DATA:
		if (vif)
			pdata = &vif->data;
		else
			pdata = &wdev->data;

		if (*pdata)
			return UBUS_STATUS_INVALID_ARGUMENT;

		*pdata = blob_memdup(cur);
		if (vif)
			wireless_interface_set_data(vif);
		break;
	case NOTIFY_CMD_PROCESS_ADD:
		return wireless_device_add_process(wdev, cur);
	case NOTIFY_CMD_PROCESS_KILL_ALL:
		return wireless_device_process_kill_all(wdev, cur, req);
	case NOTIFY_CMD_SET_RETRY:
		return wireless_device_set_retry(wdev, cur);
	default:
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	return 0;
}

void
wireless_start_pending(void)
{
	struct wireless_device *wdev;

	vlist_for_each_element(&wireless_devices, wdev, node)
		__wireless_device_set_up(wdev);
}
