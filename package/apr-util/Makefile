#
# Copyright (C) 2007-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=apr-util
PKG_VERSION:=1.3.12
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.ibiblio.org/pub/mirrors/apache/apr
PKG_MD5SUM:=0f671b037ca62751a8a7005578085560

PKG_FIXUP:=autoreconf
PKG_REMOVE_FILES:=aclocal.m4 build/ltmain.sh

PKG_INSTALL:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/libaprutil
  SECTION:=libs
  CATEGORY:=Libraries
#  DEPENDS:=+libapr +libexpat +libsqlite3 +libuuid $(ICONV_DEPENDS)
  DEPENDS:=+libapr +libexpat +libsqlite3 +libuuid $(ICONV_DEPENDS) @BUILD_NLS
  TITLE:=Apache Portable Runtime Utility Library
  URL:=http://apr.apache.org/
endef

TARGET_CFLAGS += $(FPIC)
TARGET_CPPFLAGS += -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE

CONFIGURE_ARGS += \
	--with-pgsql=no \
	--with-apr="$(STAGING_DIR)/usr/bin/apr-1-config" \
	--with-expat="$(STAGING_DIR)/usr" \
	--without-sqlite2 \
	--with-sqlite3="$(STAGING_DIR)/usr" \
        --with-iconv="$(ICONV_PREFIX)" \

CONFIGURE_VARS += \
	ac_cv_file_dbd_apr_dbd_mysql_c=no \
	ac_cv_path_ODBC_CONFIG= \
	APR_BUILD_DIR="$(STAGING_DIR)/usr/share/build-1" \

MAKE_FLAGS += \
	APRUTIL_LIBS="-lsqlite3 $(TARGET_LDFLAGS) -lexpat $(if $(ICONV_FULL),-liconv) -lapr-1 -luuid -lm -lcrypt" \

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/bin $(1)/usr/include/apr-1/ $(1)/usr/lib  $(1)/usr/lib/pkgconfig/
	$(CP)   $(PKG_INSTALL_DIR)/usr/bin/apu-1-config \
	 	$(1)/usr/bin/
	$(CP)   $(PKG_INSTALL_DIR)/usr/include/apr-1/* \
		$(1)/usr/include/apr-1/
	$(CP)   $(PKG_INSTALL_DIR)/usr/lib/libaprutil-1.{la,a,so*} \
		$(1)/usr/lib/
	$(CP)   $(PKG_BUILD_DIR)/libaprutil-1.la $(1)/usr/lib/
	$(CP)   $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/apr-util-1.pc \
		$(1)/usr/lib/pkgconfig/
	$(SED) 's, -e \"s -I$$$$prefix/lib  g\",,g' $(1)/usr/bin/apu-1-config
	$(SED) 's,^libdir=\"$$$${exec_prefix}/lib,libdir=\"$(STAGING_DIR)/usr/lib,g' $(1)/usr/bin/apu-1-config
	$(SED) 's,^includedir=\"$$$${prefix}/include/,includedir=\"$(STAGING_DIR)/usr/include/,g' $(1)/usr/bin/apu-1-config
	$(SED) 's,-L$$$$libdir,,g' $(1)/usr/bin/apu-1-config
	$(SED) 's,-R$$$$libdir,,g' $(1)/usr/bin/apu-1-config
endef

#adding a line to avoid "libtool link" error when build apache:
#$(CP)   $(PKG_BUILD_DIR)/libaprutil-1.la $(1)/usr/lib/
#
#'libaprutil-1.la' and 'libaprutil-1.lai' are almost the same but 'dependency_libs' not correct in libaprutil-1.lai
#And then 'libaprutil-1.lai' will be copied to "$(PKG_INSTALL_DIR)/usr/lib" as 'libaprutil-1.la' (description in this Makefile)
#More information: see "$(PKG_BUILD_DIR)/.libs/libaprutil-1.la", "$(PKG_BUILD_DIR)/.libs/libaprutil-1.lai"

define Package/libaprutil/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libaprutil-1.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libaprutil))
