# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-power-detect
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-power-detect
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Power-detect - EZ Packet power detection daemon.
endef

define Package/ezp-power-detect/description
	A power detect server for EZ Packet Products.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR)"

endef

define Package/ezp-power-detect/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(CP) ./files/power-detect.init $(1)/etc/init.d/power-detect
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/power-detect $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-power-detect))
