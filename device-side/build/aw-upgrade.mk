#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# author:henrisk.ho

define Aw/BuildUpgradeImage/prepare
	#$(1): target_dir #$(2): temp_usr

	#prepare for base system (no include /usr)
	#for procd
	-mv $(1)/usr/lib/libjson-c.so.* $(1)/lib/
	-mv $(1)/usr/bin/readlink $(1)/bin/
	-mv $(1)/usr/bin/basename $(1)/bin/
	-mv $(1)/usr/bin/jshn $(1)/bin/

	#for wifi
	-mv $(1)/usr/sbin/wpa_* $(1)/sbin/
	-mv $(1)/usr/lib/libnl-tiny.so $(1)/lib/

	#for filesystem tools
	-mv $(1)/usr/sbin/mke2fs $(1)/sbin/
	-mv $(1)/usr/sbin/mkfs.ext4 $(1)/sbin/
	-mv $(1)/usr/sbin/mkfs.jffs2 $(1)/sbin/

	-mv $(1)/usr/lib/libext2fs.so* $(1)/lib/
	-mv $(1)/usr/lib/libcom_err.so* $(1)/lib/
	-mv $(1)/usr/lib/libuuid.so* $(1)/lib/
	-mv $(1)/usr/lib/libe2p.so* $(1)/lib/
	-mv $(1)/usr/lib/libz.so* $(1)/lib/

	#for tinymp3player
	-mv $(1)/usr/lib/libmad.so* $(1)/lib/
	-mv $(1)/usr/lib/libasound.so* $(1)/lib/

	#ssl
	-mv $(1)/usr/lib/libssl.so* $(1)/lib/

	#curl
	-mv $(1)/usr/lib/libcurl.so* $(1)/lib/

	#crypto
	-mv $(1)/usr/lib/libcrypto.so* $(1)/lib/

	find $(1)/usr/lib | grep libstdc++.so | xargs -i mv  {} $(1)/lib

	#for link /usr/bin /usr/sbin
	rm -rf $(2)
	mv $(1)/usr $(2)
	mkdir -p $(1)/lib/functions
	mkdir -p $(1)/usr/share
	cp -r $(2)/bin $(1)/usr/
	find $(1)/usr/bin -type f -exec rm {} \;
	cp $(2)/share/libubox/jshn.sh $(1)/lib/functions/
	cp -r $(2)/share/* $(1)/usr/share/

endef

define Aw/BuildUpgradeImage/resume
	#$(1): target_dir #$(2): temp_usr
	rm -rf $(1)/usr
	mv $(2) $(1)/usr
endef

define Aw/BuildUpgradeImage/normal-prepare
	#$(1): target_dir #$(2): temp_usr
	$(call Aw/BuildUpgradeImage/prepare,$(1),$(2))
endef

define Aw/BuildUpgradeImage/normal-resume
	#$(1): target_dir #$(2): temp_usr
	$(call Aw/BuildUpgradeImage/resume,$(1),$(2))
endef

ifeq ($(CONFIG_TARGET_AW_OTA_INITRAMFS),y)
define Aw/BuildUpgradeImage/initramfs-prepare
	#$(1): target_dir #$(2): temp_usr
	$(call Aw/BuildUpgradeImage/prepare,$(1),$(2))
endef

define Aw/BuildUpgradeImage/initramfs-resume
	#$(1): target_dir #$(2): temp_usr
	$(call Aw/BuildUpgradeImage/resume,$(1),$(2))
endef
else
define Aw/BuildUpgradeImage/initramfs-prepare
endef

define Aw/BuildUpgradeImage/initramfs-resume
endef
endif
