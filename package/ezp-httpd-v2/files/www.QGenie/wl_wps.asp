<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Wireless</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var wps_display_objects =
        [ { name: 'wl_wps_enable' },
          { name: 'wps_gen_pin' },
          { name: 'wps_pbc' },
          { name: 'wps_pin' }
        ];
            
        var wps_objects =
        [ { name: 'wps_gen_pin' },
          { name: 'wps_pbc' },
          { name: 'wps_pin' }
        ];
        
        var F;

        function to_submit(F) {
            sel_change(1, 1, wps_objects);
            sel_change(1, 1, wps_display_objects);

            $("#submit_button").val("wl_wps");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function to_gen_pin(F) {
            $("#submit_button").val("wl_wps_pin");
            $("#action").val("WPS_Gen");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("wps.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function to_wps_pin(F) {
            $("#action").val("WPS_Pin");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("wps.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function to_wps_btn(F) {
            $("#action").val("WPS_Btn");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("wps.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function init() {
            var display = ($("#wl_wps_display").val() == "1") ? 1 : 0;
            if (display) {
                var enable = ($("input[name=wl_wps_enable]:checked").val()=="7") ? 1: 0;
                sel_change(enable, 1, wps_objects);
            } else {
                sel_change(enable, 1, wps_display_objects);
            }
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

        $(document).ready(function () {
            onoffsnmp();
            $("#snmp_contact").val(decode64('<%b64_nvg_attr_get("snmp_contact", "snmp_rule", "0", "contact");%>'));
        });

        
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
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('wl_wps_rule');</script>
                <h2><%lang("Wireless");%>-<%lang("WPS");%></h2>
        <input type="hidden" name="wl_wps_display" id="wl_wps_display" value="<%nvg_attr_get("wl_wps_display", "wl_wps_rule", 0, "display");%>"/>

        <div class="row">
            <div class="span12">
                <legend>WPS <%lang("Enable");%></legend>
                <div class="row show-grid">
                    <div class="span4">WPS <% lang("Enable"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                          <input type="radio" value="7" name="wl_wps_enable" id="wl_wps_enable" onclick="init()"
                                <%nvg_attr_match("wps_enable", "wl_wps_rule","0","mode","7","checked");%>>
                            <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="0" name="wl_wps_enable" id="wl_wps_enable" onclick="init()"
                                <%nvg_attr_match("wps_enable", "wl_wps_rule","0","mode","0","checked");%>>
                            <%lang("Disable");%>
                        </label>
                    </div>
                </div>
            </div>
        </div><!-- row -->

        <div class="row">
            <div class="span12">
                <legend><%lang("WPS Router PIN Code");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("WPS Router PIN Code");%>:</div>
                    <div class="span8">
                       <label><%nvg_attr_get("wps_pin", "wl_wps_rule", 0, "pin");%></label>
                       <input type="button" class="btn" name="wps_gen_pin" id="wps_gen_pin" value='<%lang("Generate PIN Code");%>' onclick= "to_gen_pin(this.form)">
                    </div>
                </div>
            </div>
        </div><!-- row -->

        <div class="row">
            <div class="span12">
                <legend><%lang("WPS Connect");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("WPS Push Button"); %>:</div>
                    <div class="span8">
                       <input type="button" class="btn" name="wps_pbc" id="wps_pbc" value='<%lang("Push Button");%>' onclick= "to_wps_btn(this.form)">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("WPS Client Pin Code Connect"); %>:</div>
                    <div class="span8">
                        <input type="text" maxLength="8" size="9" name="wps_pin" id="wps_pin"/>
                        <input type="button" class="btn" name="wps_pin" id="wps_pin" value='<%lang("Connection");%>' onclick= "to_wps_pin(this.form)">
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