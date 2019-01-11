# 
# Copyright (C) 2006-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk
include $(BUILD_DIR)/host.mk
include $(BUILD_DIR)/version.mk

override MAKEFLAGS=

SDK_NAME:=$(VERSION_DIST)-SDK-$(if $(CONFIG_VERSION_FILENAMES),$(VERSION_NUMBER)-)$(BOARD)$(if $(SUBTARGET),-$(SUBTARGET))$(if $(GCCV),_gcc-$(GCCV))$(DIR_SUFFIX).$(HOST_OS)-$(HOST_ARCH)
SDK_BUILD_DIR:=$(COMPILE_DIR)/$(SDK_NAME)

STAGING_SUBDIR_HOST := $(TOPDIR)/out/host

EXCLUDE_DIRS:=*/ccache \
	*/stamp \
	*/stampfiles \
	*/man \
	*/info \
	*/root-* \
	initial \
	*.install.clean \
	*.install.flags \
	*.install \
	*/doc

SDK_DIRS = \
		$(STAGING_SUBDIR_HOST)

BASE_FEED:=$(shell git config --get remote.origin.url 2>/dev/null | sed -ne 's/^/src-git base /p')
BASE_FEED:=$(if $(BASE_FEED),$(BASE_FEED),$(shell cd $(TOPDIR); LC_ALL=C git svn info 2>/dev/null | sed -ne 's/^URL: /src-gitsvn base /p'))
BASE_FEED:=$(if $(BASE_FEED),$(BASE_FEED),$(shell cd $(TOPDIR); LC_ALL=C svn info 2>/dev/null | sed -ne 's/^URL: /src-svn base /p'))
BASE_FEED:=$(if $(BASE_FEED),$(BASE_FEED),src-git base https://git.openwrt.org/$(filter-out unknown/ r%/,$(subst $(space),.,$(wordlist 1,2,$(subst .,$(space),$(VERSION_NUMBER))))/)openwrt.git)

KDIR_BASE = $(patsubst $(TOPDIR)/%,%,$(LINUX_DIR))

KERNEL_FILES_ARCH = \
	Makefile* \
	module.lds \
	Kbuild.platforms \
	*/Platform \
	include \
	*/include \
	scripts \
	kernel/asm-offsets.s

KERNEL_FILES_BASE := \
	.config \
	Makefile \
	scripts \
	include \
	Module.symvers \
	modules.builtin \
	$(addprefix arch/$(LINUX_KARCH)/,$(KERNEL_FILES_ARCH))

KERNEL_FILES := $(patsubst $(TOPDIR)/%,%,$(wildcard $(addprefix $(LINUX_DIR)/,$(KERNEL_FILES_BASE))))

all: compile

$(TARGET_OUT_DIR)/$(SDK_NAME).tar.bz2: clean
	mkdir -p $(SDK_BUILD_DIR)/dl $(SDK_BUILD_DIR)/package
	$(CP) $(BUILD_DIR) $(SCRIPT_DIR) $(TOPDIR)/prebuilt $(TOPDIR)/docs $(SDK_BUILD_DIR)/
	$(TAR) -cf - -C $(TOPDIR) \
		`cd $(TOPDIR); find $(KDIR_BASE) -name \*.ko` \
		$(foreach exclude,$(EXCLUDE_DIRS),--exclude="$(exclude)") \
		$(SDK_DIRS) $(KERNEL_FILES) | \
		$(TAR) -xf - -C $(SDK_BUILD_DIR)

	@-( \
		find \
			$(SDK_BUILD_DIR)/$(STAGING_SUBDIR_HOST)/bin \
			$(SDK_BUILD_DIR)/$(STAGING_SUBDIR_HOST)/usr/bin \
			-type f; \
		find \
			$(SDK_BUILD_DIR)/$(STAGING_SUBDIR_HOST)/lib \
			$(SDK_BUILD_DIR)/$(STAGING_SUBDIR_HOST)/usr/lib \
			-type f -name \*.so\* -or -name \*.a; \
	) | xargs strip 2>/dev/null >/dev/null

	mkdir -p $(SDK_BUILD_DIR)/target/allwinner
	$(CP) $(GENERIC_PLATFORM_DIR) $(PLATFORM_DIR) $(GENERIC_PLATFORM_DIR)/../$(TARGET_PLATFORM)-common $(SDK_BUILD_DIR)/target/allwinner/
	rm -rf \
		$(SDK_BUILD_DIR)/target/allwinner/*/files* \
		$(SDK_BUILD_DIR)/target/allwinner/*/patches*
	./convert-config.pl $(TOPDIR)/.config > $(SDK_BUILD_DIR)/Config-build.in
	$(CP) -L \
		$(BUILD_DIR)/LICENSE \
		$(TOPDIR)/rules.mk \
		$(TOPDIR)/.config \
		./files/Config.in \
		./files/Makefile \
		./files/build/prepare.mk \
		./files/README.SDK \
		$(SDK_BUILD_DIR)/
	$(CP) \
		$(TOPDIR)/package/Makefile \
		$(SDK_BUILD_DIR)/package/

	$(SED) 's,^# REVISION:=.*,REVISION:=$(REVISION),g' $(SDK_BUILD_DIR)/build/version.mk
	$(SED) '/LINUX_VERMAGIC:=/ { s,unknown,$(LINUX_VERMAGIC),g }' $(SDK_BUILD_DIR)/build/kernel.mk
	find $(SDK_BUILD_DIR) -name .git | $(XARGS) rm -rf
	find $(SDK_BUILD_DIR) -name .svn | $(XARGS) rm -rf
	find $(SDK_BUILD_DIR) -name CVS | $(XARGS) rm -rf
	(cd $(COMPILE_DIR); \
		tar cfj $@ $(SDK_NAME); \
	)

download:
prepare:
compile: $(TARGET_OUT_DIR)/$(SDK_NAME).tar.bz2
install: compile

clean:
	rm -rf $(SDK_BUILD_DIR) $(TARGET_OUT_DIR)/$(SDK_NAME).tar.bz2
