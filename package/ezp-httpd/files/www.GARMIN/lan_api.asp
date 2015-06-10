<html>
<head></head>
<body>
<input type="hidden" name="lan0_static_ipaddr" value="<%nv_attr_get("lan_static_rule", "0", "ipaddr");%>" />
<input type="hidden" name="lan0_static_mask" value="<%nv_attr_get("lan_static_rule", "0", "mask");%>" />
<input type="hidden" name="lan0_stp" value="<%nv_attr_get("lan_main_rule", "0", "stp");%>" />
<input type="hidden" name="lan0_mtu" value="<%nv_attr_get("lan_main_rule", "0", "mtu");%>" />
</body>
