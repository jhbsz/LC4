<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head>
<link href="images/css.css" rel="stylesheet" type="text/css" />
<link href="advance/images/table.css" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("main");

function initTranslation()
{
	var lang="<% getCfgGeneral(1, "Language"); %>";
	var cook = "en";

	if ((lang!="en") && (lang!="de") && (lang!="fr") && (lang!="es") && (lang!="zhtw") && (lang!="it"))
		lang = "en";

	if (document.cookie.length > 0) {
		var s = document.cookie.indexOf("language=");
		var e = document.cookie.indexOf(";", s);
		if (s != -1) {
			if (e == -1)
				cook = document.cookie.substring(s+9);
			else
				cook = document.cookie.substring(s+9, e);
		}
	}
	
	document.cookie="language="+lang+"; path=/";

	if (cook != lang)
		self.location.reload();
	
	var e = document.getElementById("gWelcome");
	e.innerHTML = _("g Welcome");
	e = document.getElementById("g2-2easy123");
	e.innerHTML = _("g2-2 easy123");
		e = document.getElementById("g2-2Internet");
	e.innerHTML = _("g2-2 Internet");
		e = document.getElementById("g2-2Password");
	e.innerHTML = _("g2-2 Password");
		e = document.getElementById("g2-2Security");
	e.innerHTML = _("g2-2 Security");
	e = document.getElementById("gtitle");
	e.innerHTML = _("g title");
	e = document.getElementById("gConnect");
	e.innerHTML = _("g Connect");
	e = document.getElementById("gLanguage");
	e.innerHTML = _("g Language");
	e = document.getElementById("g2-1No");
	e.innerHTML = _("g2-1 No");
	e = document.getElementById("g2-1Yes");
	e.innerHTML = _("g2-1 Yes");
}

function initValue() {
	var lang_element = document.getElementById("langSelection");
	var lang_en = "<% getLangBuilt("en"); %>";
	var lang_de = "<% getLangBuilt("de"); %>";
	var lang_fr = "<% getLangBuilt("fr"); %>";
	var lang_es = "<% getLangBuilt("es"); %>";
	var lang_zhtw = "<% getLangBuilt("zhtw"); %>";
	var lang_it = "<% getLangBuilt("it"); %>";
	var lang_zhcn = "<% getLangBuilt("zhcn"); %>";

	initTranslation();
	//lang_element.options.length = 0;
	if (lang_en == "1")
		lang_element.options[lang_element.length] = new Option('English', 'en');
	if (lang_de == "1")
		lang_element.options[lang_element.length] = new Option('Deutsch', 'de');
	if (lang_fr == "1")
		lang_element.options[lang_element.length] = new Option('Français', 'fr');
	if (lang_es == "1")
		lang_element.options[lang_element.length] = new Option('Español', 'es');
	if (lang_zhtw == "1")
		lang_element.options[lang_element.length] = new Option('繁體中文', 'zhtw');
	if (lang_it == "1")
		lang_element.options[lang_element.length] = new Option('Italiano', 'it');
	if (lang_zhcn == "1")
		lang_element.options[lang_element.length] = new Option('简体中文', 'zhcn');

	if (document.cookie.length > 0) {
		var s = document.cookie.indexOf("language=");
		var e = document.cookie.indexOf(";", s);
		var lang = "en";
		var i;

		if (s != -1) {
			if (e == -1)
				lang = document.cookie.substring(s+9);
			else
				lang = document.cookie.substring(s+9, e);
		}
		for (i=0; i<lang_element.options.length; i++) {
			if (lang == lang_element.options[i].value) {
				lang_element.options.selectedIndex = i;
				break;
			}
		}
	}
}

function setLanguage()
{
	document.cookie="language="+document.forms[0].langSelection.value+"; path=/";
	//self.location.reload();
	setlang.location.href = '/set_language.asp?'+document.forms[0].langSelection.value;
	return true;
}
</script>
    <script type="text/JavaScript">
<!--
function MM_goToURL() { //v3.0
  var i, args=MM_goToURL.arguments; document.MM_returnValue = false;
  for (i=0; i<(args.length-1); i+=2) eval(args[i]+".location='"+args[i+1]+"'");
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
//-->
    </script>
<title>.::Welcome to ZyXEL NBG4615::.</title><meta http-equiv="Content-Type" content="text/html; charset=utf-8" /></head>
<body onLoad="initValue()">
<form method="post" name="web_login" action="/goform/web_login">
<div id="popup"></div>
<div id="wrapper">
<!--
<div id="close"><a href="#"onclick="MM_showHideLayers('popup3_table','','show','popup','','show')" title="Exit"></a></div>
-->
<div class="unit_eazy"></div>
<div class="unit_title" id="g2-2easy123">eaZy 123</div>
<div class="genie_logo"></div>
<div class="genie_color"></div>
<div class="set"><ul><li class="g_default" id="g2-2Internet">Connect to Internet </li><li class="g_arrow"></li><li class="g_default" id="g2-2Password">Router Password </li><li class="g_arrow"></li><li class="g_default" id="g2-2Security"> Wireless Security</li></ul></div>
<div class="set_no3"><ul><li>1</li></ul></div>
<div class="set_no"><ul><li>2</li></ul></div>
<div class="set_no2"><ul><li>3</li></ul></div>
<div class="welcome"><a id="gWelcome">Welcome</a></div>
<div class="g_word1" ><a id="gtitle">The coming steps will guide you through the setup of your router and connecting to the Internet. Please click one of the language icons below to continue.</a></div>
<div class="language"><ul><li class="white"><span id="gLanguage">Language</span>:</li><li class="green">
<select class="select0" name="langSelection" id="langSelection" onchange="return setLanguage()">
<!-- added by initValue -->
</select>
<!--
<select name="select" class="select0"> 
                    <option value="english" selected="selected">English</option>
                  <option value="Deutsch">Deutsch</option>
                  <option value="Français">Français</option>
                  <option value="Español">Español</option>
                  <option value="繁耤¸­佢>繁耤¸­佼/option>
                  <option value="Italiano">Italiano</option>
</select>
-->
</li></ul></div>
<div class="g_line"></div>
<div class="g_button">
  <ul><li class="left2"></li><li class="center2"><a href="genie2.html" id="gConnect">Connect to Internet</a></li><li class="right2"></li></ul></div>
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
      <td background="advance/images/popup_top_center.gif" class="table_title">Log Out</td>
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
            <td width="73%">Are you sure you want to Exit?</td>
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
  <li><a href="#" title="No"><span  id="g2-1No" onclick="MM_showHideLayers('popup','','hide','popup3_table','','hide')">No</span></a></li>
<li><a href="/" title="Yes"><span  id="g2-1Yes">Yes</span></a></li>
</ul></div></td>
      <td width="19" height="37"><img src="advance/images/popup_down_right.gif" width="19" height="48" /></td>
    </tr>
  </table></td>
</tr>
</table></div>
<iframe name='setlang' src='/temp.html' width='0' height='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>             
</iframe>
</body>
<script>
setlang.location.href = '/temp.html';
</script>
</html>