<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
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
	var e = document.getElementById("applicationAddApp");
	e.innerHTML = _("application add app");
	e = document.getElementById("applicationActive");
	e.innerHTML = _("application active");		
	e = document.getElementById("applicationServiceName");
	e.innerHTML = _("application service name");
	e = document.getElementById("applicationPort");
	e.innerHTML = _("application port");		
	e = document.getElementById("applicationServerIP");
	e.innerHTML = _("application server ip");	
	e = document.getElementById("applicationAppRuleSum");
	e.innerHTML = _("application app rule sum");		
	e = document.getElementById("applicationAppRuleSum2");
	e.innerHTML = _("application app rule sum2");	
	e = document.getElementById("applicationTdActive");
	e.innerHTML = _("application td active");
	e = document.getElementById("applicationTdName");
	e.innerHTML = _("application td name");
	e = document.getElementById("applicationTdPort");
	e.innerHTML = _("application td port");
	e = document.getElementById("applicationTdServerIP");
	e.innerHTML = _("application td server ip");	
	e = document.getElementById("applicationTdModify");
	e.innerHTML = _("application td modify");
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
	e = document.getElementById("User_Defined");
	e.innerHTML = _("User_Defined");
	e = document.getElementById("Services");
	e.innerHTML = _("Services");
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
function checkSubnet(ip, mask, client)
{ 
  ip_d = atoi(ip, 1);
  mask_d = atoi(mask, 1);
  client_d = atoi(client, 1);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 2);
  mask_d = atoi(mask, 2);
  client_d = atoi(client, 2);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 3);
  mask_d = atoi(mask, 3);
  client_d = atoi(client, 3);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  ip_d = atoi(ip, 4);
  mask_d = atoi(mask, 4);
  client_d = atoi(client, 4);
  if ( (ip_d & mask_d) != (client_d & mask_d ) )
	return false;

  return true;
}
function doServiceChange(index){
    document.NAT_APP.PortRule_Active.checked = true;    
	if((index == 1) || (index == 9)){
	    document.NAT_APP.PortRule_Name.value = "";
	    document.NAT_APP.PortRule_Port.value = "";
		return;
	}else{
		if(index==2){	
			document.NAT_APP.PortRule_Name.value = "DNS";
			document.NAT_APP.PortRule_Port.value = "53";			
		}else if(index==3){
			document.NAT_APP.PortRule_Name.value = "FTP";
			document.NAT_APP.PortRule_Port.value = "21";
		}
		else if(index==4){
			document.NAT_APP.PortRule_Name.value = "HTTP";
			document.NAT_APP.PortRule_Port.value = "80";
		}
		else if(index==5){
			document.NAT_APP.PortRule_Name.value = "HTTPS";
			document.NAT_APP.PortRule_Port.value = "443";
		}
		else if(index==6){
			document.NAT_APP.PortRule_Name.value = "POP3";
			document.NAT_APP.PortRule_Port.value = "110";
		}
		else if(index==7){
			document.NAT_APP.PortRule_Name.value = "SMTP";
			document.NAT_APP.PortRule_Port.value = "25";
		}
		else if(index==8){
			document.NAT_APP.PortRule_Name.value = "TELNET";
			document.NAT_APP.PortRule_Port.value = "23";
		}
		else{
			document.NAT_APP.PortRule_Name.value = "";
			document.NAT_APP.PortRule_Port.value = "";
		}				
		return;
	}
}

function natClick(){
	document.NAT_APP.PortRule_Active.value = document.NAT_APP.PortRule_Active.checked ? "1": "0";	
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

function checkIpAddr(field, ismask)
{
	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	return true;
}

function checkTableNum()
{
	var nvr_str1= new Array(10);
	var num = "<% getCfgGeneral(1, "fr_rule_num"); %>";		
	
	if(num >= 10){
	   alert('The rule number is exceeded.');
	   return false;
	}
	else
	   return true; 
}
//Steve add
function isPortRule(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == ',' ) || (str.charAt(i) == '-' ))
			continue;
		return 0;
	}
	return 1;
}
function getValueCount(str)
{
	var num=0;
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == ',' )  || (str.charAt(i) == '-' ))
			num++;
	}
	return num;
}
function getValue2Count(str,char1)
{
	var num=0;
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;
	}
	return num;
}

function getNthValue(str,index,char1)
{
	var num=0;
	var s1=0;
	var s2=0;
	var max=0;

	max=getValue2Count(str,char1);
	
if(index == 1){
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;		
		s1=0;		
		if(num == index){
		   		s2=i;
		   		break;
		   	}
	}
}else if(index == max+1){
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;		
		s2=str.length;		
		if(num == max){
		   		s1=i+1;
		   		break;
		   	}
	}

}else{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;
			
		if(num == (index-1)){
		   		s1=i+1;
		   		break;
		   	}
	}
	num=0;
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == char1 ))
			num++;
			
		if(num == index){
		   		s2=i;
		   		break;
		   	}
	}
}
	str1=str.substring(s1,s2);
    return str1;
}

function checkPortRule(field)
{
    var len=0;
    
    //check port value if null
	if(field.value == ""){
		alert("Please set port between 1~65535");
		return false;
	}
	
	//Rule accept "0~9" & "," & "-"
	if (isPortRule(field.value) == 0) {
		alert('Invaild setting');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	//The sum of "," and  "-" is less than 15
	if (getValueCount(field.value) >= 15) {
		alert('Invaild setting');
		field.value = field.defaultValue;
		field.focus();
		    return false;
	}
	
	//check last char
	len=field.value.length;
	if ((field.value.charAt(len-1) == ',' )  || (field.value.charAt(len-1) == '-' )){
		alert('Invaild setting');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	
	
	//check port range
	if (getValueCount(field.value) == 0) {
		if(field.value != ""){
		d1 = atoi(field.value, 1);
		if(isAllNum( field.value ) == 0){
			alert("Please set port between 1~65535");
			field.focus();
			return false;
		}
		if(d1 > 65535 || d1 < 1){
			alert("Please set port between 1~65535");
			field.focus();
			return false;
		}			
	}
	}else{
	    var max=0;
		max=getValue2Count(field.value,',');
		if(max == 0){
		    str1=field.value;
			var num=0;
		    len=str1.length;
		    num=getValue2Count(str1,'-');
		    if((num >= 2) || (len <= 2)){
		    	alert("Invaild setting");field.focus();return false;
		    }else{
		    	t1=str1;
		    	t2=str1;
		    	s1=getNthValue(t1,1,'-');
		    	s2=getNthValue(t2,2,'-');
		    	
		    	d1 = atoi(s1, 1);
		    	d2 = atoi(s2, 1);
		    	if(d1 > 65535 || d1 < 1){
		    		alert("Please set port between 1~65535");field.focus();return false;
		    	}
		    	if(d2 > 65535 || d2 < 1){
		    		alert("Please set port between 1~65535");field.focus();return false;
		    	}
		    	if(d1 >= d2){
		    		alert("Invaild setting");field.focus();return false;
		    	}
		    }
		}else{
			for (var i=1; i < max+2; i++) {
	   			str1=getNthValue(field.value,i,',');
	   			if(str1 == ""){
		    		alert("Please set port between 1~65535");field.focus();return false;
		    	}
		    	if(str1.indexOf("-",0) == -1){
		    		d1 = atoi(str1, 1);
		    		if(d1 > 65535 || d1 < 1){
		    			alert("Please set port between 1~65535");field.focus();return false;
		    		}
		    	}else{
		    		var num=0;
		    		len=str1.length;
		    		num=getValue2Count(str1,'-');
		    		if((num >= 2) || (len <= 2)){
		    			alert("Invaild setting");field.focus();return false;
		    		}else{
		    		    t1=str1;
		    		    t2=str1;
		    			s1=getNthValue(t1,1,'-');
		    			s2=getNthValue(t2,2,'-');
		    			d1 = atoi(s1, 1);
		    			d2 = atoi(s2, 1);
		    			if(d1 > 65535 || d1 < 1){
		    				alert("Please set port between 1~65535");field.focus();return false;
		    			}
		    			if(d2 > 65535 || d2 < 1){
		    				alert("Please set port between 1~65535");field.focus();return false;
		    			}
		    			if(d1 >= d2){
		    				alert("Invaild setting");field.focus();return false;
		    			}
		    		}
		    	}
	   		}
		}
		
	}
	
	return true;
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

    //check rule num
    var nat_flag= "<% getCfgNat(1, "nat_flag"); %>";
	if(nat_flag == "0"){
		if (!checkTableNum())
      	return false;
    }
    
    //check name
    if(checkInput(document.NAT_APP.PortRule_Name.value)==1){
    	alert("System do not support specific invalid characters.");
		return false;
    }
	  
	//check Port value
	if (!checkPortRule(document.NAT_APP.PortRule_Port))
		    return false;
	  
	natClick();
	
	//check IP
	if (!checkIpAddr(document.NAT_APP.PortRule_SrvAddr, false))
		    return false;		    
	if ( !checkSubnet(document.NAT_APP.lanIP.value, document.NAT_APP.lanMask.value, document.NAT_APP.PortRule_SrvAddr.value)) {
        		alert('Invalid server ip address!\nIt should be located in the same subnet of current LAN IP address.');
        		document.NAT_APP.PortRule_SrvAddr.value = document.NAT_APP.PortRule_SrvAddr.defaultValue;
        		document.NAT_APP.PortRule_SrvAddr.focus();
        		return false;
    }
	document.NAT_APP.PortFor_rtnFlag.value = "0";
	
	showWebMessage(2, "");
	return true;
}
function doEditRule(){
		document.NAT_APP.PortFor_rtnFlag.value = "1";
}
function doDeleteRule(){
		document.NAT_APP.PortFor_rtnFlag.value = "2";
		showWebMessage(2, "");
}
function doFindCurrentRule(CurrentIndex){
		document.NAT_APP.PortFor_rtnCurIndex.value = CurrentIndex;
}
function updateState(){
	initTranslation();
	
	if (document.NAT_APP.PortRule_Active.value == "1")
		document.NAT_APP.PortRule_Active.checked = true;
}
function checkValue(cstr){

}
</script>
</head>
<body onload="updateState()">
<form method="post" name="NAT_APP" action="/goform/portForward_natAPP" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="applicationAddApp">Add Application Rule</td>
                   </tr></table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="3%"><input name="PortRule_Active" type="checkbox" value="<% getCfgNat(1, "nat_active"); %>" ></td>
<td width="97%" id="applicationActive">Active</td>                      
<td>&nbsp;</td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="applicationServiceName">Service Name : </td>
<td><input name="PortRule_Name" size="31" maxlength="31" value="<% getCfgNat(1, "nat_service"); %>" type="text" />
<select name="Service_Name" size="1" onchange="doServiceChange(this.selectedIndex);">
<option value="00000000" selected="selected" id=User_Defined>User Defined </option>
<option value="00000001" id=Services>---Services--- </option>
<option value="00000002">DNS </option>
<option value="00000003">FTP </option>
<option value="00000004">HTTP </option>
<option value="00000005">HTTPS </option>
<option value="00000006">POP3 </option>
<option value="00000007">SMTP </option>
<option value="00000008">TELNET </option>
</select></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="applicationPort">Port : </td>
<td><input name="PortRule_Port" size="31" maxlength="99" value="<% getCfgNat(1, "nat_port"); %>" onkeypress="" type="text" /> (Ex: 10-20,30,40)
</td>
</tr>
</table>
</li>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="40%" id="applicationServerIP">Server IP Address : </td>
<td><input name="PortRule_SrvAddr" size="15" maxlength="15" value="<% getCfgNat(1, "nat_ip"); %>"  type="text" /></td>
</tr>
</table>
</li>
<li class="space3"></li>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="applicationAppRuleSum">Application Rules Summary</td>
                   </tr></table>
</li>
<li class="space"></li>
<li class="w_text2">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><table width="100%" cellpadding="0" cellspacing="0" >
<tr>
<td height="26" colspan="7"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title" id="applicationAppRuleSum2"> Application Rules Summary<span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>                
<tr>
<td width="5%"   align=center valign="top"><span class="top_left">#</span></td>
<td width="10%"   align=center><font class="top_font"><span id="applicationTdActive">Active</span></font></td>
<td width="25%"  align=center><font class="top_font"><span id="applicationTdName">Name</span></font></td>
<td width="25%"  align=center><font class="top_font"><span id="applicationTdPort">Port</span></font></td>
<td width="25%"  align=center><font class="top_font"><span id="applicationTdServerIP">Server IP Address</span></font></td>          
<td width="10%"  align=center><font class="top_right"><span id="applicationTdModify">Modify</span></font></td>
</tr>
<% showNatApplicationRulesASP(); %>
<tr>
<td height="5" colspan="7"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
<td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
<td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
</tr>
</table></td>
</tr>
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
<input type="submit" value="Apply" id="natApply" name="addNAT_APP" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/nat_application.asp" name="nat_app_url">
<input type="hidden" name="PortFor_rtnFlag" value="0" />
<input type="hidden" name="PortFor_rtnCurIndex" value="0" />
<input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>">
<input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>">
</li>
</center>             
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
