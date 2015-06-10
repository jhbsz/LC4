include $(TOPDIR)/rules.mk

PKG_NAME:=bpalogin
PKG_VERSION:=2.0.2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk


define Package/bpalogin
  SECTION:=net
  CATEGORY:=Network
  TITLE:=BPA Login
endef

define Package/bpalogin/description
	BPA Login: For Bigpond use
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR) "

endef

define Package/bpalogin/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/bpalogin $(1)/usr/sbin
	$(INSTALL_BIN) ./files/bpalogin.init $(1)/etc/init.d/bpalogin
	ln -sf /tmp/bpalogin.conf $(1)/etc/bpalogin.conf

endef
$(eval $(call BuildPackage,bpalogin))
