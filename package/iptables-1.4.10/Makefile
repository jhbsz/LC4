#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=iptables-1.4.10
PKG_VERSION:=
PKG_RELEASE:=5

PKG_MD5SUM:=f382fe693f0b59d87bd47bea65eca198
PKG_SOURCE:=$(PKG_NAME).tar.bz2
PKG_SOURCE_URL:=http://www.netfilter.org/projects/iptables/files \
	ftp://ftp.be.netfilter.org/pub/netfilter/iptables/ \
	ftp://ftp.de.netfilter.org/pub/netfilter/iptables/ \
	ftp://ftp.no.netfilter.org/pub/netfilter/iptables/

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

ifneq ($(CONFIG_EXTERNAL_KERNEL_TREE),"")
PATCH_DIR:=
endif

include $(INCLUDE_DIR)/package.mk
ifeq ($(DUMP),)
  -include $(LINUX_DIR)/.config
  include $(INCLUDE_DIR)/netfilter.mk
  STAMP_CONFIGURED:=$(strip $(STAMP_CONFIGURED))_$(shell $(SH_FUNC) grep 'NETFILTER' $(LINUX_DIR)/.config | md5s)
endif


define Package/iptables-1.4.10/Default
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Firewall
  URL:=http://netfilter.org/
endef

define Package/iptables-1.4.10/Module
$(call Package/iptables-1.4.10/Default)
  DEPENDS:=iptables-1.4.10 $(1)
endef

define Package/iptables-1.4.10
$(call Package/iptables-1.4.10/Default)
  TITLE:=IPv4 firewall administration tool
  MENU:=1
  DEPENDS+= +kmod-ipt-core +libip4tc +libxtables
endef

define Package/iptables-1.4.10/description
IPv4 firewall administration tool.

 Matches:
  - icmp
  - tcp
  - udp
  - comment
  - limit
  - mac
  - multiport

 Targets:
  - ACCEPT
  - DROP
  - REJECT
  - LOG
  - TCPMSS

 Tables:
  - filter
  - mangle

endef

define Package/iptables-1.4.10-mod-webstr
$(call Package/iptables-1.4.10/Module, +kmod-ipt-webstr)
        TITLE:=user-space url string matching
endef

define Package/iptables-1.4.10-mod-webstr/description
support for http header and content string filter 

endef

define Package/iptables-1.4.10-mod-conntrack-extra
$(call Package/iptables-1.4.10/Module, +kmod-ipt-conntrack-extra)
  TITLE:=Extra connection tracking extensions
endef

define Package/iptables-1.4.10-mod-conntrack-extra/description
Extra iptables-1.4.10 extensions for connection tracking.

 Matches:
  - connbytes
  - connmark
  - recent
  - helper

 Targets:
  - CONNMARK

endef

define Package/iptables-1.4.10-mod-filter
$(call Package/iptables-1.4.10/Module, +kmod-ipt-filter)
  TITLE:=Content inspection extensions
endef

define Package/iptables-1.4.10-mod-filter/description
iptables-1.4.10 extensions for packet content inspection.
Includes support for:

 Matches:
  - layer7
  - string

endef

define Package/iptables-1.4.10-mod-imq
$(call Package/iptables-1.4.10/Module, +kmod-ipt-imq)
  TITLE:=Content inspection extensions
endef

define Package/iptables-1.4.10-mod-imq/description
 iptables-1.4.10 extension for IMQ support.
 Includes:
 - libipt_IMQ
endef

define Package/iptables-1.4.10-mod-ipopt
$(call Package/iptables-1.4.10/Module, +kmod-ipt-ipopt)
  TITLE:=IP/Packet option extensions
endef

define Package/iptables-1.4.10-mod-ipopt/description
iptables-1.4.10 extensions for matching/changing IP packet options.

 Matches:
  - dscp
  - ecn
  - length
  - mark
  - statistic
  - tcpmss
  - time
  - unclean
  - hl

 Targets:
  - DSCP
  - CLASSIFY
  - ECN
  - MARK
  - HL

endef

define Package/iptables-1.4.10-mod-ipsec
$(call Package/iptables-1.4.10/Module, +kmod-ipt-ipsec)
  TITLE:=IPsec extensions
endef

define Package/iptables-1.4.10-mod-ipsec/description
iptables-1.4.10 extensions for matching ipsec traffic.

 Matches:
  - ah
  - esp
  - policy

endef

define Package/iptables-1.4.10-mod-ipset
$(call Package/iptables-1.4.10/Module,)
  TITLE:=IPset iptables-1.4.10 extensions
endef

define Package/iptables-1.4.10-mod-ipset/description
IPset iptables-1.4.10 extensions.

 Matches:
  - set

 Targets:
  - SET

endef

define Package/iptables-1.4.10-mod-nat-extra
$(call Package/iptables-1.4.10/Module, +kmod-ipt-nat-extra)
  TITLE:=Extra NAT extensions
endef

define Package/iptables-1.4.10-mod-nat-extra/description
iptables-1.4.10 extensions for extra NAT targets.

 Targets:
  - MIRROR
  - NETMAP
  - REDIRECT
endef

define Package/iptables-1.4.10-mod-ulog
$(call Package/iptables-1.4.10/Module, +kmod-ipt-ulog)
  TITLE:=user-space packet logging
endef

define Package/iptables-1.4.10-mod-ulog/description
iptables-1.4.10 extensions for user-space packet logging.

 Targets:
  - ULOG

endef

define Package/iptables-1.4.10-mod-hashlimit
$(call Package/iptables-1.4.10/Module, +kmod-ipt-hashlimit)
  TITLE:=hashlimit matching
endef

define Package/iptables-1.4.10-mod-hashlimit/description
iptables-1.4.10 extensions for hashlimit matching

 Matches:
  - hashlimit

endef

define Package/iptables-1.4.10-mod-iprange
$(call Package/iptables-1.4.10/Module, +kmod-ipt-iprange)
  TITLE:=IP range extension
endef

define Package/iptables-1.4.10-mod-iprange/description
iptables-1.4.10 extensions for matching ip ranges.

 Matches:
  - iprange

endef

define Package/iptables-1.4.10-mod-extra
$(call Package/iptables-1.4.10/Module, +kmod-ipt-extra)
  TITLE:=Other extra iptables-1.4.10 extensions
endef

define Package/iptables-1.4.10-mod-extra/description
Other extra iptables-1.4.10 extensions.

 Matches:
  - condition
  - owner
  - physdev (if ebtables is enabled)
  - pkttype
  - quota

endef

define Package/iptables-1.4.10-mod-led
$(call Package/iptables-1.4.10/Module, +kmod-ipt-led)
  TITLE:=LED trigger iptables-1.4.10 extension
endef

define Package/iptables-1.4.10-mod-led/description
iptables-1.4.10 extension for triggering a LED.

 Targets:
  - LED

endef

define Package/iptables-1.4.10-mod-tproxy
$(call Package/iptables-1.4.10/Module, +kmod-ipt-tproxy)
  TITLE:=Transparent proxy iptables-1.4.10 extensions
endef

define Package/iptables-1.4.10-mod-tproxy/description
Transparent proxy iptables-1.4.10 extensions.

 Matches:
  - socket

 Targets:
  - TPROXY

endef

define Package/iptables-1.4.10-mod-tee
$(call Package/iptables-1.4.10/Module, +kmod-ipt-tee)
  TITLE:=TEE iptables-1.4.10 extensions
endef

define Package/iptables-1.4.10-mod-tee/description
TEE iptables-1.4.10 extensions.

 Targets:
  - TEE

endef

define Package/iptables-1.4.10-mod-u32
$(call Package/iptables-1.4.10/Module, +kmod-ipt-u32)
  TITLE:=U32 iptables-1.4.10 extensions
endef

define Package/iptables-1.4.10-mod-u32/description
U32 iptables-1.4.10 extensions.

 Matches:
  - u32

endef

define Package/ip6tables
$(call Package/iptables-1.4.10/Default)
  DEPENDS:=+kmod-ip6tables +libip6tc +libxtables
  CATEGORY:=IPv6
  TITLE:=IPv6 firewall administration tool
  MENU:=1
endef

define Package/libiptc
$(call Package/iptables-1.4.10/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libip4tc +libip6tc
  TITLE:=IPv4/IPv6 firewall - shared libiptc library (compatibility stub)
endef

define Package/libip4tc
$(call Package/iptables-1.4.10/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=IPv4 firewall - shared libiptc library
endef

define Package/libip6tc
$(call Package/iptables-1.4.10/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=IPv6 firewall - shared libiptc library
endef

define Package/libxtables
 $(call Package/iptables-1.4.10/Default)
 SECTION:=libs
 CATEGORY:=Libraries
 TITLE:=IPv4/IPv6 firewall - shared xtables library
endef

define Package/libipq
  $(call Package/iptables-1.4.10/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=IPv4/IPv6 firewall - shared libipq library
endef

TARGET_CPPFLAGS := \
	-I$(PKG_BUILD_DIR)/include \
	-I$(LINUX_DIR)/user_headers/include \
	$(TARGET_CPPFLAGS)

TARGET_CFLAGS += \
	-I$(PKG_BUILD_DIR)/include \
	-I$(LINUX_DIR)/user_headers/include

CONFIGURE_ARGS += \
	--enable-shared \
	--enable-devel \
	$(if $(CONFIG_IPV6),--enable-ipv6,--disable-ipv6) \
	--enable-libipq \
	--with-kernel="$(LINUX_DIR)/user_headers" \
	--with-xtlibdir=/usr/lib/iptables \
	--enable-static

MAKE_FLAGS := \
	$(TARGET_CONFIGURE_OPTS) \
	COPT_FLAGS="$(TARGET_CFLAGS)" \
	KERNEL_DIR="$(LINUX_DIR)/user_headers/" PREFIX=/usr \
	KBUILD_OUTPUT="$(LINUX_DIR)" \
	BUILTIN_MODULES="$(patsubst ip6t_%,%,$(patsubst ipt_%,%,$(patsubst xt_%,%,$(IPT_BUILTIN) $(IPT_CONNTRACK-m) $(IPT_NAT-m))))"

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/include/iptables
	$(INSTALL_DIR) $(1)/usr/include/net/netfilter

	# XXX: iptables header fixup, some headers are not installed by iptables anymore
	$(CP) $(PKG_BUILD_DIR)/include/net/netfilter/*.h $(1)/usr/include/net/netfilter/
	$(CP) $(PKG_BUILD_DIR)/include/iptables/*.h $(1)/usr/include/iptables/
	$(CP) $(PKG_BUILD_DIR)/include/iptables.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/include/libipq/libipq.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/include/libipulog $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/include/libiptc $(1)/usr/include/

	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libxtables.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libip*tc.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libipq.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/xtables.pc $(1)/usr/lib/pkgconfig/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libiptc.pc $(1)/usr/lib/pkgconfig/

	# XXX: needed by firewall3
	$(INSTALL_DIR) $(1)/usr/lib/iptables
	$(CP) $(PKG_BUILD_DIR)/extensions/libext*.a $(1)/usr/lib/iptables/
endef

define Package/iptables-1.4.10/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/iptables $(1)/usr/sbin/
	ln -sf iptables $(1)/usr/sbin/iptables-save
	ln -sf iptables $(1)/usr/sbin/iptables-restore
	$(INSTALL_DIR) $(1)/usr/lib/iptables
endef

define Package/ip6tables/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/ip6tables $(1)/usr/sbin/
	ln -fs ip6tables $(1)/usr/sbin/ip6tables-save
	ln -fs ip6tables $(1)/usr/sbin/ip6tables-restore
	$(INSTALL_DIR) $(1)/usr/lib/iptables
	(cd $(PKG_INSTALL_DIR)/usr/lib/iptables ; \
		$(CP) libip6t_*.so $(1)/usr/lib/iptables/ \
	)
endef

define Package/libiptc/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libiptc.so* $(1)/usr/lib/
endef

define Package/libip4tc/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libip4tc.so* $(1)/usr/lib/
endef

define Package/libip6tc/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libip6tc.so* $(1)/usr/lib/
endef

define Package/libxtables/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libxtables.so* $(1)/usr/lib/
endef

define Package/libipq/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libipq.so* $(1)/usr/lib/
endef

define BuildPlugin
  define Package/$(1)/install
	$(INSTALL_DIR) $$(1)/usr/lib/iptables
	echo VT_$(INSTALL_DIR)_$$(1)
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/iptables/* $$(1)/usr/lib/iptables/ 
	$(3)
  endef

  $$(eval $$(call BuildPackage,$(1)))
endef

L7_INSTALL:=\
	$(INSTALL_DIR) $$(1)/etc/l7-protocols; \
	$(CP) files/l7/*.pat $$(1)/etc/l7-protocols/


$(eval $(call BuildPackage,iptables-1.4.10))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-conntrack-extra,$(IPT_CONNTRACK_EXTRA-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-extra,$(IPT_EXTRA-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-filter,$(IPT_FILTER-m),$(L7_INSTALL)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-imq,$(IPT_IMQ-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-ipopt,$(IPT_IPOPT-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-ipsec,$(IPT_IPSEC-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-ipset,ipt_set ipt_SET))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-nat-extra,$(IPT_NAT_EXTRA-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-iprange,$(IPT_IPRANGE-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-ulog,$(IPT_ULOG-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-hashlimit,$(IPT_HASHLIMIT-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-led,$(IPT_LED-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-tproxy,$(IPT_TPROXY-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-tee,$(IPT_TEE-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-u32,$(IPT_U32-m)))
$(eval $(call BuildPlugin,iptables-1.4.10-mod-webstr,$(IPT_WEBSTR-m)))
$(eval $(call BuildPackage,ip6tables))
$(eval $(call BuildPackage,libiptc))
$(eval $(call BuildPackage,libip4tc))
$(eval $(call BuildPackage,libip6tc))
$(eval $(call BuildPackage,libxtables))
$(eval $(call BuildPackage,libipq))
