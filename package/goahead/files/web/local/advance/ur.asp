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

var keytype = "<% getCfgGeneral(1, "ApCliKey1Type"); %>";
var keytype = "<% getCfgGeneral(1, "ApCliKey2Type"); %>";
var keytype = "<% getCfgGeneral(1, "ApCliKey3Type"); %>";
var keytype = "<% getCfgGeneral(1, "ApCliKey4Type"); %>";
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

function SiteSurveySecurityModeSwitchInit()
{
	var enc = parent.site_survey_cipher;
	var mode = parent.site_survey_security_infra_mode;	
	show_div(false, "div_wep");
	show_div(false, "div_wpa_psk");	
	document.wireless_apcli.DefWEPKey.disabled = true;
	document.wireless_apcli.apcli_key1.disabled = true;
	document.wireless_apcli.apcli_key2.disabled = true;
	document.wireless_apcli.apcli_key3.disabled = true;
	document.wireless_apcli.apcli_key4.disabled = true;
	document.wireless_apcli.apcli_wpapsk.disabled = true;
	if ( ((document.wireless_apcli.apcli_mode.options.selectedIndex == 0)&&(key1 == "")&&(key2 == "")&&(key3 == "")&&(key4 == "")) ||
		((document.wireless_apcli.apcli_mode.options.selectedIndex == 0) && (enc == "NONE")) ){ // No Security

		show_div(false, "div_wep");
		show_div(false, "div_wpa_psk");	

	}
	else if ( (document.wireless_apcli.apcli_mode.options.selectedIndex == 1) || (document.wireless_apcli.apcli_mode.options.selectedIndex == 0)){	

		show_div(true, "div_wep");		
		document.wireless_apcli.DefWEPKey.disabled = false;
		document.wireless_apcli.apcli_key1.disabled = false;
		document.wireless_apcli.apcli_key2.disabled = false;
		document.wireless_apcli.apcli_key3.disabled = false;
		document.wireless_apcli.apcli_key4.disabled = false;

		//default Open
		document.wireless_apcli.auth_method.options.selectedIndex = 0;
		
		if (mode == "SHARED"){
			document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
			document.wireless_apcli.auth_method.options.selectedIndex = 1;
		}else if (mode == "OPEN"){
			document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
			document.wireless_apcli.auth_method.options.selectedIndex = 0;		
		}
		
		if ( (document.wireless_apcli.apcli_key1.value == "") && (document.wireless_apcli.apcli_key2.value == "") && (document.wireless_apcli.apcli_key3.value == "") && (document.wireless_apcli.apcli_key4.value == "") ){
			//Set ASCII default
			document.wireless_apcli.WEPKey_Code[0].checked = true; //ASCII	
		}else{
			document.wireless_apcli.WEPKey_Code[1].checked = true; //hex
		}	
    		if (document.wireless_apcli.wep_encry.options.selectedIndex == 0) {
			if (document.wireless_apcli.WEPKey_Code[1].checked == true) { //point to Hex
          			document.wireless_apcli.apcli_key1.maxLength = 10;
          			document.wireless_apcli.apcli_key2.maxLength = 10;
          			document.wireless_apcli.apcli_key3.maxLength = 10;
          			document.wireless_apcli.apcli_key4.maxLength = 10;
       			}
       			else { //point to ASCII
          			document.wireless_apcli.apcli_key1.maxLength = 5;
          			document.wireless_apcli.apcli_key2.maxLength = 5;
          			document.wireless_apcli.apcli_key3.maxLength = 5;
          			document.wireless_apcli.apcli_key4.maxLength = 5;
       			}
    		}
   		else {
       			if (document.wireless_apcli.WEPKey_Code[1].checked == true) { //point to Hex
          			document.wireless_apcli.apcli_key1.maxLength = 26;
          			document.wireless_apcli.apcli_key2.maxLength = 26;
          			document.wireless_apcli.apcli_key3.maxLength = 26;
          			document.wireless_apcli.apcli_key4.maxLength = 26;
       			}
       			else { //point to ASCII
          			document.wireless_apcli.apcli_key1.maxLength = 13;
          			document.wireless_apcli.apcli_key2.maxLength = 13;
          			document.wireless_apcli.apcli_key3.maxLength = 13;
          			document.wireless_apcli.apcli_key4.maxLength = 13;
       			}
    		}
	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 2) {
		show_div(true, "div_wpa_psk");			
		
		document.wireless_apcli.apcli_mode.disabled = false;		
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		document.wireless_apcli.cipher[0].checked = true;
				
		if (mode == "WPAPSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.cipher[0].checked = true;
			}else if (enc =='AES'){			
				document.wireless_apcli.cipher[1].checked = true;	
			}else{
				document.wireless_apcli.cipher[0].checked = true;			
			}
		}

	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 3) {

		show_div(true, "div_wpa_psk");
		document.wireless_apcli.apcli_mode.disabled = false;
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		document.wireless_apcli.cipher[1].checked = true;
		
		if (mode == "WPA2PSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.cipher[0].checked = true;	
			}else if (enc =='AES'){
				document.wireless_apcli.cipher[1].checked = true;	
			}else{
				document.wireless_apcli.cipher[1].checked = true;			
			}
		}
	}
	parent.adjustMyFrameHeight();
}

function SecurityModeSwitchInit()
{
	show_div(false, "div_wep");
	show_div(false, "div_wpa_psk");	
	document.wireless_apcli.DefWEPKey.disabled = true;
	document.wireless_apcli.apcli_key1.disabled = true;
	document.wireless_apcli.apcli_key2.disabled = true;
	document.wireless_apcli.apcli_key3.disabled = true;
	document.wireless_apcli.apcli_key4.disabled = true;
	document.wireless_apcli.apcli_wpapsk.disabled = true;

	if ( ((document.wireless_apcli.apcli_mode.options.selectedIndex == 0)&&(key1 == "")&&(key2 == "")&&(key3 == "")&&(key4 == "")) ||
		((document.wireless_apcli.apcli_mode.options.selectedIndex == 0) && (enc == "NONE")) ){ // No Security

		show_div(false, "div_wep");
		show_div(false, "div_wpa_psk");	

	}
	else if ( (document.wireless_apcli.apcli_mode.options.selectedIndex == 1) || (document.wireless_apcli.apcli_mode.options.selectedIndex == 0)){	

		show_div(true, "div_wep");		
		document.wireless_apcli.DefWEPKey.disabled = false;
		document.wireless_apcli.apcli_key1.disabled = false;
		document.wireless_apcli.apcli_key2.disabled = false;
		document.wireless_apcli.apcli_key3.disabled = false;
		document.wireless_apcli.apcli_key4.disabled = false;

		//default Open
		document.wireless_apcli.auth_method.options.selectedIndex = 0;
		
		if (mode == "SHARED"){
			document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
			document.wireless_apcli.auth_method.options.selectedIndex = 1;
		}else if (mode == "OPEN"){
			document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
			document.wireless_apcli.auth_method.options.selectedIndex = 0;		
		}
	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 2) {
		show_div(true, "div_wpa_psk");			
		
		document.wireless_apcli.apcli_mode.disabled = false;		
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		document.wireless_apcli.cipher[0].checked = true;
				
		if (mode == "WPAPSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.cipher[0].checked = true;
			}else if (enc =='AES'){
				document.wireless_apcli.cipher[1].checked = true;	
			}else{
				document.wireless_apcli.cipher[0].checked = true;			
			}
		}

	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 3) {

		show_div(true, "div_wpa_psk");
		document.wireless_apcli.apcli_mode.disabled = false;
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		document.wireless_apcli.cipher[1].checked = true;
		
		if (mode == "WPA2PSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.cipher[0].checked = true;	
			}else if (enc =='AES'){
				document.wireless_apcli.cipher[1].checked = true;	
			}else{
				document.wireless_apcli.cipher[1].checked = true;			
			}
		}
	}
	parent.adjustMyFrameHeight();
}

function SecurityModeSwitch()
{
	show_div(false, "div_wep");
	show_div(false, "div_wpa_psk");
	
	document.wireless_apcli.DefWEPKey.disabled = true;
	document.wireless_apcli.apcli_key1.disabled = true;
	document.wireless_apcli.apcli_key2.disabled = true;
	document.wireless_apcli.apcli_key3.disabled = true;
	document.wireless_apcli.apcli_key4.disabled = true;
	document.wireless_apcli.apcli_wpapsk.disabled = true;
	document.wireless_apcli.cipher.disabled = true;
		
	if (document.wireless_apcli.apcli_mode.options.selectedIndex == 0) {

		show_div(false, "div_wep");
		show_div(false, "div_wpa_psk");	
	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 1) {
	
	show_div(true, "div_wep");	
		document.wireless_apcli.DefWEPKey.disabled = false;
		document.wireless_apcli.apcli_key1.disabled = false;
		document.wireless_apcli.apcli_key2.disabled = false;
		document.wireless_apcli.apcli_key3.disabled = false;
		document.wireless_apcli.apcli_key4.disabled = false;

		//default Open
		document.wireless_apcli.auth_method.options.selectedIndex = 0;
		
		if ( (mode == "SHARED")||(mode == "OPEN") && (enc == "NONE")){

				document.wireless_apcli.auth_method.options.selectedIndex = 1;	
		}else if (mode == "OPEN"){

				document.wireless_apcli.auth_method.options.selectedIndex = 0;			
			}
		}		
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 2) {

	show_div(true, "div_wpa_psk");			
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		document.wireless_apcli.cipher.disabled = false;
		
		//WPA-PSK default TKIP
		document.wireless_apcli.cipher[0].checked = true;
		
		if (mode == "WPAPSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.cipher[0].checked = true;	
			}else if (enc =='AES'){
				document.wireless_apcli.cipher[1].checked = true;	
			}else{
				document.wireless_apcli.cipher[0].checked = true;			
			}
		}		
		
	}
	else if (document.wireless_apcli.apcli_mode.options.selectedIndex == 3) {

		show_div(true, "div_wpa_psk");
		document.wireless_apcli.apcli_wpapsk.disabled = false;
		
		//WPA2-PSK default AES
		document.wireless_apcli.cipher[1].checked = true;
		
		if (mode == "WPA2PSK"){
			if (enc =='TKIP'){
				document.wireless_apcli.cipher[0].checked = true;	
			}else if (enc =='AES'){
				document.wireless_apcli.cipher[1].checked = true;	
			}else{
				document.wireless_apcli.cipher[1].checked = true;			
			}
		}
	}
	parent.adjustMyFrameHeight();
}

function securityMode(c_f)
{
	var security_mode;

	changed = c_f;
	show_div(false, "div_wep");
	
	security_mode = document.wireless_apcli.apcli_mode.value;
	
	if (security_mode == "OPEN"){
		show_div(false, "div_wep");
		show_div(flase, "div_wpa_psk");		
	}else if (security_mode == "SHARED"){
		show_div(true, "div_wep");
	}else if (security_mode == "WPAPSK" || security_mode == "WPA2PSK"){
		show_div(true, "div_wpa_psk");
	}
}

function wep_switch_key_length()
{
    document.wireless_apcli.apcli_key1.value = "";
    document.wireless_apcli.apcli_key2.value = "";
    document.wireless_apcli.apcli_key3.value = "";
    document.wireless_apcli.apcli_key4.value = "";

    if (document.wireless_apcli.wep_encry.options.selectedIndex == 0) {
	if (document.wireless_apcli.WEPKey_Code[1].checked == true) { //point to Hex
          document.wireless_apcli.apcli_key1.maxLength = 10;
          document.wireless_apcli.apcli_key2.maxLength = 10;
          document.wireless_apcli.apcli_key3.maxLength = 10;
          document.wireless_apcli.apcli_key4.maxLength = 10;
       }
       else { //point to ASCII
          document.wireless_apcli.apcli_key1.maxLength = 5;
          document.wireless_apcli.apcli_key2.maxLength = 5;
          document.wireless_apcli.apcli_key3.maxLength = 5;
          document.wireless_apcli.apcli_key4.maxLength = 5;
       }
    }
    else {
       	if (document.wireless_apcli.WEPKey_Code[1].checked == true) { //point to Hex
          document.wireless_apcli.apcli_key1.maxLength = 26;
          document.wireless_apcli.apcli_key2.maxLength = 26;
          document.wireless_apcli.apcli_key3.maxLength = 26;
          document.wireless_apcli.apcli_key4.maxLength = 26;
       }
       else { //point to ASCII
          document.wireless_apcli.apcli_key1.maxLength = 13;
          document.wireless_apcli.apcli_key2.maxLength = 13;
          document.wireless_apcli.apcli_key3.maxLength = 13;
          document.wireless_apcli.apcli_key4.maxLength = 13;
       }
    }
}
function initTranslation()
{
	var e = document.getElementById("URTitle");
	e.innerHTML = _("UR Title");
	
	e = document.getElementById("APClientSSID");
	e.innerHTML = _("station ssid");
		
	e = document.getElementById("GeneralChannelSelect");
	e.innerHTML = _("channel selection");
	
	e = document.getElementById("addprofSecureMode");
	e.innerHTML = _("addprof security mode");
	e = document.getElementById("GeneralEncrypOpen");
	e.innerHTML = _("general nosecurity");	
	e = document.getElementById("GeneralSharedKey");
	e.innerHTML = _("general static wep");
	e = document.getElementById("GeneralWEPPassPhrase");
	e.innerHTML = _("general wep passphrase");
	e = document.getElementById("GeneralWEPEncryp");
	e.innerHTML = _("general wep encryp");
	e = document.getElementById("GeneralWEPEncryp64");
	e.innerHTML = _("general wep encryp64");	
	e = document.getElementById("GeneralWEPEncryp128");
	e.innerHTML = _("general wep encryp128");	
	e = document.getElementById("GeneralWPAPSK");
	e.innerHTML = _("general wpapsk");
	e = document.getElementById("GeneralWPA2PSK");
	e.innerHTML = _("general wpa2psk");	
	e = document.getElementById("GeneralAuthMethod");
	e.innerHTML = _("addprof secure wpa algorithm");
	e = document.getElementById("GeneralEncrypAuto");
	e.innerHTML = _("general encryp open");
	e = document.getElementById("GeneralEncrypSharedKey");
	e.innerHTML = _("general encryp sharedkey");
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

	e = document.getElementById("wpapsk_Pass_Phrase");
	e.innerHTML = _("general wpapsk presharedkey");
	e = document.getElementById("addprofWPATKIP");
	e.innerHTML = _("addprof wpa tkip");

	e = document.getElementById("addprofWPAAES");
	e.innerHTML = _("addprof wpa aes");
	
	e = document.getElementById("addprofWPAAlg");
	e.innerHTML = _("encryption type");
	
  	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}	
function initValue()
{
  initTranslation();

	
if (parent.site_survey_setting == 0)
{
	if(apclienable == 1){
		// Enable
		document.wireless_apcli.apcli_enable.checked = true;
	}else{
		// Disable
		document.wireless_apcli.apcli_enable.checked = false;
	}	

	document.wireless_apcli.apcli_ssid.value = ssid;
	
	var general_channel = <% getOpChannleNum(); %>;

	document.wireless_apcli.apcli_channel.value = general_channel;
	
	var mode = "<% getCfgGeneral(1, "ApCliAuthMode"); %>";

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

	if (keytype == ''){
		keytype = 1;
	
	}else{	
		keytype = keytype - '0';
	}	
	if (keytype == 0){
		document.wireless_apcli.WEPKey_Code[1].checked = true;
		document.wireless_apcli.WEPKey_Code[1].checked = true;
		document.wireless_apcli.WEPKey_Code[1].checked = true;
		document.wireless_apcli.WEPKey_Code[1].checked = true;
	}else{
		document.wireless_apcli.WEPKey_Code[0].checked = true;
		document.wireless_apcli.WEPKey_Code[0].checked = true;
		document.wireless_apcli.WEPKey_Code[0].checked = true;
		document.wireless_apcli.WEPKey_Code[0].checked = true;	
	}	
	var keyid = "<% getCfgGeneral(1, "ApCliDefaultKeyId"); %>";
	
	document.wireless_apcli.DefWEPKey[0].checked= false;
	document.wireless_apcli.DefWEPKey[1].checked= false;
	document.wireless_apcli.DefWEPKey[2].checked= false;
	document.wireless_apcli.DefWEPKey[3].checked= false;

	document.wireless_apcli.DefWEPKey[keyid-1].checked= true;
			
	document.wireless_apcli.apcli_key1.value = key1;
	document.wireless_apcli.apcli_key2.value = key2;
	document.wireless_apcli.apcli_key3.value = key3;
	document.wireless_apcli.apcli_key4.value = key4;
	
	document.wireless_apcli.apcli_wpapsk.value = wpapsk;
		
	SecurityModeSwitchInit();

    		if (document.wireless_apcli.wep_encry.options.selectedIndex == 0) {
			if (document.wireless_apcli.WEPKey_Code[1].checked == true) { //point to Hex
          			document.wireless_apcli.apcli_key1.maxLength = 10;
          			document.wireless_apcli.apcli_key2.maxLength = 10;
          			document.wireless_apcli.apcli_key3.maxLength = 10;
          			document.wireless_apcli.apcli_key4.maxLength = 10;
       			}
       			else { //point to ASCII
          			document.wireless_apcli.apcli_key1.maxLength = 5;
          			document.wireless_apcli.apcli_key2.maxLength = 5;
          			document.wireless_apcli.apcli_key3.maxLength = 5;
          			document.wireless_apcli.apcli_key4.maxLength = 5;
       			}
    		}
   		else {
       			if (document.wireless_apcli.WEPKey_Code[1].checked == true) { //point to Hex
          			document.wireless_apcli.apcli_key1.maxLength = 26;
          			document.wireless_apcli.apcli_key2.maxLength = 26;
          			document.wireless_apcli.apcli_key3.maxLength = 26;
          			document.wireless_apcli.apcli_key4.maxLength = 26;
       			}
       			else { //point to ASCII
          			document.wireless_apcli.apcli_key1.maxLength = 13;
          			document.wireless_apcli.apcli_key2.maxLength = 13;
          			document.wireless_apcli.apcli_key3.maxLength = 13;
          			document.wireless_apcli.apcli_key4.maxLength = 13;
       			}
    		}

	// Now, 4 Keys type are same, so only get the first key to display HEX or ASCII. It's OK
	if ( (document.wireless_apcli.apcli_key1.value == "") && (document.wireless_apcli.apcli_key2.value == "") && (document.wireless_apcli.apcli_key3.value == "") && (document.wireless_apcli.apcli_key4.value == "") ){
		//Set ASCII default
		document.wireless_apcli.WEPKey_Code[0].checked = true; //ASCII	
	}else if (keytype == "0"){

		document.wireless_apcli.WEPKey_Code[1].checked = true; //hex

		if (document.wireless_apcli.apcli_key1.value != ""){
			if (document.wireless_apcli.apcli_key1.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (document.wireless_apcli.apcli_key2.value != ""){
			if (document.wireless_apcli.apcli_key2.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (document.wireless_apcli.apcli_key3 != ""){
			if (document.wireless_apcli.apcli_key3.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (document.wireless_apcli.apcli_key4 != ""){
			if (document.wireless_apcli.apcli_key4.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}		
		}
	}else{
		document.wireless_apcli.WEPKey_Code[0].checked = true; //ASCII

		if (document.wireless_apcli.apcli_key1.value != ""){
			if (document.wireless_apcli.apcli_key1.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (document.wireless_apcli.apcli_key2.value != ""){
			if (document.wireless_apcli.apcli_key2.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (document.wireless_apcli.apcli_key3 != ""){
			if (document.wireless_apcli.apcli_key3.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (document.wireless_apcli.apcli_key4 != ""){
			if (document.wireless_apcli.apcli_key4.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}		
		}
	}

	if(mode == "OPEN"){
		document.getElementById("auth_method").selectedIndex = 0; // OPEN
	}else if (enc == "WEP"){                                            //if(mode == "SHARED"){
		document.getElementById("auth_method").selectedIndex = 1; // Shared Key
	}
	else{	
		document.getElementById("auth_method").selectedIndex = 1; // default OPEN
	}
}
else {

	parent.site_survey_setting = 0;
	
	document.wireless_apcli.apcli_enable.checked = true;
	
	document.wireless_apcli.apcli_ssid.value = parent.site_survey_ssid;
	var network_type = parent.site_survey_network_type;
	var enc = parent.site_survey_cipher;
	var mode = parent.site_survey_security_infra_mode;
	document.wireless_apcli.apcli_channel.value = parent.site_survey_channel;	
	document.wireless_apcli.apcli_bssid.value = parent.site_survey_bssid;

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

	if (enc == "WEP")
		document.wireless_apcli.apcli_mode.options.selectedIndex = 1;
	
	document.wireless_apcli.apcli_key1.value = "";
	document.wireless_apcli.apcli_key2.value = "";
	document.wireless_apcli.apcli_key3.value = "";
	document.wireless_apcli.apcli_key4.value = "";	


	document.wireless_apcli.WEPKey_Code.value = 0;
	document.wireless_apcli.WEPKey_Code.value = 0;
	document.wireless_apcli.WEPKey_Code.value = 0;
	document.wireless_apcli.WEPKey_Code.value = 0;
		
	var keyid = 1;
	document.wireless_apcli.DefWEPKey[0].checked= false;
	document.wireless_apcli.DefWEPKey[1].checked= false;
	document.wireless_apcli.DefWEPKey[2].checked= false;
	document.wireless_apcli.DefWEPKey[3].checked= false;

	document.wireless_apcli.DefWEPKey[keyid-1].checked= true;
	
	document.wireless_apcli.apcli_wpapsk.value = "";

	SiteSurveySecurityModeSwitchInit();
//	SecurityModeSwitchInit()
	// Now, 4 Keys type are same, so only get the first key to display HEX or ASCII. It's OK
	if ( (document.wireless_apcli.apcli_key1.value == "") && (document.wireless_apcli.apcli_key2.value == "") && (document.wireless_apcli.apcli_key3 == "") && (document.wireless_apcli.apcli_key4 == "") ){
		//Set ASCII default
		document.wireless_apcli.WEPKey_Code[0].checked = true; //ASCII	
	}else if (keytype == "0"){
		document.wireless_apcli.WEPKey_Code[1].checked = true; //hex

		if (document.wireless_apcli.apcli_key1.value != ""){
			if (document.wireless_apcli.apcli_key1.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (document.wireless_apcli.apcli_key2.value != ""){
			if (document.wireless_apcli.apcli_key2.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (document.wireless_apcli.apcli_key3 != ""){
			if (document.wireless_apcli.apcli_key3.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}
		}else if (document.wireless_apcli.apcli_key4 != ""){
			if (document.wireless_apcli.apcli_key4.value.length > 10){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;
			}		
		}
	}else{
		document.wireless_apcli.WEPKey_Code[0].checked = true; //ASCII

		if (document.wireless_apcli.apcli_key1.value != ""){
			if (document.wireless_apcli.apcli_key1.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (document.wireless_apcli.apcli_key2.value != ""){
			if (document.wireless_apcli.apcli_key2.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (document.wireless_apcli.apcli_key3 != ""){
			if (document.wireless_apcli.apcli_key3.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}
		}else if (document.wireless_apcli.apcli_key4 != ""){
			if (document.wireless_apcli.apcli_key4.value.length > 5){
				document.getElementById("wep_encry").selectedIndex = 1;	
			}else{
				document.getElementById("wep_encry").selectedIndex = 0;	
			}		
		}
	}

	if(mode == "OPEN"){
		document.getElementById("auth_method").selectedIndex = 0; // OPEN
	}else if (enc == "WEP"){                                            //if(mode == "SHARED"){
		document.getElementById("auth_method").selectedIndex = 1; // Shared Key
	}
	else{	
		document.getElementById("auth_method").selectedIndex = 1; // default OPEN
	}	

	
}			
	if ((wirelessmode != 5) && (wirelessmode != 6)){
	    alert("Please select WISP mode to configure this page");
	}
	parent.adjustMyFrameHeight();	
//	securityMode(0);
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
	var defaultid = 0;	
	var keylen = 0 ;
	var securitymode;
	
	securitymode = document.wireless_apcli.apcli_mode.value;

	if (document.wireless_apcli.DefWEPKey[0].checked == true){
		defaultid = 1;
		var keyvalue = document.wireless_apcli.apcli_key1.value;
	}else if (document.wireless_apcli.DefWEPKey[1].checked == true){
		defaultid = 2;
		var keyvalue = document.wireless_apcli.apcli_key2.value;
	}else if (document.wireless_apcli.DefWEPKey[2].checked == true){
		defaultid = 3;
		var keyvalue = document.wireless_apcli.apcli_key3.value;
	}else if (document.wireless_apcli.DefWEPKey[3].checked == true){
		defaultid = 4;
		var keyvalue = document.wireless_apcli.apcli_key4.value;
	}	
	
	if (keyvalue.length == 0 &&  securitymode == "SHARED"){ 
		alert(_('Please input wep key')+defaultid+'!');
		return false;
	}
			
	var keylength = document.wireless_apcli.apcli_key1.value.length;
	if (keylength != 0){
		if (document.wireless_apcli.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key1 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key1.value)== false){
					alert('Wep key1 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key1 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key1.value)== false){
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
				if(checkHex(document.wireless_apcli.apcli_key1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key1 !');
					return false;
				}
				if(checkHex(document.wireless_apcli.apcli_key1.value) == false){
					alert('Invalid Wep key1 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.wireless_apcli.apcli_key2.value.length;
	if (keylength != 0){
		if (document.wireless_apcli.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key2 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key2.value)== false){
					alert('Wep key2 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key2 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key2.value)== false){
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
				if(checkHex(document.wireless_apcli.apcli_key2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key2 !');
					return false;
				}
				if(checkHex(document.wireless_apcli.apcli_key2.value) == false){
					alert('Invalid Wep key2 format!');
					return false;
				}							
			}		
		
		}
	}

	var keylength = document.wireless_apcli.apcli_key3.value.length;
	if (keylength != 0){
		if (document.wireless_apcli.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key3 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key3.value)== false){
					alert('Wep key3 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key3 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key3.value)== false){
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
				if(checkHex(document.wireless_apcli.apcli_key3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key3 !');
					return false;
				}
				if(checkHex(document.wireless_apcli.apcli_key3.value) == false){
					alert('Invalid Wep key3 format!');
					return false;
				}							
			}		
		
		}
	}	

	var keylength = document.wireless_apcli.apcli_key4.value.length;
	if (keylength != 0){
		if (document.wireless_apcli.WEPKey_Code[0].checked == true){ //ASCII
			if (document.getElementById("wep_encry").selectedIndex == 0 ){ // 64-bits (ASCII)
				if(keylength != 5) {
					alert('Please input 5 characters of wep key4 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key4.value)== false){
					alert('Wep key4 contains invalid characters.');
					return false;
				}
			}else{ // 128-bits (ASCII)
				if(keylength != 13) {
					alert('Please input 13 characters of wep key4 !');
					return false;
				}
				if(checkInjection(document.wireless_apcli.apcli_key4.value)== false){
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
				if(checkHex(document.wireless_apcli.apcli_key4.value) == false){
					alert('Invalid Wep key4 format!');
					return false;
				}			
			}else{ // 128-bits (HEX)
				if(keylength != 26) {
					alert('Please input 26 characters of wep key4 !');
					return false;
				}
				if(checkHex(document.wireless_apcli.apcli_key4.value) == false){
					alert('Invalid Wep key4 format!');
					return false;
				}							
			}		
				
	}	
	
	}
}

function setChange(c){
	changed = c;
}

function CheckWpa()
{
	if (document.wireless_apcli.apcli_wpapsk.value.length < 8) {
		alert("Pass Phrase length should be larger than 8!");
		document.wireless_apcli.apcli_wpapsk.focus();
		return false;
	}
	if (!checkInjection(document.wireless_apcli.apcli_wpapsk.value)) {
		alert('Invalid characters in Pass Phrase.');
		document.wireless_apcli.apcli_wpapsk.focus();
		return false;
	}
	
	if (checkHex(document.wireless_apcli.apcli_wpapsk.value) == true){ // Hex
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

//	if ( (document.wireless_apcli.apcli_mode.options.selectedIndex == 1) && (document.wireless_apcli.auth_method.options.selectedIndex == 0) ){
//		document.wireless_apcli.apcli_mode.value = "OPEN"; // Auth = OPEN
 //alert("document.wireless_apcli.apcli_mode.value ="+document.wireless_apcli.apcli_mode.value);
 //		document.wireless_apcli.apcli_enc.value = "WEP"; // Enc = WEP
//	}	
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

function parse40WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.wireless_apcli.apcli_key1.value = fields_str[0];
	document.wireless_apcli.apcli_key2.value = fields_str[1];
	document.wireless_apcli.apcli_key3.value = fields_str[2];
	document.wireless_apcli.apcli_key4.value = fields_str[3];
}

function parse128WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.wireless_apcli.apcli_key1.value = fields_str[0];
	document.wireless_apcli.apcli_key2.value = fields_str[0];
	document.wireless_apcli.apcli_key3.value = fields_str[0];
	document.wireless_apcli.apcli_key4.value = fields_str[0];
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

function generate_wep()
{
	var passphrase;
	passphrase = document.wireless_apcli.wep_passphrase.value;

	document.wireless_apcli.WEPKey_Code[1].checked = true; //Hex	
	if (document.getElementById("wep_encry").selectedIndex == 1){ // get 128 bits WEP KEY
		makeRequest("/goform/wifiget128wepkey", passphrase, get128wepeyHandler);
	}else{ // get 40 bits WEP KEY
		makeRequest("/goform/wifiget40wepkey", passphrase, get40wepeyHandler);
	}
}
</script>
</head>
<body onLoad="initValue()">
<form method=post name=wireless_apcli action="/goform/urwifiApcli" onSubmit="return CheckValue()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>

<li class='title' id='URTitle'>Universal Repeater Parameters</li>

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

<!--<span id = "site_survey_bssid" class = "off" >  -->
<!-- <td><input type=hidden name="apcli_channel0" size=20 maxlength=32></td>  -->
<td><input type=hidden name="apcli_bssid" size=20 maxlength=32></td>
<!--  </span>  -->
	
<!--<span id = "general_channel" class = "off" > -->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td id ="GeneralChannelSelect" width="40%" nowrap>Channel Selection</td>
<td>
<select id="apcli_channel" name="apcli_channel" size="1">
<% getGeneralChannelsFreq(); %>
</select>                        
</td>
</tr>
</table>
</li>
<!--  </span>  -->

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr> 
<td width="40%" id="addprofSecureMode">Security Mode</td>
<td>
<select name="apcli_mode" id="apcli_mode" size="1" onchange="SecurityModeSwitch()">
<option value="OPEN" id="GeneralEncrypOpen">No Security</option>
<option value="SHARED" id="GeneralSharedKey">Static WEP</option>
<option value="WPAPSK" id="GeneralWPAPSK">WPA-PSK</option>
<option value="WPA2PSK" id="GeneralWPA2PSK">WPA2-PSK</option>
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
<select name="wep_encry" id="wep_encry" size="1" onchange="wep_switch_key_length()">
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
<select name="auth_method" id="auth_method" size="1" onchange="">
<option id="GeneralEncrypAuto" selected value="0">Open      </option>
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
<input type="radio" name="WEPKey_Code" value="1" onclick="wep_switch_key_length();" /><font id="GeneralWEPASCII">ASCII </font>
<input type="radio" name="WEPKey_Code" value="0" onclick="wep_switch_key_length();" /><font id="GeneralWEPHex">Hex</font></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">                  
<tr id="div_apcli_key1">
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey1"><input type="radio" name="DefWEPKey" value="1"/><font id="GeneralWEPKEY1"> Key 1</font></td>
<td><input name="apcli_key1" id="apcli_key1" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">                  
<tr id="div_apcli_key2">
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey2"><input type="radio" name="DefWEPKey" value="2"/><font id="GeneralWEPKEY2"> Key 2</font></td>
<td><input name="apcli_key2" id="apcli_key2" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">                  
<tr id="div_apcli_key3">
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey3"><input type="radio" name="DefWEPKey" value="3"/><font id="GeneralWEPKEY3"> Key 3</font></td>
<td><input name="apcli_key3" id="apcli_key3" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">                  
<tr id="div_apcli_key4">
<td width="10%"></td>                    
<td width="15%" id="secureWEPKey4"><input type="radio" name="DefWEPKey" value="4"/><font id="GeneralWEPKEY4"> Key 4</font></td>
<td><input name="apcli_key4" id="apcli_key4" size="28" maxlength="26" value="" onKeyUp="setChange(1)"></td>
</tr>
</table>
</li>

</span>

<!-- WPA-PSK/ WPA2-PSK -->
<span id="div_wpa_psk" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr id="div_wpa_algorithms" name="div_wpa_algorithms"> 
<td width="40%" class="head" id="addprofWPAAlg">Encryption Type</td>
<td>
<input type=radio name="cipher" id="cipher" value="0" checked><font id="addprofWPATKIP">TKIP </font>&nbsp;
<input type=radio name="cipher" id="cipher" value="1"><font id="addprofWPAAES">AES </font>&nbsp;
</td>
</tr>
</table>
</li>  

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">     
<tr id="div_apcli_wpapsk"> 
<td width="40%" id="wpapsk_Pass_Phrase">Pre-Shared Key</td>
<td><input type=password name="apcli_wpapsk" size=20 maxlength=64></td>
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
