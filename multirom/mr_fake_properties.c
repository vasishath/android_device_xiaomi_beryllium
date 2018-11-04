#include <stdlib.h>

// Allows to specify a device/custom specific file for
// additional properties needed by decryption
const char *mr_fake_properties[][2] =
{
    //{property, value}
    { "sys.keymaster.loaded", "true" },               // used by keystore.msm8952.so / keystore.default.so
    { "ro.boot.bootdevice", "1d84000.ufshc" },        // used by libcryptfs_hw.so
    { "ro.crypto.state", "encrypted" },               // used by libcryptfs.so
    { "ro.product.board", "sdm845"},
    { "hwservicemanager.ready", "true"},
    { "ro.property_service.version", "2"},
    { "ro.build.version.release", "9.0.0"},
    { "ro.build.version.security_patch", "2018-10-01"},
    { NULL, NULL}
};
