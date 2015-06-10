<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var IpAliasMode = '<% getCfgZero(1, "Lan2Enabled"); %>';
var wanIP = '<% getPhyWanIfIp(); %>';
var wanMask = '<% getPhyWanIfNetMask(); %>';

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
function checkRange(str, num, min, max)
{
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
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

function IpAlias_mode_switch()
{
    if (document.lanCfg.lan2enabled.checked == true)
	{		
		document.lanCfg.lan2Ip.disabled = false;
	    document.lanCfg.lan2Netmask.disabled = false;
    }
	else
	{
	    document.lanCfg.lan2Ip.disabled = true;
	    document.lanCfg.lan2Netmask.disabled = true;
	}

}
function IpAliasMode_Click()
{
	document.lanCfg.lan2enabled.value = document.lanCfg.lan2enabled.checked ? "1": "0";
	return true;
}
function checkNetMask(field)
{
        if ((field.value != "0.0.0.0") && (field.value != "128.0.0.0") &&
            (field.value != "192.0.0.0") && (field.value != "224.0.0.0") &&
            (field.value != "240.0.0.0") && (field.value != "248.0.0.0") &&
            (field.value != "252.0.0.0") && (field.value != "254.0.0.0") &&
            (field.value != "255.0.0.0") && (field.value != "255.128.0.0") &&
            (field.value != "255.192.0.0") && (field.value != "255.224.0.0") &&
            (field.value != "255.240.0.0") && (field.value != "255.248.0.0") &&
            (field.value != "255.252.0.0") && (field.value != "255.254.0.0") &&
            (field.value != "255.255.0.0") && (field.value != "255.255.128.0") &&
            (field.value != "255.255.192.0") && (field.value != "255.255.224.0") &&
            (field.value != "255.255.240.0") && (field.value != "255.255.248.0") &&
            (field.value != "255.255.252.0") && (field.value != "255.255.254.0") &&
            (field.value != "255.255.255.0") && (field.value != "255.255.255.128") &&
            (field.value != "255.255.255.192") && (field.value != "255.255.255.224") &&
            (field.value != "255.255.255.240") && (field.value != "255.255.255.248") &&
            (field.value != "255.255.255.252") && (field.value != "255.255.255.254") &&
            (field.value != "255.255.255.255"))
        {
		alert("Error. Invalid Subnet Mask.");
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
	return true;
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

function CheckValue()
{		     
    if (document.lanCfg.lan2enabled.checked == true) {
			if (!checkIpAddr(document.lanCfg.lan2Ip, false))
				return false;
			if (!checkIpAddr(document.lanCfg.lan2Netmask, true))
				return false;
			if ( document.lanCfg.lan2Ip.value == document.lanCfg.lan1Ip.value )
			{
			   alert('IP Alias address should not be identical to LAN IP Address');
			   return false;
			}
			if (!checkNetMask(document.lanCfg.lan2Netmask))
				return false;
			if (checkSubnet(wanIP, wanMask, document.lanCfg.lan2Ip.value)) 
			{
        		    //alert('Invalid IP Alias address!\nIt should not be located in the same subnet of current WAN IP address.');
        		    alert('Invalid IP Alias address! It should not be located in the same subnet of current WAN IP address.');
        		    document.lanCfg.lan2Ip.value = document.lanCfg.lan2Ip.defaultValue;
        		    document.lanCfg.lan2Ip.focus();
        		    return false;
                        }
	}
	
    IpAliasMode_Click();
    showWebMessage(2, "");
    return true;
}

function updateStatus()
{
        if (IpAliasMode == "1") //IP Alias enabled
        {     
	        document.lanCfg.lan2enabled.checked = true;
	        document.lanCfg.lan2Ip.disabled = false;
	        document.lanCfg.lan2Netmask.disabled = false;
	    }
        else
        {
            document.lanCfg.lan2enabled.checked = false;
            document.lanCfg.lan2Ip.disabled = true;
	        document.lanCfg.lan2Netmask.disabled = true;
	        document.lanCfg.lan2Ip.value = "0.0.0.0";
	        document.lanCfg.lan2Netmask.value = "0.0.0.0";
        }
}

function initValue()
{

      var e = document.getElementById("ipAliastitle");
	e.innerHTML = _("ipAlias title");
	e = document.getElementById("ipAliasenable");
	e.innerHTML = _("ipAlias enable");
	e = document.getElementById("ipAliasAddress");
	e.innerHTML = _("ipAlias Address");
	e = document.getElementById("ipAliasSubnet");
	e.innerHTML = _("ipAlias Subnet");
	
	e = document.getElementById("ipAliasApply");
	e.value = _("ipAlias Apply");
	e = document.getElementById("ipAliasReset");
	e.value = _("ipAlias Reset");



   updateStatus();
}
</script>
</head>

<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLanIPAlias" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="ipAliastitle">IP Alias 1 </li>
                <li class="w_text">
                  <table id=ipAliasEnabled width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input type="checkbox" name="lan2enabled" value="<% getCfgZero(1, "Lan2Enabled"); %>" onclick="IpAlias_mode_switch()" /><a id="ipAliasenable">IP Alias</a></td>
                        <!--<td id="ipAliasenable">IP Alias</td>-->
                    </tr>
                  </table>
                </li> 
                <li class="w_text">
                  <table id=ipAliasAddr width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="ipAliasAddress"> IP Address : </td>
                      <td><input name="lan2Ip" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan2_ipaddr"); %>" /></td>
                    </tr>
                  </table>
                  <input type=hidden name="lan1Ip" value="<% getCfgGeneral(1, "lan_ipaddr"); %>">
                </li>     
                <li class="w_text">
                  <table id=ipAliasNetmask width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="ipAliasSubnet"> 	IP Subnet Mask :</td>
                      <td><input name="lan2Netmask" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan2_netmask"); %>" /></td>
                    </tr>
                  </table>
                </li>
                <!--
                <li class="title"> IP Alias 2 </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input name="WAN_IP_WanToLan2" type="checkbox" />
                        IP Alias 2 </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"> IP Address : </td>
                      <td><input onblur="checkIPFormat(this)" maxlength="15" size="15" 
                        value="0.0.0.0" name="GeneralFirDNSAddr233" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"> IP Address : </td>
                      <td><input onblur="checkIPFormat(this)" maxlength="15" size="15" 
                        value="0.0.0.0" name="GeneralFirDNSAddr234" /></td>
                    </tr>
                  </table>
                </li>
                -->
                <li></li>
				<li></li>
              </ul>
            </div>
        </li>
        <center>
        <li class="table_button">
        <input type=submit value="Apply" id="ipAliasApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="ipAliasReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/ip_alias.asp" name="ip_alias_url">
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
