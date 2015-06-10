include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-serial
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-serial
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=TEST
  DEPENDS:=ezp-nvram
endef

define Package/ezp-serial/description
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

define Package/ezp-serial/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/sbin
	mkdir -p $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib
	cp -a $(PKG_BUILD_DIR)/UART $(1)/usr/sbin
	cp -a $(PKG_BUILD_DIR)/con_test $(1)/usr/sbin
	cp -a ./files/etc/init.d/* $(1)/etc/init.d/
	cp -a ./files/etc/serial.conf.default $(1)/etc/
	cp -a ./files/set_uart $(1)/usr/sbin/

endef
$(eval $(call BuildPackage,ezp-serial))
