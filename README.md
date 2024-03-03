# CorneECRevival

Revival project of the Corne ECWL.

![Corneecrevival](https://github.com/Cipulot/CorneECRevival/blob/main/Docs/images/corne_hold.jpg?raw=true)

Left half of Corne EC Revival Rev1.

## **DEPRECATED**

Please be advised that this project has been deprecated, and as of now, it is no longer supported. Regrettably, assistance or help cannot be provided for this deprecated project. The firmware is no longer targeting QMK main, and uses propositions that are now no lonfer working, making the project unusable.

The newer version, Corne EC, will have lots of new feature and will be run as a proper project that you can buy and enjoy right away.

## Introduction

The Corne EC Revival is my personal effort to revive the original Corne ECWL by [sekigon-gonnoc](https://github.com/sekigon-gonnoc).

It boosts full compatibility with the original boards sold by gonnoc while also providing updated QMK support for the latest stable release and a modified set of plates and pcbs, making it compatible with "standard" Corne accessories.

## PCB and Plate

Contrary to the standard Corne, the Corne EC Revival PCBs are not interchangable between left and right half. Plates on the other hand can be used on both sides.

In the [PCB folder](https://github.com/Cipulot/CorneECRevival/blob/main/PCB) there are all the PCB and plate design files. gerber files are in the [Production folder](https://github.com/Cipulot/CorneECRevival/blob/main/Production). The features of each revision are listed in the next section.

The naming scheme reflects the mounting system supported:

- `OG_mounting_[partname]` supports "standard" Corne mounting points
- `[partname]` supports the original Corne ECWL mounting points

Furthermore, to allow for a greater support, both NIZ and Topre housing plates are available for both mounting point configurations.

To see images and further description of the PCB refer to the dedicated section in the Notion build log [here](https://cipulot.notion.site/PCB-Design-9a60de99e9044c61900d82e630ff782b).

## Revisions

### Revision 1 (Rev1)

The Rev1 is the first iteration of the design, 1:1 compatible with the original Corne ECWL from Gonnoc.

The features are therefore shared with the original board with the extension of onboard Trackball and OLED.

Both ECWL mounting system and Standard Corne mounting points are supported with the respective PCB versions.

### Revision 2 (Rev2)

The Rev2 is a departure form the original design approach. It's a 'Ready to go!" board, meaning the MCU and complementary circuit is already onboard.

Added features compared from the Rev1 include:

- Onboard USB-C with ESD protection
- STM32F4x1 MCU for better performance, both cycle wise and in ADC performance compared to 32U4 (also 32U4 prices are to the moon as of now)
- APC (Actuation Point Changer) with predefined actuation points and full scale analog switch control (full board / per key)

Both NIZ and Topre are still supported.

## Production files

Gerber files are provided for all the pcbs of the project. BOM is also available to use. Placement files are WIP.

The prototypes were produced by JLC, therefore pretty much any PCB manufacturer can handle production without any problems.

## Firmware

The firmware folder contains 2 versions of the code based on different QMK releases:

- `EC Revival` based on the latest QMK stable
- `Original ECWL` based on gonnoc's fork, running on an older QMK branch

For the `EC Revival` there are going to be 2 configurations, one for the 32U4 (Rev1) and another for STM32F4x1 (Rev2).

Both have precompiled `*.hex` files based on the code in their respective folders to easily get the board up and running.

## License

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.
