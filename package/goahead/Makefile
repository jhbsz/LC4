# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=goahead
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/goahead
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=goahead
  DEPENDS:=+ezp-nvram
endef

define Package/goahead/description
	A network test program for Abocom factory test.
endef

EZP_CFLAGS+= -DWEBS -DUEMF -DOS="LINUX" -DLINUX -DDUSER_MANAGEMENT_SUPPORT
EZP_CFLAGS+= -DWAN_NUM=$(CONFIG_WAN_NUM) -DLAN_NUM=$(CONFIG_LAN_NUM) -DWL_NUM=$(CONFIG_WL_NUM) -DEZP_PROD_CAT_$(CONFIG_EZP_PRODUCT_CATEGORY) -DEZP_PROD_SUBCAT=$(CONFIG_EZP_PRODUCT_SUBCATEGORY) -DEZP_PROD_SUBSUBCAT_$(CONFIG_EZP_PRODUCT_SUBSUBCATEGORY) -DEZP_PUBLIC_TREE

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram $(EZP_CFLAGS) "

endef

define Package/goahead/install
	@echo " ===>SDBG: need to do "
	@echo " ===>SDBG: 1= $(1)"
	$(INSTALL_DIR) $(1)/web
	$(CP) ./files/web/* $(1)/web
	$(CP) $(PKG_BUILD_DIR)/cgi-src/upload.cgi $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/upload2.cgi $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/upload3.cgi $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/upload_bootloader.cgi $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/reboot.sh $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/ExportSettings.sh $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/upload_settings.cgi $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/usb_upgrade.cgi $(1)/web/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-src/history.sh $(1)/web/cgi-bin
	chmod 755 $(1)/web/cgi-bin/upload2.cgi
	chmod 755 $(1)/web/cgi-bin/upload3.cgi
	chmod 755 $(1)/web/cgi-bin/ExportSettings.sh
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) ./files/goahead.init $(1)/etc/init.d/goahead
	chmod 755 $(1)/etc/init.d/goahead
	-find $(1) -type d -name CVS | xargs rm -rf
	-find $(1) -type d -name .svn | xargs rm -rf
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/LINUX/goahead $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cgi-src/wifi_unload.sh $(1)/usr/sbin
	$(CP) ./files/get3G.sh $(1)/usr/sbin
	chmod 755 $(1)/usr/sbin/get3G.sh
	$(CP) ./files/getSSI.sh $(1)/usr/sbin
	chmod 755 $(1)/usr/sbin/getSSI.sh
	$(CP) ./files/cplange.sh $(1)/usr/sbin
	chmod 755 $(1)/usr/sbin/cplange.sh
	$(INSTALL_BIN) ./files/restart_web.sh $(1)/usr/sbin
endef
$(eval $(call BuildPackage,goahead))

