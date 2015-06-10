<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
</style>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var dnsType = "<% getCfgZero(1, "dnsType"); %>";
var PppoeIPv6_iptype = "<% getCfgZero(1, "PppoeIPv6_iptype"); %>";


function submit_apply()
{

	if ((wirelessmode == 2) || (wirelessmode == 6)){
		alert("You CANNOT select WISP or Client mode to configure this page");	
		return false;
	}

	return true;
}

function style_display_on()
{
	if (window.ActiveXObject)
	{ // IE
		return "block";
	}
	else if (window.XMLHttpRequest)
	{ // Mozilla, Safari,...
		return "table-row";
	}
}

function show_div(show,id)
{
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function string_count (stack, needle, offset, length)
{
	var pos = 0, cnt = 0;

	stack += '';
	needle += '';
	if (isNaN(offset)) {offset = 0;}
	if (isNaN(length)) {length = 0;}
	offset--;

	while ((offset = stack.indexOf(needle, offset+1)) != -1){
		if (length > 0 && (offset+needle.length) > length){
			return false;
		} else{
			cnt++;
		}
	}

	return cnt;
}

function validate_ipv4(addr)
{
alert("addr ipv4="+addr);
	var match = addr.match(/(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|255[0-5])\.){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])/);
alert("match v4="+match);
	return match != null;
}

function validate_ipv6addr(addr)
{
alert("addr="+addr);
  // Test for empty address
	if (addr.length<3)
	{
alert("return addr="+addr);
		return addr == "::";
		
	}

  // Check if part is in IPv4 format
	if (addr.indexOf('.')>0)
	{
		lastcolon = addr.lastIndexOf(':');
alert("lastcolon="+lastcolon);
		if (!(lastcolon && validate_ipv4(addr.substr(lastcolon + 1)))){
			alert("Invlaid address for . with v4!");
			return false;
		}
        // replace IPv4 part with dummy
		addr = addr.substr(0, lastcolon) + ':0:0';
alert("addr .>0=="+addr);
	} 

  // Check uncompressed
	if (addr.indexOf('::')< 0)
	{
		var match = addr.match(/^(?:[a-f0-9]{1,4}:){7}[a-f0-9]{1,4}$/i);
alert("match<0="+match);
		if(match == null){
			alert("Invalid address for ::< 0!!");
			return false;
		}
		else 
		return match != null;  
	}

  // Check colon-count for compressed format
	if (string_count(addr, ':') < 8)
	{
		var match = addr.match(/^(?::|(?:[a-f0-9]{1,4}:)+):(?:(?:[a-f0-9]{1,4}:)*[a-f0-9]{1,4})?$/i);
alert("match<8="+match);
		if(match == null){
			alert("Invalid address for :< 8!!");
			return false;
			}
		else 
		return match != null;  
	}
	 
 // special case with ending or starting double colon
	if (string_count(addr, ':') == 8)
	{

		var match = addr.match(/\A(?:::)?(?:[a-f0-9]{1,4}:){6}[a-f0-9]{1,4}(?:::)?$/i);
alert("match=0="+match);
		if(match == null){
			alert("Invalid address for := 8!!");
  // Not a valid IPv6 address
			return false;
		}
		else 
		return match != null;         
	}
  
//if(match == null)
//alert("Invalid address!!");
  // Not a valid IPv6 address
//  return false;
}



function connectionTypeSwitch()
{
    show_div(false, "PPPoE_setting");
	show_div(false, "DHCP_setting");
	show_div(false, "Linklocal_setting");
	show_div(false, "Ethernet_setting");
	show_div(false, "Stateful_setting0_static");
	show_div(false, "Stateful_setting1_pppoe");
	show_div(false, "Stateful_setting2_dhcp");	
	show_div(false, "Autoconfiguration");
	show_div(false, "wandhcpv6type_setting");

 	if (document.ipv6_basic.connectionType.options.selectedIndex == 0) {
		show_div(true, "Ethernet_setting");
		show_div(true, "Autoconfiguration");
		show_div(true, "Stateful_setting0_static");
		show_div(false, "Stateful_setting1_pppoe");
		show_div(false, "Stateful_setting2_dhcp"); 
		document.ipv6_basic.dhcpIPv6_addrprefix0.disabled = true ;
	    document.ipv6_basic.dhcpIPv6_addrprefix00.disabled = true ;			 		  		    
 	}
	if (document.ipv6_basic.connectionType.options.selectedIndex == 1) {
	    show_div(true, "DHCP_setting");
		show_div(true, "Autoconfiguration");
	    show_div(true, "Stateful_setting2_dhcp");
	 	show_div(false, "Stateful_setting0_static");
		show_div(false, "Stateful_setting1_pppoe");          	        	        
		document.ipv6_basic.dhcpIPv6_addrprefix2.disabled = true ;
	    document.ipv6_basic.dhcpIPv6_addrprefix22.disabled = true ;
	    show_div(true, "wandhcpv6type_setting");
	} 			
//	if (document.ipv6_basic.connectionType.options.selectedIndex == 2) {
//		show_div(true, "PPPoE_setting");
//		show_div(true, "Autoconfiguration");
//		show_div(true, "Stateful_setting1_pppoe");
//		show_div(false, "Stateful_setting0_static");
//		show_div(false, "Stateful_setting2_dhcp");
//		document.ipv6_basic.dhcpIPv6_addrprefix1.disabled = true ;
//	    document.ipv6_basic.dhcpIPv6_addrprefix11.disabled = true ;		  	
//	}
	if (document.ipv6_basic.connectionType.options.selectedIndex == 2) {
		show_div(true, "Linklocal_setting");
	}

//Steve
IPv6TypeSwitch();

//    parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
  
}

function IPv6TypeSwitch()
{

    if (document.ipv6_basic.IPv6Type.options.selectedIndex == 0) {

		show_div(false, "Stateful_setting0_static");
		show_div(false, "Stateful_setting1_pppoe");
		show_div(false, "Stateful_setting2_dhcp");  
    }
    if (document.ipv6_basic.IPv6Type.options.selectedIndex == 1) {
		if (document.ipv6_basic.connectionType.value == 0) {
	    	show_div(true, "Stateful_setting0_static");
			show_div(false, "Stateful_setting1_pppoe");
			show_div(false, "Stateful_setting2_dhcp");
		 	    					
	    	document.ipv6_basic.dhcpIPv6_addrprefix0.disabled = true ;
	    	document.ipv6_basic.dhcpIPv6_addrprefix00.disabled = true ;	            		    
 		}	
	
		if (document.ipv6_basic.connectionType.value == 1) {
		    //dhcp	
			show_div(false, "Stateful_setting0_static");
			show_div(false, "Stateful_setting1_pppoe");
			show_div(true, "Stateful_setting2_dhcp"); 	 
		
			document.ipv6_basic.dhcpIPv6_addrprefix2.disabled = true ;
	        document.ipv6_basic.dhcpIPv6_addrprefix22.disabled = true ;		
		}
		
		if (document.ipv6_basic.connectionType.value == 2) {
	    	//pppoe
			show_div(true, "Stateful_setting1_pppoe");
			show_div(false, "Stateful_setting0_static");
			show_div(false, "Stateful_setting2_dhcp"); 	    		
	       
	        document.ipv6_basic.dhcpIPv6_addrprefix1.disabled = true ;
	        document.ipv6_basic.dhcpIPv6_addrprefix11.disabled = true ;	      	        
		}       	
    	
    }
}
function DnsTypeChange()
{
    if (document.ipv6_basic.DnsType[0].checked == true){
      	document.ipv6_basic.dhcpIPv6_dns1.disabled = true;
      	document.ipv6_basic.dhcpIPv6_dns2.disabled = true;
    }
    else if (document.ipv6_basic.DnsType[1].checked == true){
        document.ipv6_basic.dhcpIPv6_dns1.disabled = false;
      	document.ipv6_basic.dhcpIPv6_dns2.disabled = false;
    }
}
function AddrTypeChange()
{
    if (document.ipv6_basic.AddrType[0].checked == true)
    {
      	document.ipv6_basic.pppoeIPv6_addr.disabled = true;
    }
    else if (document.ipv6_basic.AddrType[1].checked == true)
    {
        document.ipv6_basic.pppoeIPv6_addr.disabled = false;
    }
}
function DNS1TypeSwitch()
{
   if ((document.ipv6_basic.dns1Type.options.selectedIndex == 0) ||
       (document.ipv6_basic.dns1Type.options.selectedIndex == 2))
   {
        document.ipv6_basic.pppoeIPv6_dns1.disabled = true;

   }
   else
   {
        document.ipv6_basic.pppoeIPv6_dns1.disabled = false;
   }
}

function DNS2TypeSwitch()
{
   if ((document.ipv6_basic.dns2Type.options.selectedIndex == 0) ||
       (document.ipv6_basic.dns2Type.options.selectedIndex == 2))
   {
        document.ipv6_basic.pppoeIPv6_dns2.disabled = true;
   }
   else
   {
        document.ipv6_basic.pppoeIPv6_dns2.disabled = false;
   }
}

function initTranslation()
{
	var e = document.getElementById("LanIPv6Address0");
	e.innerHTML = _("ipv6 lan address");
	
	e = document.getElementById("LanIPv6Address1");
	e.innerHTML = _("ipv6 lan address");
	
	e = document.getElementById("LanIPv6Address2");
	e.innerHTML = _("ipv6 lan address");

	e = document.getElementById("wantitle");
	e.innerHTML = _("ipv6 connection type setup");
	
	e = document.getElementById("ipv6Encapsulation");
	e.innerHTML = _("ipv6 connection type");
	
	e = document.getElementById("wanEthernet");
	e.innerHTML = _("ethernet");
	
	e = document.getElementById("linklocal");
	e.innerHTML = _("link local only");
	
	e = document.getElementById("lanAddrSetup1");
	e.innerHTML = _("lan addr setup");
	
	e = document.getElementById("lanAddrSetup2");
	e.innerHTML = _("lan addr setup");
	
	e = document.getElementById("lanAddrSetup3");
	e.innerHTML = _("lan addr setup");

	e = document.getElementById("LanIPv6_LinklocalAddress");
	e.innerHTML = _("lan local link addr");
	
	e = document.getElementById("LanIPv6_LinklocalAddress1");
	e.innerHTML = _("lan local link addr");
	
	e = document.getElementById("LanIPv6_LinklocalAddress2");
	e.innerHTML = _("lan local link addr");
	
	e = document.getElementById("wanAddrSetup");
	e.innerHTML = _("wan addr setup");
				
//	e = document.getElementById("LanIPv6_Size");
//	e.innerHTML = _("ipv6 lan net size");

	e = document.getElementById("WanIPv6_Address");
	e.innerHTML = _("ipv6 wan address");
	
	e = document.getElementById("WanIPv6_Size");
	e.innerHTML = _("ipv6 wan net size");

	e = document.getElementById("Wan_IPv6_gateway");
	e.innerHTML = _("ipv6 wan gateway");

	e = document.getElementById("Wan_IPv6_DNS1");
	e.innerHTML = _("ipv6 wan primary dns");
	
	e = document.getElementById("Wan_IPv6_DNS2");
	e.innerHTML = _("ipv6 wan secondary dns");
	
	e = document.getElementById("addrAutoconfSetup");
	e.innerHTML = _("autoconfiguration addr setup");
	
	e = document.getElementById("IPv6_Dhcp_enable");
	e.innerHTML = _("ipv6 dhcp enable");
	
	e = document.getElementById("IPv6_Dhcp_lifetime");
	e.innerHTML = _("ipv6 dhcp life time");
	
	e = document.getElementById("IPv6_time_minutes");
	e.innerHTML = _("ipv6 time minutes");

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");

	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initValue()
{

	var wan_ipv6_proto = '<% getCfgZero(1, "wan_ipv6_proto"); %>';
	
    	show_div(false, "Ethernet_setting");
		show_div(false, "PPPoE_setting");
		show_div(false, "DHCP_setting");  	
		show_div(false, "Linklocal_setting");
		show_div(false, "Autoconfiguration");
		show_div(false, "wandhcpv6type_setting");  
	
	if(wan_ipv6_proto == '0'){ 
  		document.ipv6_basic.connectionType.options.selectedIndex = 0;
	    show_div(true, "Ethernet_setting");
		show_div(true, "Autoconfiguration");	   
	}
    else if (wan_ipv6_proto == '1') {
		document.ipv6_basic.connectionType.options.selectedIndex = 1;
		show_div(true, "DHCP_setting");
		show_div(true, "Autoconfiguration");
		show_div(true, "wandhcpv6type_setting");
   	}
//	else if (wan_ipv6_proto == '2') {
//		document.ipv6_basic.connectionType.options.selectedIndex = 2;
//		show_div(true, "PPPoE_setting");
//		show_div(true, "Autoconfiguration");
//  	}
	else if (wan_ipv6_proto == '3') {
		document.ipv6_basic.connectionType.options.selectedIndex = 2;
		show_div(true, "Linklocal_setting");		
  	}

   
	initTranslation();


	var Ipv6DHCPEnable  = <% getCfgZero(1, "Ipv6DhcpCapable"); %>;
		
	if (Ipv6DHCPEnable == 1)
		document.ipv6_basic.dhcpIPv6_capable.checked = true;
	else
		document.ipv6_basic.dhcpIPv6_capable.checked = false;

//Steve
//stateful or stateless		
	var IPv6State = <% getCfgGeneral(1, "IPv6State"); %>;
	show_div(false, "Stateful_setting0_static");
	show_div(false, "Stateful_setting1_pppoe");
	show_div(false, "Stateful_setting2_dhcp");
	if (IPv6State == 0)
	{
  		document.ipv6_basic.IPv6Type.options.selectedIndex = 0;
	}
	else if (IPv6State == 1)
	{
  		document.ipv6_basic.IPv6Type.options.selectedIndex = 1;
  		if (wan_ipv6_proto == 0){	
			show_div(true, "Stateful_setting0_static");
	        document.ipv6_basic.dhcpIPv6_addrprefix0.disabled = true ;
	        document.ipv6_basic.dhcpIPv6_addrprefix00.disabled = true ;			
		}
 		if (wan_ipv6_proto == 1){	
			show_div(true, "Stateful_setting2_dhcp");
	        document.ipv6_basic.dhcpIPv6_addrprefix2.disabled = true ;
	        document.ipv6_basic.dhcpIPv6_addrprefix22.disabled = true ;
		}
 		if (wan_ipv6_proto == 2){	
			
	        show_div(true, "Stateful_setting1_pppoe");
	        document.ipv6_basic.dhcpIPv6_addrprefix1.disabled = true ;
	        document.ipv6_basic.dhcpIPv6_addrprefix11.disabled = true ;
		}				
	}  		
//pppoe		
	if (PppoeIPv6_iptype == "0")
	{
		document.ipv6_basic.AddrType[0].checked = true;
		document.ipv6_basic.pppoeIPv6_addr.disabled = true;
	}else if(PppoeIPv6_iptype == "1"){
		document.ipv6_basic.AddrType[1].checked = true;	
		document.ipv6_basic.pppoeIPv6_addr.disabled = false;
	}
	
	if (dnsType == "0")
	{
		document.ipv6_basic.DnsType[0].checked = true;
		document.ipv6_basic.dhcpIPv6_dns1.disabled = true;
      		document.ipv6_basic.dhcpIPv6_dns2.disabled = true;
	}else if(dnsType == "1"){
		document.ipv6_basic.DnsType[1].checked = true;	
		document.ipv6_basic.dhcpIPv6_dns1.disabled = false;
      		document.ipv6_basic.dhcpIPv6_dns2.disabled = false;
	}
//dns type	
	var dns1_type = "<% getCfgGeneral(1, "ipv6_dns1_type"); %>";
	var dns2_type = "<% getCfgGeneral(1, "ipv6_dns2_type"); %>";	

	if (dns1_type == "ISP")
	{
		document.ipv6_basic.dns1Type.options.selectedIndex = 0;
	}
	else if (dns1_type == "USER")
	{
		document.ipv6_basic.dns1Type.options.selectedIndex = 1;
	}
	else if (dns1_type == "NONE")
	{   
		document.ipv6_basic.dns1Type.options.selectedIndex = 2;
	}    
	if (dns2_type == "ISP")
	{
		document.ipv6_basic.dns2Type.options.selectedIndex = 0;
	}
	else if (dns2_type == "USER")
	{
		document.ipv6_basic.dns2Type.options.selectedIndex = 1;
	}
	else if (dns2_type == "NONE")
	{   
		document.ipv6_basic.dns2Type.options.selectedIndex = 2;
	}  
	
	DNS1TypeSwitch();
	DNS2TypeSwitch();
//
var wan_dhcp_type = "<% getCfgGeneral(1, "Ipv6_wan_dhcp_type"); %>";
	if (wan_dhcp_type == "0")
	{
		document.ipv6_basic.wandhcpv6type[0].checked = true;
	}
	else if (wan_dhcp_type == "1")
	{
		document.ipv6_basic.wandhcpv6type[1].checked = true;
	}
}

function CheckValue()
{
//	if (document.ipv6_basic.connectionType.options.selectedIndex == 1){
//		document.ipv6_basic.DnsType[0].value = document.ipv6_basic.DnsType[0].checked ? "0": "1";	
//	}
//	else if (document.ipv6_basic.connectionType.options.selectedIndex == 2){
//		document.ipv6_basic.AddrType[0].value = document.ipv6_basic.AddrType[0].checked ? "0": "1";	
//	}

//validate ipv6 address for Ethernet
	if (!validate_ipv6addr(document.ipv6_basic.wanIPv6_addr.value))
		return false;
	if (!validate_ipv6addr(document.ipv6_basic.wanIPv6_gateway.value))
		return false;
//	if (!validate_ipv6addr(document.ipv6_basic.wanIPv6_dns1.value))
//		return false;
//	if(document.ipv6_basic.wanIPv6_dns2.value != ""){
//		if (!validate_ipv6addr(document.ipv6_basic.wanIPv6_dns2.value))
//			return false;
//	}
	if (!validate_ipv6addr(document.ipv6_basic.lanIPv6_addr_static.value))
		return false;
	
	if (document.ipv6_basic.pppoeIPv6_pass.value != document.ipv6_basic.pppoeIPv6_passconfirm.value)
	{
		alert("Error: Confirm password not match!!");
		return false;
	}
	
	if (document.ipv6_basic.dns1Type.options.selectedIndex == 1) //DNS1 User-Defined
	{
		if(document.ipv6_basic.pppoeIPv6_dns1.value == "")
		{
			alert('First DNS Server should not be empty with User-Defined.');
			return false;
		}
		if(document.ipv6_basic.pppoeIPv6_dns1.value == "0.0.0.0")
		{
			alert('First DNS Server should not be 0.0.0.0, please give DNS IP Address.');
			return false;
		}
//		if (!checkIpAddr(document.ipv6_basic.pppoeIPv6_dns1, false))
//				return false;
		}
	if (document.ipv6_basic.dns2Type.options.selectedIndex == 1)
	{
		if(document.ipv6_basic.pppoeIPv6_dns2.value == "")
		{
			alert('Second DNS Server should not be empty with User-Defined.');
			return false;
		}
		if(document.ipv6_basic.pppoeIPv6_dns2.value == "0.0.0.0")
		{
			alert('Second DNS Server should not be 0.0.0.0, please give DNS IP Address.');
			return false;
		}
//		if (!checkIpAddr(document.ipv6_basic.pppoeIPv6_dns2, false))
//				 return false;
	} 

	showWebMessage(2, "");
	return true;
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name=ipv6_basic action="/goform/IPV6" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>  
                <li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="wantitle">IPv6 Connection Type Setup</td>
                   </tr></table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="ipv6Encapsulation">IPv6 Connection Type : </td>
                      <td><select name="connectionType" size="1" onChange="connectionTypeSwitch();">
                          	<option value=0 id="wanEthernet">Ethernet</option>
                      	  	<option value=1 id="wanPPTP">DHCPv6</option>
<!--                      	<option value=2 id="wanPPPOE">PPP over Ethernet</option>    -->
			  				<option value=3 id="linklocal">Link-local only</option>
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>  
		<span id = "wandhcpv6type_setting" class = "off" >		
				<li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr><td width="40%" id=wandhcpv6type_id>Type : </td>
                        <td><input name="wandhcpv6type" value="0" onclick="" type="radio" /><font id=wandhcpv6type_ss>Stateless</font>                 
                            <input name="wandhcpv6type" value="1" onclick=" "type="radio" /><font id=wandhcpv6type_sl>Stateful</font>
                        </td>
                    </tr>
                  </table>
                </li>
        </span>
    	<span id = "Linklocal_setting" class = "off" >
<!--link local start  --> 		
                <li class="space3"></li>
                <li class="title" id="lanAddrSetup1">LAN IPv6 Address Setup</li>
                <li class="w_text">
                  <table id=LanIPv6_LinklocalAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=LanIPv6_LinklocalAddress>LAN IPv6 Link-local Address :</td>
						<td width="50%">  <% getLinkLocalAddr(); %>  </td>                      
						<td width="*">&nbsp;</td>
                    </tr>
                  </table>
                </li>
<!--link local end  -->		
	</span>    	    	
    	<span id = "Ethernet_setting" class = "off" >
<!--Static start  -->    
	            
                <li class="space3"></li>
                <li class="title" id="wanAddrSetup">WAN IPv6 Address Setup</li>
                <li class="w_text">
                  <table id=WanIPv6_StaAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=WanIPv6_Address>IPv6 Address :</td>
                        <td><input name="wanIPv6_addr" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "wan_ipv6addr"); %>" /> </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=WanIPv6_Netsize width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=WanIPv6_Size>Subnet Prefix Length :</td>
                      <td><input name="wanIPv6_Netsize" maxlength=3 size=3 
                      		value="<% getCfgGeneral(1, "wan_ipv6netsize"); %>" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_gateway width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Wan_IPv6_gateway>Gateway IP Address :</td>
                        <td><input name="wanIPv6_gateway" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "wan_ipv6gateway"); %>" /> </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_dns1 width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Wan_IPv6_DNS1>First DNS Server :</td>
                        <td><input name="wanIPv6_dns1" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "wan_ipv6dns1"); %>" /> </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_dns2 width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Wan_IPv6_DNS2>Second DNS Server :</td>
                        <td><input name="wanIPv6_dns2" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "wan_ipv6dns2"); %>" /> </td>
                    </tr>
                  </table>
                </li>
                
      		<li class="space3"></li>
		<li class="title" id="lanAddrSetup2">LAN IPv6 Address Setup </li>
                <li class="w_text">
                  <table id=LanIPv6_StaAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="LanIPv6Address0">LAN IPv6 address :</td>
                      <td><input name="lanIPv6_addr_static" maxlength=40 size=40 
                      	   value="<% getCfgGeneral(1, "lan_ipv6addr_static"); %>" /><font> : </font>
                        <input name="lanIPv6_addr_static_suf" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "lan_ipv6addr_static_suf"); %>"/> /64</td>
                      </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=LanIPv6_LinklocalAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=LanIPv6_LinklocalAddress1>LAN IPv6 Link-local Address :</td>
						<td width="50%"> <% getLinkLocalAddr(); %>  </td>                      
						<td width="*">&nbsp;</td>
                    </tr>
                  </table>
                  </li>               
<!--
                <li class="w_text">
                  <table id=LanIPv6_Netsize width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=LanIPv6_Size>Netsize (bits of hostpart) :</td>
                      <td><input name="lanIPv6_Netsize" maxlength=3 size=3 
                      	   value="<% getCfgGeneral(1, "lan_ipv6netsize"); %>" /></td>
                    </tr>
                  </table>
                </li>
-->                  
<!-- Static end   -->
	</span>
	
    	<span id = "PPPoE_setting" class = "off" >
<!-- PPPoe start-->
                <li class="space3"></li>
                <li class="title">PPPOE IPv6 Address Setup</li>
		<li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr><td width="40%" id=PppoeIPv6_iptype>Address mode : </td>
                        <td><input name="AddrType" value="0" onclick="AddrTypeChange()" type="radio" /><font id=addrtypeDHCP>DHCP</font>                 
                            <input name="AddrType" value="1" onclick="AddrTypeChange() "type="radio" /><font id=addrtypeIP>Static IP</font>
                        </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=PppoeIPv6_addr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=PppoeIPv6_Addr>IPv6 address :</td>
                        <td><input name="pppoeIPv6_addr" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "pppoe_ipv6addr"); %>" /> </td>
                        </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=PppoeIPv6_user width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=PppoeIPv6_Username>User name :</td>
                        <td><input name="pppoeIPv6_user" maxlength=30 size=30 
                        	value="<% getCfgGeneral(1, "ppoe_ipv6user"); %>" /> </td>
                    </tr>
                  </table>
                </li>
                   <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="PppoeIPv6_Passwd">Password</font> : </td>
    		      <td><input type="password" name="pppoeIPv6_pass" size="30" maxlength="30" value="<% getCfgGeneral(1, "pppoe_ipv6pass"); %>"></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><font id="PppoeIPv6_PassConfirm">Retype to confirm</font> : </td>
                      <td><input type="password" name="pppoeIPv6_passconfirm" size="30" maxlength="30" value="<% getCfgGeneral(1, "pppoe_ipv6pass"); %>"></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=PppoeIPv6_service width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=PppoeIPv6_Service>Service name :</td>
                        <td><input name="pppoeIPv6_service" maxlength=30 size=30 
                        	value="<% getCfgGeneral(1, "pppoe_ipv6service"); %>" /> </td>
                    </tr>
                  </table>
                </li>	
                <li class="w_text">
                  <table id=PppoeIPv6_idle width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=PppoeIPv6_Idle>Idle time :</td>
                      <td><input name="pppoeIPv6_idle" maxlength=3 size=3 
                      		value="<% getCfgGeneral(1, "pppoe_ipv6idle"); %>" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=PppoeIPv6_mtu width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=PppoeIPv6_MTU>MTU :</td>
                      <td><input name="pppoeIPv6_mtu" maxlength=3 size=3 
                      		value="<% getCfgGeneral(1, "pppoe_ipv6mtu"); %>" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=PppoeIPv6_dns1 width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=PppoeIPv6_DNS1>Primary IPv6 DNS :</td>
                        <td><select name="dns1Type" size="1" onChange="DNS1TypeSwitch();">
                          <option value="ISP" id="wanDNSISP">From ISP</option>
                          <option value="USER" id="wanDNSUSER">User-Defined</option>
                          <option value="NONE" id="wanDNSNONE">None</option>
                        </select>
                        <!--<input name="pppoeIPv6_dns1" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "pppoe_ipv6dns1"); %>" /> </td>-->
                        <input name="pppoeIPv6_dns1" maxlength=25 size=25 value="<% getIpv6Dns(1); %>"></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=PppoeIPv6_dns2 width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=PppoeIPv6_DNS2>Secondary IPv6 DNS :</td>
                        <td><select name="dns2Type" size="1" onChange="DNS2TypeSwitch();">
                          <option value="ISP" id="wanDNS2ISP">From ISP</option>
                          <option value="USER" id="wanDNS2USER">User-Defined</option>
                          <option value="NONE" id="wanDNS2NONE">None</option>
                        </select>
                        <!--<input name="pppoeIPv6_dns2" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "pppoe_ipv6dns2"); %>" /> </td>-->
                        <input name="pppoeIPv6_dns2" maxlength=25 size=25 value="<% getIpv6Dns(2); %>"></td>
                    </tr>
                  </table>
                </li>
                
      		<li class="space3"></li>
		<li class="title">LAN IPv6 Setup </li>
                <li class="w_text">
                  <table id=LanIPv6_StaAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="LanIPv6Address1">LAN IPv6 address :</td>
                      <td><input name="lanIPv6_addr_pppoe" maxlength=40 size=40 
                      	   value="<% getCfgGeneral(1, "lan_ipv6addr_pppoe"); %>" /> 1/64</td>
                      </tr>
                  </table>
                </li>
<!--
                <li class="w_text">
                  <table id=LanIPv6_Netsize width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=LanIPv6_Size>Netsize (bits of hostpart) :</td>
                      <td><input name="lanIPv6_Netsize" maxlength=3 size=3 
                      	   value="<% getCfgGeneral(1, "lan_ipv6netsize"); %>" /></td>
                    </tr>
                  </table>
                </li>
-->                  
<!-- PPPoe end-->
	</span>
	
    	<span id = "DHCP_setting" class = "off" >
<!-- DHCP start   -->
                <li class="space3"></li>
                <li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="ipv6dnsid">IPv6 DNS Setup</td>
                   </tr></table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr><td width="40%" id=dnsType>DNS Setup : </td>
                        <td><input name="DnsType" value="0" onclick="DnsTypeChange()" type="radio" /><font id=dnstypeAuto>Auto</font>                 
                            <input name="DnsType" value="1" onclick="DnsTypeChange() "type="radio" /><font id=dnstypeManual>Manual</font>
                        </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_dns1 width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Dhcp_IPv6_DNS1>First DNS Server :</td>
                        <td><input name="dhcpIPv6_dns1" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "dhcp_ipv6dns1"); %>" /> </td>
                        </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_dns2 width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Dhcp_IPv6_DNS2>Second DNS Server :</td>
                        <td><input name="dhcpIPv6_dns2" maxlength=40 size=40 
                        	value="<% getCfgGeneral(1, "dhcp_ipv6dns2"); %>" /> </td>
                        </tr>
                  </table>
                </li>
                
      		<li class="space3"></li>
		<li class="title" id="lanAddrSetup3" >LAN IPv6 Address Setup </li>
                <li class="w_text">
                  <table id=LanIPv6_StaAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="LanIPv6Address2" >LAN IPv6 address :</td>
                      <td><input name="lanIPv6_addr_dhcp" maxlength=35 size=35 
                      	   value="<% getCfgGeneral(1, "lan_ipv6addr_dhcp"); %>" /><font> : </font>
                        <input name="lanIPv6_addr_dhcp_suf" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "lan_ipv6addr_dhcp_suf"); %>"/>/64</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=LanIPv6_LinklocalAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=LanIPv6_LinklocalAddress2>LAN IPv6 Link-local Address :</td>
						<td width="50%"> <% getLinkLocalAddr(); %>  </td>                      
						<td width="*">&nbsp;</td>
                    </tr>
                  </table>
                </li>
<!--
                <li class="w_text">
                  <table id=LanIPv6_Netsize width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=LanIPv6_Size>Netsize (bits of hostpart) :</td>
                      <td><input name="lanIPv6_Netsize" maxlength=3 size=3 
                      	   value="<% getCfgGeneral(1, "lan_ipv6netsize"); %>" /></td>
                    </tr>
                  </table>
                </li>
-->               
<!-- DHCP end   -->
	</span>
<!--                <li class="space3"></li>
                <li class="title">Tunnel IPv6 Setup</li>
                <li class="w_text">
                <table id=IPv6_Tunnel width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" class="head" id="IPv6_Adv">Enable IPv6-tunnel :</td>
                      <td>
                        <input type=radio id=tunnel_0 name=tunnel value="0" checked><font id="TunnelToYes">Yes</font>
                        <input type=radio id=tunnel_1 name=tunnel value="1"><font id="TunnelToNo">No</font>
                      </td>
                     </tr>
                 </table>
                </li>
                <li class="w_text">
                  <table id=Remote_endpoint width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=RemoteEndpoint>Remote endpoint :</td>
                      <td><input name="Remote_endpoint" maxlength=15 size=15 /></td>
                      </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=Local_addr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Local_address>Local IPv6 address :</td>
                        <td><input name="Local_IPv6_addr" maxlength=20 size=20 /> </td>
                        </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_Tunnel_Netsize width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=IPv6_Tunnel_Size>Netsize (bits of hostpart) :</td>
                      <td><input name="Tunnel_IPv6_Netsize" maxlength=3 size=3 /></td>
                      </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_Tunnel_gateway width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Tunnel_IPv6_gateway>Remote IPv6 gateway :</td>
                        <td><input name="RemoteGateway" maxlength=20 size=20 /> </td>
                        </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=Tunnel_MTU width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Tunnel_IPv6_MTU>Tunnel MTU :</td>
                        <td><input name="TunnelMTU" maxlength=4 size=4 /> </td>
                        </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=Tunnel_TTl width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=Tunnel_IPv6_TTL>Tunnel TTL :</td>
                        <td><input name="TunnelTTL" maxlength=3 size=3 /> </td>
                        </tr>
                  </table>
                </li>                   -->
<!--
      		<li class="space3"></li>
		<li class="title">LAN IPv6 Setup </li>
                <li class="w_text">
                  <table id=LanIPv6_StaAddr width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=LanIPv6Address>LAN IPv6 address :</td>
                      <td><input name="lanIPv6_addr" maxlength=40 size=40 
                      	   value="<% getCfgGeneral(1, "lan_ipv6addr"); %>" /> <font >:: 1/64</font> </td>
                      </tr>
                  </table>
                </li>

                <li class="w_text">
                  <table id=LanIPv6_Netsize width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=LanIPv6_Size>Netsize (bits of hostpart) :</td>
                      <td><input name="lanIPv6_Netsize" maxlength=3 size=3 
                      	   value="<% getCfgGeneral(1, "lan_ipv6netsize"); %>" /></td>
                    </tr>
                  </table>
                </li>
-->                
<!--                <li class="w_text">
                  <table id=IPv6_gateway width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=LanIPv6_Gateway>Static IPv6 gateway :</td>
                        <td><input name="lanIPv6_gateway" maxlength=20 size=20 
                            value="<% getCfgGeneral(1, "lan_ipv6gateway"); %>" /> </td>
                        </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id=IPv6_dns width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=LanIPv6_Dns>Static IPv6 DNS :</td>
                        <td><input name="lanIPv6_dns" maxlength=20 size=20 
                            value="<% getCfgGeneral(1, "lan_ipv6dns"); %>" /> </td>
                        </tr>
                  </table>
                </li>  -->
<!-- Autoconfiguration -->
<span id = "Autoconfiguration" class = "off" >
                <li class="space3"></li>
                <li class="title" id="addrAutoconfSetup">Address Autoconfiguration Setup</li>
                <li class="w_text">
		  <table id=IPv6_DHCP width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
		    <tr>
		      <td><input name="dhcpIPv6_capable" type="checkbox" value="1" /><input name="AAA" type="hidden" value=1 />
		          <font id ="IPv6_Dhcp_enable">Enable Stateless Address Autoconfiguration</font>
		      </td>
		    </tr>
		  </table>
		</li>
        <li class="w_text">
           <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
             <tr>
               <td width="40%" id="ipv6Type">Type : </td>
               <td><select name="IPv6Type" size="1" onChange="IPv6TypeSwitch();">
                   <option value=0 id="stateless">Stateless</option>
                   <option value=1 id="stateful">Stateful</option>
                   </select> 
               </td>
             </tr>
           </table>
        </li>  
<!--
    	<span id = "Stateless_setting" class = "off" >
    	
	</span>      --> 	
    	<span id = "Stateful_setting0_static" class = "off" >
    	
<!-- dhcp stateful for ethernet-->
                <li class="w_text">
                  <table id=Ipv6Dhcp_addrstart width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=IPv6_Dhcp_addrstart>Address (start) :</td>
                        <td><input name="dhcpIPv6_addrprefix0" maxlength=35 size=35 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrprefix0"); %>"/><font> : </font>
                        <input name="dhcpIPv6_addrstart0" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrstart"); %>"/></td>                        
                        </tr>
                  </table>
                </li> 
                 <li class="w_text">
                  <table id=Ipv6Dhcp_addrend width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=IPv6_Dhcp_addrend>Address (end) :</td>
                        <td><input name="dhcpIPv6_addrprefix00" maxlength=35 size=35 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrprefix0"); %>"/><font> : </font>                       
                        <input name="dhcpIPv6_addrend0" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrend"); %>"/></td>
                        </tr>
                  </table>
                </li> 
	</span>     
<!-- dhcp stateful for pppoe --> 
    	<span id = "Stateful_setting1_pppoe" class = "off" >

                <li class="w_text">
                  <table id=Ipv6Dhcp_addrstart width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=IPv6_Dhcp_addrstart>Address (start) :</td>
                        <td><input name="dhcpIPv6_addrprefix1" maxlength=35 size=35 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrprefix1"); %>"/><font> :: </font>
                        <input name="dhcpIPv6_addrstart1" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrstart"); %>"/></td>                        
                        </tr>
                  </table>
                </li> 
                 <li class="w_text">
                  <table id=Ipv6Dhcp_addrend width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=IPv6_Dhcp_addrend>Address (end) :</td>
                        <td><input name="dhcpIPv6_addrprefix11" maxlength=35 size=35 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrprefix1"); %>"/><font> :: </font>                      
                        <input name="dhcpIPv6_addrend1" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrend"); %>"/></td>
                        </tr>
                  </table>
                </li> 
	</span>   
 
<!-- dhcp stateful for dhcpv6 -->
    	<span id = "Stateful_setting2_dhcp" class = "off" >

                <li class="w_text">
                  <table id=Ipv6Dhcp_addrstart width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=IPv6_Dhcp_addrstart>Address (start) :</td>
                        <td><input name="dhcpIPv6_addrprefix2" maxlength=35 size=35 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrprefix2"); %>"/><font> : </font>
                        <input name="dhcpIPv6_addrstart2" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrstart"); %>"/></td>                        
                        </tr>
                  </table>
                </li> 
                 <li class="w_text">
                  <table id=Ipv6Dhcp_addrend width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=IPv6_Dhcp_addrend>Address (end) :</td>
                        <td><input name="dhcpIPv6_addrprefix22" maxlength=35 size=35 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrprefix2"); %>"/><font> : </font>                       
                            <input name="dhcpIPv6_addrend2" maxlength=10 size=10 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_addrend"); %>"/></td>
                        </tr>
                  </table>
                </li> 
	</span>     
           
                <li class="w_text">
                  <table id=Ipv6Dhcp_lifetime width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                        <td width="40%" id=IPv6_Dhcp_lifetime>Router Advertisement Lifetime :</td>
                        <td><input name="dhcpIPv6_LifeTime" maxlength=3 size=3 
                             value="<% getCfgGeneral(1, "Ipv6_dhcp_lifetime"); %>"/> <font id="IPv6_time_minutes">(minutes)</font></td>
                        </tr>
                  </table>
                </li>
	</span>
                <li></li>
		<li></li>

              </ul>
            </div>
</li>
 
<center>
<li class="table_button">
<input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type=reset value="Reset" id="GeneralReset" onClick="window.location.reload()">
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

