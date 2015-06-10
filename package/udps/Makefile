# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=udps
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk


define Package/udps
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=udps - Abocom network test daemon.
endef

define Package/udps/description
	A network test program for Abocom factory test.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram -L$(STAGING_DIR)/usr/lib/ -lnvram -lshared "

endef

define Package/udps/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/udps.init $(1)/etc/init.d/udps
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/udps $(1)/usr/sbin

endef
$(eval $(call BuildPackage,udps))
