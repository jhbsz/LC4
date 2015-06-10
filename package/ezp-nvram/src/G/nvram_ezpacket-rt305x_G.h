#ifdef CONFIG_EZP_ARCH_RT305X
#if ( WAN_NUM_G == 1) 
#define WAN0_IFNAME_G "vlan2"
#define WAN0_DEVICE_G "vlan2"
#define BR_RULE_G "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^"
#elif ( WAN_NUM_G == 2)
#define WAN0_IFNAME_G "vlan2"
#define WAN0_DEVICE_G "vlan2"
#define WAN1_IFNAME_G "vlan3"
#define WAN1_DEVICE_G "vlan3"
#define BR_RULE_G "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^|WAN2^0^vlan3^"
#elif ( WAN_NUM_G == 3)
#define WAN0_IFNAME_G "vlan2"
#define WAN0_DEVICE_G "vlan2"
#define WAN1_IFNAME_G "vlan3"
#define WAN1_DEVICE_G "vlan3"
#define WAN2_IFNAME_G "vlan4"
#define WAN2_DEVICE_G "vlan4"
#define BR_RULE_G "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^|WAN2^0^vlan3^|WAN3^0^vlan4^"
#elif ( WAN_NUM_G == 4)
#define WAN0_IFNAME_G "vlan2"
#define WAN0_DEVICE_G "vlan2"
#define WAN1_IFNAME_G "vlan3"
#define WAN1_DEVICE_G "vlan3"
#define WAN2_IFNAME_G "vlan4"
#define WAN2_DEVICE_G "vlan4"
#define WAN3_IFNAME_G "vlan5"
#define WAN3_DEVICE_G "vlan5"
#define BR_RULE_G "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^|WAN2^0^vlan3^|WAN3^0^vlan4^|WAN4^0^vlan5^"
#elif ( WAN_NUM_G == 5)
#define WAN0_IFNAME_G "vlan2"
#define WAN0_DEVICE_G "vlan2"
#define WAN1_IFNAME_G "vlan3"
#define WAN1_DEVICE_G "vlan3"
#define WAN2_IFNAME_G "vlan4"
#define WAN2_DEVICE_G "vlan4"
#define WAN3_IFNAME_G "vlan5"
#define WAN3_DEVICE_G "vlan5"
#define WAN4_IFNAME_G "vlan6"
#define WAN4_DEVICE_G "vlan6"
#define BR_RULE_G "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^|WAN2^0^vlan3^|WAN3^0^vlan4^|WAN4^0^vlan5^|WAN5^0^vlan6^"
#else
#error "WAN_NUM_G is not valid!!"
#endif


#endif /* CONFIG_EZP_ARCH_RT305X */

