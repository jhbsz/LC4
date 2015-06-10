# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 12228 2008-08-06 22:43:15Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=libplist
PKG_VERSION:=1.11
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)


TARGET_LDFLAGS+=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lxml2 -lz

include $(INCLUDE_DIR)/package.mk
 
define Build/Configure
	$(call Build/Configure/Default,--without-cython)
endef


define Package/libplist
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=A library for accessing iPhone/iPod USB devices
  URL:=http://libusb.sourceforge.net/
  DEPENDS:=+libxml2 +glib2
endef

TARGET_CFLAGS += $(FPIC)

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Build/InstallDev
	mkdir -p $(1)/usr/include
	mkdir -p $(1)/usr/include/plist
	$(CP) $(PKG_INSTALL_DIR)/usr/include/plist/plist.h $(1)/usr/include/plist/
	mkdir -p $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libplist*.so* $(1)/usr/lib/
	mkdir -p $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libplist.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libplist/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libplist*.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libplist))
