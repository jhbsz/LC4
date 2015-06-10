# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 12228 2008-08-06 22:43:15Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=barry
PKG_VERSION:=0.16
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2

include $(INCLUDE_DIR)/package.mk

define Package/barry
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=barry
  DEPENDS:=+libiconv +zlib +libusb
endef

CONFIGURE_VARS += \
	LDFLAGS="-L. -L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -L$(PKG_BUILD_DIR)/src/.libs -lz -liconv -lusb" 

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" 
endef

define Package/barry/install
	mkdir -p $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/tools/.libs/pppob $(1)/usr/sbin/
	mkdir -p $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/src/.libs/libbarry.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/lib
	$(CP) $(TARGET_CROSS_DEF_DIR)/lib/libstdc++.so.* $(1)/lib/
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/ifup.barry $(1)/sbin
	$(INSTALL_DIR) $(1)/etc/chatscripts
	$(INSTALL_BIN) ./files/barry.chat $(1)/etc/chatscripts/
	$(INSTALL_DIR) $(1)/etc/hotplug.d/usb/
	$(CP) files/04-barry $(1)/etc/hotplug.d/usb/
endef

$(eval $(call BuildPackage,barry))
