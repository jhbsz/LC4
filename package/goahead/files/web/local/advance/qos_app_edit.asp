<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html>
<head>
<title> </title>
<meta http-equiv='content-type' content='text/html;charset=iso-8859-1'>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/tablet.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

<% QoSGatherValidRate(); %>

//var totWanSum;
//var totLanSum;
//var currLanSum;
//var currWanSum;

var listCount;
var oldBwE = new Array();
var oldBwSel = new Array();
var oldBwRate = new Array();

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

function rateCheck()
{
	var i,e;
	var newBwE = new Array();
	var newBwSel = new Array();
	var newBwRate = new Array();
	var cLan=0;
	var cWan =0;
	var delta;
	var diff_l_w;
	var finLanSum;
	var finWanSum;

	var service_enable=document.QoSAppCfg.service_e.value;
	
	//if( !totWanSum || !totLanSum || !currLanSum || !currWanSum)
	//	return false;

	for(i=0; i<listCount; i++)
	{
		e = document.getElementById("appEditEnable"+i);
		if(e.checked)
			newBwE[i]=1;
		else
			newBwE[i]=0;
	
		e = document.getElementById("appEditSelect"+i);
		newBwSel[i]=e.selectedIndex;
		e = document.getElementById("appEditRate"+i);
		newBwRate[i]=atoi(e.value, 1);
	}
	
	if(listCount==4) //game
		diff_l_w=1; // 0,1 lan; 2,3 wan 
	else
		diff_l_w=0;	
	
	if(service_enable==1)
	{
		//check max
		for(i=0; i<listCount; i++)
		{
			if( newBwE[i] ==1){
				if(newBwSel[i]==0){
					if( i >diff_l_w){
						if(newBwRate[i] > totWanSum){
							alert("Input maximum rate is greater than uplink rate.");
							return false;
						}
					}else{
						if(newBwRate[i] > totLanSum){
							alert("Input maximum rate is greater than total downlink rate.");
							return false;
						}
					}				
				}
			}
		}
	
		//alert(listCount);
		//check min
		for(i=0; i<listCount; i++)
		{	
			if(newBwE[i] != oldBwE[i]){ //enable change
				if( newBwE[i] ==1){ //dis -> enab
					if(newBwSel[i] != oldBwSel[i]){ //sel change
						if( newBwSel[i] ==1){ //max -> min
							if(newBwSel[i]==1){ //min plus
								if(i>diff_l_w)
									cWan+=newBwRate[i];
								else
									cLan+=newBwRate[i];
							}
						}
					}else{ //sel no change
						if( newBwSel[i] ==1){ //min->min					
							if(i>diff_l_w)
								cWan+=newBwRate[i];
							else
								cLan+=newBwRate[i];
						}
					}
				}else{ //enable-> dis
					if(newBwSel[i] != oldBwSel[i]){ //sel change
						if( newBwSel[i] ==0){ //min -> max
							if(i>diff_l_w)
								cWan-=oldBwRate[i];
							else
								cLan-=oldBwRate[i];					
						}
					}else{ //sel no change
						if( newBwSel[i] ==1){ //min->min					
							if(i>diff_l_w)
								cWan-=oldBwRate[i];
							else
								cLan-=oldBwRate[i];
						}
					}
				}
			}else{ //enbale no change
				if(newBwE[i]==1){ //enable
					if(newBwSel[i] != oldBwSel[i]){ //sel change
						if( newBwSel[i] ==1){ //max -> min
							if(newBwSel[i]==1){ //min plus
								if(i>diff_l_w)
									cWan+=newBwRate[i];
								else
									cLan+=newBwRate[i];
							}
						}else{ //min-> max
							if(newBwSel[i]==0){ 
								if(i>diff_l_w)
									cWan-=oldBwRate[i];
								else
									cLan-=oldBwRate[i];
							}
						}
					}else{  //sel no change
						if( newBwSel[i] ==1){ //min->min
							delta = newBwRate[i]-oldBwRate[i];
							if(i>diff_l_w)
								cWan+=delta;
							else
								cLan+=delta;
						}
					}
				}
			}			
		}
		//alert("Lan min rate sum=" +cLan);	
		finLanSum= currLanSum + cLan;
		//alert("final LAN sum=" + finLanSum);
		if(finLanSum > totLanSum ){		
			alert("Input minimum rate is greater than downlink rate.");
			return false;
		}

		//alert("Wan min rate sum=" +cWan);		
		finWanSum= currWanSum + cWan;
		//alert("final WAN sum=" + finWanSum);
		if(finWanSum > totWanSum ){
			alert("Input minimum rate is greater than uplink rate.");
			return false;
		}
	}

	//if disable, but number is too big, we care it.
	for(i=0; i<listCount; i++)
	{
		if( i >diff_l_w){
			if(newBwRate[i] > totWanSum){
				e = document.getElementById("appEditSelect"+i);
				e.selectedIndex=1;
				e = document.getElementById("appEditRate"+i);
				e.value=10;
			}
		}else{
			if(newBwRate[i] > totLanSum){
				e = document.getElementById("appEditSelect"+i);
				e.selectedIndex=1;
				e = document.getElementById("appEditRate"+i);
				e.value=10;
			}
		}
	}

	return true;
}

function isNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9'))
			continue;
		return 0;
	}
	return 1;
}

function doSubmit(buttonSel)
{
	if(buttonSel==1)
	{
		if(!rateCheck())
			return false;
		
		document.forms[0].BW_ButtonFlag.value=1;
		showWebMessage(2, "");
	}else if(buttonSel==2)
		document.forms[0].BW_ButtonFlag.value=2;

	return true;
}
function checkBWBudget(budgetObj)
{
	if (budgetObj.value == "")
	{
		alert("The value of bandwidth can not accept empty.");
		budgetObj.value = 10;
		return;
	}
	if(!isNum(budgetObj.value))
	{
		alert("The value of bandwidth is not valid value.");
		budgetObj.value = 10;
		return;
	}
	
}

function initTranslation()
{
	var e;
	var i;

	e = document.getElementById("appEditTitle");
	e.innerHTML = _("app edit title");
	e = document.getElementById("appEditEnable");
	e.innerHTML = _("app edit enable");
	e = document.getElementById("appEditDir");
	e.innerHTML = _("app edit dir");
	e = document.getElementById("appEditBandwidth");
	e.innerHTML = _("app edit bandwidth");
	e = document.getElementById("appEditDstPort");
	e.innerHTML = _("app edit dst port");
	e = document.getElementById("appEditSrcPort");
	e.innerHTML = _("app edit src port");
	e = document.getElementById("appEditProto");
	e.innerHTML = _("app edit proto");

	for(i=0; i<4; i++){
		e = document.getElementById("appEditMaxBW"+i);
		if(e)
			e.innerHTML = _("app edit max bw");

		e = document.getElementById("appEditMinBW"+i);
		if(e)
			e.innerHTML = _("app edit min bw");
	}
	e = document.getElementById("appEditCancel");
	e.value = _("app edit cancel");
	e = document.getElementById("appEditapply");
	e.value = _("adv apply");

	//2009-07-08, RL added,  check min sum;	
	var x;
	if(document.QoSAppCfg.cfg_idx){
		x=document.QoSAppCfg.cfg_idx.value-1;
		if(x==0 || x==1 || x==2)
			listCount=4;
		else
			listCount=2;
	} 				

	for(i=0; i<listCount; i++)
	{
		e = document.getElementById("appEditEnable"+i);
		if(e.checked)
			oldBwE[i]=1;
		else
			oldBwE[i]=0;

		//alert("old"+oldBwE[i]);
		
		e = document.getElementById("appEditSelect"+i);
		oldBwSel[i]=e.selectedIndex;
		e = document.getElementById("appEditRate"+i);
		oldBwRate[i]=atoi(e.value, 1);
		//alert( oldBwRate[i], 1);
	}
}

</script>

<style type="text/css">
<!--
.wtext_3 {font-family:Arial, Helvetica, sans-serif;color:#000000;font-size:11px;}
.cfg_title{ font-family:Arial, Helvetica, sans-serif; color:#385f95; font-size:11px; font-weight:bold; height:25px; line-height:25px;}
-->
</style>

</head>
<body onload="initTranslation();">
<div id="table">
	<ul>
    	<li class="table_content">
      		<div class="data">
        		<ul>
			   		<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="appEditTitle">Rule Configuration</td>
                   <td>- <% QoSGetInfo("qos_service"); %></td>
                   </tr></table>
			   		</li>
					<li>&nbsp;</li>
				</ul>
    		</div>
	<form method="post" name="QoSAppCfg" action="/goform/QoSAppCfg">

	<table width="100%"  border="0" cellspacing="0" cellpadding="0">
		<tr>
			<td>
				<table width="92%" border="0" align=center cellspacing="0" class="wtext_3">
					<tr><td width="5%" ><center><span class="top_left">#</span></center></td>
						<td width="8%" height="26"><span class="top_font" id="appEditEnable">Enable</span></center></td>
						<td width="16%"><center><span class="top_font" id="appEditDir">Direction</span></center></td>
						<td width="35%"><center><span class="top_font" id="appEditBandwidth">Bandwidth</span></center></td>
						<td width="15%"><center><span class="top_font" id="appEditDstPort">Destination Port</span></center></td>
						<td width="13%"><center><span class="top_font" id="appEditSrcPort">Source Port</span></center></td>
						<td width="8%"><center><span class="top_right" id="appEditProto">Protocol</span></center></td>
					</tr>
<% asp_AppCfgQoS(); %>

				</table>

				<table border="0" cellspacing="0" cellpadding="0" width="100%" align=center>
					<tr>
						<td height="10">&nbsp;&nbsp;</td>
					</tr>
					<tr>
						<td align=center class="table_button">
							<input type="submit" name="BW_CCfg_OK" id="appEditapply" value="OK" onClick="return doSubmit(1);" />
							<input type="hidden" name="BW_ButtonFlag" value="0" />
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
							<input type="submit" name="Bw_Cancel" id="appEditCancel" value="Cancel" onClick="doSubmit(2);">
						</td>
					</tr>
				</table>

			</td>
		</tr>
	</table>
	</form>
	</li>
</ul>
</div>

</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>
</html>
