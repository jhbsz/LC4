#
# Copyright (C) 2013 Zoltan Hubai
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ddserver
PKG_VERSION:=0.2
PKG_RELEASE:=12

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/ddserver
  SECTION:=utils
  CATEGORY:=Multimedia
  DEPENDS:=+libusb-1.0
  TITLE:=Server for DSLR camera to use with DslrDashboard
  MAINTAINER:=Zoltan Hubai <hubaiz@gmail.com>
endef

define Package/ddserver/description
 Server for DslrDashboard for controling connected DSLR
 camera with USB
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

		#LIBS="-nodefaultlibs -lgcc -lc -lusb-1.0 -lpthread -luClibc++"

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		LIBS=" -lgcc -lc -lusb-1.0 -lpthread -lstdc++ " \
		LDFLAGS="$(EXTRA_LDFLAGS) -L $(STAGING_DIR)/usr/lib -pthread" \
		CXXFLAGS="$(TARGET_CFLAGS) $(EXTRA_CPPFLAGS) \
		-I $(STAGING_DIR)/usr/include -pthread" \
		$(TARGET_CONFIGURE_OPTS) \
		CROSS="$(TARGET_CROSS)" \
		ARCH="$(ARCH)" 
endef

define Package/ddserver/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ddserver $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/ddserver.init $(1)/etc/init.d/ddserver
endef

define Package/ddserver/postinst
#!/bin/sh
[ -n "$${IPKG_INSTROOT}" ] || /etc/init.d/ddserver enable || true
endef

$(eval $(call BuildPackage,ddserver))
