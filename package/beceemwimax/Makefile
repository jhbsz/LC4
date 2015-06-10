#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=beceemwimax
PKG_VERSION:=1.0.0
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/beceemwimax
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libopenssl
  TITLE:=Beceem WiMAX driver
  URL:=http://pptpclient.sourceforge.net/
endef

define Package/beceemwimax/description
	Beceem WiMAX USB dangle  driver
endef

EZP_CFLAGS+= -I $(TOPDIR)/package/ezp-nvram/src -I $(TOPDIR)/package/ezp-nvram/include

define Build/Compile
	$(call Build/Compile/Default, \
		CFLAGS="$(TARGET_CFLAGS) $(EZP_CFLAGS) -fPIC" CROSS_COMPILE=$(TARGET_CROSS) \
	)
endef

define Package/beceemwimax/install	
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin_linux/bin/BeceemWiMaxApp $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin_linux/bin/tgtxvi020 $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin_linux/bin/WiMaxCommonTestApp $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin_linux/bin/libxvi020.so.05.02.93 $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin_linux/bin/libxvi020.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bcmWiMaxSwitch/switchmode $(1)/usr/bin
	$(INSTALL_DIR) -p $(1)/etc/
	$(INSTALL_BIN) ./files/RemoteProxy.cfg $(1)/etc/
	$(INSTALL_DIR) -p $(1)/etc/ssl/certs
	$(INSTALL_DIR) -p $(1)/etc/Sprint4G
	$(INSTALL_BIN) ./files/*.pem $(1)/etc/ssl/certs/
	$(INSTALL_BIN) ./files/passphrase-o-tron $(1)/usr/bin/
	$(INSTALL_BIN) ./files/PreCmoCNM-CMU30X $(1)/usr/bin/
	$(INSTALL_BIN) ./files/U30X_set_iface $(1)/usr/bin/
	$(INSTALL_BIN) ./files/bcmswitchmode $(1)/usr/bin/
	$(INSTALL_DIR) -p $(1)/lib/firmware
	$(INSTALL_BIN) ./files/macxvi.cfg.g1 $(1)/lib/firmware/macxvi.cfg
	$(INSTALL_BIN) ./files/macxvi200.bin.g1 $(1)/lib/firmware/macxvi200.bin
	$(INSTALL_BIN) ./files/wimaxd.conf* $(1)/etc/Sprint4G
	$(INSTALL_DIR) $(1)/etc/ssl/certs/
	$(INSTALL_BIN) ./files/vmax.pem $(1)/etc/ssl/certs/
	$(INSTALL_BIN) ./files/g1.pem $(1)/etc/ssl/certs/
	$(INSTALL_DIR) $(1)/etc/hotplug.d/usb/
	$(INSTALL_BIN) ./files/05-beceem $(1)/etc/hotplug.d/usb/
	$(INSTALL_DIR) $(1)/sbin/
	$(INSTALL_BIN) ./files/ifup.beceem $(1)/sbin/
#	udev rule?
#	$(INSTALL_BIN) ./files/85-CmoCNM-CMU30X.rules $(1)/etc/
endef

define Build/InstallDev
	$(INSTALL_DIR) -p $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin_linux/bin/*so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,beceemwimax))
