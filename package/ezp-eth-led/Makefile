# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-eth-led
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-eth-led
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Power-detect - EZ Packet ethernet detection daemon.
endef

define Package/ezp-eth-led/description
	A ethernet detect server for EZ Packet Products.
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

define Package/ezp-eth-led/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(CP) ./files/ezp-eth-led.init $(1)/etc/init.d/ezp-eth-led
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezp-eth-led $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-eth-led))
