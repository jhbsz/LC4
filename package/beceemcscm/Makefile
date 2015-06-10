#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=CSCM
PKG_VERSION:=v1.1.6.0_source
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/CSCM
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libopenssl +beceemwimax
  TITLE:=Beceem CSCM crypto application
  URL:=http://pptpclient.sourceforge.net/
endef

define Package/CSCM/description
	Beceem WiMAX USB dangle  driver
endef

EZP_CFLAGS+= -I$(TOPDIR)/package/ezp-nvram/src -I$(TOPDIR)/package/ezp-nvram/include
PLATFORM:=pc_linux
CM_DIR:=$(PKG_BUILD_DIR)
TARGET_BIN_DIR:=$(CM_DIR)/bin_$(PLATFORM)/bin
TARGET_OBJ_DIR:=$(CM_DIR)/bin_$(PLATFORM)/obj
DEBUG:=1
CROSS_COMPILE:=$(TARGET_CROSS) 
CROSS_TOOLCHAIN_DIR:=$(TOOLCHAIN_DIR)
CFLAGS+=" $(TARGET_CFLAGS) $(EZP_CFLAGS) -fPIC "
LDFLAGS+=" -L$(PKG_BUILD_DIR)/wpa_supplicant/ "
define Build/Compile
	$(call Build/Compile/Default, \
		CROSS_COMPILE=$(CROSS_COMPILE) CROSS_TOOLCHAIN_DIR=$(CROSS_TOOLCHAIN_DIR) \
		PLATFORM=$(PLATFORM) CM_DIR=$(CM_DIR) TARGET_BIN_DIR=$(TARGET_BIN_DIR) TARGET_OBJ_DIR=$(TARGET_OBJ_DIR) DEBUG=$(DEBUG) \
	)
endef

define Package/CSCM/install	
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wpa_supplicant/libeap_supplicant.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/BeceemEAPSupplicant/BeceemEngine/libengine_beceem.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bin_pc_linux/bin/* $(1)/usr/bin/
endef

$(eval $(call BuildPackage,CSCM))
