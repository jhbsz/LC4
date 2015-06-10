# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-wps
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk


define Package/ezp-wps
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=WPS tool - EZ Packet WPS tool.
endef

define Package/ezp-wps/description
	A wps tool for EZ Packet Products.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I$(LINUX_DIR)/include"

endef

define Package/ezp-wps/install
#	$(INSTALL_DIR) $(1)/etc/init.d/
#	$(CP) ./files/power-detect.init $(1)/etc/init.d/power-detect
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezp-wps $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-wps))
