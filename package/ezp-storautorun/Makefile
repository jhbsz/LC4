# $Id: Makefile 2361 2005-11-07 00:27:07Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ezp-storautorun
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezp-storautorun
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Auto Run for usb storage
endef

define Package/ezp-storautorun/description
	A drift framework for usb storage application storautorun
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile/Default
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/ezp-storautorun/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/storautorun.init $(1)/etc/init.d/storage_autorun
endef
$(eval $(call BuildPackage,ezp-storautorun))

