<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
</style>

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

function initTranslation()
{
	var e = document.getElementById("WirelessAssocTitle");
	e.innerHTML = _("wireless assoc title");

	var e = document.getElementById("WirelessAssocTitle1");
	e.innerHTML = _("wireless assoc title");

	var e = document.getElementById("WirelessAssocNumber");
	e.innerHTML = _("wireless assoc number");
		
	e = document.getElementById("WirelessAssocMacAddr");
	e.innerHTML = _("wireless assoc macaddr");
	
	e = document.getElementById("WirelessAssocTime");
	e.innerHTML = _("wireless assoc time");

	e = document.getElementById("WirelessAssocRefresh");
	e.value = _("wireless assoc refresh");
}

function PageInit()
{
	initTranslation();
}

</script>
</head>
<body onLoad="PageInit()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="WirelessAssocTitle">Association List</li>
<li></li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="26" colspan="6"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id="WirelessAssocTitle1"> Association List<span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr >
<td width="10%"><center>
<span class="top_left" id="WirelessAssocNumber">#</span>
</center>
</td>
<td width="30%">
<center>
<span class="top_font" id="WirelessAssocMacAddr">MAC Address  </span>
</center>
</td>
<td width="65%">
<center>

<span class="top_right" id="WirelessAssocTime">Association Time</span>
</center>
</td>
</tr>
</table>
</li>
<% getwifiStaInfo(); %> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="5" colspan="6"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
<td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
<td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
</tr>
</table>
</td>
</tr>
</table>
</li>

<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=reset value=Refresh id="WirelessAssocRefresh" onClick="window.location.reload()">
</li>
</center>
</form>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>
</html>

