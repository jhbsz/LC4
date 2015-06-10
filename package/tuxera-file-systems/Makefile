#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=tuxera-file-systems
PKG_VERSION:=0
PKG_SOURCE:=tuxera-file-systems-3013.2.5-rt5350.tgz

include $(INCLUDE_DIR)/package.mk

define Package/tuxera-file-systems
	SECTION:=utils
	CATEGORY:=Utilities
	SUBMENU:=Filesystem
	TITLE:=exFAT/NTFS/HFS+ drivers, libraries and utilities from tuxera
endef

define Package/tuxera-file-systems/description
	Prebuilt exfat/NTFS/HFS+ libraries from tuxera.
endef

define Build/Prepare
	echo "Prepare"
endef

define Build/Compile
	echo "Compile"
endef

define Package/tuxera-file-systems/install
	$(INSTALL_DIR) $(1)/lib/modules/2.6.26/
	$(CP) ./files/texfat.ko $(1)/lib/modules/2.6.26/
	$(CP) ./files/tntfs.ko $(1)/lib/modules/2.6.26/
	$(CP) ./files/thfsplus.ko $(1)/lib/modules/2.6.26/
endef

$(eval $(call BuildPackage,tuxera-file-systems))
