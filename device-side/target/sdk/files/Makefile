# Makefile for OpenWrt
#
# Copyright (C) 2007-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

TOPDIR:=${CURDIR}
LC_ALL:=C
LANG:=C
SDK:=1
export TOPDIR LC_ALL LANG SDK

world:

include $(TOPDIR)/build/host.mk

ifneq ($(OPENWRT_BUILD),1)
  override OPENWRT_BUILD=1
  export OPENWRT_BUILD

  empty:=
  space:= $(empty) $(empty)
  _SINGLE=export MAKEFLAGS=$(space);

  include $(TOPDIR)/build/debug.mk
  include $(TOPDIR)/build/depends.mk
  include $(TOPDIR)/build/toplevel.mk
else
  include rules.mk
  include $(BUILD_DIR)/depends.mk
  include $(BUILD_DIR)/subdir.mk
  include package/Makefile

$(package/stamp-compile): $(COMPILE_DIR)/.prepared
$(COMPILE_DIR)/.prepared: Makefile
	@mkdir -p $$(dirname $@)
	@touch $@

clean: FORCE
	git clean -f -d $(STAGING_DIR); true
	git clean -f -d $(COMPILE_DIR); true
	git clean -f -d $(TARGET_OUT_DIR); true

dirclean: clean
	git reset --hard HEAD
	git clean -f -d
	rm -rf feeds/

# check prerequisites before starting to build
prereq: $(package/stamp-prereq) ;

world: prepare $(package/stamp-compile) FORCE
	@$(MAKE) package/index

.PHONY: clean dirclean prereq prepare world

endif
