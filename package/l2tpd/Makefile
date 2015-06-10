#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=l2tpd
PKG_VERSION:=0.70pre
PKG_UPSTREAM_VERSION:=0.70-pre20031121
PKG_RELEASE:=4.1

PKG_SOURCE:=$(PKG_NAME)_$(PKG_UPSTREAM_VERSION).orig.tar.gz
PKG_SOURCE_URL:=http://ftp.debian.org/debian/pool/main/l/l2tpd
PKG_MD5SUM:=3f2707b6e16a8cb72e7bf64f574202fa

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_UPSTREAM_VERSION).orig

include $(INCLUDE_DIR)/package.mk

define Package/l2tpd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=An L2TP (Layer 2 Tunneling Protocol) daemon
  URL:=http://l2tpd.sourceforge.net/
  SUBMENU:=VPN
endef

define Package/l2tpd/description
	l2tpd is the open source implementation of the L2TP tunneling 
	protocol (RFC2661). It does implement both LAC and LNS role 
	in a L2TP networking architecture. The main goal of this protocol is 
	to tunnel PPP frame trough an ip network.
endef

# EZP: this should depend on menuconfig
# TODO: Remove  WAN_NUM, LAN_NUM, WL_NUM
ifeq ($(CONFIG_TARGET_ralink),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_RT305X
endif

ifeq ($(CONFIG_TARGET_ralink_7620),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_RT305X
endif

ifeq ($(CONFIG_TARGET_as500),y)
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_AS500
endif

ifeq ($(BR2_LINUX_2_4_FV13XX),y)
#BR2_PACKAGE_NVRAM:=y
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_FV13XX
endif

ifeq ($(BR2_LINUX_2_4_BRCM),y)
#BR2_PACKAGE_NVRAM:=y
EZP_CFLAGS:=-DNVRAM_MMAP=1 -DCONFIG_EZP_ARCH_BRCM
endif

ifeq ($(CONFIG_EZP_PRODUCT_SUPERCATEGORY),"")
# EZP: Pass WAN_NUM, LAN_NUM, WL_NUM while compiling
EZP_CFLAGS+= -DWAN_NUM=$(CONFIG_WAN_NUM) -DLAN_NUM=$(CONFIG_LAN_NUM) -DWL_NUM=$(CONFIG_WL_NUM) \
  -DEZP_PROD_CAT=$(CONFIG_EZP_PRODUCT_CATEGORY) \
  -DEZP_PROD_CAT_$(CONFIG_EZP_PRODUCT_CATEGORY) \
  -DEZP_PROD_SUBCAT=$(CONFIG_EZP_PRODUCT_SUBCATEGORY) \
  -DEZP_PROD_SUBSUBCAT_$(CONFIG_EZP_PRODUCT_SUBSUBCATEGORY) \
  -DEZP_PROD_SUBSUBCAT=$(CONFIG_EZP_PRODUCT_SUBSUBCATEGORY) \
  -DEZP_PROD_BRAND_$(CONFIG_EZP_SECRET) \
  -DEZP_PROD_VERSION=$(CONFIG_EZP_PRODUCT_VERSION)
else
EZP_CFLAGS+= -DWAN_NUM=$(CONFIG_WAN_NUM) -DLAN_NUM=$(CONFIG_LAN_NUM) -DWL_NUM=$(CONFIG_WL_NUM) \
  -DEZP_PROD_SUPERCAT=$(CONFIG_EZP_PRODUCT_SUPERCATEGORY) \
  -DEZP_PROD_SUPERCAT_$(CONFIG_EZP_PRODUCT_SUPERCATEGORY) \
  -DEZP_PROD_CAT=$(CONFIG_EZP_PRODUCT_MAJOR_CATEGORY) \
  -DEZP_PROD_CAT_$(CONFIG_EZP_PRODUCT_MAJOR_CATEGORY) \
  -DEZP_PROD_CAT_$(CONFIG_EZP_PRODUCT_ALT_CATEGORY) \
  -DEZP_PROD_SUBCAT=$(CONFIG_EZP_PRODUCT_MAJOR_SUBCATEGORY) \
  -DEZP_PROD_SUBSUBCAT_$(CONFIG_EZP_PRODUCT_SUBSUBCATEGORY) \
  -DEZP_PROD_SUBSUBCAT=$(CONFIG_EZP_PRODUCT_SUBSUBCATEGORY) \
  -DEZP_PROD_MAJOR_CAT_$(CONFIG_EZP_PRODUCT_MAJOR_CATEGORY) \
  -DEZP_PROD_MAJOR_SUBCAT=$(CONFIG_EZP_PRODUCT_MAJOR_SUBCATEGORY) \
  -DEZP_PROD_ALT_CAT_$(CONFIG_EZP_PRODUCT_ALT_CATEGORY) \
  -DEZP_PROD_ALT_SUBCAT=$(CONFIG_EZP_PRODUCT_ALT_SUBCATEGORY) \
  -DEZP_PROD_BRAND_$(CONFIG_EZP_SECRET) \
  -DEZP_PROD_VERSION=$(CONFIG_EZP_PRODUCT_VERSION)
endif

EZP_CFLAGS+= -I $(TOPDIR)/package/ezp-nvram/src -I $(TOPDIR)/package/ezp-nvram/include -lnvram -lshared -L $(BUILD_DIR)/ezp-nvram

define Build/Compile
	$(call Build/Compile/Default, \
		CFLAGS="$(TARGET_CFLAGS) $(EZP_CFLAGS) -I. -DLINUX -DSANITY -DIP_ALLOCATION" \
		DFLAGS="$(TARGET_CFLAGS) $(EZP_CFLAGS)" \
	)
endef

define Package/l2tpd/install	
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/l2tpd
#	$(INSTALL_DATA) ./files/l2tpd.conf $(1)/etc/l2tpd/
	ln -sf /tmp/l2tpd.conf $(1)/etc/l2tpd/l2tpd.conf
	$(INSTALL_DIR) $(1)/etc/ppp
#	$(INSTALL_DATA) ./files/options.l2tpd $(1)/etc/ppp/
	ln -sf /tmp/options.l2tpd $(1)/etc/ppp/options.l2tpd
	ln -sf /tmp/chap-secrets $(1)/etc/ppp/chap-secrets
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/l2tpd.init $(1)/etc/init.d/l2tpd
#	$(INSTALL_DIR) $(1)/sbin
#	$(INSTALL_BIN) ./files/ifup.l2tp $(1)/sbin/
	$(INSTALL_DATA) ./files/l2tpd.conf.client $(1)/etc/l2tpd/
	$(INSTALL_DATA) ./files/options.l2tpd.client $(1)/etc/ppp/

endef

$(eval $(call BuildPackage,l2tpd))

