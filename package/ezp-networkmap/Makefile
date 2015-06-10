# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-networkmap
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-networkmap
  SECTION:=net
  CATEGORY:=Network
  TITLE:=network map.
endef

define Package/ezp-networkmap/description
	Network map for EZ Packet Products.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include \
		-I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram"
endef

define Package/ezp-networkmap/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/networkmap $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-networkmap))
