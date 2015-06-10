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
td {white-space: nowrap;}
</style>

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var MAX_PROFILE = 10;

var g_ssid;
var g_networktype;
var g_channel;
var g_auth;
var g_encry;
var g_bssid;

function countTime()
{
	//var connectstatus = '<!--#include ssi=getStaConnectionStatus() -->';

	//if (connectstatus == 1)  // 0 is NdisMediaStateConnected, 1 is NdisMediaStateDisconnected
		setTimeout("window.location.reload();", 1000*4);
}

function selectedSSIDChange(ssid, bssid, networktype, channel, encry, auth)
{
	g_ssid = ssid;
	g_networktype = networktype;
	g_channel = channel;
	g_auth = auth;
	g_encry = encry;
	g_bssid = bssid

//	document.sta_site_survey.connectionButton.disabled=false;
	document.sta_site_survey.addProfileButton.disabled=false;
//	document.sta_site_survey.connectedssid.disabled=true;

	//alert("ssid="+ssid+" networktype="+networktype+" channel="+channel+" auth="+auth+" bssid="+bssid);
}

function open_connection_page()
{
	cwin = window.open("site_survey_connection.asp","sta_site_survey_connection","toolbar=no, location=yes, scrollbars=yes, resizable=no, width=660, height=600");
}

function open_profile_page()
{
//	pcwin = window.open("add_profile_page.asp","add_profile_page","toolbar=no, location=yes, scrollbars=yes, resizable=no, width=660, height=600");
	
//	if (profileNum >= MAX_PROFILE){
//		alert("The profile table is full.");	
//		return false;
//	}
	
	recordProfileSsid();
	parent.onChangeHeadMenu(21, 1, _('CONFIGURATION')+' > '+_('Network')+' > '+_('Wireless LAN')+' > '+_('General'));
	self.location.href="wisp.asp";
}

function showConnectionSsid()
{
	cwin.document.forms["sta_site_survey_connection"].Ssid.value = g_ssid;
	cwin.document.forms["sta_site_survey_connection"].bssid.value = g_bssid;

	if (g_networktype == 1)
	{
		if (g_auth.indexOf("WPA2-PSK") >= 0)
			cwin.document.forms["sta_site_survey_connection"].security_infra_mode.value = 7;
		else if (g_auth.indexOf("WPA-PSK") >= 0)
			cwin.document.forms["sta_site_survey_connection"].security_infra_mode.value = 4;
		else if (g_auth.indexOf("WPA2") >= 0)
			cwin.document.forms["sta_site_survey_connection"].security_infra_mode.value = 6;
		else if (g_auth.indexOf("WPA") >= 0)
			cwin.document.forms["sta_site_survey_connection"].security_infra_mode.value = 3;
		else
			cwin.document.forms["sta_site_survey_connection"].security_infra_mode.value = 0;
	}
	else
	{
		if ( g_auth.indexOf("WPA-NONE") >= 0 || g_auth.indexOf("WPA2-NONE") >= 0)
			cwin.document.forms["sta_site_survey_connection"].security_adhoc_mode.value = 5;
		else
			cwin.document.forms["sta_site_survey_connection"].security_adhoc_mode.value = 0;
	}

	//encry
	if (g_encry.indexOf("Not Use") >= 0)
		cwin.document.forms["sta_site_survey_connection"].openmode.value = 1;
	else if (g_encry.indexOf("AES") >= 0)
		cwin.document.forms["sta_site_survey_connection"].cipher[1].checked = true;
	else if (g_encry.indexOf("TKIP") >= 0)
		cwin.document.forms["sta_site_survey_connection"].cipher[0].checked = true;
	else
		cwin.document.forms["sta_site_survey_connection"].openmode.value = 0;

	cwin.document.forms["sta_site_survey_connection"].network_type.value = g_networktype;
}

function showProfileSsid()
{
	pcwin.document.forms["profile_page"].Ssid.value = g_ssid;


	if(g_networktype == 1 )
	{
		if (g_auth.indexOf("WPA2-PSK") >= 0)
			pcwin.document.forms["profile_page"].security_infra_mode.value = 7;
		else if (g_auth.indexOf("WPA-PSK") >= 0)
			pcwin.document.forms["profile_page"].security_infra_mode.value = 4;
		else if (g_auth.indexOf("WPA2") >= 0)
			pcwin.document.forms["profile_page"].security_infra_mode.value = 6;
		else if (g_auth.indexOf("WPA") >= 0)
			pcwin.document.forms["profile_page"].security_infra_mode.value = 3;
		else		
			pcwin.document.forms["profile_page"].security_infra_mode.value = 0;
	}
	else
	{
		if ( g_auth.indexOf("WPA-NONE") >= 0 || g_auth.indexOf("WPA2-NONE") >= 0)
			pcwin.document.forms["profile_page"].security_adhoc_mode.value = 5;
		else
			pcwin.document.forms["profile_page"].security_adhoc_mode.value = 0;
	}

	//encry
	if (g_encry.indexOf("TKIP") >= 0)
		pcwin.document.forms["profile_page"].cipher[0].checked = true;
	else if (g_encry.indexOf("AES") >= 0)
		pcwin.document.forms["profile_page"].cipher[1].checked = true;

	pcwin.document.forms["profile_page"].network_type.value = g_networktype;

	pcwin.document.forms["profile_page"].channel.value = g_channel;
}

function recordProfileSsid()
{
	parent.site_survey_ssid = g_ssid;

	if(g_networktype == 1 )
	{
		if (g_auth.indexOf("WPA2-PSK") >= 0)
			parent.site_survey_security_infra_mode = 7;
		else if (g_auth.indexOf("WPA-PSK") >= 0)
			parent.site_survey_security_infra_mode = 4;
		else if (g_auth.indexOf("WPA2") >= 0)
			parent.site_survey_security_infra_mode = 6;
		else if (g_auth.indexOf("WPA") >= 0)
			parent.site_survey_security_infra_mode = 3;
		else		
			parent.site_survey_security_infra_mode = 0;
	}
	else
	{
		if ( g_auth.indexOf("WPA-NONE") >= 0 || g_auth.indexOf("WPA2-NONE") >= 0)
			parent.site_survey_security_adhoc_mode = 5;
		else
			parent.site_survey_security_adhoc_mode = 0;
	}

	//encry
	if (g_encry.indexOf("TKIP") >= 0)
		parent.site_survey_cipher = 4;
	else if (g_encry.indexOf("AES") >= 0)
		parent.site_survey_cipher = 6;
	else if (g_encry.indexOf("WEP") >= 0)
		parent.site_survey_cipher = 1;
	else	
		parent.site_survey_cipher = 0;

	parent.site_survey_network_type = g_networktype;

	parent.site_survey_channel = g_channel;
	
	parent.site_survey_setting = 1;
}

function initTranslation()
{
	var e = document.getElementById("scanTitle");
	e.innerHTML = _("scan title");
	e = document.getElementById("scanTitle1");
	e.innerHTML = _("scan title");
	e = document.getElementById("scanSSID");
	e.innerHTML = _("station ssid");
	e = document.getElementById("scanBSSID");
	e.innerHTML = _("scan bssid");
	e = document.getElementById("scanRSSI");
	e.innerHTML = _("scan rssi");
	e = document.getElementById("scanChannel");
	e.innerHTML = _("addprof station channel");
	e = document.getElementById("scanEncryp");
	e.innerHTML = _("station encryp");
	e = document.getElementById("scanAuth");
	e.innerHTML = _("station auth");
	e = document.getElementById("scanNetType");
	e.innerHTML = _("addprof network type");
	e = document.getElementById("scanRescan");
	e.value = _("scan rescan");
	e = document.getElementById("scanAddProfile");
	e.value = _("scan add profile");	
}

function PageInit()
{
	initTranslation();
}

</script>
</head>
<body onload="PageInit()">
<form method=post name="sta_site_survey">

<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="scanTitle"></li>

<li class="w_text2">
<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="26" colspan="8"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id="scanTitle1"><span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>

<tr >

<td width="5%"><center><span class="top_left">#</span></center></td>

<td width="30%">
<center>
<span class="top_font" id="scanSSID">SSID</span>
</center>
</td>


<td width="17%">
<center>
<span class="top_font" id="scanBSSID">BSSID</span>
</center>
</td>


<td width="7%">
<center>
<span class="top_font" id="scanRSSI">Signal Strength</span>
</center>
</td>

<td width="5%">
<center>
<span class="top_font" id="scanChannel">Channel</span>
</center>
</td>

<td width="10%">
<center>
<span class="top_font" id="scanEncryp">Encryption</span>
</center>
</td>

<td width="15%">
<center>
<span class="top_font" id="scanAuth">Authentication</span>
</center>
</td>

<td width="10%">
<center>
<span class="top_font" id="scanNetType">Network Type</span>
</center>
</td>

</tr>
<% getwifiStaBSSIDList(); %>

<tr>
<td height="5" colspan="8"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
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
<li></li>
</ul>
</div>
</li>


<center>
<li class="table_button">
<input type=button value="Rescan" id="scanRescan" onClick="showWebMessage(2, ''); location.href=location.href">&nbsp;
<input type=button name="addProfileButton" value="Add Profile" id="scanAddProfile" disabled onClick="open_profile_page()">
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

