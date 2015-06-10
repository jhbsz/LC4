# $Id$

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME := ezp-mtd
PKG_RELEASE := 8

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk


#XXX: Define the hardware architecture here.
ifeq ($(CONFIG_TARGET_ralink),y)
EZP_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif
ifeq ($(CONFIG_TARGET_ralink_7620),y)
EZP_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif
ifeq ($(CONFIG_TARGET_ox810),y)
EZP_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_OX810
endif

EZP_CONFIG_ARCH_CFLAGS := -I $(TOPDIR)/tools/mkimage/src -I $(TOPDIR)/package/ezp-nvram/src -I $(LINUX_HEADERS_DIR)
EZP_CONFIG_ARCH_SHARED_LDFLAGS := -lnvram -lshared -L $(BUILD_DIR)/ezp-nvram 

define Package/ezp-mtd
  SECTION:=utils
  CATEGORY:=Base system
  DEPENDS:=ezp-nvram
  TITLE:=Update utility for EZ Packet firmware images
endef

define Package/ezp-mtd/description
 This package contains an utility useful to upgrade/downgrade EZ Packet releases.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./*.c $(PKG_BUILD_DIR)/
	$(CP) ./*.h $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(EZP_CONFIG_ARCH_CFLAGS)  -o $(PKG_BUILD_DIR)/mtd mtd.c $(EZP_CONFIG_ARCH_SHARED_LDFLAGS) $(EZP_ARCH_CFLAGS)
	$(TARGET_CC) $(TARGET_CFLAGS)  $(EZP_CONFIG_ARCH_CFLAGS) -o $(PKG_BUILD_DIR)/jffs2root jffs2root.c $(EZP_CONFIG_ARCH_SHARED_LDFLAGS) $(EZP_ARCH_CFLAGS)
endef

define Package/ezp-mtd/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mtd $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/jffs2root $(1)/sbin/
endef

$(eval $(call BuildPackage,ezp-mtd))
