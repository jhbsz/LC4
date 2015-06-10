#ifdef CONFIG_EZP_ARCH_AS500
#define ARCH "AS500"
#if ( WAN_NUM == 1 ) && ( LAN_NUM == 1 )

#define WAN0_IFNAME "eth1"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "eth1"
#define WAN0_PORTS "4"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "eth0"
#define LAN0_IFNAMES_WDS "eth0"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "eth0"
#define LAN0_PORTS "0 1 2 3"

/* WAN0 uses vlan2. XXX: No effect! */
#define VLAN2_PORTS "4 5"
/* LAN0 uses vlan1. XXX: No effect! */
#define VLAN1_PORTS "0 1 2 3 5*"

#define VLAN_HWNAME "eth1"

#define STATS_RULE "^1^60^eth0 eth1 br0"

#elif ( WAN_NUM == 2 ) && ( LAN_NUM == 1 ) 

#define WAN0_IFNAME "eth1"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "eth1"
#define WAN0_PORTS "4"

#define WAN1_IFNAME "vlan1"
#define WAN1_IFNAMES ""
#define WAN1_HWNAME ""
#define WAN1_DEVICE "vlan1"
#define WAN1_PORTS "3"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "vlan2"
#define LAN0_IFNAMES_WDS "eth0 eth1 vlan1 vlan2"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "vlan2"
#define LAN0_PORTS "0 1 2"

/* WAN0 uses vlan2. */
#define VLAN2_PORTS "4 5"
/* WAN1 uses vlan3. */
#define VLAN3_PORTS "3 5"
/* LAN0 uses vlan1. */
#define VLAN1_PORTS "0 1 2 5"

#define VLAN_HWNAME "eth0"

#define STATS_RULE "^1^60^vlan2 vlan1 eth0 eth1"

#else

#error "Undefined amount of LAN/WAN interfaces"

#endif


#if 0
#ifndef WL_NUM
#define WL_NUM 1
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
#define WL_BASIC_RULE "1^9^100^6^0"
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
#else
#error "Undefined amount of WLAN interfaces"
#endif
#endif

#define BW_TYPE \
        "A1M64K", "A1.5M640K", "A2M256K", "A2M512K", "A3M640K", "A4M1M", \
        "A6M256K", "A6M640K", "A8M256K", "A8M640K", "A8M800K", "A10M2M", \
        "A12M1M", "C1M128K", "C2M128K", "C2M384K", "C3M384K", "C6M640K", \
        "C8M640K", "C10M1M", "V2M2M", "V4M4M", "V10M10M", "V25M25M", \
        "V50M50M", "Custom"
#if ( EZP_PROD_SUBCAT == 0 )
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 1 )
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 2 )
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 3 )
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 4 )
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 5 )
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 6 )
#define BW_DOWNLOAD_MAX "51200"
#define BW_UPLOAD_MAX "51200"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#endif /* EZP_PROD_SUBCAT */

#define NVRAM_EZPACKET_DEFAULT_AS500_LAN_NUM_1   \
    /* WAN1 uses vlan1. */              \
    { "vlan1ports", VLAN1_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 port. */                \
    { "vlan1hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 device hwname. */       \

#define NVRAM_EZPACKET_DEFAULT_AS500_WAN_NUM_1   \
    /* WAN1 uses vlan2. */              \
    { "vlan2ports", VLAN2_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 port. */                \
    { "vlan2hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN1 device hwname. */       \


#define NVRAM_EZPACKET_DEFAULT_AS500_WAN_NUM_2   \
    /* WAN3 uses vlan3. */              \
    { "vlan2ports", VLAN3_PORTS,        \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN3 port. */                \
    { "vlan3hwname", VLAN_HWNAME,       \
        NVRAM_NONE,                     \
        0                               \
    }, /* VLAN3 device hwname. */       \


#define NVRAM_EZPACKET_DEFAULT_AS500_SWAP   \
    { "no_root_swap", "1",              \
        NVRAM_NONE,                     \
        0                               \
    },                                  \

#define NVRAM_EZPACKET_DEFAULT_AS500_INITD  \
    { "no_rcd_run", "0",                \
        NVRAM_NONE,                     \
        0                               \
    },                                  \

#endif /* CONFIG_EZP_ARCH_AS500 */

