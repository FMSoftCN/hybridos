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

#include <arpa/inet.h>
#include <netinet/in.h>

#include "netifd.h"
#include "interface.h"
#include "interface-ip.h"
#include "proto.h"
#include "system.h"

struct static_proto_state {
	struct interface_proto_state proto;

	struct blob_attr *config;
};

static bool
static_proto_setup(struct static_proto_state *state)
{
	struct interface *iface = state->proto.iface;
	struct device *dev = iface->main_dev.dev;

	interface_set_l3_dev(iface, dev);
	return proto_apply_static_ip_settings(state->proto.iface, state->config) == 0;
}

static int
static_handler(struct interface_proto_state *proto,
	       enum interface_proto_cmd cmd, bool force)
{
	struct static_proto_state *state;
	int ret = 0;

	state = container_of(proto, struct static_proto_state, proto);

	switch (cmd) {
	case PROTO_CMD_SETUP:
		if (!static_proto_setup(state))
			return -1;

		break;
	case PROTO_CMD_TEARDOWN:
	case PROTO_CMD_RENEW:
		break;
	}

	return ret;
}

static void
static_free(struct interface_proto_state *proto)
{
	struct static_proto_state *state;

	state = container_of(proto, struct static_proto_state, proto);
	free(state->config);
	free(state);
}

static struct interface_proto_state *
static_attach(const struct proto_handler *h, struct interface *iface,
	      struct blob_attr *attr)
{
	struct static_proto_state *state;

	state = calloc(1, sizeof(*state));
	if (!state)
		return NULL;

	state->config = malloc(blob_pad_len(attr));
	if (!state->config)
		goto error;

	memcpy(state->config, attr, blob_pad_len(attr));
	state->proto.free = static_free;
	state->proto.cb = static_handler;

	return &state->proto;

error:
	free(state);
	return NULL;
}

static struct proto_handler static_proto = {
	.name = "static",
	.flags = PROTO_FLAG_IMMEDIATE |
		 PROTO_FLAG_FORCE_LINK_DEFAULT,
	.config_params = &proto_ip_attr,
	.attach = static_attach,
};

static void __init
static_proto_init(void)
{
	add_proto_handler(&static_proto);
}
