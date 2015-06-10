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

PKG_NAME:=usbmuxd
PKG_VERSION:=1.0.9
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

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

include $(INCLUDE_DIR)/package.mk

define Package/usbmuxd
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=USB muxd
	DEPENDS:=+libusb-1.0 +libusbmuxd +libimobiledevice +libplist +libssl +libcrypto
endef

define Build/Configure
	$(call Build/Configure/Default)
endef

define Package/usbmuxd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/src/.libs/usbmuxd $(1)/usr/sbin/
	$(CP) /opt/toolchain-mipsel_gcc4.2.4/lib/libstdc++.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,usbmuxd))
