ARCH:=mipsel
SUBTARGET:=rt5350-f8d32-1t1r-1l1w
BOARDNAME:=FLASH 8M DRAM 32M RT5350 1T1R 1L1W
FEATURES:=squashfs usb
EZP_PLATFORM:=AME
EZP_SWTYPE:=esw

define Target/Description
	Build firmware images for Ralink rt5350 based boards
endef

