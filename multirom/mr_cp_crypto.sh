#!/bin/sh
# $1 - path to top of android build tree
# $2 path to /multirom/enc folder in installation zip

# Constants
DEVICE_PATH="device/xiaomi/beryllium";

# Import decrypt files
mkdir -p "$2/vendor/firmware";
mkdir -p "$2/vendor/lib64/hw";
cp -ra "$1/${DEVICE_PATH}/recovery/root/vendor/lib64/"*".so" "$2/";
cp -ra "$1/${DEVICE_PATH}/recovery/root/sbin/"*".so" "$2/";
cp -ra "$1/${DEVICE_PATH}/recovery/root/vendor/compatibility_matrix.xml"* "$2/vendor/";
cp -ra "$1/${DEVICE_PATH}/recovery/root/vendor/manifest.xml"* "$2/vendor/";
cp -ra "$1/${DEVICE_PATH}/recovery/root/vendor/firmware/"* "$2/vendor/firmware/";
cp -ra "$1/${DEVICE_PATH}/recovery/root/vendor/lib64/hw/"* "$2/vendor/lib64/hw/";
cp -ra "$1/${DEVICE_PATH}/multirom_enc_blobs/"* "$2/";
