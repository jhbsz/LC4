ARCH:=mipsel
SUBTARGET:=f8d32-2t2r-baudtec-RA300R4G-4l1w-mac-sense
BOARDNAME:=FLASH 8M DRAM 32M 2T2R Baudtec board RA300R4G 4 Lan 1 Wan mac sense
FEATURES:=squashfs
# we change EZP_PLATFORM:=BAD_mac_sense to BAD_m because the platform field in
# mkimage header only allow 5 characters
EZP_PLATFORM:=BAD_m
EZP_SWTYPE:=ar8316

define Target/Description
	Build firmware images for Ralink/rt305x based boards
endef

