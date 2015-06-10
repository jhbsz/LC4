# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=syslog-ng
PKG_VERSION:=1.6.12
PKG_RELEASE:=1
PKG_MD5SUM:=a3cbfdb6e1e5beea181a7349749719f3

LIBOL_VERSION:=0.3.18

PKG_SOURCE_URL:=http://www.balabit.com/downloads/files/syslog-ng/sources/1.6/src/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/syslog-ng
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=A new-generation syslog daemon.
endef

define Package/syslog-ng/description
  'syslog-ng' is a new-generation syslog daemon. It tries to fill the gaps original syslogd's were lacking:
	* powerful configurability
    * filtering based on message content
    * message integrity, message encryption (not yet implemented in 1.4.x)
    * portability
    * better network forwarding
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR)/libol-$(LIBOL_VERSION);\
		rm -rf config.{cache,status} ; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		./configure \
		   --target=$(GNU_TARGET_NAME) \
		   --host=$(GNU_TARGET_NAME) \
		   --build=$(GNU_HOST_NAME) \
	)
	(cd $(PKG_BUILD_DIR); rm -rf config.{cache,status} ; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		./configure \
		  --target=$(GNU_TARGET_NAME) \
		  --host=$(GNU_TARGET_NAME) \
		  --build=$(GNU_HOST_NAME) \
	)
endef
define Build/Compile
	$(MAKE) -C "$(PKG_BUILD_DIR)/libol-$(LIBOL_VERSION)"
	$(MAKE) -C "$(PKG_BUILD_DIR)"
endef
define Package/syslog-ng/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/syslog-ng $(1)/sbin/
	$(INSTALL_BIN) ./files/syslog-ng-init $(1)/sbin/
	$(INSTALL_DIR) $(1)/etc/syslog-ng
	$(INSTALL_DATA) ./files/syslog-ng.tpl $(1)/etc/syslog-ng/
endef

$(eval $(call BuildPackage,syslog-ng))
