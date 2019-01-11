#
# Copyright (C) 2006-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=pack-bintools
PKG_VERSION:=2017.08.18

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

include $(BUILD_DIR)/host-build.mk

define Host/Compile
endef

define Host/Install
	$(INSTALL_BIN) $(HOST_BUILD_DIR)/* $(STAGING_DIR_HOST)/bin/
endef

define Host/Clean
endef

$(eval $(call HostBuild))
