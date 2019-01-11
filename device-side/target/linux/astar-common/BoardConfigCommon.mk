-include target/linux/generic/common.mk

TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_CPU_SMP := true
TARGET_LINUX_VERSION:=3.4
TARGET_ARCH := arm
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_VARIANT := cortex-a7

TARGET_ARCH_PACKAGES := sunxi

TARGET_BOARD_PLATFORM := astar
