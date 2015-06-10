ARCH:=mipsel
SUBTARGET:=f8d64-2t2r-ar8316
BOARDNAME:=FLASH 8M DRAM 64M 2T2R with AR8316 Giga Switch
FEATURES:=squashfs usb
EZP_PLATFORM:=ABE
EZP_SWTYPE:=ar8316

define Target/Description
	Build firmware images for Ralink/rt305x based boards with AR8316 giga switch
endef

