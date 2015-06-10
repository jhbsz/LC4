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

var SystemMaintenance="<% getCfgGeneral(1, "log_SystemMaintenance"); %>";
var dns="<% getCfgGeneral(1, "log_dns"); %>";
var PPP="<% getCfgGeneral(1, "log_PPP"); %>";
var UPnP="<% getCfgGeneral(1, "log_upnp"); %>";
var WLAN="<% getCfgGeneral(1, "log_wlan"); %>";
var NTPClient="<% getCfgGeneral(1, "log_ntpclient"); %>";
var SYSwarning="<% getCfgGeneral(1, "log_syswarning"); %>";
var dhcpServer="<% getCfgGeneral(1, "log_dhcpServer"); %>";
var dhcpClient="<% getCfgGeneral(1, "log_dhcpClient"); %>";
var ddns="<% getCfgGeneral(1, "log_ddns"); %>";
var Firewall="<% getCfgGeneral(1, "log_Firewall"); %>";
                        
function refreshlogclick()
{
	self.location.reload();
}
function initTranslation()
{
	var config_log_index= "<% getCfgGeneral(1, "log_index"); %>";
	var select_num="<% getCfgGeneral(1, "log_selected_num"); %>";
	var LOG_index=0;
	var LOG_index_num=0;
	if(select_num>0)
	{
	for(j=1; j<=select_num; j++)
	{
	  LOG_index= document.showlog.ViewLog[j].value;
	  if(LOG_index==config_log_index)
	  {
	    LOG_index_num=j;
	  }
	}
	document.showlog.ViewLog.options[LOG_index_num].selected = true;
	}
	else
	{
	 document.showlog.ViewLog.options[0].selected = true;
	}
	var e = document.getElementById("title");
	e.innerHTML = _("logsystem title");
  e = document.getElementById("table_title");
  e.innerHTML = _("logsystem table_title");
  //e = document.getElementById("syslog_time");
  //e.innerHTML = _("logsystem time");
  e = document.getElementById("syslog_msg");
  e.innerHTML = _("logsystem msg");
  
  e = document.getElementById("display");
  e.innerHTML = _("logsystem display");
  e = document.getElementById("log_all");
  e.innerHTML = _("logsystem logAll");

	if(SystemMaintenance=="1"){
  e = document.getElementById("log_webmgt");
  e.innerHTML = _("logsystem logWebmgt");
  	}
  		
	if(dns=="1"){
  e = document.getElementById("log_dns");
  e.innerHTML = _("logsystem logDNS");
	}
	
	if(PPP=="1"){
  e = document.getElementById("log_ppp");
  e.innerHTML = _("logsystem logPPP");
	}
	
	if(UPnP=="1"){
  e = document.getElementById("log_upnp");
  e.innerHTML = _("logsystem logUPnP");
	}

	if(WLAN=="1"){
  e = document.getElementById("log_wireless");
  e.innerHTML = _("logsystem logWireless");
	}

	if(NTPClient=="1"){
  e = document.getElementById("log_ntpClient");
  e.innerHTML = _("logsystem logNTPClient");
	}
	
	if(SYSwarning=="1"){
  e = document.getElementById("log_sysWarn");
  e.innerHTML = _("logsystem logSysWarn");
	}

	if(dhcpServer=="1"){
  e = document.getElementById("log_dhcpServer");
  e.innerHTML = _("logsystem logDhcpSrv");
	}

	if(dhcpClient=="1"){
  e = document.getElementById("log_dhcpClient");
  e.innerHTML = _("logsystem logDhcpCli");
	}

	if(ddns=="1"){
  e = document.getElementById("log_ddns");
  e.innerHTML = _("logsystem logDDNS");
	}

	if(Firewall=="1"){
  e = document.getElementById("log_firewall");
  e.innerHTML = _("logsystem logFirewall");
	}

  e = document.getElementById("refresh");
  e.value = _("logsystem refresh");
  e = document.getElementById("clear");
  e.value = _("logsystem clear");
}
function send_to_cgi()
{
  var e = document.getElementById("loginfo");
  e.value = document.showlog.ViewLog.value;
  document.forms[0].submit();
  //self.reload();
}
function proceeding()
{
showWebMessage(2, "");
}
</script>
</head>
<body onLoad="initTranslation()">
    <div id="table">
      <ul>
           <li class="table_content">
            <div class="data">
              <ul>
                <li id="title" class="title"></li>
                <li class="w_text2">
               <form method="post" name="showlog"action="/goform/showlog">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                  <tr><td>
                       <script>                      
                        document.write("<font id='display'>Display:&nbsp;&nbsp;&nbsp;</font>");
                        document.write("<select name='ViewLog' size='1' onchange='send_to_cgi();'>");
                        document.write("<option value='show_all' id='log_all'>all log</option>");
                        if(SystemMaintenance=="1")
                        document.write("<option value='System_Maintenance' id='log_webmgt'>Web Management</option>");
                        if(dns=="1")
                        document.write("<option value='dns' id='log_dns'>DNS</option>");
                        if(PPP=="1")
                        document.write("<option value='PPP' id='log_ppp'>PPP</option>");
                        if(UPnP=="1")
                        document.write("<option value='UPnP' id='log_upnp'>UPnP</option>");
                        if(WLAN=="1")
                        document.write("<option value='WLAN' id='log_wireless'>Wireless</option>");
                        if(NTPClient=="1")
                        document.write("<option value='NTPClient' id='log_ntpClient'>NTPClient</option>");
                        if(SYSwarning=="1")
                        document.write("<option value='SYSwarning' id='log_sysWarn'>System Warning</option>");
                        if(dhcpServer=="1")
                        document.write("<option value='dhcpServer' id='log_dhcpServer'>DHCP Server</option>");
                        if(dhcpClient=="1")
                        document.write("<option value='dhcpClient' id='log_dhcpClient'>DHCP Client</option>");
                        if(ddns=="1")
                        document.write("<option value='ddns' id='log_ddns'>DDNS</option>");
                        if(Firewall=="1")
                        document.write("<option value='Firewall' id='log_firewall'>Firewall</option>");
                    </script>
                    <input type="hidden" name="log_index" id="loginfo" value="">
                    <!--<input id="chang_log" type="submit" value="apply" onclick="return send_to_cgi();">-->
                    </td></tr>
                    <tr><td>&nbsp;</td></tr>
                    <tr>
                      <td><table width="100%" cellpadding="0" cellspacing="0" >
                          <tr>
                            <td height="26" colspan="7">
                            <table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                  <td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
                                  <td id="table_title" background="images/table_top_center.gif"class="table_title"></td>
                                  <td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
                                </tr>
                            </table></td>
                          </tr>
                          <tr >
                            <td width="5%"><center>
                                <span class="top_left"><class="sort">#</span>
                            </center></td>
                            <td width="95%"><center>
                                <span id="syslog_msg" class="top_font"></span>
                            </center></td>
                            <td width="0%"><center>
                                <span class="top_right"><class="sort"></span>
                            </center></td>
                          </tr>
                   <% loglist(); %>
                          <tr>
                            <td height="5" colspan="7"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
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
                  </form>
                </li>
                <li></li>
				<li></li>
              </ul>
            </div>
<form method="post" action="/goform/clearlog" enctype="multipart/form-data">
        <li class="table_button">
          <center>
          <input id="refresh" type="button"  value="Refresh"  name="refreshlog" onclick="refreshlogclick();"> &nbsp; &nbsp; &nbsp;
          <input id="clear" type="submit"   value="Clear" name="clearlog" onclick="proceeding();">
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
