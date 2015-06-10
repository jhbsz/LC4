# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-upgraded
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-upgraded
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Stats - EZ Packet upgraded daemon.
endef

define Package/ezp-upgraded/description
	A statistics server for EZ Packet Products.
endef

EZP_CFLAGS += -DPLATFORM=$(CONFIG_TARGET_PLATFORM) -DPLATFORM_$(CONFIG_TARGET_PLATFORM)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) $(EZP_HTTPD_ARCH_CFLAGS) $(EZP_CFLAGS)"
endef

define Package/ezp-upgraded/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/upgraded $(1)/usr/sbin
endef

$(eval $(call BuildPackage,ezp-upgraded))
