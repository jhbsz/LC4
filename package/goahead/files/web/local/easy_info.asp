<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head><link href="images/css.css" rel="stylesheet" type="text/css" />
<script type="text/JavaScript">
var statusRoutercheck=<% statusRoutercheck(); %>;
var statusInternetcheck=<% statusInternetcheck(); %>;
var button_status=new Array(6);
if ("<% QoSGetInfo("ez_game_eng"); %>"=="checked")
button_status[0]=1;
else
button_status[0]=0;
button_status[1]=<% getCfgZero(1, "WiFiScheduleEnable"); %>;
button_status[2]=<% getCfgZero(1, "websHostFiltersEnabled"); %>;
if ("<% QoSGetInfo("qos_Enable"); %>"=="checked")
button_status[3]=1;
else
button_status[3]=0;
button_status[4]=<% getCfgZero(1, "FIREWALLEnabled"); %>;
button_status[5]=<% getCfgZero(1, "easy_wireless_security"); %>;

<% javascriptLLTD(); %>

function show_map()
{
	var show_max=4;
	
	var ethernet_j=parent.current_ethernet_start_index;

	parent.document.getElementById('ethernet_item1').style.visibility='hidden';
	parent.document.getElementById('ethernet_item2').style.visibility='hidden';
	parent.document.getElementById('ethernet_item3').style.visibility='hidden';
	parent.document.getElementById('ethernet_item4').style.visibility='hidden';
	
	if (lan_num>0)
	{
	if (ethernet_j>0)
			parent.document.getElementById('ethernet_right_arrow').style.visibility='visible';
	else		
			parent.document.getElementById('ethernet_right_arrow').style.visibility='hidden';
	for (i=0; i<show_max; i++)
	{
		if (i+ethernet_j<lan_num)
		{
			if (lan_type[i+ethernet_j]==0)
				parent.document.getElementById('ethernet_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_nb.gif)';
			if (lan_type[i+ethernet_j]==1)
				parent.document.getElementById('ethernet_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_ap.gif)';
			if (lan_type[i+ethernet_j]==2)
				parent.document.getElementById('ethernet_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_game.gif)';
			if (lan_type[i+ethernet_j]==4)
				parent.document.getElementById('ethernet_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_ap.gif)';
			if (lan_type[i+ethernet_j]==9)
				parent.document.getElementById('ethernet_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_ap.gif)';
			if (lan_name[i+ethernet_j]!="")	
			{
				parent.document.getElementById('ethernet_name'+(i+1)).innerHTML=shortname(lan_name[i+ethernet_j]);
				parent.document.getElementById('ethernet_name'+(i+1)).title=lan_name[i+ethernet_j];
			}
			else
			{	
				parent.document.getElementById('ethernet_name'+(i+1)).innerHTML=shortname(lan_ip[i+ethernet_j]);
				parent.document.getElementById('ethernet_name'+(i+1)).title=lan_ip[i+ethernet_j];
			}
			parent.document.getElementById('ethernet_item'+(i+1)).style.visibility='visible';
		}
	}
	if (ethernet_j+show_max<lan_num)
			parent.document.getElementById('ethernet_left_arrow').style.visibility='visible';
	else		
			parent.document.getElementById('ethernet_left_arrow').style.visibility='hidden';
	}

	var wireless_j=parent.current_wireless_start_index;

	parent.document.getElementById('ethernet2_item1').style.visibility='hidden';
	parent.document.getElementById('ethernet2_item2').style.visibility='hidden';
	parent.document.getElementById('ethernet2_item3').style.visibility='hidden';
	parent.document.getElementById('ethernet2_item4').style.visibility='hidden';
	parent.document.getElementById('wifi1').style.visibility='hidden';
	parent.document.getElementById('wifi2').style.visibility='hidden';
	parent.document.getElementById('wifi3').style.visibility='hidden';
	parent.document.getElementById('wifi4').style.visibility='hidden';
	if (wlan_num>0)
	{
	if (wireless_j>0)
			parent.document.getElementById('wireless_right_arrow').style.visibility='visible';
	else		
			parent.document.getElementById('wireless_right_arrow').style.visibility='hidden';
	for (i=0; i<show_max; i++)
	{
		if (i+wireless_j<wlan_num)
		{
			if (wlan_type[i+wireless_j]==0)
				parent.document.getElementById('ethernet2_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_nb.gif)';
			if (wlan_type[i+wireless_j]==1)
				parent.document.getElementById('ethernet2_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_ap.gif)';
			if (wlan_type[i+wireless_j]==2)
				parent.document.getElementById('ethernet2_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_game.gif)';
			if (wlan_type[i+wireless_j]==4)
				parent.document.getElementById('ethernet2_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_ap.gif)';
			if (wlan_type[i+wireless_j]==9)
				parent.document.getElementById('ethernet2_img'+(i+1)).style.backgroundImage='url(/local/images/network/i_ap.gif)';
			if (wlan_name[i+wireless_j]!="")	
			{
				parent.document.getElementById('ethernet2_name'+(i+1)).innerHTML=shortname(wlan_name[i+wireless_j]);
				parent.document.getElementById('ethernet2_name'+(i+1)).title=wlan_name[i+wireless_j];
			}
			else
			{	
				parent.document.getElementById('ethernet2_name'+(i+1)).innerHTML=shortname(wlan_ip[i+wireless_j]);
				parent.document.getElementById('ethernet2_name'+(i+1)).title=wlan_ip[i+wireless_j];
			}
			parent.document.getElementById('ethernet2_item'+(i+1)).style.visibility='visible';
			parent.document.getElementById('wifi'+(i+1)).style.visibility='visible';
		}
	}
	if (wireless_j+show_max<wlan_num)
			parent.document.getElementById('wireless_left_arrow').style.visibility='visible';
	else		
			parent.document.getElementById('wireless_left_arrow').style.visibility='hidden';
	}
	
	parent.document.getElementById('vline_id').style.visibility='hidden';
	parent.document.getElementById('vline_error_id').style.visibility='hidden';
	parent.document.getElementById('error2').style.visibility='hidden';
	if (statusRoutercheck)
			parent.document.getElementById('vline_id').style.visibility='visible';
	else
	{	
			parent.document.getElementById('vline_error_id').style.visibility='visible';
			parent.document.getElementById('error2').style.visibility='visible';
	}		

	parent.document.getElementById('hline4_id').style.visibility='hidden';
	parent.document.getElementById('hline4_error_id').style.visibility='hidden';
	parent.document.getElementById('error1').style.visibility='hidden';
	if (statusInternetcheck)
	{
			parent.document.getElementById('hline4_id').style.visibility='visible';
			parent.document.getElementById('earth_id').style.backgroundImage='url(/local/images/network/i_earth_on.gif)';
	}		
	else
	{	
			parent.document.getElementById('hline4_error_id').style.visibility='visible';
			parent.document.getElementById('error1').style.visibility='visible';
			parent.document.getElementById('earth_id').style.backgroundImage='url(/local/images/network/i_earth.gif)';
	}		
}

function shortname(str)
{
	var short_str=str;
	if (str.length>6)
		short_str=str.substr(0,5)+"...";
	return short_str;
}
</script>
<body>
<form method="post" name="easy_set" action="/goform/easy_setting">
<input type="hidden" name="easy_set_button_job" value="0" />
<input type="hidden" name="easy_set_button_mode" value="0" />
</form>
</body>
<script type="text/JavaScript">
parent.easy_job_setting=0;
function easy_set_option(job, mode)
{
//job - 1:Game Engine 2:Power Saving 3:Parental Control 4:Bandwidth MGMT 5:Firewall 6:Wireless Security
//mode - 0:OFF 1:ON
	if (button_status[job-1]!=mode)
	{
		parent.easy_job_setting=1;
		document.easy_set.easy_set_button_job.value=job;
		document.easy_set.easy_set_button_mode.value=mode;
		document.easy_set.submit();
	}
}

show_map();

if (parent.scanning_network)
{
	parent.scanning_network=0;
	parent.MM_showHideLayers('popup','','hide','popup_scan_network','','hide');
}

function item_connect(interface, index)
{
	var i=0;
	if (interface==1)//ethernet
	{
		i=parent.current_ethernet_start_index+index-1;
		if (lan_type[i]==0)//PC
			window.open("\\\\"+lan_ip[i]);
		if (lan_type[i]==1)//Device
			window.open("http://"+lan_ip[i]);
		if (lan_type[i]==4)//AP
			window.open("http://"+lan_ip[i]);
		if (lan_type[i]==9)//Repeater
			window.open("http://"+lan_ip[i]);
	}
	if (interface==2)//wireless
	{
		i=parent.current_wireless_start_index+index-1;
		if (wlan_type[i]==0)//PC
			window.open("\\\\"+wlan_ip[i]);
		if (wlan_type[i]==1)//Device
			window.open("http://"+wlan_ip[i]);
		if (wlan_type[i]==4)//AP
			window.open("http://"+wlan_ip[i]);
		if (wlan_type[i]==9)//Repeater
			window.open("http://"+wlan_ip[i]);
	}
}

function item_show_info(interface, index)
{
	var i=0;
	var type_str="";
	if (interface==1)//ethernet
	{
		i=parent.current_ethernet_start_index+index-1;
		if (lan_type[i]==0)
			type_str="PC";
		if (lan_type[i]==1)
			type_str="Device";
		if (lan_type[i]==2)
			type_str="XBox";
		if (lan_type[i]==4)
			type_str="AP";
		if (lan_type[i]==9)
			type_str="Repeater";
		alert("Name: "+lan_name[i]+"\nMAC Address: "+lan_mac[i]+"\nIP Address: "+lan_ip[i]+"\nType: "+type_str+"\n");
	}
	if (interface==2)//wireless
	{
		i=parent.current_wireless_start_index+index-1;
		if (wlan_type[i]==0)
			type_str="PC";
		if (wlan_type[i]==1)
			type_str="Device";
		if (wlan_type[i]==2)
			type_str="XBox";
		if (wlan_type[i]==4)
			type_str="AP";
		if (wlan_type[i]==9)
			type_str="Repeater";
		alert("Name: "+wlan_name[i]+"\nMAC Address: "+wlan_mac[i]+"\nIP Address: "+wlan_ip[i]+"\nType: "+type_str+"\n");
	}
}

</script>
</html>
