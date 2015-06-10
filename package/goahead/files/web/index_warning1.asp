<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head>
<link rel="stylesheet" title="Standard" href="ez_css.css" type="text/css" media="screen" />
<link href="index_css1.css" rel="stylesheet" type="text/css" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<style>
.on {display:on}
.off {display:none}
</style>
<script tyle="text/javascript">
if(top.location != self.location) {
		top.location.href = "index.asp";
		top.location.pathname = "index.asp";
}
</script>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script type="text/javascript" src="/DST.js"></script>
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
	
	var e = document.getElementById("manAdmPasswd");
	e.innerHTML = _("man admin passwd");
	e = document.getElementById("manSelectLang");
	e.innerHTML = _("man select language");

  e = document.getElementById("manNTPMidIsland");
  e.innerHTML = _("management manNTPMidIsland");
  e = document.getElementById("manNTPHawaii");
  e.innerHTML = _("management manNTPHawaii");
  e = document.getElementById("manNTPAlaska");
  e.innerHTML = _("management manNTPAlaska");
  e = document.getElementById("manNTPPacific");
  e.innerHTML = _("management manNTPPacific");
  e = document.getElementById("manNTPArizona");
  e.innerHTML = _("management manNTPArizona");
  e = document.getElementById("manNTPMountain");
  e.innerHTML = _("management manNTPMountain");
  e = document.getElementById("manNTPCentralAmerica");
  e.innerHTML = _("management manNTPCentralAmerica");
  e = document.getElementById("manNTPCentralTime");
  e.innerHTML = _("management manNTPCentralTime");
  e = document.getElementById("manNTPBogota");
  e.innerHTML = _("management manNTPBogota");
  e = document.getElementById("manNTPEasternTime");
  e.innerHTML = _("management manNTPEasternTime");
  e = document.getElementById("manNTPAltlantic");
  e.innerHTML = _("management manNTPAltlantic");
  e = document.getElementById("manNTPCaracas");
  e.innerHTML = _("management manNTPCaracas");
  e = document.getElementById("manNTPBrasilia");
  e.innerHTML = _("management manNTPBrasilia");
  e = document.getElementById("manNTPBuenos");
  e.innerHTML = _("management manNTPBuenos");
  e = document.getElementById("manNTPMid-Atlantic");
  e.innerHTML = _("management manNTPMid-Atlantic");
  e = document.getElementById("manNTPAzores");
  e.innerHTML = _("management manNTPAzores");
  e = document.getElementById("manNTPCasablanca");
  e.innerHTML = _("management manNTPCasablanca");
  e = document.getElementById("manNTPGreenwich");
  e.innerHTML = _("management manNTPGreenwich");
  e = document.getElementById("manNTPAmsterdam");
  e.innerHTML = _("management manNTPAmsterdam");
  e = document.getElementById("manNTPBelgrade");
  e.innerHTML = _("management manNTPBelgrade");
  e = document.getElementById("manNTPBrussels");
  e.innerHTML = _("management manNTPBrussels");
  e = document.getElementById("manNTPAthens");
  e.innerHTML = _("management manNTPAthens");
  e = document.getElementById("manNTPBucharest");
  e.innerHTML = _("management manNTPBucharest");
  e = document.getElementById("manNTPBaghdad");
  e.innerHTML = _("management manNTPBaghdad");
  e = document.getElementById("manNTPMoscow");
  e.innerHTML = _("management manNTPMoscow");
  e = document.getElementById("manNTPMuscat");
  e.innerHTML = _("management manNTPMuscat");
  e = document.getElementById("manNTPEkaterinburg");
  e.innerHTML = _("management manNTPEkaterinburg");
  e = document.getElementById("manNTPAlmaty");
  e.innerHTML = _("management manNTPAlmaty");
  e = document.getElementById("manNTPBangkok");
  e.innerHTML = _("management manNTPBangkok");
  e = document.getElementById("manNTPBeijing");
  e.innerHTML = _("management manNTPBeijing");
  e = document.getElementById("manNTPIrkutsk");
  e.innerHTML = _("management manNTPIrkutsk");
  e = document.getElementById("manNTPSingapore");
  e.innerHTML = _("management manNTPSingapore");
  e = document.getElementById("manNTPTaipei");
  e.innerHTML = _("management manNTPTaipei");
  e = document.getElementById("manNTPTokyo");
  e.innerHTML = _("management manNTPTokyo");
  e = document.getElementById("manNTPYakutsk");
  e.innerHTML = _("management manNTPYakutsk");
  e = document.getElementById("manNTPVladivostok");
  e.innerHTML = _("management manNTPVladivostok");
  e = document.getElementById("manNTPCanberra");
  e.innerHTML = _("management manNTPCanberra");
  e = document.getElementById("manNTPMagadan");
  e.innerHTML = _("management manNTPMagadan");
  e = document.getElementById("manNTPAuckland");
  e.innerHTML = _("management manNTPAuckland");
  e = document.getElementById("manNTPFiji");
  e.innerHTML = _("management manNTPFiji");
	e = document.getElementById("mantitle");
	e.innerHTML = _("login loginmsg");
	e = document.getElementById("manpassword");
	e.innerHTML = _("login passmsg");
	e = document.getElementById("manlogin");
	e.innerHTML = _("login login");

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
	e = document.getElementById("checkpassword");
	e.innerHTML = _("check_password");
	e = document.getElementById("timezone");
	e.innerHTML = _("time_zone");
	e = document.getElementById("ChangeUnit");
	e.innerHTML = _("Change_Unit");
	e = document.getElementById("Changelocation");
	e.innerHTML = _("Change_location");
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
  <ul><li class="modelname">NBG4615</li><li class="welcome_text" id="mantitle">Welcome to the router configuration interface. 
Enter the password and click 'Login'.</li>
  <li class="welcome_text"><font color="#FF0000" id="checkpassword">Error : Wrong Password ! Please try again.</font></li>
	<li>
  <table width="350" border="0" cellpadding="0" cellspacing="0">
    <tr>
      <td>&nbsp;</td>
      <td width="92">&nbsp;</td>
      <td width="117">&nbsp;</td>
      <td width="137">&nbsp;</td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td><span class="password_word" id="manAdmPasswd">Password</span><span class="password_word" id="manAdmPasswd"> :&nbsp</span></td>
      <td><input name="LoginPassword" size="15" maxlength="30" value="1234" type="password" class=""/></td>
      <td><div><ul><li class="button3"><input type="hidden" name="set_flag" size="1" value="0"><a href="javascript: document.web_login.submit();"><span id=manlogin>Login</span></a></li></ul></div></td>
    </tr>
    <tr>
      <td>&nbsp;</td>
      <td colspan="3"><span class="m" id=manpassword>( max. 30 alphanumeric, printable characters and no spaces )</span></td>
      </tr>
    <tr>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
      <td>&nbsp;</td>
    </tr>
    
    <tr>
      <td>&nbsp;</td>
      <td><span class="password_word" id="manSelectLang">Language</span><span class="password_word"> :&nbsp</span></td>
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
<div id="set_pupup1">
<div class="close1"><a href="#" onclick="MM_showHideLayers('set_pupup1','','hide')"></a></div>
  <ul><li style="margin-top:10px; font-weight:bold;" id="ChangeUnit">Change Unit</li><li>
  <select name="degree_select">
    <option value='f' id=tempc_select>&deg;F</option>
    <option value='c' id=tempf_select>&deg;C</option>
  </select>
</li><li style="font-weight:bold;" id="Changelocation">Change location
</li><li class="list"><select name="city_select" size="2">
  <option value='AUXX0025' id='Austria - Vienna'>Austria - Vienna</option>
  <option value='CHXX0008' id='China - Beijing'>China - Beijing</option>
  <option value='CSXX0009' id='Costa Rica - San Jose'>Costa Rica - San Jose</option>
  <option value='EZXX0012' id='Czech - Prague'>Czech - Prague</option>
  <option value='DAXX0009' id='Denmark - Copenhagen'>Denmark - Copenhagen</option>
  <option value='FIXX0002' id='Finland - Helsinki'>Finland - Helsinki</option>
  <option value='FRXX0076' id='France - Paris'>France - Paris</option>
  <option value='GMXX0007' id='Germany - Berlin'>Germany - Berlin</option>
  <option value='GRXX0004' id='Greece - Athens'>Greece - Athens</option>
  <option value='INXX0096' id='India - New Delhi'>India - New Delhi</option>
  <option value='IDXX0022' id='Indonesia - Jakarta'>Indonesia - Jakarta</option>
  <option value='ITXX0067' id='Italy - Roma'>Italy - Roma</option>
  <option value='JAXX0085' id='Japan - Tokyo'>Japan - Tokyo</option>
  <option value='MYXX0008' id='Malaysia - Kuala Lumpur'>Malaysia - Kuala Lumpur</option>
  <option value='NLXX0002' id='Netherlands - Amsterdam'>Netherlands - Amsterdam</option>
  <option value='NOXX0029' id='Norway - Oslo'>Norway - Oslo</option>
  <option value='RSXX0063' id='Russia - Moscow'>Russia - Moscow</option>
  <option value='SNXX0006' id='Singapore - Singapore'>Singapore - Singapore</option>
  <option value='SPXX0050' id='Spain - Madrid'>Spain - Madrid</option>
  <option value='SWXX0031' id='Sweden - Stockholm'>Sweden - Stockholm</option>
  <option value='SZXX0006' id='Switzerland - Bern'>Switzerland - Bern</option>
  <option value='TWXX0021' id='Taiwan - Taipei'>Taiwan - Taipei</option>
  <option value='THXX0002' id='Thailand - Bangkok'>Thailand - Bangkok</option>
  <option value='TUXX0002' id='Turkey - Ankara'>Turkey - Ankara</option>
  <option value='UKXX0085' id='UK - London'>UK - London</option>
  <option value='UKXX1428' id='UK - Greenwich'>UK - Greenwich</option>
  <option value='USNY0996' id='USA - New York'>USA - New York</option>
  <option value='USDC0001' id='USA - Washington D.C.'>USA - Washington D.C.</option>
  <option value='USCA0638' id='USA - Los Angeles'>USA - Los Angeles</option>
  <option value='VMXX0006' id='Vietnam - Hanoi'>Vietnam - Hanoi</option>
  </select>
</li><li class="button3">
  <div align="right" style="margin-right:10px;"><a href="javascript: document.web_login.set_flag.value='1'; document.web_login.submit();"><span id="Finish">Finish</span></a>  </div>
</li>
  </ul></div>
  <div id="set_pupup2">
<div class="close1"><a href="#" onclick="MM_showHideLayers('set_pupup2','','hide')"></a></div>
  <ul><li style="font-weight:bold; margin-top:10px;" id="timezone">Change time zone
</li><li class="list"><select name="time_zone" size="2">
                        <option value="UCT_-11" id="manNTPMidIsland">(GMT-11:00) Midway Island, Samoa</option>
                        <option value="UCT_-10" id="manNTPHawaii">(GMT-10:00) Hawaii</option>
                        <option value="NAS_-09" id="manNTPAlaska">(GMT-09:00) Alaska</option>
                        <option value="PST_-08" id="manNTPPacific">(GMT-08:00) Pacific Time (US &amp; Canada), Tijuana</option>
                        <option value="MST_-07" id="manNTPArizona">(GMT-07:00) Arizona</option>
                        <option value="MST_-07" id="manNTPMountain">(GMT-07:00) Mountain Time (US &amp; Canada)</option>
                        <option value="CST_-06" id="manNTPCentralAmerica">(GMT-06:00) Central America</option>
                        <option value="UCT_-06" id="manNTPCentralTime">(GMT-06:00) Central Time (US &amp; Canada)</option>
                        <option value="UCT_-05" id="manNTPBogota">(GMT-05:00) Bogota, Lima, Quito</option>
                        <option value="EST_-05" id="manNTPEasternTime">(GMT-05:00) Eastern Time (US &amp; Canada)</option>
                        <option value="AST_-04" id="manNTPAltlantic">(GMT-04:00) Altlantic Time (Canada)</option>
                        <option value="UCT_-04" id="manNTPCaracas">(GMT-04:00) Caracas, La Poz, Santiago</option>
                        <option value="UCT_-03" id="manNTPBrasilia">(GMT-03:00) Brasilia, Greenland</option>
                        <option value="EBS_-03" id="manNTPBuenos">(GMT-03:00) Buenos Aires, Georgetown</option>
                        <option value="NOR_-02" id="manNTPMid-Atlantic">(GMT-02:00) Mid-Atlantic</option>
                        <option value="EUT_-01" id="manNTPAzores">(GMT-01:00) Azores, Cape Verde Is.</option>
                        <option value="UCT_000" id="manNTPCasablanca">(GMT) Casablanca, Monrovia</option>
                        <option value="GMT_000" id="manNTPGreenwich">(GMT) Greenwich Mean Time : Dublin, Edinburgh, Lisbon, London</option>
                        <option value="MET_001" id="manNTPAmsterdam">(GMT+01:00) Amsterdam, Berlin, Bern, Rome, Stockholm, Vienna</option>
                        <option value="MEZ_001" id="manNTPBelgrade">(GMT+01:00) Belgrade, Bratislava, Budapest, Ljubljana, Prague</option>
                        <option value="UCT_001" id="manNTPBrussels">(GMT+01:00) Brussels, Copenhagen, Madrid, Paris</option>
                        <option value="EET_002" id="manNTPAthens">(GMT+02:00) Athens, Beirut, Istanbul, Minsk</option>
                        <option value="SAS_002" id="manNTPBucharest">(GMT+02:00) Bucharest, Cairo, Harare, Pretoria</option>
                        <option value="IST_003" id="manNTPBaghdad">(GMT+03:00) Baghdad, Kuwait, Riyadh, Nairobi</option>
                        <option value="MSK_003" id="manNTPMoscow">(GMT+03:00) Moscow, St. Petersburg, Volgograd</option>
                        <option value="UCT_004" id="manNTPMuscat">(GMT+04:00) Abu Dhabi, Muscat</option>
                        <option value="UCT_005" id="manNTPEkaterinburg">(GMT+05:00) Ekaterinburg</option>
                        <option value="UCT_006" id="manNTPAlmaty">(GMT+06:00) Almaty, Novosibirsk</option>
                        <option value="UCT_007" id="manNTPBangkok">(GMT+07:00) Bangkok, Hanoi, Jakarta</option>
                        <option value="CST_008" id="manNTPBeijing">(GMT+08:00) Beijing, Chongqing, Hong Kong, Urumqi</option>
                        <option value="CCT_008" id="manNTPIrkutsk">(GMT+08:00) Irkutsk, Ulaan Bataar</option>
                        <option value="SST_008" id="manNTPSingapore">(GMT+08:00) Kuala Lumpur, Singapore</option>
                        <option value="AWS_008" id="manNTPTaipei">(GMT+08:00) Perth, Taipei</option>
                        <option value="JST_009" id="manNTPTokyo">(GMT+09:00) Osaka, Sapporo, Tokyo</option>
                        <option value="KST_009" id="manNTPYakutsk">(GMT+09:00) Seoul, Yakutsk</option>
                        <option value="UCT_010" id="manNTPVladivostok">(GMT+10:00) Brisbane, Vladivostok</option>
                        <option value="AES_010" id="manNTPCanberra">(GMT+10:00) Canberra, Melbourne, Sydney</option>
                        <option value="UCT_011" id="manNTPMagadan">(GMT+11:00) Magadan, Solomon Is., New Caledonia</option>
                        <option value="UCT_012" id="manNTPAuckland">(GMT+12:00) Auckland, Wellington</option>
                        <option value="NZS_012" id="manNTPFiji">(GMT+12:00) Fiji, Kamchatka, Marshall Is.</option>
  </select>
</li><li class="button3">
  <div align="right" style="margin-right:10px;"><a href="javascript: document.web_login.set_flag.value='2'; document.web_login.submit();"><span id="Finish">Finish</span></a>  </div>
</li>
  </ul></div>
<span id=nointernet_msg class="off">
<div id='nointernet'>Please connect Internet to see the weather.</div>
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
	if (tz == "UCT_-11")
		document.web_login.time_zone.options.selectedIndex = 0;
	else if (tz == "UCT_-10")
		document.web_login.time_zone.options.selectedIndex = 1;
	else if (tz == "NAS_-09")
		document.web_login.time_zone.options.selectedIndex = 2;
	else if (tz == "PST_-08")
		document.web_login.time_zone.options.selectedIndex = 3;
	else if (tz == "MST_-07")
		document.web_login.time_zone.options.selectedIndex = 4;
	else if (tz == "MST_-07")
		document.web_login.time_zone.options.selectedIndex = 5;
	else if (tz == "CST_-06")
		document.web_login.time_zone.options.selectedIndex = 6;
	else if (tz == "UCT_-06")
		document.web_login.time_zone.options.selectedIndex = 7;
	else if (tz == "UCT_-05")
		document.web_login.time_zone.options.selectedIndex = 8;
	else if (tz == "EST_-05")
		document.web_login.time_zone.options.selectedIndex = 9;
	else if (tz == "AST_-04")
		document.web_login.time_zone.options.selectedIndex = 10;
	else if (tz == "UCT_-04")
		document.web_login.time_zone.options.selectedIndex = 11;
	else if (tz == "UCT_-03")
		document.web_login.time_zone.options.selectedIndex = 12;
	else if (tz == "EBS_-03")
		document.web_login.time_zone.options.selectedIndex = 13;
	else if (tz == "NOR_-02")
		document.web_login.time_zone.options.selectedIndex = 14;
	else if (tz == "EUT_-01")
		document.web_login.time_zone.options.selectedIndex = 15;
	else if (tz == "UCT_000")
		document.web_login.time_zone.options.selectedIndex = 16;
	else if (tz == "GMT_000")
		document.web_login.time_zone.options.selectedIndex = 17;
	else if (tz == "MET_001")
		document.web_login.time_zone.options.selectedIndex = 18;
	else if (tz == "MEZ_001")
		document.web_login.time_zone.options.selectedIndex = 19;
	else if (tz == "UCT_001")
		document.web_login.time_zone.options.selectedIndex = 20;
	else if (tz == "EET_002")
		document.web_login.time_zone.options.selectedIndex = 21;
	else if (tz == "SAS_002")
		document.web_login.time_zone.options.selectedIndex = 22;
	else if (tz == "IST_003")
		document.web_login.time_zone.options.selectedIndex = 23;
	else if (tz == "MSK_003")
		document.web_login.time_zone.options.selectedIndex = 24;
	else if (tz == "UCT_004")
		document.web_login.time_zone.options.selectedIndex = 25;
	else if (tz == "UCT_005")
		document.web_login.time_zone.options.selectedIndex = 26;
	else if (tz == "UCT_006")
		document.web_login.time_zone.options.selectedIndex = 27;
	else if (tz == "UCT_007")
		document.web_login.time_zone.options.selectedIndex = 28;
	else if (tz == "CST_008")
		document.web_login.time_zone.options.selectedIndex = 29;
	else if (tz == "CCT_008")
		document.web_login.time_zone.options.selectedIndex = 30;
	else if (tz == "SST_008")
		document.web_login.time_zone.options.selectedIndex = 31;
	else if (tz == "AWS_008")
		document.web_login.time_zone.options.selectedIndex = 32;
	else if (tz == "JST_009")
		document.web_login.time_zone.options.selectedIndex = 33;
	else if (tz == "KST_009")
		document.web_login.time_zone.options.selectedIndex = 34;
	else if (tz == "UCT_010")
		document.web_login.time_zone.options.selectedIndex = 35;
	else if (tz == "AES_010")
		document.web_login.time_zone.options.selectedIndex = 36;
	else if (tz == "UCT_011")
		document.web_login.time_zone.options.selectedIndex = 37;
	else if (tz == "UCT_012")
		document.web_login.time_zone.options.selectedIndex = 38;
	else if (tz == "NZS_012")
		document.web_login.time_zone.options.selectedIndex = 39;
		
	var defree = "<% getCfgGeneral(1, "WeatherDegree"); %>";
	if (defree == "f")
		document.web_login.degree_select.options.selectedIndex = 0;
	if (defree == "c")
		document.web_login.degree_select.options.selectedIndex = 1;
		
	var city = "<% getCfgGeneral(1, "WeatherCity"); %>";
	if (city == "TWXX0021")
		document.web_login.city_select.options.selectedIndex = 0;
	if (city == "CHXX0049")
		document.web_login.city_select.options.selectedIndex = 1;
	if (city == "USNY0996")
		document.web_login.city_select.options.selectedIndex = 2;

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
