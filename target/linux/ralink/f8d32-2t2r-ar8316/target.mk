ARCH:=mipsel
SUBTARGET:=f8d32-2t2r-ar8316
BOARDNAME:=FLASH 8M DRAM 32M 2T2R with AR8316 Giga Switch
FEATURES:=squashfs usb
EZP_PLATFORM:=ABD
EZP_SWTYPE:=ar8316

define Target/Description
	Build firmware images for Ralink/rt305x based boards with AR8316 giga switch
endef

