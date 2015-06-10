# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
#
# Copyright (C) 2008 Frank Cervenka
#
# This is free software, licensed under the GNU General Public License v2.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=libusbmuxd
PKG_VERSION:=1.0.9
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

CFLAGS+=-I$(STAGING_DIR)/include
LDFLAGS+=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lxml2 -lplist -lz
TARGET_LDFLAGS+=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lxml2 -lplist -lz

include $(INCLUDE_DIR)/package.mk

define Package/libusbmuxd
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=library USB muxd
	DEPENDS:=+libusb-1.0 +libxml2 +libplist
endef

define Build/Configure
	$(call Build/Configure/Default)
endef

define Package/libusbmuxd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/src/.libs/libusbmuxd.so* $(1)/usr/lib/
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/usbmuxd.h $(1)/usr/include/
	mkdir -p $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/src/.libs/libusbmuxd.so* $(1)/usr/lib/
	mkdir -p $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_BUILD_DIR)/libusbmuxd.pc $(1)/usr/lib/pkgconfig/
endef

$(eval $(call BuildPackage,libusbmuxd))
