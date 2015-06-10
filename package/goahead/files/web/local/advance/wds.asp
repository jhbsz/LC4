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

var PhyMode  = <% getCfgZero(1, "WirelessMode"); %>;
var wdsMode  = <% getCfgZero(1, "WdsEnable"); %>;
var wdsList  = '<% getCfgGeneral(1, "WdsList"); %>';
var wdsPhyMode  = "<% getCfgZero(1, "WdsPhyMode"); %>";
var wdsEncrypType  = "<% getCfgGeneral(1, "WdsEncrypType"); %>";
//var wdsEncrypKey  = '<% getCfgGeneral(1, "Wds0Key"); %>';
var wdsEncrypKey = new Array();

function submit_apply()
{

	if ((wirelessmode == 2) || (wirelessmode == 6)){
		alert("You CANNOT select WISP or Client mode to configure this page");	
		return false;
	}

	return true;
}

function style_display_on()
{
	if (window.ActiveXObject)
	{ // IE
		return "block";
	}
	else if (window.XMLHttpRequest)
	{ // Mozilla, Safari,...
		return "table-row";
	}
}

function show_div(show,id)
{
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n' || str.charAt(i) == '^' || str.charAt(i) == '|' || str.charAt(i) == '%' || str.charAt(i) == '$' || str.charAt(i) == ';'){
			return false;
		}else
	        continue;
	}
	return true;
}

var http_request = false;
function makeRequest(url, content, handler) {
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
	http_request.onreadystatechange = handler;
	http_request.open('POST', url, true);
	http_request.send(content);
}

function securityHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
//			UpdateMBSSIDList();
			LoadField();
		} else {
			alert('There was a problem with the request.');
		}
	}
}

function parseAllData(str)
{
	var fields_str = new Array();
	wdsEncrypKey = str;

}

function LoadField()
{
	document.wireless_basic.wds_encryp_key.value = wdsEncrypKey;	
}

function initTranslation()
{
	var e = document.getElementById("WDSTitle");
	e.innerHTML = _("wds title");

	e = document.getElementById("WDSMode");
	e.innerHTML = _("wds mode");

	e = document.getElementById("WDSSelfMAC");
	e.innerHTML = _("wds self mac");

	e = document.getElementById("WDSModeDisable");
	e.innerHTML = _("wds mode disable");

	e = document.getElementById("WDSModeAPBridge");
	e.innerHTML = _("wds mode apbridge");

	e = document.getElementById("WDSModeBridge");
	e.innerHTML = _("wds mode bridge");		

	e = document.getElementById("WDSPhyMode");
	e.innerHTML = _("wds phy mode");

	e = document.getElementById("WDSAPMacAddr1");
	e.innerHTML = _("wds ap macaddr1");

	e = document.getElementById("WDSAPMacAddr2");
	e.innerHTML = _("wds ap macaddr2");

	e = document.getElementById("WDSAPMacAddr3");
	e.innerHTML = _("wds ap macaddr3");

	e = document.getElementById("WDSAPMacAddr4");
	e.innerHTML = _("wds ap macaddr4");				

	e = document.getElementById("WDSsecurityTitle");
	e.innerHTML = _("wds security title");

	e = document.getElementById("WDSEncrypType");
	e.innerHTML = _("wds encryp type");

	e = document.getElementById("WDSEncrypKey");
	e.innerHTML = _("wds encryp key");

	e = document.getElementById("WDSEncrypNo");
	e.innerHTML = _("wds encryp no");

	e = document.getElementById("WDSEncrypWEP");
	e.innerHTML = _("wds encryp wep");

	e = document.getElementById("WDSEncrypTKIP");
	e.innerHTML = _("wds encryp tkip");			

	e = document.getElementById("WDSEncrypAES");
	e.innerHTML = _("wds encryp aes");	

	e = document.getElementById("WDSPHYCCK");
	e.innerHTML = _("wds phy cck");

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");

	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initValue()
{
	var wdslistArray;

	initTranslation();

	//WDS Phy Mode
	document.wireless_basic.wds_phy_mode.options.length = 1;
	
	if (PhyMode != 1) {
		document.wireless_basic.wds_phy_mode.options[1] = new Option(_("wds phy ofdm"), "OFDM;OFDM;OFDM;OFDM");
		if (PhyMode != 0 && PhyMode != 2 && PhyMode != 4)
		{
			document.wireless_basic.wds_phy_mode.options[2] = new Option(_("wds phy htmix"), "HTMIX;HTMIX;HTMIX;HTMIX");
			document.wireless_basic.wds_phy_mode.options[3] = new Option(_("wds phy greenfield"), "GREENFIELD;GREENFIELD;GREENFIELD;GREENFIELD");
		}
	}

	if (wdsList != "")
	{
		wdslistArray = wdsList.split(";");
		for(i = 1; i <= wdslistArray.length-1; i++)
			eval("document.wireless_basic.wds_"+i).value = wdslistArray[i - 1];
	}

	wdsMode = 1*wdsMode;

	if (wdsMode == 0){
		document.wireless_basic.wds_mode.options.selectedIndex = 0;
	}else if (wdsMode == 1){
		document.wireless_basic.wds_mode.options.selectedIndex = 1;
	}else if (wdsMode == 2){
		document.wireless_basic.wds_mode.options.selectedIndex = 2;
	}	

	if (wdsPhyMode.split(";",1) == "CCK" || wdsPhyMode.split(";",1) == "cck"){
		document.wireless_basic.wds_phy_mode.options.selectedIndex = 0;
	}else if (wdsPhyMode.split(";",1) == "OFDM" || wdsPhyMode.split(";",1) == "ofdm"){
		document.wireless_basic.wds_phy_mode.options.selectedIndex = 1;
	}else if (wdsPhyMode.split(";",1) == "HTMIX" || wdsPhyMode.split(";",1) == "htmix"){
		document.wireless_basic.wds_phy_mode.options.selectedIndex = 2;
	}else if (wdsPhyMode.split(";",1) == "GREENFIELD" || wdsPhyMode.split(";",1) == "greenfield"){
		document.wireless_basic.wds_phy_mode.options.selectedIndex = 3;
	}

	//document.wireless_basic.wds_encryp_type.options.selectedIndex = 1*wdsEncrypType;
	
	if (wdsEncrypType.split(";",1) == "NONE"){
		document.wireless_basic.wds_encryp_type.options.selectedIndex = 0;	
	}else if (wdsEncrypType.split(";",1) == "WEP"){
		document.wireless_basic.wds_encryp_type.options.selectedIndex = 1;
	}else if (wdsEncrypType.split(";",1) == "TKIP"){
		document.wireless_basic.wds_encryp_type.options.selectedIndex = 2;
	}else if (wdsEncrypType.split(";",1) == "AES"){
		document.wireless_basic.wds_encryp_type.options.selectedIndex = 3;
	}		
		
	WdsModeOnChange();

	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	}

	makeRequest("/goform/GetWDSSecurity", "n/a", securityHandler);
	parent.adjustMyFrameHeight();
}


function WdsModeOnChange()
{
	show_div(false, "div_wds_phy_encryp");
	show_div(false, "div_wds_encryp_key_id");
	show_div(false, "div_wds_mac_addr");

	document.wireless_basic.wds_phy_mode.disabled = true;
	document.wireless_basic.wds_encryp_type.disabled = true;
	document.wireless_basic.wds_encryp_key.disabled = true;	
	document.wireless_basic.wds_1.disabled = true;
	document.wireless_basic.wds_2.disabled = true;
	document.wireless_basic.wds_3.disabled = true;
	document.wireless_basic.wds_4.disabled = true;							
	document.wireless_basic.wds_phy_mode.disabled = true;
	document.wireless_basic.wds_encryp_type.disabled = true;
	document.wireless_basic.wds_encryp_key.disabled = true;
	document.wireless_basic.wds_1.disabled = true;
	document.wireless_basic.wds_2.disabled = true;
	document.wireless_basic.wds_3.disabled = true;
	document.wireless_basic.wds_4.disabled = true;

	document.wireless_basic.wds_encryp_type.disabled = false;

	if (document.wireless_basic.wds_mode.options.selectedIndex != 0)
	{
		if (document.wireless_basic.wds_mode.options.selectedIndex == 1){
			show_div(false, "div_wds_phy_encryp");
		}else{
			show_div(true, "div_wds_phy_encryp");
		}
		
		document.wireless_basic.wds_phy_mode.disabled = false;
		document.wireless_basic.wds_encryp_type.disabled = false;

		if (document.wireless_basic.wds_encryp_type.options.selectedIndex >= 1)
		{
	                show_div(true, "div_wds_encryp_key_id");			
			document.wireless_basic.wds_encryp_key.disabled = false;
		}

	        show_div(true, "div_wds_mac_addr");	
		document.wireless_basic.wds_1.disabled = false;
		document.wireless_basic.wds_2.disabled = false;
		document.wireless_basic.wds_3.disabled = false;
		document.wireless_basic.wds_4.disabled = false;
	}else{ // document.wireless_basic.wds_mode.options.selectedIndex == 0
		document.wireless_basic.wds_encryp_type.options.selectedIndex = 0
	}
	parent.adjustMyFrameHeight();
}

function WdsSecurityOnChange()
{
	show_div(false, "div_wds_encryp_key_id");
	document.wireless_basic.wds_encryp_key.disabled = true;

	if (document.wireless_basic.wds_encryp_type.options.selectedIndex >= 1)
	{
		show_div(true, "div_wds_encryp_key_id");	
		document.wireless_basic.wds_encryp_key.disabled = false;
	}
	parent.adjustMyFrameHeight();
}

function CheckEncKey()
{
	var key;
	key = eval("document.wireless_basic.wds_encryp_key").value;

	if (eval("document.wireless_basic.wds_encryp_type").options.selectedIndex == 1) {
		if (key.length == 10 || key.length == 26) {
			var re = /[A-Fa-f0-9]{10,26}/;
			if (!re.test(key)) {
				alert(_("WDS WEP Key should be a 10/26 hexdecimal or a 5/13 ascii"));
				eval("document.wireless_basic.wds_encryp_key").focus();
				eval("document.wireless_basic.wds_encryp_key").select();
				return false;
			}
			else
				return true;
		}
		else if (key.length == 5 || key.length == 13) {
			return true;
		}
		else {
			alert(_("WDS WEP Key should be a 10/26 hexdecimal or a 5/13 ascii"));
			eval("document.wireless_basic.wds_encryp_key").focus();
			eval("document.wireless_basic.wds_encryp_key").select();
			return false;
		}
	}
	else if (eval("document.wireless_basic.wds_encryp_type").options.selectedIndex == 2 ||
			eval("document.wireless_basic.wds_encryp_type").options.selectedIndex == 3)
	{
		if (key.length < 8 || key.length > 64) {
			alert(_("WDS TKIP or AES Key should be with length 8~64"));
			eval("document.wireless_basic.wds_encryp_key").focus();
			eval("document.wireless_basic.wds_encryp_key").select();
			return false;
		}
		if (key.length == 64) {
			var re = /[A-Fa-f0-9]{64}/;
			if (!re.test(key)) {
				alert(_("WDS TKIP or AES Key should be a 64 hexdecimal"));
				eval("document.wireless_basic.wds_encryp_key").focus();
				eval("document.wireless_basic.wds_encryp_key").select();
				return false;
			}
			else
				return true;
		}
		else
			return true;
	}
	return true;
}

function CheckValue()
{
	var all_wds_list;
	all_wds_list = '';

	if (submit_apply() == false)
	    return false;

	if (!CheckEncKey())
		return false;

	if(checkInjection(document.wireless_basic.wds_encryp_key.value) == false){
		alert('Invalid characters in Pass Phrase.');
		return false;
	}

	if (document.wireless_basic.wds_mode.options.selectedIndex != 0)	
	{
		var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
		for (i = 1; i <= 4; i++)
		{
			if (eval("document.wireless_basic.wds_"+i).value == "")
				continue;
			if (!re.test(eval("document.wireless_basic.wds_"+i).value)) {
				alert(_("Please fill WDS remote AP MAC Address in correct format! (XX:XX:XX:XX:XX:XX)"));
				return false;
			}
			else {
				all_wds_list += eval("document.wireless_basic.wds_"+i).value;
				all_wds_list += ';';
			}
		}
		if (all_wds_list == "")
		{
			alert("WDS remote AP MAC Address are empty !!!");
			document.wireless_basic.wds_1.focus();
			document.wireless_basic.wds_1.select(); 
			return false;
		}
		else
		{
			document.wireless_basic.wds_list.value = all_wds_list;
			//document.wireless_basic.wds_1.disabled = true;
			//document.wireless_basic.wds_2.disabled = true;
			//document.wireless_basic.wds_3.disabled = true;
			//document.wireless_basic.wds_4.disabled = true;
		}
	}
	
	
	
	for (i = 1; i <= 4; i++){
		if (eval("document.wireless_basic.wds_"+i).value != ""){
			if (i == 1){
				if (eval(document.wireless_basic.wds_1).value == eval(document.wireless_basic.wds_2).value ){
					alert("Duplicate Remote Mac Address!");	
					return false;	
				}
				if (eval(document.wireless_basic.wds_1).value == eval(document.wireless_basic.wds_3).value ){
					alert("Duplicate Remote Mac Address!");	
					return false;	
				}
				if (eval(document.wireless_basic.wds_1).value == eval(document.wireless_basic.wds_4).value ){
					alert("Duplicate Remote Mac Address!");	
					return false;	
				}	
			}else if (i == 2){
				if (eval(document.wireless_basic.wds_2).value == eval(document.wireless_basic.wds_3).value ){
					alert("Duplicate Remote Mac Address!");	
					return false;	
				}
				if (eval(document.wireless_basic.wds_2).value == eval(document.wireless_basic.wds_4).value ){
					alert("Duplicate Remote Mac Address!");	
					return false;	
				}			
			}else if (i == 3){
				if (eval(document.wireless_basic.wds_3).value == eval(document.wireless_basic.wds_4).value ){
					alert("Duplicate Remote Mac Address!");	
					return false;	
				}
			}
		}
	}

	
	if ((document.wireless_basic.wds_1.value == "") && (document.wireless_basic.wds_2.value == "") && (document.wireless_basic.wds_3.value == "") && (document.wireless_basic.wds_4.value != "")){
		document.wireless_basic.wds_1.value = document.wireless_basic.wds_4.value;
		document.wireless_basic.wds_4.value = "";
	}
	if ((document.wireless_basic.wds_1.value == "") && (document.wireless_basic.wds_2.value == "") && (document.wireless_basic.wds_3.value != "") && (document.wireless_basic.wds_4.value != "")){
		document.wireless_basic.wds_1.value = document.wireless_basic.wds_3.value;
		document.wireless_basic.wds_2.value = document.wireless_basic.wds_4.value;
		document.wireless_basic.wds_3.value = "";
		document.wireless_basic.wds_4.value = "";
	}
	if ((document.wireless_basic.wds_1.value == "") && (document.wireless_basic.wds_2.value == "") && (document.wireless_basic.wds_3.value != "") && (document.wireless_basic.wds_4.value == "")){
		document.wireless_basic.wds_1.value = document.wireless_basic.wds_3.value;
		document.wireless_basic.wds_3.value = "";
	}
	if ((document.wireless_basic.wds_1.value == "") && (document.wireless_basic.wds_2.value != "") && (document.wireless_basic.wds_3.value != "") && (document.wireless_basic.wds_4.value != "")){
		document.wireless_basic.wds_1.value = document.wireless_basic.wds_2.value;
		document.wireless_basic.wds_2.value = document.wireless_basic.wds_3.value;
		document.wireless_basic.wds_3.value = document.wireless_basic.wds_4.value;
		document.wireless_basic.wds_4.value = "";
	}
	if ((document.wireless_basic.wds_1.value == "") && (document.wireless_basic.wds_2.value != "") && (document.wireless_basic.wds_3.value != "") && (document.wireless_basic.wds_4.value == "")){
		document.wireless_basic.wds_1.value = document.wireless_basic.wds_2.value;
		document.wireless_basic.wds_2.value = document.wireless_basic.wds_3.value;
		document.wireless_basic.wds_3.value = "";
	}
	if ((document.wireless_basic.wds_1.value == "") && (document.wireless_basic.wds_2.value != "") && (document.wireless_basic.wds_3.value == "") && (document.wireless_basic.wds_4.value == "")){
		document.wireless_basic.wds_1.value = document.wireless_basic.wds_2.value;
		document.wireless_basic.wds_2.value = "";
	}	
	if ((document.wireless_basic.wds_1.value != "") && (document.wireless_basic.wds_2.value == "") && (document.wireless_basic.wds_3.value == "") && (document.wireless_basic.wds_4.value != "")){
		document.wireless_basic.wds_2.value = document.wireless_basic.wds_4.value;
		document.wireless_basic.wds_4.value = "";
		
	}
	if ((document.wireless_basic.wds_1.value != "") && (document.wireless_basic.wds_2.value == "") && (document.wireless_basic.wds_3.value != "") && (document.wireless_basic.wds_4.value != "")){
		document.wireless_basic.wds_2.value = document.wireless_basic.wds_3.value;
		document.wireless_basic.wds_3.value = document.wireless_basic.wds_4.value;
		document.wireless_basic.wds_4.value = "";
	}
	if ((document.wireless_basic.wds_1.value != "") && (document.wireless_basic.wds_2.value == "") && (document.wireless_basic.wds_3.value != "") && (document.wireless_basic.wds_4.value == "")){
		document.wireless_basic.wds_2.value = document.wireless_basic.wds_3.value;
		document.wireless_basic.wds_3.value = "";
	}
	if ((document.wireless_basic.wds_1.value != "") && (document.wireless_basic.wds_2.value != "") && (document.wireless_basic.wds_3.value == "") && (document.wireless_basic.wds_4.value != "")){
		document.wireless_basic.wds_3.value = document.wireless_basic.wds_4.value;
		document.wireless_basic.wds_4.value = "";
	}
	if ((document.wireless_basic.wds_1.value == "") && (document.wireless_basic.wds_2.value != "") && (document.wireless_basic.wds_3.value == "") && (document.wireless_basic.wds_4.value != "")){
		document.wireless_basic.wds_1.value = document.wireless_basic.wds_2.value;
		document.wireless_basic.wds_2.value = document.wireless_basic.wds_4.value;
		document.wireless_basic.wds_4.value = "";
	}
		
	showWebMessage(2, "");
	return true;
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name=wireless_basic action="/goform/wifiWDS" onSubmit="return CheckValue()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="WDSTitle">WDS Setup</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="40%" id="WDSMode">Basic Setting:</td>
<td>
<select name="wds_mode" id="wds_mode" size="1" onchange="WdsModeOnChange()">
<option value=0 SELECTED id="WDSModeDisable">Disable</option>
<option value=1 id="WDSModeAPBridge">AP+Bridge</option>	
<option value=2 id="WDSModeBridge">Bridge Only</option>
</select>
</td>
</tr>
</table>
</li>      

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr> 
<td width="40%" id="WDSSelfMAC">Local MAC Address:</td>
<td id="self_mac_id"><% getWlanMac(); %></td>
</tr>
</table>
</li>

<span id="div_wds_phy_encryp" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr id="div_wds_phy_mode" name="div_wds_phy_mode"> 
<td width="40%" id="WDSPhyMode">Phy Mode</td>
<td>
<select name="wds_phy_mode" id="wds_phy_mode" size="1">
<option value="CCK;CCK;CCK;CCK" selected id="WDSPHYCCK">CCK</option>
<!-- other modes are added by javascript -->
</select>
</td>
</tr>
</table>
</li>    
</span>  

<span id="div_wds_mac_addr" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr id="wds_mac_list_1" name="wds_mac_list_1">
<td width="40%" id="WDSAPMacAddr1">Remote MAC Address</td>
<td><input type=text name=wds_1 size=20 maxlength=17 value=""></td>
</tr>
</table>
</li>
  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="wds_mac_list_2" name="wds_mac_list_2">
<td width="40%" id="WDSAPMacAddr2">Remote MAC Addres</td>
<td><input type=text name=wds_2 size=20 maxlength=17 value=""></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr id="wds_mac_list_3" name="wds_mac_list_3">
<td width="40%" id="WDSAPMacAddr3">Remote MAC Address</td>
<td><input type=text name=wds_3 size=20 maxlength=17 value=""></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="wds_mac_list_4" name="wds_mac_list_4">
<td width="40%" id="WDSAPMacAddr4">Remote MAC Address</td>
<td><input type=text name=wds_4 size=20 maxlength=17 value=""></td>
<input type="hidden" name="wds_list" value="1">
</tr>
</table>
</li>
</span>
  
<li class="title" id="WDSsecurityTitle">Security</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="div_wds_encryp_type" name="div_wds_encryp_type">
<td width="40%" id="WDSEncrypType">EncrypType</td>
<td>
<select name="wds_encryp_type" id="wds_encryp_type" size="1" onchange="WdsSecurityOnChange()">
<option value="NONE;NONE;NONE;NONE" selected id="WDSEncrypNo">No Security</option>
<option value="WEP;WEP;WEP;WEP" id="WDSEncrypWEP">WEP</option>
<option value="TKIP;TKIP;TKIP;TKIP" id="WDSEncrypTKIP">TKIP</option>
<option value="AES;AES;AES;AES" id="WDSEncrypAES">AES</option>
</select>
</td>
</tr>
</table>
</li>      

<span id="div_wds_encryp_key_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="div_wds_encryp_key" name="div_wds_encryp_key">
<td width="40%" id="WDSEncrypKey">Encryp Key</td>
<td><input type=text name=wds_encryp_key size=28 maxlength=64 value=""></td>
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

