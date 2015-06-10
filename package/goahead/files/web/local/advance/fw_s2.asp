<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/tablet.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" /> 
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
var D_NUM = 32; 
function initTranslation(){
	//var e = document.getElementById("portFilterMac");
	//e.innerHTML = _("port filter macaddr");	
	var e = document.getElementById("portFilterSIPAddr");
	e.innerHTML = _("port filter source ipaddr");
	e = document.getElementById("portFilterSPortRange");
	e.innerHTML = _("port filter source port range");
	e = document.getElementById("portFilterDIPAddr");
	e.innerHTML = _("port filter dest ipaddr");
	e = document.getElementById("portFilterDPortRange");
	e.innerHTML = _("port filter dest port range");
	e = document.getElementById("portFilterProtocol");
	e.innerHTML = _("firewall protocol");	
	e = document.getElementById("portFilterResponToPingOn");
	e.innerHTML = _("port filter Respon To Ping On");	
	e = document.getElementById("portFilterPingFrmWANFilterWAN");
	e.innerHTML = _("port filter PingFrmWANFilter WAN");
	e = document.getElementById("portFilterPingFrmWANFilterDisable");
	e.innerHTML = _("port filter PingFrmWANFilter Disable");
	e = document.getElementById("portFilterEnableFirewallRule1");
	e.innerHTML = _("port filter enable firewall rule1");
	e = document.getElementById("portFilterEnableFirewallRule2");
	e.innerHTML = _("port filter enable firewall rule2");
	e = document.getElementById("portFilterFirewallRule");
	e.innerHTML = _("port filter firewall rule");	
	e = document.getElementById("portFilterFirewallRule2");
	e.innerHTML = _("port filter firewall rule");	
	e = document.getElementById("portFilterAddFirewallRule");
	e.innerHTML = _("port filter add firewall rule");
	e = document.getElementById("portFilterServiceName");
	e.innerHTML = _("port filter service name");	
	e = document.getElementById("portFilterApply1");
	e.value = _("port filter apply1");
	e = document.getElementById("portFilterApply2");
	e.value = _("port filter apply2");
	e = document.getElementById("portFilterAddRule");
	e.value = _("port filter add Rule");
	e = document.getElementById("natReset");
	e.value = _("nat reset");	
	e = document.getElementById("portFilterTableServiceNmae");
	e.innerHTML = _("port filter table service name");
	//e = document.getElementById("portFilterTableMACaddress");
	//e.innerHTML = _("port filter table mac address");
	e = document.getElementById("portFilterTableDestIP");
	e.innerHTML = _("port filter table dest ip");
	e = document.getElementById("portFilterTableSourceIP");
	e.innerHTML = _("port filter table source ip");
	e = document.getElementById("portFilterTableProtocol");
	e.innerHTML = _("port filter table source prorocol");
	e = document.getElementById("portFilterTableDestPortRange");
	e.innerHTML = _("port filter table dest port range");
	e = document.getElementById("portFilterTableSourcePortRange");
	e.innerHTML = _("port filter table source port range");
	e = document.getElementById("portFilterTableAction");
	e.innerHTML = _("port filter table source action");
	e = document.getElementById("portFilterTableDelete");
	e.innerHTML = _("port filter table source delete");	
} 

<!--
function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
//-->
   
function atoi(str, num)
{
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
function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkInput(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == '%' ) || (str.charAt(i) == "'" )  || (str.charAt(i) == '"' )  || (str.charAt(i) == '<' )  || (str.charAt(i) == '>' ))
		return 1;
	}
	return 0;
}

function formCheck(){
/*
	if( document.FiewallService.sip_address.value == "" && 
		document.FiewallService.dip_address.value == "" &&
		document.FiewallService.sFromPort.value == "" &&
		document.FiewallService.dFromPort.value == "" &&
		document.FiewallService.mac_address.value == ""){
		alert("Please input any IP or/and port value.");
		return false;
	}*/
	
	//check name
    if(checkInput(document.FiewallService.comment.value)==1){
    	alert("System do not support specific invalid characters.");
		return false;
    }
    
	if( document.FiewallService.sip_address.value == "" && 
		document.FiewallService.dip_address.value == "" &&
		document.FiewallService.sFromPort.value == "" &&
		document.FiewallService.dFromPort.value == "" ){
		alert("Please input any IP or/and port value.");
		return false;
	}
	
	
	if(document.FiewallService.sFromPort.value != ""){
		d1 = atoi(document.FiewallService.sFromPort.value, 1);
		if(isAllNum( document.FiewallService.sFromPort.value ) == 0){
			alert("Invalid port number: source port.");
			document.FiewallService.sFromPort.focus();
			return false;
		}
		if(d1 > 65535 || d1 < 1){
			alert("Invalid port number: source port.");
			document.FiewallService.sFromPort.focus();
			return false;
		}		
		if(document.FiewallService.sToPort.value != ""){
			if(isAllNum( document.FiewallService.sToPort.value ) == 0){
				alert("Invalid port number: source port.");
				return false;
			}		
			d2 = atoi(document.FiewallService.sToPort.value, 1);
			if(d2 > 65535 || d2 < 1){
				alert("Invalid source port number.");
				return false;
			}
			if(d1 > d2){
			alert("Invalid source port range setting.");
			return false;
			}
		}
	}
	if(document.FiewallService.dFromPort.value != ""){
		d1 = atoi(document.FiewallService.dFromPort.value, 1);
		if(isAllNum( document.FiewallService.dFromPort.value ) == 0){
			alert("Invalid port number: dest port.");
			return false;
		}
		if(d1 > 65535 || d1 < 1){
			alert("Invalid port number: dest port.");
			return false;
		}		
		if(document.FiewallService.dToPort.value != ""){
			if(isAllNum( document.FiewallService.dToPort.value ) == 0){
				alert("Invalid port number: dest port.");
				return false;
			}		
			d2 = atoi(document.FiewallService.dToPort.value, 1);
			if(d2 > 65535 || d2 < 1){
				alert("Invalid dest port number.");
				return false;
			}
			if(d1 > d2){
			alert("Invalid dest port range setting.");
			return false;
			}
		}
	}
	// check ip address format
	
	//Steve add
	var num = "<% getCfgGeneral(1, "fl_rule_num"); %>";		
	if(num >= D_NUM){
	   alert('The rule number is exceeded.');
	   return false;
	}
	
    showWebMessage(2, "");		
	return true;
}
function EnableClick(){
	document.FiewallServiceEnable.portFilterEnabled.value = document.FiewallServiceEnable.portFilterEnabled.checked ? "1": "0";
	document.FiewallService.Service_rtnFlag.value = "0";		
	showWebMessage(2, "");	
	return true;
}
function doFindCurrentRule(CurrentIndex){
	document.FiewallService.Service_rtnIndex.value = CurrentIndex;	
}
function doDeleteRule(){
	document.FiewallService.Service_rtnFlag.value = "2";
	showWebMessage(2, "");
}
function updateState(){
	initTranslation();	
	protocolChange();	
	if (document.FiewallServiceEnable.portFilterEnabled.value == "1")
		document.FiewallServiceEnable.portFilterEnabled.checked = true;		
	var wpf = "<% getCfgGeneral(1, "WANPingFilter"); %>";		
	if(wpf == "1")
		document.FiewallServiceIcmp.pingFrmWANFilterEnabled.options.selectedIndex = 0;
	else
		document.FiewallServiceIcmp.pingFrmWANFilterEnabled.options.selectedIndex = 1;		
}
function protocolChange(){
	if( document.FiewallService.protocol.options.selectedIndex == 1 ||
		document.FiewallService.protocol.options.selectedIndex == 2 ||
		document.FiewallService.protocol.options.selectedIndex == 3){
		document.FiewallService.dFromPort.disabled = false;
		document.FiewallService.dToPort.disabled = false;
		document.FiewallService.sFromPort.disabled = false;
		document.FiewallService.sToPort.disabled = false;
	}else{
		document.FiewallService.dFromPort.disabled = true;
		document.FiewallService.dToPort.disabled = true;
		document.FiewallService.sFromPort.disabled = true;
		document.FiewallService.sToPort.disabled = true;
		document.FiewallService.dFromPort.value = ""; 
		document.FiewallService.dToPort.value = "";
		document.FiewallService.sFromPort.value =  "";
		document.FiewallService.sToPort.value = "";
	}
}   
</script>
</head>
<body onload="updateState()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<form method="post" name="FiewallServiceIcmp" action="/goform/ipportFilter_icmp" >
<li class="title">ICMP </li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterResponToPingOn">Respond to Ping on:</td>
<td><select onchange="" size="1" name="pingFrmWANFilterEnabled">                  
<option value=1 id="portFilterPingFrmWANFilterDisable">Disable</option>
<option value=0 id="portFilterPingFrmWANFilterWAN">WAN</option>
</select></td>
</tr>
</table>
</li>   
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td colspan=2>
<input type="submit" value="Apply" id="portFilterApply1" name="add" onClick="EnableClick()">
<input type="hidden" value="/local/advance/fw_s2.asp" name="service_url"> 			         		       
</td>
</tr>
</table>
</li>
</form>               
<form method="post" name="FiewallServiceEnable" action="/goform/ipportFilter_enable" >
<li class="space3"></li>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="portFilterEnableFirewallRule1">Enable Firewall Rule</td>
                   </tr></table>
</li> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="3%"><input type="checkbox" value="<% getCfgZero(1, "IPPortFilterEnable"); %>" name="portFilterEnabled" ></td>
<td id="portFilterEnableFirewallRule2">Enable Firewall Rule </td>
</tr>
</table>
</li>    
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td colspan=2>
<input type="submit" value="Apply" id="portFilterApply2" name="add" onClick="EnableClick()">
<input type="hidden" value="/local/advance/fw_s2.asp" name="service_url"> 			         		       
</td>
</tr>
</table>
</li>
</form>                        
<form method="post" name="FiewallService" action="/goform/ipportFilter_service" >
<li class="space3"></li>
<li class="title" id="portFilterAddFirewallRule">Add Firewall Rule </li> 
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterServiceName">Service Name : </td>
<td><input name="comment" size="16" maxlength="32" value=""  type="text"></td>
</tr>
</table>
</li>
<!--
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterMac">MAC Address : </td>
<td><input name="mac_address" size="18"  value="" type="text" /></td>
</tr>
</table>
</li>
-->
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterDIPAddr">Dest IP Address : </td>
<td><input name="dip_address" size="16" value=""  type="text" />-<input name="dip_address2" size="16" value=""  type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterSIPAddr">Source IP Address : </td>
<td><input name="sip_address" size="16" value="" type="text" />-<input name="sip_address2" size="16" value="" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterProtocol">Protocol : </td>
<td>
<select name="protocol" onChange="protocolChange()" id="procotol">
<option value="ALL">ALL </option>
<option value="TCP">TCP </option>
<option value="UDP">UDP </option>
<option value="BOTH">BOTH </option>-->             
</select></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterDPortRange">DestPortRange : </td>
<td><input name="dFromPort" size="5" value="" type="text" />-
<input name="dToPort" size="5"  value="" type="text" /></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="portFilterSPortRange">SourcePortRange : </td>
<td><input name="sFromPort" size="5"  value=""  type="text" >-
<input name="sToPort" size="5"  value=""  type="text" ></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td colspan=2>
<input type="submit" value="Add Rule" id="portFilterAddRule" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="hidden" value="/local/advance/fw_s2.asp" name="service_url">
<input type="hidden" name="Service_rtnFlag" value="0" > 
<input type="hidden" name="Service_rtnIndex" value="0" >			         		       
</td>
</tr>
</table>
</li>
</form> 
<li class="space3"></li>
<li class="title" id="portFilterFirewallRule">Firewall Rule </li>
<li class="w_text2">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td>
<table width="100%" cellpadding="0" cellspacing="0" >
<tr><td height="26" colspan="9">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id=portFilterFirewallRule2> Firewall Rule </td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table>
</td></tr>
<tr>
<td width="5%"><center><span class="top_left">#</span></center></td>
<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableServiceNmae">ServiceName</span></font></td>
<!--<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableMACaddress">MACaddresse</span></font></td>-->
<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableDestIP">DestIP</span></font></td>
<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableSourceIP">SourceIP</span></font></td>
<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableProtocol">Protocol</span></font></td>
<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableDestPortRange">DestPortRange</span></font></td>
<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableSourcePortRange">SourcePortRange</span></font></td>
<td width="10%"   align=center nowrap><font class="top_font"><span id="portFilterTableAction">Action</span></font></td>
<td width="10%"   align=center nowrap><font class="top_right"><span id="portFilterTableDelete">Delete</span></font></td>
</tr>
<% showIPPortFilterServiceRulesASP(); %>  
<tr><td height="5" colspan="9">
<table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
<td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
<td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
</tr>
</table>
</td></tr>
</table></td>
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
<input type=reset value=Reset id="natReset" onClick="window.location.reload()">    
</li>
</center>
</ul>
</div>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>      
</html>     
            
            
            