# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=watchdog
PKG_VERSION:=5.6

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/watchdog
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Watchdog daemon
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" 
endef

define Package/watchdog/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/watchdog $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_BIN) ./files/watchdog.conf $(1)/etc/
	$(INSTALL_BIN) ./files/watchdog.upgrade $(1)/etc/
endef

$(eval $(call BuildPackage,watchdog))
