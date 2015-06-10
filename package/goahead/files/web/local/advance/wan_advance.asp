<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var opmode = '<% getCfgZero(1, "OP_Mode"); %>';

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

function CheckValue()
{
   document.lanCfg.WAN_AUTO_BRIDGE.value = document.lanCfg.WAN_AUTO_BRIDGE.checked ? "1": "0";
   showWebMessage(2, "");
   return true;
}

function initValue()
{
	initTranslation();

   var igmp_enabled = "<% getCfgGeneral(1, "igmpEnabled"); %>";
   
   show_div(false, "igmp_setting");
   show_div(false, "autoBridge_setting");
    
   if (opmode == "0") {
        show_div(true, "igmp_setting");
        show_div(true, "autoBridge_setting");
   }
   else if(opmode == "6") {
        show_div(false, "igmp_setting");
        show_div(true, "autoBridge_setting");
   }
   
   if (igmp_enabled == "0")
   {
       document.lanCfg.igmpEnbl.options.selectedIndex = 0;
   }
   else if (igmp_enabled == "1")
   {
       document.lanCfg.igmpEnbl.options.selectedIndex = 1;
   }
   
    var auto_br_mode = <% getCfgZero(1, "AutoBridge"); %>;

	if (auto_br_mode == 0)
		document.lanCfg.WAN_AUTO_BRIDGE[0].checked = true;
	else if (auto_br_mode == 1)
		document.lanCfg.WAN_AUTO_BRIDGE[1].checked = true;
	else if (auto_br_mode == 2)
		document.lanCfg.WAN_AUTO_BRIDGE[2].checked = true;
	else 
		document.lanCfg.WAN_AUTO_BRIDGE[0].checked = true;
    
   //if (document.lanCfg.WAN_AUTO_BRIDGE.value == "1")
   //	document.lanCfg.WAN_AUTO_BRIDGE.checked = true; 
   
//    parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
}

function initTranslation(){
	var e = document.getElementById("Auto_subnet_configuration");
	e.innerHTML = _("auto subnet configuration");
	e = document.getElementById("Multicast");
	e.innerHTML = _("Multicast");		
	e = document.getElementById("Multicast_None");
	e.innerHTML = _("rip None");		
	e = document.getElementById("Enable_Auto_bridge_mode");
	e.innerHTML = _("Enable_Auto_bridge_mode");		
	e = document.getElementById("igmpApply");
	e.value = _("rip Apply");		
	e = document.getElementById("igmpReset");
	e.value = _("rip Reset");		
	
	e = document.getElementById("WAN_AUTO_BRIDGE_None");
	e.innerHTML = _("wan auto bridge none");
	e = document.getElementById("Enable_Auto_ip_change_mode");
	e.innerHTML = _("enable auto ip change mode");
	e = document.getElementById("AutoIP_Note_0");
	e.innerHTML = _("auto ip note_0");
	e = document.getElementById("AutoIP_Note_1");
	e.innerHTML = _("auto ip note_1");
	
}
</script>
</head>

<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLanIGMP" onSubmit="return CheckValue()">
 <div id="table">
  <ul>
   <li class="table_content">
    <div class="data">
     <ul>
       <script>
       if (opmode == "0") {
       document.write("<li class='title'>"+_('Multicast_Setup')+"</li>");
       }
       </script>
       <span id = "igmp_setting" class = "off" >
       <!-- for igmp proxy (BEGIN) -->
       		<li class="w_text">
       		  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
       		    <tr>
       		      <td width="40%" id=Multicast>Multicast : </td>
       		      <td><span class="cfig_word">
       		        <select name="igmpEnbl" size="1">
       		          <option value="0" id=Multicast_None>None </option>
       		          <option value="1">IGMPv1/v2 </option>
       		        </select>
       		      </span></td>
       		    </tr>
       		  </table>
       		</li>  
            <li class="space3"></li>
       <!-- for igmp proxy (END)-->
       </span>
       <span id = "autoBridge_setting" class = "off" >
            <li class="title" id=Auto_subnet_configuration>Auto-Subnet Configuration </li>      
            <li class="w_text">
              <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr>
   	          	<td width="3%"><input name="WAN_AUTO_BRIDGE" type="radio"  value="0"></td>
	 		  	<td id=WAN_AUTO_BRIDGE_None>None </td>
   	         </tr>
   	        </table>
   	        </li>
   	            
   	        <li class="w_text">
   	        <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
   	         <tr>
   	          <td width="3%"><input name="WAN_AUTO_BRIDGE" type="radio"  value="1"></td>
			  <td id=Enable_Auto_bridge_mode>Enable Auto-bridge mode </td>
             </tr>
            </table>
            </li>
   	            
   	        <li class="w_text">
   	        <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
   	         <tr>
   	          <td width="3%"><input name="WAN_AUTO_BRIDGE" type="radio"  value="2"></td>
	 		  <td id=Enable_Auto_ip_change_mode>Enable Auto-IP-Change mode </td>
   	         </tr>
   	        </table>
   	        </li>
   	        
   	        <li class="w_text">
			<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
			<tr>
			<td>&nbsp;</td>
			</tr>
			</table>
			</li>

			<li class="w_text">
			<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
			<tr>
			<td colspan="2" class="note">
			<span class="i_note" width="18" height="17"  align="absmiddle" id="AutoIP_Note_0">Note:</span></td>
			</td>
			</tr>
			</table>
			</li>
			
			<li class="w_text">
			<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
			<tr>
			<td colspan="2" class="i_note_a" id="AutoIP_Note_1">Note: If you choose to enable Auto-IP-Change mode, the LAN IP will be automatically changed to 10.0.0.1 when WAN IP and LAN IP conflicts.</td>
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
        <input type=submit value="Apply" id="igmpApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="igmpReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/wan_advance.asp" name="igmp_url">
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
