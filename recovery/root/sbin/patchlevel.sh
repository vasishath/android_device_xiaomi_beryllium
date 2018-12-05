#!/sbin/sh

finish()
{
#	umount /v
#	umount /s
#	rmdir /v
#	rmdir /s
	setprop crypto.ready 1
	exit 0
}

osver=$(getprop ro.build.version.release_orig)
patchlevel=$(getprop ro.build.version.security_patch_orig)
suffix=$(getprop ro.boot.slot_suffix)

#venpath="/dev/block/bootdevice/by-name/vendor"
#mkdir /v
#mount -t ext4 -o ro "$venpath" /v
#syspath="/dev/block/bootdevice/by-name/system"
#mkdir /s
#mount -t ext4 -o ro "$syspath" /s

if [ -f /system/build.prop ]; then
	# TODO: It may be better to try to read these from the boot image than from /system
	device=$(grep -i 'ro.product.device' /system/build.prop  | cut -f2 -d'=' -s)
	fingerprint=$(grep -i 'ro.build.fingerprint' /system/build.prop  | cut -f2 -d'=' -s)
	osver=$(grep -i 'ro.build.version.release' /system/build.prop  | cut -f2 -d'=' -s)
	patchlevel=$(grep -i 'ro.build.version.security_patch' /system/build.prop  | cut -f2 -d'=' -s)
	product=$(grep -i 'ro.build.product' /system/build.prop  | cut -f2 -d'=' -s)
	if [ ! -z "$device" ]; then
		resetprop ro.product.device "$device"
		sed -i "s/ro.product.device=.*/ro.product.device="$device"/g" /prop.default ;
	fi
	if [ ! -z "$fingerprint" ]; then
		resetprop ro.build.fingerprint "$fingerprint"
		sed -i "s/ro.build.fingerprint=.*/ro.build.fingerprint="$osver"/g" /prop.default ;
	fi
	if [ ! -z "$osver" ]; then
		resetprop ro.build.version.release "$osver"
		sed -i "s/ro.build.version.release=.*/ro.build.version.release="$osver"/g" /prop.default ;
	fi
	if [ ! -z "$patchlevel" ]; then
		resetprop ro.build.version.security_patch "$patchlevel"
		sed -i "s/ro.build.version.security_patch=.*/ro.build.version.security_patch="$patchlevel"/g" /prop.default ;
	fi
	if [ ! -z "$product" ]; then
		resetprop ro.build.product "$product"
		sed -i "s/ro.build.product=.*/ro.build.product="$product"/g" /prop.default ;
	fi
	if [ -f /vendor/lib/modules/texfat.ko ]; then
		insmod /vendor/lib/modules/texfat.ko
	fi
	finish
else
	# Be sure to increase the PLATFORM_VERSION in build/core/version_defaults.mk to override Google's anti-rollback features to something rather insane
	if [ ! -z "$osver" ]; then
		resetprop ro.build.version.release "$osver"
		sed -i "s/ro.build.version.release=.*/ro.build.version.release="$osver"/g" /prop.default ;
	fi
	if [ ! -z "$patchlevel" ]; then
		resetprop ro.build.version.security_patch "$patchlevel"
		sed -i "s/ro.build.version.security_patch=.*/ro.build.version.security_patch="$patchlevel"/g" /prop.default ;
	fi
	if [ -f /vendor/lib/modules/texfat.ko ]; then
		insmod /vendor/lib/modules/texfat.ko
	fi
	finish
fi
