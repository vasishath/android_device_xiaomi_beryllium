# android_device_xiaomi_beryllium
Tree for building TWRP for Xiaomi Pocophone F1

## To compile

export ALLOW_MISSING_DEPENDENCIES=true

. build/envsetup.sh && lunch omni_beryllium-eng

mka adbd recoveryimage

## Device specifications

Basic   | Spec Sheet
-------:|:-------------------------
CPU     | Octa-core (4x2.8 GHz Kryo 385 Gold & 4x1.8 GHz Kryo 385 Silver)
Chipset | Qualcomm SDM845 Snapdragon 845
GPU     | Adreno 630
Memory  | 6/8 GB RAM
Shipped Android Version | 8.1
Storage | 64/128/256 GB
Battery | Li-Po 4000 mAh battery
Dimensions | 155.5 x 75.3 x 8.8 mm (6.12 x 2.96 x 0.35 in)
Display | 1080 x 2246 pixels, 18.7:9 ratio (~403 ppi density)
Rear Camera 1  | 12 MP, f/1.9, 1/2.55", 1.4µm, dual pixel PDAF
Rear Camera 2 | 5 MP, f/2.0, 1.12µm, depth sensor
Front Camera | 20 MP, f/2.0, 0.9µm


## Device picture

![Xiaomi Pocophone F1](https://xiaomi-mi.com/uploads/CatalogueImage/02k_17131_1534951297.jpg "Xiaomi Pocophone F1 in Armoured Edition")
