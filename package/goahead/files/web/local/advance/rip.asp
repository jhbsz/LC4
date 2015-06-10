<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

function CheckValue()
{	
	showWebMessage(2, "");
	return true;
}

function initValue()
{
   var rip_version = "<% getCfgGeneral(1, "RIPversion"); %>";

var e = document.getElementById("riptitle");
	e.innerHTML = _("rip title");
	e = document.getElementById("riprip");
	e.innerHTML = _("rip rip");
	e = document.getElementById("ripNone");
	e.innerHTML = _("rip None");
	e = document.getElementById("ripRIPv1");
	e.innerHTML = _("rip RIPv1");
	e = document.getElementById("ripRIPv2");
	e.innerHTML = _("rip RIPv2");
	e = document.getElementById("ripApply");
	e.value = _("rip Apply");
	e = document.getElementById("ripReset");
	e.value = _("rip Reset");



   if (rip_version == "0")
{
       document.rip.RIPSelect.options.selectedIndex = 0;
}  
   else if (rip_version == "1")
   {
       document.rip.RIPSelect.options.selectedIndex = 1;
   }
   else if (rip_version == "2")
{
       document.rip.RIPSelect.options.selectedIndex = 2;
   }
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="rip" action="/goform/ripRouting" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="riptitle">RIP Setup</li>
                <!--
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input type="checkbox" name="RIPSelect" value="<% getCfgZero(1, "RIPEnable"); %>" />
                      Enable Routing Information Protocol  </td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>              
                -->
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%" id="riprip" >RIP : </td>
                      <td><span class="cfig_word">
                        <select name="RIPSelect" size="1">
                          <option value="0" id="ripNone">None </option>
                          <option value="1" id="ripRIPv1">RIPv1 </option>
                          <option value="2" id="ripRIPv2">RIPv2 </option>
                        </select>
                      </span></td>
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
        <input type=submit value="Apply" id="ripApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="ripReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/rip.asp" name="rip_url">
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
