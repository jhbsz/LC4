<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Time</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

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
            //F = document.getElementsByName('form')[0];
            sel_change(0, 1, ntp_update_objects);
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
            $("#submit_button").val("ntp");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

            /*
            F.submit_button.value = "ntp";
            F.action.value = "Apply";
            ajax_preaction(F, "apply.cgi", 6000);
            display_mask(F);
            */
        }

        function init() {
            //F = document.getElementsByName('form')[0];
            //var enable = (document.getElementsByName("ntp_enable")[0].checked == true) ? 1 : 0;
            var enable = $("input[name=ntp_enable]:checked").val();
            sel_change(enable, 1, ntp_objects);
            if (enable == 1) {
                //var ntp_type = (document.getElementsByName("ntp_type")[0].checked == true) ? 1 : 0;
                var ntp_type = ($("input[name=ntp_type]:checked").val()=="pool") ? 1 : 0;
                if (ntp_type == 0) {
                    $("#ntp_pool").attr("disabled", true);
                    $("#ntp_ipaddr").attr("disabled", false);
                } else {
                    $("#ntp_pool").attr("disabled", false);
                    $("#ntp_ipaddr").attr("disabled", true);
                }

                //ntp_sync_enable = (document.getElementsByName("cron_ntp_enable")[0].checked == true) ? 1 : 0;
                ntp_sync_enable = $("input[name=cron_ntp_enable]:checked").val();
                sel_change(ntp_sync_enable, 1, ntp_sync_objects);
            }

            //var type_check = (EN("ntp_type")[0].checked == true ) ? "pool" : "ipaddr";
            var type_check = ($("input{name=ntp_type]:checked").val()=="1") ? "pool" : "ipaddr";
            type_check = (type_check == $("#ntp_type_org").val() ) ? 0 : 1;
            var pool_check = ($("#ntp_pool").val() == $("#ntp_pool_org").val() ) ? 0 : 1;
            var ipaddr_check = ($("#ntp_ipaddr").val() == $("#ntp_ipaddr_org").val() ) ? 0 : 1;
            var zone_check = ($("#ntp_zone").val() == $("#ntp_zone_org").val() ) ? 0 : 1;
            var cron_enable_check = $("input[name=cron_ntp_enable]:checked").val();
            cron_enable_check = (cron_enable_check == $("#cron_ntp_enable_org").val() ) ? 0 : 1;
            var daylight_check =  $("input[name=ntp_daylight]:checked").val(); 
            daylight_check = (daylight_check == $("#ntp_daylight_org").val()) ? 0 : 1;
            var cron_interval_check = ($("#cron_ntp_interval").val() == $("#cron_ntp_interval_org").val() ) ? 0 : 1;
            if (type_check || pool_check || ipaddr_check || zone_check || cron_enable_check || daylight_check || cron_interval_check) disable_update();
        }

        $(document).ready(function () {
            init();
        });

    /*
        var snmp_objects =
        [ 
        { name: 'snmp_udpport' },
        { name: 'snmp_tcpport' },
        { name: 'snmp_community' },
        { name: 'snmp_vendor' },
        { name: 'snmp_sysname' },
        { name: 'snmp_location' },
        { name: 'snmp_contact' },
        { name: 'snmp_timeout' },
        { name: 'snmp_auth' }
        ];                                    

        function onoffsnmp() {
            enable_snmp = $('input[name=snmp_enable]:checked').val();
            sel_change(enable_snmp, 1, snmp_objects);
        }


        
        function to_submit(F) {
            $("#snmp_contact").val( encode64($("#snmp_contact").val()) );
            $("#submit_button").val("snmp");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
       }
     */   

    </script>
    <input type="hidden" name="ntp_type_org" id="ntp_type_org" 
    value='<% nvg_attr_get("ntp_type_org","ntp_rule","0","type");%>'>
    <input type="hidden" name="ntp_pool_org" id="ntp_pool_org" 
    value='<% nvg_attr_get("ntp_pool_org","ntp_rule","0","pool");%>'>
    <input type="hidden" name="ntp_ipaddr_org" id="ntp_ipaddr_org" 
    value='<% nvg_attr_get("ntp_ipaddr_org","ntp_rule","0","ipaddr");%>'>
    <input type="hidden" name="ntp_zone_org" id="ntp_zone_org" 
    value='<% nvg_attr_get("ntp_zone_org","ntp_rule","0","zone");%>'>
    <input type="hidden" name="cron_ntp_enable_org" id="cron_ntp_enable_org" 
    value='<% nvg_attr_get("cron_ntp_enable_org","cron_rule","0","ntp_enable");%>'>
    <input type="hidden" name="ntp_daylight_org" id="ntp_daylight_org" 
    value='<% nvg_attr_get("ntp_daylight_org","ntp_rule","0","daylight");%>'>
    <input type="hidden" name="cron_ntp_interval_org" id="cron_ntp_interval_org"
    value='<% nvg_attr_get("cron_ntp_interval_org","cron_rule","0","ntp_interval");%>'>

    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('ntp_rule');</script>
              <input type="hidden" name="cron_rule" value="ntp">
                <h2><%lang("Setup");%>-<%lang("Time");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Time Synchronization");%></legend>

                <div class="row show-grid">
                    <div class="span4"><%lang("Time Synchronization");%></div>
                    <div class="span8">
                        <label class="radio inline">
                          <input type="radio" value="1" name= "ntp_enable" id= "ntp_enable" onClick="init()"
                            <% nvg_attr_match("ntp_enable","ntp_rule","0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="0" name= "ntp_enable" id= "ntp_enable" onClick="init()"
                            <% nvg_attr_match("ntp_enable","ntp_rule","0","enable","0","checked");%>> <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Time Server Type");%></div>
                    <div class="span8">
                        <label class="radio inline">
                          <input type="radio" value="pool" name= "ntp_type" id= "ntp_type" onClick="init()"
                            <% nvg_attr_match("ntp_enable","ntp_rule","0","type","pool","checked");%>><%lang("Time Server Pool");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="ipaddr" name= "ntp_type" id= "ntp_type" onClick="init()"
                            <% nvg_attr_match("ntp_enable","ntp_rule","0","type","ipaddr","checked");%>><%lang("Manual");%>
                        </label>
                   </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Time Server Area");%></div>
                    <div class="span8">
                      <select name="ntp_pool" id="ntp_pool">
                        <option value="pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "pool.ntp.org", "selected"); %>><%lang("Automatic");%></option>
                        <option value="asia.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "asia.pool.ntp.org", "selected"); %>><%lang("Asia");%></option>
                        <option value="europe.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "europe.pool.ntp.org", "selected"); %>><%lang("Europe");%></option>
                        <option value="north-america.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "north-america.pool.ntp.org", "selected"); %>><%lang("North America");%></option>
                        <option value="south-america.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "south-america.pool.ntp.org", "selected"); %>><%lang("South America");%></option>
                        <option value="africa.pool.ntp.org" <% nvg_attr_match("ntp_pool", "ntp_rule", "0", "pool", "africa.pool.ntp.org", "selected"); %>><%lang("Africa");%></option>
                      </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Time Server IP Address");%></div>
                    <div class="span8">
                        <input type="text" maxLength="15" size="16" id="ntp_ipaddr"
                            name="ntp_ipaddr" value='<%nv_attr_get("ntp_rule", "$0", "ipaddr");%>' 
                            onblur='valid_ipaddr(this, "NTP Server IP Address")' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Time Zone");%></div>
                    <div class="span8">
                        <select name="ntp_zone" id="ntp_zone">
                            <script language="javascript" type= "text/javascript">show_tz();</script>
                        </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Periodic Synchronization");%></div>
                    <div class="span8">
                        <label class="radio inline">
                          <input type="radio" value="1" name= "cron_ntp_enable" id= "cron_ntp_enable" onClick="init()" 
                            <% nvg_attr_match("cron_ntp_enable","cron_rule","0","ntp_enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="0" name= "cron_ntp_enable" id= "cron_ntp_enable" onClick="init()" 
                            <% nvg_attr_match("cron_ntp_enable","cron_rule","0","ntp_enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Daylight Saving Support");%></div>
                    <div class="span8">
                        <label class="radio inline">
                          <input type="radio" value="1" name= "ntp_daylight" id= "ntp_daylight" onClick="init()" 
                            <% nvg_attr_match("ntp_daylight","ntp_rule","0","daylight","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="0" name= "ntp_daylight" id= "ntp_daylight" onClick="init()" 
                            <% nvg_attr_match("ntp_daylight","ntp_rule","0","daylight","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Synchronization Interval");%></div>
                    <div class="span8">
                        <select name="cron_ntp_interval" id="cron_ntp_interval">
                            <option value="01 * * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "01 * * * *", "selected"); %>>
                                <%lang("Every Hour");%></option>
                            <option value="06 */6 * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "06 */6 * * *", "selected"); %>>
                                <%lang("Every 6 Hours");%></option>
                            <option value="12 */12 * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "12 */12 * * *", "selected"); %>>
                                <%lang("Every 12 Hours");%></option>
                            <option value="02 4 * * *" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "02 4 * * *", "selected"); %>>
                                <%lang("Every Day");%></option>
                            <option value="22 4 * * 0" <% nvg_attr_match("cron_ntp_interval", "cron_rule", "0", "ntp_interval", "22 4 * * 0", "selected"); %>>
                                <%lang("Every Week");%></option>
                          </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Action");%></div>
                    <div class="span8">
                          <input type="button" class="btn" name="ntp_update" id="ntp_update" 
                                  onclick="to_update(0, 'ntp')" value="<%lang("Update");%>" />
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>

        <div class="row">
            <div class="span12">
                <legend><%lang("Automatic");%> <%lang("Reboot");%></legend>

                <div class="row show-grid">
                    <div class="span4"><%lang("Reboot Interval");%></div>
                    <div class="span8">
                      <input type="text" maxLength="15" size="16" id="cron_reboot_interval" name="cron_reboot_interval" 
                        value='<%nv_attr_get("cron_rule", "$0", "reboot_interval");%>' 
                        onblur='valid_range(this, 5, 43200, "This number must be between 5 and 43200. (which is equal to 30 days). ")' />
                       (5 ~ 43200 min(s))
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Daily Reboot");%></div>
                    <div class="span8">
                        <select name="cron_reboot_hour" id="cron_reboot_hour">
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
                </div>
        
            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12">
                <div class="submitFooter">
                    <input type="button" class="btn btn-primary" onClick="to_submit(this.form)" value="<%lang("Save Settings");%>">
                    <input type="reset" class="btn" value="<%lang("Cancel Changes");%>">
                </div>
            </div>
        </div><!-- row -->

        </form>
        
        <div class="row">
            <div class="span12">
                   <script type="text/javascript">
                        <%show_copyright();%>
                    </script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>