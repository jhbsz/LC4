#ifndef NVRAM_EZPACKET_APP_G_H
#define NVRAM_EZPACKET_APP_G_H
#if (WL_NUM == 0)
    #define APPMGR_WLAN_G     0
#else
    #define APPMGR_WLAN_G     1
#endif
/* prod_cat=G for PGP-G series */
#if (EZP_PROD_SUBCAT == 2)      /* X-108NX G mode */
    #define APPMGR_ISCSI_G    0
    #define APPMGR_PPTP_G     0
    #define APPMGR_L2TP_G     0
    #define APPMGR_IPSEC_G    0
    #define APPMGR_STORAGE_G  1
    #define APPMGR_WWAN_G     0
    #define APPMGR_WISP_G     1
    #define APPMGR_PORTCFG_G  0
#elif (EZP_PROD_SUBCAT == 3)    /* X-116NX G mode */
    #define APPMGR_ISCSI_G    1
    #define APPMGR_PPTP_G     1
    #define APPMGR_L2TP_G     0
    #define APPMGR_IPSEC_G    0
    #define APPMGR_STORAGE_G  1
    #define APPMGR_WWAN_G     0
    #define APPMGR_WISP_G     1
    #define APPMGR_PORTCFG_G  0
#else
    #define APPMGR_ISCSI_G    0
    #define APPMGR_PPTP_G     0
    #define APPMGR_L2TP_G     0
    #define APPMGR_IPSEC_G    0
    #define APPMGR_STORAGE_G  1
    #define APPMGR_WWAN_G     0
    #define APPMGR_WISP_G     0
    #define APPMGR_PORTCFG_G  0
#endif
#define APPMGR_HWNAT_G     1
#define APPMGR_FIRMWARE_G  1
#define APPMGR_IPHONE_G    0
#define APPMGR_BARRY_G     0
#define APPMGR_SMART_G     0
#define APPMGR_USBLED_G    0
#define APPMGR_WIMAX_G     0
#define APPMGR_BECEEM_G    0
#define APPMGR_GUEST_LAN_G     1
#define APPMGR_VPNC_G      1
#define APPMGR_WIRE_G      1
/*
#ifndef APPMGR_WLAN_G
#error "Not defined product cat" EZP_PROD_CAT
#endif
*/

#define APPMBG_RULE_G     APPMGR_WLAN_G^APPMGR_ISCSI_G^APPMGR_PPTP_G^APPMGR_L2TP_G^APPMGR_IPSEC_G^APPMGR_STORAGE_G^APPMGR_WWAN_G^APPMGR_WISP_G^APPMGR_PORTCFG_G^APPMGR_HWNAT_G^APPMGR_FIRMWARE_G^APPMGR_IPHONE_G^APPMGR_BARRY_G^APPMGR_SMART_G^APPMGR_USBLED_G^APPMGR_WIMAX_G^APPMGR_BECEEM_G^APPMGR_GUEST_LAN_G^APPMGR_VPNC_G^APPMGR_WIRE_G
#endif
