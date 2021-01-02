#include <stdlib.h>

// These are paths to folders in /sys which contain "uevent" file
// need to init this device.
// MultiROM needs to init framebuffer, mmc blocks, input devices,
// some ADB-related stuff and USB drives, if OTG is supported
// You can use * at the end to init this folder and all its subfolders
const char *mr_init_devices[] =
{
    // FrameBuffers
    "/sys/class/graphics/fb0",
    "/sys/devices/platform/vfb.0/graphics/fb0",
    "/sys/bus/platform/drivers/msmdrm_smmu",
    "/sys/bus/platform/drivers/drm_dsi_ctrl",
    "/sys/bus/platform/drivers/msm_drm",
    "/sys/module/msm_drm",
    "/sys/module/drm_kms_helper",
    "/sys/module/drm",
    "/sys/devices/platform/soc/ae00000.qcom,mdss_mdp/drm*",

    // Storage devices
    "/sys/dev/block/*",
    "/sys/block/sda",
    "/sys/block/sdb",
    "/sys/block/sdc",
    "/sys/block/sdd",
    "/sys/block/sde",
    "/sys/block/sdf",
    "/sys/module/mmcblk",

    // Internal storage
    "/sys/devices/platform/soc/1d84000.ufshc",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:0",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:1",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:2",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:3",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:4",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:5",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:0/block/sda",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:1/block/sdb",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:2/block/sdc",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:3/block/sdd",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:4/block/sde",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:5/block/sdf",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:0/block/sda/*",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:1/block/sdb/*",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:2/block/sdc/*",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:3/block/sdd/*",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:4/block/sde/*",
    "/sys/devices/platform/soc/1d84000.ufshc/host0/target0:0:0/0:0:0:5/block/sdf/*",


    // External storage
    //"/sys/block/mmcblk1",
    //"/sys/block/mmcblk1",
    //"/sys/block/mmcblk1/mmcblk1p1",
    //"/sys/block/mmcblk1/mmcblk1p2",

    // Devices
    /*"/sys/bus*",
    "/sys/bus/mmc",
    "/sys/bus/mmc/drivers/mmcblk",
    "/sys/devices*",*/
    "/sys/devices/virtual/misc/binder",
    "/sys/devices/virtual/misc/vndbinder",
    "/sys/devices/virtual/misc/hwbinder",
    "/sys/module/binder",
    "/sys/module/binder_alloc",
    "/sys/devices/virtual/mem/null",
    "/sys/devices/virtual/misc/fuse",

    // Inputs
    "/sys/class/input/event3",
    "/sys/class/input/event2",
    "/sys/class/input/input2",
    "/sys/class/input/input3",
    "/sys/class/misc/uinput",
    "/sys/class/tty/ptmx",
    "/sys/devices/platform/soc/soc:gpio_keys/input*",
    "/sys/devices/virtual/input*",
    "/sys/devices/virtual/misc/uinput",
    "/sys/devices/platform/soc/a98000.i2c/i2c-3/3-0062",

    // ADB
    "/sys/devices/virtual/tty/ptmx",
    "/sys/bus/usb",
    "/sys/bus/usb-serial",
    "/sys/class/android_usb/android0/f_adb",
    "/sys/class/misc/android_adb",
    "/sys/devices/virtual/misc/mtp_usb",

    // USB
    "/sys/bus/usb",
    "/sys/bus/platform/drivers/xhci-hcd*",
    "/sys/devices/platform/soc/a800000.ssusb/a800000.dwc3/",
    "/sys/devices/platform/soc/c200000.hsusb/c200000.dwc3/",

    // Encryption
    "/sys/devices/virtual/icesdcc/iceufs",
    "/sys/devices/virtual/misc/device-mapper",
    "/sys/devices/virtual/misc/ion",
    "/sys/devices/virtual/qseecom/qseecom",
    "/sys/bus/platform/devices/soc:qcom,qsee_ipc_irq_bridge",
    "/sys/bus/platform/devices/86d00000.qseecom",
    "/sys/bus/platform/drivers/qseecom",
    "/sys/bus/platform/drivers/qsee_ipc_irq_bridge",
    "/sys/devices/platform/soc/soc:qcom,qsee_ipc_irq_bridge",
    "/sys/devices/virtual/qsee_ipc_irq_bridge/qsee_ipc_irq_spss",
    "/sys/devices/platform/soc/86d00000.qseecom",
    "/sys/bus/platform/drivers/ion-msm",
    "/sys/devices/platform/soc/soc:qcom,ion*",
    "/sys/devices/platform/soc/1d84000.ufshc*",
    "/sys/module/sg",
    "/sys/devices/virtual/mem/urandom",

    // Logging
    "/sys/devices/virtual/mem/kmsg",

    NULL
};
