#
# Copyright (C) 2006-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=lzop
PKG_VERSION:=1.04

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.lzop.org/download/
PKG_MD5SUM:=271eb10fde77a0a96b9cbf745e719ddf

HOST_BUILD_PARALLEL:=1

include $(BUILD_DIR)/host-build.mk

# compile static for host without library liblzo2.so.2
HOST_LDFLAGS += -static

define Host/Install
	+$(MAKE) $(HOST_JOBS) -C $(HOST_BUILD_DIR) install
endef

$(eval $(call HostBuild))
