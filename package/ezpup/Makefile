# $Id: Makefile 3341 2006-03-10 07:34:27Z nico $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/target.mk


PKG_NAME:=ezpup
PKG_RELEASE:=0.1.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ezpup
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=EZPacket firmware upgrade tools.
endef

define Package/ezpup/description
      A set of tools that encrypt, decrypt and upgrade EZPacket firmware.
endef

ifeq ($(shell awk -v subtarget=$(SUBTARGET) 'BEGIN{print index(subtarget,"f16")}'),0)
    EZP_SECRET_FLASH_SIZE:=$(CONFIG_EZP_SECRET)
else
    EZP_SECRET_FLASH_SIZE:=$(CONFIG_EZP_SECRET)_16M
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) ezpkey.h "KEY=$(EZP_SECRET_FLASH_SIZE)" \
		CFLAGS="-I$(PKG_BUILD_DIR)/libtomcrypt-1.00/src/headers -DLTC_NO_ASM"
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -I$(PKG_BUILD_DIR)/libtomcrypt-1.00/src/headers" ezpup
endef
define Package/ezpup/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ezpup $(1)/usr/sbin
endef

$(eval $(call BuildPackage,ezpup))
