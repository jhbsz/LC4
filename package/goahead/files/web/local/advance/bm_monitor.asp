<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html>
<head>
<title> </title>
<meta http-equiv='content-type' content='text/html;charset=iso-8859-1'>
<META HTTP-EQUIV="Pragma" CONTENT="no-cache">
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

function initTranslation()
{
	var e;
	var i;

	e = document.getElementById("moniTitle");
	e.innerHTML = _("moni title");

}

 var interval = 15 ; //sec
 setInterval("window.location.reload();", interval * 1000);

</script>
<style type="text/css">
<!--
.wtext_3 {font-family:Arial, Helvetica, sans-serif;color:#000000;font-size:11px;}
.cfg_title{ font-family:Arial, Helvetica, sans-serif; color:#385f95; font-size:11px; font-weight:bold; height:25px; line-height:25px;}
-->

</style>


</head>
<body onload="initTranslation();">
<div id="table">
	<ul>
    	<li class="table_content">
      		<div class="data">
        		<ul>
			   		<li class="title" id="moniTitle">Monitor</li>
					<li>&nbsp;</li>
				</ul>
    		</div>

<% QoSDispMonitor(); %>


	<table border="0" cellspacing="0" cellpadding="0" width="300" align=center>
		<tr>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td>&nbsp;</td>
		</tr>
	</table>
   </li>
   <li class="table_button"></li>
</ul>
</div>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>
</html>
