ARCH:=mipsel
SUBTARGET:=f16d64-2t2r-ar8316-spi-amit-cde570-prox
BOARDNAME:=SPI FLASH 16M DRAM 64M 2T2R AMIT CDE570 PROX
FEATURES:=squashfs usb
EZP_PLATFORM:=AMD
EZP_SWTYPE:=ar8316

define Target/Description
	Build firmware images for Ralink/rt305x based boards with AR8316 giga switch
    and spi flash for AMIT CDE570
endef

