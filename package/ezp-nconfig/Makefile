# $Id: Makefile 2361 2005-11-07 00:27:07Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ezp-nconfig
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezp-nconfig
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=ezp-nconfig - product authentication tool
  DEPENDS:=+libncurses +dialog
endef

define Package/ezp-nconfig/description
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

define Package/ezp-nconfig/install
	$(INSTALL_DIR) $(1)/{sbin,etc}
	$(CP) ./files/nconfig $(1)/sbin/
	$(CP) ./files/nconfig.awk $(1)/sbin/
	$(CP) ./files/set_fac_nvram $(1)/sbin/
	$(CP) ./files/set_mode $(1)/sbin/
	$(CP) ./files/upgrade $(1)/sbin/
	$(CP) ./files/inittab $(1)/etc/

endef
$(eval $(call BuildPackage,ezp-nconfig))
