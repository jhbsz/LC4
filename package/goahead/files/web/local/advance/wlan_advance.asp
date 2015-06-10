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
td {white-space: nowrap;}
</style>    
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;

var PhyMode  = '<% getCfgZero(1, "WirelessMode"); %>';
var rtsThreshold = <% getCfgZero(1, "RTSThreshold"); %>;
var noforward = <% getCfgZero(1, "NoForwardingAdv"); %>;
var fragmentThreshold = '<% getCfgZero(1, "FragThreshold"); %>';        
var txPower = '<% getCfgZero(1, "TxPower"); %>';
var ht_mode = '<% getCfgZero(1, "HT_OpMode"); %>';
var ht_bw = '<% getCfgZero(1, "HT_BW"); %>';
var ht_gi = '<% getCfgZero(1, "HT_GI"); %>';
var ht_mcs = '<% getCfgZero(1, "HT_MCS"); %>';
var ht_rdg = '<% getCfgZero(1, "HT_RDG"); %>';
var ht_amsdu = '<% getCfgZero(1, "HT_AMSDU"); %>';
var ht_autoba = '<% getCfgZero(1, "HT_AutoBA"); %>';
var ht_badecline = '<% getCfgZero(1, "HT_BADecline"); %>';	
var tx_stream_idx = '<% getCfgZero(1, "HT_TxStream"); %>';
var rx_stream_idx = '<% getCfgZero(1, "HT_RxStream"); %>';	

function submit_apply()
{
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	    return false;
	}
	return true;
}

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

function CheckValue()
{
	if (submit_apply() == false)
	    return false;
	
	// Check RTS
	if (document.wireless_advanced.rts.value == "" )
	{
		alert('Please specify RTS Threshold');
		document.wireless_advanced.rts.focus();
		document.wireless_advanced.rts.select();
		return false;
	}

	if (isNaN(document.wireless_advanced.rts.value) || document.wireless_advanced.rts.value < 256 || document.wireless_advanced.rts.value > 2346)
	{
		alert('Invalid RTS Threshold');
		document.wireless_advanced.rts.focus();
		document.wireless_advanced.rts.select();
		return false;
	}
	
	// Check Fragment
	if (document.wireless_advanced.fragment.value == "" )
	{
		alert('Please specify Fragmentation Length');
		document.wireless_advanced.fragment.focus();
		document.wireless_advanced.fragment.select();
		return false;
	}

	if (isNaN(document.wireless_advanced.fragment.value) || document.wireless_advanced.fragment.value < 256 || document.wireless_advanced.fragment.value > 2346)
	{
		alert('Invalid Fragmentation Length');
		document.wireless_advanced.fragment.focus();
		document.wireless_advanced.fragment.select();
		return false;
	}
	
	showWebMessage(2, "");
	return true;
}

function Channel_BandWidth_onClick()
{

	if (document.wireless_advanced.n_bandwidth[0].checked == true)
	{
		show_div(false, "div_extension_channel");	
		document.wireless_advanced.n_extcha.disabled = true;
		
	}
	else
	{
		show_div(true, "div_extension_channel");	
		document.wireless_advanced.n_extcha.disabled = false;
	}

	parent.adjustMyFrameHeight();
}

function wirelessModeChange()
{
	show_div(false, "div_ht_phy_1_1");
	show_div(false, "div_ht_phy_1_2");
	show_div(false, "div_ht_phy_1_3");
	show_div(false, "div_ht_phy_1_4");
	show_div(false, "div_ht_phy_1_4");
	show_div(false, "div_ht_phy_1_6");
						
	show_div(false, "div_extension_channel");
		
	show_div(false, "div_ht_phy_2_1");
	show_div(false, "div_ht_phy_2_2");
	show_div(false, "div_ht_phy_2_3");
			
	show_div(false, "div_other_1");
	show_div(false, "div_other_2");
	show_div(false, "div_other_3");	
	show_div(false, "div_other_4");		

	if (document.wireless_advanced.wirelessmode.selectedIndex == 3) {
		show_div(true, "div_ht_phy_1_1");
		show_div(true, "div_ht_phy_1_2");
		show_div(true, "div_ht_phy_1_3");
		show_div(true, "div_ht_phy_1_4");
		show_div(true, "div_ht_phy_1_4");
		show_div(false, "div_ht_phy_1_6");
		
		Channel_BandWidth_onClick();
		
		show_div(false, "div_ht_phy_2_1");
		show_div(false, "div_ht_phy_2_2");
		show_div(false, "div_ht_phy_2_3");	

	}else{
		show_div(false, "div_ht_phy_1_1");
		show_div(false, "div_ht_phy_1_2");
		show_div(false, "div_ht_phy_1_3");
		show_div(false, "div_ht_phy_1_4");
		show_div(false, "div_ht_phy_1_4");
		show_div(false, "div_ht_phy_1_6");
						
		show_div(false, "div_extension_channel");
		
		show_div(false, "div_ht_phy_2_1");
		show_div(false, "div_ht_phy_2_2");
		show_div(false, "div_ht_phy_2_3");
	}
	
	if (wirelessmode == 3){ // WDS Mode
		show_div(false, "div_all_advance_id");
		show_div(false, "div_ht_phy_1_1");
		show_div(false, "div_ht_phy_1_2");
		show_div(false, "div_ht_phy_1_3");
		show_div(false, "div_ht_phy_1_4");
		show_div(false, "div_ht_phy_1_5");
		show_div(false, "div_ht_phy_1_6");
		show_div(false, "div_extension_channel");
		show_div(false, "div_ht_phy_2_1");
		show_div(false, "div_ht_phy_2_2");
		show_div(false, "div_ht_phy_2_3");
		show_div(false, "div_other_1");
		show_div(false, "div_other_2");
		show_div(false, "div_other_3");	
	}else{
	    show_div(true, "div_all_advance_id");
	}

	parent.adjustMyFrameHeight();
}

function initTranslation()
{
	
	var e = document.getElementById("AdvanceTitle");
	e.innerHTML = _("advance title");

	e = document.getElementById("AdvanceRTSCTS");
	e.innerHTML = _("advance rts cts");	

	e = document.getElementById("AdvanceFragmentation");
	e.innerHTML = _("advance frag");

	e = document.getElementById("AdvanceIntraBSS");
	e.innerHTML = _("advance intra bss");

	e = document.getElementById("AdvanceTXPower");
	e.innerHTML = _("advance tx power");

	e = document.getElementById("AdvanceNetMode");
	e.innerHTML = _("advance net mode");

	e = document.getElementById("AdvanceHTPhyMode");
	e.innerHTML = _("advance ht phymode");

	e = document.getElementById("AdvanceHTOPMode");
	e.innerHTML = _("advance ht opmode");

	e = document.getElementById("AdvanceHTMixed");
	e.innerHTML = _("advance ht mixed");

	e = document.getElementById("AdvanceHTGreen");
	e.innerHTML = _("advance ht green");

	e = document.getElementById("AdvanceHTChannelBW");
	e.innerHTML = _("advance ht channelbw");

	e = document.getElementById("AdvanceHTGI");
	e.innerHTML = _("advance ht gi");

	e = document.getElementById("AdvanceHTLongGI");
	e.innerHTML = _("advance ht longgi");

	e = document.getElementById("AdvanceHTAutoGI");
	e.innerHTML = _("advance ht autogi");

	e = document.getElementById("AdvanceMCS");
	e.innerHTML = _("advance mcs");

	e = document.getElementById("AdvanceHTAutoMCS");
	e.innerHTML = _("advance ht automcs");

	e = document.getElementById("AdvanceHTRDG");
	e.innerHTML = _("advance ht rdg");

	e = document.getElementById("AdvanceHTRDGDisable");
	e.innerHTML = _("advance rdg disable");

	e = document.getElementById("AdvanceHTRDGEnable");
	e.innerHTML = _("advance rdg enable");

	
	e = document.getElementById("AdvanceHTExtChannel");
	e.innerHTML = _("advance ht extchannel");
	
	
	e = document.getElementById("AdvanceHTAMSDU");
	e.innerHTML = _("advance ht amsdu");
	
	
	e = document.getElementById("AdvanceHTAMSDUDisable");
	e.innerHTML = _("advance ht amsdudisable");
										
	e = document.getElementById("AdvanceHTAMSDUEnable");
	e.innerHTML = _("advance ht amsduenable");
	
	e = document.getElementById("AdvanceHTAddBA");
	e.innerHTML = _("advance ht addba");
	
	e = document.getElementById("AdvanceHTAddBADisable");
	e.innerHTML = _("advance ht addbadisable");
		
	e = document.getElementById("AdvanceHTAddBAEnable");
	e.innerHTML = _("advance ht amsduenable");
	
	e = document.getElementById("AdvanceHTDelBA");
	e.innerHTML = _("advance ht delba");
	
	e = document.getElementById("AdvanceHTDelBADisable");
	e.innerHTML = _("advance ht delbadisable");
	
	
	e = document.getElementById("AdvanceHTDelBAEnable");
	e.innerHTML = _("advance ht delbaenable");	
	
	e = document.getElementById("AdvanceOther");
	e.innerHTML = _("advance other");
	
	e = document.getElementById("AdvanceHTTxStream");
	e.innerHTML = _("advance ht txstream");
									
	e = document.getElementById("AdvanceHTRxStream");
	e.innerHTML = _("advance ht rxstream");						

	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}
	

function initValue()
		{
	initTranslation();
        
	document.wireless_advanced.rts.value = rtsThreshold;

	document.wireless_advanced.fragment.value = fragmentThreshold;

	if (txPower == 10){
		document.wireless_advanced.tx_power.options.selectedIndex = 0;
	}else if (txPower == 25){
		document.wireless_advanced.tx_power.options.selectedIndex = 1;	
	}else if (txPower == 50){
		document.wireless_advanced.tx_power.options.selectedIndex = 2;	
	}else if (txPower == 75){
		document.wireless_advanced.tx_power.options.selectedIndex = 3;	
	}else if (txPower == 90){
		document.wireless_advanced.tx_power.options.selectedIndex = 4;	
	}else if (txPower == 100){
		document.wireless_advanced.tx_power.options.selectedIndex = 5;	
	}			

	
	if (noforward == 1)
		document.wireless_advanced.IntraBSS.checked = false;
	else
		document.wireless_advanced.IntraBSS.checked = true;

	PhyMode = 1*PhyMode;

	if (PhyMode == 0)
		document.wireless_advanced.wirelessmode.options.selectedIndex = 0;
	else if (PhyMode == 1)
		document.wireless_advanced.wirelessmode.options.selectedIndex = 1;
	else if (PhyMode == 4)
		document.wireless_advanced.wirelessmode.options.selectedIndex = 2;
	else if (PhyMode == 9)
		document.wireless_advanced.wirelessmode.options.selectedIndex = 3;
	
	//HT  mode
	if (1*ht_mode == 0)
	{
		document.wireless_advanced.n_mode[0].checked = true;
	}
	else if (1*ht_mode == 1)
	{
		document.wireless_advanced.n_mode[1].checked = true;
	}

	else if (1*ht_mode == 2)
	{
		document.wireless_advanced.n_mode[2].checked = true;
	}
	
	//HT GI
	if (1*ht_gi == 0)
	{
		document.wireless_advanced.n_gi[0].checked = true;
	}
	else if (1*ht_gi == 1)
	{
		document.wireless_advanced.n_gi[1].checked = true;
	}
	else if (1*ht_gi == 2)
	{
		document.wireless_advanced.n_gi[2].checked = true;
	}

	// HT MCS
	if (1*ht_mcs <= 15)
		document.wireless_advanced.n_mcs.options.selectedIndex = ht_mcs;
	else if (1*ht_mcs == 32)
		document.wireless_advanced.n_mcs.options.selectedIndex = 16;
	else if (1*ht_mcs == 33)
		document.wireless_advanced.n_mcs.options.selectedIndex = 17;
	
	//HT RDG
	if (1*ht_rdg == 0)
		document.wireless_advanced.n_rdg[0].checked = true;
	else
		document.wireless_advanced.n_rdg[1].checked = true;	
	
	// HT BW
	if (1*ht_bw == 0)
	{
		document.wireless_advanced.n_bandwidth[0].checked = true;
		show_div(false, "div_extension_channel");		
		//document.getElementById("extension_channel").style.visibility = "hidden";
		//document.getElementById("extension_channel").style.display = "none";
		document.wireless_advanced.n_extcha.disabled = true;
	}
	else
	{
		document.wireless_advanced.n_bandwidth[1].checked = true;
		show_div(true, "div_extension_channel");		
		//document.getElementById("extension_channel").style.visibility = "visible";
		//document.getElementById("extension_channel").style.display = style_display_on();
		document.wireless_advanced.n_extcha.disabled = false;
	}
		
	// HT AMSDU
	if (1*ht_amsdu == 0)
		document.wireless_advanced.n_amsdu[0].checked = true;
	else
		document.wireless_advanced.n_amsdu[1].checked = true;

	// HT AUTOBA
	if (1*ht_autoba == 0)
		document.wireless_advanced.n_autoba[0].checked = true;
	else
		document.wireless_advanced.n_autoba[1].checked = true;

	//HT BADECLINE
	if (1*ht_badecline == 0)
		document.wireless_advanced.n_badecline[0].checked = true;
	else
		document.wireless_advanced.n_badecline[1].checked = true;	
	

	document.wireless_advanced.rx_stream.options.selectedIndex = rx_stream_idx - 1;
	document.wireless_advanced.tx_stream.options.selectedIndex = tx_stream_idx - 1;

	wirelessModeChange();
	
	if (wirelessmode == 3){ // WDS Mode
	    show_div(false, "div_all_advance_id");
	    show_div(false, "div_ht_phy_1_1");
	    show_div(false, "div_ht_phy_1_2");
	    show_div(false, "div_ht_phy_1_3");
	    show_div(false, "div_ht_phy_1_4");
	    show_div(false, "div_ht_phy_1_5");
	    show_div(false, "div_ht_phy_1_6");
	    show_div(false, "div_extension_channel");
	    show_div(false, "div_ht_phy_2_1");
	    show_div(false, "div_ht_phy_2_2");
	    show_div(false, "div_ht_phy_2_3");
	    show_div(false, "div_other_1");
	    show_div(false, "div_other_2");
	    show_div(false, "div_other_3");
	}else{
	    show_div(true, "div_all_advance_id");
	}

	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	}
}

</script>
</head>
<body onLoad="initValue()">
<form method=post name="wireless_advanced" action="/goform/wifiAdvanced" onSubmit="return CheckValue()">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
              
<li class="title" id="AdvanceTitle">Wireless Advanced Setup </li>
<span id="div_all_advance_id" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>                
<td width="40%" id="AdvanceRTSCTS">RTS/CTS  Threshold   </td>
<td><input type=text maxlength="15" size="15" value="" name="rts" />
(256 ~ 2346) </td>
</tr>
</table>
</li>

<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>                
<td width="40%" id="AdvanceFragmentation">Fragmentation  Threshold</td>
<td><input type=text maxlength="15" size="15" value="" name="fragment" /> 
(256 ~ 2346) </td>
</tr>  
</table>
</li>  
</span>

<span class="off">                                  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><input name="IntraBSS" type="checkbox" value=0 />
<font id ="AdvanceIntraBSS"> Enable Intra-BSS Traffic</font> 
</td>
</tr>
</table>
</li>
</span>
                
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>                
<td width="40%" id ="AdvanceTXPower">Output Power</td>
<td>
<select name="tx_power" size="1">
<option value = 10>10%</option>
<option value = 25>25%</option>
<option value = 50>50%</option>
<option value = 75>75%</option>
<option value = 90>90%</option>
<option value = 100>100%</option>
</select>
</td>	
</tr>  
</table>
</li>  

<span class='off'>
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr> 
<td width="40%" id="AdvanceNetMode">Network Mode</td>
<td>
<select name="wirelessmode" id="wirelessmode" size="1" onChange="wirelessModeChange()" >
<option value=0>11b/g mixed mode</option>
<option value=1>11b only</option>
<option value=4>11g only</option>
<option value=9>11b/g/n mixed mode</option>       
</select>
</td>
</tr>
</table>
</li>
</span>

<span id="div_ht_phy_1_1" class="off">
<li class="space3"></li>
<li class="title" id="AdvanceHTPhyMode">HT Physical Mode</li> 
</span>  
<span id="div_ht_phy_1_2" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">  
<tr>
<td width="40%" class="head" id="AdvanceHTOPMode">Operating Mode</td>
<td>
<input type=radio id=n_mode_0 name=n_mode value="0" checked><font id="AdvanceHTMixed">Mixed Mode</font>
<input type=radio id=n_mode_1 name=n_mode value="1"><font id="AdvanceHTGreen">Green Field</font>
</td>
</tr>
</table>
</li>         
</span>  


<span id="div_ht_phy_1_3" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceHTChannelBW">Channel BandWidth</td>
<td>
<input type=radio id=n_bandwidth_0 name=n_bandwidth value="0" onClick="Channel_BandWidth_onClick()" checked>20&nbsp;
<input type=radio id=n_bandwidth_1 name=n_bandwidth value="1" onClick="Channel_BandWidth_onClick()">20/40
</td>
</tr>
</table>
</li>         
</span>  


<span id="div_ht_phy_1_4" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceHTGI">Guard Interval</td>
<td>
<input type=radio id=n_gi_0 name=n_gi value="0" checked><font id="AdvanceHTLongGI">long</font>
<input type=radio id=n_gi_1 name=n_gi value="1"><font id="AdvanceHTAutoGI"> Auto</font>
</td>
</tr>
</table>
</li>        
</span>  


<span id="div_ht_phy_1_5" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceMCS">MCS</td>
<td>
<select name="n_mcs" size="1">
<option value = 0>0</option>
<option value = 1>1</option>
<option value = 2>2</option>
<option value = 3>3</option>
<option value = 4>4</option>
<option value = 5>5</option>
<option value = 6>6</option>
<option value = 7>7</option>
<option value = 8>8</option>
<option value = 9>9</option>
<option value = 10>10</option>
<option value = 11>11</option>
<option value = 12>12</option>
<option value = 13>13</option>
<option value = 14>14</option>
<option value = 15>15</option>
<option value = 32>32</option>
<option value = 33 selected id="AdvanceHTAutoMCS">Auto</option>
</select>
</td>
</tr>
</table>
</li>         
</span>  


<span id="div_ht_phy_1_6" class="off">  
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceHTRDG">Reverse Direction Grant(RDG)</td>
<td>
<input type=radio id=n_rdg_0 name=n_rdg value="0" checked><font id="AdvanceHTRDGDisable">Disable</font>
<input type=radio id=n_rdg_1 name=n_rdg value="1"><font id="AdvanceHTRDGEnable">Enable</font>
</td>
</tr>
</table>
</li>         
</span>  

<span id="div_extension_channel" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr name="extension_channel" id="extension_channel">
<td width="40%" class="head" id="AdvanceHTExtChannel">Extension Channel</td>
<td>
<select id="n_extcha" name="n_extcha" size="1">
<% getWlan11gExtChannelsFreq(); %>
</select>
</td>
</tr>
</table>
</li> 
</span>
  
<span id="div_ht_phy_2_1" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceHTAMSDU">Aggregation MSDU(A-MSDU)</td>
<td>
<input type=radio id=n_amsdu_0 name=n_amsdu value="0" checked><font id="AdvanceHTAMSDUDisable">Disable</font>
<input type=radio id=n_amsdu_1 name=n_amsdu value="1"><font id="AdvanceHTAMSDUEnable">Enable</font>
</td>
</tr>
</table>
</li>         
</span>


<span id="div_ht_phy_2_2" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceHTAddBA">Auto Block ACK</td>
<td>
<input type=radio id=n_autoba_0 name=n_autoba value="0" checked><font id="AdvanceHTAddBADisable">Disable</font>
<input type=radio id=n_autoba_1 name=n_autoba value="1"><font id="AdvanceHTAddBAEnable">Enable</font>
</td>
</tr>
</table>
</li>         
</span>


<span id="div_ht_phy_2_3" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceHTDelBA">Decline BA Request</td>
<td>
<input type=radio id=n_badecline_0 name=n_badecline value="0" checked><font id="AdvanceHTDelBADisable">Disable</font>
<input type=radio id=n_badecline_1 name=n_badecline value="1"><font id="AdvanceHTDelBAEnable">Enable</font>
</td>
</tr>
</table>
</li>       
</span>  

  
  
<span id="div_other_1" class="off">
<li class="title" id="AdvanceOther">Other</li> 
</span>

<span id="div_other_2" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">    
<tr>
<td width="40%" class="head" id="AdvanceHTTxStream">HT TxStream</td>
<td>
<select name="tx_stream" size="1">
<option value = 1>1</option>
<option value = 2>2</option>
</select>
</td>
</tr>
</table>
</li>       
</span>  

<span id="div_other_3" class="off">
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">      
<tr>
<td width="40%" class="head" id="AdvanceHTRxStream">HT RxStream</td>
<td>
<select name="rx_stream" size="1">
<option value = 1>1</option>
<option value = 2>2</option>
</select>
</td>
</tr>
</table>
</li>
</span>  

<span id="div_other_4" class="off">
<li class="w_text">
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
<input type=reset  value="Reset" id="GeneralReset" onClick="window.location.reload()">        
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