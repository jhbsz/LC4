 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 12228 2008-08-06 22:43:15Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=ifuse
PKG_VERSION:=1.1.3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
LDFLAGS+=-L$(STAGING_DIR)/lib

include $(INCLUDE_DIR)/package.mk

CFLAGS+=-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/usr/include/libusb-1.0
TARGET_CFLAGS+=-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/usr/include/libusb-1.0
LDFLAGS+=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lxml2 -lplist -lz -lplist++ -limobiledevice -lssl -lcrypto -lusbmuxd -lusb-1.0
TARGET_LDFLAGS+=-L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib -lxml2 -lplist -lz -lplist++ -limobiledevice -lssl -lcrypto -lusbmuxd -lusb-1.0

TARGET_CONFIGURE_OPTS = \
    libplistmm_CFLAGS="-I$(STAGING_DIR)/usr/include" \
    libplistmm_LIBS="-L$(STAGING_DIR)/usr/lib -lplist -lplist++" \
    libusb_CFLAGS="-I$(STAGING_DIR)/usr/include" \
    libusb_LIBS="-L$(STAGING_DIR)/usr/lib -lplist -lplist++" \
    libimobiledevice_CFLAGS="-I$(STAGING_DIR)/usr/include" \
    libimobiledevice_LIBS="-L$(STAGING_DIR)/usr/lib -limobiledevice"


define Package/ifuse
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=ifuse
  DEPENDS:=+libimobiledevice +libfuse +fuse-utils +libgnutls +libgcrypt +libgpg-error +kmod-usb-net-iphone
endef


define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(EZP_CFLAGS) -I. -I$(STAGING_DIR)/usr/include -L$(STAGING_DIR)/usr/lib"
		DESTDIR="$(PKG_INSTALL_DIR)"
endef

define Package/ifuse/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/ifuse $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/root/.config/libimobiledevice/
	$(INSTALL_DIR) $(1)/etc/hotplug.d/usb/
	$(CP) files/usb/* $(1)/etc/hotplug.d/usb/
	$(CP) files/etc/* $(1)/etc
endef

$(eval $(call BuildPackage,ifuse))
