/*
 * Copyright (C) 2011-2014 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __UBUSD_H
#define __UBUSD_H

#include <libubox/list.h>
#include <libubox/uloop.h>
#include <libubox/blobmsg.h>
#include "ubus_common.h"
#include "ubusd_id.h"
#include "ubusd_obj.h"
#include "ubusmsg.h"
#include "ubusd_acl.h"

#define UBUSD_CLIENT_BACKLOG	32
#define UBUS_OBJ_HASH_BITS	4

extern struct blob_buf b;

struct ubus_msg_buf {
	uint32_t refcount; /* ~0: uses external data buffer */
	struct ubus_msghdr hdr;
	struct blob_attr *data;
	int fd;
	int len;
};

struct ubus_client {
	struct ubus_id id;
	struct uloop_fd sock;
	struct blob_buf b;

	uid_t uid;
	gid_t gid;
	char *user;
	char *group;

	struct list_head objects;

	struct ubus_msg_buf *tx_queue[UBUSD_CLIENT_BACKLOG];
	unsigned int txq_cur, txq_tail, txq_ofs;

	struct ubus_msg_buf *pending_msg;
	struct ubus_msg_buf *retmsg;
	int pending_msg_offset;
	int pending_msg_fd;
	struct {
		struct ubus_msghdr hdr;
		struct blob_attr data;
	} hdrbuf;
};

struct ubus_path {
	struct list_head list;
	const char name[];
};

extern const char *ubusd_acl_dir;

struct ubus_msg_buf *ubus_msg_new(void *data, int len, bool shared);
void ubus_msg_send(struct ubus_client *cl, struct ubus_msg_buf *ub);
void ubus_msg_free(struct ubus_msg_buf *ub);
struct blob_attr **ubus_parse_msg(struct blob_attr *msg);

struct ubus_client *ubusd_proto_new_client(int fd, uloop_fd_handler cb);
void ubusd_proto_receive_message(struct ubus_client *cl, struct ubus_msg_buf *ub);
void ubusd_proto_free_client(struct ubus_client *cl);
void ubus_proto_send_msg_from_blob(struct ubus_client *cl, struct ubus_msg_buf *ub,
				   uint8_t type);

typedef struct ubus_msg_buf *(*event_fill_cb)(void *priv, const char *id);
void ubusd_event_init(void);
void ubusd_event_cleanup_object(struct ubus_object *obj);
void ubusd_send_obj_event(struct ubus_object *obj, bool add);
int ubusd_send_event(struct ubus_client *cl, const char *id,
		     event_fill_cb fill_cb, void *cb_priv);

void ubusd_acl_init(void);

void ubusd_monitor_init(void);
void ubusd_monitor_message(struct ubus_client *cl, struct ubus_msg_buf *ub, bool send);
void ubusd_monitor_disconnect(struct ubus_client *cl);

#endif
