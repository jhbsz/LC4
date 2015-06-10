<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var SnoopingEnabled = '<% getCfgZero(1, "snoopingEnabled"); %>';
var LAN1 = '<% getCfgZero(1, "snoopingLAN1"); %>';
var LAN2 = '<% getCfgZero(1, "snoopingLAN2"); %>';
var LAN3 = '<% getCfgZero(1, "snoopingLAN3"); %>';
var LAN4 = '<% getCfgZero(1, "snoopingLAN4"); %>';


function snooping_enabled_click()
{
    if (document.igmpSnooping.snoopingEnbl.checked == true)
	{		
		document.igmpSnooping.snoopingLAN1.disabled = false;
		document.igmpSnooping.snoopingLAN2.disabled = false;
        document.igmpSnooping.snoopingLAN3.disabled = false;
        document.igmpSnooping.snoopingLAN4.disabled = false;
	}
	else
	{
		document.igmpSnooping.snoopingLAN1.disabled = true;
		document.igmpSnooping.snoopingLAN2.disabled = true;
        document.igmpSnooping.snoopingLAN3.disabled = true;
        document.igmpSnooping.snoopingLAN4.disabled = true;
	}
}

function snoopingCheck()
{
	document.igmpSnooping.snoopingEnbl.value = document.igmpSnooping.snoopingEnbl.checked ? "1": "0";
	if (document.igmpSnooping.snoopingEnbl.checked == true)
	{
	  document.igmpSnooping.snoopingLAN1.value = document.igmpSnooping.snoopingLAN1.checked ? "1": "0";
	  document.igmpSnooping.snoopingLAN2.value = document.igmpSnooping.snoopingLAN2.checked ? "1": "0";
	  document.igmpSnooping.snoopingLAN3.value = document.igmpSnooping.snoopingLAN3.checked ? "1": "0";
	  document.igmpSnooping.snoopingLAN4.value = document.igmpSnooping.snoopingLAN4.checked ? "1": "0";
	
	  if((document.igmpSnooping.snoopingLAN1.value==0) && (document.igmpSnooping.snoopingLAN2.value==0)
	     && (document.igmpSnooping.snoopingLAN3.value==0) && (document.igmpSnooping.snoopingLAN4.value==0))
	  {
	     alert('Please select at least one port when snooping enabled!');
             return false;
	  }
	}
	return true;
}

function CheckValue()
{
	if(!snoopingCheck())
	    return false;
	showWebMessage(2, "");
	return true;
}

function updateStatus()
{
  if (SnoopingEnabled == "1")
  {
		document.igmpSnooping.snoopingEnbl.checked = true;
		document.igmpSnooping.snoopingLAN1.disabled = false;
		document.igmpSnooping.snoopingLAN2.disabled = false;
        document.igmpSnooping.snoopingLAN3.disabled = false;
        document.igmpSnooping.snoopingLAN4.disabled = false;
  }
  else
  {
        document.igmpSnooping.snoopingEnbl.checked = false;
        document.igmpSnooping.snoopingLAN1.disabled = true;
		document.igmpSnooping.snoopingLAN2.disabled = true;
        document.igmpSnooping.snoopingLAN3.disabled = true;
        document.igmpSnooping.snoopingLAN4.disabled = true;
  }
        
  if (LAN1 == "1")
		document.igmpSnooping.snoopingLAN1.checked = true;
  else
        document.igmpSnooping.snoopingLAN1.checked = false;
  if (LAN2 == "1")
		document.igmpSnooping.snoopingLAN2.checked = true;
  else
        document.igmpSnooping.snoopingLAN2.checked = false;
  if (LAN3 == "1")
		document.igmpSnooping.snoopingLAN3.checked = true;
  else
        document.igmpSnooping.snoopingLAN3.checked = false;
  if (LAN4 == "1")
		document.igmpSnooping.snoopingLAN4.checked = true;
  else
        document.igmpSnooping.snoopingLAN4.checked = false;
  
}  
function initValue()
{

      var e = document.getElementById("snoopingtitle");
	e.innerHTML = _("snooping title");
	e = document.getElementById("snoopingEnable");
	e.innerHTML = _("snooping Enable");
	e = document.getElementById("snoopingLAN11");
	e.innerHTML = _("snooping LAN1");
	e = document.getElementById("snoopingLAN22");
	e.innerHTML = _("snooping LAN2");
		e = document.getElementById("snoopingLAN33");
	e.innerHTML = _("snooping LAN3");
		e = document.getElementById("snoopingLAN44");
	e.innerHTML = _("snooping LAN4");
	
	

	e = document.getElementById("snoopingApply");
	e.value = _("snooping Apply");
	e = document.getElementById("snoopingReset");
	e.value = _("snooping Reset");


   updateStatus();
}

</script>
</head>

<body onLoad="initValue()">
<form method=post name="igmpSnooping" action="/goform/setIgmpSnooping" onSubmit="return CheckValue()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
                <li class="title" id="snoopingtitle">IGMP Snooping Setup</li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%" nowrap><input type="checkbox" onclick="snooping_enabled_click()" name="snoopingEnbl" value="<% getCfgZero(1, "snoopingEnabled"); %>" />
                      <td id="snoopingEnable">Enable IGMP Snooping</td></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>  

                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%" nowrap>&nbsp;&nbsp;&nbsp;&nbsp; <input type="checkbox" name="snoopingLAN1" value="<% getCfgZero(1, "snoopingLAN1"); %>" />
                      <td id="snoopingLAN11">LAN1</td></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%" nowrap>&nbsp;&nbsp;&nbsp;&nbsp; <input type="checkbox" name="snoopingLAN2" value="<% getCfgZero(1, "snoopingLAN2"); %>" />
                       <td id="snoopingLAN22">LAN2</td></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%" nowrap>&nbsp;&nbsp;&nbsp;&nbsp; <input type="checkbox" name="snoopingLAN3" value="<% getCfgZero(1, "snoopingLAN3"); %>" />
                      <td id="snoopingLAN33">LAN3</td></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="1%" nowrap>&nbsp;&nbsp;&nbsp;&nbsp; <input type="checkbox" name="snoopingLAN4" value="<% getCfgZero(1, "snoopingLAN4"); %>" />
                      <td id="snoopingLAN44">LAN4</td></td>
                      <td>&nbsp;</td>
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
        <input type=submit value="Apply" id="snoopingApply">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        <input type=reset value="Reset" id="snoopingReset" onClick="window.location.reload()">
        <input type=hidden value="/local/advance/igmp_snooping.asp" name="igmp_snooping_url">
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
