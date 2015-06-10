# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=ezp-i2c
PKG_VERSION:=1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

TARGET_FILE:=ezp-i2c
ifeq ($(I2C_TEST_MODE), 1)
TARGET_FILE:=ezp-i2c-1
endif

define Package/ezp-i2c
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=I2C command
  DEPENDS:=+ezp-nvram
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

EZP_CFLAGS+= -DEZP_I2C_KEY_$(CONFIG_EZP_I2C_KEY) \
			 -DPLATFORM=$(CONFIG_TARGET_PLATFORM) \
			 -DPLATFORM_$(CONFIG_TARGET_PLATFORM) \
		     -I$(LINUX_DIR)/include -I $(STAGING_DIR)/usr/include -I $(BUILD_DIR)/ezp-nvram -L../ezp-nvram -lnvram -lshared -lpthread \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) $(EZP_CFLAGS)" 
endef

define Package/ezp-i2c/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezp-i2c $(1)/usr/sbin/$(TARGET_FILE)
	#$(INSTALL_DIR) $(1)/etc/init.d
	#$(INSTALL_BIN) ./files/ezp-i2c-init $(1)/etc/init.d/ezp-i2c-init
endef

$(eval $(call BuildPackage,ezp-i2c))
