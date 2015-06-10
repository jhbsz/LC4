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

function initTranslation()
{
	var e = document.getElementById("title");
	e.innerHTML = _("reset title");

	e = document.getElementById("introduction");
	e.innerHTML = _("reset introduction");
	e = document.getElementById("restart");
  e.value = _("reset Restart");

}

function onSubmitCheck()
{
	if (!confirm(_("Do you really want to restart the device ?")))
		return false;
	return true;
}	
</script>
</head>
<body onLoad="initTranslation()">
<form method="post" name="systemrebooting" action="/goform/systemrebooting">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li id="title" class="title">System Restart</li>
                <li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction" height="40">Click <a class="blod">Restart</a> to have the device perform a software restart. The SYS(or PWR) LED blinks as the device restarts and then stays steady on if the restart is successful. Wait a minute before logging into the device again. </td>
                    </tr>
                  </table>
                </li>
                <li></li>
				<li></li>
              </ul>
            </div>
        <center>
        <li class="table_button">
        <input value="Restart" id="restart"  name="restartsystem" type="submit" onClick="return onSubmitCheck();">
        </li>
        </center>       
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
