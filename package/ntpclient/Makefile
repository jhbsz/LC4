# $Id: Makefile 3927 2006-06-09 03:05:11Z nbd $
# Originally, CFLAGS are assigned as the following:
#		CC=$(TARGET_CC) CFLAGS="$(TARGET_CFLAGS)" all adjtimex
# This causes -DUSE_OBSOLETE_GETTIMEOFDAY is not enabled at the package
# Makefile.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ntpclient
PKG_VERSION:=2007_365
PKG_RELEASE:=1
PKG_MD5SUM:=cb98711f11769cdd22fc592844cef414

PKG_SOURCE_URL:=http://doolittle.faludi.com/ntpclient
PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-2007

include $(INCLUDE_DIR)/package.mk
define Package/ntpclient
  SECTION:=net
  CATEGORY:=Network
  TITLE:=NTP (Network Time Protocol) client.
endef
define Package/ntpclient/description
  NTP client for setting system time from NTP servers.
endef
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC=$(TARGET_CC) all adjtimex
endef

define Package/ntpclient/install
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/ntpclient.script $(1)/etc/init.d/ntpclient
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ntpclient $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/adjtimex $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rate.awk $(1)/usr/sbin/

endef
$(eval $(call BuildPackage,ntpclient))
