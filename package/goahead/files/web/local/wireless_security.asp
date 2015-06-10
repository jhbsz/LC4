<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>Untitled Document</title>
<script type="text/javascript" src="js/jquery-1.2.6.js"></script>
<script type="text/javascript" src="js/wireless_security.js"></script>
<script type='text/javascript' src='js/jquery.simplemodal.js' ></script>
<script type="text/javascript" src="/lang/b28n.js"></script>
<link type='text/css' href='images/confirm.css' rel='stylesheet' media='screen' />
<link href="images/popup.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
</style>

<script type="text/JavaScript">
Butterlate.setTextDomain("main");
<!--
function MM_jumpMenu(targ,selObj,restore){ //v3.0
  eval(targ+".location='"+selObj.options[selObj.selectedIndex].value+"'");
  if (restore) selObj.selectedIndex=0;
}
//-->

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
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

function CheckValue()
{
	document.security_form.ssid.value = document.security_form.textfield1.value;
	document.security_form.password.value = document.security_form.textfield3.value;
	document.security_form.verify_password.value = document.security_form.textfield4.value;
			
	if (document.security_form.ssid.value == "")
	{
		alert("Please enter SSID!");
	    document.security_form.textfield1.value='<% getCfgGeneral(1, "SSID1"); %>';
		document.security_form.textfield1.focus();
		document.security_form.textfield1.select();
		return false;
	}

	if(checkInjection(document.security_form.ssid.value) == false){
		alert('System do not support specific invalid characters.');
		document.security_form.textfield1.value='<% getCfgGeneral(1, "SSID1"); %>';
		document.security_form.textfield1.focus();
		document.security_form.textfield1.select();
		return false;
	}

	if ((document.security_form.sel_security.options.selectedIndex == 1) ||(document.security_form.sel_security.options.selectedIndex == 2)){
		if (document.security_form.verify_password.value == ""){
		     alert("Please enter Password!");
		     return false;
		}
		if (document.security_form.password.value.length < 8 || document.security_form.password.value.length > 64) {
			alert("Password should be with length 8~64");
			return false;
		}
		
		if(checkInjection(document.security_form.password.value) == false){
			alert('System do not support specific invalid characters.');
			return false;
		}

		if (document.security_form.password.value != document.security_form.verify_password.value){
			alert("Please enter Password correctly!");
			return false;
		}
	}	
	
	
	return true;
}


function initAll()
{
	var AuthMode = '<% getCfgZero(1, "AuthMode"); %>';
	var EncrypType = '<% getCfgZero(1, "EncrypType"); %>';
	var Password = '<% getCfgZero(1, "WPAPSK1"); %>';


	//show_div(false, "div_password_id");
	
	if ( (AuthMode == 'OPEN') && (EncrypType=='NONE') ){
	   // No Security
	   document.security_form.sel_security.options.selectedIndex = 0;
	}else if(AuthMode == 'WPAPSK'){
	   // WPAPSK
	   document.security_form.sel_security.options.selectedIndex = 1;
	   document.security_form.textfield3.value = Password;
	   document.security_form.textfield4.value = Password;	   
	}else if(AuthMode == 'WPA2PSK'){
	   // WPA2PSK
	   document.security_form.sel_security.options.selectedIndex = 2;
	   document.security_form.textfield3.value = Password;
	   document.security_form.textfield4.value = Password;	   
	}else{
	   // No Security
	   document.security_form.sel_security.options.selectedIndex = 0;	
	}
	initTranslation();
	securityMode();
}

function securityMode()
{
	if ((document.security_form.sel_security.options.selectedIndex == 1) ||(document.security_form.sel_security.options.selectedIndex == 2)){
		show_div(true, "div_password_id");		
	}else{
		show_div(false, "div_password_id");	
	}
}	
function initTranslation()
{
	var e = document.getElementById("wstitle");
	e.innerHTML = _("ws title");
		e = document.getElementById("wsSSID");
	e.innerHTML = _("ws SSID");
		e = document.getElementById("wsSecurity");
	e.innerHTML = _("ws Security");
	e = document.getElementById("wsSecurity1");
	e.innerHTML = _("ws Security1");
	e = document.getElementById("wsSecurity2");
	e.innerHTML = _("ws Security2");
	e = document.getElementById("wsSecurity3");
	e.innerHTML = _("ws Security3");
		e = document.getElementById("wspassword");
	e.innerHTML = _("ws password");
			e = document.getElementById("wsvpassword");
	e.innerHTML = _("ws vpassword");
			e = document.getElementById("wsWPS");
	e.innerHTML = _("ws WPS");
	e = document.getElementById("wsCancel");
	e.innerHTML = _("ws Cancel");
	e = document.getElementById("wsApply");
	e.innerHTML = _("ws Apply");
	e = document.getElementById("wsCancelX");
	e.innerHTML = _("ws Cancel");
	e = document.getElementById("wsApplyX");
	e.innerHTML = _("ws Apply");
	e = document.getElementById("wstitle1");
	e.innerHTML = _("ws title1");
	e = document.getElementById("wsws");
	e.innerHTML = _("ws ws");
	e = document.getElementById("wstitle2");
	e.innerHTML = _("ws title2");
	e = document.getElementById("wstitle3");
	e.innerHTML = _("ws title3");
	e = document.getElementById("wstitle4");
	e.innerHTML = _("ws title4");
	e = document.getElementById("wstitle5");
	e.innerHTML = _("ws title5");
	e = document.getElementById("wsConfirm");
	e.innerHTML = _("ws Confirm");
	e = document.getElementById("wsOK");
	e.innerHTML = _("ws OK");
	e = document.getElementById("wsExit");
	e.innerHTML = _("ws Exit");
	e = document.getElementById("wsWPS1");
	e.value = _("ws WPS1");
	e = document.getElementById("Register");
	e.value = _("ws Register");


}
</script>
</head>
<body class="bandwidth_bg" onload="initAll()">
<div class="wrapper2">
<div class=crop2>
<div class="widearea2">
<form method="post" name="security_form" action="/goform/EZwifiAPSecurity">
<div class="position" id="ws">
<div class="p_d"><ul>
  <li id="wstitle">Data transmitted wirelessly without encryption is not safe. Guard your wireless network with a security mode and the password you setup. And then, you can use WPS to connect your computers to your wireless network with just one single click.</li>
</ul></div>
<div class="word_ws">
  <table width="455" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td height="1" colspan="2" background="images/p_line_d.gif"><img src="images/p_line_d.gif" width="1" height="1" /></td>
      </tr>
    <tr>
      <td width="240" height="36" class="ws1" id="wsSSID">Wireless Network Name (SSID) :</td>
      <td width="227"><input name="ssid" type="hidden" value="" /><input name="textfield1" type="text" class="ws" maxlength=32 size="28" value='<% getCfgGeneral(1, "SSID1"); %>' /></td>
    </tr>
    <tr>
      <td height="1" colspan="2" background="images/p_line_d.gif"><img src="images/p_line_d.gif" width="1" height="1" /></td>
      </tr>
    <tr>
      <td height="36" class="ws1" id="wsSecurity">Security mode :</td>
      <td ><label>
            <select name="sel_security" class="ws" onchange="securityMode()">
              <option value=0 id="wsSecurity1">No Security</option>
              <option value=1 id="wsSecurity2">WPA-PSK</option>
              <option value=2 id="wsSecurity3">WPA2-PSK</option>
            </select>
      </label></td>
    </tr>
    <tr>
      <td height="1" colspan="2" background="images/p_line_d.gif"><img src="images/p_line_d.gif" width="1" height="1" /></td>
      </tr>
  </table>
      
<span id="div_password_id" class="off">      
  <table width="455" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="240" height="36" class="ws1" id="wspassword">Wireless password :</td>
      <td width="227"><input name="password" type="hidden" value="" /><input name="textfield3" type="password" class="ws" size="28" maxlength=64 /></td>
    </tr>
    <tr>
      <td height="1" colspan="2" background="images/p_line_d.gif"><img src="images/p_line_d.gif" width="1" height="1" /></td>
      </tr>
    <tr>
      <td height="36" class="ws1" id="wsvpassword">Verify password :</td>
      <td><input name="verify_password" type="hidden" value="" /><input name="textfield4" type="password" class="ws" size="28" maxlength=64 /></td>
    </tr>
    <tr>
      <td height="1" colspan="2" background="images/p_line_d.gif"><img src="images/p_line_d.gif" width="1" height="1" /></td>
      </tr>
  </table>
</span>
    
</div>
<div class="b_wps" id="b_wps"><a id="wsWPS">WPS</a></div>
<span id="button_on" class="on">
<div class="button">
  <ul>
   <li>
    <a href="empty.html" id="wsCancel">Cancel
    </a>
   </li>
  <li>
    <a href="javascript: if (CheckValue()) { show_div(false, 'button_on'); show_div(true, 'button_off'); document.forms[0].submit(); }" id="wsApply">Apply
     </a>
  </li>
</ul></div>
</span>
<span id="button_off" class="off">
<div class="buttonX">
  <ul>
   <li>
    <a id="wsCancelX">Cancel
    </a>
   </li>
  <li>
    <a id="wsApplyX">Apply
     </a>
  </li>
</ul></div>
</span>
 </div>
</form>
 
<form method="post" name="wps_form" action="/goform/EZWPS">
<div class="position2" id="WPS">
<div class="p_d2">
  <ul>
   <li id="wstitle1">WiFi Protected Setup (WPS) provides you a easier and faster way to connect your computers/device to your wireless network. Use one of the following approaches to get connected.</li>   
  </ul></div>
 <div id="b_ws" class="b_ws">
   <a id="wsws">Wireless Security </a>
 </div>
  <div class="or">
    <ul>
     <li class="wps" >
     <a href="#">
<table width="100%" height="260" border="0" cellpadding="0" cellspacing="0">
         <tr> 
      <td width="8">&nbsp;
           </td>
      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
        <tr>
          <td>&nbsp;</td>
         </tr>
         <tr>
          <td><span></span></td>
         </tr>
         <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td id="wstitle2">Click the Wi-Fi Protected Setup button of your wireless client, and then click the button on the bottom.</td>
         </tr>  
         <tr> 
         <td><div id = "btnwps" align="right">
           <input name="WPSMethod" type="hidden" value=1 /> <input name="WPS" type="button" class="wps" id="wsWPS1"  value="WPS" onclick="document.forms[1].WPSMethod.value=1;document.forms[1].submit();"/>
</div>
          </td> 
         </tr>
         <tr> 
          <td>&nbsp;</td> 
         </tr>
           <tr> 
             <td>&nbsp;
             </td> 
           </tr>
      </table></td>
      <td width="8">&nbsp;</td>
    </tr>
  </table></a></li>
<li class="r"><a href="#"><table width="100%" height="260" border="0" cellpadding="0" cellspacing="0">
           <tr>
      <td width="8">&nbsp;</td>
      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td><span></span></td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td><input name="PINCode" type="hidden" value="" /><input name="pin_number" type="text" class="ws" maxlength=8 size="17" /></td>
        </tr>
        <tr>
            <td>
             <div id = "btnregister" align="right">
               <input name="Register" type="button" class="wps" id="Register" value="Register" onclick="document.forms[1].PINCode.value=document.forms[1].pin_number.value;document.forms[1].WPSMethod.value=2;document.forms[1].submit();"/>
             </div></td>
           </tr>  
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td id="wstitle3">Register the PIN number of your wireless client.</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
      </table></td>
      <td width="8">&nbsp;</td>
    </tr>
        </table>
  </a></li>
<li class="pin">
     <a href="#">
<table width="100%" height="260" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="8">&nbsp;</td>
      <td><table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
           <tr>
             <td>&nbsp;</td>
           </tr>
           <tr> 
          <td><span></span></td>
        </tr>
        <tr>
             <td>&nbsp;</td> 
           </tr> 
           <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
             <td><font id="wstitle4">If your wireless client requires the Router's PIN number, enter></font><% getPINASP(); %>&nbsp;<font id="wstitle5">in it.</font>
             </td> 
           </tr>  
           <tr> 
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
        <tr>
          <td>&nbsp;</td>
        </tr>
      </table></td>
      <td width="8">&nbsp;</td>
           </tr>
         </table>
</a></li>
 </ul>
</div>
 <div  class="button"><ul><li><a href="empty.html" id="wsExit">Exit</a></li></ul></div>
 </div>
</div>	
</div>
<div id='confirm'  style='display:none'>
	<a href='#' title='Close' class='modalCloseX modalClose'>x</a>
	<div class='header'><span id="wsConfirm">Confirm</span></div>
	<p class='message'></p>
	<div class='buttons'>
	<div class='no modalClose' id="wsOK">OK</div>
</div>
</div>
</form>
</body>
<script>
var current_url=location.href;
var current_url_len=current_url.length;
var index=current_url.charAt(current_url_len-1);
if (index==1)
{
	$(".widearea2").animate({marginLeft: "-601px"}, 300);
}
</script>        
</html>
