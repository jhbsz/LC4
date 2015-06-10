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

function allcheck()
{
 showWebMessage(2, "");
 if (document.NTP.mten_ServiceType[0].checked == true)
   {
     return curronttimeFormCheck(); 
   }
   
   if (document.NTP.enabledaylight.checked)
   {
     return DSTFormCheck();
   }
}

function curronttimeFormCheck()
{
	if (!((document.NTP.current_hour.value != "") && (document.NTP.current_hour.value >= 0)&& (document.NTP.current_hour.value <= 23)))
		{
		alert("You must input 0~23 hours!!");
		return false;
	  }
	if (!((document.NTP.current_Min.value != "") && (document.NTP.current_Min.value >= 0)&& (document.NTP.current_Min.value <= 59)))
		{
		alert("You must input 0~59 minutes!!");
		return false;
	  } 
	if (!((document.NTP.current_Sec.value != "") && (document.NTP.current_Sec.value >= 0)&& (document.NTP.current_Sec.value <= 60)))
		{
		alert("You must input 0~60 seconds!!");
		return false;
	  }  
	if (!((document.NTP.current_Year.value != "") && (document.NTP.current_Year.value >= 1970)&& (document.NTP.current_Year.value <= 2037)))
		{
		alert("You must input 1970~2037 years!!");
		return false;
	  } 
	if (!((document.NTP.current_Mon.value != "") && (document.NTP.current_Mon.value > 0)&& (document.NTP.current_Mon.value <= 12)))
		{
		alert("You must input 1~12 month!!");
		return false;
	  } 
	if ((document.NTP.current_Mon.value == 1)||(document.NTP.current_Mon.value == 3)||(document.NTP.current_Mon.value == 5)||(document.NTP.current_Mon.value == 7)||(document.NTP.current_Mon.value == 8)||(document.NTP.current_Mon.value == 10)||(document.NTP.current_Mon.value == 12))
		{
		if(!((document.NTP.current_Day.value != "") && (document.NTP.current_Day.value > 0)&& (document.NTP.current_Day.value <= 31)))
		 {
		   alert("You must input 1~31 days!");
		   return false;
		 }
	  }    
  if ((document.NTP.current_Mon.value == 4)||(document.NTP.current_Mon.value == 6)||(document.NTP.current_Mon.value == 9)||(document.NTP.current_Mon.value == 11))
		{
		if(!((document.NTP.current_Day.value != "") && (document.NTP.current_Day.value > 0)&& (document.NTP.current_Day.value <= 30)))
		 {
		   alert("You must input 1~30 days!");
		   return false;
		 }
	  }
	if (document.NTP.current_Mon.value == 2)
		{
		if(((document.NTP.current_Year.value)%4)==0)
		{
		 		if(!((document.NTP.current_Day.value != "") && (document.NTP.current_Day.value > 0)&& (document.NTP.current_Day.value <= 29)))
		 {
		   alert("You must input 1~29 days!");
		   return false;
		 }
		}
		else
		{
		if(!((document.NTP.current_Day.value != "") && (document.NTP.current_Day.value > 0)&& (document.NTP.current_Day.value <= 28)))
		 {
		   alert("You must input 1~28 days!");
		   return false;
		 }
	  } 
	  } 
	//modifyform();
	/* aron marked   
	if (document.NTP.enabledaylight.checked)
        {
            return DSTFormCheck();
        }
        */
        showWebMessage(2, "");   
	return true;
}	
function modifyform()
{
	var minutes = document.NTP.current_Min.value;
	var hours = document.NTP.current_hour.value;
	var month = document.NTP.current_Mon.value;
	var day = document.NTP.current_Day.value;

	if((minutes < 10)&&(minutes >= 0)&&(document.NTP.current_Min.value!="00")&&(document.NTP.current_Min.value!="01")&&(document.NTP.current_Min.value!="02")&&(document.NTP.current_Min.value!="03")
	&&(document.NTP.current_Min.value!="04")&&(document.NTP.current_Min.value!="05")&&(document.NTP.current_Min.value!="06")&&(document.NTP.current_Min.value!="07")
	&&(document.NTP.current_Min.value!="08")&&(document.NTP.current_Min.value!="09"))
		document.NTP.current_Min.value = "0" + minutes;
		
	if((hours < 10)&&(hours >= 0)&&(document.NTP.current_hour.value!="00")&&(document.NTP.current_hour.value!="01")&&(document.NTP.current_hour.value!="02")&&(document.NTP.current_hour.value!="03")
	&&(document.NTP.current_hour.value!="04")&&(document.NTP.current_hour.value!="05")&&(document.NTP.current_hour.value!="06")&&(document.NTP.current_hour.value!="07")&&(document.NTP.current_hour.value!="08")
	&&(document.NTP.current_hour.value!="09"))
		document.NTP.current_hour.value = "0" + hours;

	if((month < 10)&&(month > 0)&&(document.NTP.current_Mon.value!="01")&&(document.NTP.current_Mon.value!="02")&&(document.NTP.current_Mon.value!="03")
	&&(document.NTP.current_Mon.value!="04")&&(document.NTP.current_Mon.value!="05")&&(document.NTP.current_Mon.value!="06")&&(document.NTP.current_Mon.value!="07")
	&&(document.NTP.current_Mon.value!="08")&&(document.NTP.current_Mon.value!="09"))
		document.NTP.current_Mon.value = "0" + month;

	if((day < 10)&&(day > 0)&&(document.NTP.current_Day.value!="01")&&(document.NTP.current_Day.value!="02")&&(document.NTP.current_Day.value!="03")&&(document.NTP.current_Day.value!="04")&&(document.NTP.current_Day.value!="05")
	&&(document.NTP.current_Day.value!="06")&&(document.NTP.current_Day.value!="07")&&(document.NTP.current_Day.value!="08")&&(document.NTP.current_Day.value!="09"))
		document.NTP.current_Day.value = "0" + day;	
}
function timeserver_enable_switch()
{
  
	if (document.NTP.mten_ServiceType[1].checked == true)
	{
		document.NTP.current_Min.disabled = true;
		document.NTP.current_Sec.disabled = true;
		document.NTP.current_hour.disabled = true;
		document.NTP.current_Year.disabled = true;
		document.NTP.current_Mon.disabled = true;
		document.NTP.current_Day.disabled = true;
		document.NTP.time_ServerType[0].disabled = false;
		document.NTP.time_ServerType[1].disabled = false;
		document.NTP.NTPServerIP.disabled = false;
	  if (document.NTP.time_ServerType[1].checked == true)
     {
      document.NTP.NTPServerIP.disabled = false;
     }
    else
     {
      document.NTP.NTPServerIP.disabled = true;
     } 	
	}
	else
	{
		document.NTP.current_Min.disabled = false;
		document.NTP.current_Sec.disabled = false;
		document.NTP.current_hour.disabled = false;
		document.NTP.current_Year.disabled = false;
		document.NTP.current_Mon.disabled = false;
		document.NTP.current_Day.disabled = false;
		document.NTP.time_ServerType[0].disabled = true;
		document.NTP.time_ServerType[1].disabled = true;
		document.NTP.NTPServerIP.disabled = true;
	}
}

function daylightsaving()
{
	var now_sec;
	var dl_start_sec;
	var dl_end_sec;
	document.NTP.dst_startMon.value = "<% getCfgGeneral(1, "startMon"); %>";
	document.NTP.dst_startDay.value = "<% getCfgGeneral(1, "startDay"); %>";
	document.NTP.dst_startclock.value = "<% getCfgGeneral(1, "startclock"); %>";
	document.NTP.dst_endMon.value = "<% getCfgGeneral(1, "endMon"); %>";
	document.NTP.dst_endDay.value = "<% getCfgGeneral(1, "endDay"); %>";
	document.NTP.dst_endclock.value = "<% getCfgGeneral(1, "endclock"); %>";
	
	//current time
	 now_Hour=parseInt(document.NTP.mtenCurrent_Hour.value,10);
   now_Min=parseInt(document.NTP.mtenCurrent_Min.value,10);
   now_Sec=parseInt(document.NTP.mtenCurrent_Sec.value,10);
   now_Year=parseInt(document.NTP.mtenCurrent_Year.value,10);
   now_Mon=parseInt(document.NTP.mtenCurrent_Mon.value,10);
   now_Day=parseInt(document.NTP.mtenCurrent_Day.value,10);
  //dst start time
   dst_start_Mon=parseInt(document.NTP.dst_startMon.value,10);
   dst_start_Day=parseInt(document.NTP.dst_startDay.value,10);
   dst_start_Hour=parseInt(document.NTP.dst_startclock.value,10);
   
   //dst end time
   dst_end_Mon=parseInt(document.NTP.dst_endMon.value,10);
   dst_end_Day=parseInt(document.NTP.dst_endDay.value,10);
   dst_end_Hour=parseInt(document.NTP.dst_endclock.value,10);
   
   now_long_sec = Date.UTC(now_Year, now_Mon, now_Day, now_Hour, now_Min, now_Sec);
   dst_start_long_sec = Date.UTC(now_Year, dst_start_Mon, dst_start_Day, dst_start_Hour, now_Min, now_Sec);
   dst_end_long_sec = Date.UTC(now_Year, dst_end_Mon, dst_end_Day, dst_end_Hour, now_Min, now_Sec);
   
   
//DST start
   if((now_long_sec >= dst_start_long_sec)&&(now_long_sec < dst_end_long_sec))
   {
     systemhour = document.NTP.mtenCurrent_Hour.value;
     Hour = parseInt(systemhour,10);
     systemDay = document.NTP.mtenCurrent_Day.value;
     Day = parseInt(systemDay,10);
     systemmon = document.NTP.mtenCurrent_Mon.value;
     Mon = parseInt(systemmon,10);
     systemyear = document.NTP.mtenCurrent_Year.value;
     Year = parseInt(systemyear,10);
     
     if(Hour==23)
     {
       
       if(((Mon==2)&&(Day==28))||((Mon==2)&&(Day==29)))
       {
         Day = 0;
         Mon = (Mon+1);
       }
       if(((Mon==4)||(Mon==6)||(Mon==9)||(Mon==11))&&(Day==30))
       {
         Day = 0;
         Mon = (Mon+1);      
       }
       if(((Mon==1)||(Mon==3)||(Mon==5)||(Mon==7)||(Mon==8)||(Mon==10))&&(Day==31))
       {
         Day = 0;
         Mon = (Mon+1);      
       }
       if((Mon==12)&&(Day==31))
       {
         Day = 0;
         Mon = 0;
         Mon = (Mon+1);
         Year = (Year+1);
       }
     Hour=(0-1);
     Day=(Day+1);
      
     }
          
     Hour=(Hour+1);
     document.NTP.mtenCurrent_Year.value = Year;
     document.NTP.mtenCurrent_Mon.value = Mon;
     document.NTP.mtenCurrent_Day.value = Day;
     document.NTP.mtenCurrent_Hour.value = Hour;
  
   }
}

function initValue()
{
	var tz = "<% getCfgGeneral(1, "TZ"); %>";
	var timedatatype = "<% getCfgGeneral(1, "TIME_servtype"); %>";
	var sertvype = "<% getCfgGeneral(1, "NTP_servtype"); %>";
	enable_dst = "<% getCfgGeneral(1, "enabledaylight"); %>";
	
	if(enable_dst=="1")
	{
	document.NTP.enabledaylight.checked = "true";
	daylightsaving();
	}
	enableDST();
	initDateAndTime();

	var e = document.getElementById("title");
	e.innerHTML = _("management title");

	//for manul time//
	e = document.getElementById("current_hourr");
  e.value = parseInt(document.NTP.mtenCurrent_Hour.value,10);
  e = document.getElementById("current_Minn");
  e.value = parseInt(document.NTP.mtenCurrent_Min.value,10);
  e = document.getElementById("current_Secc");
  e.value = parseInt(document.NTP.mtenCurrent_Sec.value,10);
  e = document.getElementById("current_Yearr");
  e.value = parseInt(document.NTP.mtenCurrent_Year.value,10);
  e = document.getElementById("current_Monn");
  e.value = parseInt(document.NTP.mtenCurrent_Mon.value,10);
  e = document.getElementById("current_Dayy");
  e.value = parseInt(document.NTP.mtenCurrent_Day.value,10);
	//for manul time//

	e = document.getElementById("CurrentTime");
	e.innerHTML = _("management CurrentTime");
	e = document.getElementById("CurrentDate");
  e.innerHTML = _("management CurrentDate");
  e = document.getElementById("title_1");
  e.innerHTML = _("management title_1");
  e = document.getElementById("Manual");
  e.innerHTML = _("management Manual");
  e = document.getElementById("NewTime");
  e.innerHTML = _("management NewTime");
  e = document.getElementById("NewDate");
  e.innerHTML = _("management NewDate");
  e = document.getElementById("TimeServer");
  e.innerHTML = _("management TimeServer");
  e = document.getElementById("Auto");
  e.innerHTML = _("management Auto");
  e = document.getElementById("UserDefined");
  e.innerHTML = _("management UserDefined");
  e = document.getElementById("title_2");
  e.innerHTML = _("management title_2");
  e = document.getElementById("TimeZone");
  e.innerHTML = _("management TimeZone");
  
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
 
  e = document.getElementById("Apply");
  e.value = _("management Apply");
  e = document.getElementById("Reset");
  e.value = _("management Reset");
  e = document.getElementById("DaylightStart");
  e.innerHTML = _("management DaylightStart");
  e = document.getElementById("Daylight");
  e.innerHTML = _("management Daylight");
  e = document.getElementById("DaylightStartat");
  e.innerHTML = _("management DaylightStartat");
  e = document.getElementById("DaylightStartclock");
  e.innerHTML = _("management DaylightStartclock");
  e = document.getElementById("DaylightEnd");
  e.innerHTML = _("management DaylightEnd");
  e = document.getElementById("Daylightendat");
  e.innerHTML = _("management Daylightendat"); 
  e = document.getElementById("Daylightendclock");
  e.innerHTML = _("management Daylightendclock");
  
	if (sertvype == "0")
	{
	  document.NTP.time_ServerType[0].checked = true;
	}
	else
	{
	  document.NTP.time_ServerType[1].checked = true;
	}
	if (timedatatype == "0")
	{
	 document.NTP.mten_ServiceType[0].checked = true;
	 timeserver_enable_switch();
	}
	else
	{
	  document.NTP.mten_ServiceType[1].checked = true;
	  timeserver_enable_switch();
	}
         
        /*
	if (tz == "UCT_-11")
		document.NTP.time_zone.options.selectedIndex = 0;
	else if (tz == "UCT_-10")
		document.NTP.time_zone.options.selectedIndex = 1;
	else if (tz == "NAS_-09")
		document.NTP.time_zone.options.selectedIndex = 2;
	else if (tz == "PST_-08")
		document.NTP.time_zone.options.selectedIndex = 3;
	else if (tz == "MST_-07")
		document.NTP.time_zone.options.selectedIndex = 4;
	else if (tz == "MST_-07")
		document.NTP.time_zone.options.selectedIndex = 5;
	else if (tz == "CST_-06")
		document.NTP.time_zone.options.selectedIndex = 6;
	else if (tz == "UCT_-06")
		document.NTP.time_zone.options.selectedIndex = 7;
	else if (tz == "UCT_-05")
		document.NTP.time_zone.options.selectedIndex = 8;
	else if (tz == "EST_-05")
		document.NTP.time_zone.options.selectedIndex = 9;
	else if (tz == "AST_-04")
		document.NTP.time_zone.options.selectedIndex = 10;
	else if (tz == "UCT_-04")
		document.NTP.time_zone.options.selectedIndex = 11;
	else if (tz == "UCT_-03")
		document.NTP.time_zone.options.selectedIndex = 12;
	else if (tz == "EBS_-03")
		document.NTP.time_zone.options.selectedIndex = 13;
	else if (tz == "NOR_-02")
		document.NTP.time_zone.options.selectedIndex = 14;
	else if (tz == "EUT_-01")
		document.NTP.time_zone.options.selectedIndex = 15;
	else if (tz == "UCT_000")
		document.NTP.time_zone.options.selectedIndex = 16;
	else if (tz == "GMT_000")
		document.NTP.time_zone.options.selectedIndex = 17;
	else if (tz == "MET_001")
		document.NTP.time_zone.options.selectedIndex = 18;
	else if (tz == "MEZ_001")
		document.NTP.time_zone.options.selectedIndex = 19;
	else if (tz == "UCT_001")
		document.NTP.time_zone.options.selectedIndex = 20;
	else if (tz == "EET_002")
		document.NTP.time_zone.options.selectedIndex = 21;
	else if (tz == "SAS_002")
		document.NTP.time_zone.options.selectedIndex = 22;
	else if (tz == "IST_003")
		document.NTP.time_zone.options.selectedIndex = 23;
	else if (tz == "MSK_003")
		document.NTP.time_zone.options.selectedIndex = 24;
	else if (tz == "UCT_004")
		document.NTP.time_zone.options.selectedIndex = 25;
	else if (tz == "UCT_005")
		document.NTP.time_zone.options.selectedIndex = 26;
	else if (tz == "UCT_006")
		document.NTP.time_zone.options.selectedIndex = 27;
	else if (tz == "UCT_007")
		document.NTP.time_zone.options.selectedIndex = 28;
	else if (tz == "CST_008")
		document.NTP.time_zone.options.selectedIndex = 29;
	else if (tz == "CCT_008")
		document.NTP.time_zone.options.selectedIndex = 30;
	else if (tz == "SST_008")
		document.NTP.time_zone.options.selectedIndex = 31;
	else if (tz == "AWS_008")
		document.NTP.time_zone.options.selectedIndex = 32;
	else if (tz == "JST_009")
		document.NTP.time_zone.options.selectedIndex = 33;
	else if (tz == "KST_009")
		document.NTP.time_zone.options.selectedIndex = 34;
	else if (tz == "UCT_010")
		document.NTP.time_zone.options.selectedIndex = 35;
	else if (tz == "AES_010")
		document.NTP.time_zone.options.selectedIndex = 36;
	else if (tz == "UCT_011")
		document.NTP.time_zone.options.selectedIndex = 37;
	else if (tz == "UCT_012")
		document.NTP.time_zone.options.selectedIndex = 38;
	else if (tz == "NZS_012")
		document.NTP.time_zone.options.selectedIndex = 39;
        */
        if (tz == "UTC12")
		document.NTP.time_zone.options.selectedIndex = 0;
	else if (tz == "UTC11")
		document.NTP.time_zone.options.selectedIndex = 1;
	else if (tz == "UTC10")
		document.NTP.time_zone.options.selectedIndex = 2;
	else if (tz == "NAST9NADT,M3.2.0/2,M11.1.0/2")
		document.NTP.time_zone.options.selectedIndex = 3;
	else if (tz == "PST8PDT,M3.2.0/2,M11.1.0/2")
		document.NTP.time_zone.options.selectedIndex = 4;
	else if (tz == "UTC7")
		document.NTP.time_zone.options.selectedIndex = 5;
	else if (tz == "MST7MDT,M3.2.0/2,M11.1.0/2")
		document.NTP.time_zone.options.selectedIndex = 6;
	else if (tz == "UTC6")
		document.NTP.time_zone.options.selectedIndex = 7;
	else if (tz == "CST6CDT,M3.2.0/2,M11.1.0/2")
		document.NTP.time_zone.options.selectedIndex = 8;
	else if (tz == "UTC5")
		document.NTP.time_zone.options.selectedIndex = 9;
	else if (tz == "EST5EDT,M3.2.0/2,M11.1.0/2")
		document.NTP.time_zone.options.selectedIndex = 10;
	else if (tz == "UTC4")
		document.NTP.time_zone.options.selectedIndex = 11;
	else if (tz == "AST4ADT,M3.2.0/2,M11.1.0/2")
		document.NTP.time_zone.options.selectedIndex = 12;
	else if (tz == "BRWST4BRWDT,M10.3.0/0,M2.5.0/0")
		document.NTP.time_zone.options.selectedIndex = 13;
	else if (tz == "NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01")
		document.NTP.time_zone.options.selectedIndex = 14;
	else if (tz == "WGST3WGDT,M3.5.6/22,M10.5.6/23")
		document.NTP.time_zone.options.selectedIndex = 15;
	else if (tz == "BRST3BRDT,M10.3.0/0,M2.5.0/0")
		document.NTP.time_zone.options.selectedIndex = 16;
	else if (tz == "UTC3")
		document.NTP.time_zone.options.selectedIndex = 17;
	else if (tz == "UTC2")
		document.NTP.time_zone.options.selectedIndex = 18;
	else if (tz == "STD1DST,M3.5.0/2,M10.5.0/2")
		document.NTP.time_zone.options.selectedIndex = 19;
	else if (tz == "UTC0")
		document.NTP.time_zone.options.selectedIndex = 20;
	else if (tz == "GMT0BST,M3.5.0/2,M10.5.0/2")
		document.NTP.time_zone.options.selectedIndex = 21;
	else if (tz == "UTC-1")
		document.NTP.time_zone.options.selectedIndex = 22;
	else if (tz == "STD-1DST,M3.5.0/2,M10.5.0/2")
		document.NTP.time_zone.options.selectedIndex = 23;
	else if (tz == "UTC-2")
		document.NTP.time_zone.options.selectedIndex = 24;
	else if (tz == "STD-2DST,M3.5.0/2,M10.5.0/2")
		document.NTP.time_zone.options.selectedIndex = 25;
	else if (tz == "UTC-3")
		document.NTP.time_zone.options.selectedIndex = 26;
	else if (tz == "UTC-4")
		document.NTP.time_zone.options.selectedIndex = 27;
	else if (tz == "UTC-5")
		document.NTP.time_zone.options.selectedIndex = 28;
	else if (tz == "UTC-5:30")
		document.NTP.time_zone.options.selectedIndex = 29;
	else if (tz == "UTC-6")
		document.NTP.time_zone.options.selectedIndex = 30;
	else if (tz == "UTC-7")
		document.NTP.time_zone.options.selectedIndex = 31;
	else if (tz == "UTC-8")
		document.NTP.time_zone.options.selectedIndex = 32;
	else if (tz == "UTC-9")
		document.NTP.time_zone.options.selectedIndex = 33;
	else if (tz == "CST-9:30CST,M10.5.0/2,M3.5.0/3")
		document.NTP.time_zone.options.selectedIndex = 34;
	else if (tz == "UTC-10")
		document.NTP.time_zone.options.selectedIndex = 35;
	else if (tz == "STD-10DST,M10.5.0/2,M3.5.0/2")
		document.NTP.time_zone.options.selectedIndex = 36;
	else if (tz == "UTC-11")
		document.NTP.time_zone.options.selectedIndex = 37;
	else if (tz == "UTC-12")
		document.NTP.time_zone.options.selectedIndex = 38;
	else if (tz == "STD-12DST,M10.5.0/2,M3.5.0/2")
		document.NTP.time_zone.options.selectedIndex = 39;
}
function initDateAndTime()
        {
           Hour=parseInt(document.NTP.mtenCurrent_Hour.value,10);
           Min=parseInt(document.NTP.mtenCurrent_Min.value,10);
           Sec=parseInt(document.NTP.mtenCurrent_Sec.value,10);
           Year=parseInt(document.NTP.mtenCurrent_Year.value,10);
           Mon=parseInt(document.NTP.mtenCurrent_Mon.value,10);
           Day=parseInt(document.NTP.mtenCurrent_Day.value,10);
           document.getElementById("mtenCurrent_Time").innerHTML = DateAndTimeFormat(Hour) + ":"+ DateAndTimeFormat(Min) +":" + DateAndTimeFormat(Sec);
           document.getElementById("mtenCurrent_Date").innerHTML = Year + "-"+ DateAndTimeFormat(Mon) +"-" + DateAndTimeFormat(Day);
       }
function DateAndTimeFormat(num)
        {
                if(String(parseInt(num,10)).length==1)
                        return "0"+num;
                else
                        return num;
        }    
  var whichDayNum;
  var BigMon=new Array('1','3','5','7','8','10','12');
  var SmallMon=new Array('4','6','9','11');
  var Hour, Min, Sec;
  var Year, Mon, Day;

function IncreaseSec()
        {
        Sec = parseInt(document.NTP.mtenCurrent_Sec.value,10);
        Sec = Sec + 1;
        if ( Sec < 60 )
        {
            document.getElementById("mtenCurrent_Time").innerHTML = DateAndTimeFormat(Hour) + ":"+ DateAndTimeFormat(Min) +":" + DateAndTimeFormat(Sec);
            document.NTP.mtenCurrent_Sec.value = Sec;
            setTimeout('IncreaseSec()', 1000);
            return ;
        }

        document.NTP.mtenCurrent_Sec.value = 0;Sec=0;
        Min = parseInt(document.NTP.mtenCurrent_Min.value,10);
        Min = Min + 1;
        if ( Min < 60 )
        {
            document.getElementById("mtenCurrent_Time").innerHTML = DateAndTimeFormat(Hour) + ":"+ DateAndTimeFormat(Min) +":" + DateAndTimeFormat(Sec);
            document.NTP.mtenCurrent_Min.value = Min;
            setTimeout('IncreaseSec()', 1000);
            return;
        }

        document.NTP.mtenCurrent_Min.value = 0;Min=0;
        Hour = parseInt(document.NTP.mtenCurrent_Hour.value,10);
        Hour = Hour + 1;
        if ( Hour < 24 )
        {
            document.getElementById("mtenCurrent_Time").innerHTML = DateAndTimeFormat(Hour) + ":"+ DateAndTimeFormat(Min) +":" + DateAndTimeFormat(Sec);
            document.NTP.mtenCurrent_Hour.value = Hour;
            setTimeout('IncreaseSec()', 1000);
            return;
        }
        document.getElementById("mtenCurrent_Time").innerHTML = "00 : 00 : 00";
        document.NTP.mtenCurrent_Hour.value = 0;Hour=0;
        Day = parseInt(document.NTP.mtenCurrent_Day.value,10);
        Day = Day + 1;
        Mon = parseInt(document.NTP.mtenCurrent_Mon.value,10);
        Year = parseInt(document.NTP.mtenCurrent_Year.value,10);

                for(var i=0;i<BigMon.length;i++)
                        if(Mon.value==BigMon[i]) whichDayNum=31;
                for(var i=0;i<SmallMon.length;i++)
                        if(Mon.value==SmallMon[i]) whichDayNum=30;
                if((Year.value%4)==0)
                        whichDayNum=29;
                else
                        whichDayNum=28;

        if ( Day <= whichDayNum )
        {
            document.getElementById("mtenCurrent_Date").innerHTML = Year + "-"+ DateAndTimeFormat(Mon) +"-" + DateAndTimeFormat(Day);
            document.NTP.mtenCurrent_Day.value = Day;
            setTimeout('IncreaseSec()', 1000);
            return;
                }

        document.NTP.mtenCurrent_Day.value = 1;Day=1;
        Mon = parseInt(document.NTP.mtenCurrent_Mon.value,10);
        Mon = Mon + 1;

        if ( Mon <= 12 )
        {
            document.getElementById("mtenCurrent_Date").innerHTML = Year + "-"+ DateAndTimeFormat(Mon) +"-" + DateAndTimeFormat(Day);
            document.NTP.mtenCurrent_Mon.value = Mon;
            setTimeout('IncreaseSec()', 1000);
            return;
                }

        document.NTP.mtenCurrent_Mon.value = 1;Mon=1;
        Year = parseInt(document.NTP.mtenCurrent_Year.value,10);
        Year = Year + 1;

        if ( Year < 10000)
        {
            document.getElementById("mtenCurrent_Date").innerHTML = Year + "-"+ DateAndTimeFormat(Mon) +"-" + DateAndTimeFormat(Day);
            document.NTP.mtenCurrent_Year.value = Year;
            setTimeout('IncreaseSec()', 1000);
            return;
                }

        document.NTP.mtenCurrent_Year.value = 1900;Year=1900;

        this.location = "management_gordon.asp";
        return;
    }  

function enableDST()
{
if(document.NTP.enabledaylight.checked)
 {
 document.NTP.enabledaylight.value = "1";
document.NTP.dst_startMon.disabled = false;
document.NTP.dst_startDay.disabled = false;
document.NTP.dst_startclock.disabled = false;
document.NTP.dst_endMon.disabled = false;
document.NTP.dst_endDay.disabled = false;
document.NTP.dst_endclock.disabled = false;
 }
else
 {
document.NTP.enabledaylight.value = "0";  
document.NTP.dst_startMon.disabled = true;
document.NTP.dst_startDay.disabled = true;
document.NTP.dst_startclock.disabled = true;
document.NTP.dst_endMon.disabled = true;
document.NTP.dst_endDay.disabled = true;
document.NTP.dst_endclock.disabled = true;
 }
}
function DSTFormCheck()
{
	if ((!((document.NTP.dst_startclock.value != "") && (document.NTP.dst_startclock.value >= 0)&& (document.NTP.dst_startclock.value <= 23)))||(!((document.NTP.dst_endclock.value != "") && (document.NTP.dst_endclock.value >= 0)&& (document.NTP.dst_endclock.value <= 23))))
		{
		alert("You must input 0~23 hours!!");
		return false;
	  }

	if ((!((document.NTP.dst_startMon.value != "") && (document.NTP.dst_startMon.value > 0)&& (document.NTP.dst_startMon.value <= 12)))||(!((document.NTP.dst_endMon.value != "") && (document.NTP.dst_endMon.value > 0)&& (document.NTP.dst_endMon.value <= 12))))
		{
		alert("You must input 1~12 month!!");
		return false;
	  } 
	if (((document.NTP.dst_startMon.value == 1)||(document.NTP.dst_startMon.value == 3)||(document.NTP.dst_startMon.value == 5)||(document.NTP.dst_startMon.value == 7)||(document.NTP.dst_startMon.value == 8)||(document.NTP.dst_startMon.value == 10)||(document.NTP.dst_startMon.value == 12))||((document.NTP.dst_endMon.value == 1)||(document.NTP.dst_endMon.value == 3)||(document.NTP.dst_endMon.value == 5)||(document.NTP.dst_endMon.value == 7)||(document.NTP.dst_endMon.value == 8)||(document.NTP.dst_endMon.value == 10)||(document.NTP.dst_endMon.value == 12)))
		{
		if((!((document.NTP.dst_startDay.value != "") && (document.NTP.dst_startDay.value > 0)&& (document.NTP.dst_startDay.value <= 31)))||(!((document.NTP.dst_endDay.value != "") && (document.NTP.dst_endDay.value > 0)&& (document.NTP.dst_endDay.value <= 31))))
		 {
		   alert("You must input 1~31 days!");
		   return false;
		 }
	  }    
  if ((document.NTP.dst_startMon.value == 4)||(document.NTP.dst_startMon.value == 6)||(document.NTP.dst_startMon.value == 9)||(document.NTP.dst_startMon.value == 11))
		{
		if(!((document.NTP.dst_startDay.value != "") && (document.NTP.dst_startDay.value > 0)&& (document.NTP.dst_startDay.value <= 30)))
		 {
		   alert("You must input 1~30 days!");
		   return false;
		 }
	  }
	if ((document.NTP.dst_startMon.value == 1)||(document.NTP.dst_startMon.value == 3)||(document.NTP.dst_startMon.value == 5)||(document.NTP.dst_startMon.value == 7)||(document.NTP.dst_startMon.value == 8)||(document.NTP.dst_startMon.value == 10)||(document.NTP.dst_startMon.value == 12))
		{
		if(!((document.NTP.dst_startDay.value != "") && (document.NTP.dst_startDay.value > 0)&& (document.NTP.dst_startDay.value <= 31)))
		 {
		   alert("You must input 1~31 days!");
		   return false;
		 }
	  }    
  if ((document.NTP.dst_endMon.value == 4)||(document.NTP.dst_endMon.value == 6)||(document.NTP.dst_endMon.value == 9)||(document.NTP.dst_endMon.value == 11))
		{
		if(!((document.NTP.dst_endDay.value != "") && (document.NTP.dst_endDay.value > 0)&& (document.NTP.dst_endDay.value <= 30)))
		 {
		   alert("You must input 1~30 days!");
		   return false;
		 }
	  }  
	if (document.NTP.dst_startMon.value == 2)
		{
		if(((document.NTP.document.NTP.mtenCurrent_Year.value)%4)==0)
		{
		 		if(!((document.NTP.dst_startDay.value != "") && (document.NTP.dst_startDay.value > 0)&& (document.NTP.dst_startDay.value <= 29)))
		 {
		   alert("You must input 1~29 days!");
		   return false;
		 }
		}
		else
		{
		if(!((document.NTP.dst_startDay.value != "") && (document.NTP.dst_startDay.value > 0)&& (document.NTP.dst_startDay.value <= 28)))
		 {
		   alert("You must input 1~28 days!");
		   return false;
		 }
		}
	  } 
	  if (document.NTP.dst_endMon.value == 2)
		{
		if(((document.NTP.document.NTP.mtenCurrent_Year.value)%4)==0)
		{
		 		if(!((document.NTP.dst_endDay.value != "") && (document.NTP.dst_endDay.value > 0)&& (document.NTP.dst_endDay.value <= 29)))
		 {
		   alert("You must input 1~29 days!");
		   return false;
		 }
		}
		else
		{
		if(!((document.NTP.dst_endDay.value != "") && (document.NTP.dst_endDay.value > 0)&& (document.NTP.dst_endDay.value <= 28)))
		 {
		   alert("You must input 1~28 days!");
		   return false;
		 }
		}
	  } 
	  showWebMessage(2, "");
	  return true;
}	  
</script>
</head>
<body onload="initValue()">
    <div id="table">
      <ul>
           <li class="table_content">
            <div class="data">
              <ul>
                <li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td id="title"></td>
                   </tr>
                  </table>
                </li>
                <li class="w_text">
                <form method="post" name="NTP" action="/goform/NTP">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="CurrentTime" width="40%"></td>                  
                      <td id="mtenCurrent_Time" width="60%"></td>
                      <td><input type="hidden" name="mtenCurrent_Hour" value="<% getCurrentHour();%>" /></td> 

                      <td><input type="hidden" name="mtenCurrent_Min" value="<% getCurrentMin();%>" /></td> 

                      <td><input type="hidden" name="mtenCurrent_Sec" value="<% getCurrentSec();%>" /></td> 
                      
                      
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="CurrentDate" width="40%"></td>
                      <td id="mtenCurrent_Date" width="60%"></td>
                      <td><input type="hidden" name="mtenCurrent_Year" value="<% getCurrentYear();%>" /></td> 

                      <td><input type="hidden" name="mtenCurrent_Mon" value="<% getCurrentMon();%>" /></td> 

                      <td><input type="hidden" name="mtenCurrent_Day" value="<% getCurrentDay();%>" /></td> 
                     
                      
                    </tr>
                  </table>
                </li>
                <li class="space3"></li>
                <li class="title">
                   <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td id="title_1"></td>
                   </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input name="mten_ServiceType" value="0"  onclick="timeserver_enable_switch()" type="radio" /><font id="Manual">Manual</font></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="NewTime" width="40%"></td>
                      <td><input id="current_hourr" name="current_hour" size="2" maxlength="2"  value="<% getCfgGeneral(1, "current_hour"); %>" type="text" /> :
                          <input id="current_Minn" name="current_Min" size="2" maxlength="2"  value="<% getCfgGeneral(1, "current_Min"); %>"  type="text" />  :
<input id="current_Secc" name="current_Sec" size="2" maxlength="2"  value="<% getCfgGeneral(1, "current_Sec"); %>"  type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="NewDate" width="40%"></td>
                      <td><input  id="current_Yearr" name="current_Year" size="4" maxlength="4"  value="<% getCfgGeneral(1, "current_Year"); %>"  type="text" /> /
                          <input id="current_Monn" name="current_Mon" size="2" maxlength="2"  value="<% getCfgGeneral(1, "current_Mon"); %>"  type="text" /> /
<input id="current_Dayy" name="current_Day" size="2" maxlength="2"  value="<% getCfgGeneral(1, "current_Day"); %>"  type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input name="mten_ServiceType" value="1"  onclick="timeserver_enable_switch()" type="radio" /><font id="TimeServer"></font></td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input  name="time_ServerType" value="0"  onclick="timeserver_enable_switch()" type="radio" /><font id="Auto"></font></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input  name="time_ServerType" value="1" onclick="timeserver_enable_switch()" type="radio" /><font id="UserDefined"></font></td>
                      <td><input  name="NTPServerIP" size="20" maxlength="31" value="<% getCfgGeneral(1, "NTPServerIP"); %>" type="text" /></td>
                    </tr>
                  </table>
                </li>
                <li class="space3"></li>
                <li class="title">
                   <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td id="title_2"></td>
                   </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td id="TimeZone" width="40%"></td>
                      <td><select name="time_zone" size="1">
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
                      </select></td>
                    </tr>
                  </table>
                </li>
                                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="40%"><input name="enabledaylight"  type="checkbox" value="<% getCfgGeneral(1, "enabledaylight"); %>";   onclick="enableDST()"/><font id="Daylight"></font></td>
                      <td>&nbsp;</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td width="20%" id="DaylightStart" ></td>            
	  <td > <input  type="text" maxlength="2" name="dst_startMon" value="<% getCfgGeneral(1, "startMon"); %>" ></td> 
	   <td width="5%"> /</td> 
	  <td > <input  type="text" maxlength="2" name="dst_startDay" value="<% getCfgGeneral(1, "startDay"); %>" ></td> 
     <td id="DaylightStartat" width="5%"></td>
    <td > <input  type="text" maxlength="2" name="dst_startclock" value="<% getCfgGeneral(1, "startclock"); %>" ></td> 
	  <td width="5%" id="DaylightStartclock"></td>
	                   
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="90%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td  width="20%" id="DaylightEnd" ></td>       
	  <td > <input  type="text" maxlength="2" name="dst_endMon" value="<% getCfgGeneral(1, "endMon"); %>" ></td> 
	  <td width="5%">/</td>
	  <td > <input  type="text" maxlength="2" name="dst_endDay" value="<% getCfgGeneral(1, "endDay"); %>" ></td> 
    <td id="Daylightendat" width="5%"></td>
    <td > <input  type="text" maxlength="2" name="dst_endclock" value="<% getCfgGeneral(1, "endclock"); %>" ></td> 
	  <td width="5%" id="Daylightendclock"></td>              
	                   
                    </tr>
                  </table>
</li>
                <li></li>
				<li></li>
              </ul>
            </div>
        <li class="table_button">
          <center>
          <input id="Apply" type=submit  value="" onClick="return allcheck()" > &nbsp; &nbsp; &nbsp;
          <input id="Reset" type=button   value="" onClick="self.location.reload()">
          </center>
        </li>
        </li>
      </ul>
      </form>
    </div>
    <!-- timer Begin -->
  <script language="JavaScript" type="text/javascript">
  setTimeout('IncreaseSec()', 1000);
  setTimeout('self.location.reload()', 180000);
  <!-- display first time -->
  </script>
  <!-- timer End -->
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>
</html>
