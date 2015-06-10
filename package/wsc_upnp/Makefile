# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=wsc-upnp
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/wsc-upnp
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=Ralink WPS(WiFi Portected Setup) UPNP Module
  DEPENDS:=+libupnp +ezp-wps
endef

define Package/wsc-upnp/description
	Ralink WPS(WiFi Portected Setup) UPNP Module.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS)
endef

define Package/wsc-upnp/install
	$(INSTALL_DIR) $(1)/etc/xml/
	$(INSTALL_DATA) ./files/xml/* $(1)/etc/xml
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wscd $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) ./files/wps-ctrl $(1)/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/wps.init $(1)/etc/init.d/wps
endef
$(eval $(call BuildPackage,wsc-upnp))
