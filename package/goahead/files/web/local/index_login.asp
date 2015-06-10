<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head>
<link rel="stylesheet" title="Standard" href="../index_css1.css" type="text/css" media="screen" />
<link href="../ez_css.css" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style>
.on {display:on}
.off {display:none}
</style>
<script type="text/javascript" src="/DST.js"></script>
<script tyle="text/javascript">
if(top.location != self.location) {
		top.location.href = "index.asp";
		top.location.pathname = "index.asp";
}
</script>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("admin");

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

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
	
	var e = document.getElementById("manSelectLang");
	e.innerHTML = _("man select language");
	e = document.getElementById("manGO");
	e.innerHTML = _("man GO");
	e = document.getElementById("mantitle");
	e.innerHTML = _("man title");
	


	e = document.getElementById("manNTPUTC-12Kwajalein");
	e.innerHTML = _("management manNTPUTC-12Kwajalein");
	e = document.getElementById("manNTPUTC-11Samoa");
	e.innerHTML = _("management manNTPUTC-11Samoa");
	e = document.getElementById("manNTPUTC-10Hawaii");
	e.innerHTML = _("management manNTPUTC-10Hawaii");
	e = document.getElementById("manNTPUTC-09Alaska");
	e.innerHTML = _("management manNTPUTC-09Alaska");
	e = document.getElementById("manNTPUTC-08Pacific");
	e.innerHTML = _("management manNTPUTC-08Pacific");
	e = document.getElementById("manNTPUTC-07Arizona");
	e.innerHTML = _("management manNTPUTC-07Arizona");
	e = document.getElementById("manNTPUTC-07Mountain");
	e.innerHTML = _("management manNTPUTC-07Mountain");
	e = document.getElementById("manNTPUTC-06Mexico");
	e.innerHTML = _("management manNTPUTC-06Mexico");
	e = document.getElementById("manNTPUTC-06Central");
	e.innerHTML = _("management manNTPUTC-06Central");
	e = document.getElementById("manNTPUTC-05Colombia");
	e.innerHTML = _("management manNTPUTC-05Colombia");
	e = document.getElementById("manNTPUTC-05Eastern");
	e.innerHTML = _("management manNTPUTC-05Eastern");
	e = document.getElementById("manNTPUTC-04Bolivia");
	e.innerHTML = _("management manNTPUTC-04Bolivia");
	e = document.getElementById("manNTPUTC-04Atlantic");
	e.innerHTML = _("management manNTPUTC-04Atlantic");
	e = document.getElementById("manNTPUTC-04Brazil");
	e.innerHTML = _("management manNTPUTC-04Brazil");
	e = document.getElementById("manNTPUTC-03Newfoundland");
	e.innerHTML = _("management manNTPUTC-03Newfoundland");
	e = document.getElementById("manNTPUTC-03Greenland");
	e.innerHTML = _("management manNTPUTC-03Greenland");
	e = document.getElementById("manNTPUTC-03Brazil");
	e.innerHTML = _("management manNTPUTC-03Brazil");
	e = document.getElementById("manNTPUTC-03Guyana");
	e.innerHTML = _("management manNTPUTC-03Guyana");
	e = document.getElementById("manNTPUTC-02Mid-Atlantic");
	e.innerHTML = _("management manNTPUTC-02Mid-Atlantic");
	e = document.getElementById("manNTPUTC-01Azores");
	e.innerHTML = _("management manNTPUTC-01Azores");
	e = document.getElementById("manNTPUTCGambia");
	e.innerHTML = _("management manNTPUTCGambia");
	e = document.getElementById("manNTPUTCEngland");
	e.innerHTML = _("management manNTPUTCEngland");
	e = document.getElementById("manNTPUTCTunisia");
	e.innerHTML = _("management manNTPUTCTunisia");
	e = document.getElementById("manNTPUTCFrance");
	e.innerHTML = _("management manNTPUTCFrance");
	e = document.getElementById("manNTPUTCSouthAfrica");
	e.innerHTML = _("management manNTPUTCSouthAfrica");
	e = document.getElementById("manNTPUTCGreece");
	e.innerHTML = _("management manNTPUTCGreece");
	e = document.getElementById("manNTPUTCIraq");
	e.innerHTML = _("management manNTPUTCIraq");
	e = document.getElementById("manNTPUTCArmenia");
	e.innerHTML = _("management manNTPUTCArmenia");
	e = document.getElementById("manNTPUTCPakistan");
	e.innerHTML = _("management manNTPUTCPakistan");
	e = document.getElementById("manNTPUTCBombay");
	e.innerHTML = _("management manNTPUTCBombay");
	e = document.getElementById("manNTPUTCBangladesh");
	e.innerHTML = _("management manNTPUTCBangladesh");
	e = document.getElementById("manNTPUTCThailand");
	e.innerHTML = _("management manNTPUTCThailand");
	e = document.getElementById("manNTPUTCTaiwan");
	e.innerHTML = _("management manNTPUTCTaiwan");
	e = document.getElementById("manNTPUTCJapan");
	e.innerHTML = _("management manNTPUTCJapan");
	e = document.getElementById("manNTPUTCSouthAustralia");
	e.innerHTML = _("management manNTPUTCSouthAustralia");
	e = document.getElementById("manNTPUTCGuam");
	e.innerHTML = _("management manNTPUTCGuam");
	e = document.getElementById("manNTPUTCAustralia");
	e.innerHTML = _("management manNTPUTCAustralia");
	e = document.getElementById("manNTPUTCSolomonIslands");
	e.innerHTML = _("management manNTPUTCSolomonIslands");
	e = document.getElementById("manNTPUTCFiji");
	e.innerHTML = _("management manNTPUTCFiji");
	e = document.getElementById("manNTPUTCNewZealand");
	e.innerHTML = _("management manNTPUTCNewZealand");

	e = document.getElementById("Austria - Vienna");
	e.innerHTML = _("city1");
	e = document.getElementById("China - Beijing");
	e.innerHTML = _("city2");
	e = document.getElementById("Costa Rica - San Jose");
	e.innerHTML = _("city3");
	e = document.getElementById("Czech - Prague");
	e.innerHTML = _("city4");
	e = document.getElementById("Denmark - Copenhagen");
	e.innerHTML = _("city5");
	e = document.getElementById("Finland - Helsinki");
	e.innerHTML = _("city6");
	e = document.getElementById("France - Paris");
	e.innerHTML = _("city7");
	e = document.getElementById("Germany - Berlin");
	e.innerHTML = _("city8");
	e = document.getElementById("Greece - Athens");
	e.innerHTML = _("city9");
	e = document.getElementById("India - New Delhi");
	e.innerHTML = _("city10");
	e = document.getElementById("Indonesia - Jakarta");
	e.innerHTML = _("city11");
	e = document.getElementById("Italy - Roma");
	e.innerHTML = _("city12");
	e = document.getElementById("Japan - Tokyo");
	e.innerHTML = _("city13");
	e = document.getElementById("Malaysia - Kuala Lumpur");
	e.innerHTML = _("city14");
	e = document.getElementById("Netherlands - Amsterdam");
	e.innerHTML = _("city15");
	e = document.getElementById("Norway - Oslo");
	e.innerHTML = _("city16");
	e = document.getElementById("Russia - Moscow");
	e.innerHTML = _("city17");
	e = document.getElementById("Singapore - Singapore");
	e.innerHTML = _("city18");
	e = document.getElementById("Spain - Madrid");
	e.innerHTML = _("city19");
	e = document.getElementById("Sweden - Stockholm");
	e.innerHTML = _("city20");
	e = document.getElementById("Switzerland - Bern");
	e.innerHTML = _("city21");
	e = document.getElementById("Taiwan - Taipei");
	e.innerHTML = _("city22");
	e = document.getElementById("Thailand - Bangkok");
	e.innerHTML = _("city23");
	e = document.getElementById("Turkey - Ankara");
	e.innerHTML = _("city24");
	e = document.getElementById("UK - London");
	e.innerHTML = _("city25");
	e = document.getElementById("UK - Greenwich");
	e.innerHTML = _("city26");
	e = document.getElementById("USA - New York");
	e.innerHTML = _("city27");
	e = document.getElementById("USA - Washington D.C.");
	e.innerHTML = _("city28");
	e = document.getElementById("USA - Los Angeles");
	e.innerHTML = _("city29");
	e = document.getElementById("Vietnam - Hanoi");
	e.innerHTML = _("city30");
	e = document.getElementById("timezone");
	e.innerHTML = _("time_zone");
	e = document.getElementById("Finish");
	e.innerHTML = _("finish");
	e = document.getElementById("nointernet");
	e.innerHTML = _("no_internet");
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

function showcity(str)
{
	document.getElementById('city_display').innerHTML=_(str);
}
</script>
<script type="text/JavaScript">
<!--
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
</head>
<body onLoad="initValue()">
<form method="post" name="web_login" action="/goform/web_login">
<div id="wrapper">
<div class="index_bg" id="index_bg"></div>
<div class="frame_left"></div>
<div class="frame_right"></div>
<div class="logo" id="logo"></div>
<div class="home_image"></div>
<div class="home_note">
  <ul><li class="modelname">NBG4615</li>
    <li class="welcome_text" id="mantitle"></li><li>
  <table width="350" border="0" cellpadding="0" cellspacing="0">
<tr>
  <td>&nbsp;</td>
  <td colspan="3">&nbsp;</td>
  </tr>
<tr>
  <td>&nbsp;</td>
  <td colspan="3"><div><ul>
    <li class="button3"><input type="hidden" name="set_flag" size="1" value="0"><a href="networkmap.asp" title="GO"><span id="manGO"></span></a></li>
    </ul>
  </div></td>
  </tr>
<tr>
  <td>&nbsp;</td>
  <td width="92">&nbsp;</td>
  <td width="117">&nbsp;</td>
  <td width="137">&nbsp;</td>
</tr>

<tr>
  <td>&nbsp;</td>
  <td><span class="password_word" id="manSelectLang"></span><span class="password_word"> :&nbsp</span></td>
  <td><span class="green">
<select name="langSelection" id="langSelection" onchange="return setLanguage()">
<!-- added by initValue -->
</select>
<!--
        <select name="select">
          <option value="english" selected="selected">English</option>
          <option value="Deutsch">Deutsch</option>
          <option value="Français">Français</option>
          <option value="Español">Español</option>
          <option value="繁耤¸­佢>繁耤¸­佼/option>
          <option value="Italiano">Italiano</option>
        </select>
-->        
    </span></td>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td width="4">&nbsp;</td>
    <td colspan="3">	 </td>
    </tr>
</table>
</li></ul>
</div>
<div class="copyright" id="copyright">(C) Copyright  by ZyXEL Communications Corp.<img src="/goahead1.gif"></div>
<div class="set_info"><a href="#" onclick="MM_showHideLayers('set_pupup1','','show')"></a></div>
<div class="set_info2"><a href="#" onclick="MM_showHideLayers('set_pupup2','','show')"></a></div>
<div id="set_pupup1">
<div class="close1"><a href="#" onclick="MM_showHideLayers('set_pupup1','','hide')"></a></div>
<ul><li style="margin-top:10px; font-weight:bold;"></li><li>
<select name="degree_select">
    <option value='f' id=tempc_select>&deg;F</option>
    <option value='c' id=tempf_select>&deg;C</option>
</select>
</li><li style="font-weight:bold;">Change location
</li><li class="list"><select name="city_select" size="2">
<option value='AUXX0025' id='Austria - Vienna'></option>
<option value='CHXX0008' id='China - Beijing'></option>
<option value='CSXX0009' id='Costa Rica - San Jose'></option>
<option value='EZXX0012' id='Czech - Prague'></option>
<option value='DAXX0009' id='Denmark - Copenhagen'></option>
<option value='FIXX0002' id='Finland - Helsinki'></option>
<option value='FRXX0076' id='France - Paris'></option>
<option value='GMXX0007' id='Germany - Berlin'></option>
<option value='GRXX0004' id='Greece - Athens'></option>
<option value='INXX0096' id='India - New Delhi'></option>
<option value='IDXX0022' id='Indonesia - Jakarta'></option>
<option value='ITXX0067' id='Italy - Roma'></option>
<option value='JAXX0085' id='Japan - Tokyo'></option>
<option value='MYXX0008' id='Malaysia - Kuala Lumpur'></option>
<option value='NLXX0002' id='Netherlands - Amsterdam'></option>
<option value='NOXX0029' id='Norway - Oslo'></option>
<option value='RSXX0063' id='Russia - Moscow'></option>
<option value='SNXX0006' id='Singapore - Singapore'></option>
<option value='SPXX0050' id='Spain - Madrid'></option>
<option value='SWXX0031' id='Sweden - Stockholm'></option>
<option value='SZXX0006' id='Switzerland - Bern'></option>
<option value='TWXX0021' id='Taiwan - Taipei'></option>
<option value='THXX0002' id='Thailand - Bangkok'></option>
<option value='TUXX0002' id='Turkey - Ankara'></option>
<option value='UKXX0085' id='UK - London'></option>
<option value='UKXX1428' id='UK - Greenwich'></option>
<option value='USNY0996' id='USA - New York'></option>
<option value='USDC0001' id='USA - Washington D.C.'></option>
<option value='USCA0638' id='USA - Los Angeles'></option>
<option value='VMXX0006' id='Vietnam - Hanoi'></option>
</select>
</li><li class="button3">
  <div align="right" style="margin-right:10px;"><a href="javascript: document.web_login.set_flag.value='1'; document.web_login.submit();"><span id="Finish">Finish</span></a>  </div>
</li>
  </ul></div>
  <div id="set_pupup2">
<div class="close1"><a href="#" onclick="MM_showHideLayers('set_pupup2','','hide')"></a></div>
  <ul><li style="font-weight:bold; margin-top:10px;" id="timezone">Change time zone
</li><li class="list"><select name="time_zone" size="2">
	<option value="UTC12" id="manNTPUTC-12Kwajalein"></option>
	<option value="UTC11" id="manNTPUTC-11Samoa"></option>
	<option value="UTC10" id="manNTPUTC-10Hawaii"></option>
	<option value="NAST9NADT,M3.2.0/2,M11.1.0/2" id="manNTPUTC-09Alaska"></option>
	<option value="PST8PDT,M3.2.0/2,M11.1.0/2" id="manNTPUTC-08Pacific"></option>
	<option value="UTC7" id="manNTPUTC-07Arizona"></option>
	<option value="MST7MDT,M3.2.0/2,M11.1.0/2" id="manNTPUTC-07Mountain"></option>
	<option value="UTC6" id="manNTPUTC-06Mexico"></option>
	<option value="CST6CDT,M3.2.0/2,M11.1.0/2" id="manNTPUTC-06Central"></option>
	<option value="UTC5" id="manNTPUTC-05Colombia"></option>
	<option value="EST5EDT,M3.2.0/2,M11.1.0/2" id="manNTPUTC-05Eastern"></option>
	<option value="UTC4" id="manNTPUTC-04Bolivia"></option>
	<option value="AST4ADT,M3.2.0/2,M11.1.0/2" id="manNTPUTC-04Atlantic"></option>
	<option value="BRWST4BRWDT,M10.3.0/0,M2.5.0/0" id="manNTPUTC-04Brazil"></option>
	<option value="NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01" id="manNTPUTC-03Newfoundland"></option>
	<option value="WGST3WGDT,M3.5.6/22,M10.5.6/23" id="manNTPUTC-03Greenland"></option>
	<option value="BRST3BRDT,M10.3.0/0,M2.5.0/0" id="manNTPUTC-03Brazil"></option>
	<option value="UTC3" id="manNTPUTC-03Guyana"></option>
	<option value="UTC2" id="manNTPUTC-02Mid-Atlantic"></option>
	<option value="STD1DST,M3.5.0/2,M10.5.0/2" id="manNTPUTC-01Azores"></option>
	<option value="UTC0" id="manNTPUTCGambia"></option>
	<option value="GMT0BST,M3.5.0/2,M10.5.0/2" id="manNTPUTCEngland"></option>
	<option value="UTC-1" id="manNTPUTCTunisia"></option>
	<option value="STD-1DST,M3.5.0/2,M10.5.0/2" id="manNTPUTCFrance"></option>
	<option value="UTC-2" id="manNTPUTCSouthAfrica"></option>
	<option value="STD-2DST,M3.5.0/2,M10.5.0/2" id="manNTPUTCGreece"></option>
	<option value="UTC-3" id="manNTPUTCIraq"></option>
	<option value="UTC-4" id="manNTPUTCArmenia"></option>
	<option value="UTC-5" id="manNTPUTCPakistan"></option>
	<option value="UTC-5:30" id="manNTPUTCBombay"></option>
	<option value="UTC-6" id="manNTPUTCBangladesh"></option>
	<option value="UTC-7" id="manNTPUTCThailand"></option>
	<option value="UTC-8" id="manNTPUTCTaiwan">UTC+08:00 Taiwan, China, Hong Kong, Western Australia, Singapore</option>
	<option value="UTC-9" id="manNTPUTCJapan"></option>
	<option value="CST-9:30CST,M10.5.0/2,M3.5.0/3" id="manNTPUTCSouthAustralia"></option>
	<option value="UTC-10" id="manNTPUTCGuam"></option>
	<option value="STD-10DST,M10.5.0/2,M3.5.0/2" id="manNTPUTCAustralia"></option>
	<option value="UTC-11" id="manNTPUTCSolomonIslands"></option>
	<option value="UTC-12" id="manNTPUTCFiji"></option>
	<option value="STD-12DST,M10.5.0/2,M3.5.0/2" id="manNTPUTCNewZealand"></option>
  </select>
</li><li class="button3">
  <div align="right" style="margin-right:10px;"><a href="javascript: document.web_login.set_flag.value='2'; document.web_login.submit();"><span id="Finish">Finish</span></a>  </div>
</li>
  </ul></div>
<span id=nointernet_msg class="off">
<div ><a id='nointernet'>Please connect Internet to see the weather.</a></div>
</span>
<span id=nointernet_check class="on">
<div id='checkinternet'><center><img src="/loading.gif"></center></div>
</span>
</span>
  <div class="infomation">
  <table width="100%" height="82" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td width="104" class="weather" id=jpg_display></td>
      <td width="130"><div class="weather_word"><ul><li class="thermograph" id=temp_display></li>
      <li class="city" id=city_display></li>
      </ul></div></td>
      <td width="127"><div class="clock"><ul>
        <li class="time" id="mtenCurrent_Time"></li>
        <li class="date" id="mtenCurrent_Date"></li>
      </ul>
      </div></td>
    </tr>
  </table>
</div>
</div>
</form>
<form method="post" name="NTP" action="/goform/NTP">
         <input type="hidden" name="mtenCurrent_Hour" value="<% getCurrentHour();%>" />
         <input type="hidden" name="mtenCurrent_Min" value="<% getCurrentMin();%>" />
         <input type="hidden" name="mtenCurrent_Sec" value="<% getCurrentSec();%>" />
         <input type="hidden" name="mtenCurrent_Year" value="<% getCurrentYear();%>" />

         <input type="hidden" name="mtenCurrent_Mon" value="<% getCurrentMon();%>" />

         <input type="hidden" name="mtenCurrent_Day" value="<% getCurrentDay();%>" />
</form>
<iframe name='detect' src='/weather_connect.asp' width='0' height='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>             
</iframe>
<iframe name='setlang' src='/temp.html' width='0' height='0' marginwidth='0' marginheight='0' frameborder='0' scrolling='no'>             
</iframe>
</body>
<script>
	var tz = "<% getCfgGeneral(1, "TZ"); %>";
	if (tz == "UTC12")
		document.NTP.time_zone.options.selectedIndex = 0;
	else if (tz == "UTC11")
		document.web_login.time_zone.options.selectedIndex = 1;
	else if (tz == "UTC10")
		document.web_login.time_zone.options.selectedIndex = 2;
	else if (tz == "NAST9NADT,M3.2.0/2,M11.1.0/2")
		document.web_login.time_zone.options.selectedIndex = 3;
	else if (tz == "PST8PDT,M3.2.0/2,M11.1.0/2")
		document.web_login.time_zone.options.selectedIndex = 4;
	else if (tz == "UTC7")
		document.web_login.time_zone.options.selectedIndex = 5;
	else if (tz == "MST7MDT,M3.2.0/2,M11.1.0/2")
		document.web_login.time_zone.options.selectedIndex = 6;
	else if (tz == "UTC6")
		document.web_login.time_zone.options.selectedIndex = 7;
	else if (tz == "CST6CDT,M3.2.0/2,M11.1.0/2")
		document.web_login.time_zone.options.selectedIndex = 8;
	else if (tz == "UTC5")
		document.web_login.time_zone.options.selectedIndex = 9;
	else if (tz == "EST5EDT,M3.2.0/2,M11.1.0/2")
		document.web_login.time_zone.options.selectedIndex = 10;
	else if (tz == "UTC4")
		document.web_login.time_zone.options.selectedIndex = 11;
	else if (tz == "AST4ADT,M3.2.0/2,M11.1.0/2")
		document.web_login.time_zone.options.selectedIndex = 12;
	else if (tz == "BRWST4BRWDT,M10.3.0/0,M2.5.0/0")
		document.web_login.time_zone.options.selectedIndex = 13;
	else if (tz == "NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01")
		document.web_login.time_zone.options.selectedIndex = 14;
	else if (tz == "WGST3WGDT,M3.5.6/22,M10.5.6/23")
		document.web_login.time_zone.options.selectedIndex = 15;
	else if (tz == "BRST3BRDT,M10.3.0/0,M2.5.0/0")
		document.web_login.time_zone.options.selectedIndex = 16;
	else if (tz == "UTC3")
		document.web_login.time_zone.options.selectedIndex = 17;
	else if (tz == "UTC2")
		document.web_login.time_zone.options.selectedIndex = 18;
	else if (tz == "STD1DST,M3.5.0/2,M10.5.0/2")
		document.web_login.time_zone.options.selectedIndex = 19;
	else if (tz == "UTC0")
		document.web_login.time_zone.options.selectedIndex = 20;
	else if (tz == "GMT0BST,M3.5.0/2,M10.5.0/2")
		document.web_login.time_zone.options.selectedIndex = 21;
	else if (tz == "UTC-1")
		document.web_login.time_zone.options.selectedIndex = 22;
	else if (tz == "STD-1DST,M3.5.0/2,M10.5.0/2")
		document.web_login.time_zone.options.selectedIndex = 23;
	else if (tz == "UTC-2")
		document.web_login.time_zone.options.selectedIndex = 24;
	else if (tz == "STD-2DST,M3.5.0/2,M10.5.0/2")
		document.web_login.time_zone.options.selectedIndex = 25;
	else if (tz == "UTC-3")
		document.web_login.time_zone.options.selectedIndex = 26;
	else if (tz == "UTC-4")
		document.web_login.time_zone.options.selectedIndex = 27;
	else if (tz == "UTC-5")
		document.web_login.time_zone.options.selectedIndex = 28;
	else if (tz == "UTC-5:30")
		document.web_login.time_zone.options.selectedIndex = 29;
	else if (tz == "UTC-6")
		document.web_login.time_zone.options.selectedIndex = 30;
	else if (tz == "UTC-7")
		document.web_login.time_zone.options.selectedIndex = 31;
	else if (tz == "UTC-8")
		document.web_login.time_zone.options.selectedIndex = 32;
	else if (tz == "UTC-9")
		document.web_login.time_zone.options.selectedIndex = 33;
	else if (tz == "CST-9:30CST,M10.5.0/2,M3.5.0/3")
		document.web_login.time_zone.options.selectedIndex = 34;
	else if (tz == "UTC-10")
		document.web_login.time_zone.options.selectedIndex = 35;
	else if (tz == "STD-10DST,M10.5.0/2,M3.5.0/2")
		document.web_login.time_zone.options.selectedIndex = 36;
	else if (tz == "UTC-11")
		document.web_login.time_zone.options.selectedIndex = 37;
	else if (tz == "UTC-12")
		document.web_login.time_zone.options.selectedIndex = 38;
	else if (tz == "STD-12DST,M10.5.0/2,M3.5.0/2")
		document.web_login.time_zone.options.selectedIndex = 39;
		
	var defree = "<% getCfgGeneral(1, "WeatherDegree"); %>";
	if (defree == "f")
		document.web_login.degree_select.options.selectedIndex = 0;
	if (defree == "c")
		document.web_login.degree_select.options.selectedIndex = 1;
		
	var city = "<% getCfgGeneral(1, "WeatherCity"); %>";
	if (city == "AUXX0025")
		document.web_login.city_select.options.selectedIndex = 0;
		else	
	if (city == "CHXX0008")
		document.web_login.city_select.options.selectedIndex = 1;
		else
	if (city == "CSXX0009")
		document.web_login.city_select.options.selectedIndex = 2;
		else
	if (city == "EZXX0012")
		document.web_login.city_select.options.selectedIndex = 3;
		else
	if (city == "DAXX0009")
		document.web_login.city_select.options.selectedIndex = 4;
		else
	if (city == "FIXX0002")
		document.web_login.city_select.options.selectedIndex = 5;
		else
	if (city == "FRXX0076")
		document.web_login.city_select.options.selectedIndex = 6;
		else
	if (city == "GMXX0007")
		document.web_login.city_select.options.selectedIndex = 7;
		else
	if (city == "GRXX0004")
		document.web_login.city_select.options.selectedIndex = 8;
		else
	if (city == "INXX0096")
		document.web_login.city_select.options.selectedIndex = 9;
		else
	if (city == "IDXX0022")
		document.web_login.city_select.options.selectedIndex = 10;
		else
	if (city == "ITXX0067")
		document.web_login.city_select.options.selectedIndex = 11;
		else
	if (city == "JAXX0085")
		document.web_login.city_select.options.selectedIndex = 12;
		else
	if (city == "MYXX0008")
		document.web_login.city_select.options.selectedIndex = 13;
		else
	if (city == "NLXX0002")
		document.web_login.city_select.options.selectedIndex = 14;
		else
	if (city == "NOXX0029")
		document.web_login.city_select.options.selectedIndex = 15;
		else
	if (city == "RSXX0063")
		document.web_login.city_select.options.selectedIndex = 16;
		else
	if (city == "SNXX0006")
		document.web_login.city_select.options.selectedIndex = 17;
		else
	if (city == "SPXX0050")
		document.web_login.city_select.options.selectedIndex = 18;
		else
	if (city == "SWXX0031")
		document.web_login.city_select.options.selectedIndex = 19;
		else
	if (city == "SZXX0006")
		document.web_login.city_select.options.selectedIndex = 20;
		else
	if (city == "TWXX0021")
		document.web_login.city_select.options.selectedIndex = 21;
		else	
	if (city == "THXX0002")
		document.web_login.city_select.options.selectedIndex = 22;
		else
	if (city == "TUXX0002")
		document.web_login.city_select.options.selectedIndex = 23;
		else
	if (city == "UKXX0085")
		document.web_login.city_select.options.selectedIndex = 24;
		else
	if (city == "UKXX1428")
		document.web_login.city_select.options.selectedIndex = 25;
		else	
	if (city == "USNY0996")
		document.web_login.city_select.options.selectedIndex = 26;
		else	
	if (city == "USDC0001")
		document.web_login.city_select.options.selectedIndex = 27;
		else
	if (city == "USCA0638")
		document.web_login.city_select.options.selectedIndex = 28;
		else
	if (city == "VMXX0006")
		document.web_login.city_select.options.selectedIndex = 29;
		else
	if (city == "Greenwich")
		document.web_login.city_select.options.selectedIndex = 30;
		else
		document.web_login.city_select.options.selectedIndex = 30;

var refresh_interval=10000;
function refresh_info(){
detect.location.href = '/weather_checking.asp';
}
self.setTimeout('refresh_info()', refresh_interval);
</script>
<script>
 start_Mon="<% getCfgGeneral(1, "startMon"); %>";
 start_Day="<% getCfgGeneral(1, "startDay"); %>";
 start_Hour="<% getCfgGeneral(1, "startclock"); %>"; 
 end_Mon="<% getCfgGeneral(1, "endMon"); %>";
 end_Day="<% getCfgGeneral(1, "endDay"); %>";
 end_Hour="<% getCfgGeneral(1, "endclock"); %>";
 enable_dst = "<% getCfgGeneral(1, "enabledaylight"); %>";

	if(enable_dst=="1")
	{
	daylightsaving();
	}
initDateAndTime();
IncreaseSec();
setlang.location.href = '/temp.html';
</script>
</html>
