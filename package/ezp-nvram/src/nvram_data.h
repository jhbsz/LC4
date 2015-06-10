#ifndef NVARM_DATA_H
#define NVARM_DATA_H
static struct nvram_tuple nvram_ezpacket_default[] = {
    { "bcclient_wan", xstr(BCCLINET_WAN), 
        NVRAM_NONE, 
        0 
    },   
    { "fw_version", xstr(EZP_PROD_VERSION), 
        NVRAM_PROTECTED, 
        0 
    },   
    { "custom_fw_version", xstr(CUSTOM_PROD_VERSION), 
        NVRAM_PROTECTED, 
        0 
    },   
    { "ifup_types", "lan wan", 
        NVRAM_NONE, 
        0 
    },   
#if !defined(EZP_PROD_SUPERCAT_X)
    { "wan_num", xstr(WAN_NUM),     /* How many WAN interfaces. */
        NVRAM_NONE,
        0 
    },   
#endif /* !defined(EZP_PROD_SUPERCAT_X) */
    { "wan_weight_type", "1", 
        NVRAM_NONE,
        0 
    },   /* WAN weight type [0 (manual) |1 (auto) ] */
#if defined EZP_PROD_BRAND_AXIM
#include "nvram_ezpacket-brand_axim.h"
#elif EZP_PROD_BRAND_ABOCOM
#include "nvram_ezpacket-brand_abocom.h"
#elif EZP_PROD_BRAND_ZYUS
#include "nvram_ezpacket-brand_zyxel.h"
#elif EZP_PROD_BRAND_PLANET
#include "nvram_ezpacket-brand_planet.h"
#elif EZP_PROD_BRAND_WHITE
#include "nvram_ezpacket-brand_white.h"
#elif EZP_PROD_BRAND_PROX
#include "nvram_ezpacket-brand_prox.h"
#elif EZP_PROD_BRAND_MYMO
#include "nvram_ezpacket-brand_mymo.h"
#elif EZP_PROD_BRAND_SONY
#include "nvram_ezpacket-brand_sony.h"
#elif EZP_PROD_BRAND_ALLIS
#include "nvram_ezpacket-brand_allis.h"
#else
#error "No define brand"
#endif

#if defined(EZP_PROD_SUPERCAT_X)
    #if defined(EZP_PROD_CAT_P)
    #include "P/nvram_data_P.h"
    #endif
    #if defined(EZP_PROD_CAT_K)
    #include "K/nvram_data_K.h"
    #endif
    #if defined(EZP_PROD_CAT_G)
    #include "G/nvram_data_G.h"
    #endif
    #if defined(EZP_PROD_CAT_I)
    #include "I/nvram_data_I.h"
    #endif
#endif /* defined(EZP_PROD_SUPERCAT_X) */

#if (WAN_NUM >= 1)
    /* WAN0 */
    /* WAN H/W parameters */
#if !defined(EZP_PROD_SUPERCAT_X)
    { "wan0_ifname", WAN0_IFNAME, 
        NVRAM_NONE,
        0 
    }, /* WAN interface name */
    { "wan0_device", WAN0_DEVICE, 
        NVRAM_NONE,
        0 
    }, /* WAN device name backup, reserved for device restoration */
    /* MR routers use wwan as default*/
    { "wan0_proto", WAN0_PROTO, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif /* !defined(EZP_PROD_SUPERCAT_X) */
    { "wan0_ifnames", WAN0_IFNAMES, 
        NVRAM_NONE,
        0 
    },       /* WAN interface names */
    { "wan0_hwname", WAN0_HWNAME, 
        NVRAM_NONE,
        0 
    },        /* WAN driver name (e.g. et1) */
    { "wan0_device_backup", WAN0_DEVICE, 
        NVRAM_NONE,
        0 
    },        /* WAN device name */
    { "wan0_rtmark", "0x10000000", 
        NVRAM_NONE,
        0 
    }, /* WAN device routing mark. */
    { "wan0_ports", WAN0_PORTS, 
        NVRAM_NONE,
        0 
    }, /* WAN device port assignment. */
    { "wan0_bw_mode", "auto",
        NVRAM_NONE,
        0
    }, /* WAN DBM mode: auto/expert */

    /* VPN temporary parameters */
    { "vpn0_ifname", "",
       NVRAM_TEMP,
       0 
    },        /* VPN IP address */
    { "vpn0_ipaddr", "",
       NVRAM_TEMP,
       0 
    },        /* VPN IP address */
    { "vpn0_netmask", "",
       NVRAM_TEMP,
       0 
    },       /* VPN netmask */
    { "vpn0_mask", "", 
       NVRAM_TEMP,
       0 
    },          /* VPN netmask2 */
    { "vpn0_gateway", "",
       NVRAM_TEMP,
       0 
    },       /* WAN gateway */
    { "vpn0_dns", "",
       NVRAM_TEMP,
       0 
    },           /* x.x.x.x x.x.x.x ... */

    /* WAN temporary parameters */
    { "wan0_ipaddr", "",
       NVRAM_TEMP,
       0 
    },        /* WAN IP address */
    { "wan0_netmask", "",
       NVRAM_TEMP,
       0 
    },       /* WAN netmask */
    { "wan0_mask", "", 
       NVRAM_TEMP,
       0 
    },          /* WAN netmask2 */
    { "wan0_gateway", "",
       NVRAM_TEMP,
       0 
    },       /* WAN gateway */
    { "wan0_dns", "",
       NVRAM_TEMP,
       0 
    },           /* x.x.x.x x.x.x.x ... */
    { "wan0_wins", "",
       NVRAM_TEMP,
       0 
    },          /* x.x.x.x x.x.x.x ... */
    { "wan0_failcount", "",
       NVRAM_TEMP,
       0 
    },          /* WAN detection failure count */
    { "wan0_upnp_proto", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP proto */
    { "wan0_upnp_ifname", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP ifname */
#ifdef AXIMDDNS
    { "aximddns_tmp_rule", "0^^0^",
        NVRAM_TEMP,
        0
    }, /* AXIM DDNS temp rule  */
#endif
#endif /* WAN NUM >= 1 */

#if (WAN_NUM >= 2)
    /* WAN1 */
    /* WAN H/W parameters */
#if !defined(EZP_PROD_SUPERCAT_X)
    { "wan1_ifname", WAN1_IFNAME, 
        NVRAM_NONE,
        0 
    }, /* WAN interface name */
    { "wan1_device", WAN1_DEVICE, 
        NVRAM_NONE,
        0 
    }, /* WAN device name backup, reserved for device restoration */
    /* MR routers use wwan as default*/
    { "wan1_proto", WAN1_PROTO, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif /* !defined(EZP_PROD_SUPERCAT_X) */
    { "wan1_ifnames", WAN1_IFNAMES, 
        NVRAM_NONE,
        0 
    },       /* WAN interface names */
    { "wan1_hwname", WAN1_HWNAME, 
        NVRAM_NONE,
        0 
    },        /* WAN driver name (e.g. et1) */
    { "wan1_device_backup", WAN1_DEVICE, 
        NVRAM_NONE,
        0 
    },        /* WAN device name */
    { "wan1_rtmark", "0x20000000", 
        NVRAM_NONE,
        0 
    }, /* WAN device routing mark. */
    { "wan1_ports", WAN1_PORTS,
        NVRAM_NONE,
        0 
    }, /* WAN device port assignment. */
    { "wan1_bw_mode", "auto",
        NVRAM_NONE,
        0
    }, /* WAN DBM mode: auto/expert */

    /* VPN temporary parameters */
    { "vpn1_ifname", "",
       NVRAM_TEMP,
       0 
    },        /* VPN IP address */
    { "vpn1_ipaddr", "",
       NVRAM_TEMP,
       0 
    },        /* VPN IP address */
    { "vpn1_netmask", "",
       NVRAM_TEMP,
       0 
    },       /* VPN netmask */
    { "vpn1_mask", "", 
       NVRAM_TEMP,
       0 
    },          /* VPN netmask2 */
    { "vpn1_gateway", "",
       NVRAM_TEMP,
       0 
    },       /* WAN gateway */
    { "vpn1_dns", "",
       NVRAM_TEMP,
       0 
    },           /* x.x.x.x x.x.x.x ... */

    /* WAN temporary parameters */
    { "wan1_ipaddr", "", 
       NVRAM_TEMP,
        0 
    },        /* WAN IP address */
    { "wan1_netmask", "", 
       NVRAM_TEMP,
        0 
    },       /* WAN netmask */
    { "wan1_mask", "", 
       NVRAM_TEMP,
        0 
    },          /* WAN netmask2 */
    { "wan1_gateway", "", 
       NVRAM_TEMP,
        0 
    },       /* WAN gateway */
    { "wan1_dns", "", 
       NVRAM_TEMP,
        0 
    },           /* x.x.x.x x.x.x.x ... */
    { "wan1_wins", "", 
       NVRAM_TEMP,
        0 
    },          /* x.x.x.x x.x.x.x ... */

    { "wan1_failcount", "",
       NVRAM_TEMP,
       0 
    },          /* WAN detection failure count */

    { "wan1_upnp_proto", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP proto */
    { "wan1_upnp_ifname", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP ifname */
#endif /* WAN NUM >= 2 */
#if (WAN_NUM >= 3)
    /* WAN H/W parameters */
#if !defined(EZP_PROD_SUPERCAT_X)
    { "wan2_ifname", WAN2_IFNAME, 
        NVRAM_NONE,
        0 
    }, /* WAN interface name */
    { "wan2_device", WAN2_DEVICE, 
        NVRAM_NONE,
        0 
    }, /* WAN device name backup, reserved for device restoration */
    /* MR routers use wwan as default*/
    { "wan2_proto", WAN2_PROTO, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif /* !defined(EZP_PROD_SUPERCAT_X) */
    { "wan2_ifnames", WAN2_IFNAMES, 
        NVRAM_NONE,
        0 
    },       /* WAN interface names */
    { "wan2_hwname", WAN2_HWNAME, 
        NVRAM_NONE,
        0 
    },        /* WAN driver name (e.g. et1) */
    { "wan2_device_backup", WAN2_DEVICE, 
        NVRAM_NONE,
        0 
    },        /* WAN device name */
    { "wan2_rtmark", "0x30000000", 
        NVRAM_NONE,
        0 
    }, /* WAN device routing mark. */
    { "wan2_ports", WAN2_PORTS,
        NVRAM_NONE,
        0 
    }, /* WAN device port assignment. */
    { "wan2_bw_mode", "auto",
        NVRAM_NONE,
        0
    }, /* WAN DBM mode: auto/expert */

    /* VPN temporary parameters */
    { "vpn2_ifname", "",
       NVRAM_TEMP,
       0 
    },        /* VPN IP address */
    { "vpn2_ipaddr", "",
       NVRAM_TEMP,
       0 
    },        /* VPN IP address */
    { "vpn2_netmask", "",
       NVRAM_TEMP,
       0 
    },       /* VPN netmask */
    { "vpn2_mask", "", 
       NVRAM_TEMP,
       0 
    },          /* VPN netmask2 */
    { "vpn2_gateway", "",
       NVRAM_TEMP,
       0 
    },       /* WAN gateway */
    { "vpn2_dns", "",
       NVRAM_TEMP,
       0 
    },           /* x.x.x.x x.x.x.x ... */

    /* WAN temporary parameters */
    { "wan2_ipaddr", "", 
       NVRAM_TEMP,
        0 
    },        /* WAN IP address */
    { "wan2_netmask", "", 
       NVRAM_TEMP,
        0 
    },       /* WAN netmask */
    { "wan2_mask", "", 
       NVRAM_TEMP,
        0 
    },          /* WAN netmask2 */
    { "wan2_gateway", "", 
       NVRAM_TEMP,
        0 
    },       /* WAN gateway */
    { "wan2_dns", "", 
       NVRAM_TEMP,
        0 
    },           /* x.x.x.x x.x.x.x ... */
    { "wan2_wins", "", 
       NVRAM_TEMP,
        0 
    },          /* x.x.x.x x.x.x.x ... */

    { "wan2_failcount", "",
       NVRAM_TEMP,
       0 
    },          /* WAN detection failure count */

    { "wan2_upnp_proto", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP proto */
    { "wan2_upnp_ifname", "",
       NVRAM_TEMP,
       0 
    },          /* WAN UPnP ifname */
#endif /* WAN NUM >= 3 */
#if !defined(EZP_PROD_SUPERCAT_X)
    /* WAN main rules */
    { "wan_main_rule", WAN_MAIN_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^upnp^try */
#endif /* !defined(EZP_PROD_SUPERCAT_X) */
#if defined(EZP_SUB_BRAND_GARMIN)
    { "wanupnp_rule", "0",
        NVRAM_NONE,
        0
    }, /* GARMIN disable */
#else
    { "wanupnp_rule", "1",
        NVRAM_NONE,
        0
    },  /* enable */
#endif 
    { "wan_bw_rule", WAN_BW_RULE, 
        NVRAM_NONE,
        0 
    },  /* type^dl^ul^expert_dl^expert_ul^link_percent^global_percent^user_percent^dbm_upmax^dbm_upmin^dbm_downmax^dbm_downmin */
    { "wan_detect_rule", WAN_DETECT_RULE, 
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^threshold^timeout^type[arping|ping]^host_type[gateway|custom]^host */
    /* Load balance */
    { "wan_lb_rule", WAN_LB_RULE, 
        NVRAM_NONE,
        0 
    },   /* type[alwayson|backup|backup_standby]^weight^target^backup^failback[0|1] */
#if defined(PLATFORM_ALS)
    { "storage_device_mount_rule", STORAGE_DEVICE_MOUNT_RULE,
        NVRAM_NONE,
        0
    },  /* uport^mnt */
#else
    { "storage_device_mount_rule", STORAGE_DEVICE_MOUNT_RULE,
        NVRAM_NONE,
        0
    },  /* uport^mnt */
#endif
    { "storage_device_mount_rule_num", STORAGE_DEVICE_MOUNT_RULE_NUM,
        NVRAM_NONE,
        0
    },
#if defined(EZP_PROD_BRAND_SONY)
    { "countdown_rule", "1^10^10^0^60^60",
#else
    { "countdown_rule", "0^60^60^0^60^60",
#endif
        NVRAM_NONE,
        0
    },  /* state^w_countdown^w_count^p_countdown^p_count */
    { "license_status_rule", "0^0",
        NVRAM_TEMP,
        0
    },  /* state^trycount */
    { "wan_status_rule", WAN_STATUS_RULE,
        NVRAM_TEMP,
        0
    },  /* state^trycount^action[none|up|standby|down]^wisp_trycount(M1) */
    { "vpn_status_rule", VPN_STATUS_RULE,
        NVRAM_TEMP,
        0
    },
    { "mac_sense_rule", MAC_SENSE_RULE,
	NVRAM_NONE,
	0
    },  /* state^trycount^action[none|up|standby|down] */
#if defined(PLATFORM_ALS)    
    { "file_mode_rule", FILE_MODE_RULE,
        NVRAM_NONE,
        0
    },
    { "relay_schedule_rule", RELAY_SCHEDULE_RULE,
        NVRAM_NONE,
        0
    },
    { "relay_rule", RELAY_RULE,
        NVRAM_NONE,
        0
    },
    { "mail_rule", MAIL_RULE,
        NVRAM_NONE,
        0
    },
    { "als_failover", FAILOVER_RULE,
        NVRAM_NONE,
        0
    },
    { "alert_rule", ALERT_RULE,
        NVRAM_TEMP,
        0
    },
#endif
    { "wan_upnp_status_rule", WAN_UPNP_STATUS_RULE,
        NVRAM_TEMP,
        0
    },  /* state^action[none|up|down] */
    /* For some style of wan, we needs to wait it completion to next round. */
    { "wan_action_style_rule", "bg^bg|bg^bg",
        NVRAM_TEMP,
        0
    },  /* upfb^downfb */
    /* WAN proto parameters rules */
    { "wan_static_rule", WAN_STATIC_RULE, 
        NVRAM_NONE,
        0 
    },  /* ipaddr^mask^gateway^dns1^dns2^wins */
    { "wan_dhcp_rule", WAN_DHCP_RULE, 
        NVRAM_NONE,
        0 
    },   /* hostname^domain^lease*/
    { "wan_pppoe_rule", WAN_PPPOE_RULE, 
        NVRAM_NONE,
        0 
    },  /* auth^username^passwd^demand^idletime^echo_failure^redialperiod^
         * mru^mtu 
         */
    { "wan_wwan_rule", WAN_WWAN_RULE, 
        NVRAM_NONE,
        0 
    },  /* apn_type^location^isp^apn^pin^mode^auth^username^passwd^demand^
         * idletime^redialperiod^mru^mtu^dialstr^brand^model^numtty^datatty^
         * ctrltty^devname^flag^turbolink
         */
    { "wan_barry_rule", WAN_BARRY_RULE, 
        NVRAM_NONE,
        0 
    },  /* barrypasswd^apn_type^location^isp^apn^dialstr^turbolink */
#if defined(PLATFORM_M2E)
    /* wan_pptp_l2tp_rule_num */
    { "wan_pptp_l2tp_rule_num", WAN_PPTP_L2TP_RULE_NUM,
        NVRAM_NONE,
        0
    },
    /* wan_pptp_l2tp_rule_max */
    { "wan_pptp_l2tp_rule_max", WAN_PPTP_L2TP_RULE_MAX,
        NVRAM_NONE,
        0
    },
    { "wan_pptp_l2tp_global_rule", "0^0^0",
        NVRAM_NONE,
        0
    }, /* enable[0:disabled|1:enabled]^selected(selected profile index)^enabled(enabled profile index) */
    { "wan_pptp_l2tp_status", "Disabled",
        NVRAM_TEMP,
        0
    }, /* [disabled|connecting|connected|serverfail|authfail|disconn|disconned]*/
#endif
    /* wan_pptp_l2tp_rule */
    { "wan_pptp_l2tp_rule", WAN_PPTP_L2TP_RULE,
        NVRAM_NONE,
        0
    }, /* enable^name^vpn_type^host^ipaddr^netmask^gateway^username^
        * passwd^nailup^timeout^pppiptype[isp|custom]^custom_ipaddr^
        * encmode[disabled|mppe]^auth[chap|pap|none] */
    { "wan_l2tp_status_rule", "1",
        NVRAM_TEMP,
        0
    },  /* first_start */
    { "wan_htc_rule", WAN_HTC_RULE, 
        NVRAM_NONE,
        0 
    },   /* hostname^domain^lease^mtu^turbolink */
    { "wan_iphone_rule", WAN_IPHONE_RULE, 
        NVRAM_NONE,
        0 
    },   /* hostname^domain^lease^mtu^turbolink */
    { "wan_beceem_rule", WAN_BECEEM_RULE, 
        NVRAM_NONE,
        0 
    },   /* isp^username^password */
    { "wan_wimax_rule", WAN_WIMAX_RULE, 
        NVRAM_NONE,
        0 
    },   /* mtu^turbolink */
    { "wan_wwan_probe_rule", WAN_WWAN_PROBE_RULE, 
        NVRAM_TEMP,
        0 
    },  /* brand^model^signal^ttynum^datatty^ctrltty^devname^flag^sig^imei^provider^firmware^apn^vendor^product^stayfg */
    /* Added for ac340u relate control action flag */
    {
      "ac340st", "unplug",
        NVRAM_TEMP,
        0 
    },
    { "wan_bigpond_rule", WAN_BIGPOND_RULE,
        NVRAM_NONE,
        0 
    },  /* enable^server^username^passwd */
    /* WAN hwaddr */ 
    { "wan_hwaddr_rule_default", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0
    },  /* hwaddr */
    /* WAN hwaddr clone */ 
    { "wan_hwaddr_clone_rule", WAN_HWADDR_CLONE_RULE, 
        NVRAM_NONE,
        0
    },  /* enable[0|1]^hwaddr */
    /* DDNS */
    { "wan_ddns_rule", WAN_DDNS_RULE, 
        NVRAM_NONE,
        0 
    },  /* host^ipaddr */
    /* apple filter */
    { "wan_apple_filter_rule", WAN_APPLE_FILTER_RULE,
        NVRAM_NONE,
        0
    },  /* name^enable[0|1]^type[dyndns|tzo|zoneedit]^username^
           passwd^hostname */
    /* OpenDNS */
    { "wan_opendns_rule", WAN_OPENDNS_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^username^passwd^redirect[0|1]^label */
    { "lan_num", xstr(LAN_NUM), 
        NVRAM_NONE,
        0 
    },  /* How many LAN interfaces are available.*/
#if LAN_NUM >= 1
    /* LAN0 */
    /* LAN H/W parameters */

    { "lan0_ifname", LAN0_IFNAME, 
        NVRAM_NONE,
        0 
    },  /* LAN interface name */
    { "lan0_ifnames", LAN0_IFNAMES, 
        NVRAM_NONE,
        0 
    },  /* LAN interface names */
    { "lan0_hwname", LAN0_HWNAME, 
        NVRAM_NONE,
        0 
    },  /* LAN driver name (e.g. et1) */
    { "lan0_device", LAN0_DEVICE, 
        NVRAM_NONE,
        0 
    },  /* LAN device name */
    { "lan0_proto", "static", 
        NVRAM_PROTECTED,
        0 
    },  /* LAN device name */
    { "lan0_ports", LAN0_PORTS, 
        NVRAM_NONE,
        0 
    }, /* LAN device port assignment. */

    /* LAN temporary parameters */
    { "lan0_ipaddr", "", 
       NVRAM_TEMP,
        0 
    },        /* LAN IP address */
    { "lan0_netmask", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN netmask */
    { "lan0_mask", "", 
       NVRAM_TEMP,
        0 
    },          /* LAN netmask2 */
    { "lan0_gateway", "", 
       NVRAM_TEMP,
        0 
    },       /* LAN gateway */
    { "lan0_dns", "", 
       NVRAM_TEMP,
        0 
    },           /* x.x.x.x x.x.x.x ... */
    { "lan0_wins", "", 
       NVRAM_TEMP,
        0 
    },          /* x.x.x.x x.x.x.x ... */
#ifdef CONFIG_EZP_ARCH_RT305X
    { "vlan_rule", VLAN_RULE,
       NVRAM_NONE,
        0 
    },      /* name^enable[0|1]^ifname^vid^portmember^tag^prio[0|1|2|3]:1 */
    { "vlan_rule_num", xstr(VLAN_RULE_NUM),
       NVRAM_NONE,
        0 
    },      
    { "vlan_rule_max", xstr(VLAN_RULE_MAX),
       NVRAM_NONE,
        0 
    },
    { "vlanport_rule", VLANPORT_RULE,
       NVRAM_NONE,
        0 
    },       /* name^portid^pvid^tag^mode^gmemb^prio[0|1|2|3]:1 */
    { "vlanport_rule_num", xstr(VLANPORT_RULE_NUM),
       NVRAM_NONE,
        0 
    },      
    { "vlanport_rule_max", xstr(VLANPORT_RULE_MAX),
       NVRAM_NONE,
        0 
    },
#if !defined(EZP_PROD_SUPERCAT_X)
    { "br_rule", BR_RULE,
       NVRAM_NONE,
        0
    },      /* name^enable[0|1]^ifnames^flag */
#endif /* !defined(EZP_PROD_SUPERCAT_X) */
    { "br_rule_num", xstr(BR_RULE_NUM),
       NVRAM_NONE,
        0
    },
    { "br_rule_max", xstr(BR_RULE_MAX),
       NVRAM_NONE,
        0
    },
    NVRAM_EZPACKET_DEFAULT_RT305X_LAN_NUM_1
    NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_1
#if (WAN_NUM == 2)
    NVRAM_EZPACKET_DEFAULT_RT305X_WAN_NUM_2
#endif /* (WAN_NUM == 2) */
#endif /* CONFIG_EZP_ARCH_RT305X */

#ifdef CONFIG_EZP_ARCH_BRCM
    NVRAM_EZPACKET_DEFAULT_BRCM_LAN_NUM_1
    NVRAM_EZPACKET_DEFAULT_BRCM_WAN_NUM_1
#if (WAN_NUM == 2)
    NVRAM_EZPACKET_DEFAULT_BRCM_WAN_NUM_2
#endif /* (WAN_NUM == 2) */
#endif /* CONFIG_EZP_ARCH_BRCM */
#endif

#ifdef CONFIG_EZP_ARCH_AS500
    NVRAM_EZPACKET_DEFAULT_AS500_LAN_NUM_1
    NVRAM_EZPACKET_DEFAULT_AS500_WAN_NUM_1
#if (WAN_NUM == 2)
    NVRAM_EZPACKET_DEFAULT_AS500_WAN_NUM_2
#endif /* (WAN_NUM == 2) */
    NVRAM_EZPACKET_DEFAULT_AS500_SWAP
    NVRAM_EZPACKET_DEFAULT_AS500_INITD
#endif /* CONFIG_EZP_ARCH_AS500 */

    { "lan_main_rule", LAN_MAIN_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^weight */
    { "lan_hwaddr_rule_default", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0
    },  /* hwaddr */
    { "lan_static_rule_default", LAN_STATIC_RULE_DEFAULT, 
        NVRAM_NONE,
        0 
    },  /* enable^ipaddr^mask */
    { "guest_lan_rule", GUEST_LAN_RULE, 
        NVRAM_NONE,
        0 
    },  /* ipaddr^mask^gateway^dns^wins */
    { "lan_static_rule", "", 
        NVRAM_DEFAULT,
        0 
    },  /* ipaddr^mask^gateway^dns^wins */
    { "lan_status_rule", LAN_STATUS_RULE,
        NVRAM_TEMP,
        0
    },  /* state^active^trycount */
    /* DHCP server parameters */
    { "lan_dhcps_rule", LAN_DHCPS_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^start^num^lease^domain^wins^
           dnstype^dnsaddr1^dnsaddr2^dnsaddr3^dnsaddr4 */
    /* LAN hwaddr clone */
    { "lan_hwaddr_clone_rule", LAN_HWADDR_CLONE_RULE, 
        NVRAM_NONE,
        0
    },  /* enable[0|1]^hwaddr[XX:XX:XX:XX:XX:XX] */

    /* MASK */
    { "rt_mask", "0xF0000000", 
        NVRAM_NONE,
        0 },      /* Routing mask: 4-bit */
    { "auth_mask", "0x0F000000",
        NVRAM_NONE,
        0 },      /* Auth mask: 4-bit,
#define DEFAULT_FW_MARK_AUTHENTICATED   0x04000000
#define DEFAULT_FW_MARK_TRUSTED         0x02000000
#define DEFAULT_FW_MARK_BLOCKED         0x01000000
                   */
    { "dbm_max", DBM_MAX,
        NVRAM_NONE,
        0 },
    { "dbm_mask", "0x00FF0000", 
        NVRAM_NONE,
        0 },      /* DBM mask: 8-bit */
    { "fl_mask", "0x0000FFFF", 
        NVRAM_NONE,
        0 },      /* FL mask: 16-bit */
    { "fl_mark_default", "0x0000FFFF", 
        NVRAM_NONE,
        0 },      /* FL mask: 16-bit */
    /* nat rule */
#if  defined(EZP_PROD_BRAND_ZYUS)
    { "nat_rule", "1", 
#else
    { "nat_rule", "0", 
#endif
        NVRAM_NONE,
        0 
    }, /* hw_enable */
    /* turboNAT */
#if defined(EZP_PROD_BRAND_ZYUS) || !defined(TURBONAT)
    { "turbonat_enable", "0", 
#else
    { "turbonat_enable", "1", 
#endif
        NVRAM_NONE,
        0 
    },   /* enable [0|1] */
    /* SNMP */

#if defined(EZP_PROD_BRAND_PROX)
#ifdef EZP_SUB_BRAND_APOLLO
    { "snmp_rule", "1^161^161^public^^RR-WC310^^^10^0",
#else
    { "snmp_rule", "1^161^161^public^Proxicast Inc.^LAN-Cell 3^^^10^0",
#endif
#elif defined(EZP_PROD_BRAND_MYMO)
    { "snmp_rule", "1^161^161^public^MyMO Inc.^MyMO^^^10^0",
#elif defined(EZP_SUB_BRAND_RFLINK)
    { "snmp_rule", "0^161^161^public^RF-LINK Inc.^RF-LINK^^^10^0",
#else
    { "snmp_rule", "0^161^161^public^AXIMCom Inc.^AXIMCom Product^^^10^0",
#endif        
        NVRAM_NONE,
        0 
    },/* enable[0|1]^udpport^tcpport^community^vendor^sysname^location^contact^timeout^auth */
    /* Session Manager */
    { "sm_mode", "fast", 
        NVRAM_NONE,
        0 
    },   /* mode [fast|regular|slow] */
    /* Bandwidth */
    { "bw_enable", BW_ENABLE, 
        NVRAM_NONE,
        0 
    },   /* BW [0|1] */
    /* SBM */
    { "bw_sbm_mask", "0x00FF0000", 
        NVRAM_NONE,
        0 
    },   /* BW mask */
#define BW_GROUP_RULE "group1^1^10^100^|group2^1^10^100^|group3^1^10^100^|group4^1^10^100^"
#define BW_GROUP_RULE_NUM 4
#define BW_GROUP_RULE_MAX 4
    { "bw_group_rule", BW_GROUP_RULE, 
        NVRAM_NONE,
        0 
    }, /* name^enable^bw_uprate^bw_upceil^prio */
    { "bw_group_rule_num", xstr(BW_GROUP_RULE_NUM), 
        NVRAM_NONE,
        0 
    },
    { "bw_group_rule_max", xstr(BW_GROUP_RULE_MAX),
        NVRAM_NONE,
        0 
    },
    { "bw_sbm_rule", BW_SBM_RULE, 
        NVRAM_NONE,
        0 
    }, /* name^enable^mode^inipaddr_start^inipaddr_end^protocol^port_start^
          port_end^extif^bw_alloc^bw_ratio^bw_upload^bw_download^
          sched_idx^excess^excess_ratio^excess_upload^excess_download */
    { "bw_sbm_rule_num", BW_SBM_RULE_NUM, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
    { "bw_sbm_rule_max", BW_SBM_RULE_MAX, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
    { "bw_dbm_mask", "0x00FF0000", 
        NVRAM_NONE,
        0 
    },   /* BW mask */
    { "bw_dbm_rule", BW_DBM_RULE, 
        NVRAM_NONE,
        0 
    }, /* name^enable^mode^inipaddr_start^inipaddr_end^sched_idx */
    { "bw_dbm_rule_num", BW_DBM_RULE_NUM, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
    { "bw_dbm_rule_max", BW_DBM_RULE_MAX, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing BW rules */
    { "bw_mask", "0xFFFF0000", 
        NVRAM_NONE,
        0 
    },   /* BW mask */
    /* Policy-based routing */
    { "rt_enable", "1", 
        NVRAM_NONE,
        0 
    },   /* Routing [0|1] */
    { "rt_rule", RT_RULE, 
        NVRAM_EMPTY,
        0 
    }, /* name^enable[0|1]^srcipaddr_start^srcipaddr_end^dstipaddr_start^
          dstipaddr_end^proto^srcport_start^srcport_end^dstport_start^
          dstport_end^extif^routing_type[default|static]^gateway_ipaddr */
#if defined(EZP_PROD_BRAND_PROX)
    { "rt_rule_num", "0", 
#else
    { "rt_rule_num", "1", 
#endif
        NVRAM_NONE,
        0 
    }, /* The amount of existing RT rules */
    { "rt_rule_max", RT_RULE_MAX, 
        NVRAM_NONE,
        0 
    }, /* The amount of existing RT rules */

    /*  routing */
    { "rt_conn_enable", "1", 
        NVRAM_NONE,
        0 
    },   /* Routing cache [0|1] */
    /* web url/content filtering rule */
    { "wf_enable", "0",
        NVRAM_EMPTY,
        0
    }, /* web url filtering [0|1] */
    { "wf_content_rule", "0^0^0",
        NVRAM_EMPTY,
        0
    }, /* activex_enable^java_enable^proxy_enable */

#if defined(EZP_PROD_BRAND_PROX) || defined(PLATFORM_ALS)
    { "wf_rule", "",
#else
    { "wf_rule", "0^facebook.com^host^0|0^twitter.com^host^0|0^myspace^host^0|0^hi5.com^host^0",
#endif
        NVRAM_EMPTY,
        0
    }, /* enable^keyword^type^access */

#if defined(EZP_PROD_BRAND_PROX)
    { "wf_rule_num", "0",
#else
    { "wf_rule_num", "4",
#endif
        NVRAM_EMPTY,
        0
    },
    { "wf_rule_max", WF_RULE_MAX,
        NVRAM_EMPTY,
        0
    },
    /* Schedule rule */
    { "sched_rule", "", 
        NVRAM_NONE,
        0 
    },   /* name^enable[0|1]^used^start^stop^days */
    { "sched_rule_num", "0", 
        NVRAM_NONE,
        0 
    },       /* The amount of existing sched rules */
    { "sched_rule_max", SCHED_RULE_MAX, 
        NVRAM_NONE,
        0 
    },       /* The amount of existing sched rules */

    /* RRDCOLLECT */
    { "rrd_enable", "0", 
        NVRAM_NONE,
        0 },    /* RRDcollect client */

    /* Cron */
    { "cron_rule", "1^1^02 4 * * *^^^-1^", 
        NVRAM_NONE,
        0 
    }, /* enable^ntp_enable[0|1]^ntp_interval^reboot_ebable[0|1]^reboot_interval^reboot_day^reboot_hour^reboot_min */

    /* NTP */
#if defined(EZP_PROD_BRAND_PROX)
    { "ntp_rule", "^1^pool^north-america.pool.ntp.org^^GMT0BST,M3.5.0/2,M10.5.0/2^1",
#elif defined(EZP_PROD_BRAND_SONY)
    { "ntp_rule", "^1^pool^north-america.pool.ntp.org^^UTC0^1",
#elif defined(PLATFORM_AXA) || defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
    { "ntp_rule", "^1^pool^north-america.pool.ntp.org^^UTC0^0",
#else
    { "ntp_rule", "^1^pool^north-america.pool.ntp.org^^PST8PDT,M3.2.0/2,M11.1.0/2^1",
#endif
        NVRAM_NONE,
        0 
    }, /* name^enable[0|1]^type^pool^ipaddr^zone^daylight */
    /* SAMBA */
#if defined(EZP_PROD_BRAND_SONY)
    { "smbd_rule", "1^WG-C10^WG-C10^WG-C10^0777^0777^^share^mnt^^0^1^0^cp437", 
        NVRAM_NONE,
        0 
    }, /* enable[0|1]^name^workgroup^description^create_mask^dir_mask^security^share_name^share_path^share_users^read_only^guest_ok^ext_acces^codepage */
#elif defined(PLATFORM_M2E) 
    { "smbd_rule", "0^QGenie^QGenie^^0777^0777^^share^mnt^^0^0^1^cp437",
        NVRAM_NONE,
        0
    }, /* enable[0|1]^name^workgroup^description^create_mask^dir_mask^security^share_name^share_path^share_users^read_only^guest_ok^ext_acces^codepage */
#elif defined(PLATFORM_AXA) 
    { "smbd_rule", "0^togobox^AXIMCOM^^0777^0777^^share^mnt^^0^1^0^cp437",
        NVRAM_NONE,
        0
    }, /* enable[0|1]^name^workgroup^description^create_mask^dir_mask^security^share_name^share_path^share_users^read_only^guest_ok^ext_acces^codepage */
#else
    { "smbd_rule", "0^AXIMCOM^AXIMCOM^^0777^0777^^share^mnt^^0^1^0^cp437", 
        NVRAM_NONE,
        0 
    }, /* enable[0|1]^name^workgroup^description^create_mask^dir_mask^security^share_name^share_path^share_users^read_only^guest_ok^ext_acces^codepage */
#endif
    { "smbd_user_rule", "1^admin^admin", 
        NVRAM_NONE,
        0 
    }, /* enable[0|1]^name^port^format^auth_enable[0|1]^user^passwd */
    { "togo_rule_num", "0", 
        NVRAM_TEMP,
        0 
    }, /* num */
    { "togo_rule", "", 
        NVRAM_TEMP,
        0 
    }, /* status^name^proto^ip^port^mac^config^resolution */
    { "camera_rule", "1^8082^^1^admin^admin^0^1280x720", 
        NVRAM_NONE,
        0 
    }, /* status */
    { "music_rule", "1", 
        NVRAM_NONE,
        0 
    }, /* enable[0|1]^port */
#if defined(EZP_PROD_BRAND_MYMO)
    { "drive_rule", "1^0^80^1^admin^admin",
#elif defined(PLATFORM_AXA) 
    { "drive_rule", "1^1^8081^1^admin^admin^0",
#elif defined(PLATFORM_M2E) 
    { "drive_rule", "1^0^8081^1^admin^admin^0",
#else
    { "drive_rule", "1^0^8081^1^admin^admin^0", 
#endif
        NVRAM_NONE,
        0 
    }, /* enable[0|1]^guset[0|1]^port^ext_access[0|1]^name^password^0 */
    /* BT */
    { "bt_rule", "0^50^250^10^100^10^download!temp^download!session^download^51001^51002^51000^no",
        NVRAM_NONE,
        0 
    }, /* enable[0|1]^peer^max_peer^max_task^download_rate^upload_rate^directory^session^load_start^port_range_start^port_range_end^dht_port^check_hash */
    { "bt_torrent_max", BT_TORRENT_MAX,
        NVRAM_NONE,
        0
    }, /* Max torrent number */
    /* FTP */
    { "ftpd_rule", FTPD_RULE,
        NVRAM_NONE,
        0 
    }, /*enable[0|1]^port^noanonyous^chrooteveryone^maxclientsperip^maxclientnumber^peruserlimits^umask^userbandwidth^timeout^allow_ext_access */
    { "ftpd_user_rule", FTPD_USER_RULE,
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^username^passwd^upload^download^ipaddr */
    { "ftpd_user_rule_num", FTPD_USER_RULE_NUM,
        NVRAM_NONE,
        0 
    },  /* number of ftp user */
    { "ftpd_user_rule_max", FTPD_USER_RULE_MAX,
        NVRAM_NONE,
        0 
    },  /* max ftp user */
    /* iTunes */
    { "itunes_rule", "0^3689^AXIMCom^mp3^AXIMCom iTunes^nobody^^.mp3,.m4a,.m4p",
        NVRAM_NONE,
        0
    },  /* enable[0|1]^port^adminpasswd^mp3dir^servername^runas^password^extensions */    
    /*Storage rule */
    { "storage_rule", "950^16384^4096^0^^^^^",
        NVRAM_NONE,
        0 
    }, /*  codepage^swap_size^block_size^ext_enable^target_ip^target_dir^user^pw^gp */
    /*Storage state rule */
#if defined (PLATFORM_M2E)
    { "storage_state_rule_num", "3",
        NVRAM_NONE,
        0 
    },
    { "storage_state_rule_max", "3",
        NVRAM_NONE,
        0 
    },
    { "storage_state_rule", "mnt^^^|mnt^^^|mnt^^^",
        NVRAM_TEMP,
        0 
    },
    { "storage_used_state_rule", "mnt^0^0^0^0|mnt^0^0^0^0|mnt^0^0^0^0",
        NVRAM_TEMP,
        0 
    },
    { "myqnapcloud_id_rule", "^^^^^^^^^^^",
        NVRAM_NONE,
        0 
    },
    { "model_name", "QG-103N",
        NVRAM_TEMP,
        0 
    },
#ifdef QFILE_CGI 
    { "cgi_progress", "^^^^^^^|^^^^^^^|^^^^^^^|^^^^^^^|^^^^^^^|^^^^^^^|^^^^^^^|^^^^^^^|^^^^^^^|^^^^^^^",
        NVRAM_NONE,
        0 
    },
    { "cgi_progress_num", "10",
        NVRAM_NONE,
        0 
    },
#endif
#else
    { "storage_state_rule_num", "2",
        NVRAM_NONE,
        0 
    },
    { "storage_state_rule_max", "2",
        NVRAM_NONE,
        0 
    },
    { "storage_state_rule", "mnt^^^|mnt^^^",
        NVRAM_TEMP,
        0 
    }, 
    { "storage_used_state_rule", "mnt^0^0^0^0|mnt^0^0^0^0",
        NVRAM_TEMP,
        0 
    },

    /*  dev_name^format^mount_state^insert_state */
    /* iSCSI Target Rule */
#endif
    { "is_rule", IS_RULE,
        NVRAM_NONE,
        0 
    },  /* enable^isns_serv^isid^alias^max_conn^max_tx^max_rx */

    { "ishost_enable", "1", 
        NVRAM_NONE,
        0 
    },  /* ishost_enable [0|1] */

    { "ishost_rule", "",
        NVRAM_EMPTY,
        0 
    },  /* enable^host^action */
    { "ishost_rule_num", "0", 
        NVRAM_NONE,
        0 
    },  /* ishost_rule_num */
    { "ishost_rule_max", ISHOST_RULE_MAX, 
        NVRAM_NONE,
        0 
    },  /* ishost_rule_max */

    { "isuser_enable", "1", 
        NVRAM_NONE,
        0 
    },  /* isuser_enable [0|1] */

    { "isuser_rule", "0^admin^admin012345789",
        NVRAM_EMPTY,
        0 
    },  /* enable^username^password */
    { "isuser_rule_num", "1", 
        NVRAM_NONE,
        0 
    },  /* isuser_rule_num */
    { "isuser_rule_max", ISUSER_RULE_MAX, 
        NVRAM_NONE,
        0 
    },  /* isuser_rule_max */

#if !defined(EZP_PROD_SUPERCAT_X)
    /* Application control */
    { "appmgr_rule", xstr(APPMBG_RULE), 
        NVRAM_NONE,
        0 
    },  /* wlan^iscsi^pptp^l2tl^ipsec^storage^wwan^wisp^portcfg^hwnat^firmware^
           iphone^barry^smart^usbled^wimax */
#endif /* !defined(EZP_PROD_SUPERCAT_X) */

    /* SSH */
    { "ssh_rule", "^0^8123", 
        NVRAM_NONE,
        0 
    },       /* name^enable[0|1]^port */

#define LOG_RULE "syslog^1^^0^1^1^1^1^1^1^1^1^1^1^1^0^^udp^514"
    /* Log Index */
    { "log_rule", LOG_RULE,
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^ipaddr^ipt_enable^web_mgmt^dns^ppp^
           upnp^wireless^ntp^sys_warning^dhcp_serv^dhcp_cli^
           ddns^firewall^tcp_enable[0|1]^tcp_ip^tcp_port */

    /* uPnP */
#if defined(EZP_PROD_BRAND_PROX)
    { "upnp_rule", "^0^0^wan0^lan0^49152",
        NVRAM_NONE,
        0 
    },
#else
    { "upnp_rule", "^1^0^wan0^lan0^49152",
        NVRAM_NONE,
        0
    },
#endif
    /* name^enable[0|1]^pmp_enable[0|1]^extif^inif^port */

    /* Filters */
    { "fl_enable", "1", 
        NVRAM_NONE,
        0 
    },    /* fl_enable [0|1] */
    { "fl_rule_default", "1", 
        NVRAM_NONE,
        0 
    },
    { "fl_rule", FL_RULE, 
        NVRAM_EMPTY,
        0 
    },    /* name^enable[0|1]^extif^inif^proto^srcip^srcmask^
                                dstip^dstmask^srcport^dstport^
                                action[0(DROP)|1(ACCEPT)]^mark^bw_idx^
                                sched_idx */

    { "fl_rule_num", FL_RULE_NUM, 
        NVRAM_NONE,
        0 
    },      /* The amount of existing log rules */
    { "fl_rule_max", FL_RULE_MAX, 
        NVRAM_NONE,
        0 
    },

    { "fl_hwaddr_enable", "1", 
        NVRAM_NONE,
        0 
    },    /* fl_hwaddr_enable [0|1] */
#if defined(PLATFORM_ALS)
    { "fl_hwaddr_rule", "",
        NVRAM_NONE,
        0
    },    /* name^acl_enable[0|1]^dhcp_enable^arp_enable^hwaddr^
             action[0(DROP)|1(ACCEPT)]^ipaddr */
#else
    { "fl_hwaddr_rule", "ARM-Fixed-IP^00:05:4F:40:01:00^1^1^1^1^10.199.221.253", 
        NVRAM_NONE,
        0 
    },    /* name^acl_enable[0|1]^dhcp_enable^arp_enable^hwaddr^
             action[0(DROP)|1(ACCEPT)]^ipaddr */
#endif
    { "fl_hwaddr_rule_num", "1", 
        NVRAM_NONE,
        0 
    }, /* The amount of existing fl_hwaddr rules */
    { "fl_hwaddr_rule_max", FL_HWADDR_RULE_MAX, 
        NVRAM_NONE,
        0 
    },
    { "fl_hwaddr_rule_default", "1", 
        NVRAM_NONE,
        0 
    },
    { "plug_rule", "0^0^0",
        NVRAM_TEMP,
        0
    },  /*
         * Ethernet:0 unplug 1: plug
         * USB     :0 unplug 1: plug
         * Policy : when plug rule is 0, we show unplug, if plug rule is 1, we check cnnt_rule proto attribute
         */
    { "survey_counter", "0",
        NVRAM_TEMP,
        0
    }, /* cnnt_enable [0|1] */
    { "cnnt_rule", CNNT_RULE,
        NVRAM_NONE,
        0
    },  /* */
    { "cnnt_rule_num", CNNT_RULE_NUM,
        NVRAM_NONE,
        0
    }, /* cnnt_rule_num */
    { "cnnt_rule_max", CNNT_RULE_MAX,
        NVRAM_NONE,
        0
    }, 
    /* Port forwards */
    { "fr_enable", "1", 
        NVRAM_NONE,
        0 
    },  /* plug_enable [0|1] */
    { "fr_rule", FR_RULE, 
        NVRAM_EMPTY,
        0 
    },  /* name^enable[0|1]^extif^proto[tcp|udp|both]^extport_start^extport_end^ipaddr^inport_start^inport_end */

#if defined(EZP_PROD_BRAND_PROX)
#ifdef EZP_SUB_BRAND_APOLLO
    { "fr_rule_num", "8", 
#else
    { "fr_rule_num", "0", 
#endif
#else
    { "fr_rule_num", "8", 
#endif
        NVRAM_NONE,
        0 
    },      /* fr_rule_num */
    { "fr_rule_max", FR_RULE_MAX, 
        NVRAM_NONE,
        0 
    },      /* fr_rule_max */
    { "fr_dmz_rule", FR_DMZ_RULE, 
        NVRAM_NONE,
        0 
    },      /* name^enable[0|1]^ipaddr */
    { "wizard_rule", WIZARD_RULE,
        NVRAM_NONE,
        0
    }, /* set_wizard^admin^mode^wan_trad^wan_wwan^bw^time^^wifi */
    /* Virtual servers */

#if defined(EZP_PROD_BRAND_PROX)
    { "vs_enable", "0", 
#else
    { "vs_enable", "1", 
#endif
        NVRAM_NONE,
        0 
    },  /* fr_enable [0|1] */
    { "vs_rule", "", 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^extif^index^wan_ipaddr^mapped_ipaddr */
    { "vs_rule_num", "0", 
        NVRAM_NONE,
        0 
    },      /* vs_rule_num */
    { "vs_rule_max", VS_RULE_MAX, 
        NVRAM_NONE,
        0 
    },      /* vs_rule_max */
    /* Wireless interface */
    { "wl_num", xstr(WL_NUM), 
        NVRAM_NONE,
        0 
    },      /* Enable (1) or disable (0) radio */
    /* Wireless parameters */
    { "wl_region_default", "0", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },      /* Region code default */
    { "wl_region", "", 
        NVRAM_DEFAULT,
        0 
    },      /* Region code */
    { "wl_countrycode_default", WL_COUNTRY_CODE, 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },      /* Region code */
    { "wl_countrycode", "", 
        NVRAM_DEFAULT,
        0 
    },      /* Region code */
    { "wl_regionaband_default", WL_REGION_ABAND, 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },      /* Region code */
    { "wl_regionaband", "", 
        NVRAM_DEFAULT,
        0 
    },      /* Region code */
    /* Physical interface related rules*/
    { "wl_rule_num", xstr(WL_NUM),     /* How many WLan interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl_rule_max", xstr(WL_NUM),     /* How many WLan interfaces. */
        NVRAM_NONE,
        0 
    },   
    /* Virtual interface related rules for wl0/wl1... */
    { "wlv_rule_num", xstr(WL0_RULE_NUM) ,     /* How many virtual wl0 interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wlv_rule_max", xstr(WL0_RULE_MAX),     /* Max virtual wl0 interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl_basic_rule", WL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^net_mode^txpower^channel^bisolation */
    { "wl_advanced_rule", WL_ADVANCED_RULE, 
        NVRAM_NONE,
        0 
    },      /* infra^ap_mode^rateset^mrate^frag^rts^dtim^bcn^plcphdr^antdiv^
               gmode^gmode_protection^afterburner^frameburst^
               txburst[0:disable|1:enable->1]^
               pktaggr[0:disable|1:enable->0]^
               txmode[0:None|1:CCK|2:OFDM->0]^

               rdg[0:disable|1:enablei->0]^
               linkadapt[0:disable|1:enable->0]^
               htc[0:disable|1:enable->0]^

               htbw[0:20|1:20/40->1]^
               gi[0:long|1:short->1]^
               opmode[0:mixed|1:GreenField->0]^
               mcs[0~15,32,33=auto->33]^
               mpdu[0 none, 1~7 -> 5]^
               amsdu[0:disable|1:enable->0]^

               autoba[0:disable|1:enable->1]^
               bawinsize[1~64,default=8,iot=64->64]^
               badecline[0:disable|1:enable->1->0]
               extcha^wdstxmode[CCK|OFDM|HTMIX|GREENFIELD]
               => 1^ap^default^0^2346^2347^1^100^0^1^1^off^off^off^1^0^0^0^0^0^1^1^0^33^5^0^1^64^0^0^0
             */
    { "wl_wds_rule", WL_WDS_RULE, 
        NVRAM_NONE,
        0 
    },      /* mode^timeout */  /*mode: disable/bridge/repeater*/
    { "wl_wme_rule", WL_WME_RULE, 
        NVRAM_NONE,
        0 
    },      /* sta_bk^Sta_be^sta_vi^sta_vo^ap_bk^ap_be^ap_vi^ap_vo^no_ack^max_assoc */
    { "wl_wps_rule", WL_WPS_RULE,
        NVRAM_NONE,
        0
    }, /* mode[0:disabled|7:enabled]^pin^
          configured[0:unconfigured|1:configured] */
    { "wl_wps_sta_rule", WL_WPS_STA_RULE,
        NVRAM_NONE,                                                                         0
    }, /* mode[0:disabled|1:pin mode|2:PBC mode]^
          pin^reg_ssid^reg_auth^reg_enc^reg_keyindex^reg_key */
    { "wl0_basic_rule", WL0_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^hidden^rate^wme^isolation */
    { "wl0_ifname_rule", WL0_IFNAME_RULE, 
        NVRAM_NONE,
        0 
    },      /* ssid */
    { "wl0_ssid_rule_default", WL0_SSID_RULE,
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0
    },
    { "wl0_ssid_rule", "",
        NVRAM_DEFAULT,
        0 
    },      /* ssid */
    { "wl0_sec_rule", WL0_SEC_RULE, 
        NVRAM_NONE,
        0 
    },      /* secmode */
    { "wl0_sec_wep_rule", WL0_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key_index^key1^key2^key3^key4^
               keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
    { "wl0_sec_wpa_rule", WL0_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^session_timeout */
    { "wl0_sec_wpa2_rule", WL0_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^preauth^pmkperiod^session_timeout^wpacap */
/* acl policy can apply to each ssid, we add rule according to ssid number */
#if (WL0_RULE_MAX >= 1)
    { "wl00_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl00_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl00_acl_max", xstr(WL_ACL_MAX), 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl00_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
#if (WL0_RULE_MAX >= 2)
    { "wl01_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl01_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl01_acl_max", xstr(WL_ACL_MAX), 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl01_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
#if (WL0_RULE_MAX >= 4)
    { "wl02_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl02_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl02_acl_max", xstr(WL_ACL_MAX), 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl02_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
    { "wl03_acl_basic_rule", WL_ACL_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^defpolicy */
    { "wl03_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl03_acl_max", xstr(WL_ACL_MAX), 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl03_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* name^enable^mac^policy */
#endif /* WL0_RULE_MAX >= 4 */
#endif /* WL0_RULE_MAX >= 2 */
#endif /* WL0_RULE_MAX >= 1 */

    { "wl0_acl_enable", "0", 
        NVRAM_NONE,
        0 
    },      /* enable/disable acl */
    { "wl0_acl_rule_default", "0", 
        NVRAM_NONE,
        0 
    },      /* acl rule default */
    { "wl0_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl0_acl_max", "32", 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl0_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* mac */
#if ( WL_NUM >= 2 )
    { "wl1_basic_rule", WL1_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^hidden^rate^wme^isolation */
    { "wl1_ifname_rule", WL1_IFNAME_RULE, 
        NVRAM_NONE,
        0 
    },      /* ifname */
    { "wl1_ssid_rule", WL1_SSID_RULE, 
        NVRAM_NONE,
        0 
    },      /* ssid */
    { "wl1_sec_rule", WL1_SEC_RULE, 
        NVRAM_NONE,
        0 
    },      /* secmode */
    { "wl1_sec_wep_rule", WL1_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key_index^key1^key2^key3^key4^
               keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
    { "wl1_sec_wpa_rule", WL1_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^session_timeout */
    { "wl1_sec_wpa2_rule", WL1_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto^radius_key^radius_ipaddr^radius_port^rekey_mode^
               rekey_time_interval^rekey_pkt_interval^preauth^pmkperiod^session_timeout^wpacap */
    { "wl1_acl_enable", "0", 
        NVRAM_NONE,
        0 
    },      /* enable/disable acl */
    { "wl1_acl_rule_default", "0", 
        NVRAM_NONE,
        0 
    },      /* acl rule default */
    { "wl1_acl_num", "0", 
        NVRAM_NONE,
        0 
    },      /* number of acl mac list */
    { "wl1_acl_max", "32", 
        NVRAM_NONE,
        0 
    },      /* max number of acl mac list */
    { "wl1_acl_rule", "", 
        NVRAM_NONE,
        0 
    },      /* mac */
#endif
    /* wds related rules */
    { "wl0_wds_rule_num", xstr(WDS_RULE_MAX),     /* How wds interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl0_wds_rule_max", xstr(WDS_RULE_MAX),     /* Max wds interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl0_wds_basic_rule", WL0_WDS_BASIC_RULE, 
        NVRAM_NONE,
        0 
    },      /* mac^secmode */
    { "wl0_wds_sec_wep_rule", "|||", 
        NVRAM_NONE,
        0 
    },      /* key */
    { "wl0_wds_sec_wpa_rule", "^tkip|^tkip|^tkip|^tkip", 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    /* apcli related rules */
    { "wl0_apcli_rule_num", xstr(APCLI_RULE_MAX),   /* How apcli interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl0_apcli_rule_max", xstr(APCLI_RULE_MAX),   /* Max apcli interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl0_apcli_rule", WL0_APCLI_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^ssid^bssid^secmode^wifiwan */
    { "wl0_apcli_sec_wep_rule", WL0_APCLI_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key_index^key1^key2^key3^key4^keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
    { "wl0_apcli_sec_wpa_rule", WL0_APCLI_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    { "wl0_apcli_sec_wpa2_rule", WL0_APCLI_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
#if defined(EZP_PROD_BRAND_SONY) || defined(EZP_SUB_BRAND_GARMIN) || defined(PLATFORM_AXA) ||defined(PLATFORM_AXM) || defined(PLATFORM_M2E) || defined(PLATFORM_ALS)
    { "wl0_apcli_swap_basic_rule", "1^6^0",
#else
    { "wl0_apcli_swap_basic_rule", "1^6^0",
#endif
        NVRAM_NONE,
        0 
    },      /* enable^max^index */
    { "wl0_apcli_swap_channel_rule", "6^0|6^0|6^0|6^0|6^0|6^0",
        NVRAM_NONE,
        0 
    },      /* enable^max^index */
    { "wl0_apcli_swap_rule", WL0_APCLI_RULE"|"WL0_APCLI_RULE"|"WL0_APCLI_RULE"|"WL0_APCLI_RULE"|"WL0_APCLI_RULE"|"WL0_APCLI_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^ssid^bssid^secmode^wifiwan */
    { "wl0_apcli_swap_sec_wep_rule", WL0_APCLI_SEC_WEP_RULE"|"WL0_APCLI_SEC_WEP_RULE"|"WL0_APCLI_SEC_WEP_RULE"|"WL0_APCLI_SEC_WEP_RULE"|"WL0_APCLI_SEC_WEP_RULE"|"WL0_APCLI_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key_index^key1^key2^key3^key4^keytype[0:hex|1:ascii]^encmode[open|shared|auto] */
    { "wl0_apcli_swap_sec_wpa_rule", WL0_APCLI_SEC_WPA_RULE"|"WL0_APCLI_SEC_WPA_RULE"|"WL0_APCLI_SEC_WPA_RULE"|"WL0_APCLI_SEC_WPA_RULE"|"WL0_APCLI_SEC_WPA_RULE"|"WL0_APCLI_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    { "wl0_apcli_swap_sec_wpa2_rule", WL0_APCLI_SEC_WPA2_RULE"|"WL0_APCLI_SEC_WPA2_RULE"|"WL0_APCLI_SEC_WPA2_RULE"|"WL0_APCLI_SEC_WPA2_RULE"|"WL0_APCLI_SEC_WPA2_RULE"|"WL0_APCLI_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
#if ( WL_NUM >= 2 )
    { "wl1_wds_rule_num", xstr(WDS_RULE_MAX),     /* How wds interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl1_wds_rule_max", xstr(WDS_RULE_MAX),     /* Max wds interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl1_wds_basic_rule", "^disabled|^disabled|^disabled|^disabled", 
        NVRAM_NONE,
        0 
    },      /* mac^secmode */
    { "wl1_wds_sec_wep_rule", "|||", 
        NVRAM_NONE,
        0 
    },      /* key */
    { "wl1_wds_sec_wpa_rule", "^tkip|^tkip|^tkip|^tkip", 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    /* apcli related rules */
    { "wl1_apcli_rule_num", xstr(APCLI_RULE_MAX),   /* How apcli interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl1_apcli_rule_max", xstr(APCLI_RULE_MAX),   /* Max apcli interfaces. */
        NVRAM_NONE,
        0 
    },   
    { "wl1_apcli_rule", WL1_APCLI_RULE, 
        NVRAM_NONE,
        0 
    },      /* enable^ssid^bssid^secmode */
    { "wl1_apcli_sec_wep_rule", WL1_APCLI_SEC_WEP_RULE, 
        NVRAM_NONE,
        0 
    },      /* key_index^key1^key2^key3^key4 */
    { "wl1_apcli_sec_wpa_rule", WL1_APCLI_SEC_WPA_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
    { "wl1_apcli_sec_wpa2_rule", WL1_APCLI_SEC_WPA2_RULE, 
        NVRAM_NONE,
        0 
    },      /* key^crypto */
#endif
    /* for coova-chilli authentication setting */
    /* coova settings rules */
    { "auth_server_rule_num", "1",
        NVRAM_NONE,
        0
    },
    { "auth_server_rule_max", "1",
        NVRAM_NONE,
        0
    },
    { "auth_server_enable", "1",
        NVRAM_NONE,
        0
    },
    { "auth_server_rule", "10.1.1.230^3990^/www/welcome.html^/www/status.html^/www/goodbye.html^"LAN_IPADDR_MASK"253^auto^10.1.1.254^0^10.1.1.230^1812^1813^radiussecret^mynas^MySpot^MySpot^^/etc/chilli/localusers",
        NVRAM_NONE,
        0
    }, /* web_serv_ipaddr^web_serv_port^logon_page^status_page^logoff_page^admin_ipaddr^dns_type^dns_ipaddr^radius_enable^radius_ipaddr^radius_auth_port^radius_accp_port^radius_secret^nasid^locid^rlocname^rlocid^userdb */
    /* coova user rules */
    { "auth_user_rule_num", "0",
        NVRAM_NONE,
        0
    },
    { "auth_user_rule_max", "200",
        NVRAM_NONE,
        0
    },
    { "auth_user_enable", "1",
        NVRAM_NONE,
        0
    },
    { "auth_user_rule", "",
        NVRAM_NONE,
        0
    }, /* name^password^valid_time */
    /* for firewall */
    { "fw_rule", "1^1^1^1^0^1^0", 
        NVRAM_NONE,
        0 
    },  /* enable^syn_enable^icmp_enable^icmp_redir_enable
           ^rpfilter_enable^state_enable^bc_storm_enable */
    { "nat_pass_rule", "1^1^1^1^1^1^1", 
        NVRAM_NONE,
        0 
    },  /* enable^ipsec_enable^pptp_enable^l2tp_enable^
           h323_enable^rtsp_enable^mms_enable */
    /* TTL */
    { "ttl_rule", "^0^64", 
        NVRAM_NONE,
        0 
    },        /* TTL name^enable[1|0]^value */
    /* TOS */
    { "tos_rule", "1^1^1^1^1^1^1", 
        NVRAM_NONE,
        0 
    },   /* TOS enable^icmp_enable^dns_enable^ssh_enable^
            telnet_enable^check_enable^ack_enable */
    /* MSS */
    { "mss_enable", "1", 
        NVRAM_NONE,
        0 
    },        /* TOS [1|0] */

    /* SNAT */
    { "snat_enable", "1", 
        NVRAM_NONE,
        0 
    },        /* SNAT [1|0] */

    /* Conntrack max amount */
    { "ct_max", CT_MAX, 
        NVRAM_NONE,
        0 
    },        /* SNAT [1|0] */
    /* user account rule */
#if defined(PLATFORM_AXA) || defined(PLATFORM_AXM) || defined(PLATFORM_M2E)
    { "user_rule", "guest^2|admin^3",
#else
    { "user_rule", "^|^",
#endif
        NVRAM_NONE,
        0
    },        /* user^permission_class */
    /* Web server parameters */
#ifdef EZP_SUB_BRAND_GARMIN
    { "http_rule_default", "^1^1^admin^admin^8080",
#elif EZP_PROD_BRAND_ZYUS
    #ifdef EZP_SUB_BRAND_FEENEY
        { "http_rule_default", "^1^0^admin^ahj24732^80",
    #else
        { "http_rule_default", "^1^0^admin^1234^80", 
    #endif
#elif defined(EZP_PROD_BRAND_PLANET)
    { "http_rule_default", "^1^0^admin^admin^80", 
#elif defined EZP_PROD_BRAND_PROX
    #ifdef EZP_SUB_BRAND_APOLLO
        { "http_rule_default", "^1^1^admin^access^80",
    #else
        { "http_rule_default", "^1^1^admin^1234^8080", 
    #endif
#elif defined(PLATFORM_AXM)
    { "http_rule_default", "^1^1^admin^admin^80",
#elif defined(PLATFORM_AXA) || defined(PLATFORM_M2E)
    { "http_rule_default", "^1^1^admin^admin^80",
#else
    { "http_rule_default", "^1^0^admin^admin^8080", 
#endif
        NVRAM_NONE,
        0 
    }, /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port */
    { "http_rule", "", 
        NVRAM_DEFAULT,
        0 
    },  /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port */
    { "http_redirect_rule", HTTP_REDIRECT_DEFAULT,
        NVRAM_NONE,
        0
    },  /* enable(IOS dual WAN function) */
    { "led_rule", "1^0", 
        NVRAM_TEMP,
        0 
    },  /* name^enable[0|1]^rmgt_enable[0|1]^username^passwd^port */
    /* display rule */
    { "display_rule", xstr(HTTPD_DISPLAY),
        NVRAM_NONE,
        0
    },  /* widget_show[0|1] */
#define APP_ENABLE_RULE "1^0^1"
    /* app_enable_rule */
    { "app_enable_rule", APP_ENABLE_RULE,
        NVRAM_NONE,
        0
    },  /* hwnat^turbonat^sbm^dbm^gbm */
#if defined(PLATFORM_ALS)
    { "adtool_rule", "any^^4^any^^4^any^^10",
        NVRAM_NONE,
        0
    }, /* pingiface^pinghost^pingcnt^arpingiface^arpinghost^arpingcnt^tracerouteiface^traceroutehost^traceroutecnt */
#else
    { "adtool_rule", "any^^4^any^^4^any^^30",
        NVRAM_NONE,
        0
    }, /* pingiface^pinghost^pingcnt^arpingiface^arpinghost^arpingcnt^tracerouteiface^traceroutehost^traceroutecnt */
#endif
    { "stats_rule", STATS_RULE, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^stime^ifnames */
    { "stats_type", "", 
        NVRAM_TEMP,
        0 
    },  /* stats_ifname */
    { "stats_ifname", "br0", 
        NVRAM_TEMP,
        0 
    },  /* stats_ifname */
    { "stats_proto", "", 
        NVRAM_TEMP,
        0 
    },  /* stats_ifname */
    { "stats_interval", "", 
        NVRAM_TEMP,
        0 
    },  /* stats_time hourly:60, daily:600, weekly: 3600, monthly: 3600*6 */
    { "no_root_swap", "1", 
        NVRAM_NONE,
        0 
    }, /* no_root_swap */ 
    { "vnc_rule", "0^5900^5500^0.0.0.0^1^1^127.0.0.1^2002",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^vport^sport^ipaddr^mode^event_enable^event_ipaddr^event_port*/
    { "vnc_allow_rule", "192.168.0.0|10.0.0.0",
        NVRAM_NONE,
        0 
    },  /* ipaddr */
    { "vnc_session_max", VNC_SESSION_MAX, 
        NVRAM_NONE,
        0 
    },  /* value */
    { "l2tpd_rule", "0^l2tp-server^1400^1400^192.168.48.1^" xstr(L2TPD_SESSION_MAX) "^192.168.49.1^" xstr(L2TPD_SESSION_MAX) "^1^^1^0^1^1",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^name^mtu^mru^localip^localip_range^remoteip^remoteip_range^dns_auto_enable^dns^chap_enable[0|1]^pap_enable[0|1]^proxyarp_enable[0|1]^nat_enable[0|1] */
    { "l2tpd_user_rule", "",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^username^provider^passwd^ipaddr */
    { "l2tpd_user_rule_num", "0",
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */
    { "l2tpd_user_rule_max", L2TPD_USER_RULE_MAX,
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */
    { "ipsec_enable", "0",
        NVRAM_NONE,
        0
    },
    { "ipsec_rule_max", IPSEC_RULE_MAX,
        NVRAM_NONE,
        0
    }, /* value */
    { "ipsec_rule_num", "0",
        NVRAM_NONE,
        0 
    }, /* value */
    { "ipsec_rule", "", 
        NVRAM_NONE,
        0
    }, /* name^enable^mode^l2tp^local_extif^local_inipaddr^local_netmask^remote_gateway^
        * remote_inipaddr^remote_netmask^conn_init^ike_keymode^psk^rsa^ca^adv^
        * phase1_mode^phase1_auth^phase1_encrypt^phase1_group^phase1_left_id^
        * phase1_right_id^phase2_auth^phase2_encrypt^phase2_group^phase1_lifetime^
        * phase2_lifetime^dpd_enable^dpd_interval^dpd_timeout^ddns_gateway^
        * split_tunnel[enabled|initiator|responder]  */
    { "ipsec_status_rule", "",
        NVRAM_TEMP,
        0
    }, /* phase1[none|established]^phase2[none|established] */
#ifdef EZP_PROD_BRAND_PROX
    { "ipsec_split_tunnel_state", "enabled",
        NVRAM_TEMP,
        0
    }, /* [enabled|initiator|responder] */
#endif
    { "pptpd_rule", "0^pptp-server^60^1482^1482^192.168.38.1^" xstr(PPTPD_SESSION_MAX) "^192.168.39.1^" xstr(PPTPD_SESSION_MAX) "^1^^0^0^1^0^0^1^0^1",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^name^redialperiod^mtu^mru^localip^localip_range^remoteip^remoteip_range^dns_auto_enable[0|1]^dns^chap_enable[0|1]^mschap_enable[0|1]^mschapv2_enable[0|1]^eap_enable[0|1]^pap_enable[0|1]^mppe128_enable[0|1]^proxyarp_enable[0|1]^nat_enable[0|1] */
    { "pptpd_user_rule", "",
        NVRAM_NONE,
        0 
    },  /* enable[0|1]^username^provider^passwd^ipaddr */
    { "pptpd_user_rule_num", "0",
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */
    { "pptpd_user_rule_max", PPTPD_USER_RULE_MAX,
        NVRAM_NONE,
        0 
    },  /* username^provider^passwd^ipaddr */
    { "pptpd_session_max", xstr(PPTPD_SESSION_MAX), 
        NVRAM_NONE,
        0 
    },  /* value */
    { "wol_rule", "",
        NVRAM_NONE,
        0 
    },  /* name^enable^bcast^hwaddr^iface^passwd */
    { "lang", "", 
        NVRAM_DEFAULT,
        0 
    },  /* Web language support */
    { "license_key", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },              /* License Key */
    { "valid_key", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },              /* Valid Key */

#if !defined(EZP_PROD_BRAND_PROX) && !defined(EZP_PROD_BRAND_MYMO) && !defined(EZP_PROD_BRAND_SONY)
#if defined(PLATFORM_M2E)
    { "hostname", "QGenie", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },           /* Hostname */
#elif defined(PLATFORM_ALS)
    { "hostname", "ALLISCOM", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },           /* Hostname */
#else
    { "hostname", "AXIMCom", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },           /* Hostname */
#endif
#endif

    { "prod_supercat", xstr(EZP_PROD_SUPERCAT), 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },           /* Product super category */

#if !defined(EZP_PROD_SUPERCAT_X)
    { "prod_cat", xstr(EZP_PROD_CAT), 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },           /* Product category */
#else
    { "prod_cat", xstr(EZP_PROD_CAT), 
        NVRAM_NONE,
        0 
    },           /* Product category */
#endif

    { "prod_subcat", xstr(EZP_PROD_SUBCAT), 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* Product subcategory */
    { "prod_subsubcat", xstr(EZP_PROD_SUBSUBCAT), 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /*Product "X" content product category */
    { "x_mode_data", X_MODE_DATA,
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* Product sub-subcategory */
#if defined(PLATFORM_ALS)
    { "port_config", "lan",
        NVRAM_NONE,
        0 
    },
#else
    { "port_config", "wan",
        NVRAM_NONE,
        0 
    },
#endif 
    { "op_mode", "gateway",
        NVRAM_NONE,
        0 
    }, 
    { "switch_type", xstr(SWITCH),
        NVRAM_PROTECTED,
        0
    },
    { "ddns_id_rule", xstr(DDNS),
        NVRAM_PROTECTED,
        0
    },
    { "ddns_password_rule", xstr(DDNS),
        NVRAM_PROTECTED,
        0
    },
    { "platform", xstr(PLATFORM),
        NVRAM_NONE,
        0
    },
#if defined(PLATFORM_ALS)
    { "als_battery_rule", ALS_BATTERY_RULE,
        NVRAM_TEMP,
        0
    },
    { "als_status_rule", ALS_STATUS_RULE,
        NVRAM_TEMP,
        0
    },/* alias^baudrate^databit^stopbit^parity^flow^fifo^device*/
    { "als_serial_rule", ALS_SERAIL_RULE,
        NVRAM_NONE,
        0
    },/*mode^tcp_alive^inactive^max_connect^jammed_ip^allow*/
    { "als_operating_rule", ALS_OPERATING_RULE,
        NVRAM_NONE,
        0
    },/*length^limit_1^limit_2^limit_pro^transmit*/
    { "als_operating_data_packing_rule", ALS_OPERATING_DATA_PACKING_RULE,
        NVRAM_NONE,
        0
    },/*tcp_port^command^ip_1^ip_2^ip_3^ip_4^port_1^port_2^port_3^port_4^control^udp_port */
    { "als_operating_option_rule", ALS_OPERATING_OPTION_RULE,
        NVRAM_NONE,
        0
    },
#endif

#ifdef LAN_AUTOIP_SWITCH
    { "ip_switch_rule_num", IP_SWITCH_RULE_NUM,
        NVRAM_NONE,
        0
    }, 
    /* Set the rule number for lan ip switching candidates. 0 for disabling this function */
    { "ip_switch_rule", IP_SWITCH_RULE,
        NVRAM_NONE,
        0
    },
#endif
#if defined (PLATFORM_AXA) || defined (PLATFORM_M2E) || defined (PLATFORM_ALS)
#define PORTCFG_RULE "lan^1^LAN^1111101^0000001|wan^1^WAN^1111001^0000101"
#define PORTCFG_RULE_NUM 2
#else
#define PORTCFG_RULE "lan^1^LAN^1111001^0000101|wan^1^WAN^0000001^1111101"
#define PORTCFG_RULE_NUM 2
#endif

#if defined(EZP_SUB_BRAND_GARMIN)
#define WAN_RPIORITY_RULE "dhcp|wwan|wisp"
#else
#define WAN_RPIORITY_RULE ""
#endif

    { "wan_priority_rule", WAN_RPIORITY_RULE,
        NVRAM_TEMP,
        0 
    }, 
    { "serial_num_rule", "",
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0
    },        /* serial number */
    { "mfg_test_result_rule", MFG_TEST_RESULT_RULE,
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0
    },        /* MFG test result */
#if defined (PLATFORM_GMA)
    { "WISP_detect_portal_rule", WISP_DETECT_PORTAL_RULE,
        NVRAM_TEMP,
        0
    },        /* portal statue */

    { 
        "gmn_nat_rule_max", "64", 
        NVRAM_TEMP,
        0 
    },      /* gmn_nat_rule_max */
    { 
        "gmn_nat_rule_num", "0", 
        NVRAM_TEMP,
        0 
    },      /* gmn_nat_rule_num */
    {
        "gmn_nat_rule", "^^^",
        NVRAM_TEMP,
        0
    },

    { 
        "gmn_filter_rule_max", "64", 
        NVRAM_TEMP,
        0 
    },      /* gmn_filter_rule_max */
    { 
        "gmn_filter_rule_num", "0", 
        NVRAM_TEMP,
        0 
    },      /* gmn_filter_rule_num */
    {
        "gmn_filter_rule", "^^^",
        NVRAM_TEMP,
        0
    },

    { 
        "gmn_mcast_rule_max", "64", 
        NVRAM_TEMP,
        0 
    },      /* gmn_mcast_rule_max */
    { 
        "gmn_mcast_rule_num", "0", 
        NVRAM_TEMP,
        0 
    },      /* gmn_mcast_rule_num */
    {
        "gmn_mcast_rule", "^^^",
        NVRAM_TEMP,
        0
    },
#endif
    /* For one ethernet port product, config to wan or lan */
    { "portcfg_rule", PORTCFG_RULE,
        NVRAM_NONE,
        0 
    }, /* name^enable^dispstr^lan_member^wan_member */
    { "portcfg_rule_num", xstr(PORTCFG_RULE_NUM),
        NVRAM_NONE,
        0 
    }, /* port config rule number */
    { "machine_id", "", 
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* Machine id */
    { "arch", ARCH, 
        NVRAM_PROTECTED,
        0 
    },        /* Arch */
    { "license_invalid", "",
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* License status */
    { "valid_count", "",
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* Valid count */
    { "bootcount", "",
        NVRAM_PROTECTED | NVRAM_CUSTOMIZED,
        0 
    },        /* License bootcount */
    { "igmp_proxy_rule", IGMP_PROXY_ENABLE,
        NVRAM_NONE,
        0
    },        /* igmp_proxy_rule */
    { "ppp_control_rule", "1",
        NVRAM_TEMP,
        0
    },        /* ppp_control_rule:  gatewat_valid*/
    { "captive_rule", "0^0",
        NVRAM_NONE,
        0
    },
    { "card_reader_hardware_rule_max", CARD_READER_HARDWARE_RULE_MAX,
        NVRAM_NONE,
        0
    },
    { "card_reader_hardware_rule_num", CARD_READER_HARDWARE_RULE_NUM,
        NVRAM_NONE,
        0
    },
    { "card_reader_hardware_rule",     CARD_READER_HARDWARE_RULE,
        NVRAM_TEMP,
        0
    },        /* dev^gpio^gpiopol */
#if defined (PLATFORM_M2E)
    { "st_defpath_rule_num", "9",
        NVRAM_TEMP,
        0,
    },
    { "st_defpath_rule_max", "9",
        NVRAM_TEMP,
        0,
    },
    /* Storage default path rule, when mounting a device, we check the device
     * name and generate the default pathes */
    /* The last attribute, linkp, is the link and/or folder in /tmp/mnt, which
     * will be created and sorted according the nvram rule set order. */
    { "st_defpath_rule", "^^Qsync|^^SD|^^USB|Videos^Qsync^Videos|Photos^Qsync^Photos|Music^Qsync^Music|Documents^Qsync^Documents|Phonebook^Qsync^Phonebook|Others^Qsync^Others",
        NVRAM_TEMP,
        0,
    },
    /* defpath^dev^linkp */
    /* Use this option to control
     * 1. Qfile guest login 
     * 2. WebGUI guest access 
     * 3. webdav, samba and ftp server guest access */
    {
        "allow_guest", "0",
        NVRAM_NONE,
        0
    },
#endif
    { "boot_test", "0",
        NVRAM_NONE,
        0
    }, /* a flag to enable or disable boot cycle test */
#if defined (PLATFORM_M2E) || defined (PLATFORM_ALS)
    { "gpiodef_rule",     "",
        NVRAM_NONE,
        0
    },
    { "upnpc_setting_rule_num", "2",
        NVRAM_NONE,
        0
    },
    /* name^enable^inport^outport^proto */
    { "upnpc_setting_rule", "web_mgmt^1^80^80^tcp|file_station^1^8081^8081^tcp",
        NVRAM_NONE,
        0
    },
    /* name^avtivated^inport^outport */
    { "upnpc_status_rule", "web_mgmt^0^80^80^|file_station^0^8081^8081^",
        NVRAM_TEMP,
        0
    },
#endif
/* num^enable^
 * func[in|out|nc]^
 *   in : config as gpio input
 *   out : config as gpio output
 *   nc : config as gpio non-configured
 * initval^
 *   gpio initial value
 * desc 
 *   brief description of the gpio
 */
#ifdef AXIMDDNS
    { "aximddns_rule", "0^^0^0^0",
        NVRAM_NONE,
        0
    }, /* AXIM DDNS rule */
#endif
    /* netstat rule including detection interval */
    { "netstat_rule", NETSTAT_RULE,
        NVRAM_NONE,
        0
    }, /* interval */
    { 0, 0, 0 ,0 }
};
#endif

