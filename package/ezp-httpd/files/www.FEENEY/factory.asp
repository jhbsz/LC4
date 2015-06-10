<html>
<head></head>
<body>
<%show_auth_if_hwaddr("wan");%>
<%show_auth_if_hwaddr("lan");%>
<input type="hidden" name="ezp-wl0" value="<%show_auth_wl_hwaddr(0, 0);%>" />
</body>
