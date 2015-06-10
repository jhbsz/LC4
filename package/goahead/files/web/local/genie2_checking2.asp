<html>
<meta http-equiv="Pragma" content="no-cache">
<script language="JavaScript" type="text/javascript">
var refresh_interval=3000;
function refresh_info(){
var wan_gateway = "<% getWanGateway(); %>";
//alert(wan_gateway);
if ((wan_gateway.length!=0)&&(wan_gateway!="0.0.0.0"))
	parent.location.href="/local/genie2-3.html";
else
	parent.location.href="/local/genie2-2.asp";
}
self.setTimeout('refresh_info()', refresh_interval);

</script>
</html>