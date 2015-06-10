<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
function initTranslation(){
	var e = document.getElementById("RemoteManagementServerPort");
	e.innerHTML = _("remote management server port");
	e = document.getElementById("RemoteManagementServerAccess");
	e.innerHTML = _("remote management server access");
	e = document.getElementById("RemoteManagementSecuredIP");
	e.innerHTML = _("remote management secured ip");
	e = document.getElementById("RemoteManagementSecuredIPAll");
	e.innerHTML = _("remote management secured ip all");
	e = document.getElementById("RemoteManagementSecuredIPSel");
	e.innerHTML = _("remote management secured ip sel");
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
	
	e = document.getElementById("remoteManagementWWW");
	e.innerHTML = _("remote management www");
}
function atoi(str, num){
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}
	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
} 
function checkRange(str, num, min, max){
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}
function isAllNum(str){
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}
function checkIpAddr(field, ismask){
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
function formCheck(){
   var port_value = document.REMOTE.remoteManagementPort.value;
   var num = parseInt(port_value);
   if(port_value = "")
      return false;
   else{       
        if(( num > 65535 ) || (num < 0)){
            alert("The value is invalid");  
   			return false;
   		}
   }
   if (document.REMOTE.btnwww[1].checked == true){    
   		if (checkIpAddr(document.REMOTE.txtwwwip, false) == false)
   		{
			return false;
		}
	}
	showWebMessage(2, "");
    return true;
}
function updateState(){
	initTranslation();
	var rm = "<% getCfgZero(1, "RemoteManagementSel"); %>";
	if(rm == "0")
		document.REMOTE.remoteManagementSel.options.selectedIndex = 1;
	else if(rm == "2")
		document.REMOTE.remoteManagementSel.options.selectedIndex = 2;
	else
		document.REMOTE.remoteManagementSel.options.selectedIndex = 0;
	var nv_rm_sel = <% getCfgZero(1, "RemoteManagementSecuredSel"); %>;	
	if(nv_rm_sel == "0"){
		document.REMOTE.btnwww[0].checked = true;
		document.REMOTE.txtwwwip.disabled = true;
	}else if(nv_rm_sel == "1"){
		document.REMOTE.btnwww[1].checked = true;
		document.REMOTE.txtwwwip.disabled = false;
	}else{
		document.REMOTE.btnwww[0].checked = true;
		document.REMOTE.txtwwwip.disabled = true;
	}
}
function radioChange(){
	if (document.REMOTE.btnwww[0].checked == true)
      	document.REMOTE.txtwwwip.disabled = true;
    else
    	document.REMOTE.txtwwwip.disabled = false;
} 
</script>
</head>
<body onload="updateState()">
<form method="post" name="REMOTE" action="/goform/remote_web" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="remoteManagementWWW">WWW</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerPort">Server Port : </td>
<td><span class="cfig_word">
<input name="remoteManagementPort" size="5" maxlength="5" value="<% getCfgGeneral(1, "RemoteManagementPort"); %>" type="text" />
</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementServerAccess">Server Access : </td>
<td><span class="cfig_word">
<select name="remoteManagementSel" size="1" onchange="">
<option value="1">LAN &amp; WAN</option>
<option value="0">LAN</option>
<option value="2">WAN </option>
</select>
</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="RemoteManagementSecuredIP">Secured Client IP Address   : </td>
<td><input name="btnwww" value="0"  onclick="radioChange()" type="radio" /></td>
<td id="RemoteManagementSecuredIPAll"> All </td>                  
<td><input name="btnwww" value="1" onclick="radioChange()" type="radio" /></td>
<td id="RemoteManagementSecuredIPSel"> Selected </td>
<td><input name="txtwwwip" size="20" maxlength="15" value="<% getCfgGeneral(1, 'RemoteManagementSecuredIP'); %>" type="text" /></td>
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
<input type="submit" value="Apply" id="natApply" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/remote_web.asp" name="remote_url">
</li>
</center>
</ul>
</div>
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
