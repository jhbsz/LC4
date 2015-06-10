<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<script type="text/javascript" src="js/jquery-1.2.6.js"></script>
<script type="text/javascript" src="js/networkmap.js"></script>
<script type="text/javascript" src="js/jquery.jcontext.1.0.js"></script>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/DST.js"></script>
<link href="images/css.css" rel="stylesheet" type="text/css" />
<link href="/table.css" rel="stylesheet" type="text/css" />
<script   language="javascript">   
Butterlate.setTextDomain("main");

var nv_OP_Mode = <% getCfgZero(1, "OP_Mode"); %>;

function initTranslation()
{
	var e = document.getElementById("id_Home");
	e.innerHTML = _("networkmap Home");
	e = document.getElementById("id_ExpertMode");
	e.innerHTML = _("networkmap ExpertMode");
	e = document.getElementById("id_Logout");
	e.innerHTML = _("networkmap Logout");
	e = document.getElementById("id_Modem");
	e.innerHTML = _("networkmap Modem");
	e = document.getElementById("id_Status");
	e.innerHTML = _("networkmap Status");
	e = document.getElementById("id_Name");
	e.innerHTML = _("networkmap Name");
	e = document.getElementById("id_Time");
	e.innerHTML = _("networkmap Time");
//	e = document.getElementById("id_WANIP");
//	e.innerHTML = _("networkmap WANIP");
	e = document.getElementById("id_MACAddress");
	e.innerHTML = _("networkmap MACAddress");
	e = document.getElementById("id_FirmwareVersion");
	e.innerHTML = _("networkmap FirmwareVersion");
	e = document.getElementById("id_WirelessNetworkNameSSID");
	e.innerHTML = _("networkmap WirelessNetworkNameSSID");
	e = document.getElementById("id_Security");
	e.innerHTML = _("networkmap Security");
	e = document.getElementById("id_NetworkMAP");
	e.innerHTML = _("networkmap NetworkMAP");
	e = document.getElementById("id_GameEngine");
	e.innerHTML = _("networkmap GameEngine");
	e = document.getElementById("id_ParenetalControl");
	e.innerHTML = _("networkmap ParenetalControl");
	e = document.getElementById("id_BandwidthMGMT");
	e.innerHTML = _("networkmap BandwidthMGMT");
	e = document.getElementById("id_Firewall");
	e.innerHTML = _("networkmap Firewall");
	e = document.getElementById("id_WirelessSecurity");
	e.innerHTML = _("networkmap WirelessSecurity");
	e = document.getElementById("id_PowerSaving");
	e.innerHTML = _("networkmap PowerSaving");
	e = document.getElementById("id_ScanningyourNetwork");
	e.innerHTML = _("networkmap ScanningyourNetwork");
	e = document.getElementById("g2-1No");
	e.innerHTML = _("g2-1 No");
	e = document.getElementById("g2-1Yes");
	e.innerHTML = _("g2-1 Yes");
	e = document.getElementById("log_out");
	e.innerHTML = _("dash logout title");
	e = document.getElementById("ask_logout");
	e.innerHTML = _("dash logout mes");
}

   function   loadFile(){ 
		var frame = document.getElementById("contenthtml");
		var loading = document.getElementById("load");
		frame.onreadystatechange = function(){
    		if( this.readyState == "complete" ) {
				loading.style.display="none";
			}	
		}
  }   
  </script>  
  
  <script>
<!--
function stateChangeIE(_frame)
    { 
     if (_frame.readyState=="complete")//state: loading ,interactive,   complete
     {
     var loader = document.getElementById("load"); 
        loader.innerHTML      = "";    
        loader.style.display = "none";  
        _frame.style.visibility = "visible";   
     }   
    }
    function stateChangeFirefox(_frame)
    { 
     var loader = document.getElementById("load"); 
        loader.innerHTML      = "";    
        loader.style.display = "none";  
        _frame.style.visibility = "visible";   
    }

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
//-->

function closepopup()
{
		var bH=$(window).height();
		var bW=$(window).width();
		$("#b-mask").css({width:bW,height:bH,display:"none"});
		$(".layer1").css({display:"none"});
}
</script>   

</head>
<body onLoad="initTranslation()">
<div id="popup"></div>
<div id="wrapper">
<div class="in_bg" id="in_bg"></div>
<div class="in_line" id="topline" ></div>
<div class="frame_left"></div>
<div class="frame_right"></div>
<div class="unit_myrouter"></div>
<div class="unit_title" id="unitword">NBG4615</div>
<!-- Hotkey"-->
<div class="hotkey" id="Hotkey">
  <ul><li class="home"><a href="index_login.asp" id=id_Home></a></li><li class="expert"><a href="/local/advance/dashboard.asp" id=id_ExpertMode></a></li><li class="logout"><a href="#" onclick="MM_showHideLayers('popup','','show','popup3_table','','show')" id=id_Logout></a></li></ul></div>
<!-- Hotkey"-->
<div class=crop>
<div class=widearea>
<div id="position">
<div id="left_arrow"><a href="javascript: current_ethernet_start_index++; easy_info.show_map();" id=ethernet_left_arrow></a></div>
<div id="right_arrow"><a href="javascript: current_ethernet_start_index--; easy_info.show_map();" id=ethernet_right_arrow></a></div>
<div id="left_arrow2"><a href="javascript: current_wireless_start_index++; easy_info.show_map();" id=wireless_left_arrow></a></div>
<div id="right_arrow2"><a href="javascript: current_wireless_start_index--; easy_info.show_map();" id=wireless_right_arrow></a></div>
<div class="popup"><ul><li class="popuptop"></li><li class="popupmainUrl"><a href="javascript: refreshmap();">- Refresh </a></li><li class="popupmainUrl"><a href="genie.asp" target="_parent">- eaZy123 </a></li>
<li class="popupdown"></li></ul></div>
<div class="earth" id=earth_id></div>
<div class="housing" id="housing"><ul><li class="img"><a href="#"></a></li><li class="word_button"><div align="center"><a href="#">NBG4615</a></div></li >
</ul></div>
<div class="modem"><ul><li class="img"><a></a></li><li class="word_button"><div align="center"><a href="#" id=id_Modem></a></div></li ></ul></div>
<div class="ethernet">
<div id="ethernet_item1"><ul><li class="img"><a href="javascript: easy_info.item_connect(1,1);" id=ethernet_img1></a></li><li class="word_button"><div align="center"><a href="javascript: easy_info.item_show_info(1,1);" id=ethernet_name1></a></div></li ></ul></div>
<div id="ethernet_item2"><ul><li class="img"><a href="javascript: easy_info.item_connect(1,2);" id=ethernet_img2></a></li><li class="word_button"> <div align="center"><a href="javascript: easy_info.item_show_info(1,2);" id=ethernet_name2></a></div></li ></ul></div>
<div id="ethernet_item3"><ul><li class="img"><a href="javascript: easy_info.item_connect(1,3);" id=ethernet_img3></a></li><li class="word_button"><div align="center"><a href="javascript: easy_info.item_show_info(1,3);" id=ethernet_name3></a></div></li ></ul></div>
<div id="ethernet_item4"><ul><li class="img"><a href="javascript: easy_info.item_connect(1,4);" id=ethernet_img4></a></li><li class="word_button"><div align="center"><a href="javascript: easy_info.item_show_info(1,4);" id=ethernet_name4></a></div></li ></ul></div> 
</div>
<div class="ethernet2">
<div id="wifi1" title="WiFi"></div>
<div id="wifi2" title="WiFi"></div>
<div id="wifi3" title="WiFi"></div>
<div id="wifi4" title="WiFi"></div>
<div id="ethernet2_item1"><ul><li class="img"><a href="javascript: easy_info.item_connect(2,1);" id=ethernet2_img1></a></li><li class="word_button"><div align="center"><a href="javascript: easy_info.item_show_info(2,1);" id=ethernet2_name1></a></div></li ></ul></div>
<div id="ethernet2_item2"><ul><li class="img"><a href="javascript: easy_info.item_connect(2,2);" id=ethernet2_img2></a></li><li class="word_button"> <div align="center"><a href="javascript: easy_info.item_show_info(2,2);" id=ethernet2_name2></a></div></li ></ul></div>
<div id="ethernet2_item3"><ul><li class="img"><a href="javascript: easy_info.item_connect(2,3);" id=ethernet2_img3></a></li><li class="word_button"><div align="center"><a href="javascript: easy_info.item_show_info(2,3);" id=ethernet2_name3></a></div></li ></ul></div>
<div id="ethernet2_item4"><ul><li class="img"><a href="javascript: easy_info.item_connect(2,4);" id=ethernet2_img4></a></li><li class="word_button"><div align="center"><a href="javascript: easy_info.item_show_info(2,4);" id=ethernet2_name4></a></div></li ></ul></div> 
<!--
<div id="game2"><ul><li class="img"><a href="#"></a></li><li class="word_button"> <div align="center"><a href="#">WiFi2 Game </a></div></li ></ul></div>
<div id="ap3"><ul><li class="img"><a href="#"></a></li><li class="word_button"><div align="center"><a href="#">WiFi3 AP... </a></div></li ></ul></div>
-->
</div>
<div id="error1"></div>
<div id="error2"></div>
<!--
<div id="error3"></div>
-->
<!--
<div id="error4"></div>
-->
<div class="vline" id=vline_id></div>
<div class="vline_error" id=vline_error_id></div>
<div class="vline2"></div>
<!--
<div class="vline2_error"></div>
-->
<div class="vline3"></div>
<!--
<div class="vline3_error"></div>
-->
<div class="hline"></div>
<!--
<div class="hline_error"></div>
-->
<div class="hline2"></div>
<!--
<div class="hline2_error"></div>
-->
<div class="hline3"></div>
<!--
<div class="hline3_error"></div>
-->
<div class="hline4" id=hline4_id></div>
<div class="hline4_error" id=hline4_error_id></div>
   <div class="i_status" id="i_status"><a href="#" id=id_Status></a></div>
      </div>
<div id="position2">
  <div class="map_word"><table width="434" border="0" cellpadding="0" cellspacing="0">
  <tr><td class="map_title" id=id_Name> Name : </td><td class="map_content"><% getCfgGeneral(1, 'SystemName'); %></td></tr>
  <tr><td class="map_title" id=id_Time> Time : </td> <td class="map_content"><span id="mtenCurrent_Date"></span>&nbsp;<span id="mtenCurrent_Time"></span></td> </tr>
<script>
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)) {
	document.write("<tr><td class='map_title' id=id_WANIP> WAN IP : </td> <td class='map_content'><% getWanIp(); %></td></tr>");	
}
else
{
	document.write("<tr><td class='map_title' id=id_LANIP> LAN IP : </td> <td class='map_content'><% getLanIp(); %></td></tr>");	
}
</script>
  <tr> <td class="map_title" id=id_MACAddress> MAC Address : </td><td class="map_content"><% getLanMac(); %></td></tr>
  <tr><td class="map_title" id=id_FirmwareVersion> Firmware Version : </td><td class="map_content"><% getFirmwareVersion(); %></td></tr>
  <tr><td colspan="2" class="line_map">&nbsp;</td> </tr>
  <tr><td class="map_title" id=id_WirelessNetworkNameSSID> Wireless Network Name (SSID) : </td><td class="map_content"><% getCfgGeneral(1, "SSID1"); %></td></tr> 
  <tr><td class="map_title" id=id_Security> Security : </td><td class="map_content">
<script>
	var op_mode = <% getCfgZero(1, "OP_Mode"); %>;
	if (op_mode == 6){
		var profile = "<% getCfgZero(1, 'staProfile'); %>";
		if (profile == ""){
			document.write(_("dash_info wlan sec nosec"));
		}else{
			var Auth = <% getCfgZero(1, 'staAuth'); %>;
			var wepKey1 = "<% getCfgZero(1, 'staKey1'); %>";
			var wepKey2 = "<% getCfgZero(1, 'staKey2'); %>";
			var wepKey3 = "<% getCfgZero(1, 'staKey3'); %>";
			var wepKey4 = "<% getCfgZero(1, 'staKey4'); %>";
			
			if ((Auth == 0) && (wepKey1 == "0") && (wepKey2 == "0") && (wepKey3 == "0") && (wepKey4 == "0")){
				document.write(_("dash_info wlan sec nosec"));
			}else if ((Auth == 0) || (Auth == 1)){
				document.write("Static WEP");
			}else if (Auth == 4){
				document.write("WPA-PSK");
			}else if (Auth == 7){
				document.write("WPA2-PSK");
			}
		}
	
	}else{
var ieee802_1x = "<% getCfgZero(1, 'IEEE8021X'); %>"
var Auth = "<% getCfgZero(1, 'AuthMode'); %>"
var Encry = "<% getCfgZero(1, 'EncrypType'); %>"

if (ieee802_1x.charAt(0) == 1 ){
	document.write("802.1x");
}else if ((Auth.split(";",1) == "OPEN") && (Encry.split(";",1) == "NONE")){
			document.write(_("dash_info wlan sec nosec"));
}else if (Auth.split(";",1) == "SHARED"){
	document.write("Static WEP");
}else if (Auth.split(";",1) == "WPAPSK"){
	document.write("WPA-PSK");
}else if (Auth.split(";",1) == "WPA2PSK"){
	document.write("WPA2-PSK");
}else if (Auth.split(";",1) == "WPAPSKWPA2PSK"){
	document.write("WPA-PSK/WPA2-PSK");
		}else if (Auth.split(";",1) == "WPAWPA2"){
	document.write("WPA/WPA2");
}else{
	document.write(Auth.split(";",1));
}
	}
</script>
  </td></tr>
</table></div>
   <div class="i_map" id="i_map"><a href="#" id=id_NetworkMAP>Network MAP</a></div>
</div>
</div>
</div>
 <div class="menu_line" id="menuline" ></div>
 <div class="menu_line2" id="menuline"></div><div class="menu_line3"></div><div class="menu_line4"></div> 
 <div class="menu_line5"></div>
 <div class="menu_line6"></div>
<!--Howard  06/19  id="menu"-->  
  <div class="menu" id="menu">
  <ul>  
<script>                    
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6)) {
	document.write("<li class='game'><a href='game.html' target='contenthtml' id=id_GameEngine></a></li>");
	document.write("<li class='parental'><a href='parental_control.asp' target='contenthtml' id=id_ParenetalControl></a></li>");
	document.write("<li class='bandwidth'><a href='bandwidth.asp' target='contenthtml' id=id_BandwidthMGMT></a></li>");
	document.write("<li class='firewall'><a href='firewall.html' target='contenthtml' id=id_Firewall></a></li>");
}
else {	 
	document.write("<li class='gamex'><a id=id_GameEngine></a></li>");
	document.write("<li class='parentalx'><a id=id_ParenetalControl></a></li>");
	document.write("<li class='bandwidthx'><a id=id_BandwidthMGMT></a></li>");
	document.write("<li class='firewallx'><a id=id_Firewall></a></li>");
}
if (nv_OP_Mode == 6) {
	document.write("<li class='wirelessx'><a id=id_WirelessSecurity></a></li>");
	document.write("<li class='powersafex'><a id=id_PowerSaving></a></li>");
}	
else {
	document.write("<li class='wireless'><a href='wireless_security.asp' target='contenthtml' id=id_WirelessSecurity></a></li>");
	document.write("<li class='powersafe'><a href='power_saving.asp' target='contenthtml' id=id_PowerSaving></a></li>");
}	
</script>                    
</ul>
  </div>
<script>                    
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6))
	document.write("<div class='on_off'>");
else  
	document.write("<div class='on_offx'>");
</script>                    
<script>                    

if ("<% QoSGetInfo("ez_game_eng"); %>"=="checked")
	document.write("<ul><li class='on_on' ><a href='#'></a></li><li class='off'><a href='#'></a></li></ul>");
else
	document.write("<ul><li class='on'><a href='#'></a></li><li class='off_on'><a href='#'></a></li></ul>");
</script>
	</div>
<script>                    
if (nv_OP_Mode == 6)
	document.write("<div class='on_offx2'>");
else  
	document.write("<div class='on_off2'>");
</script>                    
<script>                    
if (<% getCfgZero(1, "WiFiScheduleEnable"); %>)
	document.write("<ul><li class='on_on' ><a href='#'></a></li><li class='off'><a href='#'></a></li></ul>");
else
	document.write("<ul><li class='on'><a href='#'></a></li><li class='off_on'><a href='#'></a></li></ul>");
</script>
	</div>
<script>                    
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6))
	document.write("<div class='on_off3'>");
else  
	document.write("<div class='on_offx3'>");
</script>                    
<script>                    
if (<% getCfgZero(1, "websHostFiltersEnabled"); %>)
	document.write("<ul><li class='on_on' ><a href='#'></a></li><li class='off'><a href='#'></a></li></ul>");
else
	document.write("<ul><li class='on'><a href='#'></a></li><li class='off_on'><a href='#'></a></li></ul>");
</script>
	</div>
<script>                    
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6))
	document.write("<div class='on_off4'>");
else  
	document.write("<div class='on_offx4'>");
</script>                    
<script>                    
if ("<% QoSGetInfo("qos_Enable"); %>"=="checked")
	document.write("<ul><li class='on_on' ><a href='#'></a></li><li class='off'><a href='#'></a></li></ul>");
else
	document.write("<ul><li class='on'><a href='#'></a></li><li class='off_on'><a href='#'></a></li></ul>");
</script>
	</div>
<script>                    
if ((nv_OP_Mode == 0)||(nv_OP_Mode == 6))
	document.write("<div class='on_off5'>");
else  
	document.write("<div class='on_offx5'>");
</script>                    
<script>                    
if (<% getCfgZero(1, "FIREWALLEnabled"); %>)
	document.write("<ul><li class='on_on' ><a href='#'></a></li><li class='off'><a href='#'></a></li></ul>");
else
	document.write("<ul><li class='on'><a href='#'></a></li><li class='off_on'><a href='#'></a></li></ul>");
</script>
	</div>
<!--	
  <div class="on_off6">
<script>                    
if (<% getCfgZero(1, "easy_wireless_security"); %>)
	document.write("<ul><li class='on_on' ><a href='#'></a></li><li class='off'><a href='#'></a></li></ul>");
else
	document.write("<ul><li class='on'><a href='#'></a></li><li class='off_on'><a href='#'></a></li></ul>");
</script>
	</div>
-->	
</div>
<!--Howard  06/19  new--> 
<div class="mask" id="b-mask"><!--Howard  06/19  class="bandwidthframe" id="bandwidthframe"--></div>
<div id="load" class="load" align="center">
	<img src="/loading.gif" />loading...</div>  
<div class="layer1" id="layer1">
  <div class="bandwidthframe" id="b-bandwidthframe">
    <ul>
      <li class="popup_top"></li>
      <li class="popup_main"></li>
      <li class="popup_down"></li>
    </ul>
  </div>
  <div class="unit_bandwidth_icon" id="b-icon"></div>
  <div class="unit_bandwidth_title" id="b-title">Bandwidth MGMT</div>
  <div class="close" id="b-close"><a href="empty.html" target="contenthtml"></a></div>
  <div class="bandwidth_content" id="bandwidht_conternt">
    <iframe  id="contenthtml" name = "contenthtml"  onload=loadFile()
 style="border:0px solid orange; " frameborder="0" width="600" height="390" scrolling="no"></iframe>
  </div>
</div>
<div id="popup3_table">
<table width="60%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" height="37"><img src="/local/advance/images/popup_top_left.gif" width="19" height="45" /></td>
<td background="/local/advance/images/popup_top_center.gif" class="table_title" id=log_out>Log Out</td>
<td width="15" background="/local/advance/images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')"></a></td>
<td width="19" height="45"><img src="/local/advance/images/popup_top_right.gif" width="19" height="45" /></td>
</tr>
</table></td>
</tr>
<tr>
<td><table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" background="/local/advance/images/popup_main_left.gif">&nbsp;</td>
<td bgcolor="#FFFFFF"><span class="popupcontent"> 
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="27%"><center>
<img src="/local/advance/images/i_info.jpg" width="50" height="49" />
</center>
</td>
<td width="73%" id=ask_logout>Are you sure you want to log out?</td>
</tr>
</table>       
</span></td>
<td width="19" background="/local/advance/images/popup_main_right.gif">&nbsp;</td>
</tr>
</table></td>
</tr>
<tr>
<td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" height="48"><img src="/local/advance/images/popup_down_left.gif" width="19" height="48" /></td>
<td background="/local/advance/images/popup_down_center.gif">
<div class="button2"><ul>   
<li><a href="#" title="No"><span onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')" id=g2-1No>No</span></a></li>
<li><a href="/logout.html" title="Yes"><span id=g2-1Yes>Yes</span></a></li></ul></div></td>
<td width="19" height="37"><img src="/local/advance/images/popup_down_right.gif" width="19" height="48" /></td>
</tr>
</table></td>
</tr>
</table></div>
<div id="popup_scan_network">
<table width="60%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" height="37"><img src="/local/advance/images/popup_top_left.gif" width="19" height="45" /></td>
<td background="/local/advance/images/popup_top_center.gif" class="table_title">NBG4615</td>
<td width="15" background="/local/advance/images/popup_top_center.gif"></td>
<td width="19" height="45"><img src="/local/advance/images/popup_top_right.gif" width="19" height="45" /></td>
</tr>
</table></td>
</tr>
<tr>
<td><table width="100%" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" background="/local/advance/images/popup_main_left.gif">&nbsp;</td>
<td bgcolor="#FFFFFF"><span class="popupcontent"> 
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td width="27%"><center>
<img src="/loading.gif" />
</center>
</td>
<td width="73%" id=id_ScanningyourNetwork>Scanning your Network . . .</td>
</tr>
</table>
</span></td>
<td width="19" background="/local/advance/images/popup_main_right.gif">&nbsp;</td>
</tr>
</table></td>
</tr>
<tr>
<td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
<tr>
<td width="19" height="48"><img src="/local/advance/images/popup_down_left.gif" width="19" height="48" /></td>
<td background="/local/advance/images/popup_down_center.gif">
<div class="button2"><ul>   
<li></li>
<li></li>
</ul></div></td>
<td width="19" height="37"><img src="/local/advance/images/popup_down_right.gif" width="19" height="48" /></td>
</tr>
</table></td>
</tr>
</table></div>

<iframe name='easy_info' src='/local/easy_start_lltd.asp' width='0' height='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>             
</iframe>
<form method="post" name="NTP" action="/goform/NTP">
<input type="hidden" name="mtenCurrent_Hour" value="<% getCurrentHour();%>" />
<input type="hidden" name="mtenCurrent_Min" value="<% getCurrentMin();%>" />
<input type="hidden" name="mtenCurrent_Sec" value="<% getCurrentSec();%>" />
<input type="hidden" name="mtenCurrent_Year" value="<% getCurrentYear();%>" />
<input type="hidden" name="mtenCurrent_Mon" value="<% getCurrentMon();%>" />
<input type="hidden" name="mtenCurrent_Day" value="<% getCurrentDay();%>" />
</form>
</body>
<script>
var easy_job_setting=0;
var scanning_network=1;
var current_ethernet_start_index=0;
var current_wireless_start_index=0;
if (<% existLLTDinfo(); %>==0)
MM_showHideLayers('popup','','show','popup_scan_network','','show');
</script>
<script>
function refreshmap()
{
easy_info.location.href='/local/refreshmap.asp';
}

 start_Mon="<% getCfgGeneral(1, "startMon"); %>";
 start_Day="<% getCfgGeneral(1, "startDay"); %>";
 start_Hour="<% getCfgGeneral(1, "startclock"); %>"; 
 end_Mon="<% getCfgGeneral(1, "endMon"); %>";
 end_Day="<% getCfgGeneral(1, "endDay"); %>";
 end_Hour="<% getCfgGeneral(1, "endclock"); %>";
 enable_dst = "<% getCfgGeneral(1, "enabledaylight"); %>";

	if(enable_dst=="1")
	{
	daylightsaving();
	}
initDateAndTime();
IncreaseSec();
</script>
</html>
