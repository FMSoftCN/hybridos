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
#define _GNU_SOURCE

#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#include "netifd.h"
#include "interface.h"
#include "proto.h"
#include "ubus.h"
#include "system.h"
#include "wireless.h"

struct ubus_context *ubus_ctx = NULL;
static struct blob_buf b;
static const char *ubus_path;

/* global object */

static int
netifd_handle_restart(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	netifd_restart();
	return 0;
}

static int
netifd_handle_reload(struct ubus_context *ctx, struct ubus_object *obj,
		     struct ubus_request_data *req, const char *method,
		     struct blob_attr *msg)
{
	if (netifd_reload())
		return UBUS_STATUS_NOT_FOUND;

	return UBUS_STATUS_OK;
}

enum {
	HR_TARGET,
	HR_V6,
	HR_INTERFACE,
	__HR_MAX
};

static const struct blobmsg_policy route_policy[__HR_MAX] = {
	[HR_TARGET] = { .name = "target", .type = BLOBMSG_TYPE_STRING },
	[HR_V6] = { .name = "v6", .type = BLOBMSG_TYPE_BOOL },
	[HR_INTERFACE] = { .name = "interface", .type = BLOBMSG_TYPE_STRING },
};

static int
netifd_add_host_route(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[__HR_MAX];
	struct interface *iface = NULL;
	union if_addr a;
	bool v6 = false;

	blobmsg_parse(route_policy, __HR_MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[HR_TARGET])
		return UBUS_STATUS_INVALID_ARGUMENT;

	if (tb[HR_V6])
		v6 = blobmsg_get_bool(tb[HR_V6]);

	if (tb[HR_INTERFACE])
		iface = vlist_find(&interfaces, blobmsg_data(tb[HR_INTERFACE]), iface, node);

	memset(&a, 0, sizeof(a));
	if (!inet_pton(v6 ? AF_INET6 : AF_INET, blobmsg_data(tb[HR_TARGET]), &a))
		return UBUS_STATUS_INVALID_ARGUMENT;


	iface = interface_ip_add_target_route(&a, v6, iface);
	if (!iface)
		return UBUS_STATUS_NOT_FOUND;

	blob_buf_init(&b, 0);
	blobmsg_add_string(&b, "interface", iface->name);
	ubus_send_reply(ctx, req, b.head);

	return 0;
}

static int
netifd_get_proto_handlers(struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg)
{
	blob_buf_init(&b, 0);
	proto_dump_handlers(&b);
	ubus_send_reply(ctx, req, b.head);

	return 0;
}


enum {
	DI_NAME,
	__DI_MAX
};

static const struct blobmsg_policy dynamic_policy[__DI_MAX] = {
	[DI_NAME] = { .name = "name", .type = BLOBMSG_TYPE_STRING },
};

static int
netifd_add_dynamic(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[__DI_MAX];
	struct interface *iface;
	struct blob_attr *config;

	blobmsg_parse(dynamic_policy, __DI_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[DI_NAME])
		return UBUS_STATUS_INVALID_ARGUMENT;

	const char *name = blobmsg_get_string(tb[DI_NAME]);

	iface = interface_alloc(name, msg);
	if (!iface)
		return UBUS_STATUS_UNKNOWN_ERROR;

	config = blob_memdup(msg);
	if (!config)
		goto error;

	interface_add(iface, config);

	// need to look up the interface name again, in case of config update
	// the pointer will have changed
	iface = vlist_find(&interfaces, name, iface, node);
	if (!iface)
		return UBUS_STATUS_UNKNOWN_ERROR;

	// Set interface as dynamic
	interface_set_dynamic(iface);

	return UBUS_STATUS_OK;

error:
	free(iface);
	return UBUS_STATUS_UNKNOWN_ERROR;
}

static struct ubus_method main_object_methods[] = {
	{ .name = "restart", .handler = netifd_handle_restart },
	{ .name = "reload", .handler = netifd_handle_reload },
	UBUS_METHOD("add_host_route", netifd_add_host_route, route_policy),
	{ .name = "get_proto_handlers", .handler = netifd_get_proto_handlers },
	UBUS_METHOD("add_dynamic", netifd_add_dynamic, dynamic_policy),
};

static struct ubus_object_type main_object_type =
	UBUS_OBJECT_TYPE("netifd", main_object_methods);

static struct ubus_object main_object = {
	.name = "network",
	.type = &main_object_type,
	.methods = main_object_methods,
	.n_methods = ARRAY_SIZE(main_object_methods),
};

enum {
	DEV_NAME,
	__DEV_MAX,
};

static const struct blobmsg_policy dev_policy[__DEV_MAX] = {
	[DEV_NAME] = { .name = "name", .type = BLOBMSG_TYPE_STRING },
};

static int
netifd_dev_status(struct ubus_context *ctx, struct ubus_object *obj,
		  struct ubus_request_data *req, const char *method,
		  struct blob_attr *msg)
{
	struct device *dev = NULL;
	struct blob_attr *tb[__DEV_MAX];

	blobmsg_parse(dev_policy, __DEV_MAX, tb, blob_data(msg), blob_len(msg));

	if (tb[DEV_NAME]) {
		dev = device_find(blobmsg_data(tb[DEV_NAME]));
		if (!dev)
			return UBUS_STATUS_INVALID_ARGUMENT;
	}

	blob_buf_init(&b, 0);
	device_dump_status(&b, dev);
	ubus_send_reply(ctx, req, b.head);

	return 0;
}

enum {
	ALIAS_ATTR_ALIAS,
	ALIAS_ATTR_DEV,
	__ALIAS_ATTR_MAX,
};

static const struct blobmsg_policy alias_attrs[__ALIAS_ATTR_MAX] = {
	[ALIAS_ATTR_ALIAS] = { "alias", BLOBMSG_TYPE_ARRAY },
	[ALIAS_ATTR_DEV] = { "device", BLOBMSG_TYPE_STRING },
};

static int
netifd_handle_alias(struct ubus_context *ctx, struct ubus_object *obj,
		    struct ubus_request_data *req, const char *method,
		    struct blob_attr *msg)
{
	struct device *dev = NULL;
	struct blob_attr *tb[__ALIAS_ATTR_MAX];
	struct blob_attr *cur;
	int rem;

	blobmsg_parse(alias_attrs, __ALIAS_ATTR_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[ALIAS_ATTR_ALIAS])
		return UBUS_STATUS_INVALID_ARGUMENT;

	if ((cur = tb[ALIAS_ATTR_DEV]) != NULL) {
		dev = device_get(blobmsg_data(cur), true);
		if (!dev)
			return UBUS_STATUS_NOT_FOUND;
	}

	blobmsg_for_each_attr(cur, tb[ALIAS_ATTR_ALIAS], rem) {
		if (blobmsg_type(cur) != BLOBMSG_TYPE_STRING)
			goto error;

		if (!blobmsg_check_attr(cur, NULL))
			goto error;

		alias_notify_device(blobmsg_data(cur), dev);
	}
	return 0;

error:
	device_free_unused(dev);
	return UBUS_STATUS_INVALID_ARGUMENT;
}

enum {
	DEV_STATE_NAME,
	DEV_STATE_DEFER,
	__DEV_STATE_MAX,
};

static const struct blobmsg_policy dev_state_policy[__DEV_STATE_MAX] = {
	[DEV_STATE_NAME] = { .name = "name", .type = BLOBMSG_TYPE_STRING },
	[DEV_STATE_DEFER] = { .name = "defer", .type = BLOBMSG_TYPE_BOOL },
};

static int
netifd_handle_set_state(struct ubus_context *ctx, struct ubus_object *obj,
			struct ubus_request_data *req, const char *method,
			struct blob_attr *msg)
{
	struct device *dev = NULL;
	struct blob_attr *tb[__DEV_STATE_MAX];
	struct blob_attr *cur;

	blobmsg_parse(dev_state_policy, __DEV_STATE_MAX, tb, blob_data(msg), blob_len(msg));

	cur = tb[DEV_STATE_NAME];
	if (!cur)
		return UBUS_STATUS_INVALID_ARGUMENT;

	dev = device_find(blobmsg_data(cur));
	if (!dev)
		return UBUS_STATUS_NOT_FOUND;

	cur = tb[DEV_STATE_DEFER];
	if (cur)
		device_set_deferred(dev, !!blobmsg_get_u8(cur));

	return 0;
}

static struct ubus_method dev_object_methods[] = {
	UBUS_METHOD("status", netifd_dev_status, dev_policy),
	UBUS_METHOD("set_alias", netifd_handle_alias, alias_attrs),
	UBUS_METHOD("set_state", netifd_handle_set_state, dev_state_policy),
};

static struct ubus_object_type dev_object_type =
	UBUS_OBJECT_TYPE("device", dev_object_methods);

static struct ubus_object dev_object = {
	.name = "network.device",
	.type = &dev_object_type,
	.methods = dev_object_methods,
	.n_methods = ARRAY_SIZE(dev_object_methods),
};

static void
netifd_ubus_add_fd(void)
{
	ubus_add_uloop(ubus_ctx);
	system_fd_set_cloexec(ubus_ctx->sock.fd);
}

static void
netifd_ubus_reconnect_timer(struct uloop_timeout *timeout)
{
	static struct uloop_timeout retry = {
		.cb = netifd_ubus_reconnect_timer,
	};
	int t = 2;

	if (ubus_reconnect(ubus_ctx, ubus_path) != 0) {
		DPRINTF("failed to reconnect, trying again in %d seconds\n", t);
		uloop_timeout_set(&retry, t * 1000);
		return;
	}

	DPRINTF("reconnected to ubus, new id: %08x\n", ubus_ctx->local_id);
	netifd_ubus_add_fd();
}

static void
netifd_ubus_connection_lost(struct ubus_context *ctx)
{
	netifd_ubus_reconnect_timer(NULL);
}

/* per-interface object */

static int
netifd_handle_up(struct ubus_context *ctx, struct ubus_object *obj,
		 struct ubus_request_data *req, const char *method,
		 struct blob_attr *msg)
{
	struct interface *iface;

	iface = container_of(obj, struct interface, ubus);
	interface_set_up(iface);

	return 0;
}

static int
netifd_handle_down(struct ubus_context *ctx, struct ubus_object *obj,
		   struct ubus_request_data *req, const char *method,
		   struct blob_attr *msg)
{
	struct interface *iface;

	iface = container_of(obj, struct interface, ubus);
	interface_set_down(iface);

	return 0;
}

static int
netifd_handle_renew(struct ubus_context *ctx, struct ubus_object *obj,
		   struct ubus_request_data *req, const char *method,
		   struct blob_attr *msg)
{
	struct interface *iface;

	iface = container_of(obj, struct interface, ubus);
	interface_renew(iface);

	return 0;
}

static void
netifd_add_interface_errors(struct blob_buf *b, struct interface *iface)
{
	struct interface_error *error;
	void *e, *e2, *e3;
	int i;

	e = blobmsg_open_array(b, "errors");
	list_for_each_entry(error, &iface->errors, list) {
		e2 = blobmsg_open_table(b, NULL);

		blobmsg_add_string(b, "subsystem", error->subsystem);
		blobmsg_add_string(b, "code", error->code);
		if (error->data[0]) {
			e3 = blobmsg_open_array(b, "data");
			for (i = 0; error->data[i]; i++)
				blobmsg_add_string(b, NULL, error->data[i]);
			blobmsg_close_array(b, e3);
		}

		blobmsg_close_table(b, e2);
	}
	blobmsg_close_array(b, e);
}

static void
interface_ip_dump_address_list(struct interface_ip_settings *ip, bool v6, bool enabled)
{
	struct device_addr *addr;
	char *buf;
	void *a;
	int buflen = 128;
	int af;

	time_t now = system_get_rtime();
	vlist_for_each_element(&ip->addr, addr, node) {
		if (addr->enabled != enabled)
			continue;

		if ((addr->flags & DEVADDR_FAMILY) == DEVADDR_INET4)
			af = AF_INET;
		else
			af = AF_INET6;

		if (af != (v6 ? AF_INET6 : AF_INET))
			continue;

		a = blobmsg_open_table(&b, NULL);

		buf = blobmsg_alloc_string_buffer(&b, "address", buflen);
		inet_ntop(af, &addr->addr, buf, buflen);
		blobmsg_add_string_buffer(&b);

		blobmsg_add_u32(&b, "mask", addr->mask);

		if (addr->point_to_point) {
			buf = blobmsg_alloc_string_buffer(&b, "ptpaddress", buflen);
			inet_ntop(af, &addr->point_to_point, buf, buflen);
			blobmsg_add_string_buffer(&b);
		}

		if (addr->preferred_until) {
			int preferred = addr->preferred_until - now;
			if (preferred < 0)
				preferred = 0;
			blobmsg_add_u32(&b, "preferred", preferred);
		}

		if (addr->valid_until)
			blobmsg_add_u32(&b, "valid", addr->valid_until - now);

		if (addr->pclass)
			blobmsg_add_string(&b, "class", addr->pclass);

		blobmsg_close_table(&b, a);
	}
}

static void
interface_ip_dump_route_list(struct interface_ip_settings *ip, bool enabled)
{
	struct device_route *route;
	int buflen = 128;
	char *buf;
	void *r;
	int af;

	time_t now = system_get_rtime();
	vlist_for_each_element(&ip->route, route, node) {
		if (route->enabled != enabled)
			continue;

		if ((ip->no_defaultroute == enabled) && !route->mask)
			continue;

		if ((route->flags & DEVADDR_FAMILY) == DEVADDR_INET4)
			af = AF_INET;
		else
			af = AF_INET6;

		r = blobmsg_open_table(&b, NULL);

		buf = blobmsg_alloc_string_buffer(&b, "target", buflen);
		inet_ntop(af, &route->addr, buf, buflen);
		blobmsg_add_string_buffer(&b);

		blobmsg_add_u32(&b, "mask", route->mask);

		buf = blobmsg_alloc_string_buffer(&b, "nexthop", buflen);
		inet_ntop(af, &route->nexthop, buf, buflen);
		blobmsg_add_string_buffer(&b);

		if (route->flags & DEVROUTE_TYPE)
			blobmsg_add_u32(&b, "type", route->type);

		if (route->flags & DEVROUTE_PROTO)
			blobmsg_add_u32(&b, "proto", route->proto);

		if (route->flags & DEVROUTE_MTU)
			blobmsg_add_u32(&b, "mtu", route->mtu);

		if (route->flags & DEVROUTE_METRIC)
			blobmsg_add_u32(&b, "metric", route->metric);

		if (route->flags & DEVROUTE_TABLE)
			blobmsg_add_u32(&b, "table", route->table);

		if (route->valid_until)
			blobmsg_add_u32(&b, "valid", route->valid_until - now);

		buf = blobmsg_alloc_string_buffer(&b, "source", buflen);
		inet_ntop(af, &route->source, buf, buflen);
		snprintf(buf + strlen(buf), buflen - strlen(buf), "/%u", route->sourcemask);
		blobmsg_add_string_buffer(&b);

		blobmsg_close_table(&b, r);
	}
}


static void
interface_ip_dump_prefix_list(struct interface_ip_settings *ip)
{
	struct device_prefix *prefix;
	char *buf;
	void *a, *c;
	const int buflen = INET6_ADDRSTRLEN;

	time_t now = system_get_rtime();
	vlist_for_each_element(&ip->prefix, prefix, node) {
		a = blobmsg_open_table(&b, NULL);

		buf = blobmsg_alloc_string_buffer(&b, "address", buflen);
		inet_ntop(AF_INET6, &prefix->addr, buf, buflen);
		blobmsg_add_string_buffer(&b);

		blobmsg_add_u32(&b, "mask", prefix->length);

		if (prefix->preferred_until) {
			int preferred = prefix->preferred_until - now;
			if (preferred < 0)
				preferred = 0;
			blobmsg_add_u32(&b, "preferred", preferred);
		}

		if (prefix->valid_until)
			blobmsg_add_u32(&b, "valid", prefix->valid_until - now);

		blobmsg_add_string(&b, "class", prefix->pclass);

		c = blobmsg_open_table(&b, "assigned");
		struct device_prefix_assignment *assign;
		list_for_each_entry(assign, &prefix->assignments, head) {
			if (!assign->name[0])
				continue;

			struct in6_addr addr = prefix->addr;
			addr.s6_addr32[1] |= htonl(assign->assigned);

			void *d = blobmsg_open_table(&b, assign->name);

			buf = blobmsg_alloc_string_buffer(&b, "address", buflen);
			inet_ntop(AF_INET6, &addr, buf, buflen);
			blobmsg_add_string_buffer(&b);

			blobmsg_add_u32(&b, "mask", assign->length);

			blobmsg_close_table(&b, d);
		}
		blobmsg_close_table(&b, c);

		blobmsg_close_table(&b, a);
	}
}


static void
interface_ip_dump_prefix_assignment_list(struct interface *iface)
{
	void *a;
	char *buf;
	const int buflen = INET6_ADDRSTRLEN;
	time_t now = system_get_rtime();

	struct device_prefix *prefix;
	list_for_each_entry(prefix, &prefixes, head) {
		struct device_prefix_assignment *assign;
		list_for_each_entry(assign, &prefix->assignments, head) {
			if (strcmp(assign->name, iface->name))
				continue;

			struct in6_addr addr = prefix->addr;
			addr.s6_addr32[1] |= htonl(assign->assigned);

			a = blobmsg_open_table(&b, NULL);

			buf = blobmsg_alloc_string_buffer(&b, "address", buflen);
			inet_ntop(AF_INET6, &addr, buf, buflen);
			blobmsg_add_string_buffer(&b);

			blobmsg_add_u32(&b, "mask", assign->length);

			if (prefix->preferred_until) {
				int preferred = prefix->preferred_until - now;
				if (preferred < 0)
					preferred = 0;
				blobmsg_add_u32(&b, "preferred", preferred);
			}

			if (prefix->valid_until)
				blobmsg_add_u32(&b, "valid", prefix->valid_until - now);

			void *c = blobmsg_open_table(&b, "local-address");
			if (assign->enabled) {
				buf = blobmsg_alloc_string_buffer(&b, "address", buflen);
				inet_ntop(AF_INET6, &assign->addr, buf, buflen);
				blobmsg_add_string_buffer(&b);

				blobmsg_add_u32(&b, "mask", assign->length);
			}
			blobmsg_close_table(&b, c);

			blobmsg_close_table(&b, a);
		}
	}
}

static void
interface_ip_dump_dns_server_list(struct interface_ip_settings *ip, bool enabled)
{
	struct dns_server *dns;
	int buflen = 128;
	char *buf;

	vlist_simple_for_each_element(&ip->dns_servers, dns, node) {
		if (ip->no_dns == enabled)
			continue;

		buf = blobmsg_alloc_string_buffer(&b, NULL, buflen);
		inet_ntop(dns->af, &dns->addr, buf, buflen);
		blobmsg_add_string_buffer(&b);
	}
}

static void
interface_ip_dump_dns_search_list(struct interface_ip_settings *ip, bool enabled)
{
	struct dns_search_domain *dns;

	vlist_simple_for_each_element(&ip->dns_search, dns, node) {
		if (ip->no_dns == enabled)
			continue;

		blobmsg_add_string(&b, NULL, dns->name);
	}
}

static void
netifd_dump_status(struct interface *iface)
{
	struct interface_data *data;
	struct device *dev;
	void *a, *inactive;

	blobmsg_add_u8(&b, "up", iface->state == IFS_UP);
	blobmsg_add_u8(&b, "pending", iface->state == IFS_SETUP);
	blobmsg_add_u8(&b, "available", iface->available);
	blobmsg_add_u8(&b, "autostart", iface->autostart);
	blobmsg_add_u8(&b, "dynamic", iface->dynamic);

	if (iface->state == IFS_UP) {
		time_t cur = system_get_rtime();
		blobmsg_add_u32(&b, "uptime", cur - iface->start_time);
		if (iface->l3_dev.dev)
			blobmsg_add_string(&b, "l3_device", iface->l3_dev.dev->ifname);
	}

	if (iface->proto_handler)
		blobmsg_add_string(&b, "proto", iface->proto_handler->name);

	dev = iface->main_dev.dev;
	if (dev && !dev->hidden && iface->proto_handler &&
	    !(iface->proto_handler->flags & PROTO_FLAG_NODEV))
		blobmsg_add_string(&b, "device", dev->ifname);

	if (iface->state == IFS_UP) {
		if (iface->updated) {
			a = blobmsg_open_array(&b, "updated");

			if (iface->updated & IUF_ADDRESS)
				blobmsg_add_string(&b, NULL, "addresses");
			if (iface->updated & IUF_ROUTE)
				blobmsg_add_string(&b, NULL, "routes");
			if (iface->updated & IUF_PREFIX)
				blobmsg_add_string(&b, NULL, "prefixes");
			if (iface->updated & IUF_DATA)
				blobmsg_add_string(&b, NULL, "data");

			blobmsg_close_array(&b, a);
		}

		if (iface->ip4table)
			blobmsg_add_u32(&b, "ip4table", iface->ip4table);
		if (iface->ip6table)
			blobmsg_add_u32(&b, "ip6table", iface->ip6table);
		blobmsg_add_u32(&b, "metric", iface->metric);
		blobmsg_add_u32(&b, "dns_metric", iface->dns_metric);
		blobmsg_add_u8(&b, "delegation", !iface->proto_ip.no_delegation);
		if (iface->assignment_weight)
			blobmsg_add_u32(&b, "ip6weight", iface->assignment_weight);
		a = blobmsg_open_array(&b, "ipv4-address");
		interface_ip_dump_address_list(&iface->config_ip, false, true);
		interface_ip_dump_address_list(&iface->proto_ip, false, true);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "ipv6-address");
		interface_ip_dump_address_list(&iface->config_ip, true, true);
		interface_ip_dump_address_list(&iface->proto_ip, true, true);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "ipv6-prefix");
		interface_ip_dump_prefix_list(&iface->config_ip);
		interface_ip_dump_prefix_list(&iface->proto_ip);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "ipv6-prefix-assignment");
		interface_ip_dump_prefix_assignment_list(iface);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "route");
		interface_ip_dump_route_list(&iface->config_ip, true);
		interface_ip_dump_route_list(&iface->proto_ip, true);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "dns-server");
		interface_ip_dump_dns_server_list(&iface->config_ip, true);
		interface_ip_dump_dns_server_list(&iface->proto_ip, true);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "dns-search");
		interface_ip_dump_dns_search_list(&iface->config_ip, true);
		interface_ip_dump_dns_search_list(&iface->proto_ip, true);
		blobmsg_close_array(&b, a);

		inactive = blobmsg_open_table(&b, "inactive");
		a = blobmsg_open_array(&b, "ipv4-address");
		interface_ip_dump_address_list(&iface->config_ip, false, false);
		interface_ip_dump_address_list(&iface->proto_ip, false, false);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "ipv6-address");
		interface_ip_dump_address_list(&iface->config_ip, true, false);
		interface_ip_dump_address_list(&iface->proto_ip, true, false);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "route");
		interface_ip_dump_route_list(&iface->config_ip, false);
		interface_ip_dump_route_list(&iface->proto_ip, false);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "dns-server");
		interface_ip_dump_dns_server_list(&iface->config_ip, false);
		interface_ip_dump_dns_server_list(&iface->proto_ip, false);
		blobmsg_close_array(&b, a);
		a = blobmsg_open_array(&b, "dns-search");
		interface_ip_dump_dns_search_list(&iface->config_ip, false);
		interface_ip_dump_dns_search_list(&iface->proto_ip, false);
		blobmsg_close_array(&b, a);
		blobmsg_close_table(&b, inactive);
	}

	a = blobmsg_open_table(&b, "data");
	avl_for_each_element(&iface->data, data, node)
		blobmsg_add_blob(&b, data->data);

	blobmsg_close_table(&b, a);

	if (!list_empty(&iface->errors))
		netifd_add_interface_errors(&b, iface);
}

static int
netifd_handle_status(struct ubus_context *ctx, struct ubus_object *obj,
		     struct ubus_request_data *req, const char *method,
		     struct blob_attr *msg)
{
	struct interface *iface = container_of(obj, struct interface, ubus);

	blob_buf_init(&b, 0);
	netifd_dump_status(iface);
	ubus_send_reply(ctx, req, b.head);

	return 0;
}


static int
netifd_handle_dump(struct ubus_context *ctx, struct ubus_object *obj,
		     struct ubus_request_data *req, const char *method,
		     struct blob_attr *msg)
{
	blob_buf_init(&b, 0);
	void *a = blobmsg_open_array(&b, "interface");

	struct interface *iface;
	vlist_for_each_element(&interfaces, iface, node) {
		void *i = blobmsg_open_table(&b, NULL);
		blobmsg_add_string(&b, "interface", iface->name);
		netifd_dump_status(iface);
		blobmsg_close_table(&b, i);
	}

	blobmsg_close_array(&b, a);
	ubus_send_reply(ctx, req, b.head);

	return 0;
}

enum {
	DEV_LINK_NAME,
	DEV_LINK_EXT,
	__DEV_LINK_MAX,
};

static const struct blobmsg_policy dev_link_policy[__DEV_LINK_MAX] = {
	[DEV_LINK_NAME] = { .name = "name", .type = BLOBMSG_TYPE_STRING },
	[DEV_LINK_EXT] = { .name = "link-ext", .type = BLOBMSG_TYPE_BOOL },
};

static int
netifd_iface_handle_device(struct ubus_context *ctx, struct ubus_object *obj,
			   struct ubus_request_data *req, const char *method,
			   struct blob_attr *msg)
{
	struct blob_attr *tb[__DEV_LINK_MAX];
	struct blob_attr *cur;
	struct interface *iface;
	bool add = !strncmp(method, "add", 3);
	bool link_ext = true;

	iface = container_of(obj, struct interface, ubus);

	blobmsg_parse(dev_link_policy, __DEV_LINK_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[DEV_LINK_NAME])
		return UBUS_STATUS_INVALID_ARGUMENT;

	cur = tb[DEV_LINK_EXT];
	if (cur)
		link_ext = blobmsg_get_bool(cur);

	return interface_handle_link(iface, blobmsg_data(tb[DEV_LINK_NAME]), add, link_ext);
}


static int
netifd_iface_notify_proto(struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg)
{
	struct interface *iface;

	iface = container_of(obj, struct interface, ubus);

	if (!iface->proto || !iface->proto->notify)
		return UBUS_STATUS_NOT_SUPPORTED;

	return iface->proto->notify(iface->proto, msg);
}

static void
netifd_iface_do_remove(struct uloop_timeout *timeout)
{
	struct interface *iface;

	iface = container_of(timeout, struct interface, remove_timer);
	vlist_delete(&interfaces, &iface->node);
}

static int
netifd_iface_remove(struct ubus_context *ctx, struct ubus_object *obj,
		    struct ubus_request_data *req, const char *method,
		    struct blob_attr *msg)
{
	struct interface *iface;

	iface = container_of(obj, struct interface, ubus);
	if (iface->remove_timer.cb)
		return UBUS_STATUS_INVALID_ARGUMENT;

	iface->remove_timer.cb = netifd_iface_do_remove;
	uloop_timeout_set(&iface->remove_timer, 100);
	return 0;
}

static int
netifd_handle_iface_prepare(struct ubus_context *ctx, struct ubus_object *obj,
			    struct ubus_request_data *req, const char *method,
			    struct blob_attr *msg)
{
	struct interface *iface;
	struct device *dev;
	const struct device_hotplug_ops *ops;

	iface = container_of(obj, struct interface, ubus);
	dev = iface->main_dev.dev;
	if (!dev)
		return 0;

	ops = dev->hotplug_ops;
	if (!ops)
		return 0;

	return ops->prepare(dev);
}

static int
netifd_handle_set_data(struct ubus_context *ctx, struct ubus_object *obj,
		       struct ubus_request_data *req, const char *method,
		       struct blob_attr *msg)
{
	struct interface *iface;

	iface = container_of(obj, struct interface, ubus);

	return interface_parse_data(iface, msg);
}

static struct ubus_method iface_object_methods[] = {
	{ .name = "up", .handler = netifd_handle_up },
	{ .name = "down", .handler = netifd_handle_down },
	{ .name = "renew", .handler = netifd_handle_renew },
	{ .name = "status", .handler = netifd_handle_status },
	{ .name = "prepare", .handler = netifd_handle_iface_prepare },
	{ .name = "dump", .handler = netifd_handle_dump },
	UBUS_METHOD("add_device", netifd_iface_handle_device, dev_link_policy ),
	UBUS_METHOD("remove_device", netifd_iface_handle_device, dev_link_policy ),
	{ .name = "notify_proto", .handler = netifd_iface_notify_proto },
	{ .name = "remove", .handler = netifd_iface_remove },
	{ .name = "set_data", .handler = netifd_handle_set_data },
};

static struct ubus_object_type iface_object_type =
	UBUS_OBJECT_TYPE("netifd_iface", iface_object_methods);


static struct ubus_object iface_object = {
	.name = "network.interface",
	.type = &iface_object_type,
	.n_methods = ARRAY_SIZE(iface_object_methods),
};

static void netifd_add_object(struct ubus_object *obj)
{
	int ret = ubus_add_object(ubus_ctx, obj);

	if (ret != 0)
		fprintf(stderr, "Failed to publish object '%s': %s\n", obj->name, ubus_strerror(ret));
}

static const struct blobmsg_policy iface_policy = {
	.name = "interface",
	.type = BLOBMSG_TYPE_STRING,
};

static int
netifd_handle_iface(struct ubus_context *ctx, struct ubus_object *obj,
		    struct ubus_request_data *req, const char *method,
		    struct blob_attr *msg)
{
	struct interface *iface;
	struct blob_attr *tb;
	int i;

	blobmsg_parse(&iface_policy, 1, &tb, blob_data(msg), blob_len(msg));
	if (!tb)
		return UBUS_STATUS_INVALID_ARGUMENT;

	iface = vlist_find(&interfaces, blobmsg_data(tb), iface, node);
	if (!iface)
		return UBUS_STATUS_NOT_FOUND;

	for (i = 0; i < ARRAY_SIZE(iface_object_methods); i++) {
		ubus_handler_t cb;

		if (strcmp(method, iface_object_methods[i].name) != 0)
			continue;

		cb = iface_object_methods[i].handler;
		return cb(ctx, &iface->ubus, req, method, msg);
	}

	return UBUS_STATUS_INVALID_ARGUMENT;
}

static void netifd_add_iface_object(void)
{
	struct ubus_method *methods;
	int i;

	methods = calloc(1, sizeof(iface_object_methods));
	if (!methods)
		return;

	memcpy(methods, iface_object_methods, sizeof(iface_object_methods));
	iface_object.methods = methods;

	for (i = 0; i < ARRAY_SIZE(iface_object_methods); i++) {
		if (methods[i].handler == netifd_handle_dump)
			continue;

		methods[i].handler = netifd_handle_iface;
		methods[i].policy = &iface_policy;
		methods[i].n_policy = 1;
	}
	netifd_add_object(&iface_object);
}

static struct wireless_device *
get_wdev(struct blob_attr *msg, int *ret)
{
	struct blobmsg_policy wdev_policy = {
		.name = "device",
		.type = BLOBMSG_TYPE_STRING,
	};
	struct blob_attr *dev_attr;
	struct wireless_device *wdev = NULL;


	blobmsg_parse(&wdev_policy, 1, &dev_attr, blob_data(msg), blob_len(msg));
	if (!dev_attr) {
		*ret = UBUS_STATUS_INVALID_ARGUMENT;
		return NULL;
	}

	wdev = vlist_find(&wireless_devices, blobmsg_data(dev_attr), wdev, node);
	if (!wdev) {
		*ret = UBUS_STATUS_NOT_FOUND;
		return NULL;
	}

	*ret = 0;
	return wdev;
}

static int
netifd_handle_wdev_up(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct wireless_device *wdev;
	int ret;

	wdev = get_wdev(msg, &ret);
	if (ret == UBUS_STATUS_NOT_FOUND)
		return ret;

	if (wdev) {
		wireless_device_set_up(wdev);
	} else {
		vlist_for_each_element(&wireless_devices, wdev, node)
			wireless_device_set_up(wdev);
	}

	return 0;
}

static int
netifd_handle_wdev_down(struct ubus_context *ctx, struct ubus_object *obj,
			struct ubus_request_data *req, const char *method,
			struct blob_attr *msg)
{
	struct wireless_device *wdev;
	int ret;

	wdev = get_wdev(msg, &ret);
	if (ret == UBUS_STATUS_NOT_FOUND)
		return ret;

	if (wdev) {
		wireless_device_set_down(wdev);
	} else {
		vlist_for_each_element(&wireless_devices, wdev, node)
			wireless_device_set_down(wdev);
	}

	return 0;
}

static int
netifd_handle_wdev_status(struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg)
{
	struct wireless_device *wdev;
	int ret;

	wdev = get_wdev(msg, &ret);
	if (ret == UBUS_STATUS_NOT_FOUND)
		return ret;

	blob_buf_init(&b, 0);
	if (wdev) {
		wireless_device_status(wdev, &b);
	} else {
		vlist_for_each_element(&wireless_devices, wdev, node)
			wireless_device_status(wdev, &b);
	}
	ubus_send_reply(ctx, req, b.head);
	return 0;
}

static int
netifd_handle_wdev_get_validate(struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg)
{
	struct wireless_device *wdev;
	int ret;

	wdev = get_wdev(msg, &ret);
	if (ret == UBUS_STATUS_NOT_FOUND)
		return ret;

	blob_buf_init(&b, 0);
	if (wdev) {
		wireless_device_get_validate(wdev, &b);
	} else {
		vlist_for_each_element(&wireless_devices, wdev, node)
			wireless_device_get_validate(wdev, &b);
	}
	ubus_send_reply(ctx, req, b.head);
	return 0;
}

static int
netifd_handle_wdev_notify(struct ubus_context *ctx, struct ubus_object *obj,
			  struct ubus_request_data *req, const char *method,
			  struct blob_attr *msg)
{
	struct wireless_device *wdev;
	int ret;

	wdev = get_wdev(msg, &ret);
	if (!wdev)
		return ret;

	return wireless_device_notify(wdev, msg, req);
}

static struct ubus_method wireless_object_methods[] = {
	{ .name = "up", .handler = netifd_handle_wdev_up },
	{ .name = "down", .handler = netifd_handle_wdev_down },
	{ .name = "status", .handler = netifd_handle_wdev_status },
	{ .name = "notify", .handler = netifd_handle_wdev_notify },
	{ .name = "get_validate", .handler = netifd_handle_wdev_get_validate },
};

static struct ubus_object_type wireless_object_type =
	UBUS_OBJECT_TYPE("netifd_iface", wireless_object_methods);


static struct ubus_object wireless_object = {
	.name = "network.wireless",
	.type = &wireless_object_type,
	.methods = wireless_object_methods,
	.n_methods = ARRAY_SIZE(wireless_object_methods),
};

int
netifd_ubus_init(const char *path)
{
	uloop_init();
	ubus_path = path;

	ubus_ctx = ubus_connect(path);
	if (!ubus_ctx)
		return -EIO;

	DPRINTF("connected as %08x\n", ubus_ctx->local_id);
	ubus_ctx->connection_lost = netifd_ubus_connection_lost;
	netifd_ubus_add_fd();

	netifd_add_object(&main_object);
	netifd_add_object(&dev_object);
	netifd_add_object(&wireless_object);
	netifd_add_iface_object();

	return 0;
}

void
netifd_ubus_done(void)
{
	ubus_free(ubus_ctx);
}

void
netifd_ubus_interface_event(struct interface *iface, bool up)
{
	blob_buf_init(&b, 0);
	blobmsg_add_string(&b, "action", up ? "ifup" : "ifdown");
	blobmsg_add_string(&b, "interface", iface->name);
	ubus_send_event(ubus_ctx, "network.interface", b.head);
}

void
netifd_ubus_interface_notify(struct interface *iface, bool up)
{
	const char *event = (up) ? "interface.update" : "interface.down";
	blob_buf_init(&b, 0);
	blobmsg_add_string(&b, "interface", iface->name);
	netifd_dump_status(iface);
	ubus_notify(ubus_ctx, &iface_object, event, b.head, -1);
	ubus_notify(ubus_ctx, &iface->ubus, event, b.head, -1);
}

void
netifd_ubus_add_interface(struct interface *iface)
{
	struct ubus_object *obj = &iface->ubus;
	char *name = NULL;

	if (asprintf(&name, "%s.interface.%s", main_object.name, iface->name) == -1)
		return;

	obj->name = name;
	obj->type = &iface_object_type;
	obj->methods = iface_object_methods;
	obj->n_methods = ARRAY_SIZE(iface_object_methods);
	if (ubus_add_object(ubus_ctx, &iface->ubus)) {
		DPRINTF("failed to publish ubus object for interface '%s'\n", iface->name);
		free(name);
		obj->name = NULL;
	}
}

void
netifd_ubus_remove_interface(struct interface *iface)
{
	if (!iface->ubus.name)
		return;

	ubus_remove_object(ubus_ctx, &iface->ubus);
	free((void *) iface->ubus.name);
}
