<html>
<meta http-equiv="Pragma" content="no-cache">
<body>
<form method=post name="wanCfg" action="/goform/setWan_Simple">
   <input name="connectionType" type="hidden" value="Ethernet">
   <input name="WAN_IP_Auto" type="hidden" value="0">
   <input name="dns1Type" type="hidden" value="ISP">
   <input name="dns2Type" type="hidden" value="ISP">
   <input type=hidden value="/local/genie2_checking2.asp" name="wan_url">
</form>   
</body>
<script language="JavaScript" type="text/javascript">
var str = "<% getPortStatus(); %>";//First time call get status cannot get update infomation.
var str = "<% getPortStatus(); %>";//So, need call get status 2 times
var all = new Array();
var port_num=0;
var wan_port_uplink=0;

if(str == "-1"){
		document.write("not support");
		parent.location.href="/local/genie2_error.html";//No Internet service detected
}
else
{
	if(str == "1")
				wan_port_uplink=1;
	else
		wan_port_uplink=0;

	if (wan_port_uplink)
	{
		var status=<% checkEthernetWanType(); %>;
		if (status)
			parent.location.href="/local/genie2-1.asp";//PPPoE
		else
		{
			document.wanCfg.submit();
		}
	}
	else{
		parent.location.href="/local/genie2_error.html";//No Internet service detected
	}
}
</script>
</html>