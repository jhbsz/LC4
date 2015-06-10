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
var apcli_include = '<% getWlanApcliBuilt(); %>';

var wlan_guest_en  = <% getCfgZero(1, "WlanGuestEnable"); %>;
var wlan_guest_bw_en  = <% getCfgZero(1, "WlanGuestBWEnable"); %>;
var wlan_guest_bw_priority  = <% getCfgZero(1, "WlanGuestBWPriority"); %>;

var changed = 0;
var old_MBSSID;
var defaultShownMBSSID = 0;
var SSID = new Array();
var PreAuth = new Array();
var AuthMode = new Array();
var EncrypType = new Array();
var DefaultKeyID = new Array();
var Key1Type = new Array();
var Key1Str = new Array();
var Key2Type = new Array();
var Key2Str = new Array();
var Key3Type = new Array();
var Key3Str = new Array();
var Key4Type = new Array();
var Key4Str = new Array();
var WPAPSK = new Array();
var RekeyMethod = new Array();
var RekeyInterval = new Array();
var PMKCachePeriod = new Array();
var IEEE8021X = new Array();
var RADIUS_Server = new Array();
var RADIUS_Port = new Array();
var RADIUS_Key = new Array();
var session_timeout_interval = new Array();
var AccessPolicy = new Array();
var AccessControlList = new Array();
var DefaultWEPKey = new Array();

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function clickAutoChannel()
{
	if (document.security_form.Auto_Channel.checked == true)
		document.security_form.sz11gChannel.disabled = true;
	else
		document.security_form.sz11gChannel.disabled = false;	
}


function checkMac(str){
	var len = str.length;
	if(len!=17)
		return false;

	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if (    (str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
	return true;
}

function checkRange(str, num, min, max)
{
	d = atoi(str,num);
	if(d > max || d < min)
		return false;
	return true;
}

function checkIpAddr(field)
{
	if(field.value == "")
	return false;

	if ( checkAllNum(field.value) == 0)
		return false;

	if( (!checkRange(field.value,1,0,255)) ||
		(!checkRange(field.value,2,0,255)) ||
		(!checkRange(field.value,3,0,255)) ||
		(!checkRange(field.value,4,1,254)) ){
			return false;
	}
	return true;
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpAddrX(field, ismask)
{
	if (field.value == "") {
		alert("Error. IP address is empty.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
    /*
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 256)) ||
				(!checkRange(field.value, 2, 0, 256)) ||
				(!checkRange(field.value, 3, 0, 256)) ||
				(!checkRange(field.value, 4, 0, 256)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	*/
	//aron modify to limit max to 255
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert('IP adress format error.');
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function atoi(str, num)
{
	i=1;
	if(num != 1 ){
		while (i != num && str.length != 0){
			if(str.charAt(0) == '.'){
			i++;
		}
		str = str.substring(1);
	}
	
        if(i != num )
		return -1;
	}

	for(i=0; i<str.length; i++){
		if(str.charAt(i) == '.'){
			str = str.substring(0, i);
			break;
		}
	}
	
	if(str.length == 0)
		return -1;

	return parseInt(str, 10);
}

function checkHex(str){
	var len = str.length;

	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') ){
				continue;
		}else
	        return false;
	}
	return true;
}

function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if (str.charAt(i) == '\r' || str.charAt(i) == '\n' || str.charAt(i) == '^' || str.charAt(i) == '|' || str.charAt(i) == '%' || str.charAt(i) == '$' || str.charAt(i) == ' ' || str.charAt(i) == ';' || str.charAt(i) == '"'){
			return false;
		}else
	        continue;
	}
	return true;
}

function checkStrictInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if ( str.charAt(i) == ';' || str.charAt(i) == ',' ||
			 str.charAt(i) == '\r' || str.charAt(i) == '\n'){
				return false;
		}else
	        continue;
	}
	return true;
}

function checkAllNum(str)
{
	for (var i=0; i<str.length; i++){
		if((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return false;
	}
	return true;
}

function style_display_on()
{
	if (window.ActiveXObject) { // IE
		return "block";
	}
	else if (window.XMLHttpRequest) { // Mozilla, Safari,...
		return "table-row";
	}
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
			UpdateMBSSIDList();
			LoadFields(defaultShownMBSSID);
		} else {
			alert('There was a problem with the request.');
		}
	}
}

function deleteAccessPolicyListHandler()
{
	window.location.reload(false);
}


function parseAllData(str)
{
	var wepdefaultkey = "<% getCfgZero(1, "DefaultWEPKey"); %>";
	var all_str = new Array();
	all_str = str.split("\n");

	defaultShownMBSSID = parseInt(all_str[0]);
	for (var i=0; i<all_str.length-2; i++) {
		var fields_str = new Array();
		fields_str = all_str[i+1].split("\r");

		SSID[i] = fields_str[0];
		PreAuth[i] = fields_str[1];
		AuthMode[i] = fields_str[2];
		EncrypType[i] = fields_str[3];
		DefaultKeyID[i] = fields_str[4];
		Key1Type[i] = fields_str[5];
		Key1Str[i] = fields_str[6];
		Key2Type[i] = fields_str[7];
		Key2Str[i] = fields_str[8];
		Key3Type[i] = fields_str[9];
		Key3Str[i] = fields_str[10];
		Key4Type[i] = fields_str[11];
		Key4Str[i] = fields_str[12];
		WPAPSK[i] = fields_str[13];
		RekeyMethod[i] = fields_str[14];
		RekeyInterval[i] = fields_str[15];
		PMKCachePeriod[i] = fields_str[16];
		IEEE8021X[i] = fields_str[17];
		RADIUS_Server[i] = fields_str[18];
		RADIUS_Port[i] = fields_str[19];
		RADIUS_Key[i] = fields_str[20];
		session_timeout_interval[i] = fields_str[21];
		AccessPolicy[i] = fields_str[22];
		AccessControlList[i] = fields_str[23];

		DefaultWEPKey[i] = wepdefaultkey.split(";",i+1);

		/* !!!! IMPORTANT !!!!*/
		if(IEEE8021X[i] == "1")
			AuthMode[i] = "IEEE8021X";

		if(AuthMode[i] == "OPEN" && EncrypType[i] == "NONE" && IEEE8021X[i] == "0")
			AuthMode[i] = "Disable";
	}
}

function checkData()
{

	return true;//Arthur Chow 2009-06-09
	
	var securitymode;
	securitymode = document.security_form.security_mode.value;

	if (securitymode == "OPEN" || securitymode == "SHARED" ||securitymode == "WEPAUTO")
	{
		if(! check_Wep(securitymode) )
			return false;
	}else if (securitymode == "WPAPSK" || securitymode == "WPA2PSK" || securitymode == "WPAPSKWPA2PSK" /* || security_mode == 5 */){
		var keyvalue = document.security_form.passphrase.value;

		if (keyvalue.length == 0){
			alert('Please input wpapsk key!');
			return false;
		}

		if (keyvalue.length < 8){
			alert('Please input at least 8 character of wpapsk key!');
			return false;
		}
		
		if(checkInjection(document.security_form.passphrase.value) == false){
			alert('Invalid characters in Pass Phrase.');
			return false;
		}

		if(checkAllNum(document.security_form.keyRenewalInterval.value) == false){
			alert('Please input a valid key renewal interval');
			return false;
		}
		if(document.security_form.keyRenewalInterval.value < 60){
			alert('Warning: A short key renewal interval.');
		}
		if(check_wpa() == false)
			return false;
	}
	//802.1x
	else if (securitymode == "IEEE8021X") // 802.1x
	{
		if( document.security_form.ieee8021x_wep[0].checked == false &&
			document.security_form.ieee8021x_wep[1].checked == false){
			alert('Please choose the 802.1x WEP option.');
			return false;
		}
		if(check_radius() == false)
			return false;
	}else if (securitymode == "WPA" || securitymode == "WPA1WPA2") //     WPA or WPA1WP2 mixed mode
	{
		if(check_wpa() == false)
			return false;
		if(check_radius() == false)
			return false;
	}else if (securitymode == "WPA2") // WPA2
	{
		if(check_wpa() == false)
			return false;
		if( document.security_form.PreAuthentication[0].checked == false &&
			document.security_form.PreAuthentication[1].checked == false){
			alert('Please choose the Pre-Authentication options.');
			return false;
		}

		if(!document.security_form.PMKCachePeriod.value.length){
			alert('Please input the PMK Cache Period.');
			return false;
		}
		if(check_radius() == false)
			return false;
	}

	return true;
}

function check_wpa()
{
		if(checkAllNum(document.security_form.keyRenewalInterval.value) == false){
			alert('Please input a valid key renewal interval');
			return false;
		}
		if(document.security_form.keyRenewalInterval.value < 60){
			alert('Warning: A short key renewal interval.');
		}
		return true;
}

function check_radius()
{
	if(!document.security_form.RadiusServerIP.value.length){
		alert('Please input the radius server ip address.');
		return false;		
	}
	if(!document.security_form.RadiusServerPort.value.length){
		alert('Please input the radius server port number.');
		return false;		
	}
	if(!document.security_form.RadiusServerSecret.value.length){
		alert('Please input the radius server shared secret.');
		return false;		
	}

	if(checkIpAddr(document.security_form.RadiusServerIP) == false){
		alert('Please input a valid radius server ip address.');
		return false;		
	}
	if( (checkRange(document.security_form.RadiusServerPort.value, 1, 1, 65535)==false) ||
		(checkAllNum(document.security_form.RadiusServerPort.value)==false)){
		alert('Please input a valid radius server port number.');
		return false;		
	}
	if(checkStrictInjection(document.security_form.RadiusServerSecret.value)==false){
		alert('The shared secret contains invalid characters.');
		return false;		
	}

	if(document.security_form.RadiusServerSessionTimeout.value.length){
		if(checkAllNum(document.security_form.RadiusServerSessionTimeout.value)==false){
			alert('Please input a valid session timeout number or u may left it empty.');
			return false;	
		}	
	}

	return true;
}

function securityMode(c_f)
{
	var security_mode;


	changed = c_f;

	hideWep();

	show_div(false, "div_wpapsk_compatible");
	show_div(false, "div_wpa_compatible");	
	
	show_div(false, "wpa_passphrase");
	show_div(false, "wpa_key_renewal_interval");
	show_div(false, "wpa_PMK_Cache_Period");
	show_div(false, "wpa_preAuthentication");
	
	document.security_form.passphrase.disabled = true;
	document.security_form.keyRenewalInterval.disabled = true;
	document.security_form.PMKCachePeriod.disabled = true;
	document.security_form.PreAuthentication.disabled = true;

	// 802.1x
	show_div(false, "div_radius_server");
	show_div(false, "div_8021x_wep");
	document.security_form.ieee8021x_wep.disable = true;
	document.security_form.RadiusServerIP.disable = true;
	document.security_form.RadiusServerPort.disable = true;
	document.security_form.RadiusServerSecret.disable = true;	
	document.security_form.RadiusServerSessionTimeout.disable = true;
	document.security_form.RadiusServerIdleTimeout.disable = true;	

	security_mode = document.security_form.security_mode.value;

	if (security_mode == "OPEN" || security_mode == "SHARED" ||security_mode == "WEPAUTO"){
		showWep(security_mode);
	}else if (security_mode == "WPAPSK" || security_mode == "WPA2PSK" || security_mode == "WPAPSKWPA2PSK"){
		<!-- WPA -->
		
		if (security_mode == "WPA2PSK" || security_mode == "WPAPSKWPA2PSK"){	
			show_div(true, "div_wpapsk_compatible");
		}	
		show_div(true, "wpa_passphrase");
		document.security_form.passphrase.disabled = false;

		show_div(true, "wpa_key_renewal_interval");
		document.security_form.keyRenewalInterval.disabled = false;
	}else if (security_mode == "WPA" || security_mode == "WPA2" || security_mode == "WPA1WPA2") //wpa enterprise
	{
	
		if (security_mode == "WPA2" || security_mode == "WPA1WPA2"){ //wpa enterprise	
	 	    show_div(true, "div_wpa_compatible");			
		}
		
		show_div(true, "wpa_key_renewal_interval");
		document.security_form.keyRenewalInterval.disabled = false;
	
		<!-- 802.1x -->
		show_div(true, "div_radius_server");
		document.security_form.RadiusServerIP.disable = false;
		document.security_form.RadiusServerPort.disable = false;
		document.security_form.RadiusServerSecret.disable = false;	
		document.security_form.RadiusServerSessionTimeout.disable = false;
		document.security_form.RadiusServerIdleTimeout.disable = false;	

		if(security_mode == "WPA2"){
			show_div(true, "wpa_preAuthentication");
			document.security_form.PreAuthentication.disabled = false;
			show_div(true, "wpa_PMK_Cache_Period");
			document.security_form.PMKCachePeriod.disabled = false;
		}
	}else if (security_mode == "IEEE8021X"){ // 802.1X-WEP
		show_div(true, "div_8021x_wep");
		show_div(true, "div_radius_server");
		document.security_form.ieee8021x_wep.disable = false;
		document.security_form.RadiusServerIP.disable = false;
		document.security_form.RadiusServerPort.disable = false;
		document.security_form.RadiusServerSecret.disable = false;	
		document.security_form.RadiusServerSessionTimeout.disable = false;
	}
	
	show_div(true, "div_note2_id");
	parent.adjustMyFrameHeight();
}


function hideWep()
{
	show_div(false, "div_wep");
}

function showWep(mode)
{
	show_div(true, "div_wep");
}


function check_Wep(securitymode)
{
	var defaultid, i;
	for (i=0; i<=3; i++){	
		if (document.security_form.DefWEPKey[i].checked == true){
			defaultid = i;
		}
	}	
	defaultid = defaultid+1;

	if ( defaultid == 1 )
		var keyvalue = document.security_form.wep_key_1.value;
	else if (defaultid == 2)
		var keyvalue = document.security_form.wep_key_2.value;
	else if (defaultid == 3)
		var keyvalue = document.security_form.wep_key_3.value;
	else if (defaultid == 4)
		var keyvalue = document.security_form.wep_key_4.value;

	if (keyvalue.length == 0 &&  (securitymode == "SHARED" || securitymode == "OPEN")){ // shared wep  || md5
		alert('Please input wep key'+defaultid+' !');
		return false;
	}

	var keylength = document.security_form.wep_key_1.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key1 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_1.value)== false){
					alert('Wep key1 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key1 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_1.value)== false){
					alert('Wep key1 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key1 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key1 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.security_form.wep_key_2.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key2 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_2.value)== false){
					alert('Wep key2 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key2 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_2.value)== false){
					alert('Wep key2 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key2 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key2 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.security_form.wep_key_3.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key3 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_3.value)== false){
					alert('Wep key3 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key3 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_3.value)== false){
					alert('Wep key3 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key3 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key3 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}							
			}		
		
		}
	}	

	var keylength = document.security_form.wep_key_4.value.length;
	if (keylength != 0){
		if (document.security_form.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key4 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_4.value)== false){
					alert('Wep key4 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key4 !');
					return false;
				}
				if(checkInjection(document.security_form.wep_key_4.value)== false){
					alert('Wep key4 contains invalid characters.');
					return false;
				}
			}
		}else{ //HEX
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (HEX)
				if(keylength != 10) {
					alert('Please input 10 characters of wep key4 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_4.value) == false){
					alert('Invalid Wep key4 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key4 !');
					return false;
				}
				if(checkHex(document.security_form.wep_key_4.value) == false){
					alert('Invalid Wep key4 format!');
					return false;
				}							
			}		
		
		}
	}	


	return true;
}
	
function submit_apply()
{
	var submit_ssid_num;
	
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	    return false;
	}
	
	if (((document.security_form.mssid_1.value == "") && (document.security_form.hidemssid_1.checked == true)) ||
	    ((document.security_form.mssid_2.value == "") && (document.security_form.hidemssid_2.checked == true)) ||
	    ((document.security_form.mssid_3.value == "") && (document.security_form.hidemssid_3.checked == true)) 
		 ){
		alert("SSID field cannot empty when you select hide function");
		return false;
	}	
		
	if (((document.security_form.mssid_1.value == "") && (document.security_form.IntraBSS1.checked == true)) ||
	    ((document.security_form.mssid_2.value == "") && (document.security_form.IntraBSS2.checked == true)) ||
	    ((document.security_form.mssid_3.value == "") && (document.security_form.IntraBSS3.checked == true)) 
		 ){
		alert("SSID field cannot empty when you select Enable Intra-BSS Traffic function");
		return false;
	}
		
	if (document.security_form.ssid.value == "")
	{
		alert("Please enter SSID!");
		document.security_form.ssid.focus();
		document.security_form.ssid.select();
		return false;
	}
	
	if(checkInjection(document.security_form.ssid.value) == false){
		alert('System do not support specific invalid characters.');
		return false;
	}
	if(checkInjection(document.security_form.mssid_1.value) == false){
		alert('System do not support specific invalid characters.');
		return false;
	}
	if(checkInjection(document.security_form.mssid_2.value) == false){
		alert('System do not support specific invalid characters.');
		return false;
	}
	if(checkInjection(document.security_form.mssid_3.value) == false){
		alert('System do not support specific invalid characters.');
		return false;
	}	

	submit_ssid_num = 1;

	for (i = 1; i < 4; i++)
	{
		if (eval("document.security_form.mssid_"+i).value != "")
		{
			if (i == 3)
			{
				if (1*apcli_include == 0)
					submit_ssid_num++;
					
			}
			else
				submit_ssid_num++;
		}
	}

	document.security_form.bssid_num.value = submit_ssid_num;		


	if(document.security_form.mssid_1.value == "")
		document.security_form.bssid_num1.value = "0";
	else
		document.security_form.bssid_num1.value = "1";
	
	if(document.security_form.mssid_2.value == "")
		document.security_form.bssid_num2.value = "0";
	else
		document.security_form.bssid_num2.value = "1";

	if(document.security_form.mssid_3.value == "")
		document.security_form.bssid_num3.value = "0";
	else
		document.security_form.bssid_num3.value = "1";	

	if((document.security_form.ssid.value == "") || (document.security_form.mssid_1.value == "") || (document.security_form.mssid_2.value == "") || (document.security_form.mssid_3.value == "")){
		alert("Please enter SSID!");
		return false;
	}

	if ((wirelessmode == 0) && (document.security_form.wlanguest.checked == true)){
		if(document.security_form.wlanguestip.value == ""){
			alert("Error. IP address is empty.");
			return false;
		}
		if(!checkIpAddrX(document.security_form.wlanguestip, false)){
			alert('IP address format error.');
			return false;
		}
		
		if(document.security_form.wlanguestmask.value == ""){
			alert("Error. Invalid Subnet Mask.");
			return false;
		}
	    if (!checkIpAddrX(document.security_form.wlanguestmask, true)){
	    	alert("Error. Invalid Subnet Mask.");
		    return false;
		}
		
		if (document.security_form.guestmaxbw.value == "" ){
			alert("The value of bandwitdth can not accept empty.");
			return false;
		}
		if (document.security_form.guestmaxbw.value < 0 || document.security_form.guestmaxbw.value > 32768 ){
			alert("Please set value between 0~32768");
			return false;
		}	
		
	}

	if (checkData() == true){
		changed = 0;
		
		showWebMessage(2, "");
		document.security_form.submit();
	}
}

function LoadFields(MBSSID)
{
	var result;
	sp_select = document.getElementById("security_mode");

	sp_select.options.length = 0;
    
	wpsenable = <% getWPSModeASP(); %>;

	if(wpsenable ==0){
		// disable WPS
		sp_select.options[sp_select.length] = new Option(_("general nosecurity"),"Disable",	false, AuthMode[MBSSID] == "Disable");
		sp_select.options[sp_select.length] = new Option(_("general static wep"),"SHARED", 	false, AuthMode[MBSSID] == "SHARED");
    		sp_select.options[sp_select.length] = new Option(_("general wpapsk"), "WPAPSK",	false, AuthMode[MBSSID] == "WPAPSK");
    		sp_select.options[sp_select.length] = new Option(_("general wpa"),	"WPA",		false, AuthMode[MBSSID] == "WPA");        
    		sp_select.options[sp_select.length] = new Option(_("general wpa2psk"),"WPA2PSK",	false, AuthMode[MBSSID] == "WPA2PSK");
    		sp_select.options[sp_select.length] = new Option(_("general wpa2"),	"WPA2",		false, AuthMode[MBSSID] == "WPA2");
	}else{
		// enable WPS
		sp_select.options[sp_select.length] = new Option(_("general nosecurity"),"Disable",	false, AuthMode[MBSSID] == "Disable");
		sp_select.options[sp_select.length] = new Option(_("general static wep"),"SHARED", 	false, AuthMode[MBSSID] == "SHARED");
		sp_select.options[sp_select.length] = new Option(_("general wpapsk"), "WPAPSK",	false, AuthMode[MBSSID] == "WPAPSK");
    		sp_select.options[sp_select.length] = new Option(_("general wpa2psk"),"WPA2PSK",	false, AuthMode[MBSSID] == "WPA2PSK");
	}

	// WEP
	document.getElementById("WEP1").value = Key1Str[MBSSID];
	document.getElementById("WEP2").value = Key2Str[MBSSID];
	document.getElementById("WEP3").value = Key3Str[MBSSID];
	document.getElementById("WEP4").value = Key4Str[MBSSID];

	// Now, 4 Keys type are same, so only get the first key to display HEX or ASCII. It's OK
	if (Key1Type[MBSSID] == "0"){
		document.security_form.WEPKey_Code[1].checked = true; //hex

		if (Key1Str[MBSSID] != ""){
			if (Key1Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (Key2Str[MBSSID] != ""){
			if (Key2Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (Key3Str[MBSSID] != ""){
			if (Key3Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (Key4Str[MBSSID] != ""){
			if (Key4Str[MBSSID].length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}		
		}
	}else{
		document.security_form.WEPKey_Code[0].checked = true; //ASCII

		if (Key1Str[MBSSID] != ""){
			if (Key1Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (Key2Str[MBSSID] != ""){
			if (Key2Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (Key3Str[MBSSID] != ""){
			if (Key3Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (Key4Str[MBSSID] != ""){
			if (Key4Str[MBSSID].length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}		
		}
	}

	document.security_form.DefWEPKey[0].checked= false;
	document.security_form.DefWEPKey[1].checked= false;
	document.security_form.DefWEPKey[2].checked= false;
	document.security_form.DefWEPKey[3].checked= false;
	
	var defkey_index = parseInt(DefaultKeyID[MBSSID]) - 1;
//	var defkey_index = parseInt(DefaultWEPKey[MBSSID]) - 1;
	
	if ( (defkey_index >= 0) && (defkey_index <= 3) ){
	     document.security_form.DefWEPKey[defkey_index].checked= true;
	}else {
	     document.security_form.DefWEPKey[0].checked= true;	
	}     

	// Static WEP 	
	sp_select = document.getElementById("security_mode");	

	if (AuthMode[MBSSID] == "WEPAUTO")
		document.getElementById("security_mode").selectedIndex = 1;

// +++ Let default WEP authentication method from Shared to Auto
var LastAuthMode = "<% getCfgZero(1, "LastWEPAuthMethod"); %>";
if  ((LastAuthMode == "WEPAUTO") || (LastAuthMode == "SHARED")){
	if(LastAuthMode == "WEPAUTO"){
		document.getElementById("auth_method").selectedIndex = 0; // Auto
	}else if (LastAuthMode == "SHARED"){
		document.getElementById("auth_method").selectedIndex = 1; // Shared Key
	}else{
		document.getElementById("auth_method").selectedIndex = 0; // default Auto
	}	
}else{
	if(AuthMode[MBSSID] == "WEPAUTO")
		document.getElementById("auth_method").selectedIndex = 0; // Auto
	else if (AuthMode[MBSSID] == "SHARED")
		document.getElementById("auth_method").selectedIndex = 1; // Shared Key
	else
		document.getElementById("auth_method").selectedIndex = 0; // default Auto
}
// --- Let default WEP authentication method from Shared to Auto
 	
 	// WPA Compatible and WPA-PSK Compatible	
	if(AuthMode[MBSSID] == "WPAPSKWPA2PSK"){
		if(wpsenable ==0){	
		document.getElementById("security_mode").selectedIndex = 4;	
	   	document.security_form.wpapsk_compatible.checked = true;	
	   	}else{
			document.getElementById("security_mode").selectedIndex = 3;
	   		document.security_form.wpapsk_compatible.checked = true;	   	
	   	}
	}
 	
	if(AuthMode[MBSSID] == "WPA1WPA2"){
	   document.getElementById("security_mode").selectedIndex = 5;		
	   document.security_form.wpa_compatible.checked = true;	
	}
 	
	document.getElementById("passphrase").value = WPAPSK[MBSSID];
	document.getElementById("keyRenewalInterval").value = RekeyInterval[MBSSID];
	document.getElementById("PMKCachePeriod").value = PMKCachePeriod[MBSSID];
	
	if(PreAuth[MBSSID] == "0")
		document.security_form.PreAuthentication[0].checked = true;
	else
		document.security_form.PreAuthentication[1].checked = true;

	//802.1x wep
	if(IEEE8021X[MBSSID] == "1"){
		if(EncrypType[MBSSID] == "WEP")
			document.security_form.ieee8021x_wep[1].checked = true;
		else
			document.security_form.ieee8021x_wep[0].checked = true;
	}
	
	document.getElementById("RadiusServerIP").value = RADIUS_Server[MBSSID];
	document.getElementById("RadiusServerPort").value = RADIUS_Port[MBSSID];
	document.getElementById("RadiusServerSecret").value = RADIUS_Key[MBSSID];			
	document.getElementById("RadiusServerSessionTimeout").value = session_timeout_interval[MBSSID];
	
	securityMode(0);

}


function selectMBSSIDChanged()
{
	// check if any security settings changed
	if(changed){
		ret = confirm("Are you sure to ignore changed?");
		if(!ret){
			document.security_form.ssidIndex.options.selectedIndex = old_MBSSID;
			return false;
		}
		else
			changed = 0;
	}

	var selected = document.security_form.ssidIndex.options.selectedIndex;

	// backup for user cancel action
	old_MBSSID = selected;

	MBSSIDChange(selected);
}

/*
 * When user select the different SSID, this function would be called.
 */ 
function MBSSIDChange(selected)
{
	// load wep/wpa/802.1x table for MBSSID[selected]
	LoadFields(selected);

	// radio button special case
	WPAAlgorithms = EncrypType[selected];
	IEEE8021XWEP = IEEE8021X[selected];
	PreAuthentication = PreAuth[selected];

	return true;
}


function delap(mbssid, num)
{
	makeRequest("/goform/APDeleteAccessPolicyList", mbssid+ "," +num, deleteAccessPolicyListHandler);
}

function initTranslation()
{

	var e = document.getElementById("GeneralWirelessSetup");
	e.innerHTML = _("general wireless setup");

	e = document.getElementById("GeneralWirelessLAN");
	e.innerHTML = _("general wireless lan");
	
	e = document.getElementById("GeneralWirelessSSID");
	e.innerHTML = _("general wireless ssid");
	
	e = document.getElementById("GeneralHideSSID1");
	//e.innerHTML = _("general hide ssid1");
	e.innerHTML = _("general mssid hide");
	
	e = document.getElementById("GeneralmSSIDHide1");
	e.innerHTML = _("general mssid hide");
	
	e = document.getElementById("GeneralmSSIDHide2");
	e.innerHTML = _("general mssid hide");
	
	e = document.getElementById("GeneralmSSIDHide3");
	e.innerHTML = _("general mssid hide");
	
	e = document.getElementById("GeneralChannelSelect");
	e.innerHTML = _("general channel select");
	
	e = document.getElementById("GeneralAutoChannelSelect");
	e.innerHTML = _("general autochannel select");
	
	e = document.getElementById("GeneralOperatingChannel");
	e.innerHTML = _("general operating channel");
		
	e = document.getElementById("GeneralSSIDChoice");
	e.innerHTML = _("general ssid choice");

	e = document.getElementById("GeneralSecurityTitle");
	e.innerHTML = _("general security title");
	
	e = document.getElementById("GeneralSecureMode");
	e.innerHTML = _("general security mode");

	e = document.getElementById("GeneralWEPPassPhrase");
	e.innerHTML = _("general wep passphrase");

	e = document.getElementById("GeneralWEPEncryp");
	e.innerHTML = _("general wep encryp");

	e = document.getElementById("GeneralAuthMethod");
	e.innerHTML = _("general auth method");

	e = document.getElementById("GeneralWEPGenerate");
	e.value = _("general wep generate");

	e = document.getElementById("GeneralWEPEncryp64");
	e.innerHTML = _("general wep encryp64");
	
	e = document.getElementById("GeneralWEPEncryp128");
	e.innerHTML = _("general wep encryp128");

	e = document.getElementById("GeneralEncrypSharedKey");
	e.innerHTML = _("general encryp sharedkey");

	e = document.getElementById("GeneralEncrypAuto");
	e.innerHTML = _("general encryp auto");

	e = document.getElementById("GeneralNote1_1");
	e.innerHTML = _("general note1_1");
	
	e = document.getElementById("GeneralNote1_2");
	e.innerHTML = _("general note1_2");
	
	e = document.getElementById("GeneralNote1_3");
	e.innerHTML = _("general note1_3");
	
	e = document.getElementById("GeneralNote1_4");
	e.innerHTML = _("general note1_4");

	e = document.getElementById("GeneralWEPASCII");
	e.innerHTML = _("general wep ascii");
	
	e = document.getElementById("GeneralWEPHex");
	e.innerHTML = _("general wep hex");

	e = document.getElementById("GeneralWEPKEY1");
	e.innerHTML = _("general wep key1");

	e = document.getElementById("GeneralWEPKEY2");
	e.innerHTML = _("general wep key2");
	
	e = document.getElementById("GeneralWEPKEY3");
	e.innerHTML = _("general wep key3");
		
	e = document.getElementById("GeneralWEPKEY4");
	e.innerHTML = _("general wep key4");

	e = document.getElementById("GeneralWPAPSKCompatible");
	e.innerHTML = _("general wpapsk compatible");

	e = document.getElementById("GeneralWPACompatible");
	e.innerHTML = _("general wpa compatible");

	e = document.getElementById("GeneralWPAPSKPreSharedKey");
	e.innerHTML = _("general wpapsk presharedkey");	

	e = document.getElementById("GeneralWPAKeyRenewInterval");
	e.innerHTML = _("general wpa keyrenewinterval");
	
	e = document.getElementById("GeneralWPAPMKCachePeriod");
	e.innerHTML = _("general wpa pmkcacheperiod");
	
	e = document.getElementById("GeneralWPAkeyRenewalIntervalUnit");
	e.innerHTML = _("general wpa keyrenewalintervalunit");

	e = document.getElementById("GeneralWPAPMKCachePeriodUnit");
	e.innerHTML = _("general wpa pmkcacheperiodunit");

	e = document.getElementById("GeneralWPAPreAuth");
	e.innerHTML = _("general wpa preauth");

	e = document.getElementById("GeneralWPAPreAuthDisable");
	e.innerHTML = _("general wpa preauthdisable");

	e = document.getElementById("GeneralWPAPreAuthEnable");
	e.innerHTML = _("general wpa preauthenable");

	e = document.getElementById("GeneralIEEE8021xSettings");
	e.innerHTML = _("general ieee8021x settings");

	e = document.getElementById("General1XWEP");
	e.innerHTML = _("general ieee8021x wep");

	e = document.getElementById("General1XWEPDisable");
	e.innerHTML = _("general ieee8021x wepdisable");

	e = document.getElementById("General1XWEPEnable");
	e.innerHTML = _("general ieee8021x wepenable");

	e = document.getElementById("GeneralAuthServer");
	e.innerHTML = _("general auth server");

	e = document.getElementById("GeneralRadiusIPAddr");
	e.innerHTML = _("general radius ipaddr");

	e = document.getElementById("GeneralRadiusPort");
	e.innerHTML = _("general radius port");

	e = document.getElementById("GeneralRadiusSharedSecret");
	e.innerHTML = _("general radius sharedsecret");

	e = document.getElementById("GeneralRadiusSessionTimeout");
	e.innerHTML = _("general radius sessiontimeout");

	e = document.getElementById("GeneralRadiusIdleTimeout");
	e.innerHTML = _("general radius idletimeout");

	e = document.getElementById("GeneralNote2");
	e.innerHTML = _("general note_2");	

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");

	e = document.getElementById("GeneralReset");
	e.value = _("general reset");

	e = document.getElementById("IntraBSS_0");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_1");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_2");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("IntraBSS_3");
	e.innerHTML = _("advance intra bss");	
	e = document.getElementById("Main_IntraBSS");
	e.innerHTML = _("main advance intra bss");		
	e = document.getElementById("GeneralWirelessSSID1");
	e.innerHTML = _("general wireless ssid1");
	e = document.getElementById("GeneralWirelessSSID2");
	e.innerHTML = _("general wireless ssid2");
	e = document.getElementById("GeneralWirelessSSID3");
	e.innerHTML = _("general wireless ssid3");	
	
	e = document.getElementById("enable_guest_wlan");
	e.innerHTML = _("wlan guest enable");	
	e = document.getElementById("wlanguestIP");
	e.innerHTML = _("wlan guest IP");
	e = document.getElementById("wlanguestMASK");
	e.innerHTML = _("wlan guest Subnet");
	e = document.getElementById("Guest_BWControl");
	e.innerHTML = _("wlan guest BWControl");
	e = document.getElementById("guestpriority");
	e.innerHTML = _("wlan guest priority");	
	e = document.getElementById("guestHigh");
	e.innerHTML = _("wlan guest High");	
	e = document.getElementById("guestMiddle");
	e.innerHTML = _("wlan guest Middle");	
	e = document.getElementById("guestLow");
	e.innerHTML = _("wlan guest Low");	
	e = document.getElementById("guetmaxBW");
	e.innerHTML = _("wlan guest maxBW");
	e = document.getElementById("guetBW_unit");
	e.innerHTML = _("wlan guest BW_unit");
}

function initAll()
{

	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	}
	
	initTranslation();
	
//	var Enable_SSID = '<% getCfgZero(1, "EnableSSID"); %>';
	var Enable_SSID1 = '<% getCfgZero(1, "EnableSSID1"); %>';
	var Enable_SSID2 = '<% getCfgZero(1, "EnableSSID2"); %>';	
	var Enable_SSID3 = '<% getCfgZero(1, "EnableSSID3"); %>';	
	var Intra_BSS = '<% getCfgZero(1, "NoForwarding"); %>';
	var Intra_BSS1 = '<% getCfgZero(1, "NoForwarding1"); %>';
	var Intra_BSS2 = '<% getCfgZero(1, "NoForwarding2"); %>';	
	var Intra_BSS3 = '<% getCfgZero(1, "NoForwarding3"); %>';
	var broadcastssidEnable  = '<% getCfgZero(1, "HideSSID"); %>';
	var broadcastssidEnable1  = '<% getCfgZero(1, "HideSSID1"); %>';
	var broadcastssidEnable2  = '<% getCfgZero(1, "HideSSID2"); %>';
	var broadcastssidEnable3  = '<% getCfgZero(1, "HideSSID3"); %>';
        var nv_channel = <% getCfgZero(1, "Channel"); %>;
	var nv_radio_off = "<% getRadioStatusASP(); %>";
	var MainIntra_BSS = "<% getCfgZero(1, "MainIntra_BSS"); %>";

	var t = document.getElementById("GeneralRadioStatus");		
	if (nv_radio_off == "ON")
		t.innerHTML = _("general on");
	else
		t.innerHTML = _("general off");
		
//	if (Enable_SSID == "1"){
//		document.security_form.enablessid.checked = true;
//	}else{
//		document.security_form.enablessid.checked = false;
//	}
	if (Enable_SSID1 == "1"){
		document.security_form.enablessid1.checked = true;
	}else{
		document.security_form.enablessid1.checked = false;
	}
	if (Enable_SSID2 == "1"){
		document.security_form.enablessid2.checked = true;
	}else{
		document.security_form.enablessid2.checked = false;
	}
	if (Enable_SSID3 == "1"){
		document.security_form.enablessid3.checked = true;
	}else{
		document.security_form.enablessid3.checked = false;
	}
		
	if (broadcastssidEnable == "1"){
		document.security_form.hidessid.checked = true;
	}else{
		document.security_form.hidessid.checked = false;
	}
	if (broadcastssidEnable1 == "1"){
		document.security_form.hidemssid_1.checked = true;
	}else{
		document.security_form.hidemssid_1.checked = false;
	}
	if (broadcastssidEnable2 == "1"){
		document.security_form.hidemssid_2.checked = true;
	}else{
		document.security_form.hidemssid_2.checked = false;
	}
	if (broadcastssidEnable3 == "1"){
		document.security_form.hidemssid_3.checked = true;
	}else{
		document.security_form.hidemssid_3.checked = false;
	}
	
	if (Intra_BSS == "0"){
		document.security_form.IntraBSS.checked = true;
	}else{
		document.security_form.IntraBSS.checked = false;
	}
	
	if (Intra_BSS1 == "0"){
		document.security_form.IntraBSS1.checked = true;
	}else{
		document.security_form.IntraBSS1.checked = false;
	}
			
	if (Intra_BSS2 == "0"){
		document.security_form.IntraBSS2.checked = true;
	}else{
		document.security_form.IntraBSS2.checked = false;
	}
	
	if (Intra_BSS3 == "0"){
		document.security_form.IntraBSS3.checked = true;
	}else{
		document.security_form.IntraBSS3.checked = false;
	}

	if (MainIntra_BSS == "0"){
		document.security_form.MainIntraBSS.checked = true;
	}else{
		document.security_form.MainIntraBSS.checked = false;
	}

	if (nv_channel == 0){
		document.security_form.Auto_Channel.checked = true;
		document.security_form.sz11gChannel.disabled = true;
	}else{
		document.security_form.Auto_Channel.checked = false;
	}	

	if (nv_channel != 0)
	    document.security_form.sz11gChannel.options.selectedIndex = nv_channel-1;
	    	
	if (1*apcli_include == 1)
	{
		document.security_form.mssid_7.disabled = true;
	}	    	
		
// WLAN Guest
	if (wlan_guest_en == 1)
		document.security_form.wlanguest.checked = true;
	else
		document.security_form.wlanguest.checked = false;
		
	if (wlan_guest_bw_en == 0)
		document.security_form.wlanguestBW.checked = false;
	else
		document.security_form.wlanguestBW.checked = true;		
		
	document.security_form.guest_priority.selectedIndex = wlan_guest_bw_priority - 1;
		
	if (wirelessmode == 0){
		show_div(true, "div_wlanguest_id");
		if (wlan_guest_en == 1){
			show_div(true, "div_wlanguest_id");
			show_div(true, "div_wlanguestIP_id");
			show_div(true, "div_wlanguestMASK_id");
			show_div(true, "div_wlanguestBW_id");
			show_div(true, "div_guestpriority_id");
			show_div(true, "div_guetmaxBW_id");
		}else{
			//show_div(false, "div_wlanguest_id");
			show_div(false, "div_wlanguestIP_id");
			show_div(false, "div_wlanguestMASK_id");
			show_div(false, "div_wlanguestBW_id");
			show_div(false, "div_guestpriority_id");
			show_div(false, "div_guetmaxBW_id");
		}
	}else{
		show_div(false, "div_wlanguest_id");
		
		show_div(false, "div_wlanguestIP_id");
		show_div(false, "div_wlanguestMASK_id");
		show_div(false, "div_wlanguestBW_id");
		show_div(false, "div_guestpriority_id");
		show_div(false, "div_guetmaxBW_id");
	}	
		
	show_div(true, "div_ssidchoice");	

	show_div(true, "div_note2_id");
	
	makeRequest("/goform/wirelessGetSecurity", "n/a", securityHandler);
	parent.adjustMyFrameHeight();
}

function UpdateMBSSIDList()
{
	document.security_form.ssidIndex.length = 0;

	for(var i=0; i<SSID.length; i++){
		var j = document.security_form.ssidIndex.options.length;	
		document.security_form.ssidIndex.options[j] = new Option(SSID[i], i, false, false);
	}
	
	document.security_form.ssidIndex.options.selectedIndex = defaultShownMBSSID;
	old_MBSSID = defaultShownMBSSID;
}

function setChange(c){
	changed = c;
}

var WPAAlgorithms;
function onWPAAlgorithmsClick(type)
{
	if(type == 0 && WPAAlgorithms == "TKIP") return;
	if(type == 1 && WPAAlgorithms == "AES") return;
	if(type == 2 && WPAAlgorithms == "TKIPAES") return;
	setChange(1);
}

var IEEE8021XWEP;
function onIEEE8021XWEPClick(type)
{
	if(type == 0 && IEEE8021XWEP == false) return;
	if(type == 1 && IEEE8021XWEP == true) return;
	setChange(1);
}

var PreAuthentication;
function onPreAuthenticationClick(type)
{
	if(type == 0 && PreAuthentication == false) return;
	if(type == 1 && PreAuthentication == true) return;
	setChange(1);
}

function parse40WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.security_form.wep_key_1.value = fields_str[0];
	document.security_form.wep_key_2.value = fields_str[1];
	document.security_form.wep_key_3.value = fields_str[2];
	document.security_form.wep_key_4.value = fields_str[3];
}

function parse128WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.security_form.wep_key_1.value = fields_str[0];
	document.security_form.wep_key_2.value = fields_str[0];
	document.security_form.wep_key_3.value = fields_str[0];
	document.security_form.wep_key_4.value = fields_str[0];
}

function get40wepeyHandler(){
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parse40WEPkey(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
		}
	}
}

function get128wepeyHandler(){
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parse128WEPkey(http_request.responseText);
		} else {
			alert('There was a problem with the request.');
	    }
	} 
}

function generate_wep()
{
	var passphrase;
	passphrase = document.security_form.wep_passphrase.value;

	document.security_form.WEPKey_Code[1].checked = true; //Hex	
	if (document.getElementById("wep_encry").selectedIndex == 1){ // get 128 bits WEP KEY
		makeRequest("/goform/wifiget128wepkey", passphrase, get128wepeyHandler);
	}else{ // get 40 bits WEP KEY
		makeRequest("/goform/wifiget40wepkey", passphrase, get40wepeyHandler);
	}
}

function clickwlanguest()
{
	if (document.security_form.wlanguest.checked == true){
		show_div(true, "div_wlanguestIP_id");
		show_div(true, "div_wlanguestMASK_id");
		show_div(true, "div_wlanguestBW_id");
		show_div(true, "div_guestpriority_id");
		show_div(true, "div_guetmaxBW_id");
	}else{
		show_div(false, "div_wlanguestIP_id");
		show_div(false, "div_wlanguestMASK_id");
		show_div(false, "div_wlanguestBW_id");
		show_div(false, "div_guestpriority_id");
		show_div(false, "div_guetmaxBW_id");
	}
	parent.adjustMyFrameHeight();
}

</script>
</head>
<body onload="initAll()">
<form method="post" name="security_form" action="/goform/wifiAPGeneral">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>

<li class="title" id="GeneralWirelessSetup">Wireless Setup</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" id="GeneralWirelessLAN">Wireless LAN :</td>
<td width="150" id=GeneralRadioStatus></td>
<td width="*">&nbsp;</td>
</tr>
</table>
</li>
             

<input type="hidden" name="bssid_num" value="1">

<input type="hidden" name="bssid_num1" value="">
<input type="hidden" name="bssid_num2" value="">
<input type="hidden" name="bssid_num3" value="">

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
<font id="xxx">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</font>
<!--
<font id="GeneralWirelessEnable">Enable</font>
<input type="checkbox" name=enablessid value="1">
-->
<font id="GeneralWirelessSSID">Network Name(SSID) :</font></td>
<td width="150">
<input type=text name=ssid size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID1"); %>" />
</td>        
<td width="*"><input type="checkbox" name=hidessid value="1" />
<font id ="GeneralHideSSID1"> Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS" type="checkbox" value="0" />
<font id ="IntraBSS_0"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>                      


<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap >
<!--
<font id="GeneralWirelessEnable1">Enable</font>
-->
<input type="checkbox" name=enablessid1 value="1" />
<font id="GeneralWirelessSSID1">Name(SSID1) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_1 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID2"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_1 value="1" />
<font id ="GeneralmSSIDHide1">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS1" type="checkbox" value="0" />
<font id ="IntraBSS_1"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
<!--
<font id="GeneralWirelessEnable2">Enable</font>
-->
<input type="checkbox" name=enablessid2 value="1" >
<font id="GeneralWirelessSSID2"> Name(SSID2) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_2 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID3"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_2 value="1" />
<font id ="GeneralmSSIDHide2">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS2" type="checkbox" value="0" />
<font id ="IntraBSS_2"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>

<!-- <span id="div_mssid3_id" class="on"> -->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap >
<!--
<font id="GeneralWirelessEnable3">Enable</font>
-->
<input type="checkbox" name=enablessid3 value="1" />
<font id="GeneralWirelessSSID3">Name(SSID3) :</font></td>
<td width="150" nowrap >
<input type=text name=mssid_3 size=20 maxlength=32 value="<% getCfgGeneral(1, "SSID4"); %>" />
</td>
<td><input type="checkbox" name=hidemssid_3 value="1" />
<font id ="GeneralmSSIDHide3">Hide</font>
<!-- Intra-BSS/SSID checkbox -->
<input name="IntraBSS3" type="checkbox" value="0" />
<font id ="IntraBSS_3"> Enable Intra-BSS Traffic</font> 

<!-- add WLAN Guest -->
<span id="div_wlanguest_id" class="off">
<input type="checkbox" name=wlanguest value="1" onclick="clickwlanguest()"/>
<font id ="enable_guest_wlan">Enable Guest WLAN</font>
</td>
</span>

</tr>
</table>
</li>
<!-- </span>  -->

<!-- add WLAN Guest(Begin) -->
<span id="div_wlanguestIP_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="wlanguestIP">IP Address : </font></td>
<td>
<input type=text name=wlanguestip size=20 maxlength=20 value="<% getCfgGeneral(1, "WlanGuestIP"); %>" />
</td>
<td></td>
</tr>
</table>
</li>
</span>

<span id="div_wlanguestMASK_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="wlanguestMASK"> IP Subnet Mask : </font></td>
<td>
<input type=text name=wlanguestmask size=20 maxlength=20 value="<% getCfgGeneral(1, "WlanGuestMASK"); %>" />
</td>
</tr>
</table>
</li>
</span>
                
<span id="div_wlanguestBW_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input name="wlanguestBW" type="checkbox" value="1" />
<font id ="Guest_BWControl">Enable Bandwidth Management for Guest WLAN</font> 
</td>
</tr>
</table>
</li>                
</span>

<span id="div_guestpriority_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="guestpriority"> Priority : </font></td>
<td>
<select name="guest_priority" size="1">
<option value = 1 id="guestHigh">High</option>
<option value = 2 id="guestMiddle">Middle</option>
<option value = 3 id="guestLow">Low</option>
</select>
</td>
</tr>
</table>
</li>
</span>

<span id="div_guetmaxBW_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="180" nowrap>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="guetmaxBW">Maximum Bandwidth : </font></td>
<td>
<input type=text name=guestmaxbw size=5 maxlength=4 value="<% getCfgGeneral(1, "WlanGuestBWMax"); %>" />
<font id="guetBW_unit">(kbps)</font>
</td>
</tr>
</table>
</li>                
</span>
             
<!-- add WLAN Guest(End) -->
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td id ="GeneralChannelSelect" width="180" nowrap>Channel Selection :</td>
<td width="150">
<select id="sz11gChannel" name="sz11gChannel" size="1">
<% getWlan11gChannelsFreq(); %>
</select>                        
</td>
<td width="*">
<input type="checkbox" name="Auto_Channel" value=1 onclick="clickAutoChannel()" />
<font id ="GeneralAutoChannelSelect">Auto Channel Selection</font> 
</td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td id ="GeneralOperatingChannel" width="180" nowrap>Operating Channel :</td>
<td width="150"> <% getOpChannleASP(); %>  </td>                      
<td width="*">&nbsp;</td>
</tr>
</table>
</li>

<!-- Main Intra-BSS/SSID checkbox -->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%"><input name="MainIntraBSS" type="checkbox" value="0" />
<font id ="Main_IntraBSS">Communication between wireless clients with different SSIDs</font> 
</td>
<td>&nbsp;</td>
</tr>
</table>
</li>

<span id="div_hide_security_id" class="off">
<li class="title" id="GeneralSecurityTitle">Security</li>
<span id="div_ssidchoice" class="off">                                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralSSIDChoice">SSID choice</td>
<td><select name="ssidIndex" size="1" onchange="selectMBSSIDChanged()">
<!-- ....Javascript will update options.... -->
</select>
</td>
</tr>
</table>
</li>
</span>

<li class="title" id="GeneralSecurityTitle">Security</li>                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralSecureMode">Security Mode</td>
<td>
<select name="security_mode" id="security_mode" size="1" onchange="securityMode(1)">
<!-- ....Javascript will update options.... -->
</select>
</td>
</tr>
</table>
</li>

<!-- WEP -->
<span id="div_wep" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWEPPassPhrase">PassPhrase </td>
<td><input name="wep_passphrase" id="wep_passphrase" maxlength="26" value="" onKeyUp="setChange(1)">
<input type=button value="Generate" id="GeneralWEPGenerate" onclick="generate_wep()" ></td>		      
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWEPEncryp">WEP Encryption</td>
<td>
<select name="wep_encry" id="wep_encry" size="1" onchange="securityMode(1)">
<option id="GeneralWEPEncryp64" value="0">64-bits </option>
<option id="GeneralWEPEncryp128" value="1">128-bits</option>
</select>
</td>
</tr>
</table>
</li>                

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralAuthMethod">Authentication Method</td>
<td>
<select name="auth_method" id="auth_method" size="1" onchange="securityMode(1)">
<option id="GeneralEncrypAuto"      value="0">Auto      </option>
<option id="GeneralEncrypSharedKey" value="1">Shared Key</option>
</select>
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id="GeneralNote1_1">Note:</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_2">64-bit WEP: Enter 5 ASCII characters or 10 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_3">128-bit WEP: Enter 13 ASCII characters or 26 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_4">(Select one WEP key as an active key to encrypt wireless data transmission.)</span></td>
</tr>
</table>
</li>                


<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%"></td>
<td>
<input type="radio" name="WEPKey_Code" value="1" /><font id="GeneralWEPASCII">ASCII </font>
<input type="radio" name="WEPKey_Code" value="0" /><font id="GeneralWEPHex">Hex</font></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">                  
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey1"><input type="radio" name="DefWEPKey" value="1"/><font id="GeneralWEPKEY1"> Key 1</font></td>
<td><input name="wep_key_1" id="WEP1" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey2"><input type="radio" name="DefWEPKey" value="2" /><font id="GeneralWEPKEY2"> Key 2</font></td>
<td><input name="wep_key_2" id="WEP2" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey3"><input type="radio" name="DefWEPKey" value="3" /><font id="GeneralWEPKEY3"> Key 3</font></td>
<td><input name="wep_key_3" id="WEP3" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey4"><input type="radio" name="DefWEPKey" value="4" /><font id="GeneralWEPKEY4"> Key 4</font></td>
<td><input name="wep_key_4" id="WEP4" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>
</span>

<input id==wep_key128 name=wep_key128 size=26 type=hidden>
              

<span id="div_wpapsk_compatible" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wpapsk_compatible" type="checkbox" value=1 />
<font id="GeneralWPAPSKCompatible">WPA Compatible</font>
</td>
</tr>
</table>
</li>
</span>                

<span id="div_wpa_compatible" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="wpa_compatible" type="checkbox" value=1 />
<font id="GeneralWPACompatible">WPA Compatible</font>
</td>
</tr>
</table>
</li>
</span>                
                
<span id="wpa_passphrase" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPSKPreSharedKey">Pre-Shared Key</td>
<td>
<input name="passphrase" id="passphrase" size="28" maxlength="64" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>
</span>
 
<span id="wpa_key_renewal_interval" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAKeyRenewInterval">Group Key Update Timer</td>
<td>
<input name="keyRenewalInterval" id="keyRenewalInterval" size="4" maxlength="4" value="3600" onKeyUp="setChange(1)"><font id="GeneralWPAkeyRenewalIntervalUnit"> seconds</font> 
</td>
</tr>
</table>
</li>
</span>


<span id="wpa_PMK_Cache_Period" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPMKCachePeriod">PMK Cache Period</td>
<td>
<input name="PMKCachePeriod" id="PMKCachePeriod" size="4" maxlength="4" value="" onKeyUp="setChange(1)"><font id="GeneralWPAPMKCachePeriodUnit"> minute</font> 
</td>
</tr>
</table>
</li>
</span>

<span id="wpa_preAuthentication" class="off">                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWPAPreAuth">Pre-Authentication</td>
<td>
<input name="PreAuthentication" id="PreAuthentication" value="0" type="radio" onClick="onPreAuthenticationClick(0)"><font id="GeneralWPAPreAuthDisable">Disable </font>
<input name="PreAuthentication" id="PreAuthentication" value="1" type="radio" onClick="onPreAuthenticationClick(1)"><font id="GeneralWPAPreAuthEnable">Enable </font>
</td>
</tr>
</table>
</li>
</span>                              


<!-- IEEE 802.1x WEP  -->
<span id="div_8021x_wep" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralIEEE8021xSettings">IEEE 802.1x Settings</td>
</tr>
</table>
</li>                
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="General1XWEP">WEP</td>
<td>
<input name="ieee8021x_wep" id="ieee8021x_wep" value="0" type="radio" onClick="onIEEE8021XWEPClick(0)"><font id="General1XWEPDisable">Disable</font>
<input name="ieee8021x_wep" id="ieee8021x_wep" value="1" type="radio" onClick="onIEEE8021XWEPClick(1)"><font id="General1XWEPEnable">Enable</font>
</td>
</tr>
</table>
</li>
</span>                

<!-- Radius Server  -->              
<span id="div_radius_server" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralAuthServer">Authentication Server</td>
</tr>
</table>
</li>
                                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusIPAddr">IP Address </td>
<td>
<input name="RadiusServerIP" id="RadiusServerIP" size="16" maxlength="32" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusPort">Port Number</td>
<td>
<input name="RadiusServerPort" id="RadiusServerPort" size="5" maxlength="5" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusSharedSecret">Shared Secret </td>
<td>
<input name="RadiusServerSecret" id="RadiusServerSecret" size="16" maxlength="64" value="" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusSessionTimeout">Session Timeout </td>
<td>
<input name="RadiusServerSessionTimeout" id="RadiusServerSessionTimeout" size="3" maxlength="4" value="0" onKeyUp="setChange(1)">
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="2%">&nbsp;</td>
<td width="38%" id="GeneralRadiusIdleTimeout">Idle Timeout </td>
<td>
<input name="RadiusServerIdleTimeout" id="RadiusServerIdleTimeout" size="3" maxlength="4" value="" onKeyUp="setChange(1)" readonly>
</td>
</tr>
</table>
</li>
</span>

<span id="div_note2_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span id="GeneralNote2" class="i_note">Note: WPA-PSK and WPA2-PSK can be configured when WPS enabled</span></td>
</tr>
</table>
</li>                
</span>
</span> <!-- span id="div_hide_security_id" class="off> -->

<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=button value=Apply id="GeneralApply" onclick="submit_apply()" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset value=Reset id="GeneralReset" onClick="window.location.reload()">
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
 