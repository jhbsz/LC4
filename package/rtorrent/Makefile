#
# Copyright (C) 2007-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 11950 2008-07-26 22:32:55Z nico $

include $(TOPDIR)/rules.mk

PKG_NAME:=rtorrent
PKG_VERSION:=0.8.5

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://libtorrent.rakshasa.no/downloads/

PKG_FIXUP = libtool

include $(INCLUDE_DIR)/package.mk

define Package/rtorrent
  SUBMENU:=BitTorrent
  SECTION:=net
  CATEGORY:=Network
  TITLE:=BitTorrent client for ncurses
  DEPENDS:=+libcurl +libtorrent +libncurses +libsigcxx +xmlrpc-c
  URL:=http://libtorrent.rakshasa.no/
endef

define Package/rtorrent/description
 rTorrent is a BitTorrent client for ncurses, using the libtorrent library.
 The client and library are written in C++ with emphasis on speed and
 efficiency, while delivering equivalent features to those found in GUI
 based clients in an ncurses client.
endef

CONFIGURE_ARGS+= \
	--enable-shared \
	--disable-static \
	--disable-debug \
	--with-xmlrpc-c \

CONFIGURE_VARS += \
	LIBS="-lm" \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Package/rtorrent/install
	$(INSTALL_DIR) $(1)/etc/{rtorrent,init.d}
	$(INSTALL_BIN) ./files/rtorrent.init $(1)/etc/init.d/rtorrent
	$(INSTALL_DATA) ./files/rtorrent.rc.template $(1)/etc/rtorrent/
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/rtorrent $(1)/usr/bin/
endef

$(eval $(call BuildPackage,rtorrent))
