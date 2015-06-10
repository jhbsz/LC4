#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mipsel
BOARD:=ralink-7620
BOARDNAME:=Ralink MT7620
SUBTARGETS:=mt7620-evb mt7620-als mt7620-x2\

LINUX_VERSION:=2.6.36

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += 

define Target/Description
	Build firmware images for Ralink RT305X based boards
endef

$(eval $(call BuildTarget))
