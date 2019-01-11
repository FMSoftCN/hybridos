#
# Copyright (C) 2013-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(BUILD_DIR)/image.mk
include $(BUILD_DIR)/host.mk

FAT32_BLOCK_SIZE=1024
FAT32_BLOCKS=$(shell echo $$(($(CONFIG_SUNXI_SD_BOOT_PARTSIZE)*1024*1024/$(FAT32_BLOCK_SIZE))))
RECOVERY_INITRAMFS_BLOCKS=$(shell echo $$(($(CONFIG_SUNXI_RECOVERY_INITRAMFS_PARTSIZE)*1024*1024/$(FAT32_BLOCK_SIZE))))
LOAD_ADDRESS:=0x40008000
ENTRY_POINT:=0x40008000
IMAGE_DATA:=zImage
UIMAGE_NAME:=uImage
TARCH:=$(ARCH)
COMP:=none
ifeq ($(ARCH), aarch64)
TARCH=arm64
endif

ifneq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),)
UIMAGE_NAME=uImage-initramfs
endif

ifeq ($(TARGET_BOARD_PLATFORM), tulip)
LOAD_ADDRESS=0x40080000
ENTRY_POINT=0x40080000
IMAGE_DATA=Image.gz
COMP=gzip
endif

ifeq ($(TARGET_BOARD_PLATFORM), sitar)
LOAD_ADDRESS=0x80008000
ENTRY_POINT=0x80008000
endif

define mkuImage
	mkimage -A $(TARCH) -O linux -T kernel -C $(1) -a $(2) -e $(3) \
		-n '$(call toupper,$(ARCH)) OpenWrt Linux-$(LINUX_VERSION)' -d $(4) $(5)
endef

define Image/BuildKernel
	$(call mkuImage, \
		$(COMP), $(LOAD_ADDRESS), $(ENTRY_POINT), \
		$(KDIR)/$(IMAGE_DATA), $(TARGET_OUT_DIR)/$(IMG_PREFIX)-uImage \
		)
endef

define Image/Build/Initramfs
	$(CP) $(KDIR)/$(IMAGE_DATA)-initramfs $(TARGET_OUT_DIR)/$(IMG_PREFIX)-$(IMAGE_DATA)-initramfs
	echo -ne '\x00\x00\x00\x00' >> $(TARGET_OUT_DIR)/$(IMG_PREFIX)-$(IMAGE_DATA)-initramfs
	$(call mkuImage, \
		$(COMP), $(LOAD_ADDRESS), $(ENTRY_POINT), \
		$(TARGET_OUT_DIR)/$(IMG_PREFIX)-$(IMAGE_DATA)-initramfs, \
		$(TARGET_OUT_DIR)/$(IMG_PREFIX)-$(UIMAGE_NAME) \
		)
endef

ifneq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),)
define Image/InstallKernel
	rm -f $(TARGET_OUT_DIR)/boot.img
	rm -f $(TARGET_OUT_DIR)/boot_initramfs.img
	mkdosfs $(TARGET_OUT_DIR)/boot_initramfs.img -C $(RECOVERY_INITRAMFS_BLOCKS)
	mcopy -i $(TARGET_OUT_DIR)/boot_initramfs.img $(TARGET_OUT_DIR)/$(IMG_PREFIX)-uImage-initramfs ::uImage
endef
else
define Image/InstallKernel
	rm -f $(TARGET_OUT_DIR)/boot.img
	rm -f $(TARGET_OUT_DIR)/boot_initramfs.img
	mkdosfs $(TARGET_OUT_DIR)/boot.img -C $(FAT32_BLOCKS)
	mcopy -i $(TARGET_OUT_DIR)/boot.img $(TARGET_OUT_DIR)/$(IMG_PREFIX)-$(UIMAGE_NAME) ::uImage
endef
endif

define Image/Build
	rm -f $(TARGET_OUT_DIR)/rootfs.img
	rm -f $(TARGET_OUT_DIR)/usr.img
	$(call Image/Build/$(1),$(1))
	dd if=$(KDIR)/root.$(1) of=$(TARGET_OUT_DIR)/rootfs.img bs=128k conv=sync
ifneq ($(CONFIG_SUNXI_SMALL_STORAGE_OTA),)
	dd if=$(KDIR)/usr.$(1) of=$(TARGET_OUT_DIR)/usr.img bs=128k conv=sync
endif
endef

$(eval $(call BuildImage))
