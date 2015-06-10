<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");
function logsettings()
{
var selected_num=0

if(document.LOG.System_Maintenance.checked)
 {
  document.LOG.System_Maintenance.value = "1";
  selected_num++;
 } 
 if(document.LOG.dns.checked)
 {
  document.LOG.dns.value = "1";
  selected_num++;
 } 

 if(document.LOG.PPP.checked)
 {
  document.LOG.PPP.value = "1";
  selected_num++;
 } 
 if(document.LOG.UPnP.checked)
 {
  document.LOG.UPnP.value = "1";
  selected_num++;
 } 
// if(document.LOG.WLAN.checked)
// {
//  document.LOG.WLAN.value = "1";
//  selected_num++;
// } 
 if(document.LOG.NTPClient.checked)
 {
  document.LOG.NTPClient.value = "1";
  selected_num++;
 }
 if(document.LOG.SYSwarning.checked)
 {
  document.LOG.SYSwarning.value = "1";
  selected_num++;
 }
 if(document.LOG.DhcpServer.checked)
 {
  document.LOG.DhcpServer.value = "1";
  selected_num++;
 } 
 if(document.LOG.DhcpClient.checked)
 {
  document.LOG.DhcpClient.value = "1";
  selected_num++;
 } 
 if(document.LOG.DDNS.checked)
 {
  document.LOG.DDNS.value = "1";
  selected_num++;
 }
 
// if(document.LOG.Firewall.checked)
// {
//  document.LOG.Firewall.value = "1";
//  selected_num++;
// } 
 
  var e = document.getElementById("loginfo");
  e.value = selected_num;
}

function initTranslation(){
	var e = document.getElementById("activeLog");
	e.innerHTML = _("active log");
	e = document.getElementById("log_web");
	e.innerHTML = _("logsystem logWebmgt");
	e = document.getElementById("log_dns");
	e.innerHTML = _("logsystem logDNS");
  e = document.getElementById("log_ppp");
  e.innerHTML = _("logsystem logPPP");
  e = document.getElementById("log_upnp");
  e.innerHTML = _("logsystem logUPnP");
  //e = document.getElementById("log_wireless");
  //e.innerHTML = _("logsystem logWireless");
  e = document.getElementById("log_ntpClient");
  e.innerHTML = _("logsystem logNTPClient");
  e = document.getElementById("log_sysWarn");
  e.innerHTML = _("logsystem logSysWarn");
  e = document.getElementById("log_dhcpServer");
  e.innerHTML = _("logsystem logDhcpSrv");
  e = document.getElementById("log_dhcpClient");
  e.innerHTML = _("logsystem logDhcpCli");
  e = document.getElementById("log_ddns");
  e.innerHTML = _("logsystem logDDNS");
  //e = document.getElementById("log_firewall");
  //e.innerHTML = _("logsystem logFirewall");
}

function initvalue()
{
initTranslation();
SystemMaintenance="<% getCfgGeneral(1, "log_SystemMaintenance"); %>";
dns="<% getCfgGeneral(1, "log_dns"); %>";
PPP="<% getCfgGeneral(1, "log_PPP"); %>";
UPnP="<% getCfgGeneral(1, "log_upnp"); %>";
WLAN="<% getCfgGeneral(1, "log_wlan"); %>";
NTPClient="<% getCfgGeneral(1, "log_ntpclient"); %>";
SYSwarning="<% getCfgGeneral(1, "log_syswarning"); %>";
dhcpServer="<% getCfgGeneral(1, "log_dhcpServer"); %>";
dhcpClient="<% getCfgGeneral(1, "log_dhcpClient"); %>";
ddns="<% getCfgGeneral(1, "log_ddns"); %>";
Firewall="<% getCfgGeneral(1, "log_Firewall"); %>";

if(SystemMaintenance=="1")
{
 document.LOG.System_Maintenance.checked = "true";
}
if(dns=="1")
{
 document.LOG.dns.checked = "true";
}
if(PPP=="1")
{
 document.LOG.PPP.checked = "true";
}
if(UPnP=="1")
{
 document.LOG.UPnP.checked = "true";
}
//if(WLAN=="1")
//{
// document.LOG.WLAN.checked = "true";
//}
if(NTPClient=="1")
{
 document.LOG.NTPClient.checked = "true";
}
if(SYSwarning=="1")
{
 document.LOG.SYSwarning.checked = "true";
}
if(dhcpServer=="1")
{
 document.LOG.DhcpServer.checked = "true";
}
if(dhcpClient=="1")
{
 document.LOG.DhcpClient.checked = "true";
}
if(ddns=="1")
{
 document.LOG.DDNS.checked = "true";
}
//if(Firewall=="1")
//{
// document.LOG.Firewall.checked = "true";
//}
}

function proceeding()
{
showWebMessage(2, "");
}
</script>
</head>
<body onLoad="initvalue()">
<form method="post" name="LOG" action="/goform/logsetting" >
    <div id="table">
      <ul>
           <li class="table_content">
            <div class="data">
              <ul>
                <li class="w_text2">
                      <table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<li id="activeLog" class="title"></li>
</tr>
<tr>
                         <td colspan="2">&nbsp;</td>
</tr>
</table>
<table  width="90%" border="0" align=center cellspacing="0">
<tr>
    <td valign=top class="data">
    <input type="checkbox" name="System_Maintenance" value=""/><font id="log_web">Web Management</font><br />
    <input type="checkbox" name="dns" value=""; /><font id="log_dns">DNS</font><br />
    <input type="checkbox" name="PPP" value=""; /><font id="log_ppp">PPP</font><br />
    <input type="checkbox" name="UPnP" value=""; /><font id="log_upnp">UPnP</font><br />
    <!-- <input type="checkbox" name="WLAN" value=""; /><font id="log_wireless">Wireless</font><br /> -->
    <input type="checkbox" name="NTPClient" value=""; /><font id="log_ntpClient">NTPClient</font><br />
    <input type="checkbox" name="SYSwarning" value=""; /><font id="log_sysWarn">System Warning</font><br />
    <input type="checkbox" name="DhcpServer" value=""; /><font id="log_dhcpServer">DHCP Server</font><br />
    <input type="checkbox" name="DhcpClient" value=""; /><font id="log_dhcpClient">DHCP Client</font><br />
    <input type="checkbox" name="DDNS" value=""; /><font id="log_ddns">DDNS</font><br />
    <!-- <input type="checkbox" name="Firewall" value=""; /><font id="log_firewall">Firewall</font><br /> -->
    </td>
</tr>
</table>
                </li>
                <li></li>
				<li></li>
              </ul>
            </div>

        <li class="table_button">
          <center>
          <input type="hidden" name="select_num" id="loginfo" value="">
          <input id="apply" type="submit"  value="Apply"  name="applylog" onClick="logsettings()"> &nbsp; &nbsp; &nbsp;
          <input id="refresh" type="button"   value="Refresh" name="refreshset" onClick="window.location.reload()">
          </center>
        </li>
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