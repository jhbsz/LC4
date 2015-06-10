# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-netstat
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/ezp-netstat
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=A tiny network status daemon
endef

define Package/ezp-netstat/description
	A small daemon constantly watches network flow and failover
endef

TARGET_CFLAGS+= -DDEBUG -I$(STAGING_DIR)/usr/include -I$(PKG_BUILD_DIR) -I$(BUILD_DIR)/ezp-nvram 

EZP_CFLAGS+= -DEZP_PROD_BRAND_$(CONFIG_EZP_SECRET) \

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) $(EZP_CFLAGS)"
endef

define Package/ezp-netstat/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezp-netstat $(1)/bin/
#	$(INSTALL_DIR) $(1)/etc/init.d
#	$(INSTALL_BIN) ./files/ezp-netstat.init $(1)/etc/init.d/ezp-netstat
endef

$(eval $(call BuildPackage,ezp-netstat))
