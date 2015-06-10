# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=Uboot-spi
PKG_VERSION:=1.1.3

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/poptop

ifeq ($(CONFIG_Uboot-spi-3-6-0-0),y)
PKG_VERSION:=3.6.0.0
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/poptop
endif
ifeq ($(CONFIG_Uboot-spi-3-5-2-0),y)
PKG_VERSION:=3.5.2.0
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/poptop
endif

ifeq ($(CONFIG_Uboot-spi-4-1-2-0),y)
PKG_VERSION:=4.1.2.0
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/poptop
endif


include $(INCLUDE_DIR)/package.mk

define Package/Uboot-spi
  SECTION:=boot
  CATEGORY:=Boot Loaders
  TITLE:=Uboot-spi 1.1.3 for EZPacket with SPI flash
  URL:=http://www.denx.de/wiki/UBoot/WebHome
endef

define Package/Uboot-spi/config
  choice
    prompt "Choose Uboot Version for different product"
    depends on PACKAGE_Uboot-spi
    config Uboot-spi-4-1-2-0
       bool "Generic Uboot SPI v4.1.2.0, MT7620"
    config Uboot-spi-3-6-0-0
       bool "Generic Uboot SPI v 3.6.0.0, RT5350"
    config Uboot-spi-3-5-2-0
       bool "Generic Uboot SPI v 3.5.2.0, RT5350"
    config Uboot-spi-1-1-3
       bool "Generic Uboot SPI v 1.1.3, RT305x"
  endchoice
  choice
    prompt "Choose product"
    depends on PACKAGE_Uboot-spi && Uboot-spi-1-1-3
    config UBOOT_X108NX-1-1-3
      bool "X108NX Using Uboot v1.1.3"
      depends on Uboot-spi-1-1-3
      default n
    config UBOOT_X116NX-1-1-3
      bool "X116NX Using Uboot v1.1.3"
      depends on Uboot-spi-1-1-3
      default n
  endchoice
  choice
    prompt "Choose product"
    depends on PACKAGE_Uboot-spi && Uboot-spi-3-5-2-0
    config UBOOT_X108NX-3-5-2-0
      bool "Uboot for X108NX Using Uboot v3.5.2.0"
      depends on Uboot-spi-3-5-2-0
      default n
    config UBOOT_X116NX-3-5-2-0
      bool "Uboot for X116NX Using Uboot v3.5.2.0"
      depends on Uboot-spi-3-5-2-0
      default n
    config UBOOT_PROXICAST-3-5-2-0
      bool "Uboot for Proxicast Using Uboot v3.5.2.0"
      depends on Uboot-spi-3-5-2-0
      default n
    config UBOOT_GARMIN-3-5-2-0
      bool "Uboot for Garmin Using Uboot v3.5.2.0"
      depends on Uboot-spi-3-5-2-0
      default n
    config UBOOT_TOGOMusic-3-5-2-0
      bool "Uboot for TOGOMusic Using Uboot v3.5.2.0"
      depends on Uboot-spi-3-5-2-0
      default n
  endchoice
  choice
    prompt "Choose product"
    depends on PACKAGE_Uboot-spi && Uboot-spi-3-6-0-0
    config UBOOT_Mosra-3-6-0-0
      bool "Uboot for Mosra Using Uboot v3.6.0.0"
      depends on Uboot-spi-3-6-0-0
      default n
    config UBOOT_TOGOBox-3-6-0-0
      bool "Uboot for TOGOBox Using Uboot v3.6.0.0"
      depends on Uboot-spi-3-6-0-0
      default n
    config UBOOT_TOGOMusic-3-6-0-0
      bool "Uboot for TOGOMusic Using Uboot v3.6.0.0"
      depends on Uboot-spi-3-6-0-0
      default n
  endchoice
  choice
    prompt "Choose product"
    depends on PACKAGE_Uboot-spi && Uboot-spi-4-1-2-0
    config UBOOT_QGenie-4-1-2-0
      bool "Uboot for QGenie Using Uboot SDK v4.1.2.0, on MT7620"
      depends on Uboot-spi-4-1-2-0
      default n
    config UBOOT_M2-4-1-2-0
      bool "Uboot for M2 Using Uboot SDK v4.1.2.0, on MT7620"
      depends on Uboot-spi-4-1-2-0
      default n
    config UBOOT_Allis-4-1-2-0
      bool "Uboot for Allis Using Uboot SDK v4.1.2.0, on MT7620"
      depends on Uboot-spi-4-1-2-0
      default n
  endchoice
  config UBOOT_TARGET_PRODUCT
    string
	depends on PACKAGE_Uboot-spi
    default X108NX if UBOOT_X108NX-3-5-2-0 || UBOOT_X108NX-1-1-3
    default X116NX if UBOOT_X116NX-3-5-2-0 || UBOOT_X116NX-1-1-3
    default Mosra if UBOOT_Mosra-3-6-0-0
    default TOGOBox if UBOOT_TOGOBox-3-6-0-0
    default TOGOMusic if UBOOT_TOGOMusic-3-6-0-0 || UBOOT_TOGOMusic-3-5-2-0
	default Garmin if UBOOT_GARMIN-3-5-2-0
	default Proxicast if UBOOT_PROXICAST-3-5-2-0
	default QGenie if UBOOT_QGenie-4-1-2-0
	default M2 if UBOOT_M2-4-1-2-0
	default Allis if UBOOT_Allis-4-1-2-0
    default X108NX
  config UBOOT_PRODUCT_VERSION
    string
    prompt "Uboot customer production version, not SDK version nor Uboot open source version"
	depends on PACKAGE_Uboot-spi
  config UBOOT_PRODUCT_KERNEL_SIZE
    int
    prompt "Uboot kernel roughly size"
	depends on PACKAGE_Uboot-spi
  config UBOOT_PRODUCT_IGNORE_CHECKSUM
    bool
    prompt "Ignore boot time checksum verify"
	depends on PACKAGE_Uboot-spi
    default n
endef

ifeq ($(CONFIG_EZP_PRODUCT_SUPERCATEGORY),)
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

EXTRA_CFLAGS+= CROSS_COMPILE=$(TARGET_CROSS)
ifeq ($(CONFIG_UBOOT_PRODUCT_IGNORE_CHECKSUM),y)
RELFLAGS += -DUBOOT_PRODUCT_IGNORE_CHECKSUM=1
else
RELFLAGS += -DUBOOT_PRODUCT_IGNORE_CHECKSUM=0
endif
LIBS+= -L$(PKG_BUILD_DIR)/../root-rt305x/lib -L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib

define Build/Prepare
	rm -rf patches
	ln -s uboot_patches/$(PKG_VERSION)/$(CONFIG_UBOOT_TARGET_PRODUCT) patches
	$(call Build/Prepare/Default)
endef

define Build/Compile
	cp ./files/$(PKG_VERSION)/$(CONFIG_UBOOT_TARGET_PRODUCT)/config $(PKG_BUILD_DIR)/.config
	cp ./files/$(PKG_VERSION)/$(CONFIG_UBOOT_TARGET_PRODUCT)/autoconf.h $(PKG_BUILD_DIR)/autoconf.h
	cp ./files/$(PKG_VERSION)/$(CONFIG_UBOOT_TARGET_PRODUCT)/System.map $(PKG_BUILD_DIR)/System.map
	CROSS_COMPILE=$(TARGET_CROSS) \
       RELFLAGS="$(RELFLAGS) -DUBOOT_PRODUCT_KERNEL_SIZE=$(CONFIG_UBOOT_PRODUCT_KERNEL_SIZE) -DRALINK_LOCAL_VERSION=\\\"$(PKG_VERSION).$(CONFIG_UBOOT_PRODUCT_VERSION)\\\"" $(MAKE) -C $(PKG_BUILD_DIR) all
endef

define Package/Uboot-spi/install
	$(CP) ./files/*.img $(PKG_BUILD_DIR)/
	@echo "Make install"
endef

$(eval $(call BuildPackage,Uboot-spi))
