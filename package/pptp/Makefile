# 
# Copyright (C) 2006-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 11018 2008-05-03 12:11:35Z nico $

include $(TOPDIR)/rules.mk

PKG_NAME:=pptp
PKG_VERSION:=1.6.0
PKG_RELEASE:=5

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/pptpclient
PKG_MD5SUM:=9a706327fb9827541d7c86d48ceb9631

include $(INCLUDE_DIR)/package.mk

define Package/pptp
  SECTION:=net
  CATEGORY:=Network
  TITLE:=PPTP client
  URL:=http://pptpclient.sourceforge.net/
  DEPENDS:=+ppp +kmod-gre
endef

define Package/pptp/daemon
 This package contains a PPTP (Point-to-Point Tunneling Protocol) client.
endef

define Package/pptp/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pptp $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/ppp
	$(INSTALL_DATA) ./files/options.pptp $(1)/etc/ppp/
	$(INSTALL_DATA) ./files/chap-secrets $(1)/etc/ppp/chap-secrets.template
	$(INSTALL_DIR) $(1)/etc/ppp/peers
	ln -sf /tmp/conn1 $(1)/etc/ppp/peers/conn1
	ln -sf /tmp/chap-secrets $(1)/etc/ppp/chap-secrets
	$(INSTALL_DIR) $(1)/lib/network
	$(INSTALL_BIN) ./files/pptp.sh $(1)/lib/network/
	$(INSTALL_DIR) $(1)/sbin
ifeq ($(CONFIG_TARGET_PLATFORM),"M2E")
	$(INSTALL_DATA) ./files/conn.template.M2 $(1)/etc/ppp/peers/conn.template.M2
	$(INSTALL_BIN) ./files/ifup.pptp.M2 $(1)/sbin/ifup.pptp.M2
	$(INSTALL_BIN) ./files/ifup.pptp.pap.M2 $(1)/sbin/ifup.pptp.pap.M2
else
	$(INSTALL_BIN) ./files/ifup.pptp $(1)/sbin/ifup.pptp
	$(INSTALL_DATA) ./files/conn.template $(1)/etc/ppp/peers/conn.template
endif
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/pptp $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,pptp))
