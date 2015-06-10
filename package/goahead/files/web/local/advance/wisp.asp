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

var ch1 = "<option value = 1 >1</option>";
var ch2 = "<option value = 2 >2</option>";
var ch3 = "<option value = 3 >3</option>";
var ch4 = "<option value = 4 >4</option>";
var ch5 = "<option value = 5 >5</option>";
var ch6 = "<option value = 6 >6</option>";
var ch7 = "<option value = 7 >7</option>";
var ch8 = "<option value = 8 >8</option>";
var ch9 = "<option value = 9 >9</option>";
var ch10 = "<option value = 10>10</option>";
var ch11 = "<option value = 11>11</option>";
var ch12 = "<option value = 12>12</option>";
var ch13 = "<option value = 13>13</option>";
var ch14 = "<option value = 14>14</option>";
var ch36 = "<option value = 36 >36</option>";
var ch40 = "<option value = 40 >40</option>";
var ch44 = "<option value = 44 >44</option>";
var ch48 = "<option value = 48 >48</option>";
var ch52 = "<option value = 52 >52</option>";
var ch56 = "<option value = 56 >56</option>";
var ch60 = "<option value = 60 >60</option>";
var ch64 = "<option value = 64 >64</option>";
var ch100 = "<option value = 100>100</option>";
var ch104 = "<option value = 104>104</option>";
var ch108 = "<option value = 108>108</option>";
var ch112 = "<option value = 112>112</option>";
var ch116 = "<option value = 116>116</option>";
var ch120 = "<option value = 120>120</option>";
var ch124 = "<option value = 124>124</option>";
var ch128 = "<option value = 128>128</option>";
var ch132 = "<option value = 132>132</option>";
var ch136 = "<option value = 136>136</option>";
var ch140 = "<option value = 140>140</option>";
var ch149 = "<option value = 149>149</option>";
var ch153 = "<option value = 153>153</option>";
var ch157 = "<option value = 157>157</option>";
var ch161 = "<option value = 161>161</option>";
var ch165 = "<option value = 165>165</option>";

var ch34 = "<option value = 34 >34</option>";
var ch38 = "<option value = 38 >38</option>";
var ch42 = "<option value = 42 >42</option>";
var ch46 = "<option value = 46 >46</option>";

function show_div(show,id) {

	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

function initTranslation()
{
	var e = document.getElementById("addprofSysConf");
	e.innerHTML = _("addprof system config");

	e = document.getElementById("addprofProfName");
	e.innerHTML = _("addprof profile name");

	e = document.getElementById("addprofSSID");
	e.innerHTML = _("addprof station ssid");

	e = document.getElementById("addprofNetType");
	e.innerHTML = _("addprof network type");

	e = document.getElementById("addprofAdHoc");
	e.innerHTML = _("addprof adhoc");

	e = document.getElementById("addprofInfra");
	e.innerHTML = _("addprof infrastructure");

	e = document.getElementById("addprofPWSave");
	e.innerHTML = _("addprof power save");

	e = document.getElementById("addprofCAM");
	e.innerHTML = _("addprof cam");

	e = document.getElementById("addprofPWSaveMode");
	e.innerHTML = _("addprof power save");

	e = document.getElementById("addprofChannel");
	e.innerHTML = _("addprof station channel");

	e = document.getElementById("addprofPreambleType");
	e.innerHTML = _("addprof preamble type");

	e = document.getElementById("addprofPremableAuto");
	e.innerHTML = _("addprof wireless auto");

	e = document.getElementById("addprofPremableLong");
	e.innerHTML = _("addprof wireless long");

	e = document.getElementById("addprofRTS");
	e.innerHTML = _("addprof rts threshold");

	e = document.getElementById("addprofFrag");
	e.innerHTML = _("addprof fragment threshold");

	e = document.getElementById("addprofSecurePolicy");
	e.innerHTML = _("addprof secure policy");

	e = document.getElementById("GeneralEncrypSharedKey");
	e.innerHTML = _("general encryp sharedkey");

	e = document.getElementById("GeneralEncrypOpen");
	e.innerHTML = _("general encryp open");	

	e = document.getElementById("GeneralNoSecurity");
	e.innerHTML = _("general nosecurity");

	e = document.getElementById("GeneralSharedKey");
	e.innerHTML = _("general static wep");

	e = document.getElementById("GeneralWPAPSK");
	e.innerHTML = _("general wpapsk");

	e = document.getElementById("GeneralWPA2PSK");
	e.innerHTML = _("general wpa2psk");	

	e = document.getElementById("addprofSecureMode");
	e.innerHTML = _("addprof security mode");

	e = document.getElementById("addprofAdHocSecure");
	e.innerHTML = _("secure security mode");

	e = document.getElementById("GeneralWEPPassPhrase");
	e.innerHTML = _("general wep passphrase");

	e = document.getElementById("GeneralWEPGenerate");
	e.value = _("general wep generate");

	e = document.getElementById("GeneralAuthMethod");
	e.innerHTML = _("general auth method");

	e = document.getElementById("GeneralNote1_1");
	e.innerHTML = _("general note1_1");

	e = document.getElementById("GeneralNote1_2");
	e.innerHTML = _("general note1_2");

	e = document.getElementById("GeneralNote1_3");
	e.innerHTML = _("general note1_3");

	e = document.getElementById("GeneralNote1_4");
	e.innerHTML = _("general note1_4");	

	e = document.getElementById("addprofWEPKeyLength");
	e.innerHTML = _("addprof wep key length");

	e = document.getElementById("addprofWEP64bit");
	e.innerHTML = _("addprof wep bit64");

	e = document.getElementById("addprofWEP128bit");
	e.innerHTML = _("addprof wep bit128");	

	e = document.getElementById("addprofHex");
	e.innerHTML = _("addprof hex");

	e = document.getElementById("addprofASCII");
	e.innerHTML = _("addprof ascii");

	e = document.getElementById("addprofWEPKey");
	e.innerHTML = _("addprof secure wep key");

	e = document.getElementById("addprofWEPKey1");
	e.innerHTML = _("addprof secure wep key1");

	e = document.getElementById("addprofWEPKey2");
	e.innerHTML = _("addprof secure wep key2");

	e = document.getElementById("addprofWEPKey3");
	e.innerHTML = _("addprof secure wep key3");

	e = document.getElementById("addprofWEPKey4");
	e.innerHTML = _("addprof secure wep key4");

	e = document.getElementById("addprofWPA");
	e.innerHTML = _("addprof secure wpa");

	e = document.getElementById("addprofWPAAlg");
	e.innerHTML = _("addprof secure wpa algorithm");

	e = document.getElementById("addprofWPATKIP");
	e.innerHTML = _("addprof wpa tkip");

	e = document.getElementById("addprofWPAAES");
	e.innerHTML = _("addprof wpa aes");	

	e = document.getElementById("addprofPassPhrase");
	e.innerHTML = _("addprof secure wpa pass phrase");

	e = document.getElementById("addprof1XAuthType");
	e.innerHTML = _("addprof 8021X Auth Type");

	e = document.getElementById("addprofWPAAuthType");
	e.innerHTML = _("addprof 8021X Auth Type");

	e = document.getElementById("addprofPEAPTunnelAtuth");
	e.innerHTML = _("addprof tunnel auth");

	e = document.getElementById("addprofTTLSTunnelAuth");
	e.innerHTML = _("addprof tunnel auth");

	e = document.getElementById("addprofIdentity");
	e.innerHTML = _("addprof identity");

	e = document.getElementById("addprofPasswd");
	e.innerHTML = _("addprof passwd");

	e = document.getElementById("addprofClientCert");
	e.innerHTML = _("addprof client cert");

	e = document.getElementById("addprofClientCertUsed");
	e.innerHTML = _("addprof station used");

	e = document.getElementById("addprofClientCertPath");
	e.innerHTML = _("addprof client cert path");

	e = document.getElementById("addprofPrivateKeyPath");
	e.innerHTML = _("addprof private key path");

	e = document.getElementById("addprofPrivateKeyPasswd");
	e.innerHTML = _("addprof private key passwd");

	e = document.getElementById("addprofCACert");
	e.innerHTML = _("addprof ca cert");

	e = document.getElementById("addprofCACertUsed");
	e.innerHTML = _("addprof station used");

	e = document.getElementById("addprofCACertPath");
	e.innerHTML = _("addprof ca cert path");

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");

	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}
	
function initValue()
{
	initTranslation();
	
	var initProfile = "<% getProfile(); %>";
	show_div(false, "div_addprofProfName_id");
	var PointToFirstProfile = "<% getfirstStaProfile(); %>";
	var ProfileName = "<% getStaProfileData(1); %>";
	
if (parent.site_survey_setting==0)
{
	if (ProfileName == "0"){
		show_div(false, "div_power_saving_mode_id");
		document.profile_page.power_saving_mode.disabled = true;

		show_div(false, "div_channel_id");
		document.profile_page.channel.disabled = true;

		show_div(false, "div_b_premable_type_id");
		document.profile_page.b_premable_type.disabled = true;	

		var tmp = "<% getStaNewProfileName(); %>";
		document.profile_page.profile_name.value = tmp;

		document.profile_page.wep_default_key[0].checked = true;
	}else{
		document.profile_page.profile_name.value = "<% getStaProfileData(1); %>";
		document.profile_page.Ssid.value = "<% getStaProfileData(2); %>";
		document.profile_page.network_type.value = <% getStaProfileData(3); %>;
		document.profile_page.power_saving_mode['<%getStaProfileData(4);%>'].checked = true;
		document.profile_page.b_premable_type.value = "<% getStaProfileData(5); %>";

		document.profile_page.rts_thresholdvalue.value = "<% getStaProfileData(7); %>";
		document.profile_page.fragment_thresholdvalue.value = "<% getStaProfileData(9); %>";

		var encryp = <% getStaProfileData(10); %>;
		var auth = <% getStaProfileData(11); %>;

		document.profile_page.wep_key_1.value = "<% getStaProfileData(12); %>";
		document.profile_page.wep_key_2.value = "<% getStaProfileData(13); %>";
		document.profile_page.wep_key_3.value = "<% getStaProfileData(14); %>";
		document.profile_page.wep_key_4.value = "<% getStaProfileData(15); %>";
		document.profile_page.wep_key_entry_method.value = <% getStaProfileData(16); %>;
		document.profile_page.wep_key_entry_method.value = <% getStaProfileData(17); %>;
		document.profile_page.wep_key_entry_method.value = <% getStaProfileData(18); %>;
		document.profile_page.wep_key_entry_method.value = <% getStaProfileData(19); %>;
		document.profile_page.wep_key_length.value = <% getStaProfileData(20); %>;
		document.profile_page.wep_key_length.value = <% getStaProfileData(21); %>;
		document.profile_page.wep_key_length.value = <% getStaProfileData(22); %>;
		document.profile_page.wep_key_length.value = <% getStaProfileData(23); %>;

		var keydefaultid = <% getStaProfileData(24); %>;

		document.profile_page.wep_default_key[0].checked= false;
		document.profile_page.wep_default_key[1].checked= false;
		document.profile_page.wep_default_key[2].checked= false;
		document.profile_page.wep_default_key[3].checked= false;

		document.profile_page.wep_default_key[keydefaultid-1].checked= true;

		document.profile_page.passphrase.value = "<% getStaProfileData(25); %>";
        
		show_div(false, "div_power_saving_mode_id");
		document.profile_page.power_saving_mode.disabled = true;
        
		show_div(false, "div_channel_id");
		document.profile_page.channel.disabled = true;

		show_div(false, "div_b_premable_type_id");
		document.profile_page.b_premable_type.disabled = true;

		var keymgmt = <% getStaProfileData(26); %>;
		var eap = <% getStaProfileData(27); %>;

		if (keymgmt == 1) //Rtwpa_supplicantKeyMgmtWPAEAP,
			document.profile_page.cert_auth_type_from_wpa.value = eap;
		else if (keymgmt == 2) //Rtwpa_supplicantKeyMgmtIEEE8021X,
			document.profile_page.cert_auth_type_from_1x.value = eap;

		document.profile_page.cert_id.value = "<% getStaProfileData(28); %>";
		var cacert = "<% getStaProfileData(29); %>";
		document.profile_page.cert_ca_cert_path.value = cacert;
		var clientcert = "<% getStaProfileData(30); %>";
		document.profile_page.cert_client_cert_path.value = clientcert;
		document.profile_page.cert_private_key_path.value = "<% getStaProfileData(31); %>";
		document.profile_page.cert_private_key_password.value = "<% getStaProfileData(32); %>";
		document.profile_page.cert_password.value = "<% getStaProfileData(33); %>";
		var tunnel = <% getStaProfileData(34); %>;

		if (eap == 5 ) // Rtwpa_supplicantEAPPEAP,
			document.profile_page.cert_tunnel_auth_peap.value = tunnel;
		else if (eap == 6 ) // Rtwpa_supplicantEAPTTLS,
			document.profile_page.cert_tunnel_auth_ttls.value  = tunnel;

		if (cacert.length > 1) // != '0'
			document.profile_page.cert_use_ca_cert.checked = true;
		if (clientcert.length > 1) // != '0'
			document.profile_page.cert_use_client_cert.checked = true;
	}

	networkTypeChangeInit(auth, encryp);
}
else
{
	parent.site_survey_setting=0;
	if (ProfileName == "0"){
		var tmp = "<% getStaNewProfileName(); %>";
		document.profile_page.profile_name.value = tmp;
	}
	else
	{
		document.profile_page.profile_name.value = "<% getStaProfileData(1); %>";
	}

		document.profile_page.Ssid.value = parent.site_survey_ssid;
		document.profile_page.network_type.value = parent.site_survey_network_type;
		document.profile_page.power_saving_mode['0'].checked = true;
		document.profile_page.b_premable_type.value = "0";

		document.profile_page.rts_thresholdvalue.value = "2346";
		document.profile_page.fragment_thresholdvalue.value = "2346";

		var encryp = parent.site_survey_cipher;
		if(parent.site_survey_network_type == 1 )
		var auth = parent.site_survey_security_infra_mode;
		else
		var auth = parent.site_survey_security_adhoc_mode;

		document.profile_page.wep_key_1.value = "";
		document.profile_page.wep_key_2.value = "";
		document.profile_page.wep_key_3.value = "";
		document.profile_page.wep_key_4.value = "";
		document.profile_page.wep_key_entry_method.value = 0;
		document.profile_page.wep_key_entry_method.value = 0;
		document.profile_page.wep_key_entry_method.value = 0;
		document.profile_page.wep_key_entry_method.value = 0;
		document.profile_page.wep_key_length.value = 0;
		document.profile_page.wep_key_length.value = 0;
		document.profile_page.wep_key_length.value = 0;
		document.profile_page.wep_key_length.value = 0;

		var keydefaultid = 1;

		document.profile_page.wep_default_key[0].checked= false;
		document.profile_page.wep_default_key[1].checked= false;
		document.profile_page.wep_default_key[2].checked= false;
		document.profile_page.wep_default_key[3].checked= false;

		document.profile_page.wep_default_key[keydefaultid-1].checked= true;

		document.profile_page.passphrase.value = "";
        
		show_div(false, "div_power_saving_mode_id");
		document.profile_page.power_saving_mode.disabled = true;
        
		show_div(false, "div_channel_id");
		document.profile_page.channel.disabled = true;

		show_div(false, "div_b_premable_type_id");
		document.profile_page.b_premable_type.disabled = true;

		var keymgmt = 0;
		var eap = 0;

		if (keymgmt == 1) //Rtwpa_supplicantKeyMgmtWPAEAP,
			document.profile_page.cert_auth_type_from_wpa.value = eap;
		else if (keymgmt == 2) //Rtwpa_supplicantKeyMgmtIEEE8021X,
			document.profile_page.cert_auth_type_from_1x.value = eap;

		document.profile_page.cert_id.value = "";
		var cacert = "";
		document.profile_page.cert_ca_cert_path.value = cacert;
		var clientcert = "";
		document.profile_page.cert_client_cert_path.value = clientcert;
		document.profile_page.cert_private_key_path.value = "";
		document.profile_page.cert_private_key_password.value = "";
		document.profile_page.cert_password.value = "";
		var tunnel = 0;

		if (eap == 5 ) // Rtwpa_supplicantEAPPEAP,
			document.profile_page.cert_tunnel_auth_peap.value = tunnel;
		else if (eap == 6 ) // Rtwpa_supplicantEAPTTLS,
			document.profile_page.cert_tunnel_auth_ttls.value  = tunnel;

		if (cacert.length > 1) // != '0'
			document.profile_page.cert_use_ca_cert.checked = true;
		if (clientcert.length > 1) // != '0'
			document.profile_page.cert_use_client_cert.checked = true;

	networkTypeChangeInit(auth, encryp);
}

	show_div(false, "div_power_saving_mode_id");	
	show_div(false, "div_addprofNetType_id");
	show_div(false, "div_rts_frag_id");

	parent.adjustMyFrameHeight();	
  
	if ((wirelessmode != 2) && (wirelessmode != 6)){
		alert("You MUST select System Operation Mode to WISP or Client");	
	}

}

function getChannel()
{
	var channel = "<% getStaAdhocChannel(); %>";
	var wireless_mode = "<% getCfgZero(1, "WirelessMode"); %>";

	var bg_channel = channel & 0xFF;
	var a_channel = (channel >> 8) & 0xFF;

	switch (wireless_mode)
	{
		case 0: // B/G mixed
		case 1: // B only
			getBGChannel(bg_channel);
			break;
		case 2: // A only
			getAChannel(a_channel);
			break;
		case 3: // A/B/G mixed
		default:
			getBGChannel(bg_channel);
			getAChannel(a_channel);
			break;
	}
}

function getBGChannel( channel )
{
	switch (channel)
	{
		case 0:
			document.write(ch1+ch2+ch3+ch4+ch5+ch6+ch7+ch8+ch9+ch10+ch11);
			break;
		case 1:
			document.write(ch1+ch2+ch3+ch4+ch5+ch6+ch7+ch8+ch9+ch10+ch11+ch12+ch13);
			break;
		case 2:
			document.write(ch10+ch11);
			break;
		case 3:
			document.write(ch10+ch11+ch12+ch13);
			break;
		case 4:
			document.write(ch14);
			break;
		case 5:
			document.write(ch1+ch2+ch3+ch4+ch5+ch6+ch7+ch8+ch9+ch10+ch11+ch12+ch13+ch14);
			break;
		case 6:
			document.write(ch3+ch4+ch5+ch6+ch7+ch8+ch9);
			break;
		case 7:
			document.write(ch5+ch6+ch7+ch8+ch9+ch10+ch11+ch12+ch13);
			break;
		default:
			break;
	}
}

function getAChannel( channel )
{
	switch (channel)
	{
		case 0:
			document.write(ch36+ch40+ch44+ch48+ch52+ch56+ch60+ch64+ch149+ch153+ch157+ch161+ch165);
			break;
		case 1:
			document.write(ch36+ch40+ch44+ch48+ch52+ch56+ch60+ch64+ch100+ch104+ch108+ch112+ch116+ch120+ch124+ch128+ch132+ch136+ch140);
			break;
		case 2:
			document.write(ch36+ch40+ch44+ch48+ch52+ch56+ch60+ch64);
			break;
		case 3:
			document.write(ch52+ch56+ch60+ch64+ch149+ch153+ch157+ch161);
			break;
		case 4:
			document.write(ch149+ch153+ch157+ch161+ch165);
			break;
		case 5:
			document.write(ch149+ch153+ch157+ch161);
			break;
		case 6:
			document.write(ch36+ch40+ch44+ch48);
			break;
		case 7:
			document.write(ch36+ch40+ch44+ch48+ch52+ch56+ch60+ch64+ch100+ch104+ch108+ch112+ch116+ch120+ch124+ch128+ch132+ch136+ch140+ch149+ch153+ch157+ch161+ch165);
			break;
		case 8:
			document.write(ch52+ch56+ch60+ch64);
			break;
		case 9:
			document.write(ch34+ch38+ch42+ch46);
			break;
		case 10:
			document.write(ch34+ch36+ch38+ch40+ch42+ch44+ch46+ch48+ch52+ch56+ch60+ch64);
			break;
		default:
			break;
	}
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

function networkTypeChange(auth, encryp)
{
	var nmode;
	
	show_div(false, "div_b_premable_type_id");
	document.profile_page.b_premable_type.disabled = true;

	show_div(false, "div_power_saving_mode_id");
	document.profile_page.power_saving_mode.disabled = true;

	show_div(false, "div_channel_id");
	document.profile_page.channel.disabled = true;

	show_div(false, "div_security_infra_mode_id");
	document.profile_page.security_infra_mode.disabled = true;

	show_div(false, "div_security_adhoc_mode_id");
	document.profile_page.security_adhoc_mode.disabled = true;
	
	nmode = 1*document.profile_page.network_type.options.selectedIndex;
	if (nmode==1)
	{
		show_div(true, "div_power_saving_mode_id");	
		document.profile_page.power_saving_mode.disabled = false;

		show_div(true, "div_security_infra_mode_id");
		document.profile_page.security_infra_mode.disabled = false;

		if (!auth )
		{
			auth = document.profile_page.security_infra_mode.value;
		}

		document.profile_page.security_infra_mode.value = auth;

		if (auth>=3)
		{
			if (encryp == 4) <!--tkip -->
				document.profile_page.cipher[0].checked = true;
			else if (encryp == 6)
				document.profile_page.cipher[1].checked = true;
		}
	}
	else 
	{
		show_div(true, "div_b_premable_type_id");	
		document.profile_page.b_premable_type.disabled = false;

		show_div(true, "div_channel_id");		
		document.profile_page.channel.disabled = false;

		show_div(true, "div_security_adhoc_mode_id");
		document.profile_page.security_adhoc_mode.disabled = false;

		if (!auth )
		{
			auth = document.profile_page.security_adhoc_mode.value;
		}
		document.profile_page.security_adhoc_mode.value = auth;
	}
	securityMode();
}


function networkTypeChangeInit(auth, encryp)
{

	var nmode;
	
	show_div(false, "div_b_premable_type_id");
	document.profile_page.b_premable_type.disabled = true;

	show_div(false, "div_power_saving_mode_id");
	document.profile_page.power_saving_mode.disabled = true;

	show_div(false, "div_channel_id");
	document.profile_page.channel.disabled = true;

	show_div(false, "div_security_infra_mode_id");
	document.profile_page.security_infra_mode.disabled = true;

	show_div(false, "div_security_adhoc_mode_id");
	document.profile_page.security_adhoc_mode.disabled = true;

	nmode = 1*document.profile_page.network_type.options.selectedIndex;

	if (nmode==1)
	{
		show_div(true, "div_power_saving_mode_id");	
		document.profile_page.power_saving_mode.disabled = false;

		show_div(true, "div_security_infra_mode_id");
		document.profile_page.security_infra_mode.disabled = false;

		if (!auth )
		{
			auth = document.profile_page.security_infra_mode.value;
		}

		document.profile_page.security_infra_mode.value = auth;

		if (auth>=3)
		{
			if (encryp == 4) <!--tkip -->
				document.profile_page.cipher[0].checked = true;
			else if (encryp == 6)
				document.profile_page.cipher[1].checked = true;
		}
	}
	else 
	{
		show_div(true, "div_b_premable_type_id");	
		document.profile_page.b_premable_type.disabled = false;

		show_div(true, "div_channel_id");		
		document.profile_page.channel.disabled = false;

		show_div(true, "div_security_adhoc_mode_id");
		document.profile_page.security_adhoc_mode.disabled = false;

		if (!auth )
		{
			auth = document.profile_page.security_adhoc_mode.value;
		}
		document.profile_page.security_adhoc_mode.value = auth;
	}

	securityModeInit();
}

function checkData()
{
	var securitymode;
	var profilename = document.profile_page.profile_name.value;
	var ssid = document.profile_page.Ssid.value;
	
	if (document.profile_page.network_type.value == 1) //infra
		securitymode = document.profile_page.security_infra_mode.value;
	else
		securitymode = document.profile_page.security_adhoc_mode.value;
	
	if (profilename.length <=0)
	{
		alert('Pleaes input the Profile Name!');
		return false;
	}
	else if (ssid.length <= 0)
	{
		alert('Pleaes input the SSID!');
		return false;
	}
	else if (securitymode  == 0 || securitymode  == 1)
	{
		return check_Wep(securitymode);
	}
	else if (securitymode  == 4 ||securitymode == 7 || securitymode == 5)
	{
		var keyvalue = document.profile_page.passphrase.value;

		if (keyvalue.length == 0)
		{
			alert('Please input wpapsk key!');
			return false;
		}

		if (keyvalue.length < 8)
		{
			alert('Please input at least 8 character of wpapsk key !');
			return false;
		}
	}
	//802.1x
	else if (securitymode == 3 || securitymode == 6 || securitymode == 8) //wpa enterprise, 802.1x
	{
		var certid = document.profile_page.cert_id.value;
		if (certid.length == 0)
		{
			alert('Please input the 802.1x identity !');
			return false;
		}

		if (document.profile_page.cert_password.disabled == false)
		{
			var certpassword = document.profile_page.cert_password.value;
			if (certpassword.length == 0)
			{
				alert('Please input the 802.1x password !');
				return false;
			}
		}

		if (document.profile_page.cert_use_client_cert.checked == true)
		{
			//alert("client cert check");
			var client_cert = document.profile_page.cert_client_cert_path.value;
			var private_key = document.profile_page.cert_private_key_path.value;
			var private_key_password = document.profile_page.cert_private_key_password.value;

			if (client_cert.length == 0)
			{
				alert('Please input the 802.1x Client Certificate Path !');
				return false;
			}

			if (private_key.length == 0)
			{
				alert('Please input the 802.1x Private Key Path !');
				return false;
			}

			if (private_key_password.length == 0)
			{
				alert('Please input the 802.1x Private Key Password !');
				return false;
			}
			
		}

		if (document.profile_page.cert_use_ca_cert.checked == true)
		{
			//alert("CA cert check");
			var ca_cert_path = document.profile_page.cert_ca_cert_path.value;

			if (ca_cert_path.length == 0)
			{
				alert('Please input the 802.1x CA Certificate Path !');
				return false;
			}
		}

		if (document.profile_page.cert_auth_type_from_1x.value == 0) //md5
			return check_Wep(securitymode);

	}
	else
	{
		var tmp = "<% getStaAllProfileName(); %>";
		if (tmp.indexOf(profilename) >= 0 && (tmp.length == profilename.length)) {
			alert('Duplicate Profile Name!');
			return false;
		}
	}

	// Check RTS	
	if (isNaN(document.profile_page.rts_thresholdvalue.value) || document.profile_page.rts_thresholdvalue.value < 256 || document.profile_page.rts_thresholdvalue.value > 2346)
	{
		alert('Invalid RTS Threshold');
		document.profile_page.rts_thresholdvalue.focus();
		document.profile_page.rts_thresholdvalue.select();
		return false;
	}
	
	// Check Fragment
	if (isNaN(document.profile_page.fragment_thresholdvalue.value) || document.profile_page.fragment_thresholdvalue.value < 256 || document.profile_page.fragment_thresholdvalue.value > 2346)
	{
		alert('Please specify Fragmentation Length');
		document.profile_page.fragment_thresholdvalue.focus();
		document.profile_page.fragment_thresholdvalue.select();
		return false;
	}
	
	return true;
}

function wep_switch_key_length()
{
    document.profile_page.wep_key_1.value = "";
    document.profile_page.wep_key_2.value = "";
    document.profile_page.wep_key_3.value = "";
    document.profile_page.wep_key_4.value = "";

    if (document.profile_page.wep_key_length.options.selectedIndex == 0) {
	if (document.profile_page.wep_key_entry_method[1].checked == true) { //point to Hex
          document.profile_page.wep_key_1.maxLength = 10;
          document.profile_page.wep_key_2.maxLength = 10;
          document.profile_page.wep_key_3.maxLength = 10;
          document.profile_page.wep_key_4.maxLength = 10;
       }
       else { //point to ASCII
          document.profile_page.wep_key_1.maxLength = 5;
          document.profile_page.wep_key_2.maxLength = 5;
          document.profile_page.wep_key_3.maxLength = 5;
          document.profile_page.wep_key_4.maxLength = 5;
       }
    }
    else {
       	if (document.profile_page.wep_key_entry_method[1].checked == true) { //point to Hex
          document.profile_page.wep_key_1.maxLength = 26;
          document.profile_page.wep_key_2.maxLength = 26;
          document.profile_page.wep_key_3.maxLength = 26;
          document.profile_page.wep_key_4.maxLength = 26;
       }
       else { //point to ASCII
          document.profile_page.wep_key_1.maxLength = 13;
          document.profile_page.wep_key_2.maxLength = 13;
          document.profile_page.wep_key_3.maxLength = 13;
          document.profile_page.wep_key_4.maxLength = 13;
       }
    }
}

function securityMode()
{
	var security_mode;


	show_div(false, "div_security_infra_mode_id");
	document.profile_page.security_infra_mode.disabled = true;

	show_div(false, "div_security_adhoc_mode_id");
	document.profile_page.security_adhoc_mode.disabled = true;

	hideWep();

	show_div(false, "div_wpa_algorithms_id");	
	show_div(false, "div_wpa_passphrase_id");
	
	document.profile_page.cipher[0].disabled = true;
	document.profile_page.cipher[1].disabled = true;
	document.profile_page.passphrase.disabled = true;

	show_div(false, "div_8021x_id");
	show_div(false, "div_8021x_cert_from_wpa_id");
	show_div(false, "div_8021x_cert_from_1x_id");

	document.profile_page.cert_auth_type_from_wpa.disabled = true;
	document.profile_page.cert_auth_type_from_1x.disabled = true;

	document.profile_page.cert_tunnel_auth_peap.disabled = true;
	document.profile_page.cert_tunnel_auth_ttls.disabled = true;
	document.profile_page.cert_id.disabled = true;
	document.profile_page.cert_password.disabled = true;
	document.profile_page.cert_client_cert_path.disabled = true;
	document.profile_page.cert_private_key_path.disabled = true;
	document.profile_page.cert_private_key_password.disabled = true;
	document.profile_page.cert_ca_cert_path.disabled = true;
	
	if (document.profile_page.network_type.value == 1) //infra
	{
		security_mode = document.profile_page.security_infra_mode.value;
		show_div(true, "div_security_infra_mode_id");		
		document.profile_page.security_infra_mode.disabled = false;
	}
	else
	{
		security_mode = document.profile_page.security_adhoc_mode.value;
		show_div(true, "div_security_adhoc_mode_id");			
		document.profile_page.security_adhoc_mode.disabled = false;
	}

	if (security_mode == 0){
		hideWep();
	}
	else if (security_mode == 1)
	{
		showWep();
	}
	else if (security_mode == 4 || security_mode == 7 || security_mode == 5)
	{
		show_div(true, "div_wpa_algorithms_id");
		document.profile_page.cipher[0].disabled = false;
		document.profile_page.cipher[1].disabled = false;;

		show_div(true, "div_wpa_passphrase_id");
		document.profile_page.passphrase.disabled = false;
	}
	else if (security_mode == 3 || security_mode == 6 || security_mode == 8) //wpa enterprise, 802.1x
	{

		if (security_mode != 8)
		{
			show_div(true, "div_wpa_algorithms_id");			
			document.profile_page.cipher[0].disabled = false;
			document.profile_page.cipher[1].disabled = false;;
		}

		show_div(true, "div_8021x_id");

		if( security_mode != 8) //802.1x
		{
			show_div(true, "div_8021x_cert_from_wpa_id");		
			document.profile_page.cert_auth_type_from_wpa.disabled = false;
		}
		else
		{
			show_div(true, "div_8021x_cert_from_1x_id");
			document.profile_page.cert_auth_type_from_1x.disabled = false;
		}
		document.profile_page.cert_tunnel_auth_peap.disabled = false;
		document.profile_page.cert_tunnel_auth_ttls.disabled = false;
		document.profile_page.cert_id.disabled = false;
		document.profile_page.cert_password.disabled = false;

		use_ca_cert();
		use_client_cert();
		certAuthModeChange();
	}

	parent.adjustMyFrameHeight();
}

//Tommy
function securityModeInit()
{
	var security_mode;

	show_div(false, "div_security_infra_mode_id");
	document.profile_page.security_infra_mode.disabled = true;

	show_div(false, "div_security_adhoc_mode_id");
	document.profile_page.security_adhoc_mode.disabled = true;

	hideWep();

	show_div(false, "div_wpa_algorithms_id");	
	show_div(false, "div_wpa_passphrase_id");
	
	document.profile_page.cipher[0].disabled = true;
	document.profile_page.cipher[1].disabled = true;
	document.profile_page.passphrase.disabled = true;

	// 802.1x
	show_div(false, "div_8021x_id");
	show_div(false, "div_8021x_cert_from_wpa_id");	

	show_div(false, "div_8021x_cert_from_1x_id");
	
	document.profile_page.cert_auth_type_from_wpa.disabled = true;
	document.profile_page.cert_auth_type_from_1x.disabled = true;

	document.profile_page.cert_tunnel_auth_peap.disabled = true;
	document.profile_page.cert_tunnel_auth_ttls.disabled = true;
	document.profile_page.cert_id.disabled = true;
	document.profile_page.cert_password.disabled = true;
	document.profile_page.cert_client_cert_path.disabled = true;
	document.profile_page.cert_private_key_path.disabled = true;
	document.profile_page.cert_private_key_password.disabled = true;
	document.profile_page.cert_ca_cert_path.disabled = true;
	
	if (document.profile_page.network_type.value == 1) //infra
	{
		security_mode = document.profile_page.security_infra_mode.value;
		show_div(true, "div_security_infra_mode_id");		
		document.profile_page.security_infra_mode.disabled = false;
	}
	else
	{
		security_mode = document.profile_page.security_adhoc_mode.value;
		show_div(true, "div_security_adhoc_mode_id");			
		document.profile_page.security_adhoc_mode.disabled = false;
	}

	if ( (security_mode == 0)&&(document.profile_page.wep_key_1.value == "")&&(document.profile_page.wep_key_2.value == "") && 
	     (document.profile_page.wep_key_3.value == "")&&(document.profile_page.wep_key_4.value == "") )
	{
		hideWep();
	}
	else if (security_mode == 0 || security_mode == 1)
	{
		showWep();
	}
	else if (security_mode == 4 || security_mode == 7 || security_mode == 5)
	{
		show_div(true, "div_wpa_algorithms_id");
		document.profile_page.cipher[0].disabled = false;
		document.profile_page.cipher[1].disabled = false;;

		show_div(true, "div_wpa_passphrase_id");
		document.profile_page.passphrase.disabled = false;
	}
	else if (security_mode == 3 || security_mode == 6 || security_mode == 8) //wpa enterprise, 802.1x
	{

		if (security_mode != 8)
		{
			show_div(true, "div_wpa_algorithms_id");			
			document.profile_page.cipher[0].disabled = false;
			document.profile_page.cipher[1].disabled = false;;
		}

		show_div(true, "div_8021x_id");
		if( security_mode != 8) //802.1x
		{
			show_div(true, "div_8021x_cert_from_wpa_id");		
			document.profile_page.cert_auth_type_from_wpa.disabled = false;
		}
		else
		{
			show_div(true, "div_8021x_cert_from_1x_id");
			document.profile_page.cert_auth_type_from_1x.disabled = false;
		}
		document.profile_page.cert_tunnel_auth_peap.disabled = false;
		document.profile_page.cert_tunnel_auth_ttls.disabled = false;
		document.profile_page.cert_id.disabled = false;
		document.profile_page.cert_password.disabled = false;

		use_ca_cert();
		use_client_cert();
		certAuthModeChange();
	}
	parent.adjustMyFrameHeight();
}

function use_client_cert()
{
	if (document.profile_page.cert_use_client_cert.checked)
	{
		show_div(true, "div_client_cert_path_id");
		show_div(true, "div_private_key_path_id");
		show_div(true, "div_private_key_password_id");

		document.profile_page.cert_private_key_path.disabled = false;
		document.profile_page.cert_private_key_password.disabled = false;
		document.profile_page.cert_client_cert_path.disabled = false;
	}
	else
	{
		show_div(false, "div_client_cert_path_id");
		show_div(false, "div_private_key_path_id");
		show_div(false, "div_private_key_password_id");

		document.profile_page.cert_private_key_path.disabled = true;
		document.profile_page.cert_private_key_password.disabled = true;
		document.profile_page.cert_client_cert_path.disabled = true;
	}
}

function use_ca_cert()
{
	if (document.profile_page.cert_use_ca_cert.checked)
	{
		show_div(true, "div_ca_cert_path_id");	
		document.profile_page.cert_ca_cert_path.disabled = false;
	}
	else
	{
		show_div(false, "div_ca_cert_path_id");	
		document.profile_page.cert_ca_cert_path.disabled = true;
	}
}

function certAuthModeChange()
{
	var auth_mode;
	var security_infra_mode = document.profile_page.security_infra_mode.value;

	if (security_infra_mode == 3 || security_infra_mode == 6) //wpa-enterprise
		auth_mode = document.profile_page.cert_auth_type_from_wpa.value;
	else if (security_infra_mode == 8) // 802.1x
		auth_mode = document.profile_page.cert_auth_type_from_1x.value;

	hideWep();

	show_div(false, "div_tunnel_auth_peap_id");	
	document.profile_page.cert_tunnel_auth_peap.disabled = true;

	show_div(false, "div_tunnel_auth_ttls_id");
	document.profile_page.cert_tunnel_auth_ttls.disabled = true;

	show_div(false, "div_password_id");
	document.profile_page.cert_password.disabled = true;
	
	document.profile_page.cert_id.disabled = true;

	document.profile_page.cert_use_client_cert.checked = false;
	
	show_div(false, "div_use_client_cert_id");
	document.profile_page.cert_use_client_cert.disabled = true;

	document.profile_page.cert_private_key_path.disabled = true;
	document.profile_page.cert_private_key_password.disabled = true;
	document.profile_page.cert_client_cert_path.disabled = true;

	if (auth_mode == 5 || auth_mode == 6) // PEAP & TTLS
	{
		if (auth_mode == 5)
		{
			show_div(true, "div_tunnel_auth_peap_id");		
			document.profile_page.cert_tunnel_auth_peap.disabled = false;
		}
		else 
		{
			show_div(true, "div_tunnel_auth_ttls_id");		
			document.profile_page.cert_tunnel_auth_ttls.disabled = false;
		}
		
		document.profile_page.cert_id.disabled = false;

		show_div(true, "div_password_id");
		document.profile_page.cert_password.disabled = false;

		show_div(true, "div_use_client_cert_id");
		document.profile_page.cert_use_client_cert.disabled = false;
		
	}
	else if( auth_mode == 4) //TLS
	{
		show_div(true, "div_use_client_cert_id");	
		document.profile_page.cert_use_client_cert.disabled = true;

		document.profile_page.cert_use_client_cert.checked = true;

		document.profile_page.cert_id.disabled = false;
	}
	else if ( auth_mode == 0) //MD5
	{
		document.profile_page.cert_id.disabled = false;

		show_div(true, "div_password_id");
		document.profile_page.cert_password.disabled = false;
		
		showWep();
	}
	use_ca_cert();
	use_client_cert();
}
function hideWep()
{

	show_div(false, "div_GeneralWEPPassPhrase_id");	
	show_div(false, "div_wep_key_length_id");
	show_div(false, "div_wep_key_entry_method_id");

	show_div(false, "div_authenication_type_id");
	show_div(false, "div_GeneralNote1_1_id");
	show_div(false, "div_GeneralNote1_2_id");
	show_div(false, "div_GeneralNote1_3_id");
	show_div(false, "div_GeneralNote1_4_id");

	show_div(false, "div_wep_key_1_id");
	show_div(false, "div_wep_key_2_id");
	show_div(false, "div_wep_key_3_id");
	show_div(false, "div_wep_key_4_id");

	document.profile_page.wep_key_length.disabled = true;
	document.profile_page.wep_key_entry_method.disabled = true;
	document.profile_page.wep_key_1.disabled = true;
	document.profile_page.wep_key_2.disabled = true;
	document.profile_page.wep_key_3.disabled = true;
	document.profile_page.wep_key_4.disabled = true;
	document.profile_page.wep_default_key.disabled = true;
}
function showWep()
{
	<!-- WEP -->
	show_div(true, "div_GeneralWEPPassPhrase_id");	
	show_div(true, "div_wep_key_length_id");
	show_div(true, "div_wep_key_entry_method_id");

	show_div(true, "div_authenication_type_id");
	show_div(true, "div_GeneralNote1_1_id");
	show_div(true, "div_GeneralNote1_2_id");
	show_div(true, "div_GeneralNote1_3_id");
	show_div(true, "div_GeneralNote1_4_id");					

	show_div(true, "div_wep_key_1_id");
	show_div(true, "div_wep_key_2_id");
	show_div(true, "div_wep_key_3_id");
	show_div(true, "div_wep_key_4_id");

	document.profile_page.wep_key_length.disabled = false;

	if (<% getStaProfileData(16); %> == 0){
		document.profile_page.wep_key_entry_method[1].checked = true;
	}else{
		document.profile_page.wep_key_entry_method[0].checked = true;
	}
	
	document.profile_page.wep_key_1.disabled = false;
	document.profile_page.wep_key_2.disabled = false;
	document.profile_page.wep_key_3.disabled = false;
	document.profile_page.wep_key_4.disabled = false;
	document.profile_page.wep_default_key.disabled = false;

	if (<% getStaProfileData(11); %> == 0){	
		document.profile_page.infra_auth_type.options.selectedIndex = 0;
		document.profile_page.security_infra_mode.options.selectedIndex = 1;
	}
	else{
		document.profile_page.infra_auth_type.options.selectedIndex = 1;
	}

	if (document.profile_page.wep_key_length.options.selectedIndex == 0) {
		if (document.profile_page.wep_key_entry_method[1].checked == true) { //point to Hex
		  document.profile_page.wep_key_1.maxLength = 10;
		  document.profile_page.wep_key_2.maxLength = 10;
		  document.profile_page.wep_key_3.maxLength = 10;
		  document.profile_page.wep_key_4.maxLength = 10;
		}
		else { // ASCII
		  document.profile_page.wep_key_1.maxLength = 5;
		  document.profile_page.wep_key_2.maxLength = 5;
		  document.profile_page.wep_key_3.maxLength = 5;
		  document.profile_page.wep_key_4.maxLength = 5;
		}
	}
	else {
		if (document.profile_page.wep_key_entry_method[1].checked == true) { //point to Hex
		  document.profile_page.wep_key_1.maxLength = 26;
		  document.profile_page.wep_key_2.maxLength = 26;
		  document.profile_page.wep_key_3.maxLength = 26;
		  document.profile_page.wep_key_4.maxLength = 26;
		}
		else { // ASCII
		  document.profile_page.wep_key_1.maxLength = 13;
		  document.profile_page.wep_key_2.maxLength = 13;
		  document.profile_page.wep_key_3.maxLength = 13;
		  document.profile_page.wep_key_4.maxLength = 13;
		}
	}
}

function parse40WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.profile_page.wep_key_1.value = fields_str[0];
	document.profile_page.wep_key_2.value = fields_str[1];
	document.profile_page.wep_key_3.value = fields_str[2];
	document.profile_page.wep_key_4.value = fields_str[3];
}

function parse128WEPkey(str)
{
	var fields_str = new Array();
	fields_str = str.split("\r");

	document.profile_page.wep_key_1.value = fields_str[0];
	document.profile_page.wep_key_2.value = fields_str[0];
	document.profile_page.wep_key_3.value = fields_str[0];
	document.profile_page.wep_key_4.value = fields_str[0];
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
	passphrase = document.profile_page.wep_passphrase.value;

	document.profile_page.wep_key_entry_method[1].checked = true; //Hex	
	if (document.getElementById("wep_key_length").selectedIndex == 1){ // get 128 bits WEP KEY
		makeRequest("/goform/wifiget128wepkey", passphrase, get128wepeyHandler);
	}else{ // get 40 bits WEP KEY
		makeRequest("/goform/wifiget40wepkey", passphrase, get40wepeyHandler);
	}
}

function check_Wep(securitymode)
{
	var defaultid = 0;	
	var keylen = 0 ;

	if (document.profile_page.wep_default_key[0].checked == true){
		defaultid = 1;
		var keyvalue = document.profile_page.wep_key_1.value;
	}else if (document.profile_page.wep_default_key[1].checked == true){
		defaultid = 2;
		var keyvalue = document.profile_page.wep_key_2.value;
	}else if (document.profile_page.wep_default_key[2].checked == true){
		defaultid = 3;
		var keyvalue = document.profile_page.wep_key_3.value;
	}else if (document.profile_page.wep_default_key[3].checked == true){
		defaultid = 4;
		var keyvalue = document.profile_page.wep_key_4.value;
	}		

	if (document.profile_page.wep_key_length.options.selectedIndex == 0) {
		if (document.profile_page.wep_key_entry_method[1].checked == true) { //point to Hex
			keylen = 10;
		}else{
			keylen = 5;
		}
	}else{
		if (document.profile_page.wep_key_entry_method[1].checked == true) { //point to Hex
			keylen = 26;
		}else{
			keylen = 13;
		}
	}

	if (keyvalue.length == 0 && ( securitymode == 1 || document.profile_page.cert_auth_type_from_1x.value == 3)) // shared wep  || md5
	{
		alert(_('Please input wep key')+defaultid+' !');
		return false;
	}

	if (keyvalue.length != 0)
	{
		if (keyvalue.length != keylen)
		{
			alert(_('Please input')+keylen+_('character of wep key !'));
			return false;
		}
	}
	return true;
}




function submit_apply()
{
	if ((wirelessmode != 2) && (wirelessmode != 6)){
		alert("You MUST select System Operation Mode to WISP or Client");	
		return false;
	}

	if (checkData() == true){
		if ((document.profile_page.security_infra_mode.value == 1) && (document.profile_page.infra_auth_type.value == 0)){
			document.profile_page.security_infra_mode.value = 0;		
		}
		showWebMessage(2, "");			
		document.profile_page.submit();
	}
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="profile_page" action="/goform/addProfile">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="addprofSysConf">Wireless Setup</td>
                   </tr></table>
</li>


<span id="div_addprofProfName_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" class="head" id="addprofProfName">Profile Name</td>
<td><input type=text name="profile_name" maxlength=32></td>
</tr>
</table>
</li>
</span>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" class="head" id="addprofSSID">Network Name(SSID)</td>
<td><input type=text name="Ssid" maxlength=32></td>
</tr>
</table>
</li>

<span id="div_addprofNetType_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" class="head" id="addprofNetType">Network Type</td>
<td>
<select name="network_type" size="1" onChange="networkTypeChange()">
<option value=0 id="addprofAdHoc">802.11 Ad Hoc</option>
<option value=1 id="addprofInfra" selected>Infrastructure</option>
</select>
</td>
</tr>
</table>
</li>  
</span>

<span id="div_power_saving_mode_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="div_power_saving_mode" name="div_power_saving_mode">
<td width="40%" class="head" id="addprofPWSave">Power Saving Mode</td>
<td>
<input type=radio name="power_saving_mode" value="0" checked><font id="addprofCAM">CAM (Constantly Awake Mode)</font>
<input type=radio name="power_saving_mode" value="1"><font id="addprofPWSaveMode">Power Saving Mode</font>
</td>
</tr>
</table>
</li>
</span>   

<span id="div_channel_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="div_channel" name="div_channel">
<td width="40%" class="head" id="addprofChannel">Channel</td>
<td>
<select name="channel" size="1">
<script>getChannel();</script>
</select>
</td>
</tr>
</table>
</li>  
</span>   

<span id="div_b_premable_type_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="div_b_premable_type" name="div_b_premable_type">
<td width="40%" class="head" id="addprofPreambleType">11B Premable Type</td>
<td>
<select name="b_premable_type" size="1">
<option value=0 id="addprofPremableAuto" selected>Auto</option>
<option value=1 id="addprofPremableLong">Long</option>
</select>
</td>
</tr>
</table>
</li>  
</span>

<span id="div_rts_frag_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" class="head" id="addprofRTS"> RTS Threshold </td>
<td>
<input type=text name=rts_thresholdvalue value=2346>
(256 ~ 2346)
</td>
</tr>
</table>
</li>  

 <li class="w_text">
 <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" class="head" id="addprofFrag"> Fragement Threshold </td>
<td>
<input type=text name=fragment_thresholdvalue value=2346>
(256 ~ 2346)
</td>
</tr>
</table>
</li>  
</span>

<li class="title" id="addprofSecurePolicy">Security</li> 
<span id="div_security_infra_mode_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="div_security_infra_mode" name="div_security_infra_mode"> 
<td width="40%" class="head" id="addprofSecureMode">Security Mode</td>
<td>
<select name="security_infra_mode" id="security_infra_mode" size="1" onChange="securityMode()">
<option value=0 selected id="GeneralNoSecurity">No Security</option>
<option value=1 id="GeneralSharedKey">SHARED</option>
<option value=4 id="GeneralWPAPSK">WPA-PSK</option>
<option value=7 id="GeneralWPA2PSK">WPA2-PSK</option>
</select>
</td>
</tr>
</table>
</li>  
</span>

<span id="div_GeneralWEPPassPhrase_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="GeneralWEPPassPhrase">PassPhrase </td>
<td><input name="wep_passphrase" id="wep_passphrase" maxlength="26" value="" >
<input type=button value="Generate" id="GeneralWEPGenerate" onclick="generate_wep()" ></td>		      
</tr>
</table>
</li>
</span>

<span id="div_security_adhoc_mode_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="div_security_adhoc_mode" name="div_security_adhoc_mode"> 
<td width="40%" class="head"i id="addprofAdHocSecure">Security Mode</td>
<td>
<select name="security_adhoc_mode" id="security_adhoc_mode" size="1" onChange="securityMode()">
<option value=0 id="addprofAdHocOpen" selected>OPEN</option>
<option value=1 id="addprofAdHocShared">SHARED</option>
<option value=5 id="addprofAdHocWPA-None">WPA-NONE</option>
</select>
</td>
</tr>
</table>
</li>  
</span>

<span id="div_wep_key_length_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="40%" class="head" colspan="2" id="addprofWEPKeyLength">WEP Encryption</td>
<td>
<select name="wep_key_length" size="1" onChange="wep_switch_key_length()">
<option value=0 selected id="addprofWEP64bit">64-bits</option>
<option value=1 id="addprofWEP128bit">128-bits</option>
</select>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_authenication_type_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="40%" class="head" colspan="2" id="GeneralAuthMethod">Authentication Method</td>
<td>
<select name="infra_auth_type" size="1"">
<option value=0 selected id="GeneralEncrypOpen">Open</option>
<option value=1 id="GeneralEncrypSharedKey">Shared Key</option>
</select>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_GeneralNote1_1_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id="GeneralNote1_1">Note:</span></td>
</tr>
</table>
</li>
</span>  

<span id="div_GeneralNote1_2_id" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_2">64-bit WEP: Enter 5 ASCII characters or 10 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>
</span>  

<span id="div_GeneralNote1_3_id" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_3">128-bit WEP: Enter 13 ASCII characters or 26 hexadecimal characters ("0-9", "A-F") for each Key (1-4).</span></td>
</tr>
</table>
</li>
</span>
 
<span id="div_GeneralNote1_4_id" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note_a" id="GeneralNote1_4">(Select one WEP key as an active key to encrypt wireless data transmission.)</span></td>
</tr>
</table>
</li>                
</span>

<span id="div_wep_key_entry_method_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="25%"></td>     
<td>
<input type="radio" name="wep_key_entry_method" value="1" onclick="wep_switch_key_length();" /><font id ="addprofASCII">ASCII</font> &nbsp;&nbsp;
<input type="radio" name="wep_key_entry_method" value="0" onclick="wep_switch_key_length()" /><font id ="addprofHex">Hex</font>
</td>
</tr>
</table>
</li>
</span>    


<span id="div_wep_keys_id" class="off">
<li class="title" id="addprofWEPKey">WEP Keys</li>
</span>   

<span id="div_wep_key_1_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr>
<td width="10%"></td>                    
<td width="15%"><input type="radio" name="wep_default_key" value=1/><font id ="addprofWEPKey1"> Key 1</font></td>
<td><input name=wep_key_1 size="28" maxlength=26 value=""></td>
</tr>
</table>
</li>    
</span>   


<span id="div_wep_key_2_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="10%"></td>                    
<td width="15%"><input type="radio" name="wep_default_key" value=2/><font id ="addprofWEPKey2"> Key 2</font></td>
<td><input name=wep_key_2 size="28" maxlength=26 value=""></td>  
</tr>
</table>
</li>    
</span>

<span id="div_wep_key_3_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="10%"></td>                    
<td width="15%"><input type="radio" name="wep_default_key" value=3/><font id ="addprofWEPKey3"> Key 3</font></td>
<td><input name=wep_key_3 size="28" maxlength=26 value=""></td>    
</tr>
</table>
</li>    
</span>   


<span id="div_wep_key_4_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="10%"></td>                    
<td width="15%"><input type="radio" name="wep_default_key" value=4/><font id ="addprofWEPKey4"> Key 4</font></td>
<td><input name=wep_key_4 size="28" maxlength=26 value=""></td>
</tr>
</table>
</li>    
</span>   

<span id="div_wep_key128" class="off">
<tr><td>128/104 bit WEP key</td><td><input id==wep_key128 name=wep_key128 size=26></td></tr>
</span>  

<span id="div_wpa_id" class="off">
   <li class="title" id="addprofWPA">WPA</li>
</span>

<span id="div_wpa_algorithms_id" class="off">  
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
</span>    


<span id="div_wpa_passphrase_id" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr id="wpa_passphrase" name="wpa_passphrase">
<td width="40%" class="head" id="addprofPassPhrase">Pre-Shared Key</td>
<td>
<input type=password name=passphrase size=28 maxlength=64 value="">
</td>
</tr>
</table>
</li>    
</span> 

<span id="div_8021x_id" class="off">
<li class="title" id="addprof8021x">802.1x</li> 
</span>


<span id="div_8021x_cert_from_1x_id" class="off">     
<li class="w_text">
 <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr id="div_8021x_cert_from_1x" name="div_8021x_cert_from_1x">
<td width="40%" class="head" id="addprof1XAuthType">Authentication Type</td>
<td>
<select name="cert_auth_type_from_1x" id="cert_auth_type_from_1x" size="1" onChange="certAuthModeChange()">
<option value=5 selected>PEAP</option>
<option value=6>TTLS</option>
<option value=4>TLS</option>
<option value=0>MD5</option>
</select>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_8021x_cert_from_wpa_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr id="div_8021x_cert_from_wpa" name="div_8021x_cert_from_wpa">
<td width="40%" class="head" id="addprofWPAAuthType">Authentication Type</td>
<td>
<select name="cert_auth_type_from_wpa" id="cert_auth_type_from_wpa" size="1" onChange="certAuthModeChange()">
<option value=5 selected>PEAP</option>
<option value=6>TTLS</option>
<option value=4>TLS</option>
</select>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_tunnel_auth_peap_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_tunnel_auth_peap" name="div_tunnel_auth_peap">
<td width="40%" class="head" id="addprofPEAPTunnelAtuth">Tunnel Authentication</td>
<td>
<select name="cert_tunnel_auth_peap" id="cert_tunnel_auth_peap" size="1">
<option value=1 selected>MSCHAP v2</option>
</select>
</td>
</tr>
</table>
</li>    
</span>


<span id="div_tunnel_auth_ttls_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_tunnel_auth_ttls" name="div_tunnel_auth_ttls">
<td width="40%" class="head" id="addprofTTLSTunnelAuth">Tunnel Authentication</td>
<td>
<select name="cert_tunnel_auth_ttls" id="cert_tunnel_auth_ttls" size="1">
<option value=0 selected>MSCHAP</option>
<option value=1>MSCHAP v2</option>
<option value=2>PAP</option>
</select>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_identity_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_identity" name="div_identity">
<td width="40%" class="head" id="addprofIdentity">Identity</td>
<td>
<input type=text name="cert_id" maxlength=32>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_password_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_password" name="div_password">
<td width="40%" class="head" id="addprofPasswd">Password</td>
<td>
<input type=password name="cert_password" maxlength=32>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_use_client_cert_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_use_client_cert" name="div_use_client_cert">
<td width="40%" class="head" id="addprofClientCert">Client Certificate</td>
<td>
<input type=checkbox name="cert_use_client_cert" onClick="use_client_cert()"><font id="addprofClientCertUsed">Used</font>
</td>
</tr>
</table>
</li>    
</span>


<span id="div_client_cert_path_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_client_cert_path" name="div_client_cert_path">
<td width="40%" class="head">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="addprofClientCertPath">Client Certificate Path</font></td>
<td>
<input type=text name="cert_client_cert_path" maxlength=64>
</td>
</tr>
</table>
</li>    
</span>

<span id="div_private_key_path_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_private_key_path" name="div_private_key_path">
<td width="40%" class="head">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="addprofPrivateKeyPath">Private Key Path</font></td>
<td>
<input type=text name="cert_private_key_path" maxlength=64>
</td>
</tr>
</table>
</li>    
</span>


<span id="div_private_key_password_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_private_key_password" name="div_private_key_password">
<td width="40%" class="head">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="addprofPrivateKeyPasswd">Private Key Password</font></td>
<td>
<input type=text name="cert_private_key_password" maxlength=32>
</td>
</tr>
</table>
</li>    
</span>


<span id="div_use_ca_cert_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_use_ca_cert" name="div_use_ca_cert">
<td width="40%" class="head" id="addprofCACert">CA Certificate</td>
<td>
<input type=checkbox name="cert_use_ca_cert" onClick="use_ca_cert()"><font id="addprofCACertUsed">Used</font>
</td>
</tr>
</table>
</li>    
</span>


<span id="div_ca_cert_path_id" class="off"> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0"> 
<tr id="div_ca_cert_path" name="div_ca_cert_path">
<td width="40%" class="head">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<font id="addprofCACertPath">CA Certificate Path</font></td>
<td>
<input type=text name="cert_ca_cert_path" maxlength=64>
</td>
</tr>
</table>
</li>   
</span>

<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=button value="Apply" id="GeneralApply" onClick="submit_apply()">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
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

