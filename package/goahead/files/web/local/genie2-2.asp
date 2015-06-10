<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head>
<link href="images/css.css" rel="stylesheet" type="text/css" />
<link href="advance/images/table.css" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" /></head>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("main");
function atoi(str, num)
{
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}

	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function checkRange(str, num, min, max)
{
	d = atoi(str, num);
	if (d > max || d < min)
		return false;
	return true;
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function checkIpAddr(field, ismask)
{
	if (field.value == "") {
		alert(_("Error. IP address is empty."));
		field.value = field.defaultValue;
		field.focus();
		return false;
	}

	if (isAllNum(field.value) == 0) {
		alert(_('It should be a [0-9] number.'));
		field.value = field.defaultValue;
		field.focus();
		return false;
	}
    /*
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 256)) ||
				(!checkRange(field.value, 2, 0, 256)) ||
				(!checkRange(field.value, 3, 0, 256)) ||
				(!checkRange(field.value, 4, 0, 256)))
		{
			alert(_('IP adress format error.'));
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	*/
	//aron modify to limit max to 255
	if (ismask) {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 0, 255)))
		{
			alert(_('IP adress format error.'));
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	else {
		if ((!checkRange(field.value, 1, 0, 255)) ||
				(!checkRange(field.value, 2, 0, 255)) ||
				(!checkRange(field.value, 3, 0, 255)) ||
				(!checkRange(field.value, 4, 1, 254)))
		{
			alert(_('IP adress format error.'));
			field.value = field.defaultValue;
			field.focus();
			return false;
		}
	}
	return true;
}

function CheckValue()
{
  if (!checkIpAddr(document.wanCfg.staticIp, false))
			return false;
	if (!checkIpAddr(document.wanCfg.staticNetmask, true))
			return false;
	if (document.wanCfg.staticGateway.value != "")
	{
			if (!checkIpAddr(document.wanCfg.staticGateway, false))
				  return false;
  }

  if(document.wanCfg.staticPriDns.value == "")
  {
      alert(_('First DNS Server should not be empty with User-Defined.'));
      return false;
  }
  if(document.wanCfg.staticPriDns.value == "0.0.0.0")
  {
      alert(_('First DNS Server should not be 0.0.0.0, please give DNS IP Address.'));
      return false;
  }
  if (!checkIpAddr(document.wanCfg.staticPriDns, false))
			return false;

  if(document.wanCfg.staticSecDns.value == "")
  {
//      alert('Second DNS Server should not be empty with User-Defined.');
//      return false;
			  return true;
  }
  if(document.wanCfg.staticSecDns.value == "0.0.0.0")
  {
      alert(_('Second DNS Server should not be 0.0.0.0, please give DNS IP Address.'));
      return false;
  }
  if (!checkIpAddr(document.wanCfg.staticSecDns, false))
			return false;

  return true;
}

function onChangeType(item)
{
	if (item.selectedIndex==0)
		self.location.href="genie2-2.asp";
	if (item.selectedIndex==1)
		self.location.href="genie2-3.html";
	if (item.selectedIndex==2)
		self.location.href="genie2-1.asp";
	if (item.selectedIndex==3)
		self.location.href="genie2-4.asp";
	if (item.selectedIndex==4)
		self.location.href="genie2-5.asp";
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
function initTranslation()
{
	var e = document.getElementById("g2-2easy123");
	e.innerHTML = _("g2-2 easy123");
		e = document.getElementById("g2-2Internet");
	e.innerHTML = _("g2-2 Internet");
		e = document.getElementById("g2-2Password");
	e.innerHTML = _("g2-2 Password");
		e = document.getElementById("g2-2Security");
	e.innerHTML = _("g2-2 Security");
		e = document.getElementById("g2-2InternetType");
	e.innerHTML = _("g2-2 InternetType");
		e = document.getElementById("g2-2InternetType1");
	e.innerHTML = _("g2-2 InternetType1");
		e = document.getElementById("g2-2InternetType2");
	e.innerHTML = _("g2-2 InternetType2");
		e = document.getElementById("g2-2InternetType3");
	e.innerHTML = _("g2-2 InternetType3");
		e = document.getElementById("g2-2InternetType4");
	e.innerHTML = _("g2-2 InternetType4");
		e = document.getElementById("g2-2InternetType5");
	e.innerHTML = _("g2-2 InternetType5");
		e = document.getElementById("g2-2title");
	e.innerHTML = _("g2-2 title");
		e = document.getElementById("g2-2IP");
	e.innerHTML = _("g2-2 IP");
		e = document.getElementById("g2-2Subnet");
	e.innerHTML = _("g2-2 Subnet");
		e = document.getElementById("g2-2Gateway");
	e.innerHTML = _("g2-2 Gateway");
			e = document.getElementById("g2-2DNS1");
	e.innerHTML = _("g2-2 DNS1");
			e = document.getElementById("g2-2DNS2");
	e.innerHTML = _("g2-2 DNS2");
			e = document.getElementById("g2-2Next");
	e.innerHTML = _("g2-2 Next");
			e = document.getElementById("g2-2Back");
	e.innerHTML = _("g2-2 Back");
			e = document.getElementById("g2-2Exit");
	e.innerHTML = _("g2-2 Exit");
			e = document.getElementById("g2-2No");
	e.innerHTML = _("g2-2 No");
	e = document.getElementById("g2-2Yes");
	e.innerHTML = _("g2-2 Yes");
		e = document.getElementById("g2-2msg");
	e.innerHTML = _("g2-2 msg");
		e = document.getElementById("log_out");
	e.innerHTML = _("dash logout title");
}
</script>
<body onLoad="initTranslation()">
<form method=post name="wanCfg" action="/goform/setWan">
<div id="popup"></div>
<div id="wrapper">
<div id="close"><a href="#"onclick="MM_showHideLayers('popup3_table','','show','popup','','show')" title="Exit"></a></div>
<div class="unit_eazy"></div>
<div class="unit_title" id="g2-2easy123">eaZy 123</div>
<div class="genie_logo"></div>
<div class="genie_color"></div>
<div class="set"><ul><li class="g_ing" id="g2-2Internet">Connect to Internet </li> <li class="g_arrow"></li><li class="g_default" id="g2-2Password">Router Password </li> <li class="g_arrow"></li><li class="g_default" id="g2-2Security"> Wireless Security</li></ul></div>
<div class="set_now"><ul><li>1</li></ul></div>
<div class="set_no4"><ul> <li>2</li> </ul></div>
<div class="set_no5"><ul><li>3</li></ul></div>
<div class="g_word">
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="235" class="b_txt" id="g2-2InternetType">Internet Connection Type:</td>
    <td width="425"><label>
      <select name="menu1" class="select1" onchange="onChangeType(this);">
        <option id="g2-2InternetType1" selected="selected" >Static IP</option>
        <option id="g2-2InternetType2">DHCP</option>
        <option id="g2-2InternetType3">PPPoE</option>
        <option id="g2-2InternetType4">PPTP</option>
        <option id="g2-2InternetType5">L2TP</option>
      </select>
    </label></td>
  </tr>
  <tr>
    <td colspan="2" id="g2-2title">Please refer to the information provided by your Internet Service Provider (ISP) and complete the following blanks.</td>
    </tr>
</table>
</div>
  <div class="input_bn2"><ul><li>
      <table width="576" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td width="230" id="g2-2IP">IP Address:</td>
          <td><table width="50%" border="0" cellpadding="0" cellspacing="0">
            <tr>
            <input name="staticIp" class="input1" maxlength=15 size=25 value="<% getCfgGeneral(1, "wan_ipaddr"); %>">
            </tr>
          </table></td>
        </tr>
        <tr>
          <td id="g2-2Subnet">Subnet Mask:</td>
          <td><table width="50%" border="0" cellpadding="0" cellspacing="0">
            <tr>
            <input name="staticNetmask" class="input1" maxlength=15 size=25 value="<% getCfgGeneral(1, "wan_netmask"); %>">
            </tr>
          </table></td>
        </tr>
        <tr>
          <td id="g2-2Gateway">Default Gateway:</td>
          <td><table width="50%" border="0" cellpadding="0" cellspacing="0">
            <tr>
            <input name="staticGateway" class="input1" maxlength=15 size=25 value="<% getCfgGeneral(1, "wan_gateway"); %>">
            </tr>
          </table></td>
        </tr>
        <tr>
          <td id="g2-2DNS1">Primary DNS:</td>
          <td><table width="50%" border="0" cellpadding="0" cellspacing="0">
            <tr>
            <input name="staticPriDns" class="input1" maxlength=15 size=25 value="<% getCfgGeneral(1, "wan_primary_dns"); %>"></td>
            </tr>
          </table></td>
        </tr>
        <tr>
          <td id="g2-2DNS2">Secondary DNS:</td>
          <td><table width="50%" border="0" cellpadding="0" cellspacing="0">
            <tr>
            <input name="staticSecDns" class="input1" maxlength=15 size=25 value="<% getCfgGeneral(1, "wan_secondary_dns"); %>"></td>
            </tr>
          </table></td>
        </tr>
      </table>
    </li></ul></div>
   <div class="button2"><ul><li>
   <input name="connectionType" type="hidden" value="Ethernet">
   <input name="WAN_IP_Auto" type="hidden" value="1">
   <input name="dns1Type" type="hidden" value="USER">
   <input name="dns2Type" type="hidden" value="USER">
   <input type=hidden value="/local/genie2-4.html" name="wan_url">
   <a  id="g2-2Next" href="javascript: if (CheckValue()) document.wanCfg.submit();">Next</a></li><li><a  id="g2-2Back" href="genie.asp">Back</a></li>
   <li><a  id="g2-2Exit" href="#" onclick="MM_showHideLayers('popup','','show','popup3_table','','show')" title="Exit" style="margin-right:20px;">Exit</a></li></ul></div> 
<div class="index_bg" id="index_bg"></div>
<div class="in_line" id="topline" ></div>
<div class="frame_left"></div>
<div class="frame_right"></div>
<div class="copyright" id="copyright">(C) Copyright 2008 by ZyXEL Communications Corp.</div>
</div>
</form>
<div id="popup3_table">
  <table width="60%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="37"><img src="advance/images/popup_top_left.gif" width="19" height="45" /></td>
        <td background="advance/images/popup_top_center.gif" class="table_title" id=log_out>Log Out</td>
        <td width="15" background="advance/images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')"></a></td>
        <td width="19" height="45"><img src="advance/images/popup_top_right.gif" width="19" height="45" /></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" background="advance/images/popup_main_left.gif">&nbsp;</td>
        <td bgcolor="#FFFFFF"><span class="popupcontent"> 
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="27%"><center>
                <img src="advance/images/i_info.jpg" width="50" height="49" />
              </center>
              </td>
              <td id="g2-2msg" width="73%">Are you sure you want to Exit?</td>
            </tr>
          </table>
         
        </span></td>
        <td width="19" background="advance/images/popup_main_right.gif">&nbsp;</td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="48"><img src="advance/images/popup_down_left.gif" width="19" height="48" /></td>
        <td background="advance/images/popup_down_center.gif">
		<div class="button3"><ul>   
    <li><a href="#" title="No"><span onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')"  id="g2-2No">No</span></a></li>
	<li><a href="/logout.html" title="Yes"><span id="g2-2Yes">Yes</span></a></li>
	</ul></div></td>
        <td width="19" height="37"><img src="advance/images/popup_down_right.gif" width="19" height="48" /></td>
      </tr>
    </table></td>
  </tr>
</table></div>
</body>
</html>
