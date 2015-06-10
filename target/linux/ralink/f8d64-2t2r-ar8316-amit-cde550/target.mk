ARCH:=mipsel
SUBTARGET:=f8d64-2t2r-ar8316-amit-cde550
BOARDNAME:=FLASH 8M DRAM 64M AMIT CDE550
FEATURES:=squashfs usb
EZP_PLATFORM:=AMC
EZP_SWTYPE:=ar8316

define Target/Description
	Build firmware images for Ralink/rt305x based boards with AR8316 giga switch
    and nor flash for AMIT CDE570
endef

