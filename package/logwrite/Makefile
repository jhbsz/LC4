# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=logwrite
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/logwrite
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=A tool that writes messages into shared memory.
  DEPENDS:=+ezp-nvram
endef

define Package/logwrite/description
	'logwrite' takes STDIN as input and writes messages into log buffer in shared memory with specified key ID.
endef

ifeq ($(CONFIG_EZP_SECRET), "PROX")
TARGET_CFLAGS+=-DDEFAULT_SYSLOG_BUFFER_SIZE=32
endif

define Build/Compile
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS) \
	    CFLAGS="$(TARGET_CFLAGS)"
endef

define Package/logwrite/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/logwrite $(1)/bin/
	$(INSTALL_BIN) ./files/ezp-rstlog $(1)/bin/
endef

$(eval $(call BuildPackage,logwrite))
