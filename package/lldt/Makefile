include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=lldt
PKG_VERSION:=1.0.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
include $(INCLUDE_DIR)/package.mk

define Package/lldt
	SECTION:=net
	CATEGORY:=Network
	TITLE:=Link Layer Topology Discovery
endef

define Package/lldt/description
	Link Layer Topology Discovery
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
	$(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS)" \
	LD="$(TARGET_CC)"
endef

define Package/lldt/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d
	$(CP) $(PKG_BUILD_DIR)/lld2d $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/lld2test $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/icon.ico $(1)/etc
	$(CP) $(PKG_BUILD_DIR)/icon.large.ico $(1)/etc
	$(CP) $(PKG_BUILD_DIR)/lld2d.conf $(1)/etc
	$(INSTALL_BIN) ./files/lltd.sh $(1)/usr/sbin
	$(INSTALL_BIN) ./files/lldt.init $(1)/etc/init.d/lldt
endef
$(eval $(call BuildPackage,lldt))
