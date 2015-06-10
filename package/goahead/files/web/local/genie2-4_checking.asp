<html>
<meta http-equiv="Pragma" content="no-cache">
<script language="JavaScript" type="text/javascript">
var wan_gateway = "<% getWanGateway(); %>";
if (wan_gateway!="0.0.0.0")
	parent.location.href="/local/genie3.html";
else
	parent.location.href="/local/genie2_error2.html";
</script>
</html>