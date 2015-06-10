#
# Copyright (C) 2007-2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 11950 2008-07-26 22:32:55Z nico $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=iscsitarget
PKG_VERSION:=0.4.17

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

include $(INCLUDE_DIR)/package.mk

define Package/iscsitarget/Default
  TITLE:=iSCSITarget
  URL:=http://www.iscsitarget.org/
endef

define Package/iscsitarget/Default/description
  iSCSI target package
endef

define Package/iscsitarget
  SECTION:=net
  CATEGORY:=Network
  TITLE:=ISCSI target
  DEPENDS:=+libopenssl
endef

define Package/iscsitarget/description
  iSCSI target server
endef

define KernelPackage/iscsitarget
$(call Package/iscsitarget/Default)
  SUBMENU:=Network Support
  DEPENDS:=@LINUX_2_6
  TITLE+= (kernel module)
  FILES:=$(PKG_BUILD_DIR)/kernel/iscsi_trgt.$(LINUX_KMOD_SUFFIX)
endef

define KernelPackage/iscsitarget/description
$(call Package/iscsitarget/Default/description)
 This package contains the iSCSI kernel module.
endef


define Build/Compile
	KSRC="$(LINUX_DIR)" \
	CFLAGS="$(TARGET_CFLAGS) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE" \
	LDFLAGS="-L $(STAGING_DIR)/usr/lib" \
	CC="$(TARGET_CC)" \
	CPPFLAGS="$(TARGET_CPPFLAGS) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	ARCH="$(LINUX_KARCH)" \
	DESTDIR="$(PKG_INSTALL_DIR)" \
	XDESTDIR="$(PKG_INSTALL_DIR)" \
	$(MAKE) -C $(PKG_BUILD_DIR) \
	all 
endef

define Package/iscsitarget/install
	echo "start"
#	KSRC="$(LINUX_DIR)" \
#	CFLAGS="$(TARGET_CFLAGS) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE" \
#	LDFLAGS="-L $(STAGING_DIR)/usr/lib" \
#	CC="$(TARGET_CC)" \
#	CPPFLAGS="$(TARGET_CPPFLAGS) -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE" \
#	CROSS_COMPILE="$(TARGET_CROSS)" \
#	ARCH="$(LINUX_KARCH)" \
#	DESTDIR="$(PKG_INSTALL_DIR)" \
#	XDESTDIR="$(PKG_INSTALL_DIR)" \
#	XDISTDIR="$(1)" \
#	KMOD="" \
#	INSTALL_MOD_PATH="" \
#	DISTDIR="$(1)" \
#	$(MAKE) -C $(PKG_BUILD_DIR) \
#	install-usr install-etc install-kernel

	$(INSTALL_DIR) $(1)/etc/{iscsitarget,init.d}
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/lib/modules/2.6.26
	install -vD $(PKG_BUILD_DIR)/usr/ietd $(1)/usr/sbin/ietd
	install -vD $(PKG_BUILD_DIR)/usr/ietadm $(1)/usr/sbin/ietadm
	install -vD -m 755 files/iscsi-target $(1)/etc/init.d/iscsi-target
	ln -sf /tmp/ietd.conf $(1)/etc/ietd.conf
	ln -sf /tmp/initiators.allow $(1)/etc/initiators.allow
	ln -sf /tmp/initiators.deny $(1)/etc/initiators.deny
	install -vD $(PKG_BUILD_DIR)/kernel/iscsi_trgt.ko $(1)/lib/modules/2.6.26/iscsi_trgt.ko
	echo "end"
endef

showvar:
	echo $(PKG_INSTALL_DIR)
	echo $(PKG_BUILD_DIR)
	echo $(LINUX_DIR)
	echo $(IPKG_TMP)

$(eval $(call BuildPackage,iscsitarget))
$(eval $(call KernelPackage,iscsitarget))
