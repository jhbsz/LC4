ARCH:=mipsel
SUBTARGET:=rt3352-f8d32-1t1r-1l1w
BOARDNAME:=FLASH 16M DRAM 32M RT3352 1T1R 1L1W
FEATURES:=squashfs usb
EZP_PLATFORM:=AMD
EZP_SWTYPE:=ar8328

define Target/Description
	Build firmware images for Ralink rt3352 based boards
endef

