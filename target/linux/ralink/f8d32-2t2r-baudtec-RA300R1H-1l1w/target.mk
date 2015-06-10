ARCH:=mipsel
SUBTARGET:=f8d32-2t2r-baudtec-RA300R1H-1l1w
BOARDNAME:=FLASH 8M DRAM 32M 2T2R Baudtec board RA300R1H 1 Lan 1 Wan
FEATURES:=squashfs
EZP_PLATFORM:=BAB
EZP_SWTYPE:=esw

define Target/Description
	Build firmware images for Ralink/rt305x based boards
endef

