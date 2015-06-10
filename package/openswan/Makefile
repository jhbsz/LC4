#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=openswan
PKG_VERSION:=2.6.28
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.openswan.org/download
PKG_MD5SUM:=37f2ad2664dd89e6ad96ae82f6378166

include $(INCLUDE_DIR)/package.mk

define Package/openswan/Default
  TITLE:=Openswan
  URL:=http://www.openswan.org/
  DEPENDS:=@LINUX_2_6
endef

define Package/openswan/Default/description
 Openswan is an IPsec implementation for Linux.
endef

define Package/openswan
$(call Package/openswan/Default)
  SUBMENU:=VPN
  SECTION:=net
  CATEGORY:=Network
  TITLE+= (daemon)
  DEPENDS+= +kmod-openswan +libgmp +ip
endef

define Package/openswan/description
$(call Package/openswan/Default/description)
 This package contains the Openswan user-land daemon.
endef

define KernelPackage/openswan
$(call Package/openswan/Default)
  SUBMENU:=Network Support
  TITLE+= (kernel module)
  FILES:=$(PKG_BUILD_DIR)/modobj*/ipsec.$(LINUX_KMOD_SUFFIX)
endef

define KernelPackage/openswan/description
$(call Package/openswan/Default/description)
 This package contains the Openswan kernel module.
endef

TARGET_CPPFLAGS += -I$(LINUX_DIR)/include

OPENSWAN_MAKE := $(MAKE) -C $(PKG_BUILD_DIR) \
	$(TARGET_CONFIGURE_OPTS) \
	LINUX_RELEASE="$(LINUX_RELEASE)" \
	KERNELSRC="$(LINUX_DIR)" \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	USERCOMPILE="$(TARGET_CFLAGS) -I$(PKG_BUILD_DIR)/ports/linux/include -I$(PKG_BUILD_DIR)/linux/include -I$(PKG_BUILD_DIR)/include $(TARGET_CPPFLAGS) $(TARGET_LDFLAGS)" \
	IPSECDIR="/usr/lib/ipsec" \
	INC_USRLOCAL="/usr" \
	INC_RCDEFAULT="/etc/init.d" \
	MODPROBE="/sbin/insmod" \
	LDFLAGS="$(TARGET_LDFLAGS)" \
	DESTDIR="$(PKG_INSTALL_DIR)" \
	WAN_NUM="${CONFIG_WAN_NUM}"

define Build/Compile
	$(OPENSWAN_MAKE) \
		programs module install
endef

define Package/openswan/install
	$(CP) -l $(PKG_INSTALL_DIR)/* $(1)
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/hotplug.d/iface
	$(INSTALL_DIR) $(1)/etc/ipsec
#	Remove unnecessary files and backups
	find $(1) -name \*.old | xargs rm -rf
	rm -rf $(1)/usr/share
	rm -rf $(1)/usr/man
	rm -rf $(1)/var
	rm -rf $(1)/etc/rc0.d
	rm -rf $(1)/etc/rc1.d
	rm -rf $(1)/etc/rc2.d
	rm -rf $(1)/etc/rc3.d
	rm -rf $(1)/etc/rc4.d
	rm -rf $(1)/etc/rc5.d
	rm -rf $(1)/etc/rc6.d
	rm -rf $(1)/etc/rc7.d
	rm -rf $(1)/etc/rc8.d
	rm -rf $(1)/etc/rc9.d
# Move original script to other folder
#	mv $(1)/etc/init.d/ipsec $(1)/etc/ipsec/ipsec
#	mv $(1)/etc/init.d/setup $(1)/etc/ipsec/setup
	rm $(1)/usr/libexec/ipsec/setup
# Copy AXIMCom startup script
	$(INSTALL_BIN) ./files/no_oe.conf $(1)/etc/ipsec.d/examples/no_oe.conf
	$(INSTALL_BIN) ./files/ipsec.init $(1)/etc/init.d/ipsec
	$(INSTALL_BIN) ./files/setup $(1)/usr/libexec/ipsec/setup
# Openswan will generate a default ipsec.conf and put under /etc
# We will override it by a symbolic link, so we remove it first
# The overriding procedure is in /etc/init.d/ipsec script
	ln -sf /tmp/ipsec.conf $(1)/etc/ipsec.conf
	ln -sf /tmp/ipsec.secrets $(1)/etc/ipsec.secrets
endef

$(eval $(call BuildPackage,openswan))
$(eval $(call KernelPackage,openswan))
