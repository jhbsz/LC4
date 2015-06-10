#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 10347 2008-02-02 01:21:31Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=ezpcom
PKG_VERSION:=0.32
PKG_RELEASE:=1

PKG_SOURCE:=comgt.$(PKG_VERSION).tgz
PKG_SOURCE_URL:=@SF/comgt
PKG_MD5SUM:=db2452680c3d953631299e331daf49ef

PKG_BUILD_DIR:=$(BUILD_DIR)/comgt.$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/ezpcom
  SECTION:=net
  CATEGORY:=Network
  TITLE:=EZ Packet HSPA/3G/GPRS control tool
  DEPENDS:=+chat +ezp-nvram
endef

define Package/ezpcom/description
 ezpcom is a scripting language interpreter useful for establishing 
 communications on serial lines and through PCMCIA modems as well as GPRS 
 and 3G datacards.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -I $(BUILD_DIR)/ezp-nvram" \
		gcom
endef
	
define Package/ezpcom/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gcom $(1)/usr/sbin/ezpcom
#	$(INSTALL_DIR) $(1)/etc/ppp
#	$(INSTALL_BIN) ./files/3g.connect $(1)/etc/ppp/3g.connect
	$(INSTALL_DIR) $(1)/etc/chatscripts
	$(INSTALL_DATA) ./files/3g.chat $(1)/etc/chatscripts/3g.chat
	$(INSTALL_DATA) ./files/evdo.chat $(1)/etc/chatscripts/evdo.chat
	$(INSTALL_DATA) ./files/wwan.chat $(1)/etc/chatscripts/wwan.chat
	$(INSTALL_DATA) ./files/command.gcom $(1)/etc/chatscripts/command.gcom
	$(INSTALL_DATA) ./files/script.gcom $(1)/etc/chatscripts/script.gcom
	$(INSTALL_DATA) ./files/script.GobiReset $(1)/etc/chatscripts/script.GobiReset
	$(INSTALL_DATA) ./files/script.GobiConnect $(1)/etc/chatscripts/script.GobiConnect
	$(INSTALL_DATA) ./files/script.GobiSig $(1)/etc/chatscripts/script.GobiSig
#	$(INSTALL_DIR) $(1)/lib/network
#	$(INSTALL_DATA) ./files/3g.sh $(1)/lib/network/3g.sh
#	$(INSTALL_DIR) $(1)/etc/hotplug.d/button
#	$(INSTALL_DATA) ./files/3g.button $(1)/etc/hotplug.d/button/05-3g
#	$(INSTALL_DIR) $(1)/etc/hotplug.d/iface
#	$(INSTALL_DATA) ./files/3g.iface $(1)/etc/hotplug.d/iface/05-3g
endef

$(eval $(call BuildPackage,ezpcom))
