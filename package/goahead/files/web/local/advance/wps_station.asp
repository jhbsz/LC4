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

var secs
var timerID = null
var timerRunning = false
var timeout = 3
var delay = 1000

var STATimerFlag = 0

function ValidateChecksum(PIN)
{
	var accum = 0;

	accum += 3 * (parseInt(PIN / 10000000) % 10);
	accum += 1 * (parseInt(PIN / 1000000) % 10);
	accum += 3 * (parseInt(PIN / 100000) % 10);
	accum += 1 * (parseInt(PIN / 10000) % 10);
	accum += 3 * (parseInt(PIN / 1000) % 10);
	accum += 1 * (parseInt(PIN / 100) % 10);
	accum += 3 * (parseInt(PIN / 10) % 10);
	accum += 1 * (parseInt(PIN / 1) % 10);

	return ((accum % 10) == 0);
}

function submit_apply()
{
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	    return false;
	}
	
	wpsenable = <% getWPSModeASP(); %>;

	if (wpsenable ==0){
	    alert("Please enable WPS !!!");
	    return false;	    
	}
	showWebMessage(2, "");	
	return true;
}

function cPINFormCheck()
{
		parent.is_wps_init_page=0;
		if (submit_apply() == false)
		    return false;
	
		if(document.SubmitPIN.pin_number.value != ""){
			// Driver 1.9 supports 4 digit PIN code.
			if( document.SubmitPIN.pin_number.value.length == 4 )
				return true;

			if( ! ValidateChecksum( document.SubmitPIN.pin_number.value)  ){
				alert("PIN number validation failed\n");
				return false;
			}
			return true;
		}
}

function cPBCFormCheck()
{
	parent.is_wps_init_page=0;
	if (submit_apply() == false)
	   return false;
        else
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

function show_wps_message(flag, str)
{
	if (!parent.is_wps_init_page){
		showWebMessage(flag, str);
	}else{
		showWebMessage(0, "Ready");
	}
}

function WPSUpdateHTML(str)
{
	var all_str = new Array();
	all_str = str.split("\n");

	if(all_str[7] == "-1"){
		show_wps_message(1,"Configuraton failed");
	}else if(all_str[7] == "0"){
		if (all_str[6]=="Idle")
		{
			if (parent.is_wps_init_page)
			show_wps_message(0,"Ready");
			else
			show_wps_message(0,"Configuration updated successfully.");
			}
		else
		{	
		parent.is_wps_init_page=0;
			show_wps_message(0,"WPS in Progress. Please Wait . . .");	
		}
	}else if(all_str[7] == "-2"){
		show_wps_message(1,"Configuraton failed");
	}else if(all_str[7] == "1"){
		show_wps_message(0,"Configuration updated successfully.");
	}	
}


function updateWPS(){
	makeRequest("/goform/updateWPS", "something");
}

function initTranslation()
{
	var e = document.getElementById("WPSStaTitle");
	e.innerHTML = _("wps station title");
	
	var e = document.getElementById("WPSStaNote1_0");
	e.innerHTML = _("wps station note1_0");

	var e = document.getElementById("WPSStaNote1_1");
	e.innerHTML = _("wps station note1_1");
	
	var e = document.getElementById("WPSStaNote1_2");
	e.innerHTML = _("wps station note1_2");
	
	var e = document.getElementById("WPSStaPBC");
	e.value = _("wps station pbc");
	
	var e = document.getElementById("WPSStaPinNum");
	e.innerHTML = _("wps station pinnum");	

	var e = document.getElementById("WPSStaPinCode");
	e.value = _("wps station pincode");

	var e = document.getElementById("WPSStaNote2_0");
	e.innerHTML = _("wps station note2_0");	

	var e = document.getElementById("WPSStaNote2_1");
	e.innerHTML = _("wps station note2_1");	

	var e = document.getElementById("WPSStaNote2_2");
	e.innerHTML = _("wps station note2_2");	

  parent.adjustMyFrameHeight();
}

function initValue()
{
	initTranslation();
	
	updateWPS();
	InitializeTimer(3);
	
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	}
}

</script>
</head>
<body onLoad="initValue()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id ="WPSStaTitle">Add Station by WPS</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td nowrap>
<font id="WPSStaNote1_0"> Click the below Push</font>
<font id="WPSStaNote1_1"> Button</font>
<font id="WPSStaNote1_2"> to add WPS stations to wireless network.</font>            
</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%">
<form method="post" name ="SubmitPBC" action="/goform/ExeWPSPBC">
<input name="Push" id="WPSStaPBC" value="Push Button" type="submit" onClick="return cPBCFormCheck();" />
</form>
</td>
<td>&nbsp;</td>
</tr>
</table>
</li>

<li class="w_text">
<form method="post" name ="SubmitPIN" action="/goform/ExeWPSPIN">                
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id ="WPSStaPinNum" nowrap>Or input station's PIN number :</td>
<td><input name="pin_number" size="12" maxlength="8" value="" type="text" />
<input name="pin_start" id="WPSStaPinCode" value="Start" type="submit" onClick="return cPINFormCheck();" />&nbsp;
</td>
</tr>
</table>
</form>                                
</li>
<li></li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id="WPSStaNote2_0">Note:</span></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td class="i_note_a" id="WPSStaNote2_1">1. The Push Button Configuration requires pressing a button on both the station and AP within 120 seconds.</td>
</tr>
<tr>
<td class="i_note_a" id="WPSStaNote2_2">2. You may find the PIN number in the station's utility.</td>
</tr>
</table>
</li>

<li></li>
<li></li>
<li></li>
</div>
    <div>
      <ul>
<li class="table_button">    
</li>
</ul>
</div>
</body>
<script>
//showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
