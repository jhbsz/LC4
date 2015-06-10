# $Id: Makefile 2361 2005-11-07 00:27:07Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ezp-upgrade-machine
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezp-upgrade-machine
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=ezp-upgrade-machine - build a m2 based firmware upgrade machine
endef

define Package/ezp-upgrade-machine/description
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

define Package/ezp-upgrade-machine/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/upgrade-machine $(1)/etc/init.d/upgrade-machine
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/dhcp_auto_upgrade.sh $(1)/usr/sbin/dhcp_auto_upgrade.sh
	$(INSTALL_BIN) ./files/auto_upgrade_env_setop.sh $(1)/usr/sbin/auto_upgrade_env_setop.sh
endef
$(eval $(call BuildPackage,ezp-upgrade-machine))
