# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-httpd-v2
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

#include $(TOPDIR)/package/rules.mk
include $(INCLUDE_DIR)/package.mk

define Package/ezp-httpd-v2
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=ezp-httpd-v2
  DEPENDS:=+ezp-nvram
endef

define Package/ezp-httpd-v2/description
 - The new stype which
 - 1. added bootstrap package
 - 2. added togobox and togomusic gui
 - 3. modified all original gui to bootstrap style
endef

ifeq ($(CONFIG_PACKAGE_ezp-ddns), y)
EZP_CFLAGS+=-DAXIMDDNS
endif

#XXX: Define the hardware architecture here.
ifeq ($(CONFIG_TARGET_ralink),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif

ifeq ($(CONFIG_TARGET_ralink_7620),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif

ifeq ($(CONFIG_TARGET_as500),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_AS500
endif

ifeq ($(CONFIG_TARGET_ox810),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_OX810
endif

ifeq ($(CONFIG_PACKAGE_lighttpd),y)
EZP_CFLAGS+= -DEZP_DRIVE
endif

ifeq ($(CONFIG_PACKAGE_mjpg-streamer),y)
EZP_CFLAGS+= -DEZP_CAMERA
endif

ifeq ($(CONFIG_PACKAGE_mini-snmpd),y)
EZP_CFLAGS+= -DEZP_SNMPD
endif

ifeq ($(CONFIG_PACKAGE_shairport),y)
EZP_CFLAGS+= -DEZP_MUSIC
endif


EZP_BRAND=$(CONFIG_EZP_SECRET)
EZP_COPY_FILES=$(CONFIG_EZP_SECRET)
ifeq ($(CONFIG_EZP_SECRET), "FEENEY")
EZP_BRAND:=ZYUS
endif
ifeq ($(CONFIG_EZP_SECRET), "APOLLO")
EZP_BRAND:=PROX
endif
ifeq ($(CONFIG_EZP_SECRET), "SONY")
EZP_BRAND:=AXIM
endif
ifeq ($(CONFIG_EZP_SECRET), "GARMIN")
EZP_BRAND:=AXIM
endif
ifeq ($(CONFIG_EZP_SECRET), "RFLINK")
EZP_BRAND:=AXIM
endif
ifeq ($(CONFIG_EZP_SECRET), "TOGOBOX")
EZP_BRAND:=AXIM
  # For M2, we are using Model or platform to distinct between M1 and M2, not
  # only by CONFIG_EZP_SECRET
  ifeq ($(CONFIG_EZP_MODEL),"QGenie")
    EZP_COPY_FILES=$(CONFIG_EZP_MODEL)
  endif
endif
ifeq ($(CONFIG_EZP_SECRET), "TOGOMUSIC")
EZP_BRAND:=AXIM
endif
ifeq ($(DBG_OPT), 1)
EZP_CFLAGS+= -DDEBUG
endif

EZP_CFLAGS+= -DWAN_NUM=$(CONFIG_WAN_NUM) \
			 -DLAN_NUM=$(CONFIG_LAN_NUM) \
			 -DWL_NUM=$(CONFIG_WL_NUM) \
			 -DEZP_PROD_BRAND_$(EZP_BRAND) \
			 -DEZP_SUB_BRAND_$(CONFIG_EZP_SECRET) \
			 -DPLATFORM=$(CONFIG_TARGET_PLATFORM) \
			 -DPLATFORM_$(CONFIG_TARGET_PLATFORM) \
			 -DEZP_COPYRIGHT=\\\"$(CONFIG_HTTPD_COPYRIGH_TEXT)\\\" \
			 -DEZP_LOGO_$(CONFIG_HTTPD_LOGO_SHOW) \
			 -DEZP_PROD_SUBCAT=$(CONFIG_EZP_PRODUCT_SUBCATEGORY) \

define Package/ezp-httpd-v2/config
	menu "Httpd Configuration"
		depends on PACKAGE_ezp-httpd-v2
		source "$(SOURCE)/config/Config.in"
	endmenu
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram $(EZP_HTTPD_ARCH_CFLAGS) \
		$(EZP_CFLAGS)"
endef



define Package/ezp-httpd-v2/install
	$(INSTALL_DIR) $(1)/www
	$(CP) ./files/www.$(EZP_COPY_FILES)/* $(1)/www
	$(CP) ./files/$(EZP_COPY_FILES)_code.js $(1)/www/code.js
	$(CP) ./files/style/$(EZP_COPY_FILES)/* $(1)/www/style/
ifndef CONFIG_TARGET_ralink_7620
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) ./files/httpd.init $(1)/etc/init.d/httpd
	-find $(1) -type d -name CVS | xargs rm -rf
	-find $(1) -type d -name .svn | xargs rm -rf
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/httpd $(1)/usr/sbin
endif
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/*.h $(1)/usr/include/
endef


$(eval $(call BuildPackage,ezp-httpd-v2))
