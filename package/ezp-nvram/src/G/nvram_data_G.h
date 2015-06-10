    { "wan_num_G", xstr(WAN_NUM_G), 
        NVRAM_NONE,
        0 
    },    /* number of WAN */
#if ( WAN_NUM_G >= 1 )
    { "wan0_ifname_G", WAN0_IFNAME_G, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan0_device_G", WAN0_DEVICE_G, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan0_proto_G", WAN0_PROTO_G, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_G >= 2 )
    { "wan1_ifname_G", WAN1_IFNAME_G, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan1_device_G", WAN1_DEVICE_G, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan1_proto_G", WAN1_PROTO_G, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_G >= 3 )
    { "wan2_ifname_G", WAN2_IFNAME_G, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan2_device_G", WAN2_DEVICE_G, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan2_proto_G", WAN2_PROTO_G, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_G >= 4 )
    { "wan3_ifname_G", WAN3_IFNAME_G, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan3_device_G", WAN3_DEVICE_G, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan3_proto_G", WAN3_PROTO_G, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_G >= 5 )
    { "wan4_ifname_G", WAN4_IFNAME_G, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan4_device_G", WAN4_DEVICE_G, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan4_proto_G", WAN4_PROTO_G, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
    { "br_rule_G", BR_RULE_G,
       NVRAM_NONE,
        0
    },      /* name^enable[0|1]^ifnames^flag */
    /* WAN main rules */
    { "wan_main_rule_G", WAN_MAIN_RULE_G, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^upnp */
    /* Application control */
    { "appmgr_rule_G", xstr(APPMBG_RULE_G),
        NVRAM_NONE,
        0
    },       /* wlan^iscsi^pptpd^storage^wwan^wisp^portcfg^hwnat^firmware^iphone^barry^smart^usbled^wimax */
