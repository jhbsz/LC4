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

var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;
var ssid = "<% getCfgGeneral(1, "ApCliSsid"); %>";
var bssid = "<% getCfgGeneral(1, "ApCliBssid"); %>";
var mode = "<% getCfgGeneral(1, "ApCliAuthMode"); %>";
var enc = "<% getCfgGeneral(1, "ApCliEncrypType"); %>";
var wpapsk = "<% getCfgGeneral(1, "ApCliWPAPSK"); %>";
var keyid = "<% getCfgGeneral(1, "ApCliDefaultKeyId"); %>";
var key1type = "<% getCfgGeneral(1, "ApCliKey1Type"); %>";
var key2type = "<% getCfgGeneral(1, "ApCliKey2Type"); %>";
var key3type = "<% getCfgGeneral(1, "ApCliKey3Type"); %>";
var key4type = "<% getCfgGeneral(1, "ApCliKey4Type"); %>";
var key1 = "<% getCfgGeneral(1, "ApCliKey1Str"); %>";
var key2 = "<% getCfgGeneral(1, "ApCliKey2Str"); %>";
var key3 = "<% getCfgGeneral(1, "ApCliKey3Str"); %>";
var key4 = "<% getCfgGeneral(1, "ApCliKey4Str"); %>";
var apclienable  = <% getCfgZero(1, "ApCliEnable"); %>;

function style_display_on()
{
	if (window.ActiveXObject){//IE
		return "block";
	}else if (window.XMLHttpRequest){//Mozilla, Safari,...
		return "table-row";
	}
}

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function KeyTypeSwitch(id)
{
	var keytype = eval("document.wireless_apcli.apcli_key"+id+"type.options.selectedIndex");
	document.wireless_apcli.apcli_key1type.options.selectedIndex = keytype;
	document.wireless_apcli.apcli_key2type.options.selectedIndex = keytype;
	document.wireless_apcli.apcli_key3type.options.selectedIndex = keytype;
	document.wireless_apcli.apcli_key4type.options.selectedIndex = keytype;
}

function SecurityModeSwitchInit()
{
	show_div(false, "div_apcli_enc_id");
	show_div(false, "div_wep");
	show_div(false, "div_wpa_psk");	
	document.wireless_apcli.apcli_default_key.disabled = true;
	document.wireless_apcli.apcli_key1.disabled = true;
	document.wireless_apcli.apcli_key2.disabled = true;
	document.wireless_apcli.apcli_key3.disabled = true;
	document.wireless_apcli.apcli_key4.disabled = true;
	document.wireless_apcli.apcli_wpapsk.disabled = true;

	var mysel = document.wireless_apcli.apcli_enc;
	mysel.options.length = 0;
	
	if ( ((document.wireless_apcli.apcli_mode.options.selectedIndex == 0)&&(key1 == "")&&(key2 == "")&&(key3 == "")&&(key4 == "")) ||
		((document.wireless_apcli.apcli_mode.options.selectedIndex == 0) && (enc == "NONE")) ){ // No Security

		show_div(false, "div_apcli_enc_id");
		show_div(false, "div_wep");
		show_div(false, "div_wpa_psk");	

	}
	else if ( (document.wireless_apcli.apcli_mode.options.selectedIndex == 1) || (document.wireless_apcli.apcli_mode.options.selectedIndex == 0) ){	
		mysel.options[0] = new Option("Open", "OPEN");
		mysel.options[1] = new Option("Shared Key", "WEP");

		show_div(true, "div_apcli_enc_id");
	show_div(true, "div_wep");		
		document.wireless_apcli.apcli_default_key.disabled = false;
		document.wireless_apcli.apcli_key1.disabled = false;
		document.wireless_apcli.apcli_key2.disabled = false;
		document.wireless_apcli.apcli_key3.disabled = false;
		document.wireless_apcli.apcli_key4.disabled = false;

		//default Open
		document.wireless_apcli.apcli_enc.options.selectedIndex = 0;
		
		if (mode == "SHARED"){
			document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
			document.wireless_apcli.apcli_enc.options.selectedIndex = 1;
		}else if (mode == "OPEN"){
			document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
			document.wireless_apcli.apcli_enc.options.selectedIndex = 0;		
		}
	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 2) {
		mysel.options[0] = new Option("TKIP", "TKIP");
		mysel.options[1] = new Option("AES", "AES");
		
		show_div(true, "div_apcli_enc_id");
		show_div(true, "div_wpa_psk");			
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		document.wireless_apcli.apcli_enc.options.selectedIndex = 0;
		
		if (mode == "WPAPSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 0;
			}else if (enc =='AES'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 1;	
			}else{
				document.wireless_apcli.apcli_enc.options.selectedIndex = 0;			
			}
		}

	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 3) {
		mysel.options[0] = new Option("TKIP", "TKIP");
		mysel.options[1] = new Option("AES", "AES");
		
		show_div(true, "div_apcli_enc_id");
		show_div(true, "div_wpa_psk");
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		document.wireless_apcli.apcli_enc.options.selectedIndex = 1;
		
		if (mode == "WPA2PSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 0;	
			}else if (enc =='AES'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 1;	
			}else{
				document.wireless_apcli.apcli_enc.options.selectedIndex = 1;			
			}
		}
	}
	parent.adjustMyFrameHeight();
}


function SecurityModeSwitch()
{
	show_div(false, "div_apcli_enc_id");
	show_div(false, "div_wep");
	show_div(false, "div_wpa_psk");
	
	document.wireless_apcli.apcli_default_key.disabled = true;
	document.wireless_apcli.apcli_key1.disabled = true;
	document.wireless_apcli.apcli_key2.disabled = true;
	document.wireless_apcli.apcli_key3.disabled = true;
	document.wireless_apcli.apcli_key4.disabled = true;
	document.wireless_apcli.apcli_wpapsk.disabled = true;

	var mysel = document.wireless_apcli.apcli_enc;
	mysel.options.length = 0;
	
	if (document.wireless_apcli.apcli_mode.options.selectedIndex == 0) {
		show_div(false, "div_apcli_enc_id");
		show_div(false, "div_wep");
		show_div(false, "div_wpa_psk");	
	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 1) {
		mysel.options[0] = new Option("Open", "OPEN");
		mysel.options[1] = new Option("Shared Key", "WEP");
	
		show_div(true, "div_apcli_enc_id");
	show_div(true, "div_wep");	
		document.wireless_apcli.apcli_default_key.disabled = false;
		document.wireless_apcli.apcli_key1.disabled = false;
		document.wireless_apcli.apcli_key2.disabled = false;
		document.wireless_apcli.apcli_key3.disabled = false;
		document.wireless_apcli.apcli_key4.disabled = false;

		//default Open
		document.wireless_apcli.apcli_enc.options.selectedIndex = 0;
		
		if ( (mode == "SHARED")||(mode == "OPEN") && (enc == "NONE")){
			//document.wireless_apcli.apcli_mode.options.selectedIndex == 1;
				document.wireless_apcli.apcli_enc.options.selectedIndex = 1;	
		}else if (mode == "OPEN"){
			//document.wireless_apcli.apcli_mode.options.selectedIndex == 1;
				document.wireless_apcli.apcli_enc.options.selectedIndex = 0;			
			}
		}		
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 2) {
		mysel.options[0] = new Option("TKIP", "TKIP");
		mysel.options[1] = new Option("AES", "AES");
		
		show_div(true, "div_apcli_enc_id");
	show_div(true, "div_wpa_psk");			
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		//WPA-PSK default TKIP
		document.wireless_apcli.apcli_enc.options.selectedIndex = 0;
		
		if (mode == "WPAPSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 0;	
			}else if (enc =='AES'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 1;	
			}else{
				document.wireless_apcli.apcli_enc.options.selectedIndex = 0;			
			}
		}		
		
	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 3) {
		mysel.options[0] = new Option("TKIP", "TKIP");
		mysel.options[1] = new Option("AES", "AES");
		
		show_div(true, "div_apcli_enc_id");
		show_div(true, "div_wpa_psk");
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		//WPA2-PSK default AES
		document.wireless_apcli.apcli_enc.options.selectedIndex = 1;
		
		if (mode == "WPA2PSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 0;	
			}else if (enc =='AES'){
				document.wireless_apcli.apcli_enc.options.selectedIndex = 1;	
			}else{
				document.wireless_apcli.apcli_enc.options.selectedIndex = 1;			
			}
		}
	}
	parent.adjustMyFrameHeight();
}

function initTranslation()
{
	//var e = document.getElementById("APClientTitle");
	//e.innerHTML = _("APClient Title");
	var e = document.getElementById("APClient_Enable");	
	e.innerHTML = _("wireless enable");	
	if (wirelessmode == 5){
		//e = document.getElementById("URTitle");
		//e.innerHTML = _("UR Title");
	}else if (wirelessmode == 6){
		//e = document.getElementById("WISPTitle");
		//e.innerHTML = _("WISP Title");
	}
	e = document.getElementById("APClientSSID");
	e.innerHTML = _("station ssid");
	e = document.getElementById("APClientMAC");
	e.innerHTML = _("APClient MAC");
	e = document.getElementById("addprofSecureMode");
	e.innerHTML = _("addprof security mode");
	e = document.getElementById("GeneralEncrypOpen");
	e.innerHTML = _("general nosecurity");	
	e = document.getElementById("GeneralSharedKey");
	e.innerHTML = _("general static wep");
	e = document.getElementById("GeneralWPAPSK");
	e.innerHTML = _("general wpapsk");
	e = document.getElementById("GeneralWPA2PSK");
	e.innerHTML = _("general wpa2psk");	
	e = document.getElementById("addprofWPAAlg");
	e.innerHTML = _("addprof secure wpa algorithm");
	e = document.getElementById("WEPKeyTitle");
	e.innerHTML = _("WEPKey Title");
	e = document.getElementById("WEPKeyDefault");
	e.innerHTML = _("WEPKey Default");
	e = document.getElementById("addprofWEPKey1");
	e.innerHTML = _("addprof secure wep key1");
	e = document.getElementById("addprofWEPKey2");
	e.innerHTML = _("addprof secure wep key2");
	e = document.getElementById("addprofWEPKey3");
	e.innerHTML = _("addprof secure wep key3");
	e = document.getElementById("addprofWEPKey4");
	e.innerHTML = _("addprof secure wep key4");
	e = document.getElementById("apcli_WEPkey1");
	e.innerHTML = _("apcli WEPkey1");
	e = document.getElementById("apcli_WEPkey2");
	e.innerHTML = _("apcli WEPkey2");
	e = document.getElementById("apcli_WEPkey3");
	e.innerHTML = _("apcli WEPkey3");
	e = document.getElementById("apcli_WEPkey4");
	e.innerHTML = _("apcli WEPkey4");
	e = document.getElementById("addprofHex");
	e.innerHTML = _("addprof hex");
	e = document.getElementById("addprofASCII");
	e.innerHTML = _("addprof ascii");
	e = document.getElementById("addprofHex1");
	e.innerHTML = _("addprof hex");
	e = document.getElementById("addprofASCII1");
	e.innerHTML = _("addprof ascii");
	e = document.getElementById("addprofHex2");
	e.innerHTML = _("addprof hex");
	e = document.getElementById("addprofASCII2");
	e.innerHTML = _("addprof ascii");
	e = document.getElementById("addprofHex3");
	e.innerHTML = _("addprof hex");
	e = document.getElementById("addprofASCII3");
	e.innerHTML = _("addprof ascii");
	e = document.getElementById("wpapsk_Pass_Phrase");
	e.innerHTML = _("general wpapsk presharedkey");
  	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}	
function initValue()
{
  initTranslation();
  
	if(apclienable == 1){
		// Enable
		document.wireless_apcli.apcli_enable.checked = true;
	}else{
		// Disable
		document.wireless_apcli.apcli_enable.checked = false;
	}	
  
	document.wireless_apcli.apcli_ssid.value = ssid;
	document.wireless_apcli.apcli_bssid.value = bssid;
	if (mode == "OPEN")
		document.wireless_apcli.apcli_mode.options.selectedIndex = 0;
	else if (mode == "SHARED")
		document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
	else if (mode == "WPAPSK")
		document.wireless_apcli.apcli_mode.options.selectedIndex = 2;
	else if (mode == "WPA2PSK")
		document.wireless_apcli.apcli_mode.options.selectedIndex = 3;
	else
		document.wireless_apcli.apcli_mode.options.selectedIndex = 0;		

	if (keyid == ''){
		keyid = 1;
	}else{	
		keyid = keyid - '0';
	}
	
	if (key1type == ''){
		key1type = 1;
		key2type = 1;
		key3type = 1;
		key4type = 1;		
	}else{	
		key1type = key1type - '0';
		key2type = key2type - '0';
		key3type = key3type - '0';
		key4type = key4type - '0';
	}

	document.wireless_apcli.apcli_default_key.options.selectedIndex = keyid -1;

	if (key1type == 0){
		document.wireless_apcli.apcli_key1type.options.selectedIndex = 1;
		document.wireless_apcli.apcli_key2type.options.selectedIndex = 1;
		document.wireless_apcli.apcli_key3type.options.selectedIndex = 1;
		document.wireless_apcli.apcli_key4type.options.selectedIndex = 1;
	}else{
		document.wireless_apcli.apcli_key1type.options.selectedIndex = 0;
		document.wireless_apcli.apcli_key2type.options.selectedIndex = 0;
		document.wireless_apcli.apcli_key3type.options.selectedIndex = 0;
		document.wireless_apcli.apcli_key4type.options.selectedIndex = 0;	
	}	
			
	document.wireless_apcli.apcli_key1.value = key1;
	document.wireless_apcli.apcli_key2.value = key2;
	document.wireless_apcli.apcli_key3.value = key3;
	document.wireless_apcli.apcli_key4.value = key4;
	
	document.wireless_apcli.apcli_wpapsk.value = wpapsk;
		
	SecurityModeSwitchInit();

	if ((wirelessmode != 5) && (wirelessmode != 6)){
	    alert("Please select Universal Repeater mode to configure this page");
	}
	
}

function CheckHex(str)
{
	var len = str.length;

	for (var i = 0; i < str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
				(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
				(str.charAt(i) >= 'A' && str.charAt(i) <= 'F')) {
			continue;
		}
		else
			return false;
	}
	return true;
}

function CheckInjection(str)
{
	var len = str.length;
	for (var i = 0; i < str.length; i++) {
		if (str.charAt(i) == ';' || str.charAt(i) == ',' ||
				str.charAt(i) == '\r' || str.charAt(i) == '\n') {
			return false;
		}
		else
			continue;
	}
	return true;
}

function CheckWep()
{
	var defaultid = 1*document.wireless_apcli.apcli_default_key.value;

	if (document.wireless_apcli.apcli_enc.value == "WEP") {
		if (eval("document.wireless_apcli.apcli_key"+defaultid).value.length == 0) {
			alert(_('Please input wep key')+defaultid+' !');
			eval("document.wireless_apcli.apcli_key"+defaultid).focus();
			return false;
		}
	}

	if (document.wireless_apcli.apcli_key1type.options.selectedIndex == 0) {
		for (var i = 1; i < 5; i++) {
			var mykey = eval("document.wireless_apcli.apcli_key"+i);
			if (mykey.value.length != 0 && mykey.value.length != 5 && mykey.value.length != 13) {
				alert(_('Please input 5 or 13 characters of wep key')+i);
				mykey.focus();
				return false;
			}
			if (!CheckInjection(mykey.value)) {
				alert(_('Wep key')+i+_(' contains invalid characters')+'!');
				mykey.focus();
				return false;
			}
		}
	}
	else {
		for (var i = 1; i < 5; i++) {
			var mykey = eval("document.wireless_apcli.apcli_key"+i);
			if (mykey.value.length != 0 && mykey.value.length != 10 && mykey.value.length != 26) {
				alert(_('Please input 10 or 26 characters of wep key')+i);
				mykey.focus();
				return false;
			}
			if (!CheckHex(wireless_apcli.apcli_key1.value)) {
				alert(_('Invalid Wep key')+i+_('format')+'!');
				mykey.focus();
				return false;
			}
		}
	}
}

function CheckWpa()
{
	if (document.wireless_apcli.apcli_wpapsk.value.length < 8) {
		alert("Pass Phrase length should be larger than 8!");
		document.wireless_apcli.apcli_wpapsk.focus();
		return false;
	}
	if (!CheckInjection(document.wireless_apcli.apcli_wpapsk.value)) {
		alert('Invalid characters in Pass Phrase.');
		document.wireless_apcli.apcli_wpapsk.focus();
		return false;
	}
	
	if (CheckHex(document.wireless_apcli.apcli_wpapsk.value) == true){ // Hex
		if ((document.wireless_apcli.apcli_wpapsk.value.length < 8) || (document.wireless_apcli.apcli_wpapsk.value.length > 64)){
			alert('Please input at least 8 character wpapsk key!');
			return false;
		}
	}else{ // ASCII
		if ((document.wireless_apcli.apcli_wpapsk.value.length < 8) || (document.wireless_apcli.apcli_wpapsk.value.length > 63)){
			alert('Please input 8 - 63 characters of wpapsk key!');
			return false;
		}
	}
	
	return true;
}

function submit_apply()
{
	if ((wirelessmode != 5) && (wirelessmode != 6)){
		alert("Please select Universal Repeater mode to configure this page");	
		return false;
	}

	if ( (document.wireless_apcli.apcli_mode.options.selectedIndex == 1) && (document.wireless_apcli.apcli_enc.options.selectedIndex == 0) ){
		//document.wireless_apcli.apcli_mode.options.selectedIndex = 0; // Auth = OPEN
		//document.wireless_apcli.apcli_enc.options.selectedIndex = 1; // Enc = WEP
		document.wireless_apcli.apcli_mode.value = "OPEN"; // Auth = OPEN
		document.wireless_apcli.apcli_enc.value = "WEP"; // Enc = WEP
	}
	
	showWebMessage(2, "");
	return true;	
}

function CheckValue()
{
	if (submit_apply() == false)
	    return false;

	if (document.wireless_apcli.apcli_ssid.value == '') {
		alert('empty SSID!');
		document.wireless_apcli.apcli_ssid.focus(); 
		return false;
	}

	if (document.wireless_apcli.apcli_bssid.value != '') {
		var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
		if (!re.test(document.wireless_apcli.apcli_bssid.value)) {
			alert("BSSID format error!");
			document.wireless_apcli.apcli_bssid.focus(); 
			return false;
		}
	}

	if (document.wireless_apcli.apcli_mode.options.selectedIndex == 0)
		return true;
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 1)
		return CheckWep();
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 2)
		return CheckWpa();
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 3)
		return CheckWpa();
	else {
		alert('Unknown Security Mode!');
		return false;
	}
}
</script>
</head>
<body onLoad="initValue()">
<form method=post name=wireless_apcli action="/goform/wifiApcli" onSubmit="return CheckValue()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<script>
	if (wirelessmode == 5){
		document.write("<li class='title' id='URTitle'>Universal Repeater Parameters</li>");
	}else if (wirelessmode == 6){
		document.write("<li class='title' id='WISPTitle'>WISP Parameters</li>"); 
	}
</script>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%"><input name="apcli_enable" type="checkbox" value=1 />
<font id ="APClient_Enable">Enable</font> 
</td>
<td>&nbsp;</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">   
<tr> 
<td width="40%" id="APClientSSID">SSID</td>
<td><input type=text name="apcli_ssid" size=20 maxlength=32></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr>
<td width="40%" id="APClientMAC">MAC Address (Optional)</td>
<td><input type=text name="apcli_bssid" size=20 maxlength=17></td>
</tr>
</table>
</li>  
  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr> 
<td width="40%" id="addprofSecureMode">Security Mode</td>
<td>
<select name="apcli_mode" id="apcli_mode" size="1" onchange="SecurityModeSwitch()">
<option value="OPEN" id="GeneralEncrypOpen">No Security</option>
<option value="SHARED" id="GeneralSharedKey">Static WEP</option>
<option value="WPAPSK" id="GeneralWPAPSK">WPAPSK</option>
<option value="WPA2PSK" id="GeneralWPA2PSK">WPA2PSK</option>
</select>
</td>
</tr>
</table>
</li>  
  
<span id="div_apcli_enc_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_enc">
<td width="40%" id="addprofWPAAlg">Encryption Type</td>
<td>
<select name="apcli_enc" id="apcli_enc" size="1">
</select>
</td>
</tr>
</table>
</li>
</span>


<span id="div_wep" class="off">
<li class="title" id="WEPKeyTitle">WEP Key</li> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_default_key">
<td width="40%" id="WEPKeyDefault">WEP Default Key</td>
<td>
<select name="apcli_default_key" id="apcli_default_key" size="1">
<option value="1" id ="addprofWEPKey1">Key 1</option>
<option value="2" id ="addprofWEPKey2">Key 2</option>
<option value="3" id ="addprofWEPKey3">Key 3</option>
<option value="4" id ="addprofWEPKey4">Key 4</option>
</select>
</td>
</tr>
</table>
</li>
  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_key1">
<td width="40%" id="apcli_WEPkey1">WEP Key 1 :</td>
<td>
<input name="apcli_key1" id="apcli_key1" maxlength="26">
<select id="apcli_key1type" name="apcli_key1type" onchange="KeyTypeSwitch(1)"> 
<option value="1" id ="addprofASCII">ASCII</option>
<option value="0" id ="addprofHex">Hex</option>
</select>
</td>
</tr>
</table>
</li>  
  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_key2">
<td width="40%" id="apcli_WEPkey2">WEP Key 2 :</td>
<td>
<input name="apcli_key2" id="apcli_key2" maxlength="26">
<select id="apcli_key2type" name="apcli_key2type" onchange="KeyTypeSwitch(2)"> 
<option value="1" id ="addprofASCII1">ASCII</option>
<option value="0" id ="addprofHex1">Hex</option>
</select>
</td>
</tr>
</table>
</li>    
  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_key3">
<td width="40%" id="apcli_WEPkey3">WEP Key 3 :</td>
<td>
<input name="apcli_key3" id="apcli_key3" maxlength="26">
<select id="apcli_key3type" name="apcli_key3type" onchange="KeyTypeSwitch(3)"> 
<option value="1" id ="addprofASCII2">ASCII</option>
<option value="0" id ="addprofHex2">Hex</option>
</select>
</td>
</tr>
</table>
</li>
  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_key4">
<td width="40%" id="apcli_WEPkey4">WEP Key 4 :</td>
<td>
<input name="apcli_key4" id="apcli_key4" maxlength="26">
<select id="apcli_key4type" name="apcli_key4type" onchange="KeyTypeSwitch(4)"> 
<option value="1" id ="addprofASCII3">ASCII</option>
<option value="0" id ="addprofHex3">Hex</option>
</select>
</td>
</tr>
</table>
</li>
</span>

<!-- WPA-PSK/ WPA2-PSK -->
<span id="div_wpa_psk" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_wpapsk"> 
<td width="40%" id="wpapsk_Pass_Phrase">Pre-Shared Key</td>
<td><input type=text name="apcli_wpapsk" size=20 maxlength=64></td>
</tr>
</table>
</li>  
</span>   

<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset value="Reset" id="GeneralReset" onClick="window.location.reload()">
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
