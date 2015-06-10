# $Id: Makefile 4782 2006-09-12 12:32:27Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=uvncrepeater
PKG_VERSION:=0.14
PKG_RELEASE:=1

PKG_SOURCE_URL:=http://nchc.dl.sourceforge.net/sourceforge/ultravnc
PKG_SOURCE:=$(PKG_NAME).tar.gz
PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/UVNCRepeater

include $(INCLUDE_DIR)/package.mk

define Package/vncrepeater
  SECTION:=utils
  CATEGORY:=Network
  TITLE:=vncrepeater
endef

define Package/vncrepeater/Description
  VNC repeater
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	./files/replace_cpp $(PKG_BUILD_DIR)
endef


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC=$(TARGET_CC) CFLAGS="$(TARGET_CFLAGS)"
endef

define Package/vncrepeater/install
	$(INSTALL_DIR) $(1)/etc/ $(1)/usr/sbin/ $(1)/etc/init.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/repeater $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/start-stop-daemon $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/vncrepeater.ini $(1)/etc/
	$(INSTALL_BIN) ./files/vncrepeater.init $(1)/etc/init.d/vncrepeater
endef

$(eval $(call BuildPackage,vncrepeater))
