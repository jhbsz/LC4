<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall"); 
function initTranslation()
{
	var e = document.getElementById("firewallSetup");
	e.innerHTML = _("firewall setup");
	e = document.getElementById("firewallEnable");
	e.innerHTML = _("firewall enable");			
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
}    
function firewallClick()
{
	document.FIREWALL.FIREWALLEnabled .value = document.FIREWALL.FIREWALLEnabled .checked ? "1": "0";	
	return true;
}
function formCheck()
{
	firewallClick();
	showWebMessage(2, "");
	return true;
}

function updateState()
{
	initTranslation();
	if (document.FIREWALL.FIREWALLEnabled .value == "1")
		document.FIREWALL.FIREWALLEnabled .checked = true;
		
}
</script>
</head>
<body onload="updateState()">
<form method="post" name="FIREWALL" action="/goform/FIREWALL_ACTIVE" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul><li class="title" id="firewallSetup">Firewall Setup</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="3%"><input name=FIREWALLEnabled type=checkbox value="<% getCfgZero(1, "FIREWALLEnabled"); %>" /></td>
<td width="20%" id="firewallEnable">Enable Firewall</td>
<td width="77%">
</td>
</tr>
</table>
</li>
<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type="submit" value="Apply" id="natApply" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/firewall.asp" name="firewall_url">
</li>
</center>
</ul>
</div>
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
