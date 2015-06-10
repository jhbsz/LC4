# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=atheros_ssdk
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/atheros_ssdk
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=SSDK for atheros switch
endef

define Package/atheros_ssdk/description
	A small shell to configure switch register
endef

TARGET_CFLAGS+= -DDEBUG -I$(STAGING_DIR)/usr/include -I$(PKG_BUILD_DIR)


define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/atheros_ssdk/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/bin/ssdk_sh $(1)/bin/
endef

$(eval $(call BuildPackage,atheros_ssdk))
