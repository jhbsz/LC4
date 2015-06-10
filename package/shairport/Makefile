# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=shairport
PKG_VERSION:=0.05
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/shairport
  SECTION:=sound
  CATEGORY:=Sound
  TITLE:=AirPlay speakers
  URL:=http://mafipulation.org/blagoblig  
  DEPENDS:=+salsa-lib +ezp-nvram
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		USE_STATIC_LIBSSL=y \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I$(BUILD_DIR)/ezp-nvram -I $(STAGING_DIR)/usr/include -L$(STAGING_DIR)/usr/lib"
endef

define Package/shairport/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/{hairtunes,shairport} $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/hotplug.d/sound/
	$(CP) files/30-airplay $(1)/etc/hotplug.d/sound/
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(CP) files/shairport.init $(1)/etc/init.d/shairport
endef

$(eval $(call BuildPackage,shairport))
