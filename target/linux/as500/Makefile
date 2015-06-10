#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=mips
BOARD:=as500
BOARDNAME:=Acrospeed AS500
LINUX_VERSION:=2.6.26

include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += 

define Target/Description
	Build firmware images for Acrospeed AS500 based boards
endef

$(eval $(call BuildTarget))
