<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
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

var wan_dhcp_fixedip = '<% getCfgZero(1, "wan_ip_auto"); %>';
var pptpNailup = '<% getCfgZero(1, "wan_pptp_nailup"); %>';
var pptpmode = '<% getCfgZero(1, "wan_pptp_mode"); %>';
var pptpWanIPmode = '<% getCfgZero(1, "wan_pptp_wanip_mode"); %>';
var pppoeWanIPmode = '<% getCfgZero(1, "wan_pppoe_wanip_mode"); %>';
var l2tpWanIPmode = '<% getCfgZero(1, "wan_l2tp_wanip_mode"); %>';
//var l2tpNailup = '<% getCfgZero(1, "wan_l2tp_nailup"); %>';
var l2tpmode = '<% getCfgZero(1, "wan_l2tp_mode"); %>';
var pppoeNailup = '<% getCfgZero(1, "wan_pppoe_nailup"); %>';
var macCloneMode = '<% getCfgZero(1, "wan_MacClone_mode"); %>';
var brand_3g = "<% getCfgZero(1, "brand_3g"); %>";
var model_3g = "<% getCfgZero(1, "model_3g"); %>";
var apnType_3g = "<% getCfgZero(1, "apnType_3g"); %>";
var location_3g = "<% getCfgZero(1, "location_3g"); %>";
var srvPro_3g = "<% getCfgZero(1, "srvPro_3g"); %>";
var apn_3g = "<% getCfgGeneral(1, "apn_3g"); %>";
var auth_3g = "<% getCfgZero(1, "auth_3g"); %>";
var connMode_3g = "<% getCfgZero(1, "connMode_3g"); %>";
var pppConnType = "<% getCfgZero(1, "ppp_conn_type"); %>";
var turbo_3g = "<% getCfgZero(1, "turbo_3g"); %>";
var nailup_3g = '<% getCfgZero(1, "wan_3g_nailup"); %>';
var bigPond_srv = "<% getCfgZero(1, "bigPond_srv"); %>";
var bigPond_enb = "<% getCfgZero(1, "bigPond_enb"); %>";

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
    
     if (document.wanCfg.connectionType.options.selectedIndex == 0) //Ethernet
     {   
         if (document.wanCfg.WAN_IP_Auto[0].checked == true) //DHCP
         {
             if (document.wanCfg.bigPondEnb[0].checked == true) //BigPond enabled
             {
                 if(document.wanCfg.bigPondUser.value == "")
                 {
                        alert("Bigpond user name should not be empty!");
                        document.wanCfg.bigPondUser.focus();
			return false;
                 }
                 if(document.wanCfg.bigPondPass.value == "")
                 {
                        alert("Bigpond password should not be empty!");
                        document.wanCfg.bigPondPass.focus();
			return false;
                 }
                 if(document.wanCfg.bigPondPass2.value == "")
                 {
                        alert("Please retype password to confirm!");
                        document.wanCfg.bigPondPass2.focus();
			return false;
                 }         
                 if (document.wanCfg.bigPondPass.value != document.wanCfg.bigPondPass2.value)
                 {
			alert("Password mismatched!");
			return false;
		 }
             }
             document.wanCfg.bigPondEnb[0].value = document.wanCfg.bigPondEnb[0].checked ? "1": "0";
         }
         if (document.wanCfg.WAN_IP_Auto[1].checked == true) //Static IP
         {
             if (!checkIpAddr(document.wanCfg.staticIp, false))
			     return false;
	     if (!checkIpAddr(document.wanCfg.staticNetmask, true))
			     return false;
	     if (!checkNetMask(document.wanCfg.staticNetmask))
				return false;
	     if (document.wanCfg.staticGateway.value != "")
	     {
		 if (!checkIpAddr(document.wanCfg.staticGateway, false))
			return false;
             }
             //Static IP: dns should not be assigned from ISP when WAN is Static IP
             if ((document.wanCfg.dns1Type.options.selectedIndex == 0) ||
                 (document.wanCfg.dns2Type.options.selectedIndex == 0))
             {
                alert('WAN is Fixed IP, please set DNS Assignment to User-Defined or None.');
                return false;
             }
         }
     }
  
     else if (document.wanCfg.connectionType.options.selectedIndex == 1) //PPPoE
     {
         if(document.wanCfg.pppoeUser.value == "")
         {
            alert("PPPoE user name should not be empty!");
			return false;
         }
         if(document.wanCfg.pppoePass.value == "")
         {
            alert("PPPoE password should not be empty!");
			return false;
         }
         if(document.wanCfg.pppoePass2.value == "")
         {
            alert("Please retype password to confirm!");
			return false;
         }         
         if (document.wanCfg.pppoePass.value != document.wanCfg.pppoePass2.value) {
			alert("Password mismatched!");
			return false;
		 }
		 if ((document.wanCfg.pppoeMtu.value < 1400) || (document.wanCfg.pppoeMtu.value > 1492)) {
		    alert("Please set MTU Size between 1400 ~ 1492 !");
            return false;
		 }
		 if (isAllNum(document.wanCfg.pppoeMtu.value) == 0) {
		    alert('It should be a [0-9] number with MTU Size.');
		    document.wanCfg.pppoeMtu.value = document.wanCfg.pppoeMtu.defaultValue;
		    document.wanCfg.pppoeMtu.focus();
		    return false;
	     }
	     if (document.wanCfg.pppoeIdleTime.disabled == false)
		 {
			if (document.wanCfg.pppoeIdleTime.value == "")
			{
				alert("Please specify Idle Time");
				document.wanCfg.pppoeIdleTime.focus();
				document.wanCfg.pppoeIdleTime.select();
				return false;
			}
			if (isAllNum(document.wanCfg.pppoeIdleTime.value) == 0) 
			{
		       alert('It should be a [0-9] number.');
		       document.wanCfg.pppoeIdleTime.value = document.wanCfg.pppoeIdleTime.defaultValue;
		       document.wanCfg.pppoeIdleTime.focus();
		       return false;
	        }
         }
		 if (document.wanCfg.pppoeWanIPMode[1].checked == true) {
			if (!checkIpAddr(document.wanCfg.pppoeWanIpAddr, false))
				return false;
		 }
     }
     else if (document.wanCfg.connectionType.options.selectedIndex == 2) //PPTP
     {
         if(document.wanCfg.pptpUser.value == "")
         {
            alert("PPTP user name should not be empty!");
			return false;
         }
         if(document.wanCfg.pptpPass.value == "")
         {
            alert("PPTP password should not be empty!");
			return false;
         }
         if(document.wanCfg.pptpPass2.value == "")
         {
            alert("Please retype password to confirm!");
			return false;
         }         
         if (document.wanCfg.pptpPass.value != document.wanCfg.pptpPass2.value) {
			alert("Password mismatched!");
			return false;
		 }
		 if (!checkIpAddr(document.wanCfg.pptpServer, false))
			return false;
		 if (document.wanCfg.pptpMode[1].checked == true) {
			if (!checkIpAddr(document.wanCfg.pptpIp, false))
				return false;
			if (!checkIpAddr(document.wanCfg.pptpNetmask, true))
				return false;
		        if (!checkNetMask(document.wanCfg.pptpNetmask))
				return false;
			if (!checkIpAddr(document.wanCfg.pptpGateway, false))
				return false;
		 }
		 if (document.wanCfg.pptpWanIPMode[1].checked == true) {
			if (!checkIpAddr(document.wanCfg.pptpWanIpAddr, false))
				return false;
		 }
         if (document.wanCfg.pptpIdleTime.disabled == false)
		 {
			if (document.wanCfg.pptpIdleTime.value == "")
			{
				alert("Please specify Idle Time");
				document.wanCfg.pptpIdleTime.focus();
				document.wanCfg.pptpIdleTime.select();
				return false;
			}
			if (isAllNum(document.wanCfg.pptpIdleTime.value) == 0) 
			{
		       alert('It should be a [0-9] number.');
		       document.wanCfg.pptpIdleTime.value = document.wanCfg.pptpIdleTime.defaultValue;
		       document.wanCfg.pptpIdleTime.focus();
		       return false;
	        }
		 }		 
     }
     else if (document.wanCfg.connectionType.options.selectedIndex == 3) //L2TP
     {
         if(document.wanCfg.l2tpUser.value == "")
         {
            alert("L2TP user name should not be empty!");
			return false;
         }
         if(document.wanCfg.l2tpPass.value == "")
         {
            alert("L2TP password should not be empty!");
			return false;
         }
         if(document.wanCfg.l2tpPass2.value == "")
         {
            alert("Please retype password to confirm!");
			return false;
         }        
         if (document.wanCfg.l2tpPass.value != document.wanCfg.l2tpPass2.value) {
			alert("Password mismatched!");
			return false;
		 }
		 if (!checkIpAddr(document.wanCfg.l2tpServer, false))
			return false;
		 if (document.wanCfg.l2tpMode[1].checked == true) {
			if (!checkIpAddr(document.wanCfg.l2tpIp, false))
				return false;
			if (!checkIpAddr(document.wanCfg.l2tpNetmask, true))
				return false;
		        if (!checkNetMask(document.wanCfg.l2tpNetmask))
				return false;
			if (!checkIpAddr(document.wanCfg.l2tpGateway, false))
				return false;
		 }
		 if (document.wanCfg.l2tpWanIPMode[1].checked == true) {
			if (!checkIpAddr(document.wanCfg.l2tpWanIpAddr, false))
				return false;
		 }
     }
     else if (document.wanCfg.connectionType.options.selectedIndex == 4) //3G
     {
                 //PIN
                 if(document.wanCfg.pin3g.value != "")
                 {
                     if ((document.wanCfg.pin3g.value.length < 4) || (document.wanCfg.pin3g.value.length > 8))
                     {
		         alert("PIN Code length should be between 4~8!");
		         document.wanCfg.pin3g.focus();
		         return false;
		     }
		 }
		 //APN
		 if (document.wanCfg.apn.value == "")
	         {
		     alert("Please specify APN Code");
		     document.wanCfg.apn.focus();
	             return false;
	         }
	         //Dial Number
	         if (document.wanCfg.dial3g.value == "")
	         {
                     alert("Please specify Dial Number");
                     document.wanCfg.dial3g.focus();
	             return false;
		 }
		 //Nailup
		 if (document.wanCfg.idleTime3g.disabled == false)
		 {
		     if (document.wanCfg.idleTime3g.value == "")
	             {
		         alert("Please specify Idle Time");
		         document.wanCfg.idleTime3g.focus();
		         document.wanCfg.idleTime3g.select();
		         return false;
	             }
		     if (isAllNum(document.wanCfg.idleTime3g.value) == 0) 
		     {
		         alert('It should be a [0-9] number.');
		         document.wanCfg.idleTime3g.value = document.wanCfg.idleTime3g.defaultValue;
		         document.wanCfg.idleTime3g.focus();
		         return false;
	             }
                 }            
                 /*
                 if (document.wanCfg.pppType[0].checked == true) //keep alive
                 {
                      if ((document.wanCfg.echoIntvl.value < 3) || (document.wanCfg.echoIntvl.value > 50)) {
		             alert("Please set PPP Echo Interval between 3 ~ 50 !");
                             return false;
                      }
		      if (isAllNum(document.wanCfg.echoIntvl.value) == 0) {
		             alert('It should be a [0-9] number with PPP Echo Interval.');
		             document.wanCfg.echoIntvl.value = document.wanCfg.echoIntvl.defaultValue;
		             document.wanCfg.echoIntvl.focus();
		             return false;
	              }
	              
	              if ((document.wanCfg.retryThreld.value < 3) || (document.wanCfg.retryThreld.value > 50)) {
		             alert("Please set PPP Retry Threshold between 3 ~ 50 !");
                             return false;
                      }
		      if (isAllNum(document.wanCfg.retryThreld.value) == 0) {
		             alert('It should be a [0-9] number with PPP Retry Threshold.');
		             document.wanCfg.retryThreld.value = document.wanCfg.retryThreld.defaultValue;
		             document.wanCfg.retryThreld.focus();
		             return false;
	              }
                 }
                 else //on-demand
                 {
                      if ((document.wanCfg.maxIdle.value < 60) || (document.wanCfg.maxIdle.value > 3600)) {
		             alert("Please set Max Idle Time between 60 ~ 3600 !");
                             return false;
                      }
		      if (isAllNum(document.wanCfg.maxIdle.value) == 0) {
		             alert('It should be a [0-9] number with Max Idle Time.');
		             document.wanCfg.maxIdle.value = document.wanCfg.maxIdle.defaultValue;
		             document.wanCfg.maxIdle.focus();
		             return false;
	              }
                 }
                 
                 //Mobile MTU
                 if ((document.wanCfg.mtu3g.value < 592) || (document.wanCfg.mtu3g.value > 1492)) {
		             alert("Please set Mobile WAN MTU between 592 ~ 1492 !");
                             return false;
                 }
		 if (isAllNum(document.wanCfg.mtu3g.value) == 0) {
		             alert('It should be a [0-9] number with Mobile WAN MTU.');
		             document.wanCfg.mtu3g.value = document.wanCfg.mtu3g.defaultValue;
		             document.wanCfg.mtu3g.focus();
		             return false;
	         }
                 document.wanCfg.apnType3g[0].value = document.wanCfg.apnType3g[0].checked ? "0": "1";
                 document.wanCfg.pppType[0].value = document.wanCfg.pppType[0].checked ? "0": "1";
                 document.wanCfg.turbo3g[0].value = document.wanCfg.turbo3g[0].checked ? "1": "0";
                 */
     }
     else //unknown WAN Mode
		 return false;

     //DNS
     if (document.wanCfg.dns1Type.options.selectedIndex == 1) //DNS1 User-Defined
     {
         if(document.wanCfg.staticPriDns.value == "")
         {
            alert('First DNS Server should not be empty with User-Defined.');
            return false;
         }
         if(document.wanCfg.staticPriDns.value == "0.0.0.0")
         {
            alert('First DNS Server should not be 0.0.0.0, please give DNS IP Address.');
            return false;
         }
         if (!checkIpAddr(document.wanCfg.staticPriDns, false))
				return false;
     }
     if (document.wanCfg.dns2Type.options.selectedIndex == 1)
     {
         if(document.wanCfg.staticSecDns.value == "")
         {
            alert('Second DNS Server should not be empty with User-Defined.');
            return false;
         }
         if(document.wanCfg.staticSecDns.value == "0.0.0.0")
         {
            alert('Second DNS Server should not be 0.0.0.0, please give DNS IP Address.');
            return false;
         }
         if (!checkIpAddr(document.wanCfg.staticSecDns, false))
				 return false;
     } 
     
     //Clone MAC
     if (document.wanCfg.WAN_MAC_Clone[1].checked == true) {
            if (!checkIpAddr(document.wanCfg.spoofIPAddr, false))
				 return false;
     }
     else if (document.wanCfg.WAN_MAC_Clone[2].checked == true) {
			var re = /[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}:[A-Fa-f0-9]{2}/;
			if (document.wanCfg.macCloneMac.value.length == 0) {
				alert("MAC Address should not be empty!");
				document.wanCfg.macCloneMac.focus();
				return false;
			}
			if (!re.test(document.wanCfg.macCloneMac.value)) {
				alert("Please fill the MAC Address in correct format (XX:XX:XX:XX:XX:XX) with hex value !");
				document.wanCfg.macCloneMac.focus();
				return false;
			}
	 }
   
     pppoeNailup_Click(); //PPPoE
     pptpNailup_Click(); //PPTP
     Nailup_3g_Click(); //3G
  
     showWebMessage(2, "");
     return true;  
}

function connectionTypeSwitch()
{
        show_div(false, "PPPoE_setting");
	show_div(false, "PPTP_setting1");
	show_div(false, "PPTP_setting2");  
	show_div(false, "L2TP_setting1");
	show_div(false, "L2TP_setting2");
	show_div(false, "Ethernet_setting");
	show_div(false, "3G_setting");

        if (document.wanCfg.connectionType.options.selectedIndex == 0) {
	    show_div(true, "Ethernet_setting");    
        }	
	
	if (document.wanCfg.connectionType.options.selectedIndex == 1) {
		show_div(true, "PPPoE_setting");
	}
	if (document.wanCfg.connectionType.options.selectedIndex == 2) {
	    show_div(true, "PPTP_setting1");
	    show_div(true, "PPTP_setting2");
	}
	if (document.wanCfg.connectionType.options.selectedIndex == 3) {
	    show_div(true, "L2TP_setting1");
	    show_div(true, "L2TP_setting2");
        }
        if (document.wanCfg.connectionType.options.selectedIndex == 4) {
	    show_div(true, "3G_setting");
        }
//    parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
}

function pppTypeChange()
{
   if (document.wanCfg.pppType[0].checked == true)
   {
      	document.wanCfg.maxIdle.disabled = true;
      	document.wanCfg.echoIntvl.disabled = false;
      	document.wanCfg.retryThreld.disabled = false;
   }
   else
   {
        document.wanCfg.maxIdle.disabled = false;
      	document.wanCfg.echoIntvl.disabled = true;
      	document.wanCfg.retryThreld.disabled = true;
   } 
}

function bigPondEnbChange()
{
   if (document.wanCfg.bigPondEnb[0].checked == true) //Enable
   {
      	document.wanCfg.bigPondSrv.disabled = false;
      	document.wanCfg.bigPondUser.disabled = false;
      	document.wanCfg.bigPondPass.disabled = false;
      	document.wanCfg.bigPondPass2.disabled = false;
   }
   else //Disable
   {
        document.wanCfg.bigPondSrv.disabled = true;
      	document.wanCfg.bigPondUser.disabled = true;
      	document.wanCfg.bigPondPass.disabled = true;
      	document.wanCfg.bigPondPass2.disabled = true;
   }
}

function show_apn(location,srvProvider)
{
   if(location==0) //Taiwan
   {
        switch (srvProvider)
	{
	    case 0:
	            document.wanCfg.apn.value = "internet";
		    break; 
	    case 1:
		    document.wanCfg.apn.value = "internet";
		    break;
            case 2:
		    document.wanCfg.apn.value = "fetims";
		    break;
            case 3:
		    document.wanCfg.apn.value = "internet";
		    break;
            case 4:
		    document.wanCfg.apn.value = "internet";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   
   else if(location==1) //Australia
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "internet";
		    break;
	    case 1:
		    document.wanCfg.apn.value = "telstra.bigpond";
		    break;
            case 2:
		    document.wanCfg.apn.value = "dodolns1";
		    break;
            case 3:
		    document.wanCfg.apn.value = "exetel1";
		    break;
            case 4:
		    document.wanCfg.apn.value = "splns333a1";
		    break;
	    case 5:
		    document.wanCfg.apn.value = "internet";
		    break;
	    case 6:
		    document.wanCfg.apn.value = "telstra.internet";
		    break;
	    case 7:
		    document.wanCfg.apn.value = "3netaccess";
		    break;
	    case 8:
		    document.wanCfg.apn.value = "3services";
		    break;
	    case 9:
		    document.wanCfg.apn.value = "internet";
		    break;
	    case 10:
		    document.wanCfg.apn.value = "virginbroadband";
		    break;
	    case 11:
		    document.wanCfg.apn.value = "vfinternet.au";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==2) //Canada
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "internet.com";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==3) //China
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "";
		    break;
	    case 1:
		    document.wanCfg.apn.value = "cmnet";
		    break;
            case 2:
		    document.wanCfg.apn.value = "";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==4) //HongKong
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "ipc.three.com.hk";
		    break;
	    case 1:
		    document.wanCfg.apn.value = "internet";
		    break;
            case 2:
		    document.wanCfg.apn.value = "ineternet";
		    break;
            case 3:
		    document.wanCfg.apn.value = "pccw";
		    break;
	    case 4:
		    document.wanCfg.apn.value = "ineternet";
		    break;
	    case 5:
		    document.wanCfg.apn.value = "ineternet";
		    break;
	    case 6:
		    document.wanCfg.apn.value = "ineternet";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==5) //India
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "TATA";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==6) //Mexico
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "internet.movistar.mx";
		    break;
            case 1: 
	            document.wanCfg.apn.value = "internet.itelcel.com";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==7) //Netherland
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "internet";
		    break;
            case 1: 
	            document.wanCfg.apn.value = "internet";
		    break;
	    case 2: 
	            document.wanCfg.apn.value = "internet";
		    break;
	    case 3: 
	            document.wanCfg.apn.value = "internet";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==8) //Norway
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "Telenor";
		    break;
            case 1: 
	            document.wanCfg.apn.value = "Netcom";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==9) //Philippines
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "www.globe.com.ph";
		    break;
            case 1: 
	            document.wanCfg.apn.value = "internet";
		    break;
	    case 2: 
	            document.wanCfg.apn.value = "minternet";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==10) //Russia
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "internet.beeline.ru";
		    break;
            case 1: 
	            document.wanCfg.apn.value = "internet.nw";
		    break;
	    case 2: 
	            document.wanCfg.apn.value = "internet.mts.ru";
		    break;
	    case 3: 
	            document.wanCfg.apn.value = "internet.primtel.ru";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==11) //South Africa
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "";
		    break;
            case 1: 
	            document.wanCfg.apn.value = "internet";
		    break;
	    case 2: 
	            document.wanCfg.apn.value = "internet";
		    break;
	    case 3: 
	            document.wanCfg.apn.value = "";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
   else if(location==12) //USA
   {
        switch (srvProvider)
	{
	    case 0: 
	            document.wanCfg.apn.value = "proxy";
		    break;
            case 1: 
	            document.wanCfg.apn.value = "";
		    break;
	    case 2: 
	            document.wanCfg.apn.value = "cellular1wap";
		    break;
	    case 3: 
	            document.wanCfg.apn.value = "wap.gocbw.com";
		    break;
	    case 4: 
	            document.wanCfg.apn.value = "wap.voicestream.com";
		    break;
	    case 5: 
	            document.wanCfg.apn.value = "internet2.voicestream.com";
		    break;
	    case 6: 
	            document.wanCfg.apn.value = "";
		    break;
	    default:
		    document.wanCfg.apn.value = "";
		    break; 
	}
   }
 
}

function SrvPro_3g_Switch()
{
    var location = document.wanCfg.location3g.options.selectedIndex;
    var provider = document.wanCfg.srvPro3g.options.selectedIndex;
    show_apn(location, provider);
}

function show_3g_SrvProvider(location)
{
    var mysel = document.wanCfg.srvPro3g;
	mysel.options.length = 0;
    if (location == 0) //Taiwan
    {
        mysel.options[0] = new Option("Chunghwa Telecom", "0");
        mysel.options[1] = new Option("Far EasTone (internet)", "1"); 
        mysel.options[2] = new Option("Far EasTone (fetims)", "2"); 
        mysel.options[3] = new Option("Taiwan Cellular", "3"); 
        mysel.options[4] = new Option("Vibo", "4");  
    }
    else if (location == 1) //Australia
    {
        mysel.options[0] = new Option("iburst", "0");
        mysel.options[1] = new Option("Bigpond", "1");
        mysel.options[2] = new Option("Dodo", "2");
        mysel.options[3] = new Option("Exetel", "3");
        mysel.options[4] = new Option("Internode", "4");
        mysel.options[5] = new Option("Optus", "5");
        mysel.options[6] = new Option("Telstra", "6");
        mysel.options[7] = new Option("Three", "7");
        mysel.options[8] = new Option("Three Prepaid", "8");
        mysel.options[9] = new Option("TPG", "9");
        mysel.options[10] = new Option("Virgin", "10");
        mysel.options[11] = new Option("Vodafone", "11");
    }
    else if (location == 2) //Canada
    {
        mysel.options[0] = new Option("Rogers", "0");
    }
    else if (location == 3) //China
    {
        mysel.options[0] = new Option("China Telecom", "0");
        mysel.options[1] = new Option("China Mobile", "1");
        mysel.options[2] = new Option("China Unicom", "2");
    }
    else if (location == 4) //Hongkong
    {
        mysel.options[0] = new Option("3 HK", "0");
        mysel.options[1] = new Option("CSL", "1");
        mysel.options[2] = new Option("New World", "2");
        mysel.options[3] = new Option("PCCW", "3");
        mysel.options[4] = new Option("People", "4");
        mysel.options[5] = new Option("SmarTone", "5");
        mysel.options[6] = new Option("Sunday", "6");
    }
    else if (location == 5) //India
    {
        mysel.options[0] = new Option("TATA", "0");
    }
    else if (location == 6) //Mexico
    {
        mysel.options[0] = new Option("Movistar", "0");
        mysel.options[1] = new Option("Telcel", "1");
    }
    else if (location == 7) //Netherland
    {
        mysel.options[0] = new Option("T-Mobile", "0");
        mysel.options[1] = new Option("KPN", "1");
        mysel.options[2] = new Option("Telfort", "2");
        mysel.options[3] = new Option("Vodafone", "3");
    }
    else if (location == 8) //Norway
    {
        mysel.options[0] = new Option("Telenor Mobil", "0");
        mysel.options[1] = new Option("Netcom Mobil", "1");
    }
    else if (location == 9) //Philippines
    {
        mysel.options[0] = new Option("Globe", "0");
        mysel.options[1] = new Option("Smart", "1");
        mysel.options[2] = new Option("Sun Cellula", "2");
    }
    else if (location == 10) //Russia
    {
        mysel.options[0] = new Option("BeeLine", "0");
        mysel.options[1] = new Option("Megafon (NWGSM)", "1");
        mysel.options[2] = new Option("MTS", "2");
        mysel.options[3] = new Option("PrimTel", "3");
    }
    else if (location == 11) //South Africa
    {
        mysel.options[0] = new Option("MTN", "0");
        mysel.options[1] = new Option("Vodacom", "1");
        mysel.options[2] = new Option("Cell-C", "2");
        mysel.options[3] = new Option("Neotel", "3");
    }
     else if (location == 12) //USA
    {
        mysel.options[0] = new Option("AT&T", "0");
        mysel.options[1] = new Option("Bell Mobility", "1");
        mysel.options[2] = new Option("Cellular One", "2");
        mysel.options[3] = new Option("Cincinnati Bell", "3");
        mysel.options[4] = new Option("T-Mobile(T-Zone)", "4");
        mysel.options[5] = new Option("T-Mobile(internet)", "5");
        mysel.options[6] = new Option("Verizon", "6");
    }
}

function Location_3g_Switch()
{
    var location = document.wanCfg.location3g.options.selectedIndex;
    show_3g_SrvProvider(location);
    switch (location) //show the first ISP's APN of each location
    {
        case 0:
	        document.wanCfg.apn.value = "internet";
		break;
	case 1: 
	        document.wanCfg.apn.value = "internet";
		break;
	case 2: 
	        document.wanCfg.apn.value = "internet.com";
		break;
	case 3: 
	        document.wanCfg.apn.value = "";
		break;
	case 4: 
	        document.wanCfg.apn.value = "ipc.three.com.hk";
		break;
	case 5: 
	        document.wanCfg.apn.value = "TATA";
		break;
	case 6: 
	        document.wanCfg.apn.value = "internet.movistar.mx";
		break;
	case 7: 
	        document.wanCfg.apn.value = "internet";
		break;
	case 8: 
	        document.wanCfg.apn.value = "Telenor";
		break;
	case 9: 
	        document.wanCfg.apn.value = "www.globe.com.ph";
		break;
	case 10: 
	        document.wanCfg.apn.value = "internet.beeline.ru";
		break;
	case 11: 
	        document.wanCfg.apn.value = "";
		break;
	case 12: 
	        document.wanCfg.apn.value = "proxy";
		break;
        default:
		document.wanCfg.apn.value = "";
		break; 
    }
}


function ApnType3gChange()
{
    if (document.wanCfg.apnType3g[0].checked == true)
    {
      	document.wanCfg.location3g.disabled = false;
      	document.wanCfg.srvPro3g.disabled = false;
    }
    else
    {
        document.wanCfg.location3g.disabled = true;
      	document.wanCfg.srvPro3g.disabled = true;
    }
}

function show_3g_model(brand)
{
    var mysel = document.wanCfg.model3g;
	mysel.options.length = 0;
    if (brand == 0) //Auto
    {
        mysel.options[0] = new Option("Auto", "0"); //(text,value)
    }
    else if (brand == 1) //Huawei
    {
        mysel.options[0] = new Option("Generic", "0");
        mysel.options[1] = new Option("E156G", "1");
        mysel.options[2] = new Option("E160E", "2");
        mysel.options[3] = new Option("E160G", "3");
        mysel.options[4] = new Option("E161", "4");
        mysel.options[5] = new Option("E169", "5");
        mysel.options[6] = new Option("E1692", "6");
        mysel.options[7] = new Option("E169U", "7");
        mysel.options[8] = new Option("E170", "8");
        mysel.options[9] = new Option("E172", "9");
        mysel.options[10] = new Option("E1750", "10");
        mysel.options[11] = new Option("E180", "11");
        mysel.options[12] = new Option("E219", "12");
        mysel.options[13] = new Option("E220", "13");
        mysel.options[14] = new Option("E226", "14");
        mysel.options[15] = new Option("E270", "15");
        mysel.options[16] = new Option("E800", "16");
        mysel.options[17] = new Option("E1762", "17");
        mysel.options[18] = new Option("EC1260", "18");
        mysel.options[19] = new Option("ET128", "19");
        mysel.options[20] = new Option("K1550", "20");
        mysel.options[21] = new Option("K3715", "21");
        mysel.options[22] = new Option("E870", "22");
    }
    else if (brand == 2) //Huawei(Alternative)
    {
        mysel.options[0] = new Option("E630", "0");
    }
    else if (brand == 3) //ZTE
    {
        mysel.options[0] = new Option("Generic", "0");
        mysel.options[1] = new Option("MF622", "1");
        mysel.options[2] = new Option("MF622 (version 2)", "2");
        mysel.options[3] = new Option("MF628", "3");
        mysel.options[4] = new Option("MF628+", "4");
        mysel.options[5] = new Option("MF626", "5");
        mysel.options[6] = new Option("MF633", "6");
        mysel.options[7] = new Option("MF636", "7");
        mysel.options[8] = new Option("MF637", "8");
        mysel.options[9] = new Option("MF638", "9");
        mysel.options[10] = new Option("MU351", "10");
        mysel.options[11] = new Option("K3565-Z", "11");
        mysel.options[12] = new Option("ONDA MT503HS", "12");
        mysel.options[13] = new Option("ONDA MT505UP", "13");
        mysel.options[14] = new Option("ONDA MT525UP", "14");
    }
}

function Brand_3g_Switch()
{
    show_3g_model(document.wanCfg.brand3g.options.selectedIndex);
}

function pptp_mode_switch()
{
    if (document.wanCfg.pptpMode[0].checked == true)
	{		
		document.wanCfg.pptpIp.disabled = true;
	    document.wanCfg.pptpNetmask.disabled = true;
        document.wanCfg.pptpGateway.disabled = true;
	}
	else
	{
		document.wanCfg.pptpIp.disabled = false;
	    document.wanCfg.pptpNetmask.disabled = false;
        document.wanCfg.pptpGateway.disabled = false;
	}

}

function pptp_wanIP_mode_switch()
{
    if (document.wanCfg.pptpWanIPMode[0].checked == true)
	{		
		document.wanCfg.pptpWanIpAddr.disabled = true;
	}
	else
	{
		document.wanCfg.pptpWanIpAddr.disabled = false;
        document.wanCfg.dns1Type.options.selectedIndex=1;
        document.wanCfg.dns2Type.options.selectedIndex=1;
        document.wanCfg.staticPriDns.disabled = false;
        document.wanCfg.staticSecDns.disabled = false;
	}
}

function pptp_nailup_switch()
{
    if (document.wanCfg.pptpNailup.checked == true)
	{
		document.wanCfg.pptpIdleTime.disabled = true;
	}
	else
	{
		document.wanCfg.pptpIdleTime.disabled = false;
	}
}
function pptpNailup_Click()
{
	document.wanCfg.pptpNailup.value = document.wanCfg.pptpNailup.checked ? "1": "0";
	return true;
}

function nailup_3g_switch()
{
        if (document.wanCfg.nailedup3g.checked == true)
	{
		document.wanCfg.idleTime3g.disabled = true;
	}
	else
	{
		document.wanCfg.idleTime3g.disabled = false;
	}
}
function Nailup_3g_Click()
{
	document.wanCfg.nailedup3g.value = document.wanCfg.nailedup3g.checked ? "1": "0";
	return true;
}

function pppoe_nailup_switch()
{
    if (document.wanCfg.pppoeNailedup.checked == true)
	{
		document.wanCfg.pppoeIdleTime.disabled = true;
	}
	else
	{
		document.wanCfg.pppoeIdleTime.disabled = false;
	}
}
function pppoeNailup_Click()
{
	document.wanCfg.pppoeNailedup.value = document.wanCfg.pppoeNailedup.checked ? "1": "0";
	return true;
}
function pppoe_wanIP_mode_switch()
{
    if (document.wanCfg.pppoeWanIPMode[0].checked == true)
	{		
		document.wanCfg.pppoeWanIpAddr.disabled = true;
	}
	else
	{
		document.wanCfg.pppoeWanIpAddr.disabled = false;
        document.wanCfg.dns1Type.options.selectedIndex=1;
        document.wanCfg.dns2Type.options.selectedIndex=1;
        document.wanCfg.staticPriDns.disabled = false;
        document.wanCfg.staticSecDns.disabled = false;
	}
}

function l2tp_mode_switch()
{
    if (document.wanCfg.l2tpMode[0].checked == true)
	{		
		document.wanCfg.l2tpIp.disabled = true;
	    document.wanCfg.l2tpNetmask.disabled = true;
        document.wanCfg.l2tpGateway.disabled = true;
	}
	else
	{
		document.wanCfg.l2tpIp.disabled = false;
	    document.wanCfg.l2tpNetmask.disabled = false;
        document.wanCfg.l2tpGateway.disabled = false;
	}

}

function l2tp_wanIP_mode_switch()
{
    if (document.wanCfg.l2tpWanIPMode[0].checked == true)
	{		
		document.wanCfg.l2tpWanIpAddr.disabled = true;
	}
	else
	{
		document.wanCfg.l2tpWanIpAddr.disabled = false;
        document.wanCfg.dns1Type.options.selectedIndex=1;
        document.wanCfg.dns2Type.options.selectedIndex=1;
        document.wanCfg.staticPriDns.disabled = false;
        document.wanCfg.staticSecDns.disabled = false;
	}
}

/*
function l2tp_nailup_switch()
{
    if (document.wanCfg.l2tpNailup.checked == true)
	{
		document.wanCfg.l2tpIdleTime.disabled = true;
	}
	else
	{
		document.wanCfg.l2tpIdleTime.disabled = false;
	}
}
function l2tpNailup_Click()
{
	document.wanCfg.l2tpNailup.value = document.wanCfg.l2tpNailup.checked ? "1": "0";
	return true;
}
*/

function mac_clone_switch()
{
    if (document.wanCfg.WAN_MAC_Clone[0].checked == true) //Factory Default
	{
	   document.wanCfg.spoofIPAddr.disabled = true;
	   document.wanCfg.macCloneMac.disabled = true;
	}
	else if (document.wanCfg.WAN_MAC_Clone[1].checked == true) //SpoofIP
	{
	   document.wanCfg.spoofIPAddr.disabled = false;
	   document.wanCfg.macCloneMac.disabled = true;
	}
	else if (document.wanCfg.WAN_MAC_Clone[2].checked == true) //MacClone
	{
	   document.wanCfg.spoofIPAddr.disabled = true;
	   document.wanCfg.macCloneMac.disabled = false;
	}
}

function ethernet_mode_switch()
{
    if (document.wanCfg.WAN_IP_Auto[0].checked == true) //DHCP
    {		
        document.wanCfg.staticIp.disabled = true;
	document.wanCfg.staticNetmask.disabled = true;
        document.wanCfg.staticGateway.disabled = true;
        document.wanCfg.dns1Type.options.selectedIndex=0;
        document.wanCfg.dns2Type.options.selectedIndex=0;
        document.wanCfg.staticPriDns.disabled = true;
        //document.wanCfg.staticPriDns.value = "0.0.0.0";
        //document.wanCfg.staticPriDns.value = "";
        document.wanCfg.staticSecDns.disabled = true;
        //document.wanCfg.staticSecDns.value = "0.0.0.0";
        //document.wanCfg.staticSecDns.value = "";
        
        document.wanCfg.bigPondEnb[0].disabled = false;
        document.wanCfg.bigPondEnb[1].disabled = false;
        if (document.wanCfg.bigPondEnb[0].checked == true) //BigPond enabled
        {
            document.wanCfg.bigPondSrv.disabled = false;
      	    document.wanCfg.bigPondUser.disabled = false;
      	    document.wanCfg.bigPondPass.disabled = false;
      	    document.wanCfg.bigPondPass2.disabled = false;
        }
        else
        {
            document.wanCfg.bigPondSrv.disabled = true;
      	    document.wanCfg.bigPondUser.disabled = true;
      	    document.wanCfg.bigPondPass.disabled = true;
      	    document.wanCfg.bigPondPass2.disabled = true;
        }
    }
    else //Static IP
    {
	document.wanCfg.staticIp.disabled = false;
	document.wanCfg.staticNetmask.disabled = false;
        document.wanCfg.staticGateway.disabled = false;
        document.wanCfg.dns1Type.options.selectedIndex=1;
        document.wanCfg.dns2Type.options.selectedIndex=1;
        document.wanCfg.staticPriDns.disabled = false;
        document.wanCfg.staticSecDns.disabled = false;
        
        document.wanCfg.bigPondEnb[0].disabled = true;
        document.wanCfg.bigPondEnb[1].disabled = true;
        document.wanCfg.bigPondSrv.disabled = true;
      	document.wanCfg.bigPondUser.disabled = true;
      	document.wanCfg.bigPondPass.disabled = true;
      	document.wanCfg.bigPondPass2.disabled = true;
    }
}

function DNS1TypeSwitch()
{
   if ((document.wanCfg.dns1Type.options.selectedIndex == 0) ||
       (document.wanCfg.dns1Type.options.selectedIndex == 2))
   {
        document.wanCfg.staticPriDns.disabled = true;
        //document.wanCfg.staticPriDns.value = "0.0.0.0";
        //document.wanCfg.staticPriDns.value = "";
   }
   else
   {
        document.wanCfg.staticPriDns.disabled = false;
   }
}

function DNS2TypeSwitch()
{
   if ((document.wanCfg.dns2Type.options.selectedIndex == 0) ||
       (document.wanCfg.dns2Type.options.selectedIndex == 2))
   {
        document.wanCfg.staticSecDns.disabled = true;
        //document.wanCfg.staticSecDns.value = "0.0.0.0";
        //document.wanCfg.staticSecDns.value = "";
   }
   else
   {
        document.wanCfg.staticSecDns.disabled = false;
   }
}

function DnsUpdate()
{
      if (isAllNum(document.wanCfg.staticPriDns.value) == 0) //dns1 not all numbers
      {
          if ((document.wanCfg.staticSecDns.value != "" ) &&
              (isAllNum(document.wanCfg.staticSecDns.value) == 1)) //dns2 all numbers
          {
             document.wanCfg.staticPriDns.value=document.wanCfg.staticSecDns.value;
             document.wanCfg.staticSecDns.value="";
          }
          else
          {
            document.wanCfg.staticPriDns.value="";
            document.wanCfg.staticSecDns.value="";
          }  
      }
      else
      {
          if ((document.wanCfg.staticSecDns.value != "" ) &&
              (isAllNum(document.wanCfg.staticSecDns.value) == 0)) //dns2 not all numbers
          {
             document.wanCfg.staticSecDns.value="";
          }
      }     
  
  if (document.wanCfg.dns1Type.options.selectedIndex == 2)
  {
      if (document.wanCfg.dns2Type.options.selectedIndex == 2)
      {
           document.wanCfg.staticSecDns.value="";
      }
      else
      {
          if ((document.wanCfg.staticPriDns.value != "" ) &&
              (isAllNum(document.wanCfg.staticPriDns.value) == 1)) //dns1 all numbers
          {
             document.wanCfg.staticSecDns.value=document.wanCfg.staticPriDns.value;
          }
      }
      document.wanCfg.staticPriDns.value="";   
  }
  if (document.wanCfg.dns2Type.options.selectedIndex == 2)
  {
      document.wanCfg.staticSecDns.value="";
  }
  
  if ((document.wanCfg.dns1Type.options.selectedIndex == 0)) //ISP
  {
      if (document.wanCfg.staticPriDns.value == "0.0.0.0" )
            document.wanCfg.staticPriDns.value="";
  } 
}

function updateStatus()
{
        //Static IP or DHCP
        if (wan_dhcp_fixedip == "0") //DHCP
        {
              document.wanCfg.WAN_IP_Auto[0].checked = true;
              document.wanCfg.staticIp.disabled = true;
	      document.wanCfg.staticNetmask.disabled = true;
              document.wanCfg.staticGateway.disabled = true;
              
      	      document.wanCfg.bigPondEnb[0].disabled = false;
              document.wanCfg.bigPondEnb[1].disabled = false;
      	      if(bigPond_enb == "1") //BigPond enabled
      	      {
      	         document.wanCfg.bigPondSrv.disabled = false;
      	         document.wanCfg.bigPondUser.disabled = false;
      	         document.wanCfg.bigPondPass.disabled = false;
      	         document.wanCfg.bigPondPass2.disabled = false;
      	      }
      	      else
      	      {
      	         document.wanCfg.bigPondSrv.disabled = true;
      	         document.wanCfg.bigPondUser.disabled = true;
      	         document.wanCfg.bigPondPass.disabled = true;
      	         document.wanCfg.bigPondPass2.disabled = true;
      	      }
        }
        else //Static IP
        {
	      document.wanCfg.WAN_IP_Auto[1].checked = true;
	      document.wanCfg.staticIp.disabled = false;
	      document.wanCfg.staticNetmask.disabled = false;
              document.wanCfg.staticGateway.disabled = false;
              
              document.wanCfg.bigPondEnb[0].disabled = true;
              document.wanCfg.bigPondEnb[1].disabled = true;
              document.wanCfg.bigPondSrv.disabled = true;
      	      document.wanCfg.bigPondUser.disabled = true;
      	      document.wanCfg.bigPondPass.disabled = true;
      	      document.wanCfg.bigPondPass2.disabled = true;
	}
	//BigPond
        if(bigPond_enb == "1") //enabled
        {
            document.wanCfg.bigPondEnb[0].checked = true;
        }
        else
        {
            document.wanCfg.bigPondEnb[1].checked = true;
        }
        if (bigPond_srv == "61.9.208.13")
        {
           document.wanCfg.bigPondSrv.options.selectedIndex = 0;
        }
        else if (bigPond_srv == "61.9.192.13")
        {
           document.wanCfg.bigPondSrv.options.selectedIndex = 1;
        }
        else if (bigPond_srv == "61.9.128.13")
        {
           document.wanCfg.bigPondSrv.options.selectedIndex = 2;
        }
        else if (bigPond_srv == "61.9.224.13")
        {
           document.wanCfg.bigPondSrv.options.selectedIndex = 3;
        }
        else if (bigPond_srv == "61.9.240.13")
        {
           document.wanCfg.bigPondSrv.options.selectedIndex = 4;
        }
        
        //PPPoE
        if (pppoeNailup == "1") //Nailup=checked
        {
	        document.wanCfg.pppoeNailedup.checked = true;
	        document.wanCfg.pppoeIdleTime.disabled = true;
	    }
        else
        {
            document.wanCfg.pppoeNailedup.checked = false;
            document.wanCfg.pppoeIdleTime.disabled = false;
        }
        if (pppoeWanIPmode == "1") //DHCP for PPPoE WAN IP Address assigned
        {     
	        document.wanCfg.pppoeWanIPMode[0].checked = true;
	        document.wanCfg.pppoeWanIpAddr.disabled = true;
	    }
        else //Static IP for PPPoE WAN IP Address assigned
        {
            document.wanCfg.pppoeWanIPMode[1].checked = true;
            document.wanCfg.pppoeWanIpAddr.disabled = false;
	    }
        
        //PPTP
        if (pptpNailup == "1") //Nailup=checked
        {
	        document.wanCfg.pptpNailup.checked = true;
	        document.wanCfg.pptpIdleTime.disabled = true;
	    }
        else
        {
            document.wanCfg.pptpNailup.checked = false;
            document.wanCfg.pptpIdleTime.disabled = false;
        }
        if (pptpmode == "1") //DHCP for PPTP IP Address
        {     
	        document.wanCfg.pptpMode[0].checked = true;
	        document.wanCfg.pptpIp.disabled = true;
	        document.wanCfg.pptpNetmask.disabled = true;
            document.wanCfg.pptpGateway.disabled = true;
	    }
        else //Static IP for PPTP IP Address
        {
            document.wanCfg.pptpMode[1].checked = true;
            document.wanCfg.pptpIp.disabled = false;
	        document.wanCfg.pptpNetmask.disabled = false;
            document.wanCfg.pptpGateway.disabled = false;
        }
        if (pptpWanIPmode == "1") //DHCP for PPTP WAN IP Address assigned
        {     
	        document.wanCfg.pptpWanIPMode[0].checked = true;
	        document.wanCfg.pptpWanIpAddr.disabled = true;
	    }
        else //Static IP for PPTP WAN IP Address assigned
        {
            document.wanCfg.pptpWanIPMode[1].checked = true;
            document.wanCfg.pptpWanIpAddr.disabled = false;
	    }
        //L2TP
        /*
        if (l2tpNailup == "1")
        {
	        document.wanCfg.l2tpNailup.checked = true;
	        document.wanCfg.l2tpIdleTime.disabled = true;
	    }
        else
        {
            document.wanCfg.l2tpNailup.checked = false;
            document.wanCfg.l2tpIdleTime.disabled = false;
        }
        */
        if (l2tpmode == "1") //DHCP for L2TP IP Address
        {     
	        document.wanCfg.l2tpMode[0].checked = true;
	        document.wanCfg.l2tpIp.disabled = true;
	        document.wanCfg.l2tpNetmask.disabled = true;
            document.wanCfg.l2tpGateway.disabled = true;
	    }
        else //Static IP for L2TP IP Address
        {
            document.wanCfg.l2tpMode[1].checked = true;
            document.wanCfg.l2tpIp.disabled = false;
	        document.wanCfg.l2tpNetmask.disabled = false;
            document.wanCfg.l2tpGateway.disabled = false;
        }
        if (l2tpWanIPmode == "1") //DHCP for L2TP WAN IP Address assigned
        {     
	        document.wanCfg.l2tpWanIPMode[0].checked = true;
	        document.wanCfg.l2tpWanIpAddr.disabled = true;
	    }
        else //Static IP for L2TP WAN IP Address assigned
        {
            document.wanCfg.l2tpWanIPMode[1].checked = true;
            document.wanCfg.l2tpWanIpAddr.disabled = false;
	    }
        
        DNS1TypeSwitch();
        DNS2TypeSwitch();
        //DnsUpdate();

        //WAN MAC Clone
        if (macCloneMode == "0") //Factory Default
        {     
	        document.wanCfg.WAN_MAC_Clone[0].checked = true;
	        document.wanCfg.spoofIPAddr.disabled = true;
	        document.wanCfg.macCloneMac.disabled = true;
	    }
        else if (macCloneMode == "1") //SpoofIP
        {
            document.wanCfg.WAN_MAC_Clone[1].checked = true;
            document.wanCfg.spoofIPAddr.disabled = false;
	        document.wanCfg.macCloneMac.disabled = true;
        }
        else if (macCloneMode == "2") //MacClone
        {
            document.wanCfg.WAN_MAC_Clone[2].checked = true;
            document.wanCfg.spoofIPAddr.disabled = true;
	        document.wanCfg.macCloneMac.disabled = false;
        }
        
        //3G configurations
        document.wanCfg.apn.value = apn_3g;
        if (nailup_3g == "1") //Nailup=checked
        {
	    document.wanCfg.nailedup3g.checked = true;
	    document.wanCfg.idleTime3g.disabled = true;
	}
        else
        {
            document.wanCfg.nailedup3g.checked = false;
            document.wanCfg.idleTime3g.disabled = false;
        }
        /*
        if (brand_3g == 0)
        {
           document.wanCfg.brand3g.options.selectedIndex = 0;
        }
        else if (brand_3g == 1)
        {
           document.wanCfg.brand3g.options.selectedIndex = 1;
        }
        else if (brand_3g == 2)
        {
           document.wanCfg.brand3g.options.selectedIndex = 2;
        }
        else if (brand_3g == 3)
        {
           document.wanCfg.brand3g.options.selectedIndex = 3;
        }
        show_3g_model(brand_3g);
        document.wanCfg.model3g.options.selectedIndex = model_3g;
        
	if(apnType_3g == "0")
	{
		document.wanCfg.apnType3g[0].checked = true;
		document.wanCfg.location3g.disabled = false;
      	        document.wanCfg.srvPro3g.disabled = false;
	}
	else if(apnType_3g == "1")
	{
		document.wanCfg.apnType3g[1].checked = true;
		document.wanCfg.location3g.disabled = true;
      	        document.wanCfg.srvPro3g.disabled = true;
	}
	
	if (location_3g == 0)
        {
           document.wanCfg.location3g.options.selectedIndex = 0;
        }
        else if (location_3g == 1)
        {
           document.wanCfg.location3g.options.selectedIndex = 1;
        }
        else if (location_3g == 2)
        {
           document.wanCfg.location3g.options.selectedIndex = 2;
        }
        else if (location_3g == 3)
        {
           document.wanCfg.location3g.options.selectedIndex = 3;
        }
        else if (location_3g == 3)
        {
           document.wanCfg.location3g.options.selectedIndex = 3;
        }
        else if (location_3g == 4)
        {
           document.wanCfg.location3g.options.selectedIndex = 4;
        }
        else if (location_3g == 5)
        {
           document.wanCfg.location3g.options.selectedIndex = 5;
        }
        else if (location_3g == 6)
        {
           document.wanCfg.location3g.options.selectedIndex = 6;
        }
        else if (location_3g == 7)
        {
           document.wanCfg.location3g.options.selectedIndex = 7;
        }
        else if (location_3g == 8)
        {
           document.wanCfg.location3g.options.selectedIndex = 8;
        }
        else if (location_3g == 9)
        {
           document.wanCfg.location3g.options.selectedIndex = 9;
        }
        else if (location_3g == 10)
        {
           document.wanCfg.location3g.options.selectedIndex = 10;
        }
        else if (location_3g == 11)
        {
           document.wanCfg.location3g.options.selectedIndex = 11;
        }
        else if (location_3g == 12)
        {
           document.wanCfg.location3g.options.selectedIndex = 12;
        }
	show_3g_SrvProvider(location_3g);
	document.wanCfg.srvPro3g.options.selectedIndex = srvPro_3g;
	
	if (auth_3g == "chap")
        {
           document.wanCfg.auth3g.options.selectedIndex = 0;
        }
        else if (auth_3g == "pap")
        {
           document.wanCfg.auth3g.options.selectedIndex = 1;
        }
        else if (auth_3g == "none")
        {
           document.wanCfg.auth3g.options.selectedIndex = 2;
        }
        
        if (connMode_3g == "auto")
        {
           document.wanCfg.connMode3g.options.selectedIndex = 0;
        }
        else if (connMode_3g == "hsdpa")
        {
           document.wanCfg.connMode3g.options.selectedIndex = 1;
        }
        else if (connMode_3g == "umts")
        {
           document.wanCfg.connMode3g.options.selectedIndex = 2;
        }
        else if (connMode_3g == "edge")
        {
           document.wanCfg.connMode3g.options.selectedIndex = 3;
        }
        else if (connMode_3g == "gprs")
        {
           document.wanCfg.connMode3g.options.selectedIndex = 4;
        }
        
        if(pppConnType == "0") //keep alive
	{
		document.wanCfg.pppType[0].checked = true;
		document.wanCfg.maxIdle.disabled = true;
      	        document.wanCfg.echoIntvl.disabled = false;
      	        document.wanCfg.retryThreld.disabled = false;
	}
	else if(pppConnType == "1") //on demand
	{
		document.wanCfg.pppType[1].checked = true;
		document.wanCfg.maxIdle.disabled = false;
      	        document.wanCfg.echoIntvl.disabled = true;
      	        document.wanCfg.retryThreld.disabled = true;
	}
	
	if(turbo_3g == "1")
	{
	  document.wanCfg.turbo3g[0].checked = true; 
	}
	else if(turbo_3g == "0")
	{
	  document.wanCfg.turbo3g[1].checked = true; 
	}
	*/
}

function initValue()
{
   var mode = "<% getCfgGeneral(1, "wanConnectionMode"); %>";
   var dns1_type = "<% getCfgGeneral(1, "wan_dns1_type"); %>";
   var dns2_type = "<% getCfgGeneral(1, "wan_dns2_type"); %>";
           
    show_div(false, "PPPoE_setting");
	show_div(false, "PPTP_setting1");
	show_div(false, "PPTP_setting2");  
	show_div(false, "L2TP_setting1");
	show_div(false, "L2TP_setting2");
	show_div(false, "Ethernet_setting");
	show_div(false, "3G_setting");
     
   if ((mode == "STATIC") || (mode == "DHCP"))
   {
  		document.wanCfg.connectionType.options.selectedIndex = 0;
	        show_div(true, "Ethernet_setting");
   }
   else if (mode == "PPPOE") {
		document.wanCfg.connectionType.options.selectedIndex = 1;
		show_div(true, "PPPoE_setting");
   }
   else if (mode == "PPTP") {
            document.wanCfg.connectionType.options.selectedIndex = 2;
	    show_div(true, "PPTP_setting1");
	    show_div(true, "PPTP_setting2");
   }
   else if (mode == "L2TP") {
            document.wanCfg.connectionType.options.selectedIndex = 3;
	    show_div(true, "L2TP_setting1");
	    show_div(true, "L2TP_setting2");
   }
   else if (mode == "WWAN") {
            document.wanCfg.connectionType.options.selectedIndex = 4;
	    show_div(true, "3G_setting");
   }
   //for dns1 and dns2 types
   if (dns1_type == "ISP")
   {
       document.wanCfg.dns1Type.options.selectedIndex = 0;
   }
   else if (dns1_type == "USER")
   {
       document.wanCfg.dns1Type.options.selectedIndex = 1;
   }
   else if (dns1_type == "NONE")
   {   document.wanCfg.dns1Type.options.selectedIndex = 2;

   }    
   if (dns2_type == "ISP")
   {
       document.wanCfg.dns2Type.options.selectedIndex = 0;
   }
   else if (dns2_type == "USER")
   {
       document.wanCfg.dns2Type.options.selectedIndex = 1;
   }
   else if (dns2_type == "NONE")
   {   document.wanCfg.dns2Type.options.selectedIndex = 2;

   }        
   
   var e = document.getElementById("wantitle");
	e.innerHTML = _("wan title");
	e = document.getElementById("wanEncapsulation");
	e.innerHTML = _("wan Encapsulation");
	e = document.getElementById("wanEthernet");
	e.innerHTML = _("wan Ethernet");
	e = document.getElementById("wanPPPOE");
	e.innerHTML = _("wan PPPOE");
	e = document.getElementById("wanPPTP");
	e.innerHTML = _("wan PPTP");
	e = document.getElementById("wanL2TP");
	e.innerHTML = _("wan L2TP");
	e = document.getElementById("wPppoeUser");
	e.innerHTML = _("wPppoe User");
	e = document.getElementById("wPppoePassword");
	e.innerHTML = _("wPppoe Password");
        e = document.getElementById("wPppoePass2");
	e.innerHTML = _("wPppoe Pass2");
	e = document.getElementById("wPppoeMtu");
	e.innerHTML = _("wPppoe Mtu");
	e = document.getElementById("wPppoeNailedup");
	e.innerHTML = _("wPppoe Nailedup");
	e = document.getElementById("wPppoeIdleTime");
	e.innerHTML = _("wPppoe IdleTime");
	e = document.getElementById("wPppoeIdleTimesec");
	e.innerHTML = _("wPppoe IdleTimesec");
	e = document.getElementById("wPppoeSrvName");
	e.innerHTML = _("wPppoe service name");
        
       
	e = document.getElementById("wantitle2");
	e.innerHTML = _("wan title2");
	e = document.getElementById("wanPppoeWanIPMode");
	e.innerHTML = _("wPppoe WanIPMode");
	e = document.getElementById("wanPppoeWanIPMode1");
	e.innerHTML = _("wPppoe WanIPMode1");
	e = document.getElementById("wPppoeWanIp");
	e.innerHTML = _("wPppoe WanIp");
	e = document.getElementById("wPptpUser");
	e.innerHTML = _("wPptp User");
	e = document.getElementById("wPptpPassword");
	e.innerHTML = _("wPptp Password");
	e = document.getElementById("wPptpPass2");
	e.innerHTML = _("wPptp Pass2");
	e = document.getElementById("wPptpNailup");
	e.innerHTML = _("wPptp Nailup");
	e = document.getElementById("wPptpIdleTime");
	e.innerHTML = _("wPptp IdleTime");
	e = document.getElementById("wPptpIdleTimesec");
	e.innerHTML = _("wPptp IdleTimesec");
	

	e = document.getElementById("wPptptitle");
	e.innerHTML = _("wPptp title");
	e = document.getElementById("wPptpServer");
	e.innerHTML = _("wPptp Server");
	e = document.getElementById("wanPptpAddrMode");
	e.innerHTML = _("wPptp AddrMode");
	e = document.getElementById("wPptpAddrMode1");
	e.innerHTML = _("wPptp AddrMode1");
	e = document.getElementById("wPptpIp");
	e.innerHTML = _("wPptp Ip");
	e = document.getElementById("wPptpNetmask");
	e.innerHTML = _("wPptp Netmask");
	e = document.getElementById("wPptpGateway");
	e.innerHTML = _("wPptp Gateway");
	e = document.getElementById("wPptptitle1");
	e.innerHTML = _("wPptp title1");
	e = document.getElementById("wPptpWanIPMode1");
	e.innerHTML = _("wPptp WanIPMode1");
	e = document.getElementById("wPptpWanIPMode2");
	e.innerHTML = _("wPptp WanIPMode2");
	e = document.getElementById("wPptpWanIp");
	e.innerHTML = _("wPptp WanIp");
	e = document.getElementById("wL2tpUser");
	e.innerHTML = _("wL2tp User");
	e = document.getElementById("wL2tpPassword");
	e.innerHTML = _("wL2tp Password");
	e = document.getElementById("wL2tpPass2");
	e.innerHTML = _("wL2tp Pass2");
	e = document.getElementById("L2TPtitle");
	e.innerHTML = _("L2TP title");
	e = document.getElementById("wL2tpServer");
	e.innerHTML = _("wL2tp Server");
	e = document.getElementById("wL2tpAddrMode1");
	e.innerHTML = _("wL2tp AddrMode1");
	e = document.getElementById("wL2tpAddrMode2");
	e.innerHTML = _("wL2tp AddrMode2");
	e = document.getElementById("wL2tpIp");
	e.innerHTML = _("wL2tp Ip");
	e = document.getElementById("wL2tpNetmask");
	e.innerHTML = _("wL2tp Netmask");
	e = document.getElementById("wL2tpGateway");
	e.innerHTML = _("wL2tp Gateway");
	e = document.getElementById("ethernettitle");
	e.innerHTML = _("ethernet title");
	e = document.getElementById("ethernet_d1");
	e.innerHTML = _("ethernet d1");
	e = document.getElementById("ethernet_d2");
	e.innerHTML = _("ethernet d2");
	e = document.getElementById("wStaticIp");
	e.innerHTML = _("wStatic Ip");
	e = document.getElementById("wStaticNetmask");
	e.innerHTML = _("wStatic Netmask");
	e = document.getElementById("wStaticGateway");
	e.innerHTML = _("wStatic Gateway");
	e = document.getElementById("wantitle3");
	e.innerHTML = _("wan title3");
	e = document.getElementById("wanDNS");
	e.innerHTML = _("wan DNS");
	e = document.getElementById("wanDNSISP");
	e.innerHTML = _("wan DNSISP");
	e = document.getElementById("wanDNSUSER");
	e.innerHTML = _("wan DNSUSER");
	e = document.getElementById("wanDNSNONE");
	e.innerHTML = _("wan DNSNONE");
	e = document.getElementById("wanDNS2");
	e.innerHTML = _("wan DNS2");
	e = document.getElementById("wanDNS2ISP");
	e.innerHTML = _("wan DNS2ISP");
	e = document.getElementById("wanDNS2USER");
	e.innerHTML = _("wan DNS2USER");
	e = document.getElementById("wanDNS2NONE");
	e.innerHTML = _("wan DNS2NONE");
	e = document.getElementById("wantitle4");
	e.innerHTML = _("wan title4");
	e = document.getElementById("wanmacop1");
	e.innerHTML = _("wan macop1");
	e = document.getElementById("wanmacop2");
	e.innerHTML = _("wan macop2");
	e = document.getElementById("wanmacop3");
	e.innerHTML = _("wan macop3");

	//3G
	e = document.getElementById("3gtitle");
	e.innerHTML = _("wan 3G title");
        //e = document.getElementById("wan3G"); //marked temporarily
	//e.innerHTML = _("wan 3G");
	e = document.getElementById("wApn");
	e.innerHTML = _("wan 3G access point name");
	e = document.getElementById("wPin");
	e.innerHTML = _("wan 3G pin");
	e = document.getElementById("wUser3g");
	e.innerHTML = _("wan 3G username");
	e = document.getElementById("wPass3g");
	e.innerHTML = _("wan 3G password");
	e = document.getElementById("wDial");
	e.innerHTML = _("wan 3G dial number");
	e = document.getElementById("w3gNailedup");
	e.innerHTML = _("wan 3G nailup");
	e = document.getElementById("w3gIdleTime");
	e.innerHTML = _("wan 3G IdleTime");
	e = document.getElementById("w3gIdleTimesec");
	e.innerHTML = _("wan 3G IdleTimesec");
	
	//BigPond
	e = document.getElementById("wBigPond");
	e.innerHTML = _("wan BigPond");
	e = document.getElementById("bigpond_enable");
	e.innerHTML = _("wan BigPond enable");
	e = document.getElementById("bigpond_disable");
	e.innerHTML = _("wan BigPond disable");
	e = document.getElementById("wBigPondSrv");
	e.innerHTML = _("wan BigPond server");
	e = document.getElementById("bigPondSrv_queenland");
	e.innerHTML = _("wan BigPond server queenland");
	e = document.getElementById("bigPondSrv_newSouthWales");
	e.innerHTML = _("wan BigPond server newSouthWales");
	e = document.getElementById("bigPondSrv_victoria");
	e.innerHTML = _("wan BigPond server victoria");
	e = document.getElementById("bigPondSrv_southAus");
	e.innerHTML = _("wan BigPond server southAus");
	e = document.getElementById("bigPondSrv_westernAus");
	e.innerHTML = _("wan BigPond server westernAus");
	e = document.getElementById("wBigPondUser");
	e.innerHTML = _("wan BigPond username");
	e = document.getElementById("wBigPondPass");
	e.innerHTML = _("wan BigPond password");
	e = document.getElementById("wBigPondPass2");
	e.innerHTML = _("wan BigPond password2");
	
	e = document.getElementById("wanApply");
	e.value = _("wan Apply");
	e = document.getElementById("wanReset");
	e.value = _("wan Reset");
	
   updateStatus();
//   parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
}
</script>
</head>



<body onLoad="initValue()">
<form method=post name="wanCfg" action="/goform/setWanNormal" onSubmit="return CheckValue()">

    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="wantitle">ISP Parameters for Internet Access</td>
                   </tr></table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wanEncapsulation">Encapsulation : </td>
                      <td><select name="connectionType" size="1" onChange="connectionTypeSwitch();">
                          <option value="Ethernet" id="wanEthernet">Ethernet</option>
                          <option value="PPPOE" id="wanPPPOE">PPP over Ethernet</option>
                          <option value="PPTP" id="wanPPTP">PPTP</option>
                          <option value="L2TP" id="wanL2TP">L2TP</option>
                          <!--<option value="WWAN" id="wan3G">Mobile 3G</option>-->
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li> 
     <span id = "PPPoE_setting" class = "off" >
     <!-- PPPoE Settings (BEGIN) -->
                <!--
                <li class="w_text">
                  <table id="PPPoEServiceName" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPPPoEServiceName">Service Name :</td>
                      <td><span class="cfig_word">
                        <input name="pppoeServiceName" maxlength=32 size=32 value="<% getCfgGeneral(1, "wan_pppoe_service_name"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                -->
                <li class="w_text">
                  <table id="PPPoEUserName" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPppoeUser">User Name :</td>
                      <td><span class="cfig_word">
                        <input name="pppoeUser" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_pppoe_user"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPPoEPassword" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPppoePassword">Password :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="pppoePass" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_pppoe_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPPoEPassword2" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPppoePass2">Retype to Confirm :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="pppoePass2" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_pppoe_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPPoESrvName" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPppoeSrvName">Service Name :</td>
                      <td><span class="cfig_word">
                        <input name="pppoeSrvName" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_pppoe_srvname"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPPoEMtu" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPppoeMtu">MTU Size :</td>
                      <td><span class="cfig_word">
                        <input name="pppoeMtu" maxlength=4 size=8 value="<% getCfgGeneral(1, "wan_pppoe_mtu"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                
                <li class="w_text">
                  <table id="PPPoENailup" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="3%"><span class="cfig_word">
                        <input type="checkbox" name="pppoeNailedup" value="<% getCfgZero(1, "wan_pppoe_nailup"); %>" onclick="pppoe_nailup_switch()">
                        <a id="wPppoeNailedup">Nailed-Up Connection</a>
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPPoEIdleTime" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPppoeIdleTime">Idle Timeout (sec)</td>
                      <td ><span class="cfig_word">
                        <input width="12%" name="pppoeIdleTime" maxlength=5 size=8 value="<% getCfgGeneral(1, "wan_pppoe_optime"); %>">
                      <a id="wPppoeIdleTimesec">(in seconds)</a></span></td>
                    </tr>  
                  </table>
                </li>
                
                <li class="space3"></li>
                <li class="title" id="wantitle2">WAN IP Address Assignment</li>
                <li class="w_text">
                  <table id="PPPoEWanIPMode_d" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wPppoeWanIPMode" width="1%"><input    type="radio" value="1" onclick="pppoe_wanIP_mode_switch()" name="pppoeWanIPMode" />
                      <a id="wanPppoeWanIPMode">Get automatically from ISP </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPPoEWanIPMode_s" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wPppoeWanIPMode" width="1%"><input    type="radio" value="0" onclick="pppoe_wanIP_mode_switch()"
                        name="pppoeWanIPMode" />
                        <a id="wanPppoeWanIPMode1">Use Fixed IP Address </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPPoEWanIp" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPppoeWanIp">My WAN IP Address : </td>
                      <td><span class="cfig_word">
                        <!--<input name="pppoeWanIpAddr" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pppoe_wanip_addr"); %>">-->
                            <input name="pppoeWanIpAddr" maxlength=15 size=15 value="<% getPPPoEMyWanIp(); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
    <!-- PPPoE Settings (END)-->
    </span>
    <span id = "PPTP_setting1" class = "off" >
    <!-- PPTP Settings1 (BEGIN) -->               
                <li class="w_text">
                  <table id="PPTPUserName" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpUser">User Name :</td>
                      <td><span class="cfig_word">
                        <input name="pptpUser" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_pptp_user"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPPassword" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpPassword">Password :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="pptpPass" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_pptp_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPPassword2" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpPass2">Retype to Confirm :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="pptpPass2" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_pptp_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPNailup" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="3%"><span class="cfig_word">
                        <input type="checkbox" name="pptpNailup" value="<% getCfgZero(1, "wan_pptp_nailup"); %>" onclick="pptp_nailup_switch()">
                        <a id="wPptpNailup">Nailed-Up Connection</a>
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPIdleTime" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpIdleTime">Idle Timeout (sec)</td>
                      <td ><span class="cfig_word">
                        <input width="12%" name="pptpIdleTime" maxlength=5 size=8 value="<% getCfgGeneral(1, "wan_pptp_optime"); %>">
                      <a id="wPptpIdleTimesec">(in seconds)</a></span></td>
                    </tr>  
                  </table>
                </li>
    <!-- PPTP Settings1 (END)-->
    </span>
    <span id = "L2TP_setting1" class = "off" >
    <!-- L2TP Settings1 (BEGIN) -->            
                <li class="w_text">
                  <table id="L2TPUserName" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpUser">User Name :</td>
                      <td><span class="cfig_word">
                        <input name="l2tpUser" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_l2tp_user"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPPassword" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpPassword">Password :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="l2tpPass" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_l2tp_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPPassword2" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpPass2">Retype to Confirm :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="l2tpPass2" maxlength=50 size=32 value="<% getCfgGeneral(1, "wan_l2tp_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <!--
                <li class="w_text">
                  <table id="L2TPNailup" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="3%"><span class="cfig_word">
                        <input type="checkbox" name="l2tpNailup" value="<% getCfgZero(1, "wan_l2tp_nailup"); %>" onclick="l2tp_nailup_switch()">L2TP Nailed-Up Connection
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPIdleTime" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpIdleTime">L2TP Idle Timeout(sec)</td>
                      <td><span class="cfig_word">
                        <input name="l2tpIdleTime" maxlength=5 size=8 value="<% getCfgGeneral(1, "wan_l2tp_optime"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                -->
    <!-- L2TP Settings1 (END)-->
    </span>          
    <span id = "PPTP_setting2" class = "off" >
    <!-- PPTP Settings2 (BEGIN) -->           
                <li class="space3"></li>
                <li class="title" id="wPptptitle">PPTP Configuration</li>
                <li class="w_text">
                  <table id="PPTPServerIP" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpServer">Server IP Address : </td>
                      <td><span class="cfig_word">
                        <input name="pptpServer" maxlength="15" value="<%getCfgGeneral(1, "wan_pptp_server"); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPAddrMode_d" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td  id="wPptpAddrMode"><input width="1%" type="radio" value="1" onclick="pptp_mode_switch()" name="pptpMode" />
                      <a id="wanPptpAddrMode">Get automatically from ISP </a></td>
                    </tr>
                  </table>
                </li>
                
                <li class="w_text">
                  <table id="PPTPAddrMode_s" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wPptpAddrMode" ><input width="1%" type="radio" value="0" onclick="pptp_mode_switch()"
                        name="pptpMode" />
                        <a id="wPptpAddrMode1">Use Fixed IP Address </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPIp" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpIp">IP Address : </td>
                      <td><span class="cfig_word">
                        <!--<input name="pptpIp" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pptp_ip"); %>">-->
                            <input name="pptpIp" maxlength=15 size=15 value="<% getPhyWanIfIp(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPNetMask" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpNetmask">IP Subnet Mask : </td>
                      <td><span class="cfig_word">
                        <!--<input name="pptpNetmask" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pptp_netmask"); %>">-->
                            <input name="pptpNetmask" maxlength=15 size=15 value="<% getPhyWanIfNetMask(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPGateway" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpGateway">Gateway IP Address : </td>
                      <td><span class="cfig_word">
                        <!--<input name="pptpGateway" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pptp_gateway"); %>">-->
                            <input name="pptpGateway" maxlength=15 size=15 value="<% getPhyWanIfGateway(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="space3"></li>
                <li class="title" id="wPptptitle1">WAN IP Address Assignment</li>
                <li class="w_text">
                  <table id="PPTPWanIPMode_d" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wPptpWanIPMode"><input width="1%" type="radio" value="1" onclick="pptp_wanIP_mode_switch()" name="pptpWanIPMode" />
                      <a id="wPptpWanIPMode1">Get automatically from ISP </a></td>
                    </tr>
                  </table>
                </li>
                
                <li class="w_text">
                  <table id="PPTPWanIPMode_s" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td  id="wPptpWanIPMode"><input width="1%" type="radio" value="0" onclick="pptp_wanIP_mode_switch()"
                        name="pptpWanIPMode" />
                        <a id="wPptpWanIPMode2">Use Fixed IP Address </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="PPTPWanIp" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPptpWanIp">My WAN IP Address : </td>
                      <td><span class="cfig_word">
                        <!--<input name="pptpWanIpAddr" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_pptp_wanip_addr"); %>">-->
                        <input name="pptpWanIpAddr" maxlength=15 size=15 value="<% getPPTPMyWanIp(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
    <!-- PPTP Settings2 (END)-->
    </span>           
    <span id = "L2TP_setting2" class = "off" >
    <!-- L2TP Settings2 (BEGIN) -->           
                <li class="space3"></li>
                <li class="title" id="L2TPtitle">L2TP Configuration</li>
                <li class="w_text">
                  <table id="L2TPServerIP" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpServer">Server IP Address : </td>
                      <td><span class="cfig_word">
                        <input name="l2tpServer" maxlength="15" value="<%getCfgGeneral(1, "wan_l2tp_server"); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPAddrMode_d" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wL2tpAddrMode"><input width="1%" type="radio" value="1" onclick="l2tp_mode_switch()" name="l2tpMode" />
                      <a id="wL2tpAddrMode1">Get automatically from ISP </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPAddrMode_s" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wl2tpAddrMode"><input width="1%" type="radio" value="0" onclick="l2tp_mode_switch()"
                        name="l2tpMode" />
                        <a id="wL2tpAddrMode2">Use Fixed IP Address </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPIp" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpIp">IP Address : </td>
                      <td><span class="cfig_word">
                        <!--<input name="l2tpIp" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_l2tp_ip"); %>">-->
                            <input name="l2tpIp" maxlength=15 size=15 value="<% getPhyWanIfIp(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPNetMask" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpNetmask">IP Subnet Mask : </td>
                      <td><span class="cfig_word">
                        <!--<input name="l2tpNetmask" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_l2tp_netmask"); %>">-->
                            <input name="l2tpNetmask" maxlength=15 size=15 value="<% getPhyWanIfNetMask(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPGateway" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpGateway">Gateway IP Address : </td>
                      <td><span class="cfig_word">
                        <!--<input name="l2tpGateway" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_l2tp_gateway"); %>">-->
                            <input name="l2tpGateway" maxlength=15 size=15 value="<% getPhyWanIfGateway(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>               
                <li class="space3"></li>
                <li class="title">WAN IP Address Assignment</li>
                <li class="w_text">
                  <table id="L2TPWanIPMode_d" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wL2tpWanIPMode"><input type="radio" value="1" onclick="l2tp_wanIP_mode_switch()" name="l2tpWanIPMode" />Get automatically from ISP </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPWanIPMode_s" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="wL2tpWanIPMode"><input type="radio" value="0" onclick="l2tp_wanIP_mode_switch()"
                        name="l2tpWanIPMode" />Use Fixed IP Address </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="L2TPWanIp" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wL2tpWanIp">My WAN IP Address : </td>
                      <td><span class="cfig_word">
                        <!--<input name="l2tpWanIpAddr" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_l2tp_wanip_addr"); %>">-->
                            <input name="l2tpWanIpAddr" maxlength=15 size=15 value="<% getL2TPMyWanIp(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
    <!-- L2TP Settings2 (END)-->
    </span> 
    <span id = "Ethernet_setting" class = "off" >
    <!-- Ethernet Settings (BEGIN) -->            
                <li class="space3"></li>
                <li class="title" id="ethernettitle">WAN IP Address Assignment </li>
                <li class="w_text">
                  <table id="ethernet_d" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%"><input type="radio" value="0" onclick="ethernet_mode_switch()" name="WAN_IP_Auto" />
                      <a id="ethernet_d1">Get automatically from ISP (Default) </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr><td width="40%" id=wBigPond>Bigpond : </td>
                        <td><input name="bigPondEnb" value="1" onclick="bigPondEnbChange()" type="radio" />&nbsp;<font id=bigpond_enable>Enable</font>                 
                            <input name="bigPondEnb" value="0" onclick="bigPondEnbChange()" type="radio" />&nbsp;<font id=bigpond_disable>Disable</font>
                        </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wBigPondSrv">Server : </td>
                      <td><select name="bigPondSrv" size="1">
                          <option value="61.9.208.13" id="bigPondSrv_queenland">Queensland (61.9.208.13)</option>
                          <option value="61.9.192.13" id="bigPondSrv_newSouthWales">New South Wales (61.9.192.13)</option>
                          <option value="61.9.128.13" id="bigPondSrv_victoria">Victoria (61.9.128.13)</option>
                          <option value="61.9.224.13" id="bigPondSrv_southAus">South Australia (61.9.224.13)</option>
                          <option value="61.9.240.13" id="bigPondSrv_westernAus">Western Australia (61.9.240.13)</option>
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wBigPondUser">User Name : </td>
                      <td><span class="cfig_word">
                        <input name="bigPondUser" maxlength=50 size=32 value="<% getCfgGeneral(1, "bigPond_username"); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="BigPondPassword" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wBigPondPass">Password :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="bigPondPass" maxlength=50 size=32 value="<% getCfgGeneral(1, "bigPond_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="BigPondPassword2" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wBigPondPass2">Retype to Confirm :</td>
                      <td><span class="cfig_word">
                        <input type="password" name="bigPondPass2" maxlength=50 size=32 value="<% getCfgGeneral(1, "bigPond_pass"); %>">
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="ethernet_s" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%"><input type="radio" value="1" onclick="ethernet_mode_switch()"  
                        name="WAN_IP_Auto" />
                       <a id="ethernet_d2">Use Fixed IP Address </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="StaticIp" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wStaticIp">IP Address : </td>
                      <td><span class="cfig_word">
                            <input name="staticIp" maxlength=15 value="<% getPhyWanIfIp(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="StaticNetmask" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wStaticNetmask">IP Subnet Mask : </td>
                      <td><span class="cfig_word">
                        <input name="staticNetmask" maxlength=15 value="<% getPhyWanIfNetMask(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table id="StaticGateway" width="92%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wStaticGateway">Gateway IP Address : </td>
                      <td><span class="cfig_word">
                        <input name="staticGateway" maxlength=15 value="<% getPhyWanIfGateway(); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
    <!-- Ethernet Settings (END)-->
    </span>
    <span id = "3G_setting" class = "off" >
    <!-- 3G Settings (BEGIN) -->            
                <li class="space3"></li>
                <li class="title" id="3gtitle">3G Configuration </li>
                <!--
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wBrand3g">Modem Brand : </td>
                      <td><select name="brand3g" size="1" onChange="Brand_3g_Switch();">
                          <option value="0" id="brand3g_Auto">Auto</option>
                          <option value="1" id="brand3g_Huawei">Huawei</option>
                          <option value="2" id="brand3g_Huawei_2">Huawei (Alternative)</option>
                          <option value="3" id="brand3g_ZTE">ZTE</option>
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wModel3g">Modem Model : </td>
                      <td><select name="model3g" size="1">
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr><td width="40%" id=wApnType3g>APN Type : </td>
                        <td><input name="apnType3g" value="0" onclick="ApnType3gChange()" type="radio" /><font id=srv_provider>Service Provider</font>                 
                            <input name="apnType3g" value="1" onclick="ApnType3gChange() "type="radio" /><font id=srv_manual>Manual</font>
                        </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wLocation3g">Location : </td>
                      <td><select name="location3g" size="1" onChange="Location_3g_Switch();">
                          <option value="0" id="location3g_Taiwan">Taiwan</option>
                          <option value="1" id="location3g_Australia">Australia</option>
                          <option value="2" id="location3g_Huawei_2">Canada</option>
                          <option value="3" id="location3g_China">China</option>
                          <option value="4" id="location3g_Hongkong">Hongkong</option>
                          <option value="5" id="location3g_India">India</option>
                          <option value="6" id="location3g_Mexico">Mexico</option>
                          <option value="7" id="location3g_Netherland">Netherland</option>
                          <option value="8" id="location3g_Norway">Norway</option>
                          <option value="9" id="location3g_Philippines">Philippines</option>
                          <option value="10" id="location3g_Russia">Russia</option>
                          <option value="11" id="location3g_South Africa">South Africa</option>
                          <option value="12" id="location3g_USA">USA</option>
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wSrvPro3g">Service Provider : </td>
                      <td><select name="srvPro3g" size="1" onChange="SrvPro_3g_Switch();">
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                -->
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPin">PIN Code : </td>
                      <td><span class="cfig_word">
                        <input name="pin3g" size="25" maxlength="8" value="<% getCfgGeneral(1, "pin_3g"); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wApn">APN Code : </td>
                      <td><span class="cfig_word">
                            <input name="apn" size="25">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <!--
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wAuth3g">Authentication : </td>
                      <td><select name="auth3g" size="1">
                          <option value="chap" id="auth3g_chap">CHAP (Auto)</option>
                          <option value="pap" id="auth3g_pap">PAP</option>
                          <option value="none" id="auth3g_none">None</option>
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                -->
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wDial">Dial Number : </td>
                      <td><span class="cfig_word">
                            <input name="dial3g" size="25" value="<% getCfgGeneral(1, "dial_3g"); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wUser3g">User Name : </td>
                      <td><span class="cfig_word">
                        <input name="username3g" size="25" value="<% getCfgGeneral(1, "username_3g"); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wPass3g">Password : </td>
                      <td><span class="cfig_word">
                        <input type="password" name="password3g" size="25" value="<% getCfgGeneral(1, "password_3g"); %>">
                      </span></td>
                    </tr>
                  </table>
                </li>
                <!--
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wConnMode3g">Connection Mode : </td>
                      <td><select name="connMode3g" size="1">
                          <option value="auto" id="conn3g_auto">Auto</option>
                          <option value="hsdpa" id="conn3g_hsdpa">HSDPA / 3.5G</option>
                          <option value="umts" id="conn3g_umts">UMTS / 3G</option>
                          <option value="edge" id="conn3g_edge">EDGE / 2.5G</option>
                          <option value="gprs" id="conn3g_gprs">GPRS / 2G</option>
                          </select> 
                      </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr><td width="40%" id=wPppType>PPP Connection Type : </td>
                        <td><input name="pppType" value="0" onclick="pppTypeChange()" type="radio" /><font id=ppp_keep>Keep Alive</font>                 
                            <input name="pppType" value="1" onclick="pppTypeChange()" type="radio" /><font id=ppp_demand>On Demand</font>
                        </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wMaxIdle">Max Idle Time : </td>
                      <td><span class="cfig_word">
                            <input name="maxIdle" size="5" value="<% getCfgGeneral(1, "max_idle_time"); %>">&nbsp;Seconds (60~3600)
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wEchoIntvl">PPP Echo Interval : </td>
                      <td><span class="cfig_word">
                            <input name="echoIntvl" size="5" value="<% getCfgGeneral(1, "ppp_echo_interval"); %>">&nbsp;Seconds (3~50)
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wRetryThreld">PPP Retry Threshold : </td>
                      <td><span class="cfig_word">
                            <input name="retryThreld" size="5" value="<% getCfgGeneral(1, "ppp_retry_threshold"); %>">&nbsp;Time(s) (3~50)
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wMtu3g">Mobile WAN MTU : </td>
                      <td><span class="cfig_word">
                            <input name="mtu3g" size="5" value="<% getCfgGeneral(1, "mtu_3g"); %>">&nbsp;Bytes (592~1492)
                      </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr><td width="40%" id=wTurbo3g>TurboLink (Enable it might increase your 3G data charge) : </td>
                        <td><input name="turbo3g" value="1" type="radio" /><font id=turbo3g_enable>Enable</font>                 
                            <input name="turbo3g" value="0" type="radio" /><font id=turbo3g_disable>Disable</font>
                        </td>
                    </tr>
                  </table>
                </li>
                -->
                <li class="w_text">
                  <table id="Nailup3g" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="3%"><span class="cfig_word">
                        <input type="checkbox" name="nailedup3g" value="<% getCfgZero(1, "wan_3g_nailup"); %>" onclick="nailup_3g_switch()">
                        <a id="w3gNailedup">Nailed-Up Connection</a>
                      </span></td>
                    </tr>  
                  </table>
                </li>
                <li class="w_text">
                  <table id="IdleTime3g" width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="w3gIdleTime">Idle Timeout (sec)</td>
                      <td ><span class="cfig_word">
                        <input width="12%" name="idleTime3g" maxlength=5 size=8 value="<% getCfgGeneral(1, "max_idle_time"); %>">
                      <a id="w3gIdleTimesec">(in seconds)</a></span></td>
                    </tr>  
                  </table>
                </li>         
    <!-- 3G Settings (END)-->
    </span>
                <li class="space3"></li>
                <li class="title" id="wantitle3">WAN DNS Assignment </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wanDNS">First DNS Server : </td>
                      <td><select name="dns1Type" size="1" onChange="DNS1TypeSwitch();">
                          <option value="ISP" id="wanDNSISP">From ISP</option>
                          <option value="USER" id="wanDNSUSER">User-Defined</option>
                          <option value="NONE" id="wanDNSNONE">None</option>
                        </select>
                          <!--<input name="staticPriDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_primary_dns"); %>"></td>-->
                              <input name="staticPriDns" maxlength=15 size=15 value="<% getDns(1); %>"></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="wanDNS2">Second DNS Server : </td>
                      <td><select name="dns2Type" size="1" onChange="DNS2TypeSwitch();">
                          <option value="ISP" id="wanDNS2ISP">From ISP</option>
                          <option value="USER" id="wanDNS2USER">User-Defined</option>
                          <option value="NONE" id="wanDNS2NONE">None</option>
                        </select>
                          <!--<input name="staticSecDns" maxlength=15 size=15 value="<% getCfgGeneral(1, "wan_secondary_dns"); %>"></td>-->
                              <input name="staticSecDns" maxlength=15 size=15 value="<% getDns(2); %>"></td>
                    </tr>
                  </table>
                </li>
                 
                <li class="space3"></li>
                <li class="title" id="wantitle4">WAN MAC Address</li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%"><input onclick="mac_clone_switch()" 
                        type="radio" value="0" name="WAN_MAC_Clone" />
                        <a id="wanmacop1">Factory default </a></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%"><input onclick="mac_clone_switch()" 
                        type="radio" value="1" name="WAN_MAC_Clone" /></td>
                        <td  width="38%" id="wanmacop2">Clone the computer's MAC address - IP Address</td>
   
                       <td> <input maxlength="15" size="15" name="spoofIPAddr"
                        value="<% getCfgGeneral(1, "spoofIpCloneMac"); %>"  /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%"><input onclick="mac_clone_switch()" 
                        type="radio" value="2" name="WAN_MAC_Clone" /></td>

                         <td  width="38%" id="wanmacop3"> Set WAN MAC Address</td>
                        <td><input maxlength="17" 
                        name="macCloneMac" size="17" 
                        value="<% getCfgGeneral(1, "macCloneMac"); %>" /></td>
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
        <input type=submit value="Apply" id="wanApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <!--<input value="Upload" id="uploadFWApply" name="UploadFirmwareSubmit" type="submit" >-->
        <input type=reset value="Reset" id="wanReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/wan.asp" name="wan_url">
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
