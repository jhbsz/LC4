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

var userEnb;
var userDire;
var oldBwSel;
var oldBwRate;

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
	if(d!=0){
		if (d > max || d < min)
			return false;
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

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function rateCheck()
{
	var i,e;
	var newBwSel;
	var newBwRate;
	var delta=0;
	var finLanSum;
	var finWanSum;
	
	//if( !totWanSum || !totLanSum || !currLanSum || !currWanSum)
	//	return false;

	e = document.getElementById("userEditSelect");
	newBwSel=e.selectedIndex;
	e = document.getElementById("userEditRate");
	newBwRate=atoi(e.value, 1);
	
	if(userEnb==1)
	{ 	
		//check max
		if(newBwSel == 0){
			if(userDire==0){
				if(newBwRate > totLanSum){
					alert("Input maximum rate is greater than downlink rate.");
					return false;
				}
			}else{
				if(newBwRate > totWanSum){
					alert("Input maximum rate is greater than uplink rate.");
					return false;
				}
			}
		}
		
		//check min
		if(newBwSel != oldBwSel){//sel change 
			if(newBwSel == 0){ //min -> max
				delta-=oldBwRate;
			}else{ //max->min
				delta+=newBwRate;
			}
		}else{ //sel no changed
			if(newBwSel == 1){ //min -> min
				delta= newBwRate - oldBwRate;
			}
		}

		if(userDire==0){ //to lan
			alert(delta);
			finLanSum = currLanSum + delta;
			if(finLanSum > totLanSum ){		
				alert("Input minimum rate is greater than downlink rate.");
				return false;
			}
		}else{
			alert(delta);
			finWanSum = currWanSum + delta;
			if(finWanSum > totWanSum ){		
				alert("Input minimum rate is greater than uplink rate.");
				return false;
			}
		}		
	}

	//if disable, but number is too big, we care it.
	if(userDire==0){ 
		if(newBwRate > totLanSum ){
			e = document.getElementById("userEditSelect");
			e.selectedIndex=1;
			e = document.getElementById("userEditRate");
			e.value=10;
		}
	}else{
		if(newBwRate > totWanSum ){
			e = document.getElementById("userEditSelect");
			e.selectedIndex=1;
			e = document.getElementById("userEditRate");
			e.value=10;
		}
	}

	return true;
}

function checkIpAddr(field)
{
if (field.value != "") {
	if (isAllNum(field.value) == 0) {
		alert('It should be a [0-9] number.');
		field.value = field.defaultValue;
		field.focus();
		return false;
	}	
	
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

function doSubmit(buttonSel)
{
	if(buttonSel==1)
	{
	    if(!checkIpAddr(document.forms[0].BWDestAddr))
			return false;
		
		if(!checkIpAddr(document.forms[0].BWDestNetmask))
			return false;
		
		if(!checkIpAddr(document.forms[0].BWSrcAddr))
			return false;
		
		if(!checkIpAddr(document.forms[0].BWSrcNetmask))
			return false;
			
		if(!rateCheck())
			return false;
		
		document.forms[0].BW_ButtonFlag.value=1;
		showWebMessage(2, "");
	}else if(buttonSel==2)
		document.forms[0].BW_ButtonFlag.value=2;

	return true;
}

function doBWProtocol()
{
/*
	if ( document.forms[0].BWCCfgProtocolSel.selectedIndex == 2 )
	{
		document.forms[0].BWCCfgProtocol.disabled = false;
	}
	else
	{
		document.forms[0].BWCCfgProtocol.disabled = true;
	}
*/
}
function chk_wzBWWANBudget(value)
{
	if (value == "")
	{
		alert("The value of bandwidth can not accept empty.");
		document.forms[0].BWCCBudget.value = 10;
		return;
	}
	if(!isNum(value))
	{
		alert("The value of bandwidth is not valid value.");
		document.forms[0].BWCCBudget.value = 10;
		return;
	}
}
function checkBWPort( portObj)
{
	if (portObj.value == "")
	{
		alert("The value of port can not accept empty.");
		portObj.value = 0;
		return;
	}
	var portVal=atoi(portObj.value, 1);
	if ( isNaN(portVal) || portVal < 0 || portVal > 65535 )
	{
		alert("The value of port is not valid value.");
		portObj.value = 0;
		return;
	}
}

function SubnetCheck(whichAddr, mask)
{
	if (mask.value == ""){
			alert("The value of netmask can not accept empty.");
			mask.value="0.0.0.0";
			return;
		}
	if(whichAddr.value == "0.0.0.0")
		mask.value="0.0.0.0";
	else
		SubnetCheck2(mask);

	return;
}
function SubnetCheck2(mask)
{
	if ( !((mask.value == "128.0.0.0")||
     	(mask.value == "192.0.0.0")||
     	(mask.value == "224.0.0.0")||
     	(mask.value == "240.0.0.0")||
     	(mask.value == "248.0.0.0")||
     	(mask.value == "252.0.0.0")||
     	(mask.value == "254.0.0.0")||
     	(mask.value == "255.0.0.0")||
     	(mask.value == "255.128.0.0")||
     	(mask.value == "255.192.0.0")||
     	(mask.value == "255.224.0.0")||
     	(mask.value == "255.240.0.0")||
     	(mask.value == "255.248.0.0")||
     	(mask.value == "255.252.0.0")||
     	(mask.value == "255.254.0.0")||
     	(mask.value == "255.255.0.0")||
     	(mask.value == "255.255.128.0")||
     	(mask.value == "255.255.192.0")||
     	(mask.value == "255.255.224.0")||
     	(mask.value == "255.255.240.0")||
     	(mask.value == "255.255.248.0")||
     	(mask.value == "255.255.252.0")||
     	(mask.value == "255.255.254.0")||
     	(mask.value == "255.255.255.0")||
     	(mask.value == "255.255.255.128")||
     	(mask.value == "255.255.255.192")||
     	(mask.value == "255.255.255.224")||
     	(mask.value == "255.255.255.240")||
     	(mask.value == "255.255.255.248")||
     	(mask.value == "255.255.255.252")||
			(mask.value == "255.255.255.254")||
      (mask.value == "255.255.255.255")
		) )
	{
		alert("Subnet Mask error");
    	 	mask.value = "0.0.0.0";
    	 	return(false);
	}
}

function checkProtoVal(pObj)
{
	if (pObj.value == "")
	{
		alert("The value of protocol can not accept empty.");
		pObj.value = 0;
		return;
	}
	var pVal=atoi(pObj.value, 1);
	if ( isNaN(pVal) || pVal < 0 || pVal > 65535 )
	{
		alert("The value of protocol is not valid value.");
		pObj.value = 0;
		return;
	}
}

function initTranslation()
{
	var e;
	var i;

	e = document.getElementById("userEditTitle");
	e.innerHTML = _("user edit title");
	e = document.getElementById("userEditBW");
	e.innerHTML = _("user edit bw");
	e = document.getElementById("userEditMaxBW");
	e.innerHTML = _("user edit max bw");
	e = document.getElementById("userEditMinBW");
	e.innerHTML = _("user edit min bw");
	e = document.getElementById("userEditDstAddr1");
	e.innerHTML = _("user edit dst addr1");
	e = document.getElementById("userEditDstAddr2");
	e.innerHTML = _("user edit dst addr2");
	e = document.getElementById("userEditDstPort");
	e.innerHTML = _("user edit dst port");
	e = document.getElementById("userEditSrcAddr1");
	e.innerHTML = _("user edit src addr1");
	e = document.getElementById("userEditSrcAddr2");
	e.innerHTML = _("user edit src addr2");
	e = document.getElementById("userEditSrcPort");
	e.innerHTML = _("user edit src port");
	e = document.getElementById("userEditProto");
	e.innerHTML = _("user edit proto");
	//e = document.getElementById("userEditProtoDefine");
	//e.innerHTML = _("user edit proto define");
	e = document.getElementById("userEditCancel");
	e.value = _("user edit cancel");
	e = document.getElementById("userEditapply");
	e.value = _("adv apply");

	//doBWProtocol();

	//alert("old"+oldBwE[i]);
	userEnb=document.QoSUserCfg.service_e.value;
	userDire=document.QoSUserCfg.service_dire.value;

	e = document.getElementById("userEditSelect");
	oldBwSel=e.selectedIndex;
	e = document.getElementById("userEditRate");
	oldBwRate=atoi(e.value, 1);
	//alert( oldBwRate[i], 1);

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
			   		<li class="title" ><font id="userEditTitle">Rule Configuration></font> - <% QoSGetInfo("qos_usr_service"); %></li>
					<li>&nbsp;</li>
				</ul>
    		</div>
			<form method="post" name="QoSUserCfg" action="/goform/QoSUserCfg">
			<table width="100%" border="0" cellspacing="0" cellpadding="0" >
				<tr>
					<td>
				  		<table width="90%" border="0" align=center cellspacing="0" class="wtext_3">

<% asp_UsrCfgQoS(); %>

						</table>

						<table border="0" cellspacing="0" cellpadding="0" width="100%" align=center>
							<tr>
								<td height="10">&nbsp;&nbsp;</td>
							</tr>
							<tr>
								<td align=center class="table_button">
									<input type="submit" name="BW_CCfg_OK" id=userEditapply value="OK" onClick="return doSubmit(1);" >
									<input type="hidden" name="BW_ButtonFlag" value="0" >
									&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
									<input type="submit" name="Bw_Cancel" id="userEditCancel" value="Cancel" onClick="doSubmit(2);">
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
