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
#ifndef __NETIFD_UBUS_H
#define __NETIFD_UBUS_H

extern struct ubus_context *ubus_ctx;

int netifd_ubus_init(const char *path);
void netifd_ubus_done(void);
void netifd_ubus_add_interface(struct interface *iface);
void netifd_ubus_remove_interface(struct interface *iface);
void netifd_ubus_interface_event(struct interface *iface, bool up);
void netifd_ubus_interface_notify(struct interface *iface, bool up);

#endif
