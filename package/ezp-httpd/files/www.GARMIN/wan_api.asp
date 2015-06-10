<html>
<head></head>
<body>
<input type="hidden" name="wan0_vpnc_enable" value="<%nv_attr_get("wan_pptp_l2tp_rule", "0", "enable");%>" />
<input type="hidden" name="wanupnp_enable" value="<%nv_get("wanupnp_rule");%>" />
<input type="hidden" name="wan0_enable" value="<%nv_attr_get("wan_main_rule", "0", "enable");%>" />
<input type="hidden" name="wan0_proto" value="<%nv_get("wan0_proto");%>" />
<input type="hidden" name="wan0_stp" value="<%nv_attr_get("wan_main_rule", "0", "stp");%>" />
<input type="hidden" name="wan0_mtu" value="<%nv_attr_get("wan_main_rule", "0", "mtu");%>" />

<input type="hidden" name="wan0_dhcp_hostname" value="<%nv_attr_get("wan_dhcp_rule", "0", "hostname");%>" />
<input type="hidden" name="wan0_bigpond_enable" value="<%nv_attr_get("wan_bigpond_rule", "0", "enable");%>" />
<input type="hidden" name="wan0_bigpond_server" value="<%nv_attr_get("wan_bigpond_rule", "0", "server");%>" />
<input type="hidden" name="wan0_bigpond_username" value="<%nv_attr_get("wan_bigpond_rule", "0", "username");%>" />
<input type="hidden" name="wan0_bigpond_passwd" value="<%nv_attr_get("wan_bigpond_rule", "0", "passwd");%>" />
<input type="hidden" name="wan0_dhcp_domain" value="<%nv_attr_get("wan_dhcp_rule", "0", "domain");%>" />
<input type="hidden" name="wan0_dhcp_lease" value="<%nv_attr_get("wan_dhcp_rule", "0", "lease");%>" />

<input type="hidden" name="wan0_wwan_brand" value="<%nv_attr_get("wan_wwan_rule", "0", "brand");%>" />
<input type="hidden" name="wan0_wwan_model" value="<%nv_attr_get("wan_wwan_rule", "0", "model");%>" />
<input type="hidden" name="wan0_wwan_apn_type" value="<%nv_attr_get("wan_wwan_rule", "0", "apn_type");%>" />
<input type="hidden" name="wan0_wwan_location" value="<%nv_attr_get("wan_wwan_rule", "0", "location");%>" />
<input type="hidden" name="wan0_wwan_isp" value="<%nv_attr_get("wan_wwan_rule", "0", "isp");%>" />
<input type="hidden" name="wan0_wwan_apn" value="<%nv_attr_get("wan_wwan_rule", "0", "apn");%>" />
<input type="hidden" name="wan0_wwan_pin" value="<%nv_attr_get("wan_wwan_rule", "0", "pin");%>" />
<input type="hidden" name="wan0_wwan_auth" value="<%nv_attr_get("wan_wwan_rule", "0", "auth");%>" />
<input type="hidden" name="wan0_wwan_username" value="<%nv_attr_get("wan_wwan_rule", "0", "username");%>" />
<input type="hidden" name="wan0_wwan_passwd" value="<%nv_attr_get("wan_wwan_rule", "0", "passwd");%>" />
<input type="hidden" name="wan0_wwan_dialstr" value="<%nv_attr_get("wan_wwan_rule", "0", "dialstr");%>" />
<input type="hidden" name="wan0_wwan_mode" value="<%nv_attr_get("wan_wwan_rule", "0", "mode");%>" />
<input type="hidden" name="wan0_wwan_demand" value="<%nv_attr_get("wan_wwan_rule", "0", "demand");%>" />
<input type="hidden" name="wan0_wwan_idletime" value="<%nv_attr_get("wan_wwan_rule", "0", "idletime");%>" />
<input type="hidden" name="wan0_wwan_lcp_echo_interval" value="<%nv_attr_get("wan_wwan_rule", "0", "redialperiod");%>" />
<input type="hidden" name="wan0_wwan_lcp_echo_failure" value="<%nv_attr_get("wan_wwan_rule", "0", "echo_failure");%>" />
<input type="hidden" name="wan0_wwan_mtu" value="<%nv_attr_get("wan_wwan_rule", "0", "mtu");%>" />
<input type="hidden" name="wan0_wwan_turbolink" value="<%nv_attr_get("wan_wwan_rule", "0", "turbolink");%>" />

<input type="hidden" name="wan0_static_ipaddr" value="<%nv_attr_get("wan_static_rule", "0", "ipaddr");%>" />
<input type="hidden" name="wan0_static_mask" value="<%nv_attr_get("wan_static_rule", "0", "mask");%>" />
<input type="hidden" name="wan0_static_gateway" value="<%nv_attr_get("wan_static_rule", "0", "gateway");%>" />
<input type="hidden" name="wan0_static_dns1" value="<%nv_attr_get("wan_static_rule", "0", "dns1");%>" />
<input type="hidden" name="wan0_static_dns2" value="<%nv_attr_get("wan_static_rule", "0", "dns2");%>" />

<input type="hidden" name="wl0_apcli0_ssid" value="<%nv_attr_get("wl0_apcli_rule", "0", "ssid");%>" />
<input type="hidden" name="wl0_apcli0_bssid" value="<%nv_attr_get("wl0_apcli_rule", "0", "bssid");%>" />
<input type="hidden" name="wl0_apcli0_secmode" value="<%nv_attr_get("wl0_apcli_rule", "0", "secmode");%>" />
<input type="hidden" name="wl0_channel" value="<%nv_attr_get("wl0_apcli_swap_channel_rule", "0", "channel");%>" />
<input type="hidden" name="wl0_extcha" value="<%nv_attr_get("wl0_apcli_swap_channel_rule", "0", "extcha");%>" />
<input type="hidden" name="wl0_apcli0_wep_key_index" value="<%nv_attr_get("wl0_apcli_sec_wep_rule", "0", "key_index");%>" />
<input type="hidden" name="wl0_apcli0_wep_key1" value="<%nv_attr_get("wl0_apcli_sec_wep_rule", "0", "key1");%>" />
<input type="hidden" name="wl0_apcli0_wep_key2" value="<%nv_attr_get("wl0_apcli_sec_wep_rule", "0", "key2");%>" />
<input type="hidden" name="wl0_apcli0_wep_key3" value="<%nv_attr_get("wl0_apcli_sec_wep_rule", "0", "key3");%>" />
<input type="hidden" name="wl0_apcli0_wep_key4" value="<%nv_attr_get("wl0_apcli_sec_wep_rule", "0", "key4");%>" />

<input type="hidden" name="wl0_apcli0_wpa_key" value="<%nv_attr_get("wl0_apcli_sec_wpa_rule", "0", "key");%>" />
<input type="hidden" name="wl0_apcli0_wpa_crypto" value="<%nv_attr_get("wl0_apcli_sec_wpa_rule", "0", "crypto");%>" />
<input type="hidden" name="wl0_apcli0_wpa2_key" value="<%nv_attr_get("wl0_apcli_sec_wpa2_rule", "0", "key");%>" />
<input type="hidden" name="wl0_apcli0_wpa2_crypto" value="<%nv_attr_get("wl0_apcli_sec_wpa2_rule", "0", "crypto");%>" />
</body>
