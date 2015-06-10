ARCH:=mipsel
SUBTARGET:=f8d32-2t2r-4615g
BOARDNAME:=FLASH 8M DRAM 32M 2T2R ABOCOM 4615
FEATURES:=squashfs usb
EZP_PLATFORM:=ABF
EZP_SWTYPE:=ar8316

define Target/Description
	Build firmware images for Ralink/rt305x based boards with AR8316 giga switchfor ABOCOM 4615
endef

