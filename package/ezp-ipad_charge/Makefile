# $Id: Makefile 3112 2006-02-01 23:53:19Z mbm $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-ipad_charge
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-ipad_charge
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Ipad_charge - EZ Packet ipad_charge daemon.
endef

define Package/ezp-ipad_charge/description
	A ipad_charge for EZ Packet Products.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR)"

endef

define Package/ezp-ipad_charge/install
	$(INSTALL_DIR) $(1)/etc/hotplug.d/usb/
	$(INSTALL_BIN) ./files/05-ipad_charge $(1)/etc/hotplug.d/usb/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ipad_charge $(1)/usr/sbin

endef
$(eval $(call BuildPackage,ezp-ipad_charge))
