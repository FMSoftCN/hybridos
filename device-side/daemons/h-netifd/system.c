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
#include "system.h"
#include <fcntl.h>

static const struct blobmsg_policy tunnel_attrs[__TUNNEL_ATTR_MAX] = {
	[TUNNEL_ATTR_TYPE] = { .name = "mode", .type = BLOBMSG_TYPE_STRING },
	[TUNNEL_ATTR_LOCAL] = { .name = "local", .type = BLOBMSG_TYPE_STRING },
	[TUNNEL_ATTR_REMOTE] = { .name = "remote", .type = BLOBMSG_TYPE_STRING },
	[TUNNEL_ATTR_MTU] = { .name = "mtu", .type = BLOBMSG_TYPE_INT32 },
	[TUNNEL_ATTR_DF] = { .name = "df", .type = BLOBMSG_TYPE_BOOL },
	[TUNNEL_ATTR_TTL] = { .name = "ttl", .type = BLOBMSG_TYPE_INT32 },
	[TUNNEL_ATTR_TOS] = { .name = "tos", .type = BLOBMSG_TYPE_STRING },
	[TUNNEL_ATTR_LINK] = { .name = "link", .type = BLOBMSG_TYPE_STRING },
	[TUNNEL_ATTR_DATA] = { .name = "data", .type = BLOBMSG_TYPE_TABLE },
};

const struct uci_blob_param_list tunnel_attr_list = {
	.n_params = __TUNNEL_ATTR_MAX,
	.params = tunnel_attrs,
};

static const struct blobmsg_policy vxlan_data_attrs[__VXLAN_DATA_ATTR_MAX] = {
	[VXLAN_DATA_ATTR_ID] = { .name = "id", .type = BLOBMSG_TYPE_INT32 },
	[VXLAN_DATA_ATTR_PORT] = { .name = "port", .type = BLOBMSG_TYPE_INT32 },
	[VXLAN_DATA_ATTR_MACADDR] = { .name = "macaddr", .type = BLOBMSG_TYPE_STRING },
	[VXLAN_DATA_ATTR_RXCSUM] = { .name = "rxcsum", .type = BLOBMSG_TYPE_BOOL },
	[VXLAN_DATA_ATTR_TXCSUM] = { .name = "txcsum", .type = BLOBMSG_TYPE_BOOL },
};

const struct uci_blob_param_list vxlan_data_attr_list = {
	.n_params = __VXLAN_DATA_ATTR_MAX,
	.params = vxlan_data_attrs,
};

static const struct blobmsg_policy gre_data_attrs[__GRE_DATA_ATTR_MAX] = {
	[GRE_DATA_IKEY] = { .name = "ikey", .type = BLOBMSG_TYPE_INT32 },
	[GRE_DATA_OKEY] = { .name = "okey", .type = BLOBMSG_TYPE_INT32 },
	[GRE_DATA_ICSUM] = { .name = "icsum", .type = BLOBMSG_TYPE_BOOL },
	[GRE_DATA_OCSUM] = { .name = "ocsum", .type = BLOBMSG_TYPE_BOOL },
	[GRE_DATA_ISEQNO] = { .name = "iseqno", .type = BLOBMSG_TYPE_BOOL },
	[GRE_DATA_OSEQNO] = { .name = "oseqno", .type = BLOBMSG_TYPE_BOOL },
};

const struct uci_blob_param_list gre_data_attr_list = {
	.n_params = __GRE_DATA_ATTR_MAX,
	.params = gre_data_attrs,
};

static const struct blobmsg_policy vti_data_attrs[__VTI_DATA_ATTR_MAX] = {
	[VTI_DATA_IKEY] = { .name = "ikey", .type = BLOBMSG_TYPE_INT32 },
	[VTI_DATA_OKEY] = { .name = "okey", .type = BLOBMSG_TYPE_INT32 },
};

const struct uci_blob_param_list vti_data_attr_list = {
	.n_params = __VTI_DATA_ATTR_MAX,
	.params = vti_data_attrs,
};

static const struct blobmsg_policy sixrd_data_attrs[__SIXRD_DATA_ATTR_MAX] = {
	[SIXRD_DATA_PREFIX] = { .name = "prefix", .type = BLOBMSG_TYPE_STRING },
	[SIXRD_DATA_RELAY_PREFIX] = { .name = "relay-prefix", .type = BLOBMSG_TYPE_STRING },
};

const struct uci_blob_param_list sixrd_data_attr_list = {
	.n_params = __SIXRD_DATA_ATTR_MAX,
	.params = sixrd_data_attrs,
};

static const struct blobmsg_policy ipip6_data_attrs[__SIXRD_DATA_ATTR_MAX] = {
	[IPIP6_DATA_ENCAPLIMIT] = { .name = "encaplimit", .type = BLOBMSG_TYPE_STRING },
	[IPIP6_DATA_FMRS] = { .name = "fmrs", .type = BLOBMSG_TYPE_ARRAY },
};

const struct uci_blob_param_list ipip6_data_attr_list = {
	.n_params = __IPIP6_DATA_ATTR_MAX,
	.params = ipip6_data_attrs,
};

static const struct blobmsg_policy fmr_data_attrs[__FMR_DATA_ATTR_MAX] = {
	[FMR_DATA_PREFIX6] = { .name = "prefix6", .type = BLOBMSG_TYPE_STRING },
	[FMR_DATA_PREFIX4] = { .name = "prefix4", .type = BLOBMSG_TYPE_STRING },
	[FMR_DATA_EALEN] = { .name = "ealen", .type = BLOBMSG_TYPE_INT32 },
	[FMR_DATA_OFFSET] = { .name = "offset", .type = BLOBMSG_TYPE_INT32 },
};

const struct uci_blob_param_list fmr_data_attr_list = {
	.n_params = __FMR_DATA_ATTR_MAX,
	.params = fmr_data_attrs,
};

void system_fd_set_cloexec(int fd)
{
#ifdef FD_CLOEXEC
	fcntl(fd, F_SETFD, fcntl(fd, F_GETFD) | FD_CLOEXEC);
#endif
}
