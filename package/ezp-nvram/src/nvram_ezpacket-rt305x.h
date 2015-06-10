#ifdef CONFIG_EZP_ARCH_RT305X
#define ARCH "RT305X"
#if defined(EZP_PROD_SUPERCAT_X)
    #if defined(EZP_PROD_CAT_P)
    #include "P/nvram_ezpacket-rt305x_P.h"
    #endif
    #if defined(EZP_PROD_CAT_K)
    #include "K/nvram_ezpacket-rt305x_K.h"
    #endif
    #if defined(EZP_PROD_CAT_G)
    #include "G/nvram_ezpacket-rt305x_G.h"
    #endif
    #if defined(EZP_PROD_CAT_I)
    #include "I/nvram_ezpacket-rt305x_I.h"
    #endif
#endif /*defined(EZP_PROD_SUPERCAT_X)*/

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

#ifndef WL_NUM
#define WL_NUM 1
#endif

#ifdef EZP_PROD_BRAND_PLANET
#define WL_VIRTUAL_NUM 4
#endif

#if defined(PLATFORM_AXA) || defined(PLATFORM_AXB) || defined(PLATFORM_LIS) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
#define WL_VIRTUAL_NUM 1
#elif defined(PLATFORM_ALS)
#define WL_VIRTUAL_NUM 2
#endif

#ifndef WL_VIRTUAL_NUM
#define WL_VIRTUAL_NUM 2
#endif

#ifndef WL_WDS_NUM
#define WL_WDS_NUM 4
#endif

#ifndef WL_APCLI_NUM
#define WL_APCLI_NUM 1
#endif

#if ( WL_NUM == 1 )
#if defined (EZP_PROD_BRAND_PROX) && !defined(EZP_SUB_BRAND_APOLLO)
#define WL_BASIC_RULE "0^9^100^6^0"
#elif defined (EZP_PROD_BRAND_AXIM)
#define WL_BASIC_RULE "1^9^100^0^0"
#else
#define WL_BASIC_RULE "1^9^100^6^0"
#endif 
#if ( WL_VIRTUAL_NUM == 1 ) /* WL0_IFNAME_RULE*/
#define WL0_IFNAME_RULE "ra0"
#elif ( WL_VIRTUAL_NUM == 2 )
#define WL0_IFNAME_RULE "ra0|ra1"
#elif ( WL_VIRTUAL_NUM == 4 )
#define WL0_IFNAME_RULE "ra0|ra1|ra2|ra3"
#endif /* WL0_IFNAME_RULE*/
#define WL0_RULE_NUM WL_VIRTUAL_NUM
#define WL0_RULE_MAX WL_VIRTUAL_NUM
#define WDS_RULE_MAX WL_WDS_NUM
#define APCLI_RULE_MAX WL_APCLI_NUM
#elif ( WL_NUM == 2 )
#define WL_BASIC_RULE "1^9^100^6^0|1^9^100^6^0"
#if ( WL_VIRTUAL_NUM == 1 ) /* WL0_IFNAME_RULE/WL1_IFNAME_RULE*/
#define WL0_IFNAME_RULE "ra0"
#define WL1_IFNAME_RULE "ra1"
#elif ( WL_VIRTUAL_NUM == 2 )
#define WL0_IFNAME_RULE "ra0|ra2"
#define WL1_IFNAME_RULE "ra1|ra3"
#elif ( WL_VIRTUAL_NUM == 4 )
#define WL0_IFNAME_RULE "ra0|ra1|ra2|ra3"
#define WL1_IFNAME_RULE "ra1|ra5|ra6|ra7"
#endif /* WL0_IFNAME_RULE/WL1_RULE_NUM */
#define WL0_RULE_NUM WL_VIRTUAL_NUM
#define WL0_RULE_MAX WL_VIRTUAL_NUM
#define WL1_RULE_NUM WL_VIRTUAL_NUM
#define WL1_RULE_MAX WL_VIRTUAL_NUM
#define WDS_RULE_MAX WL_WDS_NUM
#define APCLI_RULE_MAX WL_APCLI_NUM
#elif (WL_NUM == 0 )
#define WL_BASIC_RULE "0^9^100^6^0"
#if ( WL_VIRTUAL_NUM == 1 ) /* WL0_IFNAME_RULE*/
#define WL0_IFNAME_RULE "ra0"
#elif ( WL_VIRTUAL_NUM == 2 )
#define WL0_IFNAME_RULE "ra0|ra1"
#elif ( WL_VIRTUAL_NUM == 4 )
#define WL0_IFNAME_RULE "ra0|ra1|ra2|ra3"
#endif /* WL0_IFNAME_RULE*/
#define WL0_RULE_NUM WL_VIRTUAL_NUM
#define WL0_RULE_MAX WL_VIRTUAL_NUM
#define WDS_RULE_MAX WL_WDS_NUM
#define APCLI_RULE_MAX WL_APCLI_NUM
#else
#error "Undefined amount of WLAN interfaces"
#endif

#if ( WAN_NUM == 1 ) && ( LAN_NUM == 1 )

#if !defined(EZP_PROD_SUPERCAT_X)
    #if defined(PLATFORM_AXM) || defined(PLATFORM_LIS)
    #define WAN0_IFNAME "apcli0"
    #elif defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
    #define WAN0_IFNAME "vlan2"
    #elif defined(PLATFORM_AXA)
    #define WAN0_IFNAME "vlan2"
    #elif (APPMGR_WWAN == 1)
    /* 3G/4G WAN default interface should be ppp0. */
    #define WAN0_IFNAME "ppp0"
    #else
    #define WAN0_IFNAME "vlan2"
    #endif
#endif /*!defined(EZP_PROD_SUPERCAT_X)*/

#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "vlan2"
#define WAN0_PORTS "4"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "vlan1 ra0 ra1 apcli0"
#define LAN0_IFNAMES_WDS "vlan1 ra0 ra1 apcli0 wds0 wds1 wds2 wds3"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "vlan1"

#if defined(AR8316) || defined(AR8328)
	#if defined (EZP_SUB_BRAND_GARMIN)
    	#define VLAN_RULE_NUM 4
	#else
    	#define VLAN_RULE_NUM 2
	#endif
    	#define VLAN_RULE_MAX 8
    	#define VLANPORT_RULE_NUM 5
    	#define VLANPORT_RULE_MAX 6
	#if defined (EZP_SUB_BRAND_GARMIN)
		/* WAN1^1^eth2^"2" && PORT3^2^"2" so PORT3 is wan port */
        #define VLAN_RULE "LAN1^1^eth2^1^011111|WAN1^1^eth2^2^100001|ARM^1^eth2^4^011111|GMN^1^eth2^3^011111"
		#define VLANPORT_RULE "PORT1^0^1^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^2^1^2^010111^1|PORT4^3^3^1^2^011011^1|PORT5^4^4^1^2^011101^1|PORT6^5^5^2^2^111111^1"
    #elif defined (PLATFORM_AMD) || defined (PLATFORM_AMC) || defined (PLATFORM_GMA)
        #define VLAN_RULE "LAN1^1^eth2^1^011111|WAN1^1^eth2^2^100001"
        #define VLANPORT_RULE "PORT1^0^2^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^1^1^2^011011^1|PORT5^4^1^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #else
        #define VLAN_RULE "LAN1^1^eth2^1^111101|WAN1^1^eth2^2^000011"
        #define VLANPORT_RULE "PORT1^0^1^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^1^1^2^011011^1|PORT5^4^2^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #endif /* PLATFORM_AMD  PLATFORM_AMC PLATFORM_GMA */
#else
    #define VLAN_RULE_NUM 2
    #define VLAN_RULE_MAX 8
    #define VLANPORT_RULE_NUM 5
    #define VLANPORT_RULE_MAX 7
#if defined (PLATFORM_BAB) || defined (PLATFORM_AXB) || defined(PLATFORM_AXM)
    /* Baudtec 1 lan 1 wan board uses p1 as wan port, not p4 */
    #define VLAN_RULE "LAN1^1^eth2^1^0111101|WAN1^1^eth2^2^1000001"
    #define VLANPORT_RULE "PORT1^0^2^0^1|PORT2^1^1^0^1|PORT3^2^1^0^1|PORT4^3^1^0^1|PORT5^4^1^0^1|PORT6^5^0^0^1|PORT7^6^0^1^1"
#elif defined (PLATFORM_AXA) || defined (PLATFORM_M2E) || defined(PLATFORM_ALS)
    /* Default set port4 to wan */
    #define VLAN_RULE "LAN1^1^eth2^1^1111001|WAN1^1^eth2^2^0000101"
    #define VLANPORT_RULE "PORT1^0^1^0^1|PORT2^1^1^0^1|PORT3^2^1^0^1|PORT4^3^1^0^1|PORT5^4^2^0^1|PORT6^5^0^0^1|PORT7^6^0^1^1"
#elif defined (PLATFORM_LIS)
    #define VLAN_RULE "LAN1^1^eth2^1^1111101|WAN1^1^eth2^2^0000001"
    #define VLANPORT_RULE "PORT1^0^1^0^1|PORT2^1^1^0^1|PORT3^2^1^0^1|PORT4^3^1^0^1|PORT5^4^1^0^1|PORT6^5^0^0^1|PORT7^6^0^1^1"
#else
    #define VLAN_RULE "LAN1^1^eth2^1^1111001|WAN1^1^eth2^2^0000101"
    #define VLANPORT_RULE "PORT1^0^1^0^1|PORT2^1^1^0^1|PORT3^2^1^0^1|PORT4^3^1^0^1|PORT5^4^2^0^1|PORT6^5^0^0^1|PORT7^6^0^1^1"
#endif /*PLATFORM_*/
#endif /* AR8316 */

/* EZP: Old VLAN definition should be Obsolete! */
#define LAN0_PORTS "0 1 2 3"
/* WAN0 uses vlan2. XXX: No effect! */
#define VLAN2_PORTS "4 5"
/* LAN0 uses vlan1. XXX: No effect! */
#define VLAN1_PORTS "0 1 2 3 5*"
#define VLAN_HWNAME "eth2"

#if !defined(EZP_PROD_SUPERCAT_X)
#if defined(EZP_PROD_CAT_K)
/* For NAS series, we use apcli on wan, so don't apply br_rule */
#define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^"
#elif defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
#define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
#else
#define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
#endif /* EZP_PROD_CAT */
#endif /* !defined(EZP_PROD_SUPERCAT_X) */

#define BR_RULE_NUM 2
#define BR_RULE_MAX 8

#define STATS_RULE "^1^60^vlan1 vlan2 ppp0 br0 ra0 usb0 eth0"

#elif ( WAN_NUM == 2 ) && ( LAN_NUM == 1 ) 

#define WAN0_IFNAME "vlan2"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "vlan2"
#define WAN0_PORTS "4"

/* 3G/4G 2nd WAN default interface should be ppp0. */
#if defined(EZP_PROD_CAT_N) || defined(EZP_PROD_CAT_A) || defined(EZP_PROD_CAT_I)
#define WAN1_IFNAME "ppp1"
#else
#define WAN1_IFNAME "vlan3"
#endif /* EZP_PROD_CAT */

#if defined(EZP_PROD_BRAND_ZYUS)
#define WAN1_DEVICE "ppp1"
#else
#define WAN1_DEVICE "vlan3"
#endif /* EZP_PROD_BRAND */

#define WAN1_IFNAMES ""
#define WAN1_HWNAME ""
#define WAN1_PORTS "3"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "vlan1 ra0 ra1 apcli0"
#define LAN0_IFNAMES_WDS "vlan1 ra0 ra1 apcli0 wds0 wds1 wds2 wds3"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "vlan1"
#define LAN0_PORTS "0 1 2"


#if defined(AR8316) || defined(AR8328)
    #define VLAN_RULE_MAX 16 
    #define VLANPORT_RULE_NUM 5
    #define VLANPORT_RULE_MAX 6
    #if defined (PLATFORM_AMD) || defined (PLATFORM_AMC) || defined (PLATFORM_GMA)
        #define VLAN_RULE_NUM 2
        #define VLAN_RULE "LAN1^1^eth2^1^011111|WAN1^1^eth2^2^100001"
        #define VLANPORT_RULE "PORT1^0^2^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^1^1^2^011011^1|PORT5^4^1^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #elif defined(PLATFORM_ABE) || defined(PLATFORM_ABE16)
        #define VLAN_RULE_NUM 2
        #define VLAN_RULE "LAN1^1^eth2^1^1111011^^1|WAN1^1^eth2^2^0000111^^1"
        #define VLANPORT_RULE "PORT1^0^1^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^1^1^2^011011^1|PORT5^4^2^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #elif defined(PLATFORM_BAD)
        #define VLAN_RULE_NUM 2
        #define VLAN_RULE "LAN1^1^eth2^1^111101|WAN1^1^eth2^2^000011"
        #define VLANPORT_RULE "PORT1^0^1^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^1^1^2^011011^1|PORT5^4^2^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #else
        #define VLAN_RULE_NUM 3
        #define VLAN_RULE "LAN1^1^eth2^1^1110011^^1|WAN1^1^eth2^2^0000111^^1|WAN2^1^eth2^3^0001011^^1"
        #define VLANPORT_RULE "PORT1^0^1^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^3^1^2^011011^1|PORT5^4^2^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #endif /* PLATFORM */
#else
    #define VLAN_RULE_NUM 3
    #define VLAN_RULE_MAX 16 
    #define VLANPORT_RULE_NUM 5
    #define VLANPORT_RULE_MAX 7
    #define VLAN_RULE "LAN1^1^eth2^1^1110001|WAN1^1^eth2^2^0000101|WAN2^1^eth2^3^0001001"
    #define VLANPORT_RULE "PORT1^0^1^0^1|PORT2^1^1^0^1|PORT3^2^1^0^1|PORT4^3^3^0^1|PORT5^4^2^0^1|PORT6^5^0^0^1|PORT7^6^0^1^1"
#endif /* AR8316 */

/* WAN0 uses vlan2. */
#define VLAN2_PORTS "4 5"
/* WAN1 uses vlan3. */
#define VLAN3_PORTS "3 5"
/* LAN0 uses vlan1. */
#define VLAN1_PORTS "0 1 2 5"

#define VLAN_HWNAME "eth2"

#if defined(EZP_PROD_BRAND_ZYUS)
    #if defined(EZP_PROD_CAT_N)
        #define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
    #elif defined(EZP_PROD_CAT_A)
        #define BR_RULE "LAN1^1^vlan1 ra0^|WAN1^0^vlan2^"
    #endif /* EZP_PROD_CAT */
#define BR_RULE_NUM 2
#define STATS_RULE "^1^60^vlan1 vlan2 ppp0 ppp1 br0 ra0 usb0 eth0 eth1"
#else
#if defined (PLATFORM_AMC)
    #define BR_RULE "LAN1^1^vlan1 ra0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#else
    #if defined(EZP_PROD_CAT_K)
            /* For NAS series, we use apcli on wan, so don't apply br_rule */
        #define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^|WAN2^0^vlan3^"
    #else
        #define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
    #endif /* EZP_PROD_CAT */
#endif /* PLATFORM */
#define BR_RULE_NUM 3
#define STATS_RULE "^1^60^vlan3 vlan1 vlan2 ppp0 ppp1 br0 ra0 usb0 eth0 eth1"
#endif /* EZP_PROD_BRAND */

#define BR_RULE_MAX 16


#elif ( WAN_NUM == 3 ) && ( LAN_NUM == 1 ) 

#define WAN0_IFNAME "vlan2"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "vlan2"
#define WAN0_PORTS "4"

#define WAN1_IFNAME "vlan3"
#define WAN1_IFNAMES ""
#define WAN1_HWNAME ""
#define WAN1_DEVICE "vlan3"
#define WAN1_PORTS "3"

#if defined(EZP_PROD_CAT_N) || defined(EZP_PROD_CAT_A)
#define WAN2_IFNAME "ppp2"
#define WAN2_DEVICE "ppp2"
#else
#define WAN2_IFNAME "vlan4"
#define WAN2_DEVICE "vlan4"
#endif

#define WAN2_IFNAMES ""
#define WAN2_HWNAME ""
#define WAN2_PORTS "2"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "vlan1 ra0 ra1 apcli0"
#define LAN0_IFNAMES_WDS "vlan1 ra0 ra1 apcli0 wds0 wds1 wds2 wds3"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "vlan1"
#define LAN0_PORTS "0 1"

#if defined(AR8316) || defined(AR8328)
    #define VLAN_RULE_NUM 3
    #define VLAN_RULE_MAX 16 
    #define VLANPORT_RULE_NUM 5
    #define VLANPORT_RULE_MAX 6
    #if defined (PLATFORM_AMD) || defined (PLATFORM_AMC) || defined (PLATFORM_GMA)
        #define VLAN_RULE "LAN1^1^eth2^1^011101|WAN1^1^eth2^2^100001|WAN2^1^eth2^3^000011"
        #define VLANPORT_RULE "PORT1^0^2^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^1^1^2^011011^1|PORT5^4^3^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #else
        #define VLAN_RULE "LAN1^1^eth2^1^1110011^^1|WAN1^1^eth2^2^0000111^^1|WAN2^1^eth2^3^0001011^^1"
        #define VLANPORT_RULE "PORT1^0^1^1^2^000001^1|PORT2^1^1^1^2^001111^1|PORT3^2^1^1^2^010111^1|PORT4^3^3^1^2^011011^1|PORT5^4^2^1^2^011101^1|PORT6^5^2^2^2^111111^1"
    #endif
#else
    #define VLAN_RULE_NUM 3
    #define VLAN_RULE_MAX 16 
    #define VLANPORT_RULE_NUM 5
    #define VLANPORT_RULE_MAX 7
    #define VLAN_RULE "LAN1^1^eth2^1^1110001|WAN1^1^eth2^2^0000101|WAN2^1^eth2^3^0001001"
    #define VLANPORT_RULE "PORT1^0^1^0^1|PORT2^1^1^0^1|PORT3^2^1^0^1|PORT4^3^3^0^1|PORT5^4^2^0^1|PORT6^5^0^0^1|PORT7^6^0^1^1"
#endif /* AR8316 */

/* WAN0 uses vlan2. */
#define VLAN2_PORTS "4 5"
/* WAN1 uses vlan3. */
#define VLAN3_PORTS "3 5"
/* LAN0 uses vlan1. */
#define VLAN1_PORTS "0 1 2 5"

#define VLAN_HWNAME "eth2"

#if defined (PLATFORM_AMC)
    #define BR_RULE "LAN1^1^vlan1 ra0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#else
    #if defined(EZP_PROD_CAT_K)
            /* For NAS series, we use apcli on wan, so don't apply br_rule */
    #define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^|WAN2^0^vlan3^"
    #else
    #define BR_RULE "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
    #endif
#endif

#define BR_RULE_NUM 3
#define BR_RULE_MAX 16
#define STATS_RULE "^1^60^vlan3 vlan1 vlan2 ppp0 ppp1 ppp2 br0 ra0 usb0 eth0 eth1"
#else

#error "Undefined amount of LAN/WAN interfaces"

#endif

#define BW_TYPE \
        "1M64K", "1M128K", "1.5M640K", "2M128K", "2M384K","2M256K", "2M512K", \
        "3M640K", "3M384K", "4M1M", "6M256K", "6M640K", "8M256K", "8M640K", \
        "8M800K", "10M2M", "10M1M", "12M1M", "20M2M", "50M5M", "100M10M", \
        "2M2M", "4M4M", "10M10M", "25M25M", "50M50M", "100M100M","Custom"
#if ( EZP_PROD_SUBCAT == 0 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 1 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 2 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 3 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 4 )
#if defined(EZP_PROD_BRAND_ZYUS)
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#else
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#endif /* EZP_PROD_BRAND */
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 5 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 6 )
#define BW_DOWNLOAD_MAX "102400"
#define BW_UPLOAD_MAX "102400"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#endif /* EZP_PROD_SUBCAT */

#define NVRAM_EZPACKET_DEFAULT_RT305X_LAN_NUM_1   \
    /* WAN1 uses vlan1. */              \
    { "vlan1ports", VLAN1_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 port. */                \
    { "vlan1hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 device hwname. */       \

#define NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_1   \
    /* WAN1 uses vlan2. */              \
    { "vlan2ports", VLAN2_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 port. */                \
    { "vlan2hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 device hwname. */       \


#define NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_2   \
    /* WAN3 uses vlan3. */              \
    { "vlan2ports", VLAN3_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN3 port. */                \
    { "vlan3hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN3 device hwname. */       \



#endif /* CONFIG_EZP_ARCH_RT305X */

