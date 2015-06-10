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

var UPnPEnabled = '<% getCfgZero(1, "upnpEnabled"); %>';

function upnpClick()
{
	document.lanCfg.upnpEnbl.value = document.lanCfg.upnpEnbl.checked ? "1": "0";
	
	return true;
}

function CheckValue()
{
	upnpClick();
	showWebMessage(2, "");
	return true;
}

function updateStatus()
{
  if (UPnPEnabled == "1")
		document.lanCfg.upnpEnbl.checked = true;
  else
        document.lanCfg.upnpEnbl.checked = false;
}  
function initValue()
{
   var e = document.getElementById("upnptitle");
	e.innerHTML = _("upnp title");
	e = document.getElementById("upnpDeviceName");
	e.innerHTML = _("upnp DeviceName");
	e = document.getElementById("upnpEnb");
	e.innerHTML = _("upnp Enb");
	
	e = document.getElementById("upnpApply");
	e.value = _("upnp Apply");
	e = document.getElementById("upnpReset");
	e.value = _("upnp Reset");
   updateStatus();
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="lanCfg" action="/goform/setLanUPnP" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="upnptitle">UPnP Setup</li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="upnpDeviceName">Device Name:   ZyXEL NBG4615 Internet Sharing Gateway </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%" nowrap ><input  type="checkbox" name="upnpEnbl" value="<% getCfgZero(1, "upnpEnabled"); %>" />
                      <td id="upnpEnb">Enable the Universal Plug and Play (UPnP) Feature</td>
                      </td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>  
                <!--
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" nowrap><input name="sysDNSActive2" type="checkbox" />
                        Allow users to make port forwarding changes through UPnP</td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" nowrap><input name="sysDNSActive3" type="checkbox" />
                        Allow UPnP to pass through Firewall</td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li></li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><span class="i_note">Note:</span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><span class="i_note_a">1. For UPnP to function normally, the <a href="remote.html" class="note">HTTP</a> service must be available for LAN computers using UPnP.</span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><span class="i_note_a">2. For <a href="wps.html" class="note">WPS</a> to function normally, the UPnP service must be available.</span></td>
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
        <input type=submit value="Apply" id="upnpApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="upnpReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/upnp.asp" name="upnp_url">
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
