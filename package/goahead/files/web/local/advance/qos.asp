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

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;
function submit_apply()
{
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	alert("You CANNOT select WISP or Client mode to configure this page");
		return false;
	}
	showWebMessage(2, "");
	return true;

}

function initTranslation()
{
	var e = document.getElementById("QoSTitle");
	e.innerHTML = _("qos qostitle");

	e = document.getElementById("QoSWMMEnable");
	e.innerHTML = _("qos wmm enable");
	
	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initValue()
{
	initTranslation();

	var WMMEnable  = <% getCfgZero(1, "WmmCapable"); %>;
	
	if (WMMEnable == 1)
		document.wireless_advanced.wmm_capable.checked = true;
	else
		document.wireless_advanced.wmm_capable.checked = false;

	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");
	}
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name="wireless_advanced" action="/goform/WiFiWMM" onSubmit="return submit_apply()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="QoSTitle"> WMM Configuration </li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wmm_capable" type="checkbox" value="1" /><input name="AAA" type="hidden" value=1/>
<font id ="QoSWMMEnable">Enable WMM QoS</font>
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
<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset  value="Reset" id="GeneralReset" onClick="window.location.reload()">
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
