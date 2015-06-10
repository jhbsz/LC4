
#define ARCH "Realtek"
#if ( WAN_NUM == 1 ) && ( LAN_NUM == 1 )

#define WAN0_IFNAME "eth0"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "eth0"
#define WAN0_PORTS "4"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "eth1 ra0"
#define LAN0_IFNAMES_WDS "eth1 ra0 ra1 ra2 ra3 ra4"
#define LAN0_HWNAME ""
#define LAN0_DEVICE "eth1"
#define LAN0_PORTS "0 1 2 3"

#define STATS_RULE "^1^60^eth0 eth1 br0 ra0"

#elif ( WAN_NUM == 2 ) && ( LAN_NUM == 1 ) 

#define WAN0_IFNAME "eth0"
#define WAN0_IFNAMES ""
#define WAN0_HWNAME ""
#define WAN0_DEVICE "eth0"
#define WAN0_PORTS "4"

#define WAN1_IFNAME "eth1"
#define WAN1_IFNAMES ""
#define WAN1_HWNAME ""
#define WAN1_DEVICE "eth1"
#define WAN1_PORTS "3"

#define LAN0_IFNAME "br0"
#define LAN0_IFNAMES "eth2 ra0"
#define LAN0_IFNAMES_WDS "eth2 ra0 ra1 ra2 ra3 ra4"
#define LAN0_HWNAME ""
#define LAN0_DEVICE ""
#define LAN0_PORTS "0 1 2"

#define STATS_RULE "^1^60^eth0 eth1 eth2 br0 ra0"

#else

#error "Undefined amount of LAN/WAN interfaces"

#endif

#ifndef WL_NUM
#define WL_NUM 1
#endif

#ifndef WL_VIRTUAL_NUM
#define WL_VIRTUAL_NUM 1
#endif

#ifndef WL_WDS_NUM
#define WL_WDS_NUM 4
#endif

#ifndef WL_APCLI_NUM
#define WL_APCLI_NUM 1
#endif

#if ( WL_NUM == 1 )
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




/* BW_* definition based on ARCH and EZP_PROD_SUBCAT */
/* RTL865X */
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
