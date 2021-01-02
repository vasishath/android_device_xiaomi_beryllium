#
# Copyright 2018 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This contains the module build definitions for the hardware-specific
# components for this device.
#
# As much as possible, those components should be built unconditionally,
# with device-specific names to avoid collisions, to avoid device-specific
# bitrot and build breakages. Building a component unconditionally does
# *not* include it on all devices, so it is safe even with hardware-specific
# components.

DEVICE_PATH := device/xiaomi/beryllium

# Architecture
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := kryo

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := cortex-a53

ENABLE_CPUSETS := true
ENABLE_SCHEDBOOST := true

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := sdm845
TARGET_NO_BOOTLOADER := true

# Kernel
BOARD_KERNEL_CMDLINE := console=ttyMSM0,115200n8 earlycon=msm_geni_serial,0xA84000 androidboot.hardware=qcom
BOARD_KERNEL_CMDLINE += androidboot.console=ttyMSM0 video=vfb:640x400,bpp=32,memsize=3072000 msm_rtb.filter=0x237
BOARD_KERNEL_CMDLINE += ehci-hcd.park=3 lpm_levels.sleep_disabled=1 service_locator.enable=1 swiotlb=2048
BOARD_KERNEL_CMDLINE += androidboot.configfs=true androidboot.usbcontroller=a600000.dwc3
BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive printk.devkmsg=on
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_PAGESIZE := 4096
BOARD_KERNEL_TAGS_OFFSET := 0x00000100
BOARD_RAMDISK_OFFSET := 0x01000000
TARGET_PREBUILT_KERNEL := device/xiaomi/beryllium/prebuilt/Image.gz-dtb
TARGET_KERNEL_CONFIG := beryllium_defconfig
BOARD_KERNEL_IMAGE_NAME := Image.gz-dtb

#BOARD_KERNEL_IMAGE_NAME := Image.gz-dtb
#TARGET_KERNEL_ARCH := arm64
#TARGET_KERNEL_HEADER_ARCH := arm64
#TARGET_KERNEL_CONFIG := beryllium_user_defconfig
#TARGET_KERNEL_SOURCE := kernel/xiaomi/sdm845
#TARGET_KERNEL_CROSS_COMPILE_PREFIX := aarch64-linux-android-

# Platform
TARGET_BOARD_PLATFORM := sdm845
TARGET_BOARD_PLATFORM_GPU := qcom-adreno630

# Partitions
BOARD_FLASH_BLOCK_SIZE := 262144 # (BOARD_KERNEL_PAGESIZE * 64)

BOARD_BOOTIMAGE_PARTITION_SIZE := 67108864
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 67108864
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3221225472
BOARD_USERDATAIMAGE_PARTITION_SIZE := 57453555712
BOARD_USERDATAIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_VENDORIMAGE_PARTITION_SIZE := 788529152

TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true

# Workaround for error copying vendor files to recovery ramdisk
BOARD_VENDORIMAGE_FILE_SYSTEM_TYPE := ext4
TARGET_COPY_OUT_VENDOR := vendor

# Recovery
TARGET_OTA_ASSERT_DEVICE := beryllium
BOARD_HAS_LARGE_FILESYSTEM := true
BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_RECOVERY_PIXEL_FORMAT := "BGRA_8888"

# Android Verified Boot
BOARD_AVB_ENABLE := false
BOARD_BUILD_DISABLED_VBMETAIMAGE := true

FLEX_EXEC := flex
BISON_EXEC := bison

# Crypto
TARGET_HW_DISK_ENCRYPTION := true
#TARGET_HW_DISK_ENCRYPT_NEW := true
TW_INCLUDE_CRYPTO := true
TW_INCLUDE_CRYPTO_FBE := true
TARGET_CRYPTFS_HW_PATH := vendor/qcom/opensource/cryptfs_hw

# TWRP specific build flags
BOARD_HAS_NO_REAL_SDCARD := true
TARGET_RECOVERY_QCOM_RTC_FIX := true
TW_BRIGHTNESS_PATH := "/sys/class/backlight/panel0-backlight/brightness"
TW_DEFAULT_BRIGHTNESS := 2047
TW_EXCLUDE_DEFAULT_USB_INIT := true
#TW_EXCLUDE_SUPERSU := true
TW_EXTRA_LANGUAGES := true
TW_INCLUDE_NTFS_3G := true
TW_INPUT_BLACKLIST := "hbtp_vm"
TW_USE_LEDS_HAPTICS := true
TW_MAX_BRIGHTNESS := 4095
TW_SCREEN_BLANK_ON_BOOT := true
TW_THEME := portrait_hdpi
# MTP seems to cause a kernel panic, at least with some kernels, so disable it for now
#TW_EXCLUDE_MTP := true
TWRP_INCLUDE_LOGCAT := true
TARGET_USES_LOGD := true
PLATFORM_SECURITY_PATCH := 2099-12-31
PLATFORM_VERSION := 16.1.0

# MultiROM configuration
MR_DEVICE_HOOKS := $(DEVICE_PATH)/multirom/mr_hooks.c
MR_DEVICE_HOOKS_VER := 6
MR_DEVICE_BOOTDEVICE := /dev/block/platform/soc/1d84000.ufshc
MR_DPI := xhdpi
MR_DPI_FONT := 340
MR_ENCRYPTION := true
MR_ENCRYPTION_FAKE_PROPERTIES := true
MR_ENCRYPTION_FAKE_PROPERTIES_EXTRAS := $(DEVICE_PATH)/multirom/mr_fake_properties.c
MR_ENCRYPTION_SETUP_SCRIPT := $(DEVICE_PATH)/multirom/mr_cp_crypto.sh
MR_FSTAB := $(DEVICE_PATH)/recovery.fstab
MR_INIT_DEVICES := $(DEVICE_PATH)/multirom/mr_init_devices.c
MR_INPUT_TYPE := type_b
MR_KEXEC_MEM_MIN := 0x86000000
MR_NO_KEXEC := enabled
MR_PIXEL_FORMAT := "BGRA_8888"
MR_UNIFIED_TABS := true
MR_USE_MROM_FSTAB := true
MR_EXTRA_FIRMWARE_DIR := "/mrom_enc/vendor/firmware"
MR_FIRMWARE_DIR := "/vendor/firmware_mnt"
RECOVERY_GRAPHICS_USE_LINELENGTH := true
DEVICE_RESOLUTION := 1080x2160
TARGET_RECOVERY_IS_MULTIROM := true
MR_DEVICE_HAS_VENDOR_PARTITION := true
MR_DEVICE_HAS_DRM_GRAPHICS := true
MR_CUSTOM_FORBIDDEN_PARTITIONS := $(DEVICE_PATH)/multirom/mr_forbidden_partitions.cpp
