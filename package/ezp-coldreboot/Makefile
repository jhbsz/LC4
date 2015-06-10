# $Id: Makefile 2361 2005-11-07 00:27:07Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ezp-coldreboot
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezp-coldreboot
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=ezp-coldreboot - put at 99zwakeup to trigger a cold reboot action
endef

define Package/ezp-coldreboot/description
	product authentication tool
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile/Default
endef

define Build/Compile
	$(call Build/Compile/Default)
endef

define Package/ezp-coldreboot/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/zwakeup.init $(1)/etc/init.d/zwakeup
endef
$(eval $(call BuildPackage,ezp-coldreboot))
