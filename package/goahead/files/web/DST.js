var whichDayNum;
  var BigMon=new Array('1','3','5','7','8','10','12');
  var SmallMon=new Array('4','6','9','11');
  var Hour, Min, Sec;
  var Year, Mon, Day;
function initDateAndTime()
        {
           //alert("run initDateAndTime");
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
function daylightsaving()
{
	//alert("run daylightsaving");
	var now_sec;
	var dl_start_sec;
	var dl_end_sec;

	//current time
	 now_Hour=parseInt(document.NTP.mtenCurrent_Hour.value,10);
   now_Min=parseInt(document.NTP.mtenCurrent_Min.value,10);
   now_Sec=parseInt(document.NTP.mtenCurrent_Sec.value,10);
   now_Year=parseInt(document.NTP.mtenCurrent_Year.value,10);
   now_Mon=parseInt(document.NTP.mtenCurrent_Mon.value,10);
   now_Day=parseInt(document.NTP.mtenCurrent_Day.value,10);
  //dst start time
   dst_start_Mon=parseInt(start_Mon,10);
   dst_start_Day=parseInt(start_Day,10);
   dst_start_Hour=parseInt(start_Hour,10);
   
   //dst end time
   dst_end_Mon=parseInt(end_Mon,10);
   dst_end_Day=parseInt(end_Day,10);
   dst_end_Hour=parseInt(end_Hour,10);
   
   
   
   now_long_sec = Date.UTC(now_Year, now_Mon, now_Day, now_Hour, now_Min, now_Sec);
   dst_start_long_sec = Date.UTC(now_Year, dst_start_Mon, dst_start_Day, dst_start_Hour, now_Min, now_Sec);
   dst_end_long_sec = Date.UTC(now_Year, dst_end_Mon, dst_end_Day, dst_end_Hour, now_Min, now_Sec);
   //alert(now_long_sec);
   //alert(dst_start_long_sec);
   //alert(dst_end_long_sec);
   
   
//DST start
   if((now_long_sec >= dst_start_long_sec)&&(now_long_sec < dst_end_long_sec))
   {
     //alert(document.NTP.mtenCurrent_Hour.value);
     //alert("DST start");
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

//DST end
}           