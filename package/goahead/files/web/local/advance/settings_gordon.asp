<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

function initTranslation()
{
	var e = document.getElementById("title");
	e.innerHTML = _("syssettings title");
	e = document.getElementById("introduction");
	e.innerHTML = _("syssettings introduction");

	e = document.getElementById("title_1");
	e.innerHTML = _("syssettings title_1");
	e = document.getElementById("introduction_1");
	e.innerHTML = _("syssettings introduction_1");
	e = document.getElementById("path");
	e.innerHTML = _("syssettings path");

	e = document.getElementById("upload");
	e.value = _("syssettings upload");
	e = document.getElementById("title_2");
	e.innerHTML = _("syssettings title_2");
	e = document.getElementById("introduction_2");
	e.innerHTML = _("syssettings introduction_2");
	e = document.getElementById("introduction_3");
	e.innerHTML = _("syssettings introduction_3");
	e = document.getElementById("introduction_4");
	e.innerHTML = _("syssettings introduction_4");
	e = document.getElementById("introduction_5");
	e.innerHTML = _("syssettings introduction_5");
	e = document.getElementById("reset");
	e.value = _("syssettings Reset");
		e = document.getElementById("backup");
	e.value = _("syssettings Backup");
}

function AdmFormCheck()
{
	if (document.ImportSettings.filename.value == "") {
		alert("Please specify the file name.");
		return false;
	}
	if (!confirm(_("Do you wish to continue with this process ?")))
		return false;
	return true;
}

function LoadDefaultFormCheck()
{
	if (!confirm(_("Do you wish to continue with this process ?")))
		return false;
	return true;
}

</script>
</head>
<body onLoad="initTranslation()">
    <div id="table">
      <ul>
           <li class="table_content">
            <div class="data">
              <ul>
                <li id="title" class="title">Backup Configuration </li>
                <li class="w_text">
                  <form method="post" name="ExportSettings" action="/cgi-bin/ExportSettings.sh">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction" width="50%">Click Backup to save the current configuration of your system to your computer.</td>
                      <td><input id="backup" name="Export" value="Backup" " type="submit" ></td>
                    </tr>
                  </table>
                  </form>
                </li>
                <li></li>
                <li class="space3"></li>
                <li id="title_1" class="title">Restore Configuration </li>
                <li class="w_text">
                  <form method="post" name="ImportSettings" action="/cgi-bin/upload_settings.cgi" enctype="multipart/form-data">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction_1">To restore a previously saved configuration file to your system, browse to 
                          the location of the configuration file and click Upload. </td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="path" width="50%">File Path :                      </td>
                      <td width="7%"><input  name="filename" size="20" maxlength="128" type="file" ></td>
                      <td><input  id="upload" value="Upload" type="submit"  onClick="return AdmFormCheck()"></td>
                    </tr>
                  </table>
                  </form>
                </li>
                <li class="space3"></li>
                <li id="title_2" class="title">Back to Factory Defaults </li>
                <li class="w_text">
                  <form method="post" name="LoadDefaultSettings" action="/goform/LoadDefaultSettings">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction_2"><span style="font-weight: 400;">Click <b>Reset</b> to clear all user-entered configuration information and return to factory defaults. After resetting, the </span></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="introduction_3" width="50%"><span style="font-weight: 400;">- Password will be 1234 </span></td></tr>
                      <tr><td id="introduction_4"><span  style="font-weight: 400;">- LAN IP address will be 192.168.1.1 </span></td></tr>
                      <tr><td id="introduction_5"><span  style="font-weight: 400;">- DHCP will be reset to server </span></td>
                      <td><input name="LoadDefault" value="Reset"  id="reset" type="submit" onClick="return LoadDefaultFormCheck()" ></td>
                    </tr>
                  </table>
                  </form>
                </li>
                <li></li>
				<li></li>
              </ul>
            </div>
        <li class="table_button">
        </li>
        </li>
        </ul>
    </div>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
if (parent.upload_message=="Ready")
	showWebMessage(0, parent.upload_message);
else
	showWebMessage(1, parent.upload_message);
parent.upload_message="Ready";
<!--showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));-->
</script> 
</html>
