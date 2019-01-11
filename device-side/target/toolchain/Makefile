#
# Copyright (C) 2008-2009 Industrie Dial Face S.p.A.
#               Luigi 'Comio' Mantellini <luigi.mantellini@idf-hit.com>
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/kernel.mk
include $(BUILD_DIR)/host.mk
include $(BUILD_DIR)/version.mk

override MAKEFLAGS=

ifeq ($(TARGET_CPU_VARIANT),arm926ej-s)
TOOLCHAIN_NAME:=toolchain-$(ARCH_PACKAGES)-arm9-$(LIBC)
else
TOOLCHAIN_NAME:=toolchain-$(ARCH_PACKAGES)-$(LIBC)
endif
TOOLCHAIN_BUILD_DIR:=$(COMPILE_DIR)/$(TOOLCHAIN_NAME)
EXCLUDE_DIRS:=*/ccache \
	*/initial \
	*/stamp \
	*/stampfiles \
	*/man \
	*/info

all: compile

TOOLCHAIN_PREFIX:=$(TOOLCHAIN_BUILD_DIR)/toolchain

$(TARGET_OUT_DIR)/$(TOOLCHAIN_NAME).tar.bz2: clean
	mkdir -p $(TOOLCHAIN_BUILD_DIR)
	$(TAR) -cf - -C $(TARGET_OUT_DIR)/staging_dir/  \
	       $(foreach exclude,$(EXCLUDE_DIRS),--exclude="$(exclude)") \
	       toolchain | \
	       $(TAR) -xf - -C $(TOOLCHAIN_BUILD_DIR)

	$(CP)  $(TOPDIR)/build/LICENSE ./files/README.TOOLCHAIN \
		$(TOOLCHAIN_BUILD_DIR)/

	$(CP) ./files/wrapper.sh $(TOOLCHAIN_PREFIX)/bin/$(REAL_GNU_TARGET_NAME)-wrapper.sh
	chmod +x $(TOOLCHAIN_PREFIX)/bin/$(REAL_GNU_TARGET_NAME)-wrapper.sh
	(cd $(TOOLCHAIN_PREFIX)/bin; \
		for app in cc gcc g++ c++ cpp ld as ; do \
			[ -f $(REAL_GNU_TARGET_NAME)-$${app} ] && mv $(REAL_GNU_TARGET_NAME)-$${app} $(REAL_GNU_TARGET_NAME)-$${app}.bin ; \
			ln -sf $(REAL_GNU_TARGET_NAME)-wrapper.sh $(REAL_GNU_TARGET_NAME)-$${app} ; \
		done; \
	)

	@-( \
		find \
			$(TOOLCHAIN_BUILD_DIR)/*/bin \
			$(TOOLCHAIN_BUILD_DIR)/*/*/bin \
			$(TOOLCHAIN_BUILD_DIR)/*/libexec \
			-type f; \
	) | xargs strip 2>/dev/null >/dev/null

	echo REVISION:="$(REVISION)" > $(TOOLCHAIN_BUILD_DIR)/version.mk
	find $(TOOLCHAIN_BUILD_DIR) -name .git | $(XARGS) rm -rf
	find $(TOOLCHAIN_BUILD_DIR) -name .svn | $(XARGS) rm -rf
	find $(TOOLCHAIN_BUILD_DIR) -name CVS | $(XARGS) rm -rf
	(cd $(COMPILE_DIR); \
		tar cfj $@ $(TOOLCHAIN_NAME); \
	)
ifeq ($(CONFIG_UPDATE_TOOLCHAIN), y)
	@rm -rf $(CONFIG_TOOLCHAIN_ROOT)/$(TOOLCHAIN_NAME)
	@$(TAR) -xf $@ -C $(CONFIG_TOOLCHAIN_ROOT)
endif

download:
prepare:
compile: $(TARGET_OUT_DIR)/$(TOOLCHAIN_NAME).tar.bz2
install: compile

clean:
	rm -rf $(TOOLCHAIN_BUILD_DIR) $(TARGET_OUT_DIR)/$(TOOLCHAIN_NAME).tar.bz2
