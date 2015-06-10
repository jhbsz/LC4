ARCH:=mipsel
SUBTARGET:=f8d32-2t2r-baudtec-RA300R4G-4l1w
BOARDNAME:=FLASH 8M DRAM 32M 2T2R Baudtec board RA300R4G 4 Lan 1 Wan
FEATURES:=squashfs
EZP_PLATFORM:=BAD
EZP_SWTYPE:=ar8316

define Target/Description
	Build firmware images for Ralink/rt305x based boards
endef

