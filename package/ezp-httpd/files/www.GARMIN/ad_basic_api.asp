<html>
<head></head>
<body>
<input type="hidden" name="lang" value="<%nv_get("lang");%>" />
<input type="hidden" name="http_admin" value="<%nv_attr_get("http_rule","0","username");%>" />
<input type="hidden" name="http_passwd" value="<%nv_attr_get("http_rule","0","passwd");%>" />
<input type="hidden" name="http_rmgt_enable" value="<%nv_attr_get("http_rule","0","rmgt_enable");%>" />
<input type="hidden" name="http_port" value="<%nv_attr_get("http_rule","0","port");%>" />
<input type="hidden" name="port_config" value="<%nv_get("port_config");%>" />
</body>
