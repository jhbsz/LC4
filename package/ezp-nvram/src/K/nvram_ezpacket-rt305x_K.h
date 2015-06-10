#ifdef CONFIG_EZP_ARCH_RT305X

#if ( WAN_NUM_P == 1) 
#define WAN0_IFNAME_K "vlan2"
#define WAN0_DEVICE_K "vlan2"
#define BR_RULE_K "LAN1^1^vlan1 ra0 ra1 wds0 wds1 wds2 wds3^|WAN1^0^vlan2^"
#else
#error "WAN_NUM_P is not valid!!"
#endif

#endif /* CONFIG_EZP_ARCH_RT305X */

