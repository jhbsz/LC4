<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<meta http-equiv="Pragma" content="no-cache">
<style>
td {white-space: nowrap;}
</style>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
var D_NUM = 16; 
function initTranslation(){
	var e = document.getElementById("SCFilterRestrict");
	e.innerHTML = _("sc filter restrict");
	e = document.getElementById("SCFilterKeywordBlocking");
	e.innerHTML = _("sc filter keyword blocking");	
	e = document.getElementById("SCFilterEnableKeywordBlocking");
	e.innerHTML = _("sc filter enable keyword blocking");		
	e = document.getElementById("SCFilterKeyword");
	e.innerHTML = _("sc filter keyword");		
	e = document.getElementById("SCFilterKeywordList");
	e.innerHTML = _("sc filter keyword list");	
	e = document.getElementById("SCFilterTrustedIP");
	e.innerHTML = _("sc filter trusted ip");	
	e = document.getElementById("SCFilterStr1");
	e.innerHTML = _("sc filter str1");	
	e = document.getElementById("SCFilterStr2");
	e.innerHTML = _("sc filter str2");	
	e = document.getElementById("SCFilterAdd");
	e.value = _("sc filter add");
	e = document.getElementById("SCFilterDelete");
	e.value = _("sc filter delete");
	e = document.getElementById("SCFilterClear");
	e.value = _("sc filter clear");
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
	
	e = document.getElementById("SCFilterRWFActiveX");
	e.innerHTML = _("sc filter rwf activex");
	e = document.getElementById("SCFilterRWFJava");
	e.innerHTML = _("sc filter rwf java");
	e = document.getElementById("SCFilterRWFCookies");
	e.innerHTML = _("sc filter rwf cookies");
	e = document.getElementById("SCFilterRWFWeb");
	e.innerHTML = _("sc filter rwf web");
}    
function updateState(){
	initTranslation();
	if (document.SCFilter.websFilterProxy.value == "1")
		document.SCFilter.websFilterProxy.checked = true;
	if (document.SCFilter.websFilterJava.value == "1")
		document.SCFilter.websFilterJava.checked = true;
	if (document.SCFilter.websFilterActivex.value == "1")
		document.SCFilter.websFilterActivex.checked = true;
	if (document.SCFilter.websFilterCookies.value == "1")
		document.SCFilter.websFilterCookies.checked = true;		
	if (document.SCFilter.cfKeyWord_Enable.value == "1")
		document.SCFilter.cfKeyWord_Enable.checked = true;	
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

function checkSubnet(ip, mask, client){
  if(client == "0.0.0.0")
    return true;
    
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

function formCheck(){

    if (!checkIpAddr(document.SCFilter.websTrustedIPAddress, false))
		    return false;
			
	if ( !checkSubnet(document.SCFilter.lanIP.value, document.SCFilter.lanMask.value, document.SCFilter.websTrustedIPAddress.value)) {
        		alert('IP is invaild');
        		document.SCFilter.websTrustedIPAddress.value = document.SCFilter.websTrustedIPAddress.defaultValue;
        		document.SCFilter.websTrustedIPAddress.focus();
        		return false;
    }		
    
	document.SCFilter.websFilterProxy.value = document.SCFilter.websFilterProxy.checked ? "1": "0";
	document.SCFilter.websFilterJava.value = document.SCFilter.websFilterJava.checked ? "1": "0";
	document.SCFilter.websFilterActivex.value = document.SCFilter.websFilterActivex.checked ? "1": "0";
	document.SCFilter.websFilterCookies.value = document.SCFilter.websFilterCookies.checked ? "1": "0";
	document.SCFilter.cfKeyWord_Enable.value = document.SCFilter.cfKeyWord_Enable.checked ? "1": "0";	
	var set = "";
	for(var i=0; i < document.SCFilter.cfKeyWord_DomainList.options.length; i++){
	       if(i!=(document.SCFilter.cfKeyWord_DomainList.options.length - 1)) 
		   	{set += document.SCFilter.cfKeyWord_DomainList.options[i].text + ";";}
		   else
		   	{set += document.SCFilter.cfKeyWord_DomainList.options[i].text;}
	}		
	document.SCFilter.url_str.value=set;
	showWebMessage(2, "");	
	return true;
}
function checkValue(str){
   var flag=0;
	for(i=0;i < document.SCFilter.cfKeyWord_DomainList.options.length;i++){
		if(document.SCFilter.cfKeyWord_DomainList.options[i].text == str){
		   flag=1;
		   break;	   
		}		  
	}
	if(flag == 1)
	   	return 1;
	else
    	return 0;
}
function cfKeyWord_AddKeyword_click(){	
	var D_str=document.SCFilter.cfKeyWord_Domain.value;
	if(D_str!=""){	
		var list_num=document.SCFilter.cfKeyWord_DomainList.length;
		if(list_num < D_NUM){ 
		    if(checkValue(D_str)==0){
				document.SCFilter.cfKeyWord_DomainList.options[list_num]=new Option(D_str, D_str);
			}
			else
			{	    
				alert("Key word is exist"); 
				document.SCFilter.cfKeyWord_Domain.value="";
			}
		}
		else
		   alert("The rule number is exceeded.");
	} 
}
function cfKeyWord_ClearList_click(){
	document.SCFilter.cfKeyWord_DomainList.options.length=0;		    
}
function cfKeyWord_DeleteKeyword_click(){
    
	//for IE & firefox
	if(document.SCFilter.cfKeyWord_DomainList.selectedIndex >= 0){
		document.SCFilter.cfKeyWord_DomainList.remove(document.SCFilter.cfKeyWord_DomainList.selectedIndex);
		cfKeyWord_DeleteKeyword_click();
	}
} 
</script>
</head>
<body onload="updateState()">
<form method="post" name="SCFilter" action="/goform/webSCFilter">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="SCFilterTrustedIP">Trusted IP Setup</td>
                   </tr></table>
</li>
<li class="w_text2">
<table width="95%" border="0" align=center  cellspacing="0">
<tr><td colspan=2 id="SCFilterStr1">A trusted computer has full access to all blocked resources. 0.0.0.0 means there is no trusted computer.</td></tr>
<tr><td width="21%" id="SCFilterStr2">Trusted Computer IP Address:</td><td><input type="text" size="24" name="websTrustedIPAddress" value="<% showTrustedIPAddressASP(); %>"></td></tr>
</table>
</li>
<li class="space3"></li>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="SCFilterRestrict">Restrict Web Features</td>
                   </tr></table>
</li>
<li class="w_text2">
<table width="95%" border="0" align=center  cellspacing="0">
<tr>
<td width="3%"><input name=websFilterActivex type="checkbox" value="<% getCfgZero(1, "websFilterActivex"); %>" ></td>
<td width="22%" id="SCFilterRWFActiveX">ActiveX    </td>
<td>&nbsp;</td>
<td width="3%"><input name=websFilterJava type="checkbox" value="<% getCfgZero(1, "websFilterJava"); %>" ></td>
<td width="22%" id="SCFilterRWFJava">Java    </td>
<td>&nbsp;</td>
<td width="3%"><input name=websFilterCookies type="checkbox" value="<% getCfgZero(1, "websFilterCookies"); %>" ></td>
<td width="22%" id="SCFilterRWFCookies">Cookies    </td>
<td>&nbsp;</td>
<td width="3%"><input name=websFilterProxy type="checkbox" value="<% getCfgZero(1, "websFilterProxy"); %>" ></td>
<td width="22%" id="SCFilterRWFWeb">Web Proxy  </td>
<td>&nbsp;</td>
</tr>
</table>
</li>
<li class="space3"></li>
<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="SCFilterKeywordBlocking">Keyword Blocking</td>
                   </tr></table>
</li>
<li class="w_text2">
<table width="95%" border="0" align=center cellspacing="0">
<tr><td width="3%"><input type="checkbox" name="cfKeyWord_Enable" value="<% getCfgZero(1, "websHostFiltersEnabled"); %>" ></td>
<td width="97%" id="SCFilterEnableKeywordBlocking">Enable URL Keyword Blocking</td></tr>
</table>
<table width="95%" border="0" align=center cellspacing="0">		   
<tr><td  width="10%" id="SCFilterKeyword" nowrap>Keyword</td>
<td  width="2%">&nbsp;</td>
<td  width="20%"><input type="text" name="cfKeyWord_Domain" size="48" maxlength="127" value=""></td>
<td  width="2%">&nbsp;</td>
<td  width="66%"><input type="button" name="cfKeyWord_AddKeyword"  id="SCFilterAdd" value="Add" style="width:60pt;" onClick="cfKeyWord_AddKeyword_click()" ></td>
</tr>
<tr><td>&nbsp;</td></tr>
</table>	   
<table width="95%" border="0" align=center cellspacing="0">	   
<tr><td id="SCFilterKeywordList">Keyword List                  </td></tr>
<tr><td>
<select name="cfKeyWord_DomainList" size="6" multiple="true" OnChange="this.form.cfKeyWord_Domain.value = this.options[this.selectedIndex].text;">                     
<script language="JavaScript" type="text/javascript">
var i;
var entries = new Array();
var all_str = "<% getCfgGeneral(1, "websHostFilters"); %>";
if(all_str.length){
entries = all_str.split(";");
for(i=0; i<entries.length; i++){                       
document.write(" <option >");
document.write(entries[i]);
document.write("</option >");
}	
}
</script>
</select>
</td></tr>
<tr><td height="5"></td></tr>
<tr><td>
<input type="button" name="cfKeyWord_DeleteKeyword"  id="SCFilterDelete" value="Delete" style="width:60pt;"  onClick="cfKeyWord_DeleteKeyword_click()" />&nbsp;
<input type="button" name="cfKeyWord_ClearList" id="SCFilterClear" value="Clear All" style="width:60pt;" onClick="cfKeyWord_ClearList_click()" />
</td></tr>
<tr><td>&nbsp;</td></tr>
</table>
</li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type="submit" value="Apply" id="natApply" name="add" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/SCFilter.asp" name="scfilter_url">
<input type=hidden value="" name="url_str">
<input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>">
<input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>">

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
