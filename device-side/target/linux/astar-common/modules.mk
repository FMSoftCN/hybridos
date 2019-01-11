#
# Copyright (C) 2015-2016 Allwinner
#
# This is free software, licensed under the GNU General Public License v2.
# See /build/LICENSE for more information.

define KernelPackage/sunxi-vfe
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-vfe support
  FILES:=$(LINUX_DIR)/drivers/media/video/videobuf-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/videobuf-dma-contig.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/csi_cci/cci.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/vfe_os.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/vfe_subdev.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/device/gc0308.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/sunxi-vfe/vfe_v4l2.ko
  KCONFIG:= \
    CONFIG_MEDIA_SUPPORT=y \
    CONFIG_VIDEO_CAPTURE_DRIVERS=y \
    CONFIG_V4L_PLATFORM_DRIVERS=y \
    CONFIG_VIDEO_V4L2=y \
    CONFIG_VIDEO_SUNXI_VFE \
    CONFIG_CSI_VFE \
    CONFIG_VIDEOBUF_DMA_CONTIG \
    CONFIG_VIDEOBUF_DMA_SG \
    CONFIG_VIDEOBUF_GEN
  AUTOLOAD:=$(call AutoLoad,90,videobuf-core videobuf-dma-contig cci vfe_os vfe_subdev gc0308 vfe_v4l2)
endef

define KernelPackage/sunxi-vfe/description
  Kernel modules for sunxi-vfe support
endef

$(eval $(call KernelPackage,sunxi-vfe))

define KernelPackage/sunxi-uvc
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=sunxi-uvc support
  DEPENDS:=@USB_SUPPORT +kmod-usb-core +kmod-input-core +kmod-dma-buf
  FILES:=$(LINUX_DIR)/drivers/media/video/videobuf2-core.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/videobuf2-memops.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/videobuf2-vmalloc.ko
  FILES+=$(LINUX_DIR)/drivers/media/video/uvc/uvcvideo.ko
  KCONFIG:= \
    CONFIG_V4L_USB_DRIVERS=y \
    CONFIG_VIDEOBUF2_CORE \
    CONFIG_VIDEOBUF2_MEMOPS \
    CONFIG_VIDEOBUF2_VMALLOC \
    CONFIG_USB_VIDEO_CLASS \
    CONFIG_MEDIA_USB_SUPPORT
  AUTOLOAD:=$(call AutoProbe,videobuf2-core videobuf2-memops videobuf2_vmalloc uvcvideo)
endef

define KernelPackage/sunxi-uvc/description
Kernel modules for sunxi-uvc support
endef

$(eval $(call KernelPackage,sunxi-uvc))

define KernelPackage/leds-sunxi
  SUBMENU:=$(LEDS_MENU)
  TITLE:=leds-sunxi support
  FILES:=$(LINUX_DIR)/drivers/leds/leds-sunxi.ko
  KCONFIG:=CONFIG_LEDS_SUNXI
  AUTOLOAD:=$(call AutoLoad,60,leds-sunxi)
endef

define KernelPackage/leds-sunxi/description
  Kernel modules for leds sunxi support
endef

$(eval $(call KernelPackage,leds-sunxi))

define KernelPackage/ledtrig-doubleflash
  SUBMENU:=$(LEDS_MENU)
  TITLE:=LED DoubleFlash Trigger
  KCONFIG:=CONFIG_LEDS_TRIGGER_DOUBLEFLASH
  FILES:=$(LINUX_DIR)/drivers/leds/ledtrig-doubleflash.ko
  AUTOLOAD:=$(call AutoLoad,50,ledtrig-doubleflash)
endef

define KernelPackage/ledtrig-doubleflash/description
 Kernel module that allows LEDs to be controlled by a programmable doubleflash
 via sysfs
endef

$(eval $(call KernelPackage,ledtrig-doubleflash))

define KernelPackage/sunxi-disp
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=SUNXI-DISP support
  FILES:=$(LINUX_DIR)/drivers/video/sunxi/disp/disp.ko
  AUTOLOAD:=$(call AutoLoad,50,disp,1)
endef

define KernelPackage/sunxi-disp/description
 Kernel modules for sunxi-disp support
endef

$(eval $(call KernelPackage,sunxi-disp))

define KernelPackage/sunxi-lcd
  SUBMENU:=$(VIDEO_MENU)
  TITLE:=SUNXI-LCD support
  DEPENDS:=+kmod-sunxi-disp
  FILES:=$(LINUX_DIR)/drivers/video/sunxi/lcd/lcd.ko
  AUTOLOAD:=$(call AutoLoad,50,lcd,1)
endef

define KernelPackage/sunxi-lcd/description
 Kernel modules for sunxi-lcd support
endef

$(eval $(call KernelPackage,sunxi-lcd))
