#ifdef CONFIG_EZP_ARCH_RT305X

/* 3G/4G WAN default interface should be ppp0. */
#if ( WAN_NUM_I == 1) 
#define WAN0_IFNAME_I "ppp0"
#define WAN0_DEVICE_I "vlan2"
#define BR_RULE_I "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
#elif ( WAN_NUM_I == 2)
#define WAN0_IFNAME_I "vlan2"
#define WAN0_DEVICE_I "vlan2"
#define WAN1_IFNAME_I "ppp1"
#define WAN1_DEVICE_I "ppp1"
#define BR_RULE_I "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^"
#elif ( WAN_NUM_I == 3)
#define WAN0_IFNAME_I "vlan2"
#define WAN0_DEVICE_I "vlan2"
#define WAN1_IFNAME_I "vlan3"
#define WAN1_DEVICE_I "vlan3"
#define WAN2_IFNAME_I "ppp2"
#define WAN2_DEVICE_I "ppp2"
#define BR_RULE_I "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#elif ( WAN_NUM_I == 4)
#define WAN0_IFNAME_I "vlan2"
#define WAN0_DEVICE_I "vlan2"
#define WAN1_IFNAME_I "vlan3"
#define WAN1_DEVICE_I "vlan3"
#define WAN2_IFNAME_I "vlan4"
#define WAN2_DEVICE_I "vlan4"
#define WAN3_IFNAME_I "ppp3"
#define WAN3_DEVICE_I "ppp3"
#define BR_RULE_I "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^|WAN3^0^vlan4"
#elif ( WAN_NUM_I == 5)
#define WAN0_IFNAME_I "vlan2"
#define WAN0_DEVICE_I "vlan2"
#define WAN1_IFNAME_I "vlan3"
#define WAN1_DEVICE_I "vlan3"
#define WAN2_IFNAME_I "vlan4"
#define WAN2_DEVICE_I "vlan4"
#define WAN3_IFNAME_I "vlan5"
#define WAN3_DEVICE_I "vlan5"
#define WAN4_IFNAME_I "ppp4"
#define WAN4_DEVICE_I "ppp4"
#define BR_RULE_I "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3 apcli0^|WAN1^0^vlan2^|WAN2^0^vlan3^|WAN3^0^vlan4^|WAN4^0^vlan5^"
#else
#error "WAN_NUM_I is not valid!!"
#endif

#endif /* CONFIG_EZP_ARCH_RT305X */
