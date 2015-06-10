#ifndef NVRAM_EZPACKET_APP_I_H
#define NVRAM_EZPACKET_APP_I_H
#if (WL_NUM == 0)
    #define APPMGR_WLAN_I     0
#else
    #define APPMGR_WLAN_I     1
#endif
/* prod_cat=I for MR-I series */
#if (EZP_PROD_SUBCAT == 2)      /* X-108NX I mode */
    #define APPMGR_ISCSI_I    0
    #define APPMGR_PPTP_I     0
    #define APPMGR_L2TP_I     0
    #define APPMGR_IPSEC_I    0
    #define APPMGR_STORAGE_I  0
    #define APPMGR_WWAN_I     1
    #define APPMGR_WISP_I     1
    #define APPMGR_PORTCFG_I  0
#elif (EZP_PROD_SUBCAT == 3)    /* X-116NX I mode */
    #define APPMGR_ISCSI_I    0
    #define APPMGR_PPTP_I     1
    #define APPMGR_L2TP_I     0
#if defined(EZP_PROD_BRAND_PROX)
    #define APPMGR_IPSEC_I    1
#else
    #define APPMGR_IPSEC_I    0
#endif
    #define APPMGR_STORAGE_I  0
    #define APPMGR_WWAN_I     1
    #define APPMGR_WISP_I     1
    #define APPMGR_PORTCFG_I  0
#else
    #define APPMGR_ISCSI_I    0
    #define APPMGR_PPTP_I     0
    #define APPMGR_L2TP_I     0
    #define APPMGR_IPSEC_I    0
    #define APPMGR_STORAGE_I  0
    #define APPMGR_WWAN_I     1
    #define APPMGR_WISP_I     0
    #define APPMGR_PORTCFG_I  0
#endif
#define APPMGR_HWNAT_I     1
#define APPMGR_FIRMWARE_I  1
#define APPMGR_IPHONE_I    0
#define APPMGR_BARRY_I     0
#define APPMGR_SMART_I     0
#define APPMGR_USBLED_I    0
#define APPMGR_WIMAX_I     0
#define APPMGR_BECEEM_I    0
#define APPMGR_GUEST_LAN_I     1
#define APPMGR_VPNC_I      1
#define APPMGR_WIRE_I      1
/*
#ifndef APPMGR_WLAN_I
#error "Not defined product cat" EZP_PROD_CAT
#endif
*/

#define APPMBG_RULE_I     APPMGR_WLAN_I^APPMGR_ISCSI_I^APPMGR_PPTP_I^APPMGR_L2TP_I^APPMGR_IPSEC_I^APPMGR_STORAGE_I^APPMGR_WWAN_I^APPMGR_WISP_I^APPMGR_PORTCFG_I^APPMGR_HWNAT_I^APPMGR_FIRMWARE_I^APPMGR_IPHONE_I^APPMGR_BARRY_I^APPMGR_SMART_I^APPMGR_USBLED_I^APPMGR_WIMAX_I^APPMGR_BECEEM_I^APPMGR_GUEST_LAN_I^APPMGR_VPNC_I^APPMGR_WIRE_I
#endif
