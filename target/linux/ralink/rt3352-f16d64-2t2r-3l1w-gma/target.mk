ARCH:=mipsel
SUBTARGET:=rt3352-f16d64-2t2r-3l1w-gma
BOARDNAME:=FLASH 16M DRAM 64M RT3352 2T2R 3L1W for Garmin project(GMA)
FEATURES:=squashfs usb
EZP_PLATFORM:=GMA
EZP_SWTYPE:=ar8328

define Target/Description
	Build firmware images for Ralink rt3352 based boards for Garmin project
endef

