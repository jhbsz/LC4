include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-vpnstatus
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-vpnstatus
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:= VPN status updater
  DEPENDS:=ezp-nvram
endef

define Package/ezp-vpnstatus/description
	Command Line Interface.
endef

ifeq ($(CONFIG_TARGET_ralink),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif
ifeq ($(CONFIG_TARGET_ralink_7620),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif
ifeq ($(CONFIG_TARGET_ox810),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_OX810 -DTYPEDEF_BOOL
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram $(EZP_HTTPD_ARCH_CFLAGS) -DEZP_PROD_CAT_$(CONFIG_EZP_PRODUCT_CATEGORY) -DEZP_PRODUCT_VERSION=\\\"$(CONFIG_EZP_PRODUCT_VERSION)\\\" "

endef

define Package/ezp-vpnstatus/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/vpnstatus $(1)/usr/bin
endef
$(eval $(call BuildPackage,ezp-vpnstatus))
