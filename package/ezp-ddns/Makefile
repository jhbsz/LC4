include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-ddns
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-ddns
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=DDNS 
  DEPENDS:=+ezp-nvram
endef

define Package/ezp-ddns/description
	Command Line Interface for AXIM DDNS register.
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
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram $(EZP_HTTPD_ARCH_CFLAGS) $(EZP_CFLAGS)"

endef

define Package/ezp-ddns/install
    $(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezp-ddns $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/aximddns $(1)/etc/init.d/aximddns
endef

$(eval $(call BuildPackage,ezp-ddns))
