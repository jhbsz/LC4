#ifndef NVRAM_EZPACKET_APP_P_H
#define NVRAM_EZPACKET_APP_P_H
#if (WL_NUM == 0)
    #define APPMGR_WLAN_P     0
#else
    #define APPMGR_WLAN_P     1
#endif
/* prod_cat=P for MR-P series */
#if (EZP_PROD_SUBCAT_P == 0)      /* MR-101N */
    #define APPMGR_ISCSI_P    0
    #define APPMGR_PPTP_P     0
    #define APPMGR_L2TP_P     0
    #define APPMGR_IPSEC_P    0
    #define APPMGR_STORAGE_P  0
    #define APPMGR_WWAN_P     1
    #define APPMGR_WISP_P     1
    #define APPMGR_PORTCFG_P  0
#elif (EZP_PROD_SUBCAT_P == 1)    /* MR-102N */
    #define APPMGR_ISCSI_P    0
    #define APPMGR_PPTP_P     0
    #define APPMGR_L2TP_P     0
    #define APPMGR_IPSEC_P    0
    #define APPMGR_STORAGE_P  0
    #define APPMGR_WWAN_P     1
    #define APPMGR_WISP_P     1
    #define APPMGR_PORTCFG_P  1
#else
    #define APPMGR_ISCSI_P    0
    #define APPMGR_PPTP_P     0
    #define APPMGR_L2TP_P     0
    #define APPMGR_IPSEC_P    0
    #define APPMGR_STORAGE_P  0
    #define APPMGR_WWAN_P     1
    #define APPMGR_WISP_P     0
    #define APPMGR_PORTCFG_P  0
#endif
#define APPMGR_HWNAT_P     0
#define APPMGR_FIRMWARE_P  1
#define APPMGR_IPHONE_P    1
#define APPMGR_BARRY_P     1
#define APPMGR_SMART_P     1
#define APPMGR_USBLED_P    0
#define APPMGR_WIMAX_P     0
#define APPMGR_BECEEM_P    0
#define APPMGR_GUEST_LAN_P     1
#define APPMGR_VPNC_P      1
#define APPMGR_WIRE_P      1
/*
#ifndef APPMGR_WLAN_P
#error "Not defined product cat" EZP_PROD_CAT
#endif
*/

#define APPMBG_RULE_P     APPMGR_WLAN_P^APPMGR_ISCSI_P^APPMGR_PPTP_P^APPMGR_L2TP_P^APPMGR_IPSEC_P^APPMGR_STORAGE_P^APPMGR_WWAN_P^APPMGR_WISP_P^APPMGR_PORTCFG_P^APPMGR_HWNAT_P^APPMGR_FIRMWARE_P^APPMGR_IPHONE_P^APPMGR_BARRY_P^APPMGR_SMART_P^APPMGR_USBLED_P^APPMGR_WIMAX_P^APPMGR_BECEEM_P^APPMGR_GUEST_LAN_P^APPMGR_VPNC_P^APPMGR_WIRE_P
#endif
