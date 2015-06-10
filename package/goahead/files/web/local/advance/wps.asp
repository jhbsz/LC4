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
td {white-space: nowrap;}
</style>    
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;

var secs
var timerID = null
var timerRunning = false
var timeout = 3
var delay = 1000

var STATimerFlag = 0

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}


function submit_apply()
{
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	    return false;
	}
	
    var Auth = "<% getCfgZero(1, 'AuthMode'); %>"

    if (document.WPSCfg.WPS_Active.checked == true){    
	if (Auth.split(";",1) == "WPA"){
	    alert("Cannot enable WPS, when security mode is WPA");	
	    return false;	
	}

	if (Auth.split(";",1) == "WPA2"){
	    alert("Cannot enable WPS, when security mode is WPA2");	
	    return false;	
	}	
	
	if (Auth.split(";",1) == "WPAWPA2"){
	    alert("Cannot enable WPS, when security mode is WPA-WPA2");	
	    return false;	
	}				
    }
    	showWebMessage(2, "");
	return true;
}

function InitializeTimer()
{
    // Set the length of the timer, in seconds
    secs = timeout;
    StopTheClock();
    StartTheTimer();
}

function StopTheClock()
{
    if(timerRunning);
        clearTimeout(timerID);
    timerRunning = false;
}

function StartTheTimer()
{
    if (secs==0)
    {
		StopTheClock();

		if(STATimerFlag == 0)
			updateWPS();
		else
			updateWPSStaStatus();

		secs = timeout;
		StartTheTimer();
    }else{
        self.status = secs;
        secs = secs - 1;
        timerRunning = true;
        timerID = self.setTimeout("StartTheTimer()", delay);
    }
}


function style_display_on(){
    if(window.ActiveXObject) { // IE
        return "block";
    } else if (window.XMLHttpRequest) { // Mozilla, Safari,...
        return "table-row";
    }
}

var http_request = false;
function makeRequest(url, content) {
	http_request = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if (http_request.overrideMimeType) {
			http_request.overrideMimeType('text/xml');
		}
	} else if (window.ActiveXObject) { // IE
		try {
			http_request = new ActiveXObject("Msxml2.XMLHTTP");
		} catch (e) {
			try {
			http_request = new ActiveXObject("Microsoft.XMLHTTP");
			} catch (e) {}
		}
	}
	if (!http_request) {
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request.onreadystatechange = alertContents;
	http_request.open('POST', url, true);
	http_request.send(content);
}

function alertContents() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			WPSUpdateHTML( http_request.responseText);
		} else {
			//alert('There was a problem with the request.');
		}
	}
}

function WPSUpdateHTML(str)
{
	var all_str = new Array();
	all_str = str.split("\n");
	var APPin = "<% getPINASP(); %>";
	
	rWPSAPPin = document.getElementById("WPSAPPin");
	if (APPin.length != 8){			
		rWPSAPPin.innerHTML = "0"+APPin;		
	}else{
		rWPSAPPin.innerHTML = APPin;	
	}	

	rWPSCfgStatus = document.getElementById("WPSCfgStatus");
	runWPSCfgStatus = document.getElementById("unWPSCfgStatus");
		
	if(all_str[0] == "1" || all_str[0] == "0"){
		rWPSCfgStatus.innerHTML = _("wps unconfig1");
		runWPSCfgStatus.innerHTML = _("wps unconfig2");
	}else if(all_str[0] == "2"){
		rWPSCfgStatus.innerHTML = _("wps config1");
		runWPSCfgStatus.innerHTML = _("wps config2");		
	}else{
		rWPSCfgStatus.innerHTML = _("wps unknown1");
		runWPSCfgStatus.innerHTML = _("wps unknown2");		
	}	

	if(all_str[0] == "1" || all_str[0] == "0"){
		show_div(false, "div_wps_config_id");
		show_div(false, "div_wps_id");			
		show_div(true, "div_wps_unconfig_id");
	}else{
		show_div(false, "div_wps_unconfig_id");
		show_div(true, "div_wps_config_id");		
		show_div(true, "div_wps_id");		

	rWPSCfgSSID = document.getElementById("WPSCfgSSID");
	rWPSCfgSSID.innerHTML = all_str[1];

	rWPSCfgSecurity = document.getElementById("WPSCfgSecurity");
	var WPSAuthMode = all_str[2];

	var EncrypType = all_str[3];


	if ((WPSAuthMode == "Open") && (EncrypType == "None")){
		rWPSCfgSecurity.innerHTML = "No Security";
	}else if ((WPSAuthMode == "Open") && (EncrypType == "WEP")){
		rWPSCfgSecurity.innerHTML = "Static WEP";
	}else if (WPSAuthMode == "Shared"){
		rWPSCfgSecurity.innerHTML = "Static WEP";
	}else if (WPSAuthMode == "WPA-PSKWPA2-PSK"){
		rWPSCfgSecurity.innerHTML = "WPA-PSK/WPA2-PSK";
	}else if (WPSAuthMode == "WPAWPA2"){
		rWPSCfgSecurity.innerHTML = "WPA/WPA2"
	}else{
	rWPSCfgSecurity.innerHTML = all_str[2];
	}
	
	rWPSCfg80211Mode = document.getElementById("WPSCfg80211Mode");	
	if (<% getCfgZero(1, "WirelessMode"); %> == 0){
		rWPSCfg80211Mode.innerHTML = "11 b/g";
	}else if (<% getCfgZero(1, "WirelessMode"); %> == 1){
		rWPSCfg80211Mode.innerHTML = "11 b";
	}else if (<% getCfgZero(1, "WirelessMode"); %> == 4){
		rWPSCfg80211Mode.innerHTML = "11 g";
	}else if (<% getCfgZero(1, "WirelessMode"); %> == 9){
		rWPSCfg80211Mode.innerHTML = "11 b/g/n";
	}		
       }
  parent.adjustMyFrameHeight();
}


function updateWPS(){
	makeRequest("/goform/updateWPS", "something");
}

function initTranslation()
{

	var e = document.getElementById("WPSTitle");
	e.innerHTML = _("wps wpstitle");

	e = document.getElementById("WPSEnable");
	e.innerHTML = _("wps wpsenable");

	e = document.getElementById("WPSPinNumber");
	e.innerHTML = _("wps pin number");

	e = document.getElementById("WPSGeneratePin");
	e.value = _("wps generate pincode");
	
	e = document.getElementById("WPSOOBReset");
	e.value = _("wps oob reset");		

	e = document.getElementById("WPSStatus");
	e.innerHTML = _("wps wpsstatus");
		
	e = document.getElementById("WPSConfigStatus1");
	e.innerHTML = _("wps config status1");
	
	e = document.getElementById("WPSConfigStatus2");
	e.innerHTML = _("wps config status2");
	
	e = document.getElementById("WPSConfigMode");
	e.innerHTML = _("wps config mode");				

	e = document.getElementById("WPSConfigSSID");
	e.innerHTML = _("wps config ssid");
	
	e = document.getElementById("WPSConfigSecurity");
	e.innerHTML = _("wps config security");
	
	if ((wirelessmode==0) || (wirelessmode==6)){ // Router and WISP
		e = document.getElementById("WPSConfigNote1");
		e.innerHTML = _("wps config note1");

		e = document.getElementById("WPSConfigNote2");
		e.innerHTML = _("wps config note2");
	}	

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initValue()
{
	initTranslation();

	wpsenable = <% getWPSModeASP(); %>;

	if(wpsenable ==0){
		// disable WPS
		document.WPSCfg.WPS_Active.checked = false;
		
		updateWPS();
	}else{
		// enable WPS
		document.WPSCfg.WPS_Active.checked = true;

		updateWPS();
		InitializeTimer(3);
	}

	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	}		

}

function Release_Configuration_FormCheck()
{
	if (!confirm(_("Do you wish to continue with this process ?")))
		return false;
	document.forms[0].release.value='Release_Configuration';
  showWebMessage(2, "");
	document.forms[0].submit();
	return true;
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name="WPSCfg" action="/goform/WPSConfig" onSubmit="return submit_apply()">
<div id="table">
<ul>

<li class="table_content">
<div class="data">
<ul>
<li class="title" id ="WPSTitle">WPS Setup</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%"><input name="WPS_Active" type="checkbox" value=1 />
<font id ="WPSEnable">Enable WPS</font> 
</td>
<td>&nbsp;</td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="30%" id ="WPSPinNumber">PIN Number	:</td>
<td width="30%" id = WPSAPPin></td>
<td width="40%">
<input name="Generate" type="hidden" value="" />
<input name="WPS_Gen" id ="WPSGeneratePin" type="button" value="Generate" onclick="document.forms[0].Generate.value='Generate';document.forms[0].submit();"/>
</td>
</tr>
</table>
</li>
                
<li class="space3"></li>
<li class="title" id ="WPSStatus"> WPS Status</li>
<span id="div_wps_config_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="30%" id ="WPSConfigStatus1">Status  :</td>
<td width="30%" id = WPSCfgStatus></td>
<td width="40%">
<input name="release" type="hidden" value="" />
<input name="WPS_CFG" id="WPSOOBReset" type="button" value="Release_Configuration"  onClick="return Release_Configuration_FormCheck();" />
</td>
</tr>
</table>
</li>
</span>                
                
<span id="div_wps_unconfig_id" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="30%" id ="WPSConfigStatus2">Status  :</td>
<td width="30%" id = unWPSCfgStatus></td>
<td width="40%"></td>
</tr>
</table>
</li>
</span>                

<span id="div_wps_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="30%" id ="WPSConfigMode">802.11 Mode  :</td>
<td width="30%" id= WPSCfg80211Mode></td>
<td width="40%"></td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="30%" id ="WPSConfigSSID">SSID  :</td>
<td width="30%" id = WPSCfgSSID></td>
<td width="40%"></td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="30%" id ="WPSConfigSecurity">Security  :</td>
<td width="30%" id = WPSCfgSecurity></td>
<td width="40%"></td>
</tr>
</table>
</li>
<li></li>
</span>
<script>
if ((wirelessmode==0) || (wirelessmode==6)){ // Router and WISP
document.write("<li class='w_text'>");
document.write("<table width='95%' border='0' align='center' cellpadding='0' cellspacing='0'>");
document.write("<tr><td><span class='i_note'><a id ='WPSConfigNote1'>Note: If you enable WPS, the </a><a target='_parent' href='/local/advance/main_config_frame.asp?2' class='note'>UPnP</a>&nbsp;<a id ='WPSConfigNote2'>service will be turned on automatically.</a></span></td></tr>");
document.write("</table></li>");
}
</script>
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