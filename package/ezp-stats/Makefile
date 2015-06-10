# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-stats
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-stats
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Stats - EZ Packet stats daemon.
  DEPENDS:=+ezp-nvram
endef

define Package/ezp-stats/description
	A statistics server for EZ Packet Products.
endef

ifeq ($(CONFIG_TARGET_ralink),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif

ifeq ($(CONFIG_TARGET_ralink_7620),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

EZP_BRAND=$(CONFIG_EZP_SECRET)
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
endif
ifeq ($(CONFIG_EZP_SECRET), "TOGOMUSIC")
EZP_BRAND:=AXIM
endif

EZP_CFLAGS+= -DEZP_SUB_BRAND_$(CONFIG_EZP_SECRET) \
 -DPLATFORM_$(CONFIG_TARGET_PLATFORM) \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram -I $(BUILD_DIR)/ezp-httpd $(EZP_HTTPD_ARCH_CFLAGS) $(EZP_CFLAGS)"

endef

define Package/ezp-stats/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(CP) ./files/stats.init $(1)/etc/init.d/stats
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/stats $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-stats))
