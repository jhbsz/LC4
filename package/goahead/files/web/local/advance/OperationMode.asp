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
Butterlate.setTextDomain("wireless");

function show_div(show,id)
{
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
	}

function initTranslation()
{
	var e = document.getElementById("OPModeTitle");
	e.innerHTML = _("op mode title");

	e = document.getElementById("OPModeRouter");
	e.innerHTML = _("op mode router");

	e = document.getElementById("OPModeAP");
	e.innerHTML = _("op mode ap");
	
	e = document.getElementById("OPModeUR");
	e.innerHTML = _("op mode ur");	
	
	e = document.getElementById("OPModeWISP");
	e.innerHTML = _("op mode wisp");

	e = document.getElementById("OPModeNote1_0");
	e.innerHTML = _("op mode note1_0");
	
	e = document.getElementById("OPModeNote1_1");
	e.innerHTML = _("op mode note1_1");
	
	e = document.getElementById("OPModeNote1_2");
	e.innerHTML = _("op mode note1_2");

	e = document.getElementById("OPModeNote1_3");
	e.innerHTML = _("op mode note1_3");
	
	e = document.getElementById("OPModeNote1_4");
	e.innerHTML = _("op mode note1_4");					

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}

function initValue()
{
	initTranslation();
	
	var nv_OP_Mode = <% getCfgZero(1, "OP_Mode"); %>;

	if (nv_OP_Mode == 0)
		document.CfgOperationMode.OPMode[0].checked = true;
	else if (nv_OP_Mode == 1)
		document.CfgOperationMode.OPMode[1].checked = true;
	else if (nv_OP_Mode == 2)
		document.CfgOperationMode.OPMode[2].checked = true;
	else if (nv_OP_Mode == 3)
		document.CfgOperationMode.OPMode[3].checked = true;
	else if (nv_OP_Mode == 4)
		document.CfgOperationMode.OPMode[4].checked = true;
	else if (nv_OP_Mode == 5)
		document.CfgOperationMode.OPMode[5].checked = true;
	else if (nv_OP_Mode == 6)
		document.CfgOperationMode.OPMode[6].checked = true;

	show_div(false, "div_hidden_some_op_modes");
//	parent.getElement("myFrame").height = document.body.scrollHeight;	
  parent.adjustMyFrameHeight();
}


lanip_router = "<% getCfgGeneral(1, "lan_ipaddr_router"); %>";
lanip_ap = "<% getCfgGeneral(1, "lan_ipaddr_ap"); %>";
function changmod_msg_Router()
{
alert(_("- Ports are LAN 4 Ethernet LAN ports, WLAN, WAN")+"\n"+_("- LAN DHCP server is configurable")+"\n"+_("- LAN IP is")+lanip_router+"\n"+_("- WAN IP is configurable"));      
}
function changmod_msg_WISP()
{
alert(_("- Ports are LAN 5 Ethernet LAN ports, WLAN as WAN")+"\n"+_("- LAN DHCP server is configurable")+"\n"+_("- LAN IP is")+lanip_router+"\n"+_("- WAN IP is configurable"));      
}
function changmod_msg_AP()
{
alert(_("- Ports are LAN 5 Ethernet LAN ports,WLAN")+"\n"+_("- LAN DHCP server is disabled & unconfigurable")+"\n"+_("- LAN IPb is")+lanip_ap+"\n"+_("- WAN IP is unreachable"));      
}
</script>
</head>
<body onLoad="initValue()">
<form method=post name="CfgOperationMode" action="/goform/SetOperationMode" >
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                 <li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td id="OPModeTitle" nowrap>Configuration Mode</td>
                   </tr>
                  </table>
                 </li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="0" onClick="changmod_msg_Router()"/><font id ="OPModeRouter">Router Mode</font></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="1" onClick="changmod_msg_AP()"/><font id ="OPModeAP">Access Point Mode</font></td>
</tr>
</table>
</li>

<span id="div_hidden_some_op_modes" class="off"> <!-- hidden some OP Modes -->
<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="2" />Client Mode</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="3" />Bridge Mode (WDS)</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="4" />AP + Repeater Mode (AP +WDS)</td>
</tr>
</table>
</li>
</span> <!-- hidden some OP Modes -->

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="5" onClick="changmod_msg_AP()"/><font id ="OPModeUR">Universal Repeater Mode</font></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td nowrap><input type="radio" name="OPMode" value="6" onClick="changmod_msg_WISP()"/><font id ="OPModeWISP">WISP Mode</font></td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align=center cellspacing="0">
<tr>
<td>&nbsp;</td>
</tr>
</table>
</li>

<li class="w_text">
<table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><span class="i_note" id="OPModeNote1_0">Note:</span></td>
</tr>
</table>
</li>
<li class="w_text">
<table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td class="i_note_a" id="OPModeNote1_1">Router: In this mode, the device is supported to connect to internet via ADSL/Cable Modem. PCs in LAN ports share the same IP to ISP through WAN Port.</td>
</tr>
<tr>
<td class="i_note_a" id="OPModeNote1_2">Access Point: In this mode, all Ethernet ports are bridged together. The device allows the wireless-equipped computer can communicate with a wired network.</td>
</tr>
<tr>
<td class="i_note_a" id="OPModeNote1_4">Universal Repeater Mode: In this mode, the device acts as both access point and wireless client. It can transmit wireless traffic between two wireless networks.</td>
</tr>
<tr>
<td class="i_note_a" id="OPModeNote1_3">WISP Mode: In this mode, the device acts as a wireless client. It can connect to an existing network via an access point. Also router functions are added between the wireless WAN and the LAN.</td>
</tr>
</table>
</li>
<li></li>
<li></li>
<li></li>
<li></li>
<li></li>
<li></li>
<li></li>
<li></li>
              </ul>
            </div>
        <center>
        <li class="table_button">        
        <input type=submit value="Apply" id="GeneralApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset  value="Reset" id="GeneralReset" onClick="window.location.reload()">        
        </li>
        </center>
</form>        
</body>
<script>
//function alert(str)
//{
//	showWebMessage(1, str);
//}
//showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>