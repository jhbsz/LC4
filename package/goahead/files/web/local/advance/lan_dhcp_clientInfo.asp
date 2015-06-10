<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/tablet.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

function initValue()
{
         var e = document.getElementById("Dhcpinfotitle");
	e.innerHTML = _("Dhcpinfo title");
	e = document.getElementById("DhcpinfoTable");
	e.innerHTML = _("Dhcpinfo Table");
	e = document.getElementById("DhcpinfoTableMAC");
	e.innerHTML = _("Dhcpinfo TableMAC");
	e = document.getElementById("DhcpinfoTableIP");
	e.innerHTML = _("Dhcpinfo TableIP");
	e = document.getElementById("DhcpinfoTableExp");
	e.innerHTML = _("Dhcpinfo TableExp");
	

	e = document.getElementById("dhcpClientRefresh");
	e.value = _("dhcpClient Refresh");
   
}

</script>
</head>

<body onLoad="initValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
				<li class="title" id="Dhcpinfotitle">DHCP Client Table</li>
				<li></li>
				<li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" cellpadding="0" cellspacing="0" >
                          <tr>
                            <td height="26" colspan="7"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                  <td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
                                  <td background="images/table_top_center.gif"class="table_title" id="DhcpinfoTable"> Table List </td>
                                  <td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
                                </tr>
                            </table></td>
                          </tr>
                          <tr >
                            <td width="5%"><center>
                              <span class="top_left">#</span>
                            </center>                            </td>
                            <td width="36%"><center>
                              <span class="top_font" id="DhcpinfoTableMAC">MAC Address</span>
                            </center>                            </td>
                            <td width="36%"><center>
                              <span class="top_font" id="DhcpinfoTableIP">IP Address </span>
                            </center>                            </td>
                            <td width="23%"><center>
                              <span class="top_right" id="DhcpinfoTableExp">Expires in</span>
                            </center>                            </td>
                          </tr>
                          <% getDhcpCliList(); %>
                      </table></td>
                    </tr>
                          <tr>
                            <td height="5" colspan="7"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
                                <tr>
                                  <td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
                                  <td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
                                  <td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
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
        <form method="post" name ="dhcpRefreshTbl" action="/goform/reflashClientTbl">
        <input type=submit value="Refresh" id="dhcpClientRefresh">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=hidden value="/local/advance/lan_dhcp_clientInfo.asp" name="lan_dhcp_clientInfo_url">
        </form>
        </li>
        </center>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>  
</html>
