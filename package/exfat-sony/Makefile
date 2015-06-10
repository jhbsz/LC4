#
# Copyright (C) 2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=exfat-libfuse-sony
PKG_RELEASE:=1

PKG_VERSION:=0.9.5

PKG_SOURCE:=fuse-2.8.3.tar.gz
PKG_SOURCE_URL:=http://exfat.googlecode.com/files

include $(INCLUDE_DIR)/package.mk

define Package/exfat-libfuse-sony
	SECTION:=utils
	CATEGORY:=Utilities
	URL:=http://code.google.com/p/exfat/
	SUBMENU:=Filesystem
	TITLE:=exFAT drivers, libraries and utilities for Sony
endef

define Package/exfat-libfuse-sony/description
	Prebuilt exfat libraries for Sony.
endef

define Build/Prepare
	echo "Prepare"
endef

define Build/Compile
	echo "Compile"
endef

define Package/exfat-libfuse-sony/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) ./files/exfatfuse $(1)/usr/bin/
	$(INSTALL_BIN) ./files/fusermount $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) ./files/libfuse.so.2.8.3 $(1)/usr/lib
	ln -s libfuse.so.2.8.3 $(1)/usr/lib/libfuse.so.2
	ln -s libfuse.so.2.8.3 $(1)/usr/lib/libfuse.so
	$(CP) ./files/libl10n.so.7.0 $(1)/usr/lib
	ln -s libl10n.so.7.0 $(1)/usr/lib/libl10n.so.7
	ln -s libl10n.so.7.0 $(1)/usr/lib/libl10n.so
endef

$(eval $(call BuildPackage,exfat-libfuse-sony))
