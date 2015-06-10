# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-license
PKG_VERSION:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezp-license
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=EZP License
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile/Default
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/ezp-license/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(CP) ./files/* $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,ezp-license))
