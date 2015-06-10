    { "wan_num_K", xstr(WAN_NUM_K), 
        NVRAM_NONE,
        0 
    },    /* number of WAN */
#if ( WAN_NUM_K >= 1 )
    { "wan0_ifname_K", WAN0_IFNAME_K, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan0_device_K", WAN0_DEVICE_K, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan0_proto_K", WAN0_PROTO_K, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
    { "br_rule_K", BR_RULE_K,
       NVRAM_NONE,
        0
    },      /* name^enable[0|1]^ifnames^flag */
    /* WAN main rules */
    { "wan_main_rule_K", WAN_MAIN_RULE_K, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^upnp */
    /* Application control */
    { "appmgr_rule_K", xstr(APPMBG_RULE_K),
        NVRAM_NONE,
        0
    },       /* wlan^iscsi^pptpd^storage^wwan^wisp^portcfg^hwnat^firmware^iphone^barry^smart^usbled^wimax */
