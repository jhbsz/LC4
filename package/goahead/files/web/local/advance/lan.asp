<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var opmode = '<% getCfgZero(1, "OperationMode"); %>';
var lanIpmode = '<% getCfgZero(1, "lanIp_mode"); %>';

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
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

function checkLanGatewayIpAddr(field, ismask)
{
	if (field.value == "0.0.0.0") {
	    return true;
	}
	if (field.value == "") {
	    field.value = "0.0.0.0";
	    return true;
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

function lanIp_mode_switch()
{
    if (document.lanCfg.lanIpMode[0].checked == true)
	{		
		document.lanCfg.lanIpApMode.disabled = true;
	    document.lanCfg.lanNetmaskApMode.disabled = true;
        document.lanCfg.lanGatewayApMode.disabled = true;
	}
	else
	{
		document.lanCfg.lanIpApMode.disabled = false;
	    document.lanCfg.lanNetmaskApMode.disabled = false;
        document.lanCfg.lanGatewayApMode.disabled = false;
	}

}

function DNS1TypeSwitch()
{
   if ((document.lanCfg.dns1Type.options.selectedIndex == 0) ||
       (document.lanCfg.dns1Type.options.selectedIndex == 2))
   {
        document.lanCfg.staticPriDns.disabled = true;
        //document.lanCfg.staticPriDns.value = "0.0.0.0";
   }
   else
   {
        document.lanCfg.staticPriDns.disabled = false;
   }
}

function DNS2TypeSwitch()
{
   if ((document.lanCfg.dns2Type.options.selectedIndex == 0) ||
       (document.lanCfg.dns2Type.options.selectedIndex == 2))
   {
        document.lanCfg.staticSecDns.disabled = true;
        //document.lanCfg.staticSecDns.value = "0.0.0.0";
   }
   else
   {
        document.lanCfg.staticSecDns.disabled = false;
   }
}

function DnsUpdate()
{
      if (isAllNum(document.lanCfg.staticPriDns.value) == 0) //dns1 not all numbers
      {
          if ((document.lanCfg.staticSecDns.value != "" ) &&
              (isAllNum(document.lanCfg.staticSecDns.value) == 1)) //dns2 all numbers
          {
             document.lanCfg.staticPriDns.value=document.lanCfg.staticSecDns.value;
             document.lanCfg.staticSecDns.value="";
          }
          else
          {
            document.lanCfg.staticPriDns.value="";
            document.lanCfg.staticSecDns.value="";
          }  
      }
      else
      {
          if ((document.lanCfg.staticSecDns.value != "" ) &&
              (isAllNum(document.lanCfg.staticSecDns.value) == 0)) //dns2 not all numbers
          {
             document.lanCfg.staticSecDns.value="";
          }
      }     
  
  if (document.lanCfg.dns1Type.options.selectedIndex == 2)
  {
      if (document.lanCfg.dns2Type.options.selectedIndex == 2)
      {
           document.lanCfg.staticSecDns.value="";
      }
      else
      {
          if ((document.lanCfg.staticPriDns.value != "" ) &&
              (isAllNum(document.lanCfg.staticPriDns.value) == 1)) //dns1 all numbers
          {
             document.lanCfg.staticSecDns.value=document.lanCfg.staticPriDns.value;
          }
      }
      document.lanCfg.staticPriDns.value="";   
  }
  if (document.lanCfg.dns2Type.options.selectedIndex == 2)
  {
      document.lanCfg.staticSecDns.value="";
  }
}

function updateStatus()
{
   DNS1TypeSwitch();
   DNS2TypeSwitch();
   DnsUpdate();
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

function CheckValue()
{   
    if (opmode == "0" || opmode == "6") //Router and WISP mode
    {

        if (!checkIpAddr(document.lanCfg.lanIp, false))
		    return false;
	    if (!checkIpAddr(document.lanCfg.lanNetmask, true))
		    return false;
	    if (!checkNetMask(document.lanCfg.lanNetmask))
				return false;
	    	
	    document.lanCfg.dhcpGateway.value = document.lanCfg.lanIp.value;
	    document.lanCfg.dhcpNetmask.value = document.lanCfg.lanNetmask.value; //aron add 2009.05.25
	    
	    //-------- aron add 2009.02.25
	    d1 = atoi(document.lanCfg.lanIp.value, 1);
        d2 = atoi(document.lanCfg.lanIp.value, 2);
        d3 = atoi(document.lanCfg.lanIp.value, 3);
        d_start_part4 = atoi(document.lanCfg.dhcpStart.value, 4);
        d_end_part4 = atoi(document.lanCfg.dhcpEnd.value, 4);
    
        dhcp_start_ip = d1 + "." + d2 +"." + d3 + "." + d_start_part4;
        dhcp_end_ip = d1 + "." + d2 +"." + d3 + "." + d_end_part4;
        //alert(dhcp_start_ip);
        //alert(dhcp_end_ip); 
        document.lanCfg.dhcpStart.value = dhcp_start_ip;
        document.lanCfg.dhcpEnd.value = dhcp_end_ip;
        //----------------------------------------------
    }
    else if(opmode == "1" || opmode == "5") //AP and Universal Repeater mode
    {
        if (document.lanCfg.lanIpMode[1].checked == true) //fixed IP
	    {			
			if ((document.lanCfg.dns1Type.options.selectedIndex == 0) ||
                 (document.lanCfg.dns2Type.options.selectedIndex == 0))
            {
                alert('LAN is Fixed IP, please set DNS Assignment to User-Defined or None.');
                return false;
            }
			if (!checkIpAddr(document.lanCfg.lanIpApMode, false))
		        return false;
	        if (!checkIpAddr(document.lanCfg.lanNetmaskApMode, true))
		        return false;
		    //if (!checkIpAddr(document.lanCfg.lanGatewayApMode, false))
		    if (!checkLanGatewayIpAddr(document.lanCfg.lanGatewayApMode, false))
		        return false;
		    if (!checkNetMask(document.lanCfg.lanNetmaskApMode))
				return false;
	    }
	    //DNS
        if (document.lanCfg.dns1Type.options.selectedIndex == 1) //DNS1 User-Defined
        {
            if(document.lanCfg.staticPriDns.value == "")
            {
                alert('First DNS Server should not be empty with User-Defined.');
                return false;
            }
            if(document.lanCfg.staticPriDns.value == "0.0.0.0")
            {
                alert('First DNS Server should not be 0.0.0.0, please give DNS IP Address.');
                return false;
            }
            if (!checkIpAddr(document.lanCfg.staticPriDns, false))
				return false;
        }
        if (document.lanCfg.dns2Type.options.selectedIndex == 1)
        {
            if(document.lanCfg.staticSecDns.value == "")
            {
                alert('Second DNS Server should not be empty with User-Defined.');
                return false;
            }
            if(document.lanCfg.staticSecDns.value == "0.0.0.0")
            {
                alert('Second DNS Server should not be 0.0.0.0, please give DNS IP Address.');
                return false;
            }
            if (!checkIpAddr(document.lanCfg.staticSecDns, false))
				return false;
        } 
        
        /*
        document.lanCfg.DhcpGateway.value = document.lanCfg.lanIpApMode.value;
	    d1 = atoi(document.lanCfg.lanIpApMode.value, 1);
        d2 = atoi(document.lanCfg.lanIpApMode.value, 2);
        d3 = atoi(document.lanCfg.lanIpApMode.value, 3);
        d_start_part4 = atoi(document.lanCfg.DhcpStart.value, 4);
        d_end_part4 = atoi(document.lanCfg.DhcpEnd.value, 4);
    
        dhcp_start_ip = d1 + "." + d2 +"." + d3 + "." + d_start_part4;
        dhcp_end_ip = d1 + "." + d2 +"." + d3 + "." + d_end_part4; 
        document.lanCfg.DhcpStart.value = dhcp_start_ip;
        document.lanCfg.DhcpEnd.value = dhcp_end_ip;
        */
    }
    showWebMessage(2, "");
    return true;
}

function initTranslation(){
	var e = document.getElementById("IP_Address");
	e.innerHTML = _("ipAlias Address");
	e = document.getElementById("IP_Subnet_Mask");
	e.innerHTML = _("ipAlias Subnet");
	e = document.getElementById("lanApply");
	e.value = _("ipAlias Apply");
	e = document.getElementById("lanReset");
	e.value = _("ipAlias Reset");
}

function initValue()
{
		initTranslation();

    //var opmode = "<% getCfgZero(1, "OperationMode"); %>";
    var dns1_type = "<% getCfgGeneral(1, "wan_dns1_type"); %>";
    var dns2_type = "<% getCfgGeneral(1, "wan_dns2_type"); %>";

    show_div(false, "routing_setting");
    show_div(false, "ap_setting");
        
    if (opmode == "0" || opmode == "6") //Router and WISP mode
    {
        show_div(true, "routing_setting");
    }
    else if(opmode == "1" || opmode == "5") //AP and Universal Repeater mode
    {
        show_div(true, "ap_setting");
        if (lanIpmode == "0") //LAN IP get from DHCP Server

        {     
	        document.lanCfg.lanIpMode[0].checked = true;
	        document.lanCfg.lanIpApMode.disabled = true;
	        document.lanCfg.lanNetmaskApMode.disabled = true;
            document.lanCfg.lanGatewayApMode.disabled = true;
	    }
        else //use fixed LAN IP
        {
            document.lanCfg.lanIpMode[1].checked = true;
            document.lanCfg.lanIpApMode.disabled = false;
	        document.lanCfg.lanNetmaskApMode.disabled = false;
            document.lanCfg.lanGatewayApMode.disabled = false;
        }
        
        //for dns1 and dns2 types
        if (dns1_type == "ISP")
        {
            document.lanCfg.dns1Type.options.selectedIndex = 0;
        }
        else if (dns1_type == "USER")
        {
            document.lanCfg.dns1Type.options.selectedIndex = 1;
        }
        else if (dns1_type == "NONE")
        {   
            document.lanCfg.dns1Type.options.selectedIndex = 2;

        }    
        if (dns2_type == "ISP")
        {
            document.lanCfg.dns2Type.options.selectedIndex = 0;
        }
        else if (dns2_type == "USER")
        {
            document.lanCfg.dns2Type.options.selectedIndex = 1;
        }
        else if (dns2_type == "NONE")
        {   
            document.lanCfg.dns2Type.options.selectedIndex = 2;
        }        
        updateStatus();
    }
//    parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
}

</script>
</head>



<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLanIP" onSubmit="return CheckValue()">

    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title">LAN TCP/IP </li>
       <span id = "routing_setting" class = "off" >
       <!-- for Routing Mode (BEGIN) -->
                <li class="w_text">
                  <table id=LanIPAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=IP_Address>IP Address :</td>
                      <td><input name="lanIp" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_ipaddr"); %>" /></td>
                    </tr>
                  </table>
                  <input type=hidden name="dhcpGateway">
                  <input type=hidden name="dhcpNetmask">
                  <input type=hidden name="dhcpStart" value="<% getCfgGeneral(1, "dhcpStart"); %>"></tr>
                  <input type=hidden name="dhcpEnd" value="<% getCfgGeneral(1, "dhcpEnd"); %>"></tr>
                </li> 
                <li class="w_text">
                  <table id=LanIPNetMask width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=IP_Subnet_Mask>IP Subnet Mask :</td>
                      <td><input name="lanNetmask" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_netmask"); %>" /></td>
                    </tr>
                  </table>
                </li>
       <!-- for Routing Mode (END)-->
       </span>
       <span id = "ap_setting" class = "off" >
       <!-- for AP Mode (BEGIN) -->
                <li class="w_text">
                  <table id="lanIp_client" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wlanIpMode"><input type="radio" value="0" onclick="lanIp_mode_switch()" name="lanIpMode" />Get from DHCP Server </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="lanIp_fixed" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wlanIpMode"><input type="radio" value="1" onclick="lanIp_mode_switch()"
                        name="lanIpMode" />Use Defined LAN IP Address </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=LanIPAddrApMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">IP Address :</td>
                      <!--<td><input name="lanIpApMode" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_ipaddr"); %>" /></td>-->
                        <td><input name="lanIpApMode" maxlength=15 size=15 
                        value="<% getLanIp(); %>" /></td>
                    </tr>
                  </table>
                  <input type=hidden name="DhcpGateway">
                  <input type=hidden name="DhcpStart" value="<% getCfgGeneral(1, "dhcpStart"); %>"></tr>
                  <input type=hidden name="DhcpEnd" value="<% getCfgGeneral(1, "dhcpEnd"); %>"></tr>
                </li> 
                <li class="w_text">
                  <table id=LanIPNetMaskApMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">IP Subnet Mask :</td>
                      <!--<td><input name="lanNetmaskApMode" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_netmask"); %>" /></td>-->
                        <td><input name="lanNetmaskApMode" maxlength=15 size=15 
                        value="<% getLanNetmask(); %>" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=LanGatewayApMode width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">Gateway IP Address :</td>
                      <!--<td><input name="lanGatewayApMode" maxlength=15 size=15 
                        value="<% getCfgGeneral(1, "lan_gateway"); %>" /></td>-->
                        <td><input name="lanGatewayApMode" maxlength=15 size=15 
                        value="<% getLanIfGateway(); %>" /></td>
                    </tr>
                  </table>
                </li> 
                
                
                <li class="space3"></li>
                <li class="title">DNS Assignment </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">First DNS Server : </td>
                      <td><select name="dns1Type" size="1" onChange="DNS1TypeSwitch();">
                          <option value="ISP">From ISP</option>
                          <option value="USER">User-Defined</option>
                          <option value="NONE">None</option>
                        </select>
                          <!--<input name="staticPriDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_primary_dns"); %>"></td>-->
                          <input name="staticPriDns" maxlength=15 size=15 value="<% getDns(1); %>"></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">Second DNS Server : </td>
                      <td><select name="dns2Type" size="1" onChange="DNS2TypeSwitch();">
                          <option value="ISP">From ISP</option>
                          <option value="USER">User-Defined</option>
                          <option value="NONE">None</option>
                        </select>
                          <!--<input name="staticSecDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_secondary_dns"); %>"></td>-->
                          <input name="staticSecDns" maxlength=15 size=15 value="<% getDns(2); %>"></td>
                    </tr>
                  </table>
                </li> 
       <!-- for AP Mode (END)-->
       </span>
                <li></li>
				<li></li>
              </ul>
            </div>
        </li>
        <center>
        <li class="table_button">
        <input type=submit value="Apply" id="lanApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="lanReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/lan.asp" name="lan_url">
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
