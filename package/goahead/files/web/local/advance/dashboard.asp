<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/expert.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script>
Butterlate.setTextDomain("main");
</script>
<script type="text/JavaScript">
<!--
function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
//-->
var refresh_interval_select=0;
var	time_id=0;

function refresh_info(){
InfoFrame.location.href = '/local/advance/dashboard_info.asp';
if (refresh_interval_select)
	time_id=self.setTimeout('refresh_info()', refresh_interval_select);
}

function refresh_now(){
	if (time_id)
		clearTimeout(time_id);
	refresh_info();
}

function change_iframe_height(h)
{
document.getElementById('myFrame').height=h;
}

function resetInterval(item)
{
	if (item.selectedIndex==0)
		refresh_interval_select=0;
	if (item.selectedIndex==1)
		refresh_interval_select=60000;
	if (item.selectedIndex==2)
		refresh_interval_select=120000;
	if (item.selectedIndex==3)
		refresh_interval_select=180000;
	if (item.selectedIndex==4)
		refresh_interval_select=240000;
	if (item.selectedIndex==5)
		refresh_interval_select=300000;

	if (time_id)
		clearTimeout(time_id);
	if (refresh_interval_select)
	{
		InfoFrame.location.href = '/local/advance/dashboard_info.asp';
		time_id=self.setTimeout('refresh_info()', refresh_interval_select);
	}
}

function initTranslation()
{
	var e;

	e = document.getElementById("dashEzMode");
	e.innerHTML = _("dash ezmode");
	e = document.getElementById("dashAbout");
	e.innerHTML = _("dash about");
	e = document.getElementById("dashHome");
	e.innerHTML = _("dash home");
	e = document.getElementById("dashLogout");
	e.innerHTML = _("dash logout");
	e = document.getElementById("dashWelcome");
	e.innerHTML = _("dash welcome");

	e = document.getElementById("dashTitle");
	e.innerHTML = _("dash title");
	e = document.getElementById("dashRefInterval");
	e.innerHTML = _("dash ref interval");
	e = document.getElementById("dashRefNone");
	e.innerHTML = _("dash ref none");
	e = document.getElementById("dashRef5s");
	e.innerHTML = _("dash ref 5s");
	e = document.getElementById("dashRef10s");
	e.innerHTML = _("dash ref 10s");
	e = document.getElementById("dashRef15s");
	e.innerHTML = _("dash ref 15s");
	e = document.getElementById("dashRef20s");
	e.innerHTML = _("dash ref 20s");
	e = document.getElementById("dashRef25s");
	e.innerHTML = _("dash ref 25s");
//	e = document.getElementById("dashRef30s");
//	e.innerHTML = _("dash ref 30s");
	e = document.getElementById("dashRefApply");
	e.value = _("dash ref apply");

	e = document.getElementById("dashLogoutTitle");
	e.innerHTML = _("dash logout title");
	e = document.getElementById("dashLogoutMes");
	e.innerHTML = _("dash logout mes");
	e = document.getElementById("dashLogoutNo");
	e.innerHTML = _("dash logout no");
	e = document.getElementById("dashLogoutYes");
	e.innerHTML = _("dash logout yes");

	e = document.getElementById("dashAboutTitle");
	e.innerHTML = _("dash about title");
	e = document.getElementById("dashAboutCheck1");
	e.innerHTML = _("dash about check1");
	e = document.getElementById("dashAboutCheck2");
	e.innerHTML = _("dash about check2");
	e = document.getElementById("dashAboutClose");
	e.innerHTML = _("dash about close");

}
</script>
</head>
<body onload="initTranslation();MM_preloadImages('images/i_edit_on.gif')">
<div id="popup"></div>
<div id="wrapper">
  <div id="top"><div class="logo"></div>
    <div class="modelname">NBG4615</div>
    <div class="quick_link">
      <ul>
        <li class="easy" title="Easy Mode"><a href="../networkmap.asp" target="_parent"><font id="dashEzMode"></font></a></li>
        <li class="about" onclick="MM_showHideLayers('popup','','show','popup4_table','','show')"><a href="#" title="About"><font id="dashAbout"></font></a></li>
        <li class="home"><a href="../index_login.asp" title="Home" target="_parent"><font id="dashHome"></font></a></li>
        <li onclick="MM_showHideLayers('popup','','show','popup3_table','','show')"><a href="#" title="Logout"><font id="dashLogout"></font></a></li>
        <li class="line"></li>
        <li><font id="dashWelcome"></font></li>
      </ul>
    </div>
  </div>
  <div id="blueline"></div>
</div>
<div id="contentframe">
  <div class="mainconent">
    <table width="100%" border="0" cellspacing="0" cellpadding="0"><tr>
        <td width="100%"  valign="top" bgcolor="#39639e">
		<div class="contentdata">
		  <div class="content_title"><font id="dashTitle"></font></div>
		  <div class="refresh"><ul><li class="refresh_left"></li><li class="refresh_center">
		    <table width="280" border="0" cellpadding="0" cellspacing="0">
              <tbody>
                <tr>
                  <td width="65%" height="26" nowrap><font id="dashRefInterval"></font>:
                    <select name="Sys_RefreshList" size="1" onchange="resetInterval(this)">
                      <option value="00000000" selected="selected" id="dashRefNone"> </option>
                      <option value="00000001" id="dashRef5s" ></option>
                      <option value="00000002" id="dashRef10s"></option>
                      <option value="00000003" id="dashRef15s"></option>
                      <option value="00000004" id="dashRef20s"></option>
                      <option value="00000005" id="dashRef25s">300 seconds</option>
                    </select>
                  </td>
                  <td align="center"><input name="Sys_RefreshBtn" id="dashRefApply" value="" onclick="refresh_now();" type="button" />
                  </td>
                </tr>
              </tbody>
		      </table>
		  </li><li class="refresh_right"></li></ul>
	  </div>
  </div></td>
        <td width="1" bgcolor="#779bc8"><img src="../images/air.gif" width="1" height="38" /></td>
  </tr></table>
<iframe id='myFrame' name='InfoFrame' src='/local/advance/dashboard_info.asp' width='100%' height='100%' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>
</iframe>
  </div>
  <br />
  <div class="submenu">
    <ul>
      <li class="i_dashboard" title="Status"></li>
      <li class="i_monitor"><a href="main_monitor_frame.asp" title="Monitor"></a></li>
      <li class="i_config"><a href="main_config_frame.asp" title="Configuration"></a></li>
      <li class="i_maintenance"><a href="main_maintenance_frame.html" title="Maintenance"></a></li>
    </ul>
  </div>
</div>
<div id="popup3_table">
  <table width="60%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="37"><img src="images/popup_top_left.gif" width="19" height="45" /></td>
        <td background="images/popup_top_center.gif" class="table_title"><font id="dashLogoutTitle"></font></td>
        <td width="15" background="images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')"></a></td>
        <td width="19" height="45"><img src="images/popup_top_right.gif" width="19" height="45" /></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" background="images/popup_main_left.gif">&nbsp;</td>
        <td bgcolor="#FFFFFF"><span class="popupcontent">
          <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
              <td width="27%"><center>
                <img src="images/i_info.jpg" width="50" height="49" />
              </center>
              </td>
              <td width="73%"><font id="dashLogoutMes"></font></td>
            </tr>
          </table>

        </span></td>
        <td width="19" background="images/popup_main_right.gif">&nbsp;</td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="19" height="48"><img src="images/popup_down_left.gif" width="19" height="48" /></td>
        <td background="images/popup_down_center.gif"><div class="button3">
          <ul>
    <li><a href="#" title="Reset"><span onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')"><font id="dashLogoutNo"></font></span></a></li>
	<li><a href="/logout.html" title="Reset"><span><font id="dashLogoutYes"></font></span></a></li>
	</ul>
        </div></td>
        <td width="19" height="37"><img src="images/popup_down_right.gif" width="19" height="48" /></td>
      </tr>
    </table></td>
  </tr>
</table></div>
<div id="popup4_table">
  <table width="60%" border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" height="37"><img src="images/popup_top_left.gif" width="19" height="45" /></td>
          <td background="images/popup_top_center.gif" class="table_title"><font id="dashAboutTitle"></font></td>
          <td width="15" background="images/popup_top_center.gif"><a href="#" class="iclose" onclick="MM_showHideLayers('popup','','hide','popup4_table','','hide')"></a></td>
          <td width="19" height="45"><img src="images/popup_top_right.gif" width="19" height="45" /></td>
        </tr>
      </table></td>
    </tr>
    <tr>
      <td><table width="100%" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" background="images/popup_main_left.gif">&nbsp;</td>
          <td bgcolor="#FFFFFF"><span class="popupcontent">
            <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr>
                <td width="27%"><center>
                  <img src="images/i_info.jpg" width="50" height="49" />
                </center></td>
                <td width="73%"><span class="blod">NBG4615</span> <br />
                  (C) Copyright by ZyXEL Communications Corp.<br />
                  <br />
                  <font id="dashAboutCheck1"></font> <a href="http://www.zyxel.com" target="_blank">www.ZyXEL.com</a> <font id="dashAboutCheck2"></font></td>
              </tr>
              <tr>
                <td height="20" colspan="2"><center>
                </center></td>
              </tr>
            </table>
          </span></td>
          <td width="19" background="images/popup_main_right.gif">&nbsp;</td>
        </tr>
      </table></td>
    </tr>
    <tr>
      <td><table width="100%" height="37" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td width="19" height="48"><img src="images/popup_down_left.gif" width="19" height="48" /></td>
          <td background="images/popup_down_center.gif"><div class="button3">
            <ul>
              <li><a href="#" title="Reset"><span onclick="MM_showHideLayers('popup','','hide','popup4_table','','hide')"><font id="dashAboutClose"></font></span></a></li>
            </ul>
          </div></td>
          <td width="19" height="37"><img src="images/popup_down_right.gif" width="19" height="48" /></td>
        </tr>
      </table></td>
    </tr>
  </table>
</div>
</body>
<script>
refresh_info();
</script>
</html>
