#ifndef NVRAM_EZPACKET_APP_H
#define NVRAM_EZPACKET_APP_H
#if (WL_NUM == 0)
    #define APPMGR_WLAN     0
#else
    #define APPMGR_WLAN     1
#endif
/* prod_cat=A for AF-2160NX   */
#if defined(EZP_PROD_CAT_A)
#if defined(EZP_PROD_BRAND_ZYUS)
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     1
    #define APPMGR_IPSEC    1
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     1
    #define APPMGR_IPSEC    1
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
#if defined(EZP_PROD_SUBSUBCAT_W)
    #define APPMGR_WISP     1
    #define APPMGR_HWNAT    1
#else
    #define APPMGR_WISP     0
    #define APPMGR_HWNAT    0
#endif /* EZP_PROD_SUBSUBCAT_W */
    #define APPMGR_PORTCFG  0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif /* EZP_PROD_BRAND */
    #define APPMGR_GUEST_LAN   0
#endif /* EZP_PROD_CAT_A */
/* prod_cat=C for PGP series */
#if defined(EZP_PROD_CAT_C)
#if (EZP_PROD_SUBCAT == 2)      /* PGP-108N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#elif (EZP_PROD_SUBCAT == 3)    /* PGP-116N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=D for PGP-Dual series */
#ifdef EZP_PROD_CAT_D
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=G for X series */
#ifdef EZP_PROD_CAT_G
#if (EZP_PROD_SUBCAT == 2)      /* X-108NX - Storage mode or PGP-108NX */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#elif (EZP_PROD_SUBCAT == 3)    /* X-116NX - Storage mode or PGP-116NX */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=I for X series */
#ifdef EZP_PROD_CAT_I
#if (EZP_PROD_SUBCAT == 2)      /* X-108NX - 3G mode or MR-108NX */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#elif (EZP_PROD_SUBCAT == 3)    /* X-116NX - 3G mode or MR-116NX */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     0
#if ! defined(EZP_PROD_BRAND_PROX)
    #define APPMGR_IPSEC    0
#else
    #define APPMGR_IPSEC    1
#endif
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=K for Storage-K series */
#ifdef EZP_PROD_CAT_K
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=M for MR series */
#ifdef EZP_PROD_CAT_M
#if (EZP_PROD_SUBCAT == 0)      /* MR-105N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#elif (EZP_PROD_SUBCAT == 2)    /* MR-108N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    1
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=N for MR-216NV */
#ifdef EZP_PROD_CAT_N
#ifdef EZP_PROD_BRAND_ZYUS
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     1
    #define APPMGR_IPSEC    1
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     1
    #define APPMGR_IPSEC    1
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    1
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif /* EZP_PROD_BRAND */
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=O for Planet VRT-402N series */
#ifdef EZP_PROD_CAT_O
#if (EZP_PROD_SUBCAT == 0)      /* VRT-402N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     1
    #define APPMGR_IPSEC    1
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#elif (EZP_PROD_SUBCAT == 4)    /* VRT-420N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     1
    #define APPMGR_L2TP     1
    #define APPMGR_IPSEC    1
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=P for MR-P series */
#ifdef EZP_PROD_CAT_P
#if (EZP_PROD_SUBCAT == 0)      /* MR-101N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#elif (EZP_PROD_SUBCAT == 1)    /* MR-102N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
#if defined(PLATFORM_AXB) || defined(PLATFORM_AXM)
    #define APPMGR_WWAN     0
    #define APPMGR_VPNC     0
    #define APPMGR_WIRE     0
#elif defined(PLATFORM_AXA)
    #define APPMGR_WWAN     1
    #define APPMGR_VPNC     0
#else
    #define APPMGR_WWAN     1
#endif
#ifdef EZP_PROD_BRAND_MYMO
    #define APPMGR_WISP     0
#else
    #define APPMGR_WISP     1
#endif
#if defined(PLATFORM_AME)
    #define APPMGR_PORTCFG  1
#else
    #define APPMGR_PORTCFG  0
#endif
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXB) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    #define APPMGR_GUEST_LAN   0
#else
    #define APPMGR_GUEST_LAN   1
#endif 
#endif
/* prod_cat=R for MR-R series */
#ifdef EZP_PROD_CAT_R
#if (EZP_PROD_SUBCAT == 1)    /* MR-102N */
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   0
#endif
/* prod_cat=T for PGP-T series */
#ifdef EZP_PROD_CAT_T
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  1
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
    #define APPMGR_GUEST_LAN   0
#endif
/* prod_cat=U for MR-105NL series */
#ifdef EZP_PROD_CAT_U
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
#ifdef EZP_PROD_SUBSUBCAT_R
    #define APPMGR_WIMAX    1
    #define APPMGR_BECEEM   1
#else
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=V for VA-402N Malaysia VLAN product */
#ifdef EZP_PROD_CAT_V
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     0
    #define APPMGR_WISP     1
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   0
    #define APPMGR_BARRY    0
    #define APPMGR_SMART    0
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=W for White brand MobileRouter product */
#ifdef EZP_PROD_CAT_W
#if (EZP_PROD_SUBCAT == 0)      /* 101N */
    #define APPMGR_WLAN     1
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#elif (EZP_PROD_SUBCAT == 1)    /* 102N */
    #define APPMGR_WLAN     1
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  1
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#else
    #define APPMGR_WLAN     1
    #define APPMGR_ISCSI    0
    #define APPMGR_PPTP     0
    #define APPMGR_L2TP     0
    #define APPMGR_IPSEC    0
    #define APPMGR_STORAGE  0
    #define APPMGR_WWAN     1
    #define APPMGR_WISP     0
    #define APPMGR_PORTCFG  0
    #define APPMGR_HWNAT    0
    #define APPMGR_IPHONE   1
    #define APPMGR_BARRY    1
    #define APPMGR_SMART    1
    #define APPMGR_WIMAX    0
    #define APPMGR_BECEEM   0
#endif
    #define APPMGR_GUEST_LAN   1
#endif
/* prod_cat=W remove firmware upgrade GUI */
#ifdef EZP_PROD_CAT_W
    #define APPMGR_FIRMWARE 0
#else
    #define APPMGR_FIRMWARE 1
#endif

#define APPMGR_USBLED       0
/*
#ifndef APPMGR_WLAN
#error "Not defined product cat" EZP_PROD_CAT
#endif
*/

#ifndef APPMGR_VPNC
#define APPMGR_VPNC 1
#endif
#ifndef APPMGR_WIRE
#define APPMGR_WIRE 1
#endif
#define APPMBG_RULE     APPMGR_WLAN^APPMGR_ISCSI^APPMGR_PPTP^APPMGR_L2TP^APPMGR_IPSEC^APPMGR_STORAGE^APPMGR_WWAN^APPMGR_WISP^APPMGR_PORTCFG^APPMGR_HWNAT^APPMGR_FIRMWARE^APPMGR_IPHONE^APPMGR_BARRY^APPMGR_SMART^APPMGR_USBLED^APPMGR_WIMAX^APPMGR_BECEEM^APPMGR_GUEST_LAN^APPMGR_VPNC^APPMGR_WIRE
#endif
