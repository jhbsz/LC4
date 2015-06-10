#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=salsa-lib
PKG_VERSION:=0.1.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=ftp://ftp.suse.com/pub/people/tiwai/salsa-lib/

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

ifeq ($(KERNEL),2.4)
DEPENDS_KMOD:=+kmod-alsa
else
DEPENDS_KMOD:=+kmod-sound-core
endif

define Package/salsa-lib
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=SALSA (Advanced Linux Sound Architecture) library
  URL:=http://www.alsa-project.org/
endef
  #DEPENDS:=@AUDIO_SUPPORT $(DEPENDS_KMOD)

define Package/salsa-lib/description
 This is the library package for alsa, needed by some userspace programs.
 You must have enabled the ALSA support in the kernel.
endef

TARGET_CFLAGS += $(FPIC)

#		--enable-libasound \

define Build/Configure
	$(call Build/Configure/Default, \
		--disable-mixer \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include/alsa \
		$(1)/usr/include/

	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/libsalsa.{la,a,so*} \
		$(1)/usr/lib/
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/alsa.pc \
		$(1)/usr/lib/pkgconfig/

	$(INSTALL_DIR) $(1)/usr/share/aclocal
	$(INSTALL_DATA) \
		$(PKG_INSTALL_DIR)/usr/share/aclocal/alsa.m4 \
		$(1)/usr/share/aclocal/
endef

define Package/salsa-lib/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/libsalsa.so.* \
		$(1)/usr/lib/
endef

$(eval $(call BuildPackage,salsa-lib))
