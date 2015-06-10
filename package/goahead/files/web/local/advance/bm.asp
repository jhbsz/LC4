<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"  />
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
function doAutoCheck()
{
	if(document.forms[0].BW_Cfg_Active.checked){
//		document.forms[0].BW_Auto_Active.disabled=false;
//		document.forms[0].BW_Auto_Active.checked=true;
		return;
	}else{
//		document.forms[0].BW_Auto_Active.disabled=true;
//		document.forms[0].BW_Auto_Active.checked=false;
		return;
	}
}
function initTranslation()
{
	var e;
	var i;

	e = document.getElementById("bmTitle");
	e.innerHTML = _("bm title");
	e = document.getElementById("bmEnableQos");
	e.innerHTML = _("bm enable qos");
//	e = document.getElementById("bmEnableAuto");
//	e.innerHTML = _("bm enable auto");
	e = document.getElementById("bmApply");
	e.value = _("bm apply");
	e = document.getElementById("bmReset");
	e.value = _("bm reset");

}

function check_confirm()
{
		showWebMessage(2, "");

		return true;
}

</script>
</head>

<body onload="initTranslation();">
<form method="post" name="QoSGeneral" action="/goform/QoSGeneral">
<input type="hidden" name="g_qos_hidden" value="1">
	<div id="table">
  	<ul>
    	<li class="table_content">
      	<div class="data">
        	<ul>
        		<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="bmTitle">Service Management</td>
                   </tr></table>
        		</li>
						<li class="w_text">
            	<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
              	<tr>
                	<td><input onclick="doAutoCheck()" type="checkbox" name="BW_Cfg_Active" value=1 <% QoSGetInfo("qos_Enable"); %> >
<font id="bmEnableQos">Enable Bandwidth Management</font></td>
                </tr>
              </table>
            </li>
            <!--2009/2/9 marked by RL
						<li class="w_text">
            	<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0" >
              	<tr>
                	<td><input type="checkbox" name="BW_Auto_Active" value=1 <% QoSGetInfo("qos_auto_Enable"); %> >
<font id="bmEnableAuto">Enable Automatic Traffic Classifier</font></td>
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
      <input type=submit id="bmApply" value=Apply onclick="return check_confirm();">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
      <input type=button id="bmReset" value=Reset>
      </li>
      </center>
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
