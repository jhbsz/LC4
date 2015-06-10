include $(TOPDIR)/rules.mk

PKG_NAME:=als-ddns
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/als-ddns
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=ALLIS DDNS
  DEPENDS:=ezp-nvram
endef

define Package/als-ddns/description
	Command Line Interface.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. "

endef

define Package/als-ddns/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/sbin
	cp -a $(PKG_BUILD_DIR)/als-ddns $(1)/usr/sbin
	cp -a ./files/* $(1)/etc/init.d/

endef
$(eval $(call BuildPackage,als-ddns))
