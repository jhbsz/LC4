

#ifdef CONFIG_EZP_ARCH_BRCM

#define ARCH "Broadcom"
#if ( WAN_NUM == 1 ) && ( LAN_NUM == 1 )

#define WAN0_IFNAME "vlan1"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME "et0"
#define WAN0_DEVICE "vlan1"
#define WAN0_PORTS "4"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "vlan0 eth1"
#define LAN0_IFNAMES_WDS "vlan0 eth1"
#define LAN0_HWNAME ""
#define LAN0_DEVICE ""
#define LAN0_PORTS "0 1 2 3"

/* WAN0 uses vlan1. */
#define VLAN1_PORTS "0 5"
/* LAN0 uses vlan0. */
#define VLAN0_PORTS "1 2 3 4 5*"

#define VLAN_HWNAME "et0"

#define STATS_RULE "^1^60^vlan1 vlan0 eth1 br0"

#elif ( WAN_NUM == 2 ) && ( LAN_NUM == 1 ) 

#define WAN0_IFNAME "vlan1"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "vlan1"
#define WAN0_PORTS "4"

#define WAN1_IFNAME "vlan2"
#define WAN1_IFNAMES ""
#define WAN1_HWNAME ""
#define WAN1_DEVICE "vlan2"
#define WAN1_PORTS "3"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "vlan0 eth1"
#define LAN0_IFNAMES_WDS "vlan0 eth1"
#define LAN0_HWNAME ""
#define LAN0_DEVICE ""
#define LAN0_PORTS "0 1 2"

/* WAN0 uses vlan1. */
#define VLAN1_PORTS "4 5"
/* WAN1 uses vlan2. */
#define VLAN2_PORTS "3 5"
/* LAN0 uses vlan0. */
#define VLAN0_PORTS "2 1 0 5*"

#define VLAN_HWNAME "et0"

#define STATS_RULE "^1^60^vlan1 vlan2 vlan0 eth1 br0"

#else

#error "Undefined amount of LAN/WAN interfaces"

#endif

#define WL0_IFNAME "eth1"

/* BRCM */
#define BW_TYPE \
        "A1M64K", "A1.5M640K", "A2M256K", "A2M512K", "A3M640K", "A4M1M", \
        "A6M256K", "A6M640K", "A8M256K", "A8M640K", "A8M800K", "A10M2M", \
        "A12M1M", "C1M128K", "C2M128K", "C2M384K", "C3M384K", "C6M640K", \
        "C8M640K", "C10M1M", "V2M2M", "V4M4M", "V10M10M", "Custom"
#if ( EZP_PROD_SUBCAT == 0 )
#define BW_DOWNLOAD_MAX "12288"
#define BW_UPLOAD_MAX "12288"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 1 )
#define BW_DOWNLOAD_MAX "12288"
#define BW_UPLOAD_MAX "12288"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 2 )
#define BW_DOWNLOAD_MAX "12288"
#define BW_UPLOAD_MAX "12288"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 3 )
#define BW_DOWNLOAD_MAX "12288"
#define BW_UPLOAD_MAX "12288"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 4 )
#define BW_DOWNLOAD_MAX "12288"
#define BW_UPLOAD_MAX "12288"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 5 )
#define BW_DOWNLOAD_MAX "12288"
#define BW_UPLOAD_MAX "12288"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#elif ( EZP_PROD_SUBCAT == 6 )
#define BW_DOWNLOAD_MAX "12288"
#define BW_UPLOAD_MAX "12288"
#define BW_DOWNLOAD_MIN "64"
#define BW_UPLOAD_MIN "64"
#endif /* EZP_PROD_SUBCAT */




#define NVRAM_EZPACKET_DEFAULT_BRCM_LAN_NUM_1   \
    /* LAN0 uses vlan0. */                      \
    { "vlan0ports", VLAN0_PORTS,                \
        NVRAM_NONE,                             \
        0                                       \
    }, /* VLAN0 device port assignment. */      \
    { "vlan0hwname", VLAN_HWNAME,               \
        NVRAM_NONE,                             \
        0                                       \
    }, /* VLAN0 device hwname. */               \

#define NVRAM_EZPACKET_DEFAULT_BRCM_WAN_NUM_1   \
    /* WAN0 always uses vlan1. */               \
    { "vlan1ports", VLAN1_PORTS,                \
        NVRAM_NONE,                             \
        0                                       \
    }, /* VLAN1 device port assignment. */      \
    { "vlan1hwname", VLAN_HWNAME,               \
        NVRAM_NONE                              \
        0                                       \
    }, /* VLAN1 device hwname. */               \


#define NVRAM_EZPACKET_DEFAULT_BRCM_WAN_NUM_2   \
    /* WAN1 uses vlan2. */
    { "vlan2ports", VLAN2_PORTS, 
        NVRAM_NONE,
        0 
    }, /* VLAN2 device port assignment. */
    { "vlan2hwname", VLAN_HWNAME, 
        NVRAM_NONE,
        0 
    }, /* VLAN2 device hwname. */


#endif
