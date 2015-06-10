    { "wan_num_P", xstr(WAN_NUM_P), 
        NVRAM_NONE,
        0 
    },    /* number of WAN */
#if ( WAN_NUM_P >= 1 )
    { "wan0_ifname_P", WAN0_IFNAME_P, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan0_device_P", WAN0_DEVICE_P, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan0_proto_P", WAN0_PROTO_P, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
    { "br_rule_P", BR_RULE_P,
       NVRAM_NONE,
        0
    },      /* name^enable[0|1]^ifnames^flag */
    /* WAN main rules */
    { "wan_main_rule_P", WAN_MAIN_RULE_P, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^upnp */
    /* Application control */
    { "appmgr_rule_P", xstr(APPMBG_RULE_P),
        NVRAM_NONE,
        0
    },       /* wlan^iscsi^pptpd^storage^wwan^wisp^portcfg^hwnat^firmware^iphone^barry^smart^usbled^wimax */
