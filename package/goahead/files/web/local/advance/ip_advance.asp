<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
    <link href="images/table.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title"> Multicast Setup </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%">Multicast : </td>
                      <td><span class="cfig_word">
                        <select name="WAN_Multicast" size="1">
                          <option value="00000000" selected="selected">None </option>
                          <option value="00000001">IGMP-v1 </option>
                          <option value="00000002">IGMP-v2 </option>
                        </select>
                      </span></td>
                    </tr>
                  </table>
                </li>
                <!--<li class="title">Windows Networking (NetBIOS over TCP/IP) </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><input name="WAN_IP_WanToLan" type="checkbox" />
                        Allow between LAN and WAN </td>
                    </tr>
                  </table>
                </li>-->
                <li></li>
				<li></li>
              </ul>
            </div>
        </li>
        <center>
        <li class="table_button">
        <input type=submit value=Apply>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value=Reset>
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
