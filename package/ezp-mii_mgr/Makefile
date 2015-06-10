include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-mii_mgr
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-mii_mgr
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=test - EZ Packet test Process
  DEPENDS:=ezp-nvram
endef

define Package/ezp-mii_mgr/description
	Command Line Interface.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. "

endef

define Package/ezp-mii_mgr/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mii_mgr $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-mii_mgr))
