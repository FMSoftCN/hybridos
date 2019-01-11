#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/net-ap6212
  SUBMENU:=$(WIRELESS_MENU)
  TITLE:=ap6212 support
  DEPENDS:=@TARGET_astar_joobot +ap6212-firmware
  FILES:=$(LINUX_DIR)/drivers/net/wireless/bcmdhd/bcmdhd.ko
  AUTOLOAD:=$(call AutoProbe,bcmdhd)
endef

define KernelPackage/net-ap6212/description
 Kernel modules for Broadcom AP6212  support
endef

$(eval $(call KernelPackage,net-ap6212))

define KernelPackage/sunxi-bma250
  SUBMENU:=$(I2C_MENU)
  TITLE:=SUNXI-BMA250 support
  FILES:=$(LINUX_DIR)/drivers/hwmon/bma250.ko
  AUTOLOAD:=$(call AutoLoad,50,bma250,1)
endef

define KernelPackage/sunxi-bma250/description
 Kernel modules for sunxi-bma250 support
endef

$(eval $(call KernelPackage,sunxi-bma250))

define KernelPackage/touchscreen-cst016se
  SUBMENU:=$(INPUT_MODULES_MENU)
  TITLE:=cst016se support
  DEPENDS:= +kmod-input-core
  KCONFIG:= \
          CONFIG_INPUT_TOUCHSCREEN \
          CONFIG_INPUT_TOUCHSCREEN_CST016SE
  FILES:=$(LINUX_DIR)/drivers/input/touchscreen/cst016se/cst016se.ko
  AUTOLOAD:=$(call AutoProbe,cst016se)
endef

define KernelPackage/touchscreen-cst016se/description
 Enable support for cst016se touchscreen port.
endef

$(eval $(call KernelPackage,touchscreen-cst016se))
