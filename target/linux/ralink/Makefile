#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mipsel
BOARD:=ralink
BOARDNAME:=Ralink RT305X
SUBTARGETS:=f4d16-1t1r f4d32-1t1r f8d32-1t1r f4d32-2t2r f8d32-2t2r \
			f8d64-2t2r f8d32-2t2r-ar8316 f8d32-2t2r-4615g \
			f8d64-2t2r-ar8316 f8d64-2t2r-5205g f16d64-2t2r-5205g \
		    f8d32-1t1r-amit f8d32-1t1r-amit-cdw531ac f8d32-2t2r-baudtec \
			f8d64-2t2r-ar8316-amit-cde550 f8d64-2t2r-ar8316-spi-amit-cde570 \
			f8d32-2t2r-baudtec-RA300R1H-1l1w f8d32-2t2r-baudtec-RA300R4G-4l1w \
			f8d32-2t2r-baudtec-RA300R4H-4l1w f8d32-1t1r-poray-R50E \
			rt5350-f8d32-1t1r-1l1w f16d64-2t2r-ar8316-spi-amit-cde570-prox \
			rt5350-f8d32-1t1r-1l1w-m1 rt5350-f8d32-1t1r-1l1w-bee \
			rt5350-f8d32-1t1r-1l1w-sony rt3352-f8d32-2t2r-1l1w \
			f8d32-2t2r-baudtec-RA300R4G-4l1w-mac-sense \
			rt3352-f16d64-2t2r-3l1w-gma rt5350-f8d32-1t1r-1l1w-music
 

LINUX_VERSION:=2.6.26

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += 

define Target/Description
	Build firmware images for Ralink RT305X based boards
endef

$(eval $(call BuildTarget))
