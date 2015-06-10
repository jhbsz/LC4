include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-usb-testrw
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_RELEASE)

include $(INCLUDE_DIR)/package.mk


define Package/ezp-usb-testrw
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=test - EZ Packet test usb read write sleep
endef

define Package/ezp-usb-testrw/description
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

define Package/ezp-usb-testrw/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/usb_read_write_sleep $(1)/usr/bin

endef
$(eval $(call BuildPackage,ezp-usb-testrw))
