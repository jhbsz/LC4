<% do_pagehead1(); %>
<html>
<head>
  <title><% nv_get("model"); %> -
  Time</title><% do_pagehead2(); %>
  <script type="text/javascript">

var ntp_objects =
[ { name: 'ntp_type', type: 'radio' },
  { name: 'ntp_pool' },
  { name: 'ntp_ipaddr' },
  { name: 'ntp_zone' },
  { name: 'cron_ntp_enable', type: 'radio' },
  { name: 'ntp_daylight', type: 'radio' },
  { name: 'cron_ntp_interval' },
  { name: 'ntp_update' }
];
var ntp_type_objects =
[ { name: 'ntp_pool' },
  { name: 'ntp_ipaddr' }
];
 
var ntp_update_objects =
[
  { name: 'ntp_update' }
];

var ntp_sync_objects = [ { name: 'cron_ntp_interval' } ];

var F;

var tz_array = new Array(
		['UTC12','UTC-12:00 Kwajalein'],
		['UTC11','UTC-11:00 Midway Island, Samoa'],
		['UTC10','UTC-10:00 Hawaii'],
		['NAST9NADT,M3.2.0/2,M11.1.0/2','UTC-09:00 Alaska'],
		['PST8PDT,M3.2.0/2,M11.1.0/2','UTC-08:00 Pacific Time'],
		['UTC7','UTC-07:00 Arizona'],
		['MST7MDT,M3.2.0/2,M11.1.0/2','UTC-07:00 Mountain Time'],
		['UTC6','UTC-06:00 Mexico'],
		['CST6CDT,M3.2.0/2,M11.1.0/2','UTC-06:00 Central Time'],
		['UTC5','UTC-05:00 Colombia, Panama'],
		['EST5EDT,M3.2.0/2,M11.1.0/2','UTC-05:00 Eastern Time'],
		['UTC4','UTC-04:00 Bolivia, Venezuela'],
		['AST4ADT,M3.2.0/2,M11.1.0/2','UTC-04:00 Atlantic Time'],
		['BRWST4BRWDT,M10.3.0/0,M2.5.0/0','UTC-04:00 Brazil West'],
		['NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01','UTC-03:30 Newfoundland'],
		['WGST3WGDT,M3.5.6/22,M10.5.6/23','UTC-03:00 Greenland'],
		['BRST3BRDT,M10.3.0/0,M2.5.0/0','UTC-03:00 Brazil East'],
		['UTC3','UTC-03:00 Guyana'],
		['UTC2','UTC-02:00 Mid-Atlantic'],
		['STD1DST,M3.5.0/2,M10.5.0/2','UTC-01:00 Azores'],
		['UTC0','UTC+00:00 Gambia, Liberia, Morocco'],
		['GMT0BST,M3.5.0/2,M10.5.0/2','UTC+00:00 England'],
		['UTC-1','UTC+01:00 Tunisia'],
		['STD-1DST,M3.5.0/2,M10.5.0/2','UTC+01:00 France, Germany, Italy'],
		['UTC-2','UTC+02:00 South Africa'],
		['STD-2DST,M3.5.0/2,M10.5.0/2','UTC+02:00 Greece, Ukraine, Romania, Turkey'],
		['UTC-3','UTC+03:00 Iraq, Jordan, Kuwait'],
		['UTC-4','UTC+04:00 Armenia'],
		['UTC-5','UTC+05:00 Pakistan, Russia'],
		['UTC-5:30','UTC+05:30 Bombay, Calcutta, Madras, New Delhi'],
		['UTC-6','UTC+06:00 Bangladesh, Russia'],
		['UTC-7','UTC+07:00 Thailand, Russia'],
		['UTC-8','UTC+08:00 Taiwan, China, Hong Kong, Western Australia, Singapore'],
		['UTC-9','UTC+09:00 Japan, Korea'],
		['CST-9:30CST,M10.5.0/2,M3.5.0/3', 'UTC+09:30 South Australia'],
		['UTC-10','UTC+10:00 Guam, Russia'],
		['STD-10DST,M10.5.0/2,M3.5.0/2','UTC+10:00 Australia'],
		['UTC-11','UTC+11:00 Solomon Islands'],
		['UTC-12','UTC+12:00 Fiji'],
		['STD-12DST,M10.5.0/2,M3.5.0/2','UTC+12:00 New Zealand'],
		['','']);

var tz_str = '<%nv_attr_get("ntp_rule","0","zone");%>';

function disable_update() {
    F = document.getElementsByName('form')[0];
    sel_change(0, 1, F, ntp_update_objects);
}

function show_tz() {
    var buf = "";
    for(i=0; tz_array[i][0] != ""; i++){
        if(tz_array[i][0] == tz_str) 
            buf = "selected";
        else
            buf = "";
        document.write("<option value=" + tz_array[i][0] + " " + buf + "> " + tz_array[i][1] + "<\/option>");
    }
}

function to_submit(F) {
    F.submit_button.value = "ntp";
	F.action.value = "Apply";
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}

function init() {
    F = document.getElementsByName('form')[0];
    var enable = (document.getElementsByName("ntp_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, ntp_objects);
    if (enable == 1) {
        var ntp_type = (document.getElementsByName("ntp_type")[0].checked == true) ? 1 : 0;
        if (ntp_type == 0) {
            E("ntp_pool").disabled = true;
            E("ntp_ipaddr").disabled = false;
        } else {
            E("ntp_pool").disabled = false;
            E("ntp_ipaddr").disabled = true;
        }

        ntp_sync_enable = (document.getElementsByName("cron_ntp_enable")[0].checked == true) ? 1 : 0;
        sel_change(ntp_sync_enable,1,F,ntp_sync_objects);
    }
    var type_check = (EN("ntp_type")[0].checked == true ) ? "pool" : "ipaddr";
    type_check = (type_check == EN("ntp_type_org")[0].value) ? 0 : 1;
    var pool_check = (EN("ntp_pool")[0].value == EN("ntp_pool_org")[0].value ) ? 0 : 1;
    var ipaddr_check = (EN("ntp_ipaddr")[0].value == EN("ntp_ipaddr_org")[0].value ) ? 0 : 1;
    var zone_check = (EN("ntp_zone")[0].value == EN("ntp_zone_org")[0].value ) ? 0 : 1;
    var cron_enable_check = (EN("cron_ntp_enable")[0].checked == true ) ? 1 : 0;
    cron_enable_check = (cron_enable_check == EN("cron_ntp_enable_org")[0].value) ? 0 : 1;
    var daylight_check = (EN("ntp_daylight")[0].checked == true ) ? 1 : 0;
    daylight_check = (daylight_check == EN("ntp_daylight_org")[0].value) ? 0 : 1;
    var cron_interval_check = (EN("cron_ntp_interval")[0].value == EN("cron_ntp_interval_org")[0].value ) ? 0 : 1;
    if (type_check || pool_check || ipaddr_check || zone_check || cron_enable_check || daylight_check || cron_interval_check) disable_update();
    <%widget_start();%>
}
  </script>
<input type="hidden" name="ntp_type_org" 
value='<% nvg_attr_get("ntp_type_org","ntp_rule","0","type");%>'>
<input type="hidden" name="ntp_pool_org" 
value='<% nvg_attr_get("ntp_pool_org","ntp_rule","0","pool");%>'>
<input type="hidden" name="ntp_ipaddr_org" 
value='<% nvg_attr_get("ntp_ipaddr_org","ntp_rule","0","ipaddr");%>'>
<input type="hidden" name="ntp_zone_org" 
value='<% nvg_attr_get("ntp_zone_org","ntp_rule","0","zone");%>'>
<input type="hidden" name="cron_ntp_enable_org" 
value='<% nvg_attr_get("cron_ntp_enable_org","cron_rule","0","ntp_enable");%>'>
<input type="hidden" name="ntp_daylight_org" 
value='<% nvg_attr_get("ntp_daylight_org","ntp_rule","0","daylight");%>'>
<input type="hidden" name="cron_ntp_interval_org" 
value='<% nvg_attr_get("cron_ntp_interval_org","cron_rule","0","ntp_interval");%>'>
</head>


<body class="gui" id="gui" onload="init();">
  <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("setup", "Time"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("Time");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name='form' action="apply.cgi" method="post">
	        <script type="text/javascript">do_hidden_elements('ntp_rule')</script>	
            <input type="hidden" name="cron_rule" value="ntp"> 

            <fieldset>
              <legend><%lang("Time Synchronization");%></legend>

                <div class="setting">
                  <div class="label">
                    <%lang("Time Synchronization");%>
                  </div>
                  <input type="radio" value="1" name= "ntp_enable" onClick="init()"
                    <% nvg_attr_match("ntp_enable","ntp_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  <input type="radio" value="0" name= "ntp_enable" onClick="init()"
                    <% nvg_attr_match("ntp_enable","ntp_rule","0","enable","0","checked");%>> <%lang("Disable");%>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Time Server Type");%>
                  </div>
                  <input type="radio" value="pool" name= "ntp_type" onClick="init()"
                    <% nvg_attr_match("ntp_enable","ntp_rule","0","type","pool","checked");%>><%lang("Time Server Pool");%>
                  <input type="radio" value="ipaddr" name= "ntp_type" onClick="init()"
                    <% nvg_attr_match("ntp_enable","ntp_rule","0","type","ipaddr","checked");%>><%lang("Manual");%>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Time Server Area");%>
                  </div>

                  <select name="ntp_pool" id="ntp_pool">
                    <option value="pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "pool.ntp.org", "selected"); %>><%lang("Automatic");%></option>
                    <option value="asia.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "asia.pool.ntp.org", "selected"); %>><%lang("Asia");%></option>
                    <option value="europe.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "europe.pool.ntp.org", "selected"); %>><%lang("Europe");%></option>
                    <option value="north-america.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "north-america.pool.ntp.org", "selected"); %>><%lang("North America");%></option>
                    <option value="south-america.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "south-america.pool.ntp.org", "selected"); %>><%lang("South America");%></option>
                    <option value="africa.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "africa.pool.ntp.org", "selected"); %>><%lang("Africa");%></option>
                  </select>
                </div>

                <div class="setting">
                    <div class="label"><%lang("Time Server IP Address");%></div>
                    <input type="text" maxLength="15" size="16" id="ntp_ipaddr"
                        name="ntp_ipaddr" value='<%nv_attr_get("ntp_rule", "$0", "ipaddr");%>' 
                        onblur='valid_ipaddr(this, "NTP Server IP Address")' />
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Time Zone");%>
                  </div><select name="ntp_zone">
                  <script language="javascript" type= "text/javascript">
                    show_tz();
                  </script>
                  </select>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Periodic Synchronization");%>
                  </div>
                  <input type="radio" value="1" name= "cron_ntp_enable" onClick="init()" 
                    <% nvg_attr_match("cron_ntp_enable","cron_rule","0","ntp_enable","1","checked");%>><%lang("Enable");%>
                  <input type="radio" value="0" name= "cron_ntp_enable" onClick="init()" 
                    <% nvg_attr_match("cron_ntp_enable","cron_rule","0","ntp_enable","0","checked");%>><%lang("Disable");%>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Daylight Saving Support");%>
                  </div>
                  <input type="radio" value="1" name= "ntp_daylight" onClick="init()" 
                    <% nvg_attr_match("ntp_daylight","ntp_rule","0","daylight","1","checked");%>><%lang("Enable");%>
                  <input type="radio" value="0" name= "ntp_daylight" onClick="init()" 
                    <% nvg_attr_match("ntp_daylight","ntp_rule","0","daylight","0","checked");%>><%lang("Disable");%>
                </div>

                <div class="setting">
                  <div class="label">
                    <%lang("Synchronization Interval");%>
                  </div><select name="cron_ntp_interval">
                    <option value="01 * * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "01 * * * *", "selected"); %>><%lang("Every Hour");%></option>
                    <option value="06 */6 * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "06 */6 * * *", "selected"); %>><%lang("Every 6 Hours");%></option>
                    <option value="12 */12 * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "12 */12 * * *", "selected"); %>><%lang("Every 12 Hours");%></option>
                    <option value="02 4 * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "02 4 * * *", "selected"); %>><%lang("Every Day");%></option>
                    <option value="22 4 * * 0" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "22 4 * * 0", "selected"); %>><%lang("Every Week");%></option>
                  </select>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Action");%></div>
                  <input type="button" name="ntp_update" id="ntp_update" 
                          onclick="to_update(0, 'ntp')" value="<%lang("Update");%>" />
                </div>
            </fieldset><br>
            <fieldset>
              <legend><%lang("Automatic");%> <%lang("Reboot");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Reboot Interval");%></div>
                  <input type="text" maxLength="15" size="16" 
                  id="cron_reboot_interval" name="cron_reboot_interval" value=
                  '<%nv_attr_get("cron_rule", "$0", "reboot_interval");%>' 
                        onblur='valid_range(this, 5, 43200, 
                        "This number must be between 5 and 43200.
                         (which is equal to 30 days). ")' />
                  (5 ~ 43200 min(s))
                </div>
                <div class="setting">
                  <div class="label">
                    <%lang("Daily Reboot");%>
                  </div><select name="cron_reboot_hour">
                  <script language="javascript" type="text/javascript"> 
                    var hour =  <%nvg_attr_get("cron_reboot_hour", "cron_rule", "$0", "reboot_hour");%>; 
                    for (var i=-1;i<24;i++){
                        if(i == hour) 
                            buf = "selected";
                        else
                            buf = "";
                        var j = i;
                        if (i < 12) j = i + ' AM'
                        else if (i >= 12 ) j = i - 12 + ' PM'
                        if (i == -1) j = "NONE"
                        if (i == 0) j = "12 AM"
                        if (i == 12) j = "12 PM"
                        document.write("<option value=\"" + i + "\" " + buf + ">" + j + "<\/option>");
                    }
                  </script>
                  </select>
                </div>
            </fieldset><br>

            <div class="submitFooter">
              <input type="button" name="save_button" value=
              "<%lang("Save Settings");%>" onClick= "to_submit(this.form)" />
              <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()" />
            </div>
          </form>
        </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
      <%widget_show();%>
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
    </div>
    <script type="text/javascript">
    document.getElementById('gui').addEventListener('change', function(){
      disable_update()
    }, false);
    </script>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window()</script>
</body>
</html>

