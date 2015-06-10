<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var DDNS_Enabled = '<% getCfgZero(1, "DDNSEnabled"); %>';

function ddns_mode_switch()
{
    if (document.DDNS.ddnsenabled.checked == true)
	{		
		document.DDNS.DDNSProvider.disabled = false;
	    document.DDNS.DDNS.disabled = false;
	    document.DDNS.Account.disabled = false;
	    document.DDNS.Password.disabled = false;
	}
	else
	{
		document.DDNS.DDNSProvider.disabled = true;
	    document.DDNS.DDNS.disabled = true;
	    document.DDNS.Account.disabled = true;
	    document.DDNS.Password.disabled = true;
	}
}

function ddnsMode_Click()
{
	document.DDNS.ddnsenabled.value = document.DDNS.ddnsenabled.checked ? "1": "0";
	return true;
}

function CheckValue()
{
   if (document.DDNS.ddnsenabled.checked == true) 
   {
		 if(document.DDNS.DDNS.value == "")
         {
            alert('Host Name can not be empty!');
            return false;
         }
         if(document.DDNS.Account.value == "")
         {
            alert('User Name can not be empty!');
            return false;
         }
         if(document.DDNS.Password.value == "")
         {
            alert('Password can not be empty!');
            return false;
         }
   }
  
   ddnsMode_Click();
   showWebMessage(2, "");
   return true;
}
function updateStatus()
{
   var ddnsProvider = "<% getCfgGeneral(1, "DDNSProvider"); %>";
   
   if (ddnsProvider == "dyndns.org")
   {
       document.DDNS.DDNSProvider.options.selectedIndex = 0;
   }
   else if (ddnsProvider == "no-ip.com")
   {
       document.DDNS.DDNSProvider.options.selectedIndex = 1;
   }
   else if (ddnsProvider == "eurodyndns")
   {
       document.DDNS.DDNSProvider.options.selectedIndex = 2;
   }
   else if (ddnsProvider == "regfish")
   {
       document.DDNS.DDNSProvider.options.selectedIndex = 3;
   }

   
   if (DDNS_Enabled == "0")
   {
       document.DDNS.ddnsenabled.checked = false;
       document.DDNS.DDNSProvider.disabled = true;
	   document.DDNS.DDNS.disabled = true;
	   document.DDNS.Account.disabled = true;
	   document.DDNS.Password.disabled = true;
   }
   else
   {
       document.DDNS.ddnsenabled.checked = true;
       document.DDNS.DDNSProvider.disabled = false;
	   document.DDNS.DDNS.disabled = false;
	   document.DDNS.Account.disabled = false;
	   document.DDNS.Password.disabled = false;
   }
}  

function initTranslation(){
	var e = document.getElementById("Dynamic_DNS_Setup");
	e.innerHTML = _("Dynamic_DNS_Setup");
	e = document.getElementById("Enable_Dynamic_DNS");
	e.innerHTML = _("Enable_Dynamic_DNS");
	e = document.getElementById("Service_Provider");
	e.innerHTML = _("Service_Provider");
	e = document.getElementById("Host_Name");
	e.innerHTML = _("Host_Name");
	e = document.getElementById("User_Name");
	e.innerHTML = _("wPppoe User");
	e = document.getElementById("Password");
	e.innerHTML = _("wPppoe Password");
	e = document.getElementById("ddnsApply");
	e.value = _("ipAlias Apply");
	e = document.getElementById("ddnsReset");
	e.value = _("ipAlias Reset");
}

function initValue()
{
	 initTranslation();
   updateStatus();
}
</script>
</head>

<body onLoad="initValue()">
<form method="post" name="DDNS" action="/goform/DDNS" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="Dynamic_DNS_Setup">Dynamic DNS Setup</td>
                   </tr></table>
                </li>
                <li class="w_text">
                  <table id=ddnsEnabled width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input type="checkbox" name="ddnsenabled" value="<% getCfgZero(1, "DDNSEnabled"); %>" onclick="ddns_mode_switch()" />
<font id=Enable_Dynamic_DNS>Enable Dynamic DNS </font></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=Service_Provider>Service Provider  :</td>
                      <td><select name="DDNSProvider" size="1">
                        <option value="dyndns.org">WWW.DynDNS.ORG </option>
                        <option value="no-ip.com">WWW.NO-IP.COM </option>
                        <option value="eurodyndns">WWW.EuroDNS.COM </option>
                        <option value="regfish">WWW.REGFISH.COM </option>
                      </select></td>
                    </tr>
                  </table>
                </li>
                <!--   
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">Dynamic DNS Type  :</td>
                      <td><select name="sysDDNSType" size="1" onclick="DDNS_TypeClick(this.form);">
                        <option value="00000000" selected="selected">Dynamic DNS </option>
                        <option value="00000001">Static DNS </option>
                        <option value="00000002">Custom DNS </option>
                      </select></td>
                    </tr>
                  </table>
                </li>
                -->
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=Host_Name>Host Name  :</td>
                      <td><input size="32" name="DDNS" value="<% getCfgGeneral(1, "DDNS"); %>" type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=User_Name>User Name  :</td>
                      <td><input size="16" name="Account" value="<% getCfgGeneral(1, "DDNSAccount"); %>" type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id=Password>Password  :</td>
                      <td><input size="16" name="Password" value="<% getCfgGeneral(1, "DDNSPassword"); %>" type="password" /></td>
                    </tr>
                  </table>
                </li>
                <!--
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">Token   :</td>
                      <td><input disabled="disabled" name="sysDNSToken" size="30" maxlength="64" value="" type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input name="sysDNSWildCard" type="checkbox" />
Enable Wildcard Option </td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input disabled="disabled" name="sysDDNSOffLine" type="checkbox" />
Enable off line option (Only applies to custom DNS) </td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="space3"></li>
                <li class="title">IP Address Update Policy</li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input name="DDNS_IP_ADDR_POLICY" value="0" checked="checked" onclick="do_Checked(this.form);" type="radio" />
Use WAN IP Address </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input name="DDNS_IP_ADDR_POLICY" value="1" onclick="do_Checked(this.form);" type="radio" />
Dynamic DNS server auto detect IP Address </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input name="DDNS_IP_ADDR_POLICY" value="2" onclick="do_Checked(this.form);" type="radio" />
Use specified IP Address
  <input disabled="disabled" name="sysDDNSIPAddr" size="15" maxlength="15" value="0.0.0.0" onblur="checkIPRange(this)" type="text" /></td>
                    </tr>
                  </table>
                </li>
                -->
                <li></li>
				<li></li>
              </ul>
            </div>
        </li>
        <center>
        <li class="table_button">
        <input type=submit value="Apply" id="ddnsApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="ddnsReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/ddns.asp" name="ddns_url">
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
