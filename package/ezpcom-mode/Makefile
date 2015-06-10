#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 10347 2008-02-02 01:21:31Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezpcom-mode
PKG_VERSION:=1.0
PKG_RELEASE:=2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezpcom-mode
  SECTION:=net
  CATEGORY:=Network
  TITLE:=EZPacket USB mode tool
  DEPENDS:=+libusb +ezp-nvram
endef

define Package/ezpcom-mode/description
 ezpcom-mode is EZ Packet USB mode tool.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(EZP_CFLAGS) -I. -I$(BUILD_DIR)/ezp-nvram -I$(STAGING_DIR)/usr/include -L$(STAGING_DIR)/usr/lib -lusb -lnvram -lshared "
endef
	
define Package/ezpcom-mode/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezpcom-mode $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/hotplug.d/{usb,scsi_generic}
	$(INSTALL_BIN) ./files/02-wwan-usb $(1)/etc/hotplug.d/usb/02-wwan
	$(INSTALL_BIN) ./files/10-wwan-sg $(1)/etc/hotplug.d/scsi_generic/10-wwan
endef

$(eval $(call BuildPackage,ezpcom-mode))
