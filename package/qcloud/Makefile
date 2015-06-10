include $(TOPDIR)/rules.mk

PKG_NAME:=qcloud
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/qcloud
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=qcloud - myQNAPcloud library, daemon & sample code
  DEPENDS:=+ezp-nvram +libjson +libjson-c +curl +sqlite3
endef

define Package/qcloud/description
	myQNAPcloud library, daemon & sample code
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I$(STAGING_DIR)/usr/include -I$(BUILD_DIR)/ezp-nvram -fhonour-copts" \
		LDFLAGS="-L$(STAGING_DIR)/usr/lib -L../ezp-nvram -lnvram -lshared -lpthread"

endef

define Package/qcloud/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) ./files/porter.init $(1)/etc/init.d/porter
	$(CP) $(PKG_BUILD_DIR)/libs/qid/*.so $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libs/utils/*.so $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/sample/qcloud_sample $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/porter/porter $(1)/usr/sbin/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) ./files/porter.init $(1)/etc/init.d/porter
	$(CP) $(PKG_BUILD_DIR)/libs/qid/*.so $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/libs/utils/*.so $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/sample/qcloud_sample $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/porter/porter $(1)/usr/sbin/
	$(CP) ./src/libs/qid/include/*.h $(1)/usr/include/
	$(CP) ./src/libs/utils/include/*.h $(1)/usr/include/
endef

$(eval $(call BuildPackage,qcloud))
