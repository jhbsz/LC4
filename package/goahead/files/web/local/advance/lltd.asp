<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
  
  <script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("firewall");
    
function initTranslation()
{

}    

function firewallClick()
{
}


function updateState()
{
	initTranslation();	
}
    
</script>   
</head>
<body onload="updateState()">
<form method="post" name="LLTD" action="" >
<div id="table">
<ul>
	<li class="table_content">
    <div class="data">
    <ul>
    	<li class="title">LAN Info - <% getCfgLLTD(0, "00",0); %></li>
        <li class="w_text2">
        <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
        	<tr>            
            	<td width="30%"><span class="table_left">Device LAN 1 MAC</span></td>
            	<td><span class="table_right2"><% getCfgLLTD(0, "01",1); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 1 IP</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "07",1); %></span</td>
            </tr>
            <tr>            
                <td width="30%"><span class="table_left">Device LAN 1 Machine Name</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "15",1); %></span</td>
            </tr>
            <tr>            
                <td width="30%"><span class="table_left">Device LAN 1 Type</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "99",1); %></span</td>
            </tr>
            <tr><td>&nbsp;&nbsp;</td><td>&nbsp;&nbsp;</td></tr>
            <tr>            
                <td width="30%"><span class="table_left">Device LAN 2 MAC</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "01",2); %></span</td>
            </tr>
            <tr>            
                <td width="30%"><span class="table_left">Device LAN 2 IP</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "07",2); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 2 Machine Name</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "15",2); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 2 Type</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "99",2); %></span</td>
            </tr>
            <tr><td>&nbsp;&nbsp;</td><td>&nbsp;&nbsp;</td></tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 3 MAC</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "01",3); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 3 IP</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "07",3); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 3 Machine Name</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "15",3); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 3 Type</span></td>
                <td><span class="table_right2"><% getCfgLLTD(0, "99",3); %></span</td>
            </tr>
            <tr><td>&nbsp;&nbsp;</td><td>&nbsp;&nbsp;</td></tr>
        </table>
        </li>
        <li class="space3"></li>
        <li class="title">WLAN Info - <% getCfgLLTD(1, "00",0); %></li>
        <li class="w_text2">
        <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>            
            	<td width="30%"><span class="table_left">Device WLAN 1 MAC</span></td>
                <td><span class="table_right2"><% getCfgLLTD(1, "01",1); %></span></td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device WLAN 1 IP</span></td>
                <td><span class="table_right2"><% getCfgLLTD(1, "07",1); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device WLAN 1 Machine Name</span></td>
                <td><span class="table_right2"><% getCfgLLTD(1, "15",1); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device WLAN 1 SSID</span></td>
                <td><span class="table_right2"><% getCfgLLTD(1, "06",1); %></span</td>
            </tr>
            <tr>            
            	<td width="30%"><span class="table_left">Device LAN 1 Type</span></td>
                <td><span class="table_right2"><% getCfgLLTD(1, "99",1); %></span</td>
            </tr>
            <!--<tr>            
            	<td width="30%"><span class="table_left">Device WLAN Device Num</span></td>
                <td><span class="table_right2"><% getCfgLLTD(1, "00",0); %></span</td>
            </tr>-->
        </table>
        </li>
    </ul>
    </div>
	</li>
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
