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

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/uio.h>
#ifdef FreeBSD
#include <sys/param.h>
#endif
#include <syslog.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <libubox/blob.h>
#include <libubox/uloop.h>
#include <libubox/usock.h>
#include <libubox/list.h>

#include "ubusd.h"

static struct ubus_msg_buf *ubus_msg_ref(struct ubus_msg_buf *ub)
{
	struct ubus_msg_buf *new_ub;
	if (ub->refcount == ~0) {
		new_ub = ubus_msg_new(ub->data, ub->len, false);
		if (!new_ub)
			return NULL;
		memcpy(&new_ub->hdr, &ub->hdr, sizeof(struct ubus_msghdr));
		new_ub->fd = ub->fd;
		return new_ub;
	}

	ub->refcount++;
	return ub;
}

struct ubus_msg_buf *ubus_msg_new(void *data, int len, bool shared)
{
	struct ubus_msg_buf *ub;
	int buflen = sizeof(*ub);

	if (!shared)
		buflen += len;

	ub = calloc(1, buflen);
	if (!ub)
		return NULL;

	ub->fd = -1;

	if (shared) {
		ub->refcount = ~0;
		ub->data = data;
	} else {
		ub->refcount = 1;
		ub->data = (void *) (ub + 1);
		if (data)
			memcpy(ub + 1, data, len);
	}

	ub->len = len;
	return ub;
}

void ubus_msg_free(struct ubus_msg_buf *ub)
{
	switch (ub->refcount) {
	case 1:
	case ~0:
		if (ub->fd >= 0)
			close(ub->fd);

		free(ub);
		break;
	default:
		ub->refcount--;
		break;
	}
}

static int ubus_msg_writev(int fd, struct ubus_msg_buf *ub, int offset)
{
	static struct iovec iov[2];
	static struct {
		struct cmsghdr h;
		int fd;
	} fd_buf = {
		.h = {
			.cmsg_len = sizeof(fd_buf),
			.cmsg_level = SOL_SOCKET,
			.cmsg_type = SCM_RIGHTS,
		},
	};
	struct msghdr msghdr = {
		.msg_iov = iov,
		.msg_iovlen = ARRAY_SIZE(iov),
		.msg_control = &fd_buf,
		.msg_controllen = sizeof(fd_buf),
	};

	fd_buf.fd = ub->fd;
	if (ub->fd < 0) {
		msghdr.msg_control = NULL;
		msghdr.msg_controllen = 0;
	}

	if (offset < sizeof(ub->hdr)) {
		struct ubus_msghdr hdr;

		hdr.version = ub->hdr.version;
		hdr.type = ub->hdr.type;
		hdr.seq = cpu_to_be16(ub->hdr.seq);
		hdr.peer = cpu_to_be32(ub->hdr.peer);

		iov[0].iov_base = ((char *) &hdr) + offset;
		iov[0].iov_len = sizeof(hdr) - offset;
		iov[1].iov_base = (char *) ub->data;
		iov[1].iov_len = ub->len;

		return sendmsg(fd, &msghdr, 0);
	} else {
		offset -= sizeof(ub->hdr);
		return write(fd, ((char *) ub->data) + offset, ub->len - offset);
	}
}

static void ubus_msg_enqueue(struct ubus_client *cl, struct ubus_msg_buf *ub)
{
	if (cl->tx_queue[cl->txq_tail])
		return;

	cl->tx_queue[cl->txq_tail] = ubus_msg_ref(ub);
	cl->txq_tail = (cl->txq_tail + 1) % ARRAY_SIZE(cl->tx_queue);
}

/* takes the msgbuf reference */
void ubus_msg_send(struct ubus_client *cl, struct ubus_msg_buf *ub)
{
	int written;

	if (ub->hdr.type != UBUS_MSG_MONITOR)
		ubusd_monitor_message(cl, ub, true);

	if (!cl->tx_queue[cl->txq_cur]) {
		written = ubus_msg_writev(cl->sock.fd, ub, 0);

		if (written < 0)
			written = 0;

		if (written >= ub->len + sizeof(ub->hdr))
			return;

		cl->txq_ofs = written;

		/* get an event once we can write to the socket again */
		uloop_fd_add(&cl->sock, ULOOP_READ | ULOOP_WRITE | ULOOP_EDGE_TRIGGER);
	}
	ubus_msg_enqueue(cl, ub);
}

static struct ubus_msg_buf *ubus_msg_head(struct ubus_client *cl)
{
	return cl->tx_queue[cl->txq_cur];
}

static void ubus_msg_dequeue(struct ubus_client *cl)
{
	struct ubus_msg_buf *ub = ubus_msg_head(cl);

	if (!ub)
		return;

	ubus_msg_free(ub);
	cl->txq_ofs = 0;
	cl->tx_queue[cl->txq_cur] = NULL;
	cl->txq_cur = (cl->txq_cur + 1) % ARRAY_SIZE(cl->tx_queue);
}

static void handle_client_disconnect(struct ubus_client *cl)
{
	while (ubus_msg_head(cl))
		ubus_msg_dequeue(cl);

	ubusd_monitor_disconnect(cl);
	ubusd_proto_free_client(cl);
	if (cl->pending_msg_fd >= 0)
		close(cl->pending_msg_fd);
	uloop_fd_delete(&cl->sock);
	close(cl->sock.fd);
	free(cl);
}

static void client_cb(struct uloop_fd *sock, unsigned int events)
{
	struct ubus_client *cl = container_of(sock, struct ubus_client, sock);
	struct ubus_msg_buf *ub;
	static struct iovec iov;
	static struct {
		struct cmsghdr h;
		int fd;
	} fd_buf = {
		.h = {
			.cmsg_type = SCM_RIGHTS,
			.cmsg_level = SOL_SOCKET,
			.cmsg_len = sizeof(fd_buf),
		}
	};
	struct msghdr msghdr = {
		.msg_iov = &iov,
		.msg_iovlen = 1,
	};

	/* first try to tx more pending data */
	while ((ub = ubus_msg_head(cl))) {
		int written;

		written = ubus_msg_writev(sock->fd, ub, cl->txq_ofs);
		if (written < 0) {
			switch(errno) {
			case EINTR:
			case EAGAIN:
				break;
			default:
				goto disconnect;
			}
			break;
		}

		cl->txq_ofs += written;
		if (cl->txq_ofs < ub->len + sizeof(ub->hdr))
			break;

		ubus_msg_dequeue(cl);
	}

	/* prevent further ULOOP_WRITE events if we don't have data
	 * to send anymore */
	if (!ubus_msg_head(cl) && (events & ULOOP_WRITE))
		uloop_fd_add(sock, ULOOP_READ | ULOOP_EDGE_TRIGGER);

retry:
	if (!sock->eof && cl->pending_msg_offset < sizeof(cl->hdrbuf)) {
		int offset = cl->pending_msg_offset;
		int bytes;

		fd_buf.fd = -1;

		iov.iov_base = ((char *) &cl->hdrbuf) + offset;
		iov.iov_len = sizeof(cl->hdrbuf) - offset;

		if (cl->pending_msg_fd < 0) {
			msghdr.msg_control = &fd_buf;
			msghdr.msg_controllen = sizeof(fd_buf);
		} else {
			msghdr.msg_control = NULL;
			msghdr.msg_controllen = 0;
		}

		bytes = recvmsg(sock->fd, &msghdr, 0);
		if (bytes < 0)
			goto out;

		if (fd_buf.fd >= 0)
			cl->pending_msg_fd = fd_buf.fd;

		cl->pending_msg_offset += bytes;
		if (cl->pending_msg_offset < sizeof(cl->hdrbuf))
			goto out;

		if (blob_pad_len(&cl->hdrbuf.data) > UBUS_MAX_MSGLEN)
			goto disconnect;

		cl->pending_msg = ubus_msg_new(NULL, blob_raw_len(&cl->hdrbuf.data), false);
		if (!cl->pending_msg)
			goto disconnect;

		cl->hdrbuf.hdr.seq = be16_to_cpu(cl->hdrbuf.hdr.seq);
		cl->hdrbuf.hdr.peer = be32_to_cpu(cl->hdrbuf.hdr.peer);

		memcpy(&cl->pending_msg->hdr, &cl->hdrbuf.hdr, sizeof(cl->hdrbuf.hdr));
		memcpy(cl->pending_msg->data, &cl->hdrbuf.data, sizeof(cl->hdrbuf.data));
	}

	ub = cl->pending_msg;
	if (ub) {
		int offset = cl->pending_msg_offset - sizeof(ub->hdr);
		int len = blob_raw_len(ub->data) - offset;
		int bytes = 0;

		if (len > 0) {
			bytes = read(sock->fd, (char *) ub->data + offset, len);
			if (bytes <= 0)
				goto out;
		}

		if (bytes < len) {
			cl->pending_msg_offset += bytes;
			goto out;
		}

		/* accept message */
		ub->fd = cl->pending_msg_fd;
		cl->pending_msg_fd = -1;
		cl->pending_msg_offset = 0;
		cl->pending_msg = NULL;
		ubusd_monitor_message(cl, ub, false);
		ubusd_proto_receive_message(cl, ub);
		goto retry;
	}

out:
	if (!sock->eof || ubus_msg_head(cl))
		return;

disconnect:
	handle_client_disconnect(cl);
}

static bool get_next_connection(int fd)
{
	struct ubus_client *cl;
	int client_fd;

	client_fd = accept(fd, NULL, 0);
	if (client_fd < 0) {
		switch (errno) {
		case ECONNABORTED:
		case EINTR:
			return true;
		default:
			return false;
		}
	}

	cl = ubusd_proto_new_client(client_fd, client_cb);
	if (cl)
		uloop_fd_add(&cl->sock, ULOOP_READ | ULOOP_EDGE_TRIGGER);
	else
		close(client_fd);

	return true;
}

static void server_cb(struct uloop_fd *fd, unsigned int events)
{
	bool next;

	do {
		next = get_next_connection(fd->fd);
	} while (next);
}

static struct uloop_fd server_fd = {
	.cb = server_cb,
};

static int usage(const char *progname)
{
	fprintf(stderr, "Usage: %s [<options>]\n"
		"Options: \n"
		"  -A <path>:		Set the path to ACL files\n"
		"  -s <socket>:		Set the unix domain socket to listen on\n"
		"\n", progname);
	return 1;
}

static void sighup_handler(int sig)
{
	ubusd_acl_load();
}

int main(int argc, char **argv)
{
	const char *ubus_socket = UBUS_UNIX_SOCKET;
	int ret = 0;
	int ch;

	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, sighup_handler);

	openlog("ubusd", LOG_PID, LOG_DAEMON);
	uloop_init();

	while ((ch = getopt(argc, argv, "A:s:")) != -1) {
		switch (ch) {
		case 's':
			ubus_socket = optarg;
			break;
		case 'A':
			ubusd_acl_dir = optarg;
			break;
		default:
			return usage(argv[0]);
		}
	}

	unlink(ubus_socket);
	umask(0111);
	server_fd.fd = usock(USOCK_UNIX | USOCK_SERVER | USOCK_NONBLOCK, ubus_socket, NULL);
	if (server_fd.fd < 0) {
		perror("usock");
		ret = -1;
		goto out;
	}
	uloop_fd_add(&server_fd, ULOOP_READ | ULOOP_EDGE_TRIGGER);
	ubusd_acl_load();

	uloop_run();
	unlink(ubus_socket);

out:
	uloop_done();
	return ret;
}
