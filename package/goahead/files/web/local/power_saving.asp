<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>Untitled Document</title>
<script type="text/javascript" src="js/jquery-1.2.6.js"></script>
<script type="text/javascript" src="js/power-saving.js"></script>
<script type="text/javascript" src="/lang/b28n.js"></script>
<link href="images/popup.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
</style>

<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");
var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function CheckValue()
{
	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_0.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_0.options.selectedIndex) ||
		(document.CfgWiFiScheduler.WLAN_scheduling_end_hour_1.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_1.options.selectedIndex) ||
		(document.CfgWiFiScheduler.WLAN_scheduling_end_hour_2.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_2.options.selectedIndex) ||
		(document.CfgWiFiScheduler.WLAN_scheduling_end_hour_3.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_3.options.selectedIndex) ||
		(document.CfgWiFiScheduler.WLAN_scheduling_end_hour_4.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_4.options.selectedIndex) ||
		(document.CfgWiFiScheduler.WLAN_scheduling_end_hour_5.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_5.options.selectedIndex) ||
		(document.CfgWiFiScheduler.WLAN_scheduling_end_hour_6.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_6.options.selectedIndex) ||
		(document.CfgWiFiScheduler.WLAN_scheduling_end_hour_7.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_hour_7.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}

	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_0.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_0.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_0.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_0.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();		
			return false;
	}

	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_1.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_1.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_1.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_1.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}
	
	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_2.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_2.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_2.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_2.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}

	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_3.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_3.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_3.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_3.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}

	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_4.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_4.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_4.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_4.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}

	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_5.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_5.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_5.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_5.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}
	
	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_6.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_6.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_6.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_6.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}
	
	if ((document.CfgWiFiScheduler.WLAN_scheduling_end_hour_7.options.selectedIndex == document.CfgWiFiScheduler.WLAN_scheduling_start_hour_7.options.selectedIndex) && 
		(document.CfgWiFiScheduler.WLAN_scheduling_end_min_7.options.selectedIndex < document.CfgWiFiScheduler.WLAN_scheduling_start_min_7.options.selectedIndex)){
			alert("Error : Start Time greater than End Time");
			initValue();
			return false;
	}						

	return true;
}

function clickEveryday()
{
	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked )
	{
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_1.checked=false;
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_2.checked=false;
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_3.checked=false;
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_4.checked=false;
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_5.checked=false;
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_6.checked=false;
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_7.checked=false;

		return;          
	}    	    

}

function checkAndSetWeekdays()
{

	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_1.checked){
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;
		return;
	}
  	     	    
      	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_2.checked){
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;      	
		return;
    	}    
    	    
    	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_3.checked){
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;
		return;
    	}
    	     	    
      	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_4.checked){
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;
		return;
    	}    
    	
    	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_5.checked){
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;
		return;
    	}
    	     	    
      	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_6.checked){
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;
		return;
    	}        	 
 
      	if ( document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_7.checked){
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;
		return;
    	}
}

function initValue()
{
  
  
  initTranslation();
	var nv_WLAN_scheduling_action_0 = <% getCfgZero(1, "WiFischeduling_action_0"); %>;
	var nv_WLAN_Scheduling_day_Active_0 = <% getCfgZero(1, "WiFiScheduling_day_Active_0"); %>;
	var nv_WLAN_scheduling_start_hour_0 = <% getCfgZero(1, "WiFischeduling_start_hour_0"); %>;
	var nv_WLAN_scheduling_start_min_0 = <% getCfgZero(1, "WiFischeduling_start_min_0"); %>;
	var nv_WLAN_scheduling_end_hour_0 = <% getCfgZero(1, "WiFischeduling_end_hour_0"); %>;
	var nv_WLAN_scheduling_end_min_0 = <% getCfgZero(1, "WiFischeduling_end_min_0"); %>;

	var nv_WLAN_scheduling_action_1 = <% getCfgZero(1, "WiFischeduling_action_1"); %>;
	var nv_WLAN_Scheduling_day_Active_1 = <% getCfgZero(1, "WiFiScheduling_day_Active_1"); %>;
	var nv_WLAN_scheduling_start_hour_1 = <% getCfgZero(1, "WiFischeduling_start_hour_1"); %>;
	var nv_WLAN_scheduling_start_min_1 = <% getCfgZero(1, "WiFischeduling_start_min_1"); %>;
	var nv_WLAN_scheduling_end_hour_1 = <% getCfgZero(1, "WiFischeduling_end_hour_1"); %>;
	var nv_WLAN_scheduling_end_min_1 = <% getCfgZero(1, "WiFischeduling_end_min_1"); %>;

	var nv_WLAN_scheduling_action_2 = <% getCfgZero(1, "WiFischeduling_action_2"); %>;
	var nv_WLAN_Scheduling_day_Active_2 = <% getCfgZero(1, "WiFiScheduling_day_Active_2"); %>;
	var nv_WLAN_scheduling_start_hour_2 = <% getCfgZero(1, "WiFischeduling_start_hour_2"); %>;
	var nv_WLAN_scheduling_start_min_2 = <% getCfgZero(1, "WiFischeduling_start_min_2"); %>;
	var nv_WLAN_scheduling_end_hour_2 = <% getCfgZero(1, "WiFischeduling_end_hour_2"); %>;
	var nv_WLAN_scheduling_end_min_2 = <% getCfgZero(1, "WiFischeduling_end_min_2"); %>;
	
	var nv_WLAN_scheduling_action_3 = <% getCfgZero(1, "WiFischeduling_action_3"); %>;
	var nv_WLAN_Scheduling_day_Active_3 = <% getCfgZero(1, "WiFiScheduling_day_Active_3"); %>;
	var nv_WLAN_scheduling_start_hour_3 = <% getCfgZero(1, "WiFischeduling_start_hour_3"); %>;
	var nv_WLAN_scheduling_start_min_3 = <% getCfgZero(1, "WiFischeduling_start_min_3"); %>;
	var nv_WLAN_scheduling_end_hour_3 = <% getCfgZero(1, "WiFischeduling_end_hour_3"); %>;
	var nv_WLAN_scheduling_end_min_3 = <% getCfgZero(1, "WiFischeduling_end_min_3"); %>;
	
	var nv_WLAN_scheduling_action_4 = <% getCfgZero(1, "WiFischeduling_action_4"); %>;
	var nv_WLAN_Scheduling_day_Active_4 = <% getCfgZero(1, "WiFiScheduling_day_Active_4"); %>;
	var nv_WLAN_scheduling_start_hour_4 = <% getCfgZero(1, "WiFischeduling_start_hour_4"); %>;
	var nv_WLAN_scheduling_start_min_4 = <% getCfgZero(1, "WiFischeduling_start_min_4"); %>;
	var nv_WLAN_scheduling_end_hour_4 = <% getCfgZero(1, "WiFischeduling_end_hour_4"); %>;
	var nv_WLAN_scheduling_end_min_4 = <% getCfgZero(1, "WiFischeduling_end_min_4"); %>;
	
	var nv_WLAN_scheduling_action_5 = <% getCfgZero(1, "WiFischeduling_action_5"); %>;
	var nv_WLAN_Scheduling_day_Active_5 = <% getCfgZero(1, "WiFiScheduling_day_Active_5"); %>;
	var nv_WLAN_scheduling_start_hour_5 = <% getCfgZero(1, "WiFischeduling_start_hour_5"); %>;
	var nv_WLAN_scheduling_start_min_5 = <% getCfgZero(1, "WiFischeduling_start_min_5"); %>;
	var nv_WLAN_scheduling_end_hour_5 = <% getCfgZero(1, "WiFischeduling_end_hour_5"); %>;
	var nv_WLAN_scheduling_end_min_5 = <% getCfgZero(1, "WiFischeduling_end_min_5"); %>;

	var nv_WLAN_scheduling_action_6 = <% getCfgZero(1, "WiFischeduling_action_6"); %>;
	var nv_WLAN_Scheduling_day_Active_6 = <% getCfgZero(1, "WiFiScheduling_day_Active_6"); %>;
	var nv_WLAN_scheduling_start_hour_6 = <% getCfgZero(1, "WiFischeduling_start_hour_6"); %>;
	var nv_WLAN_scheduling_start_min_6 = <% getCfgZero(1, "WiFischeduling_start_min_6"); %>;
	var nv_WLAN_scheduling_end_hour_6 = <% getCfgZero(1, "WiFischeduling_end_hour_6"); %>;
	var nv_WLAN_scheduling_end_min_6 = <% getCfgZero(1, "WiFischeduling_end_min_6"); %>;
	
	var nv_WLAN_scheduling_action_7 = <% getCfgZero(1, "WiFischeduling_action_7"); %>;
	var nv_WLAN_Scheduling_day_Active_7 = <% getCfgZero(1, "WiFiScheduling_day_Active_7"); %>;
	var nv_WLAN_scheduling_start_hour_7 = <% getCfgZero(1, "WiFischeduling_start_hour_7"); %>;
	var nv_WLAN_scheduling_start_min_7 = <% getCfgZero(1, "WiFischeduling_start_min_7"); %>;
	var nv_WLAN_scheduling_end_hour_7 = <% getCfgZero(1, "WiFischeduling_end_hour_7"); %>;
	var nv_WLAN_scheduling_end_min_7 = <% getCfgZero(1, "WiFischeduling_end_min_7"); %>;	


	if (nv_WLAN_scheduling_action_0 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_0[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_0[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_0 == 0)
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = false;
	else
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_0.checked = true;
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_0.options.selectedIndex = nv_WLAN_scheduling_start_hour_0;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_0.options.selectedIndex = nv_WLAN_scheduling_start_min_0;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_0.options.selectedIndex = nv_WLAN_scheduling_end_hour_0;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_0.options.selectedIndex = nv_WLAN_scheduling_end_min_0;
	
	if (nv_WLAN_scheduling_action_1 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_1[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_1[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_1 == 0)		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_1.checked = false;
	else
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_1.checked = true;		
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_1.options.selectedIndex = nv_WLAN_scheduling_start_hour_1;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_1.options.selectedIndex = nv_WLAN_scheduling_start_min_1;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_1.options.selectedIndex = nv_WLAN_scheduling_end_hour_1;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_1.options.selectedIndex = nv_WLAN_scheduling_end_min_1;
	
	if (nv_WLAN_scheduling_action_2 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_2[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_2[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_2 == 0)		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_2.checked = false;
	else	
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_2.checked = true;		
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_2.options.selectedIndex = nv_WLAN_scheduling_start_hour_2;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_2.options.selectedIndex = nv_WLAN_scheduling_start_min_2;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_2.options.selectedIndex = nv_WLAN_scheduling_end_hour_2;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_2.options.selectedIndex = nv_WLAN_scheduling_end_min_2;

	if (nv_WLAN_scheduling_action_3 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_3[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_3[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_3 == 0)		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_3.checked = false;
	else	
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_3.checked = true;		
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_3.options.selectedIndex = nv_WLAN_scheduling_start_hour_3;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_3.options.selectedIndex = nv_WLAN_scheduling_start_min_3;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_3.options.selectedIndex = nv_WLAN_scheduling_end_hour_3;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_3.options.selectedIndex = nv_WLAN_scheduling_end_min_3;
				
	if (nv_WLAN_scheduling_action_4 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_4[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_4[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_4 == 0)		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_4.checked = false;
	else
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_4.checked = true;		
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_4.options.selectedIndex = nv_WLAN_scheduling_start_hour_4;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_4.options.selectedIndex = nv_WLAN_scheduling_start_min_4;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_4.options.selectedIndex = nv_WLAN_scheduling_end_hour_4;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_4.options.selectedIndex = nv_WLAN_scheduling_end_min_4;
			
	if (nv_WLAN_scheduling_action_5 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_5[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_5[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_5 == 0)		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_5.checked = false;
	else		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_5.checked = true;		
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_5.options.selectedIndex = nv_WLAN_scheduling_start_hour_5;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_5.options.selectedIndex = nv_WLAN_scheduling_start_min_5;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_5.options.selectedIndex = nv_WLAN_scheduling_end_hour_5;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_5.options.selectedIndex = nv_WLAN_scheduling_end_min_5;
	
	if (nv_WLAN_scheduling_action_6 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_6[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_6[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_6 == 0)		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_6.checked = false;
	else	
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_6.checked = true;		
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_6.options.selectedIndex = nv_WLAN_scheduling_start_hour_6;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_6.options.selectedIndex = nv_WLAN_scheduling_start_min_6;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_6.options.selectedIndex = nv_WLAN_scheduling_end_hour_6;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_6.options.selectedIndex = nv_WLAN_scheduling_end_min_6;

	if (nv_WLAN_scheduling_action_7 == 1)
		document.CfgWiFiScheduler.WLAN_scheduling_action_7[0].checked = true;
	else
		document.CfgWiFiScheduler.WLAN_scheduling_action_7[1].checked = true;
		
	if (nv_WLAN_Scheduling_day_Active_7 == 0)		
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_7.checked = false;
	else	
		document.CfgWiFiScheduler.WLAN_Scheduling_day_Active_7.checked = true;		
							
	document.CfgWiFiScheduler.WLAN_scheduling_start_hour_7.options.selectedIndex = nv_WLAN_scheduling_start_hour_7;
	document.CfgWiFiScheduler.WLAN_scheduling_start_min_7.options.selectedIndex = nv_WLAN_scheduling_start_min_7;
	document.CfgWiFiScheduler.WLAN_scheduling_end_hour_7.options.selectedIndex = nv_WLAN_scheduling_end_hour_7;
	document.CfgWiFiScheduler.WLAN_scheduling_end_min_7.options.selectedIndex = nv_WLAN_scheduling_end_min_7;
	
on_select_start_hour(0);
on_select_end_hour(0);
on_select_start_hour(1);
on_select_end_hour(1);
on_select_start_hour(2);
on_select_end_hour(2);
on_select_start_hour(3);
on_select_end_hour(3);
on_select_start_hour(4);
on_select_end_hour(4);
on_select_start_hour(5);
on_select_end_hour(5);
on_select_start_hour(6);
on_select_end_hour(6);
on_select_start_hour(7);
on_select_end_hour(7);
}
function initTranslation()
{

	var e = document.getElementById("SchedulingTitleNote");
	e.innerHTML = _("scheduling title note");

	e = document.getElementById("SchedulingWLANStatus");
	e.innerHTML = _("scheduling wlan status");
	
	e = document.getElementById("SchedulingWLANDay");
	e.innerHTML = _("scheduling wlan day");
	
	e = document.getElementById("SchedulingWLANExcept");
	e.innerHTML = _("scheduling wlan except");
	
	e = document.getElementById("Schedulingday0on");
	e.innerHTML = _("scheduling day0 on");
	e = document.getElementById("Schedulingday0off");
	e.innerHTML = _("scheduling day0 off");
	e = document.getElementById("Schedulingday0Everyday");
	e.innerHTML = _("scheduling day0 everyday");		
	e = document.getElementById("Schedulingday0StartHour");
	e.innerHTML = _("scheduling day0 starthour");									
	e = document.getElementById("Schedulingday0StartMin");
	e.innerHTML = _("scheduling day0 startmin");
	e = document.getElementById("Schedulingday0EndHour");
	e.innerHTML = _("scheduling day0 endhour");									
	e = document.getElementById("Schedulingday0EndMin");
	e.innerHTML = _("scheduling day0 endmin");
	
	e = document.getElementById("Schedulingday1on");
	e.innerHTML = _("scheduling day1 on");
	e = document.getElementById("Schedulingday1off");
	e.innerHTML = _("scheduling day1 off");
	e = document.getElementById("Schedulingday1Mon");
	e.innerHTML = _("scheduling day1 mon");		
	e = document.getElementById("Schedulingday1StartHour");
	e.innerHTML = _("scheduling day1 starthour");									
	e = document.getElementById("Schedulingday1StartMin");
	e.innerHTML = _("scheduling day1 startmin");
	e = document.getElementById("Schedulingday1EndHour");
	e.innerHTML = _("scheduling day1 endhour");									
	e = document.getElementById("Schedulingday1EndMin");
	e.innerHTML = _("scheduling day1 endmin");

	e = document.getElementById("Schedulingday2on");
	e.innerHTML = _("scheduling day2 on");
	e = document.getElementById("Schedulingday2off");
	e.innerHTML = _("scheduling day2 off");
	e = document.getElementById("Schedulingday2Tue");
	e.innerHTML = _("scheduling day2 tue");		
	e = document.getElementById("Schedulingday2StartHour");
	e.innerHTML = _("scheduling day2 starthour");									
	e = document.getElementById("Schedulingday2StartMin");
	e.innerHTML = _("scheduling day2 startmin");
	e = document.getElementById("Schedulingday2EndHour");
	e.innerHTML = _("scheduling day2 endhour");									
	e = document.getElementById("Schedulingday2EndMin");
	e.innerHTML = _("scheduling day2 endmin");
	
	e = document.getElementById("Schedulingday3on");
	e.innerHTML = _("scheduling day3 on");
	e = document.getElementById("Schedulingday3off");
	e.innerHTML = _("scheduling day3 off");
	e = document.getElementById("Schedulingday3Wed");
	e.innerHTML = _("scheduling day3 wed");		
	e = document.getElementById("Schedulingday3StartHour");
	e.innerHTML = _("scheduling day3 starthour");									
	e = document.getElementById("Schedulingday3StartMin");
	e.innerHTML = _("scheduling day3 startmin");
	e = document.getElementById("Schedulingday3EndHour");
	e.innerHTML = _("scheduling day3 endhour");									
	e = document.getElementById("Schedulingday3EndMin");
	e.innerHTML = _("scheduling day3 endmin");
	
	e = document.getElementById("Schedulingday4on");
	e.innerHTML = _("scheduling day4 on");
	e = document.getElementById("Schedulingday4off");
	e.innerHTML = _("scheduling day4 off");
	e = document.getElementById("Schedulingday4Thu");
	e.innerHTML = _("scheduling day4 thu");		
	e = document.getElementById("Schedulingday4StartHour");
	e.innerHTML = _("scheduling day4 starthour");									
	e = document.getElementById("Schedulingday4StartMin");
	e.innerHTML = _("scheduling day4 startmin");
	e = document.getElementById("Schedulingday4EndHour");
	e.innerHTML = _("scheduling day4 endhour");									
	e = document.getElementById("Schedulingday4EndMin");
	e.innerHTML = _("scheduling day4 endmin");
	
	e = document.getElementById("Schedulingday5on");
	e.innerHTML = _("scheduling day5 on");
	e = document.getElementById("Schedulingday5off");
	e.innerHTML = _("scheduling day5 off");
	e = document.getElementById("Schedulingday5Fri");
	e.innerHTML = _("scheduling day5 fri");		
	e = document.getElementById("Schedulingday5StartHour");
	e.innerHTML = _("scheduling day5 starthour");									
	e = document.getElementById("Schedulingday5StartMin");
	e.innerHTML = _("scheduling day5 startmin");
	e = document.getElementById("Schedulingday5EndHour");
	e.innerHTML = _("scheduling day5 endhour");									
	e = document.getElementById("Schedulingday5EndMin");
	e.innerHTML = _("scheduling day5 endmin");

	e = document.getElementById("Schedulingday6on");
	e.innerHTML = _("scheduling day6 on");
	e = document.getElementById("Schedulingday6off");
	e.innerHTML = _("scheduling day6 off");
	e = document.getElementById("Schedulingday6Sat");
	e.innerHTML = _("scheduling day6 sat");		
	e = document.getElementById("Schedulingday6StartHour");
	e.innerHTML = _("scheduling day6 starthour");									
	e = document.getElementById("Schedulingday6StartMin");
	e.innerHTML = _("scheduling day6 startmin");
	e = document.getElementById("Schedulingday6EndHour");
	e.innerHTML = _("scheduling day6 endhour");									
	e = document.getElementById("Schedulingday6EndMin");
	e.innerHTML = _("scheduling day6 endmin");						

	e = document.getElementById("Schedulingday7on");
	e.innerHTML = _("scheduling day7 on");
	e = document.getElementById("Schedulingday7off");
	e.innerHTML = _("scheduling day7 off");
	e = document.getElementById("Schedulingday7Sun");
	e.innerHTML = _("scheduling day7 sun");		
	e = document.getElementById("Schedulingday7StartHour");
	e.innerHTML = _("scheduling day7 starthour");									
	e = document.getElementById("Schedulingday7StartMin");
	e.innerHTML = _("scheduling day7 startmin");
	e = document.getElementById("Schedulingday7EndHour");
	e.innerHTML = _("scheduling day7 endhour");									
	e = document.getElementById("Schedulingday7EndMin");
	e.innerHTML = _("scheduling day7 endmin");
	
	e = document.getElementById("GeneralApply");
	e.innerHTML = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.innerHTML = _("general reset");

	e = document.getElementById("GeneralApplyX");
	e.innerHTML = _("general apply");
	
	e = document.getElementById("GeneralResetX");
	e.innerHTML = _("general reset");
}

function on_select_start_hour(index)
{
	var item1=eval("document.CfgWiFiScheduler."+"WLAN_scheduling_start_hour_"+index);
	var item2=eval("document.CfgWiFiScheduler."+"WLAN_scheduling_start_min_"+index);
	
	if (item1.options.selectedIndex==24){
		item2.options.selectedIndex=0;
		item2.disabled=true;
	}else{
		item2.disabled=false;
	}
}

function on_select_end_hour(index)
{
	var item1=eval("document.CfgWiFiScheduler."+"WLAN_scheduling_end_hour_"+index);
	var item2=eval("document.CfgWiFiScheduler."+"WLAN_scheduling_end_min_"+index);
	
	if (item1.options.selectedIndex==24){
		item2.options.selectedIndex=0;
		item2.disabled=true;
	}else{
		item2.disabled=false;
	}
}

</script>
</head>
<body class="bandwidth_bg" onLoad="initValue()">
<form method=post name="CfgWiFiScheduler" action="/goform/EZwifischeduler" >
<div class="wrapper">
<div class="bandwidth_d"><ul><li id="SchedulingTitleNote">Please schedule the wireless service with the table below.</li>
</ul></div>
<div class = "schedule" id= "schedule">
<table width="100%" border="0" cellpadding="0" cellspacing="0" >
<tr >
<td width="99" valign="top" nowrap><center>
<span class="top_font" id ="SchedulingWLANStatus">WLAN status </span>
</center></td>
<td width="86" valign="top"><center>
<span class="top_font" id ="SchedulingWLANDay">Day </span>
</center></td>
<td width="388"><center>
<span class="top_font" id ="SchedulingWLANExcept">Except for the following times   (24-Hour Format) </span>
</center></td>
</tr>
<tr >
<td valign="top" nowrap><center>
<span class="table_font">
<input name="WLAN_scheduling_action_0" value="1" type="radio" />
<font id ="Schedulingday0on">On</font>
<input name="WLAN_scheduling_action_0" value="0" type="radio" />
<font id ="Schedulingday0off">Off</font></span>
</center></td>
<td valign="top" nowrap><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_0" onclick="clickEveryday();" type="checkbox" value=1 />
<font id ="Schedulingday0Everyday">Everyday</font></span></div></td>
<td nowrap><div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_0" size="1" onchange="on_select_start_hour(0);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id ="Schedulingday0StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_0" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id ="Schedulingday0StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_0" size="1" onchange="on_select_end_hour(0);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id ="Schedulingday0EndHour">(hour)</font>
<select name="WLAN_scheduling_end_min_0" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id ="Schedulingday0EndMin">(min)</font></span>
</div></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_font">
<input name="WLAN_scheduling_action_1" value="1" type="radio" />
<font id="Schedulingday1on"> On</font>
<input name="WLAN_scheduling_action_1" value="0" type="radio" />
<font id="Schedulingday1off"> Off</font></span>
</center></td>
<td><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_1" onclick="checkAndSetWeekdays();" type="checkbox" value=1 />
<font id="Schedulingday1Mon"> Mon</font></span> </div></td>
<td><div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_1" size="1" onchange="on_select_start_hour(1);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday1StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_1" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday1StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_1" size="1" onchange="on_select_end_hour(1);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday1EndHour">(hour)</font>	
<select name="WLAN_scheduling_end_min_1" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday1EndMin">(min)</font></span> </div></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_font">
<input name="WLAN_scheduling_action_2" value="1" type="radio" />
<font id="Schedulingday2on"> On</font>
<input name="WLAN_scheduling_action_2" value="0" type="radio" />
<font id="Schedulingday2off"> Off</font></span>
</center></td>
<td><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_2" onclick="checkAndSetWeekdays();" type="checkbox" value=1 />
<font id="Schedulingday2Tue"> Tue</font></span> </div></td>
<td><div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_2" size="1" onchange="on_select_start_hour(2);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday2StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_2" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday2StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_2" size="1" onchange="on_select_end_hour(2);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday2EndHour">(hour)</font>
<select name="WLAN_scheduling_end_min_2" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday2EndMin">(min)</font></span></div></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_font">
<input name="WLAN_scheduling_action_3" value="1" type="radio" />
<font id="Schedulingday3on"> On</font>
<input name="WLAN_scheduling_action_3" value="0" type="radio" />
<font id="Schedulingday3off"> Off</font></span>
</center></td>
<td><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_3" onclick="checkAndSetWeekdays();" type="checkbox" value=1 />
<font id="Schedulingday3Wed"> Wed</font></span> </div></td>
<td> <div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_3" size="1" onchange="on_select_start_hour(3);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday3StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_3" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
 </select>
<font id="Schedulingday3StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_3" size="1" onchange="on_select_end_hour(3);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday3EndHour">(hour)</font>
<select name="WLAN_scheduling_end_min_3" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday3EndMin">(min)</font></span> </div></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_font">
<input name="WLAN_scheduling_action_4" value="1" type="radio" />
<font id="Schedulingday4on"> On</font>
<input name="WLAN_scheduling_action_4" value="0" type="radio" />
<font id="Schedulingday4off"> Off</font></span>
</center></td>
<td><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_4" onclick="checkAndSetWeekdays();" type="checkbox"  value=1 />
<font id="Schedulingday4Thu"> Thu</font> </span> </div></td>
 <td><div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_4" size="1" onchange="on_select_start_hour(4);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday4StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_4" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday4StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_4" size="1" onchange="on_select_end_hour(4);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday4EndHour">(hour)</font>
<select name="WLAN_scheduling_end_min_4" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday4EndMin">(min)</font></span> </div></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_font">
<input name="WLAN_scheduling_action_5" value="1" type="radio" />
<font id="Schedulingday5on"> On</font>
<input name="WLAN_scheduling_action_5" value="0" type="radio" />
<font id="Schedulingday5off"> Off</font> </span>
</center></td>
<td><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_5" onclick="checkAndSetWeekdays();" type="checkbox" value=1 />
<font id="Schedulingday5Fri"> Fri</font></span> </div></td>
<td><div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_5" size="1" onchange="on_select_start_hour(5);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday5StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_5" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday5StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_5" size="1" onchange="on_select_end_hour(5);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday5EndHour">(hour)</font>	
<select name="WLAN_scheduling_end_min_5" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday5EndMin">(min)</font></span> </div></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_font">
<input name="WLAN_scheduling_action_6" value="1" type="radio" />
<font id="Schedulingday6on"> On</font>
<input name="WLAN_scheduling_action_6" value="0" type="radio" />
<font id="Schedulingday6off"> Off</font></span>
</center></td>
<td><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_6" onclick="checkAndSetWeekdays();" type="checkbox" value=1 />
<font id="Schedulingday6Sat"> Sat</font> </span> </div></td>
<td><div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_6" size="1" onchange="on_select_start_hour(6);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday6StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_6" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday6StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_6" size="1" onchange="on_select_end_hour(6);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday6EndHour">(hour)</font>	
<select name="WLAN_scheduling_end_min_6" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday6EndMin">(min)</font></span> </div></td>
</tr>
<tr >
<td valign="top"><center>
<span class="table_font">
<input name="WLAN_scheduling_action_7" value="1" type="radio">
<font id="Schedulingday7on"> On</font>
<input name="WLAN_scheduling_action_7" value="0" type="radio">
<font id="Schedulingday7off"> Off</font></span>
</center></td>
<td><div align="left"><span class="table_font">
<input name="WLAN_Scheduling_day_Active_7" onClick="checkAndSetWeekdays();" type="checkbox" value=1 >
<font id="Schedulingday7Sun"> Sun</font></span> </div></td>
<td><div align="center"><span class="table_font">
<select name="WLAN_scheduling_start_hour_7" size="1" onchange="on_select_start_hour(7);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday7StartHour">(hour)</font>&nbsp;
<select name="WLAN_scheduling_start_min_7" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday7StartMin">(min)</font>&nbsp;&nbsp;&nbsp;~&nbsp;&nbsp;&nbsp;
<select name="WLAN_scheduling_end_hour_7" size="1" onchange="on_select_end_hour(7);">
<option value=00 >00 </option>
<option value=01 >01 </option>
<option value=02 >02 </option>
<option value=03 >03 </option>
<option value=04 >04 </option>
<option value=05 >05 </option>
<option value=06 >06 </option>
<option value=07 >07 </option>
<option value=08 >08 </option>
<option value=09 >09 </option>
<option value=10 >10 </option>
<option value=11 >11 </option>
<option value=12 >12 </option>
<option value=13 >13 </option>
<option value=14 >14 </option>
<option value=15 >15 </option>
<option value=16 >16 </option>
<option value=17 >17 </option>
<option value=18 >18 </option>
<option value=19 >19 </option>
<option value=20 >20 </option>
<option value=21 >21 </option>
<option value=22 >22 </option>
<option value=23 >23 </option>
<option value=24 >24 </option>
</select>
<font id="Schedulingday7EndHour">(hour)</font>	
<select name="WLAN_scheduling_end_min_7" size="1">
<option value=0 >00 </option>
<option value=1 >30 </option>
</select>
<font id="Schedulingday7EndMin">(min)</font></span> </div></td>
</tr>
</table>
</div>	
<span id="button_on" class="on">
<div  class="button"><ul><li><a href="empty.html" id="GeneralReset">Cancel
</a></li><li><a href="javascript: if (CheckValue()) { show_div(false, 'button_on'); show_div(true, 'button_off'); document.forms[0].submit(); }" id="GeneralApply">Apply</a></li>
</ul></div>
</span>
<span id="button_off" class="off">
<div  class="buttonX"><ul><li><a id="GeneralResetX">Cancel
</a></li><li><a id="GeneralApplyX">Apply</a></li>
</ul></div>
</span>
</div>
</form>
</body>
</html>
