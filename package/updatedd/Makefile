# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=updatedd
PKG_VERSION:=2.6
PKG_RELEASE:=6
PKG_MD5SUM:=2957496de0c8e08e9c6492fabf1a88be

PKG_SOURCE_URL:=http://downloads.openwrt.org/sources/
PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).tar.bz2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

ifeq ($(CONFIG_EZP_SECRET), "FEENEY")
EZP_BRAND:=ZYUS
else
EZP_BRAND:=$(CONFIG_EZP_SECRET)
endif

EZP_CFLAGS+=-DWAN_NUM=$(CONFIG_WAN_NUM) \
	-DLAN_NUM=$(CONFIG_LAN_NUM) \
	-DWL_NUM=$(CONFIG_WL_NUM) \
	-DEZP_PROD_BRAND_$(EZP_BRAND) \
	-DEZP_PROD_SUBCAT=$(CONFIG_EZP_PRODUCT_SUBCATEGORY) \

ifeq ($(CONFIG_EZP_SECRET), "PROX")
TARGET_CONFIGURE_OPTS+=EZP_BRAND=$(EZP_BRAND) \
		CUSTOM_VERSION=$(CONFIG_CUSTOM_PRODUCT_VERSION)
else
TARGET_CONFIGURE_OPTS+=EZP_BRAND=$(EZP_BRAND)
endif

define Package/updatedd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=A tool to update dynamic DNS serv  ices.
endef
define Package/updatedd/description
	Updatedd is a small tool that will update one of many dynamic dns services on boot.
endef
define Build/Configure
	(cd $(PKG_BUILD_DIR); \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		./configure \
			--target=$(GNU_TARGET_NAME) \
			--host=$(GNU_TARGET_NAME) \
			--build=$(GNU_HOST_NAME) \
			--prefix=/usr \
			--sysconfdir=/etc \
	);
endef
define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(TARGET_CONFIGURE_OPTS) \
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(EZP_CFLAGS)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef
define Package/updatedd/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib/updatedd
	$(INSTALL_BIN) ./files/ddns $(1)/etc/init.d/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/${CONFIG_ARCH}-linux-updatedd $(1)/usr/bin/updatedd
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/updatedd/*.so $(1)/usr/lib/updatedd/
	$(INSTALL_DIR) $(1)/usr/lib/updatedd
endef

$(eval $(call BuildPackage,updatedd))
