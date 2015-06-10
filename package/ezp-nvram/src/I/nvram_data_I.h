    { "wan_num_I", xstr(WAN_NUM_I), 
        NVRAM_NONE,
        0 
    },    /* number of WAN */
#if ( WAN_NUM_I >= 1 )
    { "wan0_ifname_I", WAN0_IFNAME_I, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan0_device_I", WAN0_DEVICE_I, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan0_proto_I", WAN0_PROTO_I, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_I >= 2 )
    { "wan1_ifname_I", WAN1_IFNAME_I, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan1_device_I", WAN1_DEVICE_I, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan1_proto_I", WAN1_PROTO_I, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_I >= 3 )
    { "wan2_ifname_I", WAN2_IFNAME_I, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan2_device_I", WAN2_DEVICE_I, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan2_proto_I", WAN2_PROTO_I, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_I >= 4 )
    { "wan3_ifname_I", WAN3_IFNAME_I, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan3_device_I", WAN3_DEVICE_I, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan3_proto_I", WAN3_PROTO_I, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
#if ( WAN_NUM_I >= 5 )
    { "wan4_ifname_I", WAN4_IFNAME_I, 
        NVRAM_NONE,
        0 
    },    /* WAN interface name */
    { "wan4_device_I", WAN4_DEVICE_I, 
        NVRAM_NONE,
        0 
    }, /* WAN device */
    { "wan4_proto_I", WAN4_PROTO_I, 
        NVRAM_NONE,
        0 
    },        /* WAN proto */
#endif
    { "br_rule_I", BR_RULE_I,
       NVRAM_NONE,
        0
    },      /* name^enable[0|1]^ifnames^flag */
    /* WAN main rules */
    { "wan_main_rule_I", WAN_MAIN_RULE_I, 
        NVRAM_NONE,
        0 
    },  /* name^enable[0|1]^mtu^stp^upnp */
    /* Application control */
    { "appmgr_rule_I", xstr(APPMBG_RULE_I),
        NVRAM_NONE,
        0
    },       /* wlan^iscsi^pptpd^storage^wwan^wisp^portcfg^hwnat^firmware^iphone^barry^smart^usbled^wimax */
