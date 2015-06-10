<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head><link href="images/css.css" rel="stylesheet" type="text/css" />
<link href="advance/images/table.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/JavaScript">
Butterlate.setTextDomain("main");
<!--
function MM_goToURL() { //v3.0
  var i, args=MM_goToURL.arguments; document.MM_returnValue = false;
  for (i=0; i<(args.length-1); i+=2) eval(args[i]+".location='"+args[i+1]+"'");
}

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
		alert(_("Please enter SSID!"));
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
		if (document.security_form.password.value == ""){
		     alert(_("Please enter Password!"));
		     return false;
		}
		if (document.security_form.password.value.length < 8 || document.security_form.password.value.length > 64) {
			alert(_("Password should be with length 8~64"));
			return false;
		}
		
		if(checkInjection(document.security_form.password.value) == false){
			alert('System do not support specific invalid characters.');
			return false;
		}

		if (document.security_form.password.value != document.security_form.verify_password.value){
			alert(_("Please enter Verify Password correctly!"));
			return false;
		}
	}	
	
	return true;
}


function initAll()
{

	//var AuthMode = '<% getCfgZero(1, "AuthMode"); %>';
	var AuthMode = 'WPA2PSK';
	var EncrypType = '<% getCfgZero(1, "EncrypType"); %>';
	//var Password = '<% getCfgZero(1, "WPAPSK1"); %>';
	var Password = '';


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
  var e = document.getElementById("g2-2easy123");
	e.innerHTML = _("g2-2 easy123");
		e = document.getElementById("g2-2Internet");
	e.innerHTML = _("g2-2 Internet");
		e = document.getElementById("g2-2Password");
	e.innerHTML = _("g2-2 Password");
		e = document.getElementById("g2-2Security");
	e.innerHTML = _("g2-2 Security");
			e = document.getElementById("g41title");
	e.innerHTML = _("g41 title");
			e = document.getElementById("g41msg");
	e.innerHTML = _("g41 msg");
	e = document.getElementById("g41SSID");
	e.innerHTML = _("g41 SSID");
	e = document.getElementById("g41Security");
	e.innerHTML = _("g41 Security");
	e = document.getElementById("g41Security0");
	e.innerHTML = _("g41 Security0");
	e = document.getElementById("g41Security1");
	e.innerHTML = _("g41 Security1");
	e = document.getElementById("g41Security2");
	e.innerHTML = _("g41 Security2");
	
	e = document.getElementById("g41password");
	e.innerHTML = _("g41 password");
	e = document.getElementById("g41Vpassword");
	e.innerHTML = _("g41 Vpassword");
	e = document.getElementById("g2-2Exit");
	e.innerHTML = _("g2-2 Exit");
			e = document.getElementById("g2-2Back");
	e.innerHTML = _("g2-2 Back");
			e = document.getElementById("g2-2Exit");
	e.innerHTML = _("g2-2 Exit");
			e = document.getElementById("g2-2No");
	e.innerHTML = _("g2-2 No");
	e = document.getElementById("g2-2Yes");
	e.innerHTML = _("g2-2 Yes");
		e = document.getElementById("g2-2msg");
	e.innerHTML = _("g2-2 msg");
		e = document.getElementById("g2-2Next");
	e.innerHTML = _("g2-2 Next");
  
		e = document.getElementById("log_out");
	e.innerHTML = _("dash logout title");
  
    //e = document.getElementById("g41msg2");
	//e.innerHTML = _("g41 msg2");
  
	securityMode();
}

function securityMode()
{
	if ((document.security_form.sel_security.options.selectedIndex == 1) ||(document.security_form.sel_security.options.selectedIndex == 2)){
		show_div(true, "div_password_id");		
	}else{
		show_div(false, "div_password_id");
		alert(_("It is strongly recommended to set WPA or WPA2 encryption for better wireless security."));
	}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
</script>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" /></head>
<body onload="initAll()">
<form method="post" name="security_form" action="/goform/EZAPSecurity">
<div id="popup"></div>
<div id="wrapper">
<div id="close"><a href="#"onclick="MM_showHideLayers('popup3_table','','show','popup','','show')" title="Exit"></a></div>
<div class="unit_eazy"></div>
<div class="unit_title" id="g2-2easy123">eaZy 123</div>
<div class="genie_logo"></div>
<div class="genie_color"></div>
<div class="set"><ul><li class="g_ed" id="g2-2Internet">Connect to Internet </li> <li class="g_arrow"></li><li class="g_ed" id="g2-2Password">Router Password </li> <li class="g_arrow"></li><li class="g_ing" id="g2-2Security"> Wireless Security</li></ul></div>
<div class="set_now3"><ul><li>1<span></span></li></ul></div>
<div class="set_now4"><ul><li>2<span></span></li></ul></div>
<div class="set_now5"><ul><li>3</li></ul></div>
<div class="g_word"><span id="g41title">Wireless Security</span><a id="g41msg">A protected wireless network secures the data transferring when you are doing any network  activities wirelessly. Guard it with one of the following security modes and a password.</a><br>
<!--<a id="g41msg2">It is strongly recommended to set WPA or WPA2 encryption for better wireless security.</a>--></div>
<div class="input_bn2">
<ul>
  <li>
    <table width="100%" border="0" cellspacing="2" cellpadding="0">
      <tr>
        <td width="230" id="g41SSID">Wireless Network Name (SSID):</td>
        <td width="200"><input name="ssid" type="hidden" value="" /><input name="textfield1" type="text" class="input1" size="25" maxlength=32 value='<% getCfgGeneral(1, "SSID1"); %>' /></td>
        <td width="200" class="note">&nbsp;</td>
      </tr>
      <tr>
        <td id="g41Security">Security mode:</td>
        <td><label>
            <select name="sel_security" class="select1" onchange="securityMode()">
             <option value=0 id="g41Security0">No Security</option>
             <option value=1 id="g41Security1">WPA-PSK</option>
             <option value=2 id="g41Security2">WPA2-PSK</option>
            </select>
  </label></td>
        <td>&nbsp;</td>
      </tr>
</table>

<span id="div_password_id" class="off">          
<table width="100%" border="0" cellspacing="2" cellpadding="0">
  <tr>
    <td width="230" id="g41password">Wireless password :</td>
    <td width="200"><input name="password" type="hidden" value="" /><input name="textfield3" type="password" class="input1" size="25" maxlength=64 /></td>
    <td width="200" class="note">&nbsp;</td>
  </tr>
  <tr>
    <td width="230" id="g41Vpassword">Verify Password:</td>
    <td width="200"><input name="verify_password" type="hidden" value="" /><input name="textfield4" type="password" class="input1" size="25" maxlength=64 /></td>
    <td width="200" class="note">&nbsp;</td>
  </tr>
</table>
</span>
        
      </li>
    </ul>
  </div>
     <div class="button2"><ul><li><a href="javascript: if (CheckValue()) { document.forms[0].submit(); }" id="g2-2Next">Next</a></li>
     <li><a href="genie3.html" id="g2-2Back">Back</a></li>
  <li><a href="#" onclick="MM_showHideLayers('popup','','show','popup3_table','','show')" title="Exit" style="margin-right:20px;" id="g2-2Exit">Exit</a></li></ul></div> 
<div class="index_bg" id="index_bg"></div>
<div class="in_line" id="topline" ></div>
<div class="frame_left"></div>
<div class="frame_right"></div>
<div class="copyright" id="copyright">(C) Copyright 2008 by ZyXEL Communications Corp.</div>
</div>
</form>
<div id="popup3_table">
<table width="60%" border="0" cellspacing="0" cellpadding="0">
<tr>
  <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="19" height="37"><img src="advance/images/popup_top_left.gif" width="19" height="45" /></td>
      <td background="advance/images/popup_top_center.gif" class="table_title" id=log_out>Log Out</td>
      <td width="15" background="advance/images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')"></a></td>
      <td width="19" height="45"><img src="advance/images/popup_top_right.gif" width="19" height="45" /></td>
    </tr>
  </table></td>
</tr>
<tr>
  <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="19" background="advance/images/popup_main_left.gif">&nbsp;</td>
      <td bgcolor="#FFFFFF"><span class="popupcontent"> 
        <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
          <tr>
            <td width="27%"><center>
              <img src="advance/images/i_info.jpg" width="50" height="49" />
            </center>
            </td>
            <td width="73%" id="g2-2msg">Are you sure you want to Exit?</td>
          </tr>
        </table>
       
      </span></td>
      <td width="19" background="advance/images/popup_main_right.gif">&nbsp;</td>
    </tr>
  </table></td>
</tr>
<tr>
  <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="19" height="48"><img src="advance/images/popup_down_left.gif" width="19" height="48" /></td>
      <td background="advance/images/popup_down_center.gif">
	<div class="button3"><ul>   
  <li><a href="#" title="No"><span id="g2-2No" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')">No</span></a></li>
<li><a href="/logout.html" title="Yes"><span id="g2-2Yes">Yes</span></a></li>
</ul></div></td>
      <td width="19" height="37"><img src="advance/images/popup_down_right.gif" width="19" height="48" /></td>
    </tr>
  </table></td>
</tr>
</table></div>
</body>
</html>
