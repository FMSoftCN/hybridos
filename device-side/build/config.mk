# ---------------------------------------------------------------
# Set up configuration for host machine.  We don't do cross-
# compiles except for arm/mips, so the HOST is whatever we are
# running on

UNAME := $(shell uname -sm)

# HOST_OS
ifneq (,$(findstring Linux,$(UNAME)))
  HOST_OS := linux
endif

# BUILD_OS is the real host doing the build.
ifeq ($(HOST_OS),linux)
BUILD_OS := $(HOST_OS)
endif

ifeq ($(HOST_OS),)
$(error Unable to determine HOST_OS from uname -sm: $(UNAME)!)
endif

# HOST_ARCH
ifneq (,$(findstring x86_64,$(UNAME)))
  HOST_ARCH := x86_64
  HOST_2ND_ARCH := x86
  HOST_IS_64_BIT := true
endif

BUILD_ARCH := $(HOST_ARCH)
BUILD_2ND_ARCH := $(HOST_2ND_ARCH)

ifeq ($(HOST_ARCH),)
$(error Unable to determine HOST_ARCH from uname -sm: $(UNAME)!)
endif

# the host build defaults to release, and it must be release or debug
ifeq ($(HOST_BUILD_TYPE),)
HOST_BUILD_TYPE := release
endif

ifneq ($(HOST_BUILD_TYPE),release)
ifneq ($(HOST_BUILD_TYPE),debug)
$(error HOST_BUILD_TYPE must be either release or debug, not '$(HOST_BUILD_TYPE)')
endif
endif

# We don't want to move all the prebuilt host tools to a $(HOST_OS)-x86_64 dir.
HOST_PREBUILT_ARCH := x86
# This is the standard way to name a directory containing prebuilt host
# objects. E.g., prebuilt/$(HOST_PREBUILT_TAG)/cc
ifeq ($(HOST_OS),linux)
  HOST_PREBUILT_TAG := linux
else
  HOST_PREBUILT_TAG := $(HOST_OS)-$(HOST_PREBUILT_ARCH)
endif

include $(BUILD_SYSTEM)/product-config.mk

# Boards may be defined under target/linux/$(TARGET_DEVICE)
# Search in both places, make sure only one exists.
# Real boards should always be associated with an OEM vendor.
board_config_mk := \
	$(strip $(wildcard \
		$(shell test -d target && find target -maxdepth 4 -path '*/$(TARGET_DEVICE)/BoardConfig.mk') \
	))
ifeq ($(board_config_mk),)
  $(error No config file found for TARGET_DEVICE $(TARGET_DEVICE))
endif
ifneq ($(words $(board_config_mk)),1)
  $(error Multiple board config files for TARGET_PRODUCT $(TARGET_PRODUCT): $(board_config_mk))
endif
include $(board_config_mk)

# Set up version information
include $(BUILD_SYSTEM)/version.mk

ifeq ($(TARGET_ARCH),)
  $(error TARGET_ARCH not defined by board config: $(board_config_mk))
endif
TARGET_DEVICE_DIR := $(patsubst %/,%,$(dir $(board_config_mk)))
board_config_mk :=

# ---------------------------------------------------------------
# Set up configuration for target machine.
# The following must be set:
# 		TARGET_OS = { linux }
# 		TARGET_ARCH = { arm | x86 | mips }

TARGET_OS := linux
# TARGET_ARCH should be set by BoardConfig.mk and will be checked later
ifneq ($(filter %64,$(TARGET_ARCH)),)
TARGET_IS_64_BIT := true
endif

# the target build type defaults to release
ifneq ($(TARGET_BUILD_TYPE),debug)
TARGET_BUILD_TYPE := release
endif

ifeq ($(PRINT_BUILD_CONFIG),)
PRINT_BUILD_CONFIG := true
endif

ifeq ($(OUT_DIR),)
OUT_DIR := out/$(TARGET_BOARD)
endif

include $(BUILD_SYSTEM)/dumpvar.mk
