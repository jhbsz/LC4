<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
    
function initTranslation()
{
	var e = document.getElementById("NatAdvanveAppRuleSum");
	e.innerHTML = _("nat advance app rule sum");
	e = document.getElementById("NatAdvanvePortTriggerRule");
	e.innerHTML = _("nat advance port trigger rule");		
	e = document.getElementById("NatAdvanveName");
	e.innerHTML = _("nat advance name");		
	e = document.getElementById("NatAdvanveIncoming");
	e.innerHTML = _("nat advance incoming");		
	e = document.getElementById("NatAdvanveTrigger");
	e.innerHTML = _("nat advance trigger");		
	e = document.getElementById("NatAdvanvePort1");
	e.innerHTML = _("nat advance port1");
	e = document.getElementById("NatAdvanveEndPort1");
	e.innerHTML = _("nat advance endport1");	
	e = document.getElementById("NatAdvanvePort2");
	e.innerHTML = _("nat advance port2");	
	e = document.getElementById("NatAdvanveEndPort2");
	e.innerHTML = _("nat advance endport2");			
	e = document.getElementById("natApply");
	e.value = _("nat apply");
	e = document.getElementById("natReset");
	e.value = _("nat reset");
}

function checkInput(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) == '%' ) || (str.charAt(i) == "'" )  || (str.charAt(i) == '"' )  || (str.charAt(i) == '<' )  || (str.charAt(i) == '>' ))
		return 1;
	}
	return 0;
}

function formCheck()
{
	var ui_inS= new Array(12); 
	var ui_inE= new Array(12); 
	var ui_trS= new Array(12); 
	var ui_trE= new Array(12);
	var ui_name = new Array(12);
		
	ui_inS[0] = document.NAT_ADV.PortRule_IncS1.value;
	ui_inS[1] = document.NAT_ADV.PortRule_IncS2.value;
	ui_inS[2] = document.NAT_ADV.PortRule_IncS3.value;
	ui_inS[3] = document.NAT_ADV.PortRule_IncS4.value;
	ui_inS[4] = document.NAT_ADV.PortRule_IncS5.value;
	ui_inS[5] = document.NAT_ADV.PortRule_IncS6.value;
	ui_inS[6] = document.NAT_ADV.PortRule_IncS7.value;
	ui_inS[7] = document.NAT_ADV.PortRule_IncS8.value;
	ui_inS[8] = document.NAT_ADV.PortRule_IncS9.value;
	ui_inS[9] = document.NAT_ADV.PortRule_IncS10.value;
	ui_inS[10] = document.NAT_ADV.PortRule_IncS11.value;
	ui_inS[11] = document.NAT_ADV.PortRule_IncS12.value;	
	ui_inE[0] = document.NAT_ADV.PortRule_IncE1.value;
	ui_inE[1] = document.NAT_ADV.PortRule_IncE2.value;
	ui_inE[2] = document.NAT_ADV.PortRule_IncE3.value;
	ui_inE[3] = document.NAT_ADV.PortRule_IncE4.value;
	ui_inE[4] = document.NAT_ADV.PortRule_IncE5.value;
	ui_inE[5] = document.NAT_ADV.PortRule_IncE6.value;
	ui_inE[6] = document.NAT_ADV.PortRule_IncE7.value;
	ui_inE[7] = document.NAT_ADV.PortRule_IncE8.value;
	ui_inE[8] = document.NAT_ADV.PortRule_IncE9.value;
	ui_inE[9] = document.NAT_ADV.PortRule_IncE10.value;
	ui_inE[10] = document.NAT_ADV.PortRule_IncE11.value;
	ui_inE[11] = document.NAT_ADV.PortRule_IncE12.value;	
	ui_trS[0] = document.NAT_ADV.PortRule_TriS1.value;
	ui_trS[1] = document.NAT_ADV.PortRule_TriS2.value;
	ui_trS[2] = document.NAT_ADV.PortRule_TriS3.value;
	ui_trS[3] = document.NAT_ADV.PortRule_TriS4.value;
	ui_trS[4] = document.NAT_ADV.PortRule_TriS5.value;
	ui_trS[5] = document.NAT_ADV.PortRule_TriS6.value;
	ui_trS[6] = document.NAT_ADV.PortRule_TriS7.value;
	ui_trS[7] = document.NAT_ADV.PortRule_TriS8.value;
	ui_trS[8] = document.NAT_ADV.PortRule_TriS9.value;
	ui_trS[9] = document.NAT_ADV.PortRule_TriS10.value;
	ui_trS[10] = document.NAT_ADV.PortRule_TriS11.value;
	ui_trS[11] = document.NAT_ADV.PortRule_TriS12.value;	
	ui_trE[0] = document.NAT_ADV.PortRule_TriE1.value;
	ui_trE[1] = document.NAT_ADV.PortRule_TriE2.value;
	ui_trE[2] = document.NAT_ADV.PortRule_TriE3.value;
	ui_trE[3] = document.NAT_ADV.PortRule_TriE4.value;
	ui_trE[4] = document.NAT_ADV.PortRule_TriE5.value;
	ui_trE[5] = document.NAT_ADV.PortRule_TriE6.value;
	ui_trE[6] = document.NAT_ADV.PortRule_TriE7.value;
	ui_trE[7] = document.NAT_ADV.PortRule_TriE8.value;
	ui_trE[8] = document.NAT_ADV.PortRule_TriE9.value;
	ui_trE[9] = document.NAT_ADV.PortRule_TriE10.value;
	ui_trE[10] = document.NAT_ADV.PortRule_TriE11.value;
	ui_trE[11] = document.NAT_ADV.PortRule_TriE12.value;
	
	ui_name[0] = document.NAT_ADV.PortRule_Name1.value;
	ui_name[1] = document.NAT_ADV.PortRule_Name2.value;
	ui_name[2] = document.NAT_ADV.PortRule_Name3.value;
	ui_name[3] = document.NAT_ADV.PortRule_Name4.value;
	ui_name[4] = document.NAT_ADV.PortRule_Name5.value;
	ui_name[5] = document.NAT_ADV.PortRule_Name6.value;
	ui_name[6] = document.NAT_ADV.PortRule_Name7.value;
	ui_name[7] = document.NAT_ADV.PortRule_Name8.value;
	ui_name[8] = document.NAT_ADV.PortRule_Name9.value;
	ui_name[9] = document.NAT_ADV.PortRule_Name10.value;
	ui_name[10] = document.NAT_ADV.PortRule_Name11.value;
	ui_name[11] = document.NAT_ADV.PortRule_Name12.value;

	
	//check value
	for(i=0;i<12;i++){
	
		if(checkInput(ui_name[i])==1){
    		alert("System do not support specific invalid characters.");
			return false;
    	}
	
	   if(ui_inS[i]!=0){	
	        if(ui_inS[i] > 65535 || ui_inS[i] < 1){
				alert("Please set port between 1~65535");
				return false;
			}
			
			if(ui_inE[i] > 65535 || ui_inE[i] < 1){
				alert("Please set port between 1~65535");
				return false;
			}
			
			if(ui_trS[i] > 65535 || ui_trS[i] < 1){
				alert("Please set port between 1~65535");
				return false;
			}
			
			if(ui_trE[i] > 65535 || ui_trE[i] < 1){
				alert("Please set port between 1~65535");
				return false;
			}	        
	        	   
	        if(parseInt(ui_inE[i],10) < parseInt(ui_inS[i],10)){
	         	alert('Incoming End port is error');
	         	return false;
	    	}	    	 	   
	        if(ui_trS[i]==0){
	    		alert('Trigger port should not be zero');
	         	return false;
	    	} 	    		         
	    	if(parseInt(ui_trE[i],10) < parseInt(ui_trS[i],10)){
	         	alert('Trigger End port is error');
	         	return false;
	    	}		    	
	   }     
	}	
	//check range
	for(i=1;i<12;i++){
	    if(ui_inS[i]!=0){	          
	         for(j=0;j < i;j++){	    
	            if((ui_inS[i]>=ui_inS[j])&& (ui_inS[i]<=ui_inE[j])){
	               alert('Incoming port is exist');
	               return false;
	             }	             
	             if((ui_inE[i]>=ui_inS[j])&& (ui_inE[i]<=ui_inE[j])){
	               alert('Incoming End port is exist');
	               return false;
	             }	            
	            if((ui_trS[i]>=ui_trS[j])&& (ui_trS[i]<=ui_trE[j])){
	               alert('Trigger port is exist');	             
	               return false;   
	               }            
	            if((ui_trE[i]>=ui_trS[j])&& (ui_trE[i]<=ui_trE[j])){
	               alert('Trigger End port is exist');	             
	               return false;   
	               }   
	         }
	    }
	}
	showWebMessage(2, "");
	return true;	
}

function updateState()
{
	initTranslation();	
}
    
</script>
</head>
<body onload="updateState()">
<form method="post" name="NAT_ADV" action="/goform/portTrigger_natADV" >
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="NatAdvanveAppRuleSum">Application Rules Summary </li>
<li class="space"></li>
<li class="w_text2">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td><table width="100%" cellpadding="0" cellspacing="0" >
<tr>
<td height="26" colspan="6"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td id="NatAdvanvePortTriggerRule" background="images/table_top_center.gif"class="table_title"> 	Port Triggering Rules <span class="cTitle"></span></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>
<tr>
<td width="5%" rowspan="2" valign="top" align=center><span class="top_left3">#</span></td>
<td align=center rowspan=2><font class="top_font3"><span id="NatAdvanveName">Name</span></font></td>
<td align=center colspan="2"><font class="top_font"><span id="NatAdvanveIncoming">Incoming</span></font></td>
<td align=center colspan="2"><font class="top_right"><span id="NatAdvanveTrigger">Trigger</span></font></td>
</tr>
<tr>
<td align=center width="20%"><font class="top2_font"><span id="NatAdvanvePort1">Port</span></font></td>
<td align=center width="20%"><font class="top2_font"><span id="NatAdvanveEndPort1">End Port</span></font></td>
<td align=center width="20%"><font class="top2_font"><span id="NatAdvanvePort2">Port</span></font></td>
<td align=center width="20%"><font class="top2_right"><span id="NatAdvanveEndPort2">End Port</span></font></td>
</tr>
<% showNatAdvanceRulesASP(); %>  
<tr>
<td height="5" colspan="6"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
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
<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type="submit" value="Apply" id="natApply" name="addNAT_ADV" onClick="return formCheck()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
<input type="reset" value="Reset" id="natReset" name="reset" onClick="window.location.reload()">
<input type=hidden value="/local/advance/nat_advance.asp" name="nat_adv_url">
</li>
</center>
</ul>    
</div>
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
