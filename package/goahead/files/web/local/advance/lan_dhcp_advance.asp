<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/tablet.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var dhcpMode = '<% getCfgZero(1, "dhcpEnabled"); %>';

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

function DNS1TypeSwitch()
{
   if ((document.lanCfg.dhcpDns1Type.options.selectedIndex == 0) ||
       (document.lanCfg.dhcpDns1Type.options.selectedIndex == 2) ||
       (document.lanCfg.dhcpDns1Type.options.selectedIndex == 3))
   {
        document.lanCfg.dhcpPriDns.disabled = true;
        document.lanCfg.dhcpPriDns.value = "0.0.0.0";
   }
   else //User-Defined
   {
        document.lanCfg.dhcpPriDns.disabled = false;
   }
}

function DNS2TypeSwitch()
{
   if ((document.lanCfg.dhcpDns2Type.options.selectedIndex == 0) ||
       (document.lanCfg.dhcpDns2Type.options.selectedIndex == 2) ||
       (document.lanCfg.dhcpDns2Type.options.selectedIndex == 3))
   {
        document.lanCfg.dhcpSecDns.disabled = true;
        document.lanCfg.dhcpSecDns.value = "0.0.0.0";
   }
   else //User-Defined
   {
        document.lanCfg.dhcpSecDns.disabled = false;
   }
}

function CheckValue()
{
   if (dhcpMode != "1") //DHCP Server not enabled
   { 
         alert('Please enable DHCP Server firstly before using LAN Static DHCP Table or DNS Server.');
         return false;
   } 
   
   if((document.lanCfg.dhcpStaticMac1.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac2.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac3.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac4.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac5.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac6.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac7.value != "00:00:00:00:00:00") ||
      (document.lanCfg.dhcpStaticMac8.value != "00:00:00:00:00:00"))
   {   
      if (dhcpMode != "1") //DHCP Server not enabled
      { 
         alert('Please enable DHCP Server before using LAN Static DHCP Table.');
         return false;
      }
   }   
  
   //Static1
   if (document.lanCfg.dhcpStaticMac1.value.length == 0) {
   			    document.lanCfg.dhcpStaticMac1.value = document.lanCfg.dhcpStaticMac1.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac1.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac1.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac1.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp1, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp1.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp1.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp1.value = "0.0.0.0";
   }    
   //Static2
   if (document.lanCfg.dhcpStaticMac2.value.length == 0) {
			    document.lanCfg.dhcpStaticMac2.value = document.lanCfg.dhcpStaticMac2.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac2.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac2.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac2.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp2, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp2.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp2.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp2.value = "0.0.0.0";
   }
   //Static3
   if (document.lanCfg.dhcpStaticMac3.value.length == 0) {
			    document.lanCfg.dhcpStaticMac3.value = document.lanCfg.dhcpStaticMac3.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac3.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac3.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac3.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp3, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp3.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp3.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp3.value = "0.0.0.0";
   }
   //Static4
   if (document.lanCfg.dhcpStaticMac4.value.length == 0) {
			    document.lanCfg.dhcpStaticMac4.value = document.lanCfg.dhcpStaticMac4.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac4.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac4.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac4.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp4, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp4.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp4.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp4.value = "0.0.0.0";
   }
   //Static5
   if (document.lanCfg.dhcpStaticMac5.value.length == 0) {
			    document.lanCfg.dhcpStaticMac5.value = document.lanCfg.dhcpStaticMac5.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac5.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac5.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac5.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp5, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp5.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp5.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp5.value = "0.0.0.0";
   }
   //Static6
   if (document.lanCfg.dhcpStaticMac6.value.length == 0) {
			    document.lanCfg.dhcpStaticMac6.value = document.lanCfg.dhcpStaticMac6.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac6.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac6.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac6.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp6, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp6.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp6.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp6.value = "0.0.0.0";
   }
   //Static7
   if (document.lanCfg.dhcpStaticMac7.value.length == 0) {
			    document.lanCfg.dhcpStaticMac7.value = document.lanCfg.dhcpStaticMac7.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac7.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac7.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac7.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp7, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp7.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp7.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp7.value = "0.0.0.0";
   }
   //Static8
   if (document.lanCfg.dhcpStaticMac8.value.length == 0) {
			    document.lanCfg.dhcpStaticMac8.value = document.lanCfg.dhcpStaticMac8.defaultValue;
   }
   if(document.lanCfg.dhcpStaticMac8.value != "00:00:00:00:00:00")
   {
            var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			
			if (!re.test(document.lanCfg.dhcpStaticMac8.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.lanCfg.dhcpStaticMac8.focus();
				return false;
			}
			if (!checkIpAddr(document.lanCfg.dhcpStaticIp8, false))
		    return false;
		    
		    if ( !checkSubnet(document.lanCfg.lanIP.value, document.lanCfg.lanMask.value, document.lanCfg.dhcpStaticIp8.value)) 
		    {
        		//alert('Invalid DHCP start address!\nIt should be located in the same subnet of current LAN IP address.');
        		alert('Invalid IP address! It should be located in the same subnet of current LAN IP address.');
        		document.lanCfg.dhcpStaticIp8.focus();
        		return false;
                    }
   }
   else
   {
       document.lanCfg.dhcpStaticIp8.value = "0.0.0.0";
   }
   
    //DNS
     if (document.lanCfg.dhcpDns1Type.options.selectedIndex == 1) //DNS1 User-Defined
     {
         if(document.lanCfg.dhcpPriDns.value == "")
         {
            alert('First DNS Server should not be empty with User-Defined.');
            return false;
         }
         if(document.lanCfg.dhcpPriDns.value == "0.0.0.0")
         {
            alert('First DNS Server should not be 0.0.0.0, please give DNS IP Address.');
            return false;
         }
         if (!checkIpAddr(document.lanCfg.dhcpPriDns, false))
				return false;
     }
     if (document.lanCfg.dhcpDns2Type.options.selectedIndex == 1)
     {
         if(document.lanCfg.dhcpSecDns.value == "")
         {
            alert('Second DNS Server should not be empty with User-Defined.');
            return false;
         }
         if(document.lanCfg.dhcpSecDns.value == "0.0.0.0")
         {
            alert('Second DNS Server should not be 0.0.0.0, please give DNS IP Address.');
            return false;
         }
         if (!checkIpAddr(document.lanCfg.dhcpSecDns, false))
				 return false;
     }
   
   showWebMessage(2, "");
   return true;
}

function updateStatus()
{
     DNS1TypeSwitch();
     DNS2TypeSwitch();

}

function initValue()
{
   var dns1_type = "<% getCfgGeneral(1, "dhcp_dns1_type"); %>";
   var dns2_type = "<% getCfgGeneral(1, "dhcp_dns2_type"); %>";
   
   //for dns1 and dns2 types
   if (dns1_type == "ISP")
   {
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 0;
   }
   else if (dns1_type == "USER")
   {
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 1;
   }
   else if (dns1_type == "RELAY")
   {   
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 2;
   } 
   else if (dns1_type == "NONE")
   {   
       document.lanCfg.dhcpDns1Type.options.selectedIndex = 3;
   }    
   if (dns2_type == "ISP")
   {
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 0;
   }
   else if (dns2_type == "USER")
   {
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 1;
   }
   else if (dns2_type == "RELAY")
   {   
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 2;
   }   
   else if (dns2_type == "NONE")
   {   
       document.lanCfg.dhcpDns2Type.options.selectedIndex = 3;
   }        

      var e = document.getElementById("lanAdvtitle");
	e.innerHTML = _("lanAdv title");
	e = document.getElementById("lanAdvMAC");
	e.innerHTML = _("lanAdv MAC");
	e = document.getElementById("lanAdvIP");
	e.innerHTML = _("lanAdv IP");
	e = document.getElementById("lanAdvDNS");
	e.innerHTML = _("lanAdv DNS");
		e = document.getElementById("lanAdvDNStitle");
	e.innerHTML = _("lanAdvDNS title");
		e = document.getElementById("lanAdvDNSServer1");
	e.innerHTML = _("lanAdvDNS Server1");
		e = document.getElementById("lanAdvDNSServer1ISP");
	e.innerHTML = _("lanAdv DNSServer1ISP");
			e = document.getElementById("lanAdvDNSServer1Defined");
	e.innerHTML = _("lanAdv DNSServer1Defined");
			e = document.getElementById("lanAdvDNSServer1Relay");
	e.innerHTML = _("lanAdv DNSServer1Relay");
			e = document.getElementById("lanAdvDNSServer1None");
	e.innerHTML = _("lanAdv DNSServer1None");
			e = document.getElementById("lanAdvDNSServer2");
	e.innerHTML = _("lanAdv DNSServer2");
			e = document.getElementById("lanAdvDNSServer2ISP");
	e.innerHTML = _("lanAdv DNSServer2ISP");
			e = document.getElementById("lanAdvDNSServer2Defined");
	e.innerHTML = _("lanAdv DNSServer2Defined");
			e = document.getElementById("lanAdvDNSServer2Relay");
	e.innerHTML = _("lanAdv DNSServer2Relay");
			e = document.getElementById("lanAdvDNSServer2None");
	e.innerHTML = _("lanAdv DNSServer2None");
	e = document.getElementById("lan_static_dhcp_table");
	e.innerHTML = _("lan static dhcp table");
        
	
	e = document.getElementById("lanAdvApply");
	e.value = _("lanAdv Apply");
	e = document.getElementById("lanAdvReset");
	e.value = _("lanAdv Reset");



   updateStatus();
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLan_StaicLease_Dns" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
				<li class="title" id="lanAdvtitle">LAN Static DHCP Table</li>
				<li></li>
				<li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" cellpadding="0" cellspacing="0" >
                          
                          <tr>
                            <td height="26" colspan="10"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                  <td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
                                  <td background="images/table_top_center.gif" class="table_title" id="lan_static_dhcp_table"> LAN Static DHCP Table </td>
                                  <td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
                                </tr>
                            </table></td>
                          </tr>
                          
                          <tr >
                            <td width="5%"><center>
                              <span class="top_left">#</span>
                            </center>                            </td>
                            <td width="46%"><center>
                              <span class="top_font" id="lanAdvMAC">MAC Address</span>
                            </center>                            </td>
                            <!--<td width="20%"><center>
                              <span class="top_font">Direction</span>
                            </center>                            </td>-->
                            <td width="46%"><center>
                              <span class="top_right" id="lanAdvIP">IP Address </span>
                            </center>                            </td>
                            <!--<td width="15%"><center>
                              <span class="top_font">Priority</span>
                            </center>                            </td>
                            <td width="15%"><center>
                              <span class="top_right">Modify </span>
                            </center>                            </td>-->
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">1</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac1" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac1"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp1" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp1"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">2</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac2" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac2"); %>" />
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp2" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp2"); %>" />
                                </span>
                            </center>                            </td>
                          </tr>                            
                          <tr >
                            <td><center>
                              <span class="table_left">3</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac3" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac3"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp3" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp3"); %>" />
                                </span>
                            </center>                            </td>
                          </tr>                          
                          <tr >
                            <td><center>
                              <span class="table_left">4</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac4" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac4"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp4" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp4"); %>" />
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">5</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac5" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac5"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp5" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp5"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">6</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac6" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac6"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp6" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp6"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">7</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac7" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac7"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp7" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp7"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr >
                            <td><center>
                              <span class="table_left">8</span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_font">
                                <input type="text" name="dhcpStaticMac8" size="17" maxlength="17" value="<% getCfgGeneral(1, "dhcpStaticMac8"); %>" /> 
                                </span>
                            </center>                            </td>
                            <td><center>
                              <span class="table_right">
                                <input type="text" name="dhcpStaticIp8" size="15" maxlength="15" value="<% getCfgGeneral(1, "dhcpStaticIp8"); %>" /> 
                                </span>
                            </center>                            </td>
                          </tr>
                          <tr><input type=hidden name="lanIP" value="<% getCfgGeneral(1, "lan_ipaddr"); %>"></tr>
                          <tr><input type=hidden name="lanMask" value="<% getCfgGeneral(1, "lan_netmask"); %>"></tr>
                          
                          <tr>
                            <td height="5" colspan="10"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
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
			    <li class="space3"></li>
                <li class="title"  id="lanAdvDNS" >DNS Server</li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="lanAdvDNStitle">DNS Servers Assigned by DHCP Server </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="lanAdvDNSServer1">First DNS Server : </td>
                      <td><select name="dhcpDns1Type" size="1" onChange="DNS1TypeSwitch();">
                          <option value="ISP" id="lanAdvDNSServer1ISP">From ISP</option>
                          <option value="USER" id="lanAdvDNSServer1Defined">User-Defined</option>
                          <option value="RELAY" id="lanAdvDNSServer1Relay">DNS Relay</option>
                          <option value="NONE" id="lanAdvDNSServer1None">None</option>
                        </select>
                          <input name="dhcpPriDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "dhcpPriDns"); %>"></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="lanAdvDNSServer2">Second DNS Server : </td>
                      <td><select name="dhcpDns2Type" size="1" onChange="DNS2TypeSwitch();">
                          <option value="ISP" id="lanAdvDNSServer2ISP">From ISP</option>
                          <option value="USER" id="lanAdvDNSServer2Defined">User-Defined</option>
                          <option value="RELAY" id="lanAdvDNSServer2Relay">DNS Relay</option>
                          <option value="NONE" id="lanAdvDNSServer2None">None</option>
                        </select>
                          <input name="dhcpSecDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "dhcpSecDns"); %>"></td>
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
        <input type=submit value="Apply" id="lanAdvApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="lanAdvReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/lan_dhcp_advance.asp" name="lan_dhcp_advance_url">
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
