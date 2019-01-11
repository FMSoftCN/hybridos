#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lzo
PKG_VERSION:=2.10
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.oberhumer.com/opensource/lzo/download/
PKG_MD5SUM:=39d3f3f9c55c87b1e5d6888e1420f4b5

HOST_FIXUP:=autoreconf

HOST_BUILD_PARALLEL:=1

include $(BUILD_DIR)/host-build.mk

define Host/Configure
	$(call Host/Configure/Default, \
		--enable-shared \
		--enable-static \
	)
endef

define Host/Install
	$(MAKE) -C $(HOST_BUILD_DIR) install
endef

$(eval $(call HostBuild))

