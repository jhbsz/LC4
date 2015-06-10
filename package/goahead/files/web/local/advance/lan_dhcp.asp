<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
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

var dhcpMode = '<% getCfgZero(1, "dhcpEnabled"); %>';
var dhcpStartIP = '<% getCfgGeneral(1, "dhcpStart"); %>';

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

function dhcp_mode_switch()
{
    if (document.lanCfg.lanDhcpType.checked == true)
	{		
		document.lanCfg.dhcpStart.disabled = false;
	    document.lanCfg.dhcpPoolSize.disabled = false;
    }
	else
	{
	    document.lanCfg.dhcpStart.disabled = true;
	    document.lanCfg.dhcpPoolSize.disabled = true;
	}

}

function dhcpMode_Click()
{
	document.lanCfg.lanDhcpType.value = document.lanCfg.lanDhcpType.checked ? "1": "0";
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

function checkClientRange(gw,start,end)
{
	gw_d = atoi(gw,4);
	gw_d += atoi(gw,3)*256;
  	gw_d += atoi(gw,2)*256;
  	gw_d += atoi(gw,1)*256;

  	start_d = atoi(start,4);
  	start_d += atoi(start,3)*256;
  	start_d += atoi(start,2)*256;
  	start_d += atoi(start,1)*256;

  	end_d = atoi(end,4);
  	end_d += atoi(end,3)*256;
  	end_d += atoi(end,2)*256;
  	end_d += atoi(end,1)*256;
  	if( start_d <= end_d ){
		if((start_d == gw_d) || (gw_d == end_d))
			return false;
		else
			return true;
	}

  	return false;
}

function CheckValue()
{ 
    d1 = atoi(document.lanCfg.dhcpStart.value, 1);
    d2 = atoi(document.lanCfg.dhcpStart.value, 2);
    d3 = atoi(document.lanCfg.dhcpStart.value, 3);
    d4 = atoi(document.lanCfg.dhcpStart.value, 4);
    d5 = d4 + parseInt(document.lanCfg.dhcpPoolSize.value,10)-1;
    
    dhcp_end_ip = d1 + "." + d2 +"." + d3 + "." + d5;
    //alert(dhcp_end_ip); 
    document.lanCfg.dhcpEnd.value = dhcp_end_ip;
   
    if (document.lanCfg.lanDhcpType.checked == true) 
    {

        if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStart.value)) {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid DHCP start address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStart.value = document.lanCfg.dhcpStart.defaultValue;
        		document.lanCfg.dhcpStart.focus();
        		return false;
        }
        if (!checkIpAddr(document.lanCfg.dhcpStart, false))
				return false;
        if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpEnd.value)) {
        		//alert('Invalid DHCP end address with pool size allocated!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid DHCP end address with pool size allocated! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpPoolSize.value = document.lanCfg.dhcpPoolSize.defaultValue;
        		document.lanCfg.dhcpPoolSize.focus();
        		return false;
        }
        if ( !checkClientRange(document.lanCfg.lanIP.value, document.lanCfg.dhcpStart.value, document.lanCfg.dhcpEnd.value) ) {
        		alert('Invalid DHCP address range!');
        		document.lanCfg.dhcpStart.focus();
        		return false;
        }
        if(d5 > 254 )
        {
            alert('Invalid DHCP address range!');
            return false;
        }
    }
    dhcpMode_Click();
    document.lanCfg.dhcpGateway.value = document.lanCfg.lanIP.value;
    document.lanCfg.dhcpStarting.value = d4;
    showWebMessage(2, "");
    return true;
}

function updateStatus()
{
        if (dhcpMode == "1") //DHCP Server enabled
        {     
	        document.lanCfg.lanDhcpType.checked = true;
	        document.lanCfg.dhcpStart.disabled = false;
	        document.lanCfg.dhcpPoolSize.disabled = false;
	    }
        else
        {
            document.lanCfg.lanDhcpType.checked = false;
            document.lanCfg.dhcpStart.disabled = true;
	        document.lanCfg.dhcpPoolSize.disabled = true;
        }
        
        
        d1 = atoi(document.lanCfg.lanIP.value, 1);
        d2 = atoi(document.lanCfg.lanIP.value, 2);
        d3 = atoi(document.lanCfg.lanIP.value, 3);
        d4 = dhcpStartIP;
        var startIpAddr = d1 + "." + d2 +"." + d3 + "." + d4;
        document.lanCfg.dhcpStart.value = startIpAddr;
        document.lanCfg.dhcpStart.defaultValue = startIpAddr; //aron add for user input check
}

function initValue()
{
      var e = document.getElementById("lanDhcptitle");
	e.innerHTML = _("LanDhcp title");
	e = document.getElementById("LanDhcpEnable");
	e.innerHTML = _("LanDhcp Enable");
	e = document.getElementById("lanDhcpIP");
	e.innerHTML = _("LanDhcp IP");
	e = document.getElementById("lanDhcpPool");
	e.innerHTML = _("LanDhcp PoolSize");
	
	e = document.getElementById("lanDhcpApply");
	e.value = _("lanDhcp Apply");
	e = document.getElementById("lanDhcpReset");
	e.value = _("lanDhcp Reset");
	
	
   updateStatus();
}
</script>
</head>

<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLanDhcp" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="lanDhcptitle">LAN DHCP Setup</li>
                <li class="w_text">
                  <table id=LanDhcpType width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%"><input type="checkbox" name="lanDhcpType" value="<% getCfgZero(1, "dhcpEnabled"); %>" onclick="dhcp_mode_switch()" />
                       <td id="LanDhcpEnable">Enable DHCP Server  </td> </td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <!--  
                <li class="w_text">
                  <table id=DhcpStartIp width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="19%" id="lanDhcpIP"><span class="cfig_word">
                        IP Pool Starting Address</td>
                        <td><input name="dhcpStart" maxlength=15 size=15 />
                      </span></td>
                      <td width="8%" id="lanDhcpPool">Pool Size </td>
                      <td><input type="text" name="dhcpPoolSize" size="3" maxlength="3" value="<% getCfgGeneral(1, "lan_dhcp_pool_size"); %>" /></td>
                    </tr>
                    <tr><input type=hidden name="dhcpEnd"></tr>
                    <tr><input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>"></tr>
                    <tr><input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>"></tr>
                    <tr><input type=hidden name="dhcpGateway"></tr>
                    <tr><input type=hidden name="dhcpStarting"></tr>
                  </table>
                </li>
                -->
                <li class="w_text">
                  <table id=DhcpStartIp width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="lanDhcpIP"><span class="cfig_word">
                        IP Pool Starting Address</td>
                        <td><input name="dhcpStart" maxlength=15 size=15 />
                      </span></td>
                    </tr>
                    <tr><input type=hidden name="dhcpEnd"></tr>
                    <tr><input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>"></tr>
                    <tr><input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>"></tr>
                    <tr><input type=hidden name="dhcpGateway"></tr>
                    <tr><input type=hidden name="dhcpStarting"></tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=DhcpStartIp width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="lanDhcpPool">Pool Size </td>
                      <td><input type="text" name="dhcpPoolSize" size="3" maxlength="3" value="<% getCfgGeneral(1, "lan_dhcp_pool_size"); %>" /></td>
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
        <input type=submit value="Apply" id="lanDhcpApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="lanDhcpReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/lan_dhcp.asp" name="lan_dhcp_url">
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
