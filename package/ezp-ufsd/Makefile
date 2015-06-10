include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-ufsd
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-ufsd
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Package for paragon ntfs/hfsplus driver 
  DEPENDS:=ezp-nvram
endef

define Package/ezp-ufsd/description
	Command Line Interface.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile

endef

define Package/ezp-ufsd/install
	$(INSTALL_DIR) $(1)/lib/modules/2.6.36
	cp ./files/*.ko $(1)/lib/modules/2.6.36
endef
$(eval $(call BuildPackage,ezp-ufsd))
