/*
 * EZ Packet product default NVRAM values
 *
 * Copyright 2006, 2007 EZ Packet Technologies
 * All Rights Reserved.
 */

/*
 * Router default NVRAM values
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: defaults.c,v 1.104 2005/05/25 12:31:20 honor Exp $
 */

#ifndef _NVRAM_EZPACKET_H
#define _NVRAM_EZPACKET_H

#include <bcmnvram.h>

/* HW dependent definition. */
#ifdef CONFIG_EZP_ARCH_RT305X
#include "nvram_ezpacket-rt305x.h"
#endif
#ifdef CONFIG_EZP_ARCH_AS500
#include "nvram_ezpacket-as500.h"
#endif
#ifdef CONFIG_EZP_ARCH_BRCM
#include "nvram_ezpacket-brcm.h"
#endif
#ifdef CONFIG_EZP_ARCH_OX810
#include "nvram_ezpacket-ox810.h"
#endif
#ifdef CONFIG_EZP_ARCH_RTL865X
#include "nvram_ezpacket-rtl865x.h"
#endif /* CONFIG_EZP_ARCH_RTL865X */

#if !defined(EZP_PROD_SUPERCAT_X) && !defined(NVRAM_EZPACKET_APP_H)
#include "nvram_ezpacket-app.h"
#endif

#if defined(EZP_PROD_SUPERCAT_X)
    #if defined(EZP_PROD_CAT_P)
    #include "P/nvram_ezpacket_P.h"
    #include "P/nvram_ezpacket-app_P.h"
    #endif
    #if defined(EZP_PROD_CAT_K)
    #include "K/nvram_ezpacket_K.h"
    #include "K/nvram_ezpacket-app_K.h"
    #endif
    #if defined(EZP_PROD_CAT_G)
    #include "G/nvram_ezpacket_G.h"
    #include "G/nvram_ezpacket-app_G.h"
    #endif
    #if defined(EZP_PROD_CAT_I)
    #include "I/nvram_ezpacket_I.h"
    #include "I/nvram_ezpacket-app_I.h"
    #endif
#endif /*defined(EZP_PROD_SUPERCAT_X)*/

/* open bcclinet wan socket */
#if defined (PLATFORM_AXA) || defined (PLATFORM_M2E)
#define BCCLINET_WAN 1
#else
#define BCCLINET_WAN 0
#endif

/* LAN amount dependent definition. */
#if ( LAN_NUM == 1 )
#define LAN_MAIN_RULE "LAN1^1^1500^0^1"
#if defined(EZP_PROD_BRAND_ZYUS)
#define LAN_IPADDR_MASK "192.168.10." 
#elif defined(EZP_PROD_BRAND_PLANET)
#define LAN_IPADDR_MASK "192.168.0." 
#elif defined(EZP_SUB_BRAND_GARMIN)
#define LAN_IPADDR_MASK "10.199.221." 
#elif defined(EZP_SUB_BRAND_SONY)
#define LAN_IPADDR_MASK "192.168.40." 
#elif defined(PLATFORM_AXM)
#define LAN_IPADDR_MASK "192.168.50." 
#elif defined(PLATFORM_M2E)  || defined(PLATFORM_AXA) || defined(PLATFORM_ALS)
#define LAN_IPADDR_MASK "192.168.60." 
#else
#define LAN_IPADDR_MASK "192.168.1." 
#endif /* EZP_PROD_BRAND_ZYUS */
#if defined(EZP_SUB_BRAND_APOLLO)
#define LAN_STATIC_RULE_DEFAULT LAN_IPADDR_MASK"254^24^^"LAN_IPADDR_MASK"254^"
#elif defined(EZP_SUB_BRAND_GARMIN)
#define LAN_STATIC_RULE_DEFAULT LAN_IPADDR_MASK"254^25^^"LAN_IPADDR_MASK"254^"
#else
#define LAN_STATIC_RULE_DEFAULT LAN_IPADDR_MASK"1^24^^"LAN_IPADDR_MASK"1^"
#endif
#define GUEST_LAN_RULE "0^192.168.2.1^24"
#define LAN_HWADDR_CLONE_RULE "0^"
#define LAN_STATUS_RULE "0^0^0^none^0"
#else
#error "Undefined amount of LAN interface(s)"
#endif

#define WIZARD_RULE "1^1^0^1^1^0^1^0"
#define WL_COUNTRY_CODE "TW"
#define WL_REGION_ABAND "7"

#if ( EZP_PROD_SUBCAT == 0 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^K|3G/4G^P"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#define FL_HWADDR_RULE_MAX "64"

#if defined(EZP_PROD_CAT_P) || defined(EZP_PROD_CAT_K) || defined(EZP_PROD_CAT_U) || defined(EZP_PROD_CAT_W) || defined(EZP_PROD_CAT_R)
    #define CT_MAX "10000"
#else
    #define CT_MAX "20000"
#endif

#define RT_RULE_MAX "32"

#ifdef EZP_PROD_BRAND_PLANET
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "16"
#else
#define BW_SBM_RULE_MAX "0"
#endif
#define BW_DBM_RULE_MAX "0"
#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define WF_RULE_MAX "16"

#ifdef EZP_PROD_BRAND_PLANET
#define SBM_MAX "0"
#define DBM_MAX "1"
#else
#define DBM_MAX "0"
#endif

#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#define VS_RULE_MAX "32"
#define BW_ENABLE "0"


#ifdef EZP_PROD_BRAND_PLANET
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^^disabled^disabled^1"
#else
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#endif

#define BW_DBM_RULE_NUM "0"
#define BW_DBM_RULE ""
#define LAN_DHCPS_RULE "default^1^20^8^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#define VNC_SESSION_MAX "16"
#define BT_TORRENT_MAX "10"
#define FTPD_USER_RULE_MAX "4"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "4"
#define ISHOST_RULE_MAX "4"

/* VPN comes in. Note: No VPN in this subcat.*/
#if ( WAN_NUM == 1 )

#if defined(EZP_PROD_CAT_O)
#define PPTPD_SESSION_MAX 10	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "10"
#define IPSEC_RULE_MAX "25"
#else
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#endif /* defined(EZP_PROD_CAT_O) */

#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"

#ifdef LAN_AUTOIP_SWITCH
    #define IP_SWITCH_RULE_NUM  "2"
    #define IP_SWITCH_RULE  "192.168.100.1|"LAN_IPADDR_MASK"1"
#endif

#elif ( WAN_NUM == 2 )

#if defined(EZP_PROD_CAT_O)
#define PPTPD_SESSION_MAX 10	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "10"
#define IPSEC_RULE_MAX "25"
#else
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#endif /* defined(EZP_PROD_CAT_O) */
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"

#ifdef LAN_AUTOIP_SWITCH
    #define IP_SWITCH_RULE_NUM  "3"
    #define IP_SWITCH_RULE  "192.168.100.1|192.168.200.1|"LAN_IPADDR_MASK"1"
#endif

#elif ( WAN_NUM == 3 )

#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"

#ifdef LAN_AUTOIP_SWITCH
    #define IP_SWITCH_RULE_NUM  "4"
    #define IP_SWITCH_RULE  "192.168.100.1|192.168.200.1|192.168.150.1|"LAN_IPADDR_MASK"1"
#endif

#define IPSEC_RULE_MAX "0"

#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 1 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^G|3G/4G^I"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#define FL_HWADDR_RULE_MAX "64"

#if defined(EZP_PROD_CAT_P) || defined(EZP_PROD_CAT_W) || defined(EZP_PROD_CAT_K) || defined(EZP_PROD_CAT_R)
    #define CT_MAX "10000"
#else
    #define CT_MAX "20000"
#endif

#define RT_RULE_MAX "32"
#ifdef EZP_PROD_BRAND_PLANET
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^^disabled^disabled^1"
#else
#define BW_SBM_RULE_MAX "0"
#define BW_DBM_RULE_MAX "0"
#endif
#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define WF_RULE_MAX "16"

#ifdef EZP_PROD_BRAND_PLANET
#define SBM_MAX "8"
#endif
#define DBM_MAX "8"
#ifdef PLATFORM_ALS
#define FL_RULE_NUM "0"
#define FL_RULE ""
#else
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#endif
#define VS_RULE_MAX "32"

#if defined (PLATFORM_AXM)
#define BW_ENABLE "1"
#else
#define BW_ENABLE "0"
#endif
#ifdef EZP_PROD_BRAND_PLANET
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^^disabled^disabled^1"
#else
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#endif
#define BW_DBM_RULE_NUM "0"
#define BW_DBM_RULE ""

#if defined (EZP_PROD_BRAND_SONY)
#define LAN_DHCPS_RULE "default^1^20^10^43200^lan^^dnsrelay^^^^"
#elif defined (EZP_SUB_BRAND_GARMIN)
#define LAN_DHCPS_RULE "default^1^128^100^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#elif defined (PLATFORM_AXA) || defined (PLATFORM_AXM) || defined (PLATFORM_M2E) || defined (PLATFORM_ALS)
#define LAN_DHCPS_RULE "default^1^20^50^120^lan^^dnsrelay^^^^"
#else
#define LAN_DHCPS_RULE "default^1^20^8^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#endif
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "10"
#define FTPD_USER_RULE_MAX "4"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "4"
#define ISHOST_RULE_MAX "4"

/* VPN comes in. Note: No VPN in this subcat.*/
#if ( WAN_NUM == 1 ) /* EZP_PROD_CAT: "B" */
#ifdef PLATFORM_ALS
#define PPTPD_USER_RULE_MAX "10"
#define L2TPD_USER_RULE_MAX "10"
#define PPTPD_SESSION_MAX 10   /* Stringification later!*/
#define L2TPD_SESSION_MAX 10   /* Stringification later!*/
#define IPSEC_RULE_MAX "10"
#else
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#endif

#ifdef LAN_AUTOIP_SWITCH
    #define IP_SWITCH_RULE_NUM  "2"
  #ifdef EZP_SUB_BRAND_GARMIN
    #define IP_SWITCH_RULE  "10.199.237.254|"LAN_IPADDR_MASK"254"
  #else
    #define IP_SWITCH_RULE  "192.168.100.1|"LAN_IPADDR_MASK"1"
  #endif
#endif

#elif ( WAN_NUM == 2 ) /* EZP_PROD_CAT: "L" */
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 2 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^G|3G/4G^I"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#define FL_HWADDR_RULE_MAX "64"

/* PGP consumes too much memory and leaves less for conntrack. */
#if defined(EZP_PROD_CAT_C) || defined (EZP_PROD_CAT_T) || defined (EZP_PROD_CAT_D) || defined (EZP_PROD_CAT_K)
#define CT_MAX "20000"
#else
#define CT_MAX "40000"
#endif

#define RT_RULE_MAX "32"
#ifdef EZP_PROD_BRAND_PLANET
#define BW_RULE_MAX "32"
#endif

#if defined(EZP_PROD_SUBSUBCAT_L)
#define BW_DBM_RULE_MAX "0"
#define BW_SBM_RULE_MAX "64"
#ifdef EZP_PROD_BRAND_PLANET
#define SBM_MAX "64"
#endif
#define DBM_MAX "1"
#define BW_DBM_RULE_NUM "0"
#define BW_DBM_RULE "DBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"27^8^"
#else
#define BW_DBM_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#ifdef EZP_PROD_BRAND_PLANET
#define SBM_MAX "8"
#define DBM_MAX "8"
#else
#define DBM_MAX "32"
#endif
#define BW_DBM_RULE_NUM "1"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"27^8^"
#endif

#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define FL_RULE_NUM "3"

#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#define VS_RULE_MAX "32"
#define BW_ENABLE "0"
#define BW_SBM_RULE_NUM "1"
#ifdef EZP_PROD_BRAND_PLANET
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^^disabled^disabled^1"
#else
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^"
#endif
#define LAN_DHCPS_RULE "default^1^20^8^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "10"
#define FTPD_USER_RULE_MAX "8"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "8"
#define ISHOST_RULE_MAX "8"

/* VPN comes in. Note: No VPN in this subcat except MR-108N and X-108NX.*/
#if ( WAN_NUM == 1 ) 
#if defined(EZP_PROD_CAT_M)
#define PPTPD_SESSION_MAX 15	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "15"
#define IPSEC_RULE_MAX "30"
#elif defined(EZP_PROD_CAT_G) || defined(EZP_PROD_CAT_I)
#define PPTPD_SESSION_MAX 4	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "4"
#define IPSEC_RULE_MAX "0"
#else
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#endif
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"

#elif ( WAN_NUM == 2 ) /* EZP_PROD_CAT: "L" */
#define PPTPD_SESSION_MAX 0	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "0"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"
#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 3 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^G|3G/4G^I"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "50000"
#define RT_RULE_MAX "32"

#ifdef EZP_PROD_BRAND_PLANET
#define BW_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#else
#define BW_SBM_RULE_MAX "64"
#define BW_DBM_RULE_MAX "64"
#endif

#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define WF_RULE_MAX "16"

#ifdef EZP_PROD_BRAND_PLANET
#define SBM_MAX "16"
#define DBM_MAX "16"
#else
#define DBM_MAX "64"
#endif

#define VS_RULE_MAX "32"
#if defined (EZP_SUB_BRAND_APOLLO)
#define BW_ENABLE "1"
#else
#define BW_ENABLE "0"
#endif

#if defined (EZP_PROD_BRAND_PLANET)
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^^disabled^disabled^1"
#elif defined (EZP_SUB_BRAND_APOLLO)
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "DVR_PRIORITY^1^"LAN_IPADDR_MASK".129^"LAN_IPADDR_MASK".129^1^^^^wan1^ratio^75^^^^1^100^25600^25600"
#elif defined (EZP_PROD_BRAND_PROX)
#define BW_SBM_RULE_NUM "0"
#define BW_SBM_RULE ""
#else
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^"
#endif

#if defined (EZP_PROD_BRAND_PROX)
#define FL_RULE_NUM "0"
#define FL_RULE ""
#if defined (EZP_SUB_BRAND_APOLLO)
#define LAN_DHCPS_RULE "WAN1^1^50^50^3600^APOLLO^^dnsrelay^208.67.222.222^208.67.220.220^^"
#else
#define LAN_DHCPS_RULE "default^1^33^32^86400^LAN-Cell^^dnsrelay^208.67.222.222^208.67.220.220^^"
#endif
#define BW_DBM_RULE_NUM "0"
#define BW_DBM_RULE ""
#else
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#define LAN_DHCPS_RULE "default^1^20^8^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#define BW_DBM_RULE_NUM "1"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"35^16^"
#endif

#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "16"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "16"
#define ISHOST_RULE_MAX "16"

/* VPN comes in. */
#if ( WAN_NUM == 1 ) /* EZP_PROD_CAT: "B" "C", subcat = 3, x116 series */
#define PPTPD_SESSION_MAX 16	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "16"
#define L2TPD_SESSION_MAX 0	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "0"
#define IPSEC_RULE_MAX "0"

#elif ( WAN_NUM == 2 ) /* EZP_PROD_CAT: "L" */
#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
#ifdef EZP_PROD_CAT_I
#define IPSEC_RULE_MAX "32"
#else
#define IPSEC_RULE_MAX "0"
#endif
#else
#error "Undefined amount of LAN/WAN interfaces"
#endif /* VPN */

#elif ( EZP_PROD_SUBCAT == 4 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^G|3G/4G^I"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#ifdef EZP_PROD_BRAND_PLANET
#define FL_HWADDR_RULE_MAX "128"
#define RT_RULE_MAX "32"
#define BW_RULE_MAX "32"
#endif

#if defined(EZP_PROD_BRAND_ZYUS)
#define CT_MAX "60000"
#define FL_HWADDR_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define WF_RULE_MAX "64"
#define DBM_MAX "16"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"35^16^"
#define LAN_DHCPS_RULE "default^1^20^16^86400^lan^^ispdns^^^^"
#define PPTPD_SESSION_MAX 16	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "16"
#define IPSEC_RULE_MAX "32"
#elif defined(EZP_PROD_BRAND_PLANET)
#define CT_MAX "20000"
#define FL_HWADDR_RULE_MAX "128"
#define BW_SBM_RULE_MAX "16"
#define BW_DBM_RULE_MAX "0"
#define SBM_MAX "0"
#define WF_RULE_MAX "16"
#define DBM_MAX "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^^disabled^disabled^1"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"35^16^"
#define LAN_DHCPS_RULE "default^1^20^16^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#define PPTPD_SESSION_MAX 10	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "10"
#define IPSEC_RULE_MAX "25"
#else
#define CT_MAX "60000"
#define FL_HWADDR_RULE_MAX "128"
#define BW_SBM_RULE_MAX "64"
#define BW_DBM_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define DBM_MAX "64"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"35^16^"
#define LAN_DHCPS_RULE "default^1^20^16^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "32"
#endif 

#define RT_RULE_MAX "32"
#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#define VS_RULE_MAX "32"
#define BW_ENABLE "0"
#define BW_SBM_RULE_NUM "1"
#define BW_DBM_RULE_NUM "1"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "32"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "32"
#define ISHOST_RULE_MAX "32"

#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"

#elif ( EZP_PROD_SUBCAT == 5 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^G|3G/4G^I"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "80000"
#define RT_RULE_MAX "32"
#define BW_SBM_RULE_MAX "32"
#define BW_DBM_RULE_MAX "32"
#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define DBM_MAX "32"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#define VS_RULE_MAX "32"
#define BW_ENABLE "0"
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^"
#define BW_DBM_RULE_NUM "1"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"51^32^"
#define LAN_DHCPS_RULE "default^1^20^32^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "32"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "32"
#define ISHOST_RULE_MAX "32"

#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "32"

#elif ( EZP_PROD_SUBCAT == 6 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^G|3G/4G^I"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "100000"
#define RT_RULE_MAX "32"
#define BW_SBM_RULE_MAX "64"
#define BW_DBM_RULE_MAX "64"
#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define DBM_MAX "64"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#define VS_RULE_MAX "32"
#define BW_ENABLE "0"
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^"
#define BW_DBM_RULE_NUM "1"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"83^64^"
#define LAN_DHCPS_RULE "default^1^20^64^86400^lan^^opendns^208.67.222.222^208.67.220.220^^"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "32"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "32"
#define ISHOST_RULE_MAX "32"

#define PPTPD_SESSION_MAX 32	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "32"
#define L2TPD_SESSION_MAX 32	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "32"
#define IPSEC_RULE_MAX "32"

#elif ( EZP_PROD_SUBCAT == 7 )

#ifdef EZP_PROD_SUPERCAT_X
#define X_MODE_DATA "Storage^G|3G/4G^I"
#else
#define X_MODE_DATA ""
#endif /* EZP_PROD_SUPERCAT_X */

#define FL_HWADDR_RULE_MAX "64"
#define CT_MAX "120000"
#define RT_RULE_MAX "32"
#define BW_SBM_RULE_MAX "128"
#define BW_DBM_RULE_MAX "128"
#define SCHED_RULE_MAX "16"
#define FL_RULE_MAX "64"
#define FR_RULE_MAX "64"
#define WF_RULE_MAX "16"
#define DBM_MAX "128"
#define FL_RULE_NUM "3"
#define FL_RULE "MSN Messenger^0^^^^^207.46.1.1^207.46.255.254^^^^^^0^0001^0^0|MSN Messenger^0^^^^^65.54.1.1^65.54.255.254^^^^^^0^0002^1^1|Yahoo! Messenger^0^^^^^216.155.1.1^216.155.255.254^^^^^^0^0003^2^2"
#define VS_RULE_MAX "32"
#define BW_ENABLE "0"
#define BW_SBM_RULE_NUM "1"
#define BW_SBM_RULE "SBM^0^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"20^1^^^^wan0^ratio^20^^^^0^100^^"
#define BW_DBM_RULE_NUM "1"
#define BW_DBM_RULE "DBM^1^"LAN_IPADDR_MASK"20^"LAN_IPADDR_MASK"83^64^"
#define LAN_DHCPS_RULE "default^1^20^64^86400^lan^^opendns^208.67.220.220^208.67.222.222^^"
#define VNC_SESSION_MAX "16"

#define BT_TORRENT_MAX "15"
#define FTPD_USER_RULE_MAX "64"
#define IS_RULE "0^^AXIMCom^iscsi_disc^5^8192^8192"
#define ISUSER_RULE_MAX "64"
#define ISHOST_RULE_MAX "64"


#define PPTPD_SESSION_MAX 64	/* Stringification later!*/
#define PPTPD_USER_RULE_MAX "64"
#define L2TPD_SESSION_MAX 64	/* Stringification later!*/
#define L2TPD_USER_RULE_MAX "64"
#define IPSEC_RULE_MAX "32"

#else

#error "Undefined product subcategory"

#endif /* EZP_PROD_SUBCAT */

#define SUBCAT_MAX  7

#define WAN_WWAN_MODE "auto", "hsdpa", "umts", "edge", "gprs"

/* WAN amount dependent definition. */
#if ( WAN_NUM == 1 )

#if !defined(EZP_PROD_SUPERCAT_X)
    #if defined(PLATFORM_AXM) || defined(PLATFORM_LIS)
    #define WAN0_PROTO "wisp"
    #elif defined(PLATFORM_AXA) || defined (PLATFORM_M2E) || defined (PLATFORM_ALS)
    #define WAN0_PROTO "dhcp"
    #elif (APPMGR_WWAN == 1)
    #define WAN0_PROTO "wwan"
    #else
    #define WAN0_PROTO "dhcp"
    #endif
#endif /*!defined(EZP_PROD_SUPERCAT_X)*/

#define WAN_MAIN_RULE "WAN1^1^1500^0^10"
#if defined (EZP_SUB_BRAND_GARMIN)
#define WAN_DETECT_RULE "1^4^5^ping^gateway^8.8.8.8"
#elif defined (PLATFORM_AXA) || defined (PLATFORM_M2E) 
#define WAN_DETECT_RULE "1^4^5^ping^gateway^8.8.8.8"
#elif defined (PLATFORM_ALS)
#define WAN_DETECT_RULE "0^4^5^ping^custom^8.8.8.8"
#else
#define WAN_DETECT_RULE "0^4^30^ping^gateway^"
#endif
#define WAN_LB_RULE "alwayson^1^^none^1"
#if defined (EZP_PROD_BRAND_PROX)    
#define WAN_STATUS_RULE "0^0^none^0^0"
#elif defined (EZP_SUB_BRAND_GARMIN)
#define WAN_STATUS_RULE "0^0^none^0^0^0"
#elif defined (PLATFORM_AXA) || defined (PLATFORM_M2E) || defined (PLATFORM_ALS)
#define WAN_STATUS_RULE "0^0^none^0^0"
#else
#define WAN_STATUS_RULE "0^0^none^0"
#endif
#define MAC_SENSE_RULE "^^^30^5^0^"
#define VPN_STATUS_RULE "0^0^none^0"
#define WAN_UPNP_STATUS_RULE "0^none^"
#define WAN_BW_RULE "2M2M^2048^2048^0^0^100^95^95^2048^256^2048^256"
#define WAN_STATIC_RULE "10.1.1.25^24^10.1.1.254^10.1.1.254^^"
#define WAN_DHCP_RULE "^^86400"
#define WAN_HTC_RULE "^^86400^512^0"
#define WAN_IPHONE_RULE "^^86400^1500^0"
#define WAN_BECEEM_RULE "g1^^"
#define WAN_WIMAX_RULE "1386^0"
#define WAN_PPPOE_RULE "chap^^^0^300^20^20^1492^1492"
#if defined (PLATFORM_ALS) 
#define WAN_WWAN_RULE "0^0^3^^^auto^chap^^^0^300^20^20^1492^1492^#777^40^0^2^1^1^ttyUSB^1^0"
#define RELAY_RULE "0^0|0^0"
#define ALERT_RULE "0^0^0^0^0"
#define FAILOVER_RULE "0^0^0"
#define MAIL_RULE "0^0^0^0^0^0^0^^^^^^^^"
#define RELAY_SCHEDULE_RULE "^^^^^^|^^^^^^"
#define FILE_MODE_RULE "1^1^1^0^0^0"
#else
#define WAN_WWAN_RULE "2^0^0^^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0"
#endif
#define WAN_BARRY_RULE "^1^0^0^^^^*99***1#^0"
#if defined(PLATFORM_M2E)
#define WAN_PPTP_L2TP_RULE_NUM "6"
#define WAN_PPTP_L2TP_RULE_MAX "6"
#define WAN_PPTP_L2TP_RULE "0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap"
#else
#define WAN_PPTP_L2TP_RULE "0^^pptp^^static^^^^^^1^300^isp^^^chap"
#endif

#define WAN_WWAN_PROBE_RULE "^^^^^^^^^^^^^^^"
#define WAN_BIGPOND_RULE "0^61.9.192.13^^"
#define WAN_HWADDR_CLONE_RULE "0^"
#define WAN_DDNS_RULE "WAN1_DDNS^0^dyndns^^^^"
#define WAN_OPENDNS_RULE "WAN1_OPENDNS^0^^^0^"
#define WAN_APPLE_FILTER_RULE "airport.us^118.214.253.15|apple.com^118.214.253.15|itools.info^118.214.253.15|thinkdifferent.us^118.214.253.15|appleiphonecell.com^118.214.253.15"
#define RT_RULE "SMTP^0^^^^^tcp^^^25^25^wan0^default^"
#if defined (PLATFORM_ALS) 
#define FR_RULE ""
#else
#define FR_RULE "HTTP^0^wan0^tcp^80^80^"LAN_IPADDR_MASK"20^^|HTTPS^0^wan0^tcp^443^443^"LAN_IPADDR_MASK"20^^|POP3^0^wan0^tcp^110^110^"LAN_IPADDR_MASK"20^^|POP3S^0^wan0^tcp^995^995^"LAN_IPADDR_MASK"20^^|SMTP^0^wan0^tcp^25^25^"LAN_IPADDR_MASK"20^^|SMTPS^0^wan0^tcp^465^465^"LAN_IPADDR_MASK"20^^|SSH^0^wan0^tcp^22^22^"LAN_IPADDR_MASK"21^^|eMule^0^wan0^both^4662^4672^"LAN_IPADDR_MASK"21^^"
#endif


#define FR_DMZ_RULE "^0^"

#elif ( WAN_NUM == 2 )

#if !defined(EZP_PROD_SUPERCAT_X)
    #if (APPMGR_WWAN == 1)
    #define WAN0_PROTO "wwan"
    #else
#ifdef EZP_SUB_BRAND_APOLLO
    #define WAN0_PROTO "static"
#else
    #define WAN0_PROTO "dhcp"
#endif
    #endif
#endif /*!defined(EZP_PROD_SUPERCAT_X)*/

#define WAN_MAIN_RULE "WAN1^1^1500^0^10|WAN2^1^1500^0^10"

#if defined (EZP_PROD_CAT_N) || defined (EZP_PROD_CAT_A)  || defined (EZP_PROD_CAT_I)
#define WAN1_PROTO "wwan"
#else
#define WAN1_PROTO "dhcp"
#endif /* EZP_PROD_CAT_N  EZP_PROD_CAT_A  EZP_PROD_CAT_I */

#if defined (EZP_PROD_BRAND_ZYUS)
#define WAN_BW_RULE "^1000^100^0^0^75^90^90^60^3^607^37|^1000^100^0^0^75^90^90^60^3^607^37"
#ifdef EZP_SUB_BRAND_FEENEY
#define WAN_WWAN_RULE "0^12^0^isp.cingular^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^^1^0|0^12^5^^^auto^chap^^^0^300^20^20^1492^1492^#777^30^5^3^0^2^ttyUSB^1^0"
#else
#define WAN_WWAN_RULE "0^12^0^isp.cingular^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^^1^0|0^12^5^^^auto^chap^^^0^300^20^20^1492^1492^#777^0^0^0^0^0^^1^0"
#endif
#define FR_RULE "HTTP^0^wan0^tcp^80^80^"LAN_IPADDR_MASK"20^^|HTTPS^0^wan0^tcp^443^443^"LAN_IPADDR_MASK"20^^|POP3^0^wan0^tcp^110^110^"LAN_IPADDR_MASK"20^^|POP3S^0^wan0^tcp^995^995^"LAN_IPADDR_MASK"20^^|SMTP^0^wan0^tcp^25^25^"LAN_IPADDR_MASK"20^^|SMTPS^0^wan0^tcp^465^465^"LAN_IPADDR_MASK"20^^|SSH^0^wan0^tcp^22^22^"LAN_IPADDR_MASK"21^^|eMule^0^wan0^both^4662^4672^"LAN_IPADDR_MASK"21^^"
#elif defined (EZP_PROD_BRAND_PROX)
#if defined (EZP_SUB_BRAND_APOLLO)
#define WAN_BW_RULE "100M100M^102400^102400^0^0^90^95^95^92160^1440^92160^1440|25M25M^25600^25600^0^0^95^95^95^24320^380^24320^380"
#define FR_RULE "DVR_WATCH_CELL^1^wan1^tcp^8016^8016^192.168.1.129^8016^8016|DVR_AUDIO_CELL^1^wan1^tcp^8116^8116^192.168.1.129^8116^8116|DVR_ADMIN_CELL^1^wan1^tcp^8200^8200^192.168.1.129^8200^8200|DVR_SEARCH_CELL^1^wan1^tcp^10019^10019^192.168.1.129^10019^10019|DVR_WATCH_WAN^1^wan0^tcp^8016^8016^192.168.1.129^8016^8016|DVR_AUDIO_WAN^1^wan0^tcp^8116^8116^192.168.1.129^8116^8116|DVR_ADMIN_WAN^1^wan0^tcp^8200^8200^192.168.1.129^8200^8200|DVR_SEARCH_WAN^1^wan0^tcp^10019^10019^192.168.1.129^10019^10019"
#else
#define WAN_BW_RULE "2M256K^2048^256^0^0^90^95^95^155^19^1244^155|2M256K^2048^256^0^0^90^95^95^155^19^1244^155"
#define FR_RULE ""
#endif
#if defined (EZP_SUB_BRAND_APOLLO)
#define WAN_WWAN_RULE "2^0^0^^^auto^chap^^^0^300^5^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0|2^0^2^Internet^^auto^chap^^^0^300^5^20^1492^1492^*99***1#^0^0^0^0^0^^1^0"
#else
#define WAN_WWAN_RULE "2^0^0^^^auto^chap^^^0^300^5^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0|2^0^0^^^auto^chap^^^0^300^5^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0"
#endif
#else
#define WAN_BW_RULE "2M256K^2048^256^0^0^90^95^95^155^19^1244^155|2M256K^2048^256^0^0^90^95^95^155^19^1244^155"
#define WAN_WWAN_RULE "2^0^0^^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0|2^0^0^^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0"
#define FR_RULE "HTTP^0^wan0^tcp^80^80^"LAN_IPADDR_MASK"20^^|HTTPS^0^wan0^tcp^443^443^"LAN_IPADDR_MASK"20^^|POP3^0^wan0^tcp^110^110^"LAN_IPADDR_MASK"20^^|POP3S^0^wan0^tcp^995^995^"LAN_IPADDR_MASK"20^^|SMTP^0^wan0^tcp^25^25^"LAN_IPADDR_MASK"20^^|SMTPS^0^wan0^tcp^465^465^"LAN_IPADDR_MASK"20^^|SSH^0^wan0^tcp^22^22^"LAN_IPADDR_MASK"21^^|eMule^0^wan0^both^4662^4672^"LAN_IPADDR_MASK"21^^"
#endif /* EZP_PROD_BRAND_ZYUS */


#ifdef EZP_PROD_BRAND_ZYUS
#define WAN_LB_RULE "alwayson^1^^wan1^1|backup^1^^none^1"
#else
#define WAN_LB_RULE "alwayson^1^^none^1|alwayson^1^^none^1"
#endif

#if defined (EZP_PROD_BRAND_PROX)    
#define WAN_STATUS_RULE "0^0^none^0^0|0^0^none^0^0"
#else
#define WAN_STATUS_RULE "0^0^none^0|0^0^none^0"
#endif
#define MAC_SENSE_RULE "^^^30^5^0^"
#ifdef defined (PLATFORM_ALS)
#define RELAY_RULE "0^0|0^0"
#define ALERT_RULE "0^0^0^0^0"
#define FAILOVER_RULE "0^0^0"
#define MAIL_RULE "0^0^0^0^0^0^0^^^^^^^^"
#define RELAY_SCHEDULE_RULE "^^^^^^|^^^^^^"
#define FILE_MODE_RULE "1^1^1^0^0^0"
#endif
#define VPN_STATUS_RULE "0^0^none^0|0^0^none^0"
#define WAN_UPNP_STATUS_RULE "0^none^"
#define WAN_DHCP_RULE "^^86400|^^86400"
#define WAN_HTC_RULE "^^86400^512^0|^^86400^512^0"
#define WAN_IPHONE_RULE "^^86400^1500^0|^^86400^1500^0"
#define WAN_BECEEM_RULE "g1^^|g1^^"
#define WAN_WIMAX_RULE "1386^0|1386^0"
#if defined (EZP_PROD_BRAND_PROX)
#ifdef EZP_SUB_BRAND_APOLLO
#define WAN_DETECT_RULE "1^4^5^ping^gateway^8.8.8.8|1^4^5^ping^custom^8.8.8.8"
#define WAN_STATIC_RULE "172.22.0.254^16^172.22.0.1^172.22.1.10^^^|^^^^^"
#else
#define WAN_DETECT_RULE "1^4^5^ping^custom^8.8.8.8|1^4^5^ping^custom^8.8.8.8"
#define WAN_STATIC_RULE "^^^^^|^^^^^"
#endif
#define WAN_PPPOE_RULE "chap^^^0^300^5^20^1492^1492|chap^^^0^300^5^20^1492^1492"
#else
#define WAN_DETECT_RULE "0^4^30^ping^gateway^|0^4^30^ping^gateway^"
#define WAN_STATIC_RULE "10.1.1.25^24^10.1.1.254^10.1.1.254^^|10.1.2.25^24^10.1.2.254^10.1.2.254^^"
#define WAN_PPPOE_RULE "chap^^^0^300^20^20^1492^1492|chap^^^0^300^20^20^1492^1492"
#endif
#define WAN_BARRY_RULE "^1^0^0^^^^*99***1#^0|^1^0^0^^^^*99***1#^0"
#define WAN_PPTP_L2TP_RULE "0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap"
#define WAN_WWAN_PROBE_RULE "^^^^^^^^^^^^^^^|^^^^^^^^^^^^^^^"
#define WAN_BIGPOND_RULE "0^61.9.192.13^^|0^61.9.192.13^^"
#define WAN_HWADDR_CLONE_RULE "0^|0^"
#define WAN_DDNS_RULE "WAN1_DDNS^0^dyndns^^^^|WAN2_DDNS^0^dyndns^^^^"
#define WAN_OPENDNS_RULE "WAN1_OPENDNS^0^^^0^|WAN2_OPENDNS^0^^^0^"
#define WAN_APPLE_FILTER_RULE "airport.us^118.214.253.15|apple.com^118.214.253.15|itools.info^118.214.253.15|thinkdifferent.us^118.214.253.15|appleiphonecell.com^118.214.253.15"

#if defined (EZP_PROD_BRAND_PROX)
#define RT_RULE ""
#else
#define RT_RULE "SMTP^0^^^^^tcp^^^25^25^wan0^default^"
#endif 

#define FR_DMZ_RULE "^0^|^0^"

#elif ( WAN_NUM == 3 )

#define WAN_MAIN_RULE "WAN1^1^1500^0^10|WAN2^1^1500^0^10|WAN3^1^1500^0^10"
#define WAN0_PROTO "dhcp"
#define WAN1_PROTO "dhcp"
#define WAN2_PROTO "wwan"
#define WAN_DETECT_RULE "0^4^30^ping^gateway^|0^4^30^ping^gateway^|0^4^30^ping^gateway^"
#define WAN_LB_RULE "alwayson^1^^none^1|alwayson^1^^none^1|alwayson^1^^none^1"
#if defined (EZP_PROD_BRAND_PROX)    
#define WAN_STATUS_RULE "0^0^none^0^0|0^0^none^0^0|0^0^none^0^0"
#else
#define WAN_STATUS_RULE "0^0^none^0|0^0^none^0|0^0^none^0"
#endif
#define MAC_SENSE_RULE "^^^30^5^0^"
#ifdef defined (PLATFORM_ALS)
#define RELAY_RULE "0^0|0^0"
#define ALERT_RULE "0^0^0^0^0"
#define FAILOVER_RULE "0^0^0"
#define MAIL_RULE "0^0^0^0^0^0^0^^^^^^^^"
#define RELAY_SCHEDULE_RULE "^^^^^^|^^^^^^"
#define FILE_MODE_RULE "1^1^1^0^0^0"
#endif
#define VPN_STATUS_RULE "0^0^none^0|0^0^none^0|0^0^none^0"
#define WAN_UPNP_STATUS_RULE "0^none^"
#define WAN_BW_RULE "2M256K^2048^256^0^0^90^95^95^155^19^1244^155|2M256K^2048^256^0^0^90^95^95^155^19^1244^155|2M256K^2048^256^0^0^90^95^95^155^19^1244^155"
#define WAN_STATIC_RULE "10.1.1.25^24^10.1.1.254^10.1.1.254^^|10.1.2.25^24^10.1.2.254^10.1.2.254^^|10.1.2.25^24^10.1.2.254^10.1.2.254^^"
#define WAN_DHCP_RULE "^^86400|^^86400|^^86400"
#define WAN_HTC_RULE "^^86400^512^0|^^86400^512^0|^^86400^512^0"
#define WAN_IPHONE_RULE "^^86400^1500^0|^^86400^1500^0|^^86400^1500^0"
#define WAN_BECEEM_RULE "g1^^|g1^^|g1^^"
#define WAN_WIMAX_RULE "1386^0|1386^0|1386^0"
#define WAN_PPPOE_RULE "chap^^^0^300^20^20^1492^1492|chap^^^0^300^20^20^1492^1492|chap^^^0^300^20^20^1492^1492"
#define WAN_WWAN_RULE "2^0^0^^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0|2^0^0^^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0|2^0^0^^^auto^chap^^^0^300^20^20^1492^1492^*99***1#^0^0^0^0^0^0^1^0"
#define WAN_BARRY_RULE "^1^0^0^^^^*99***1#^0|^1^0^0^^^^*99***1#^0|^1^0^0^^^^*99***1#^0"
#define WAN_PPTP_L2TP_RULE "0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap|0^^pptp^^static^^^^^^1^300^isp^^^chap"
#define WAN_WWAN_PROBE_RULE "^^^^^^^^^^^^^^^|^^^^^^^^^^^^^^^|^^^^^^^^^^^^^^^"
#define WAN_BIGPOND_RULE "0^61.9.192.13^^|0^61.9.192.13^^|0^61.9.192.13^^"
#define WAN_HWADDR_CLONE_RULE "0^|0^|0^"
#define WAN_DDNS_RULE "WAN1_DDNS^0^dyndns^^^^|WAN2_DDNS^0^dyndns^^^^|WAN3_DDNS^0^dyndns^^^^"
#define WAN_OPENDNS_RULE "WAN1_OPENDNS^0^^^0^|WAN2_OPENDNS^0^^^0^|WAN3_OPENDNS^0^^^0^"
#define RT_RULE "SMTP^0^^^^^tcp^^^25^25^wan0^default^"
#define FR_RULE "HTTP^0^wan0^tcp^80^80^"LAN_IPADDR_MASK"20^^|HTTPS^0^wan0^tcp^443^443^"LAN_IPADDR_MASK"20^^|POP3^0^wan0^tcp^110^110^"LAN_IPADDR_MASK"20^^|POP3S^0^wan0^tcp^995^995^"LAN_IPADDR_MASK"20^^|SMTP^0^wan0^tcp^25^25^"LAN_IPADDR_MASK"20^^|SMTPS^0^wan0^tcp^465^465^"LAN_IPADDR_MASK"20^^|SSH^0^wan0^tcp^22^22^"LAN_IPADDR_MASK"21^^|eMule^0^wan0^both^4662^4672^"LAN_IPADDR_MASK"21^^"
#define FR_DMZ_RULE "^0^|^0^|^0^"

#else
#error "Undefined amount of WAN interface(s)"
#endif

#define WL_ACL_MAX "64"
#define WL_ACL_BASIC_RULE "1^disabled"

#if (WL_NUM > 0) /* WL_NUM is not zero. */

#if ( WL_NUM == 1 )
/* XXX: #define WL_BASIC_RULE "1^0^100^6^0" */
#define WL_ADVANCED_RULE "1^ap^default^0^2346^2347^1^100^0^1^1^off^off^off^1^1^HT^1^0^1^1^1^0^33^5^0^1^64^0^0^0"
#if defined (EZP_PROD_BRAND_PROX)
    #define WL_WPS_RULE "0^00000000^1^0"
#else
    #define WL_WPS_RULE "0^00000000^1^1"
#endif
#define WL_WPS_STA_RULE "0^^^^^^"
#define WL_WDS_RULE "disabled^1"
#define WL_WME_RULE "15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128"
#elif ( WL_NUM == 2 )
/* #define WL_BASIC_RULE "1^0^100^6^0|1^0^100^6^0" */
#define WL_ADVANCED_RULE "1^ap^default^0^2346^2347^1^1000^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^0^0|1^ap^default^0^2346^2347^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^0^0"
#define WL_WDS_RULE "disabled^1|disabled^1"
#define WL_WME_RULE "15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128|15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128"
#define WL_WPS_RULE "0^00000000^1^1|0^00000000^1^1"
#define WL_WPS_STA_RULE "0^^^^^^|0^^^^^^"
#else
#error "Undefined amount of WLAN interfaces"
#endif

#if ( WL_NUM == 1 )
  /* WDS */
  #define WL0_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
  #define WL0_WDS_SEC_WEP_RULE "|||"
  #define WL0_WDS_SEC_WPA_RULE "^tkip|^tkip|^tkip|^tkip"
  /* APCLI */
#ifdef EZP_PROD_BRAND_PROX
  #define WL0_APCLI_RULE "0^^^disabled^0^dhcp"
#elif EZP_SUB_BRAND_APOLLO
  #define WL0_APCLI_RULE "0^^^psk2^0^static"
#else
  #define WL0_APCLI_RULE "0^^^disabled^0"
#endif
  #define WL0_APCLI_SEC_WEP_RULE "1^^^"
  #define WL0_APCLI_SEC_WPA_RULE "^tkip"
  #define WL0_APCLI_SEC_WPA2_RULE "^aes"
  #if (WL0_RULE_MAX == 1)
    #define WL0_BASIC_RULE "1^0^0^1^0"
#if defined (EZP_PROD_BRAND_SONY)
    #define WL0_SSID_RULE "WG-C10"
#elif defined (EZP_SUB_BRAND_RFLINK)
    #define WL0_SSID_RULE "RF-LINK"
#elif defined (PLATFORM_AXM)
    #define WL0_SSID_RULE "TOGOMusic"
#else
    #define WL0_SSID_RULE "AXIMCom"
#endif /* EZP_PROD_BRAND_SONY */
    #define WL0_SEC_RULE "disabled"
    #define WL0_SEC_WEP_RULE   "1^^^^^^"
    #define WL0_SEC_WPA_RULE   "^tkip^^^1812^disable^3600^5000^3000"
    #define WL0_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^5000^0^10^3000^1"
  #elif (WL0_RULE_MAX == 2)
    #define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0"
#if defined (EZP_PROD_CAT_W)
    #define WL0_SSID_RULE "default1|default2"
#else
#if defined (EZP_PROD_BRAND_ZYUS)
    #define WL0_SSID_RULE "VFG6005N|Guest"
#elif defined (EZP_SUB_BRAND_APOLLO)
    #define WL0_SSID_RULE "WC-310|WC-310-Guest"
#elif defined (EZP_PROD_BRAND_PROX)
    #define WL0_SSID_RULE "Proxicast01|Proxicast02"
#elif defined (EZP_PROD_BRAND_PLANET)
    #define WL0_SSID_RULE "default|PLANET1"
#elif defined (EZP_PROD_BRAND_MYMO)
    #define WL0_SSID_RULE "MyMO1|MyMO2"
#elif defined (PLATFORM_M2E)
    #define WL0_SSID_RULE "AXIMCom|AXIMCom2"
#elif defined (PLATFORM_ALS)
    #define WL0_SSID_RULE "ALLISCOM1|ALLISCOM2"
#else
    #define WL0_SSID_RULE "AXIMCom1|AXIMCom2"
#endif /* EZP_PROD_BRAND_ZYUS */
#endif /* EZP_PROD_CAT_W */
#if defined (EZP_SUB_BRAND_APOLLO)
    #define WL0_SEC_RULE "psk2|disabled"
#else
    #define WL0_SEC_RULE "disabled|disabled"
#endif
    #define WL0_SEC_WEP_RULE   "1^^^^^^|1^^^^^^"
    #define WL0_SEC_WPA_RULE   "^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
#if defined (EZP_SUB_BRAND_APOLLO)
    #define WL0_SEC_WPA2_RULE   "bW9iaWxldmlkZW8h^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
#else
    #define WL0_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
#endif
  #elif (WL0_RULE_MAX == 4)
    #define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
#if defined (EZP_PROD_BRAND_PLANET)
    #define WL0_SSID_RULE "default|PLANET1|PLANET2|PLANET3"
#else
#if defined (EZP_PROD_CAT_W)
    #define WL0_SSID_RULE "default1|default2|default3|default4"
#else
    #define WL0_SSID_RULE "AXIMCom1|AXIMCom2|AXIMCom3|AXIMCom4"
#endif
#endif /* EZP_PROD_CAT_W */
    #define WL0_SEC_RULE "disabled|disabled|disabled|disabled"
    #define WL0_SEC_WEP_RULE   "1^^^^^^|1^^^^^^|1^^^^^^|1^^^^^^"
    #define WL0_SEC_WPA_RULE   "^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
    #define WL0_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
  #else
/* default set visual interface number to 2 */
    #define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0"
#if defined (EZP_PROD_CAT_W)
    #define WL0_SSID_RULE "default1|default2"
#else
    #define WL0_SSID_RULE "AXIMCom1|AXIMCom2"
#endif /* EZP_PROD_CAT_W */
    #define WL0_SEC_RULE "disabled|disabled"
    #define WL0_SEC_WEP_RULE   "1^^^^^^|1^^^^^^"
    #define WL0_SEC_WPA_RULE   "^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
    #define WL0_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
  #endif /* WL0_RULE_MAX */
#endif /* WL_NUM = 1 */

#if ( WL_NUM == 2 )
  /* WDS */
  #define WL1_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
  #define WL1_WDS_SEC_WEP_RULE "|||"
  #define WL1_WDS_SEC_WPA_RULE "^tkip|^tkip|^tkip|^tkip"
  /* APCLI */
  #define WL1_APCLI_RULE "1^^^disabled"
  #define WL1_APCLI_SEC_WEP_RULE "1^^^"
  #define WL1_APCLI_SEC_WPA_RULE "^tkip"
  #define WL1_APCLI_SEC_WPA2_RULE "^aes"
  #if (WL1_RULE_MAX == 2)
    #define WL1_BASIC_RULE "1^0^0^1^0|0^0^0^1^0"
    #define WL1_SSID_RULE "AXIMCom5|AXIMCom6"
    #define WL1_SEC_RULE "disabled|disabled"
    #define WL1_SEC_WEP_RULE   "1^^^^^^|1^^^^^^"
    #define WL1_SEC_WPA_RULE   "^tkip^^^1812^disable^3600|^tkip^^^1812^disable^3600"
    #define WL1_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^0|^aes^^^1812^disable^3600^0"
  #elif (WL1_RULE_MAX == 4)
    #define WL1_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
    #define WL1_SSID_RULE "AXIMCom5|AXIMCom6|AXIMCom7|AXIMCom8"
    #define WL1_SEC_RULE "disabled|disabled|disabled|disabled"
    #define WL1_SEC_WEP_RULE   "1^^^^^^|1^^^^^^|1^^^^^^|1^^^^^^"
    #define WL1_SEC_WPA_RULE   "^tkip^^^1812^disable^3600|^tkip^^^1812^disable^3600|^tkip^^^1812^disable^3600|^tkip^^^1812^disable^3600"
    #define WL1_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^0|^aes^^^1812^disable^3600^0|^aes^^^1812^disable^3600^0|^aes^^^1812^disable^3600^0"
  #else
    /* default set visual interface number to 2 */
    #define WL1_BASIC_RULE "1^0^0^1^0|0^0^0^1^0"
    #define WL1_SSID_RULE "AXIMCom5|AXIMCom6"
    #define WL1_SEC_RULE "disabled|disabled"
    #define WL1_SEC_WEP_RULE   "1^^^^^^|1^^^^^^"
    #define WL1_SEC_WPA_RULE   "^tkip^^^1812^disable^3600|^tkip^^^1812^disable^3600"
    #define WL1_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^0|^aes^^^1812^disable^3600^0"
  #endif
#endif
#else /* WL_NUM is zero. */
/* WL_NUM == 0 */ 
/* XXX: #define WL_BASIC_RULE "1^0^100^6^0" */
#define WL_ADVANCED_RULE "1^ap^default^0^2346^2347^1^1000^0^1^1^off^off^off^1^1^HT^1^0^1^1^1^0^33^5^0^1^64^0^0^0"
#define WL_WPS_RULE "0^00000000^1^1"
#define WL_WPS_STA_RULE "0^^^^^^"
#define WL_WDS_RULE "disabled^1"
#define WL_WME_RULE "15 1023 7 0 0 off^15 1023 3 0 0 off^7 15 2 6016 3008 off^3 7 2 3264 1504 off^15 1023 7 0 0 off^15 63 3 0 0 off^7 15 1 6016 3008 off^3 7 1 3264 1504 off^off^128"
/* WDS */
#define WL0_WDS_BASIC_RULE "^disabled|^disabled|^disabled|^disabled"
#define WL0_WDS_SEC_WEP_RULE "|||"
#define WL0_WDS_SEC_WPA_RULE "^tkip|^tkip|^tkip|^tkip"
/* APCLI */
#define WL0_APCLI_RULE "0^^^disabled^0"
#define WL0_APCLI_SEC_WEP_RULE "1^^^"
#define WL0_APCLI_SEC_WPA_RULE "^tkip"
#define WL0_APCLI_SEC_WPA2_RULE "^aes"
#if (WL0_RULE_MAX == 2)
#define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0"
#if defined (EZP_PROD_CAT_W)
#define WL0_SSID_RULE "default1|default2"
#else
#if defined (EZP_PROD_BRAND_ZYUS)
#define WL0_SSID_RULE "VFG6005N|Guest"
#else
#define WL0_SSID_RULE "AXIMCom1|AXIMCom2"
#endif /* EZP_PROD_BRAND_ZYUS */
#endif /* EZP_PROD_CAT_W */
#define WL0_SEC_RULE "disabled|disabled"
#define WL0_SEC_WEP_RULE   "1^^^^^^|1^^^^^^"
#define WL0_SEC_WPA_RULE   "^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
#define WL0_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
#elif (WL0_RULE_MAX == 4)
#define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0|0^0^0^1^0|0^0^0^1^0"
#if defined (EZP_PROD_CAT_W)
#define WL0_SSID_RULE "default1|default2|default3|default4"
#else
#define WL0_SSID_RULE "AXIMCom1|AXIMCom2|AXIMCom3|AXIMCom4"
#endif /* EZP_PROD_CAT_W */
#define WL0_SEC_RULE "disabled|disabled|disabled|disabled"
#define WL0_SEC_WEP_RULE   "1^^^^^^|1^^^^^^|1^^^^^^|1^^^^^^"
#define WL0_SEC_WPA_RULE   "^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
#define WL0_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
#else
/* default set visual interface number to 2 */
#define WL0_BASIC_RULE "1^0^0^1^0|0^0^0^1^0"
#if defined (EZP_PROD_CAT_W)
#define WL0_SSID_RULE "default1|default2"
#else
#define WL0_SSID_RULE "AXIMCom1|AXIMCom2"
#endif /* EZP_PROD_CAT_W */
#define WL0_SEC_RULE "disabled|disabled"
#define WL0_SEC_WEP_RULE   "1^^^^^^|1^^^^^^"
#define WL0_SEC_WPA_RULE   "^tkip^^^1812^disable^3600^5000^3000|^tkip^^^1812^disable^3600^5000^3000"
#define WL0_SEC_WPA2_RULE   "^aes^^^1812^disable^3600^5000^0^10^3000^1|^aes^^^1812^disable^3600^5000^0^10^3000^1"
#endif /* WL0_RULE_MAX */
#endif /* WL_NUM is zero. */
#if defined (PLATFORM_AXA) || defined (PLATFORM_M2E)
  #define CNNT_RULE_NUM "3"
  #define CNNT_RULE_MAX "8"
  #define CNNT_RULE "1^Ethernet^0^1^dhcp|1^WISP^0^0^wisp|1^USB^0^0^iphone"
  #define FTPD_RULE           "1^21^1^1^10^10^3:4^133:022^1000^5^1"
  #define FTPD_USER_RULE      "1^admin^adminpassword^50^100^"
  #define FTPD_USER_RULE_NUM  "1"
  #define NETSTAT_RULE        "5"
#elif defined (PLATFORM_AXM)
  #define CNNT_RULE_NUM "1"
  #define CNNT_RULE_MAX "8"
  #define CNNT_RULE "1^WISP^0^0^wisp"
  #define FTPD_RULE           "0^21^1^1^10^10^3:4^133:022^1000^5^0"
  #define FTPD_USER_RULE      "0^admin^adminpassword^50^100^"
  #define FTPD_USER_RULE_NUM  "1"
  #define NETSTAT_RULE        "3"
#else
  #define CNNT_RULE_NUM "3"
  #define CNNT_RULE_MAX "8"
  #define CNNT_RULE "1^Ethernet^0^0^dhcp|1^WISP^0^0^wisp|1^USB^0^0^iphone"
  #define FTPD_RULE           "0^21^1^1^10^10^3:4^133:022^1000^5^0"
  #define FTPD_USER_RULE      "0^admin^adminpassword^50^100^"
  #define FTPD_USER_RULE_NUM  "1"
  #define NETSTAT_RULE        "3"
#endif

#if defined (PLATFORM_AXA)
  /* Card reader and storage mount rule for TOGOBox M1*/
  #define CARD_READER_HARDWARE_RULE_MAX "1"
  #define CARD_READER_HARDWARE_RULE_NUM "1"
  /* dev^gpio^gpiopol */
  #define CARD_READER_HARDWARE_RULE     "sda^14^1"
  #define STORAGE_DEVICE_MOUNT_RULE     "1-1.1^USB^0777^0777^1^0|1-1.2^SD^0777^0777^1^0"
  #define STORAGE_DEVICE_MOUNT_RULE_NUM "2"
  #define GPIODEF_RULE ""
#elif defined (PLATFORM_M2E)
  /* Card reader and storage mount rule for TOGOBox M2*/
  #define CARD_READER_HARDWARE_RULE_MAX "1"
  #define CARD_READER_HARDWARE_RULE_NUM "1"
  /* dev^gpio^gpiopol */
  #define CARD_READER_HARDWARE_RULE     "sda^43^1"
  #define STORAGE_DEVICE_MOUNT_RULE     "1-1.1^USB^0777^0777^1^0|1-1.2^SD^0777^0777^1^0|1-1.3^Qsync^0777^0777^0^0"
  #define STORAGE_DEVICE_MOUNT_RULE_NUM "3"
  #define GPIODEF_RULE ""
#elif defined (PLATFORM_ALS)
  /* Card reader and storage mount rule for TOGOBox M2*/
  #define CARD_READER_HARDWARE_RULE_MAX "2"
  #define CARD_READER_HARDWARE_RULE_NUM "2"
  /* dev^gpio^gpiopol */
  #define CARD_READER_HARDWARE_RULE     "sda^43^1"
  #define STORAGE_DEVICE_MOUNT_RULE     "1-1.2^USB^0777^0777^1^0|mmc0^SD^0777^0777^1^0"
  #define STORAGE_DEVICE_MOUNT_RULE_NUM "2"
  #define GPIODEF_RULE ""
#elif defined (EZP_SUB_BRAND_SONY)
  /* Card reader and storage mount rule for Sony Mosra */
  #define CARD_READER_HARDWARE_RULE_MAX "2"
  #define CARD_READER_HARDWARE_RULE_NUM "2"
  /* dev^gpio^gpiopol , for Sony Mosra */
  #define CARD_READER_HARDWARE_RULE     "sda^11^1|sda^12^1"
  #define STORAGE_DEVICE_MOUNT_RULE     "1-1.2^USB^0777^0777^1^0|1-1.1^SD^0777^0777^1^0"
  #define STORAGE_DEVICE_MOUNT_RULE_NUM "2"
  #define GPIODEF_RULE ""
#else
  #define CARD_READER_HARDWARE_RULE_MAX "0"
  #define CARD_READER_HARDWARE_RULE_NUM "0"
  /* dev^gpio^gpiopol */
  #define CARD_READER_HARDWARE_RULE     ""
  #define STORAGE_DEVICE_MOUNT_RULE     ""
  #define STORAGE_DEVICE_MOUNT_RULE_NUM "0"
  #define GPIODEF_RULE ""
#endif

#if defined(PLATFORM_ALS)
#define ALS_BATTERY_RULE ""
#define ALS_STATUS_RULE "0^0^0^0^0^0^0^0|0^0^0^0^0^0^0^0|0^0^0^0^0^0^0^0"
#define ALS_SERAIL_RULE "port1^115200^8^1^none^none^1^/dev/ttyS0|port2^115200^8^1^none^none^1^/dev/ttyUSB0|port3^115200^8^1^none^none^1^/dev/ttyUSB1"
#define ALS_OPERATING_RULE "0^7^0^4^1|0^7^0^4^1|0^7^0^4^1"
#define ALS_OPERATING_DATA_PACKING_RULE "1024^700|1024^700|1024^700"
#define ALS_OPERATING_OPTION_RULE "101^^4001^^4001^^4001^^4001^5011^5012^5013^5014^100|102^^4001^^4001^^4001^^4001^5011^5012^5013^5014^100|103^^4001^^4001^^4001^^4001^5011^5012^5013^5014^100"
#endif


/* default assigned gateway */
#define HTTP_REDIRECT_DEFAULT "0"

/* IGMP PROXY FUNCTION */
#if defined(EZP_SUB_BRAND_GARMIN) 
#define IGMP_PROXY_ENABLE "1"
#else
#define IGMP_PROXY_ENABLE "0"
#endif

/* record for MFG test */
#define MFG_TEST_RESULT_RULE "^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|^|"
#if defined (PLATFORM_GMA)
/* portal status*/
#define WISP_DETECT_PORTAL_RULE "0^0"
#endif
/* 
 * The following macros are to stringify the macro argument. Please refer to
 * "C Preprocessor - Stringification".
 */
#define xstr(s) str(s)
#define str(s)  #s 

#endif /*_NVRAM_EZPACKET_H */
