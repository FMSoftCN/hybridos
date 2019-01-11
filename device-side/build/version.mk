#
# Copyright (C) 2012-2015 OpenWrt.org
# Copyright (C) 2015-2016 tracewong<tracewong@outlook.com>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# Handle various build version information.
#
# Guarantees that the following are defined:
#     PLATFORM_VERSION
#     PLATFORM_SDK_VERSION
#     PLATFORM_VERSION_CODENAME
#     DEFAULT_APP_TARGET_SDK
#     BUILD_ID
#     BUILD_NUMBER

ifeq "" "$(PLATFORM_VERSION)"
  PLATFORM_VERSION := 2.1.2
endif

ifeq "" "$(PLATFORM_VERSION_CODENAME)"
  PLATFORM_VERSION_CODENAME := Neptune
endif

ifeq "" "$(BUILD_ID)"
  BUILD_ID := 593A1793
endif

ifeq "" "$(TARGET_VERSION_DIST)"
  TARGET_VERSION_DIST := tina.$(USER).$(shell date +%Y%m%d.%H%M%S)
endif

ifeq "" "$(TARGET_VERSION_MANUFACTURER)"
  TARGET_VERSION_MANUFACTURER := fmsoft
endif

ifeq "" "$(TARGET_VERSION_MANUFACTURER_URL)"
  TARGET_VERSION_MANUFACTURER_URL := www.fmsoft.cn
endif

ifeq "" "$(TARGET_VERSION_REPO)"
	TARGET_VERSION_REPO := https://github.com/FMSoftCN/hybridos/
endif

ifeq "" "$(TARGET_VERSION_PRODUCT)"
  TARGET_VERSION_PRODUCT := $(PLATFORM_VERSION)
endif

qstrip_escape=$(subst ','\'',$(call qstrip,$(1)))
#'

VERSION_NUMBER:=$(call qstrip_escape,$(PLATFORM_VERSION))

VERSION_CODENAME:=$(call qstrip_escape,$(PLATFORM_VERSION_CODENAME))

VERSION_DIST:=$(call qstrip_escape,$(TARGET_VERSION_DIST))

VERSION_MANUFACTURER:=$(call qstrip_escape,$(TARGET_VERSION_MANUFACTURER))

VERSION_MANUFACTURER_URL:=$(call qstrip_escape,$(TARGET_VERSION_MANUFACTURER_URL))

VERSION_PRODUCT:=$(call qstrip_escape,$(TARGET_VERSION_PRODUCT))

VERSION_HWREV:=$(call qstrip_escape,$(TARGET_VERSION_HWREV))
VERSION_HWREV:=$(if $(VERSION_HWREV),$(VERSION_HWREV),V_0_0)

define taint2sym
$(CONFIG_$(firstword $(subst :, ,$(subst +,,$(subst -,,$(1))))))
endef

define taint2name
$(lastword $(subst :, ,$(1)))
endef

VERSION_TAINT_SPECS := \
	-ALL_KMODS:no-all \
	-IPV6:no-ipv6 \
	+USE_GLIBC:glibc \
	+USE_MKLIBS:mklibs \
	+BUSYBOX_CUSTOM:busybox \
	+OVERRIDE_PKGS:override \

VERSION_TAINTS := $(strip $(foreach taint,$(VERSION_TAINT_SPECS), \
	$(if $(findstring +,$(taint)), \
		$(if $(call taint2sym,$(taint)),$(call taint2name,$(taint))), \
		$(if $(call taint2sym,$(taint)),,$(call taint2name,$(taint))) \
	)))

PKG_CONFIG_DEPENDS += $(foreach taint,$(VERSION_TAINT_SPECS),$(call taint2sym,$(taint)))

VERSION_SED:=$(SED) 's,%U,$(VERSION_REPO),g' \
	-e 's,%V,$(VERSION_NUMBER),g' \
	-e 's,%v,\L$(subst $(space),_,$(VERSION_NUMBER)),g' \
	-e 's,%C,$(VERSION_CODENAME),g' \
	-e 's,%c,\L$(subst $(space),_,$(VERSION_CODENAME)),g' \
	-e 's,%D,$(VERSION_DIST),g' \
	-e 's,%d,\L$(subst $(space),_,$(VERSION_DIST)),g' \
	-e 's,%R,$(BUILD_ID),g' \
	-e 's,%T,$(BOARD),g' \
	-e 's,%S,$(BOARD)/$(if $(SUBTARGET),$(SUBTARGET),generic),g' \
	-e 's,%t,$(VERSION_TAINTS),g' \
	-e 's,%M,$(VERSION_MANUFACTURER),g' \
	-e 's,%m,$(VERSION_MANUFACTURER_URL),g' \
	-e 's,%P,$(VERSION_PRODUCT),g' \
	-e 's,%h,$(VERSION_HWREV),g'

VERSION_SED_SCRIPT:=$(subst '\'','\'\\\\\'\'',$(VERSION_SED))
