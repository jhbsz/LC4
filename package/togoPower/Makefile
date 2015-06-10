# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=togoPower
PKG_VERSION:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/togoPower
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Mouse click counter
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)"
endef

define Package/togoPower/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/comTxRx $(1)/usr/sbin
endef

$(eval $(call BuildPackage,togoPower))
