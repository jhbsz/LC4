#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=rt76xx-tools
PKG_RELEASE:=1
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

ifeq ($(CONFIG_TARGET_SW_TYPE), "ar8316")
EZP_CFLAGS+=-DAR8316
endif

ifeq ($(CONFIG_TARGET_SW_TYPE), "ar8328")
EZP_CFLAGS+=-DAR8328
endif

define Package/rt76xx-tools
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Tool for RT76XX
  DEPENDS:=@TARGET_ralink_7620
endef

define Package/rt76xx-tools/description
	Tool for RT76XX
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ./files/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) CFLAGS="$(TARGET_CFLAGS) \
		-I$(LINUX_DIR)/include	$(EZP_CFLAGS)"
endef

define Package/rt76xx-tools/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/switch $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezp-phy-rst $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezp-mac-clone $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,rt76xx-tools))
