#
# Copyright (C) 2009-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=flux
PKG_VERSION:=1_4_4

https://github.com/Distrotech/flux/releases/tag/FLUX_1_4_4
https://github.com/Distrotech/flux/archive/FLUX_1_4_4.tar.gz

PKG_SOURCE:=FLUX_$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/Distrotech/flux/archive/
#PKG_MD5SUM:=3d2a4bd0bbf5ba964b0a1ecdafd1ea9a

HOST_BUILD_DIR:=$(COMPILE_DIR_HOST)/flux-FLUX_$(PKG_VERSION)

HOST_FIXUP:=autoreconf

include $(BUILD_DIR)/host-build.mk

HOST_CONFIGURE_ARGS += \
        --enable-static=yes \
        --enable-shared=no \

define Host/Configure
	(cd $(HOST_BUILD_DIR); \
		./autogen.sh; \
	)
	$(Host/Configure/Default)
endef

define Host/Install
       $(INSTALL_BIN) \
               $(HOST_BUILD_DIR)/src/fluxcomp \
               $(STAGING_DIR_HOST)/bin/
endef

define Host/Clean
       rm -f $(STAGING_DIR_HOST)/bin/fluxcomp
endef

$(eval $(call HostBuild))
