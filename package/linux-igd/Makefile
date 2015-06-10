include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=linux-igd
PKG_VERSION:=1.0.0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
include $(INCLUDE_DIR)/package.mk

# To enable upnpd check alive
# TARGET_CFLAGS+= -DUPNP_CHECK_ALIVE
# To enable periodic callback
# TARGET_CFLAGS+= -DPERIODIC_CALLBACK
# To enable x tag support
# TARGET_CFLAGS+= -DX_TAG_SUPPORT
TARGET_CFLAGS+= -DDEBUG
TARGET_CFLAGS+= -DX_TAG_SUPPORT1
# TARGET_CFLAGS+= -DX_TAG_SUPPORT

define Package/linux-igd
	SECTION:=net
	CATEGORY:=Network
	TITLE:=IGD UPNP support
	DEPENDS:=+libupnp
endef

define Package/linux-igd/description
	IGD UPNP support
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
	$(TARGET_CONFIGURE_OPTS) \
	CFLAGS="$(TARGET_CFLAGS) -D_GNU_SOURCE -I$(STAGING_DIR)/usr/include/upnp -I$(LINUX_DIR)/drivers/net/raeth -L$(STAGING_DIR)/usr/lib " \
	LD="$(TARGET_CC) -L$(STAGING_DIR)/usr/lib "
endef

define Package/linux-igd/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/upnpd $(1)/usr/bin
	$(INSTALL_BIN) ./files/upnp_xml.sh $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/linuxigd
	ln -sf /tmp/gateconnSCPD.xml $(1)/etc/linuxigd/gateconnSCPD.xml
	ln -sf /tmp/gateicfgSCPD.xml $(1)/etc/linuxigd/gateicfgSCPD.xml
	ln -sf /tmp/l3fwdSCPD.xml $(1)/etc/linuxigd/l3fwdSCPD.xml
	ln -sf /tmp/gatedesc.xml $(1)/etc/linuxigd/gatedesc.xml
	$(INSTALL_DATA) ./files/ligd.gif $(1)/etc/linuxigd
	$(INSTALL_DATA) ./files/upnpd.conf $(1)/etc/linuxigd
#	$(INSTALL_BIN) ./files/*.xml $(1)/etc/linuxigd
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/upnpd.init $(1)/etc/init.d/upnpd
endef
$(eval $(call BuildPackage,linux-igd))
