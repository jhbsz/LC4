#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=lighttpd
PKG_VERSION:=1.4.30
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://download.lighttpd.net/lighttpd/releases-1.4.x

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

ifeq ($(CONFIG_TARGET_ralink_7620),y)
EZP_HTTPD_ARCH_CFLAGS:=-DCONFIG_EZP_ARCH_RT305X
endif

ifeq ($(CONFIG_EZP_SECRET), "TOGOBOX")
EZP_BRAND:=AXIM
  # For M2, we are using Model or platform to distinct between M1 and M2, not
  # only by CONFIG_EZP_SECRET
  ifeq ($(CONFIG_EZP_MODEL),"QGenie")
    EZP_COPY_FILES=$(CONFIG_EZP_MODEL)
  endif
endif

EZP_CFLAGS+= -DWAN_NUM=$(CONFIG_WAN_NUM) \
             -DLAN_NUM=$(CONFIG_LAN_NUM) \
             -DWL_NUM=$(CONFIG_WL_NUM) \
             -DEZP_PROD_BRAND_$(EZP_BRAND) \
             -DEZP_SUB_BRAND_$(CONFIG_EZP_SECRET) \
             -DPLATFORM=$(CONFIG_TARGET_PLATFORM) \
             -DPLATFORM_$(CONFIG_TARGET_PLATFORM) \
             -DEZP_COPYRIGHT=\\\"$(CONFIG_HTTPD_COPYRIGH_TEXT)\\\" \
             -DEZP_LOGO_$(CONFIG_HTTPD_LOGO_SHOW) \
             -DEZP_PROD_SUBCAT=$(CONFIG_EZP_PRODUCT_SUBCATEGORY) \

define Package/lighttpd/Default
  SUBMENU:=Web Servers/Proxies
  SECTION:=net
  CATEGORY:=Network
  URL:=http://www.lighttpd.net/
endef

define Package/lighttpd
  $(call Package/lighttpd/Default)
  MENU:=1
  DEPENDS:=ezp-nvram +libopenssl +libpcre
  TITLE:=A flexible and lightweight web server
endef

define Package/lighttpd-mod-access
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Access restrictions module
endef

define Package/lighttpd-mod-accesslog
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Access logging module
endef

define Package/lighttpd-mod-alias
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Directory alias module
endef

define Package/lighttpd-mod-auth
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Authentication module
endef

define Package/lighttpd-mod-cgi
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=CGI module
endef

define Package/lighttpd-mod-cml
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Cache Meta Language module
endef

define Package/lighttpd-mod-compress
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +zlib
  TITLE:=Compress output module
endef

define Package/lighttpd-mod-evasive
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Evasive module
endef

define Package/lighttpd-mod-evhost
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Exnhanced Virtual-Hosting module
endef

define Package/lighttpd-mod-expire
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Expire module
endef

define Package/lighttpd-mod-extforward
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Extract client IP module
endef

define Package/lighttpd-mod-fastcgi
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +qcloud
  TITLE:=FastCGI module
endef

define Package/lighttpd-mod-flv-streaming
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=flv streaming module
endef

define Package/lighttpd-mod-magnet
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=magnet module
endef

define Package/lighttpd-mod-mysql-vhost
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +libmysqlclient
  TITLE:=Mysql virtual hosting module
endef

define Package/lighttpd-mod-proxy
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Proxy module
endef

define Package/lighttpd-mod-redirect
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +libpcre
  TITLE:=URL redirection module
endef

define Package/lighttpd-mod-rewrite
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +libpcre
  TITLE:=URL rewriting module
endef

define Package/lighttpd-mod-rrdtool
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=rrdtool module
endef

define Package/lighttpd-mod-scgi
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=SCGI module
endef

define Package/lighttpd-mod-secdownload
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Secure and fast download module
endef

define Package/lighttpd-mod-setenv
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Environment variable setting module
endef

define Package/lighttpd-mod-simple-vhost
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Simple virtual hosting module
endef

define Package/lighttpd-mod-ssi
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +libpcre
  TITLE:=SSI module
endef

define Package/lighttpd-mod-status
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=Server status display module
endef

define Package/lighttpd-mod-trigger-b4-dl
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +libpcre
  TITLE:=Trigger before download module
endef

define Package/lighttpd-mod-userdir
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=User directory module
endef

define Package/lighttpd-mod-usertrack
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd
  TITLE:=User tracking module
endef

define Package/lighttpd-mod-webdav
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +libsqlite3 +libuuid +libxml2
  TITLE:=WebDAV module
endef

define Package/lighttpd-cgi-qfile
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +lighttpd-mod-cgi +libjson
  TITLE:=QFile cgi methods with CGI module
endef

define Package/lighttpd-fastcgi-qfile
  $(call Package/lighttpd/Default)
  DEPENDS:=lighttpd +lighttpd-mod-fastcgi +libjson +fcgi
  TITLE:=QFile cgi methods with FAST CGI module
endef

ifdef CONFIG_PACKAGE_lighttpd-cgi-qfile
EXTRA_CFLAGS+= -DPLATFORM_$(CONFIG_TARGET_PLATFORM)  -D_FILE_OFFSET_BITS=64 

EXTRA_LDFLAGS+= -L$(STAGING_DIR)/usr/lib -lpthread -lxml2 -ljson -ljson-c -lnvram -lz -lshared -lm -lcrypto -luuid
endif

ifdef CONFIG_PACKAGE_lighttpd-fastcgi-qfile
EXTRA_CFLAGS+= -DPLATFORM_$(CONFIG_TARGET_PLATFORM) -D_FILE_OFFSET_BITS=64 

EXTRA_LDFLAGS+= -L$(STAGING_DIR)/usr/lib -lpthread -lxml2 -ljson -ljson-c -lnvram -lz -lshared -lm -lcrypto -luuid
endif

EXTRA_LDFLAGS+= -lpthread

CONFIGURE_ARGS+= \
    --prefix=$(STAGING_DIR) \
    --with-webdav-locks \
    --with-webdav-props \
	--libdir=/usr/lib/lighttpd \
	--sysconfdir=/etc/lighttpd \
	--enable-shared \
	--enable-static \
	--without-attr \
	--without-bzip2 \
	--without-fam \
	--without-gdbm \
	--without-ldap \
	--without-lua \
	--without-memcache \
	--without-mysql \
	--with-openssl="$(STAGING_DIR)/usr" \
	--with-pcre \
	--without-valgrind \
	 $(call autoconf_bool,CONFIG_IPV6,ipv6)

CONFIGURE_VARS+= \
	PCRE_LIB="-lpcre" \

define Build/Configure
$(call Build/Configure/Default)
	# XXX: override pcre (mis)detection by ./configure when cross-compiling
	echo "#define HAVE_LIBPCRE 1" >>$(PKG_BUILD_DIR)/config.h
	echo "#define HAVE_PCRE_H 1" >>$(PKG_BUILD_DIR)/config.h
endef

ifdef CONFIG_PACKAGE_lighttpd-cgi-qfile

  define Build/Prepare/lighttpd-cgi-qfile
	mkdir -p $(PKG_BUILD_DIR)/cgi-bin
	$(CP) ./src/* $(PKG_BUILD_DIR)/cgi-bin
  endef

  define Build/Compile/lighttpd-cgi-qfile
	$(MAKE) -C $(PKG_BUILD_DIR)/cgi-bin \
        $(TARGET_CONFIGURE_OPTS) \
        LDFLAGS="$(EXTRA_LDFLAGS)" \
        CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include \
			-I$(STAGING_DIR)/usr/include/libxml2 -I$(STAGIN_DIR)/usr/include/json-c -I /usr/include \
			-I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram $(EZP_HTTPD_ARCH_CFLAGS) \
        	$(EZP_CFLAGS)"
  endef

endif

ifdef CONFIG_PACKAGE_lighttpd-fastcgi-qfile

  define Build/Prepare/lighttpd-fastcgi-qfile
	mkdir -p $(PKG_BUILD_DIR)/cgi-bin
	$(CP) ./src/* $(PKG_BUILD_DIR)/cgi-bin
	$(CP) ./src_httpd/* $(PKG_BUILD_DIR)/cgi-bin
  endef

  define Build/Compile/lighttpd-fastcgi-qfile
	$(MAKE) -C $(PKG_BUILD_DIR)/cgi-bin \
        $(TARGET_CONFIGURE_OPTS) \
        LDFLAGS="$(EXTRA_LDFLAGS) -L$(STAGING_DIR)/usr/lib -lqcloud -lqcloud_util -lmcn  -ljson -ljson-c -lshared -lnvram -lcurl -lsqlite3 -lm -lssl -lcrypto -lpthread " \
        CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS) -Wall -I. -I $(STAGING_DIR)/usr/include \
			-I$(STAGING_DIR)/usr/include/libxml2 -I$(STAGIN_DIR)/usr/include/json-c \
			-I $(PKG_BUILD_DIR) -I $(BUILD_DIR)/ezp-nvram $(EZP_HTTPD_ARCH_CFLAGS) \
        	$(EZP_CFLAGS) "
  endef

endif

define Build/Prepare
$(call Build/Prepare/Default)
$(call Build/Prepare/lighttpd-cgi-qfile)
$(call Build/Prepare/lighttpd-fastcgi-qfile)
endef

define Build/Compile
$(call Build/Compile/Default)
$(call Build/Compile/lighttpd-cgi-qfile)
$(call Build/Compile/lighttpd-fastcgi-qfile)
endef

define Package/lighttpd/conffiles
/etc/lighttpd/lighttpd.conf.template
endef

define Package/lighttpd/install
	$(INSTALL_DIR) $(1)/etc/lighttpd/
	$(INSTALL_DIR) $(1)/etc/lighttpd/cgi-bin/filemanager
ifdef CONFIG_PACKAGE_lighttpd-cgi-qfile
	$(INSTALL_DATA) ./files/lighttpd.conf.qfile.cgi $(1)/etc/lighttpd/lighttpd.conf.template
else
ifdef CONFIG_PACKAGE_lighttpd-fastcgi-qfile
	$(INSTALL_DATA) ./files/lighttpd.conf.qfile.fcgi $(1)/etc/lighttpd/lighttpd.conf.template
else
	$(INSTALL_DATA) ./files/lighttpd.conf $(1)/etc/lighttpd/lighttpd.conf.template
endif
endif
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/lighttpd.init $(1)/etc/init.d/lighttpd
	$(INSTALL_DIR) $(1)/usr/lib/lighttpd
	for m in dirlisting indexfile staticfile; do \
		$(CP) $(PKG_INSTALL_DIR)/usr/lib/lighttpd/mod_$$$${m}.so $(1)/usr/lib/lighttpd/ ; \
	done
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/lighttpd $(1)/usr/sbin/
#	$(CP) $(PKG_BUILD_DIR)/cgi-bin/authLogin.cgi $(1)/etc/lighttpd/cgi-bin
#	$(CP) $(PKG_BUILD_DIR)/cgi-bin/sysinfoReq.cgi $(1)/etc/lighttpd/cgi-bin
#	$(CP) $(PKG_BUILD_DIR)/cgi-bin/utilRequest.cgi $(1)/etc/lighttpd/cgi-bin/filemanager
endef

define Package/lighttpd-cgi-qfile/install
	$(INSTALL_DIR) $(1)/etc/lighttpd/cgi-bin/filemanager
	$(CP) $(PKG_BUILD_DIR)/cgi-bin/authLogin.cgi $(1)/etc/lighttpd/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-bin/sysinfoReq.cgi $(1)/etc/lighttpd/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-bin/utilRequest.cgi $(1)/etc/lighttpd/cgi-bin/filemanager
endef

define Package/lighttpd-fastcgi-qfile/install
	$(INSTALL_DIR) $(1)/etc/lighttpd/cgi-bin/filemanager
	mkdir -p $(1)/etc/lighttpd/cgi-bin/qid
	#$(CP) $(PKG_BUILD_DIR)/cgi-bin/authLogin.cgi $(1)/etc/lighttpd/cgi-bin
	#$(CP) $(PKG_BUILD_DIR)/cgi-bin/sysinfoReq.cgi $(1)/etc/lighttpd/cgi-bin
	#$(CP) $(PKG_BUILD_DIR)/cgi-bin/utilRequest.cgi $(1)/etc/lighttpd/cgi-bin/filemanager
	$(CP) $(PKG_BUILD_DIR)/cgi-bin/stdenv.cgi $(1)/etc/lighttpd/cgi-bin
	$(CP) $(PKG_BUILD_DIR)/cgi-bin/fcgi_http.exe $(1)/etc/lighttpd/cgi-bin
	ln -sf stdenv.cgi $(1)/etc/lighttpd/cgi-bin/sysinfoReq.cgi
	ln -sf stdenv.cgi $(1)/etc/lighttpd/cgi-bin/authLogin.cgi
	ln -sf ../stdenv.cgi $(1)/etc/lighttpd/cgi-bin/filemanager/utilRequest.cgi
	ln -sf ../stdenv.cgi $(1)/etc/lighttpd/cgi-bin/filemanager/qsyncPrepare.cgi
	touch $(1)/etc/lighttpd/cgi-bin/qid/qidRequest.cgi
endef


define BuildPlugin
  define Package/$(1)/install
	[ -z "$(2)" ] || $(INSTALL_DIR) $$(1)/usr/lib/lighttpd
	for m in $(2); do \
		$(CP) $(PKG_INSTALL_DIR)/usr/lib/lighttpd/mod_$$$$$$$${m}.so $$(1)/usr/lib/lighttpd/ ; \
	done
  endef

  $$(eval $$(call BuildPackage,$(1)))
endef

$(eval $(call BuildPackage,lighttpd))
$(eval $(call BuildPlugin,lighttpd-mod-cgi,cgi))
$(eval $(call BuildPackage,lighttpd-cgi-qfile))
$(eval $(call BuildPackage,lighttpd-fastcgi-qfile))

$(eval $(call BuildPlugin,lighttpd-mod-access,access))
$(eval $(call BuildPlugin,lighttpd-mod-accesslog,accesslog))
$(eval $(call BuildPlugin,lighttpd-mod-alias,alias))
$(eval $(call BuildPlugin,lighttpd-mod-auth,auth))
$(eval $(call BuildPlugin,lighttpd-mod-cml,cml))
$(eval $(call BuildPlugin,lighttpd-mod-compress,compress))
$(eval $(call BuildPlugin,lighttpd-mod-evasive,evasive))
$(eval $(call BuildPlugin,lighttpd-mod-evhost,evhost))
$(eval $(call BuildPlugin,lighttpd-mod-expire,expire))
$(eval $(call BuildPlugin,lighttpd-mod-extforward,extforward))
$(eval $(call BuildPlugin,lighttpd-mod-fastcgi,fastcgi))
$(eval $(call BuildPlugin,lighttpd-mod-flv-streaming,flv_streaming))
$(eval $(call BuildPlugin,lighttpd-mod-magnet,magnet))
$(eval $(call BuildPlugin,lighttpd-mod-proxy,proxy))
$(eval $(call BuildPlugin,lighttpd-mod-redirect,redirect))
$(eval $(call BuildPlugin,lighttpd-mod-rewrite,rewrite))
$(eval $(call BuildPlugin,lighttpd-mod-rrdtool,rrdtool))
$(eval $(call BuildPlugin,lighttpd-mod-scgi,scgi))
$(eval $(call BuildPlugin,lighttpd-mod-secdownload,secdownload))
$(eval $(call BuildPlugin,lighttpd-mod-setenv,setenv))
$(eval $(call BuildPlugin,lighttpd-mod-simple-vhost,simple_vhost))
$(eval $(call BuildPlugin,lighttpd-mod-ssi,ssi))
$(eval $(call BuildPlugin,lighttpd-mod-status,status))
$(eval $(call BuildPlugin,lighttpd-mod-trigger-b4-dl,trigger_b4_dl))
$(eval $(call BuildPlugin,lighttpd-mod-userdir,userdir))
$(eval $(call BuildPlugin,lighttpd-mod-usertrack,usertrack))
$(eval $(call BuildPlugin,lighttpd-mod-webdav,webdav))

