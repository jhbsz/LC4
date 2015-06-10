# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=rt2860apd
PKG_VERSION:=2.3.0.0
PKG_RELEASE:=1

PKG_SOURCE:=rt2860ap-$(PKG_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/rt2860ap-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/rt2860apd
	SECTION:=uitls
	CATEGORY:=Network
	TITLE:=Ralink wireless management daemon
endef

define Package/rt2860apd/description
	  A wireless management daemon for Ralink RT2860 11n wireless card.
endef

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)/src
	$(CP) ./src/* $(PKG_BUILD_DIR)/src
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		$(TARGET_CONFIGURE_OPTS) 
	$(MAKE) -C $(PKG_BUILD_DIR)/DOT1X \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -I. -I $(STAGING_DIR)/usr/include -I $(PKG_BUILD_DIR)"
endef

define Package/rt2860apd/install
	$(INSTALL_DIR) $(1)/etc/wl
	$(INSTALL_BIN) ./files/rt2860ap-init $(1)/etc/wl
	$(INSTALL_DIR) $(1)/etc_ro/wlan/
	$(INSTALL_BIN) ./files/RT3050_AP_1T1R_V1_0.bin $(1)/etc_ro/wlan/
	$(INSTALL_BIN) ./files/RT3052_AP_2T2R_V1_1.bin $(1)/etc_ro/wlan/
	$(INSTALL_BIN) ./files/RT5350_AP_1T1R_V1_0.bin $(1)/etc_ro/wlan/
	$(INSTALL_BIN) ./files/RT3352_AP_2T2R_V11.BIN $(1)/etc_ro/wlan/
	$(INSTALL_BIN) ./files/MT7620_AP_2T2R-4L_V13.BIN $(1)/etc_ro/wlan/
	$(INSTALL_DIR) $(1)/etc/Wireless
	ln -sf /etc/wl $(1)/etc/Wireless/RT2860
	ln -sf /var/RT2860AP.dat $(1)/etc/wl/RT2860AP.dat
	ln -sf /var/RT2860AP.dat $(1)/etc/wl/RT2860.dat
	-find $(1) -type d -name CVS | xargs rm -rf
	-find $(1) -type d -name .svn | xargs rm -rf
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/DOT1X/rt2860apd $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/sbin
	# $(INSTALL_BIN) ./files/ezp-wl-mode $(1)/sbin
	$(INSTALL_BIN) ./files/ezp-wl-ctrl $(1)/sbin
	$(INSTALL_BIN) ./files/ezp-wl-mode $(1)/sbin
	# $(INSTALL_BIN) ./files/ezp-auto-bridge $(1)/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	# $(INSTALL_BIN) ./files/autobridge $(1)/etc/init.d/
	$(CP) ./files/rt2860apd.init $(1)/etc/init.d/rt2860apd
	$(CP) $(PKG_BUILD_DIR)/src/encode_ssid $(1)/etc/wl
	$(CP) $(PKG_BUILD_DIR)/src/site_survey $(1)/etc/wl
	$(CP) $(PKG_BUILD_DIR)/src/b64 $(1)/etc/wl
endef

$(eval $(call BuildPackage,rt2860apd))
