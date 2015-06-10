#
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=transmission
PKG_VERSION:=2.11
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://mirrors.m0k.org/transmission/files
PKG_MD5SUM:=ef2b40ddc2c17a57c47bfe15b1f23cc4

PKG_FIXUP:=libtool

include $(INCLUDE_DIR)/package.mk

define Package/transmission/template
  SUBMENU:=BitTorrent
  SECTION:=net
  CATEGORY:=Network
  TITLE:=A free, lightweight BitTorrent client
  URL:=http://www.transmissionbt.com
endef

define Package/transmission-daemon
  $(call Package/transmission/template)
  DEPENDS:=+libcurl +libopenssl +libevent
  MENU:=1
endef

define Package/transmission-cli
  $(call Package/transmission/template)
#  DEPENDS:=transmission-daemon
endef

define Package/transmission-remote
  $(call Package/transmission/template)
endef

define Package/transmission-web
  $(call Package/transmission/template)
  DEPENDS:=transmission-daemon
endef

define Package/transmission-daemon/description
 Transmission is a simple BitTorrent client.
 It features a very simple, intuitive interface
 on top on an efficient, cross-platform back-end.
 This package contains the daemon itself.
endef

define Package/transmission-cli/description
 CLI utilities for transmission.
endef

define Package/transmission-remote/description
 CLI remote interface for transmission.
endef

define Package/transmission-web/description
 Webinterface resources for transmission.
endef

CONFIGURE_VARS += \
	LIBEVENT_LIBS="$(STAGING_DIR)/usr/lib/libevent.so"

CONFIGURE_ARGS += \
	--enable-daemon \
	--disable-gtk \
	--enable-largefile

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)"
endef

define Package/transmission-daemon/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/daemon/transmission-daemon $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/transmission.init $(1)/etc/init.d/bt
endef

define Package/transmission-cli/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/transmission-cli \
			$(PKG_INSTALL_DIR)/usr/bin/transmission-create \
			$(PKG_INSTALL_DIR)/usr/bin/transmission-edit \
			$(PKG_INSTALL_DIR)/usr/bin/transmission-show \
			$(1)/usr/bin/
endef

define Package/transmission-remote/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/daemon/transmission-remote $(1)/usr/bin/
endef

define Package/transmission-web/install
	$(INSTALL_DIR) $(1)/usr/share/transmission
	$(CP) $(PKG_INSTALL_DIR)/usr/share/transmission/web $(1)/usr/share/transmission/
endef

$(eval $(call BuildPackage,transmission-daemon))
$(eval $(call BuildPackage,transmission-cli))
$(eval $(call BuildPackage,transmission-remote))
$(eval $(call BuildPackage,transmission-web))
