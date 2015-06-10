#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dnsmasq
PKG_VERSION:=2.57
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://thekelleys.org.uk/dnsmasq
PKG_MD5SUM:=d10faeb409717eae94718d7716ca63a4

include $(INCLUDE_DIR)/package.mk

define Package/dnsmasq
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=A lightweight DNS and DHCP server
  URL:=http://www.thekelleys.org.uk/dnsmasq/
endef

define Package/dnsmasq/description
 It is intended to provide coupled DNS and DHCP service to a LAN.
endef

define Package/dnsmasq/conffiles
endef

ifeq ($(CONFIG_EZP_SECRET), "GARMIN")
INIT_SCRIPT:=dnsmasq.init.garmin
else
INIT_SCRIPT:=dnsmasq.init
endif

ifeq ($(CONFIG_TARGET_ralink_7620),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X -DPLATFORM_M2E
endif
ifeq ($(CONFIG_TARGET_ox810),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_OX810 -DTYPEDEF_BOOL
endif

TARGET_CFLAGS += -ffunction-sections -fdata-sections

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		BINDIR="/usr/sbin" MANDIR="/usr/man" \
		AWK="awk" \
		LDFLAGS="-Wl,--gc-sections" \
		all
endef

define Package/dnsmasq/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/dnsmasq $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/dhcp.conf $(1)/etc/config/dhcp
	$(INSTALL_BIN) ./files/dhcp_users.sh $(1)/usr/sbin/dhcp_users.sh
#	$(INSTALL_DATA) ./files/dnsmasq.conf $(1)/etc/dnsmasq.conf
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/$(INIT_SCRIPT) $(1)/etc/init.d/dnsmasq
endef

$(eval $(call BuildPackage,dnsmasq))
