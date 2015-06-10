# $Id: Makefile 2361 2005-11-07 00:27:07Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ezp-autoip
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezp-autoip
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Auto Lan IP Switch
endef

define Package/ezp-autoip/description
	Auto switch lan network and ip address while conflict with wan ip
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile/Default
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/ezp-autoip/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/autoip.sh $(1)/usr/bin/
	$(INSTALL_BIN) ./files/autoip.init $(1)/etc/init.d/autoip
endef
$(eval $(call BuildPackage,ezp-autoip))
