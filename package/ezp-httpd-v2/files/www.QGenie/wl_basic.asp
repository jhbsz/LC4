<% do_pagehead1(); %>
<html>
    <head>

                <meta charset="utf-8" />

                <%do_headmeta();%>
                <%do_basic_css();%>

                <%do_ace_font_css();%>
                <%do_custom_css();%>

                <%do_custom_js();%>
                <%do_basic_js();%>


    <script type="text/javascript">
        var F;

        var wl_basic_objects = new Array(<%nv_get("wl_rule_num");%>);
        var wlv_basic_objects = new Array(<%nv_get("wl_rule_num");%>);
        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            wlv_basic_objects[i] = new Array(<%nv_get("wlv_rule_num");%>);
        }                                
        var wlv_local_basic_objects = new Array(<%nv_get("wl_rule_num");%>);
        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            wlv_local_basic_objects[i] = new Array(<%nv_get("wlv_rule_num");%>);
        }                                
        var guest_lan_objects = new Array("1");
        var guest_lan_enable_object = new Array("1");

        function local_table_check(F) {
            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                var disabled = false;
                for (var j = 0; j < <%nv_get("wlv_rule_num");%>; j++) {
                    if ($("input[name=wl'+i+'_ssid'+j+'_enable]:checked").val()=="1") {
                        if (disabled == true) {
                            var k = i+1;
                            var l = j+1;
                            alert('Some SSID has been disabled before SSID '+l+ 
                                    ' of the wireless interface '+k+', so SSID '+l+
                                    ' should not be enabled');
                            return false;
                        }
                    } else {
                        disabled = true;
                    }
                }
            }
            return true;
        }

        var wl_ssid_key = new Array(<%nv_get("wl_rule_num");%>); 
        var wl_ssid_radius_key; 
        function encodekey(backup) {
            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                if (backup) wl_ssid_key[i]=new Array(<%nv_get("wlv_rule_num");%>);
                for (var j = 0; j < <%nv_get("wlv_rule_num");%>; j++) {
                    var secmode = $('#wl'+i+'_ssid'+j+'_secmode').val();
                    if (secmode == 'wep') {
                        var keyid = $('#wl'+i+'_ssid'+j+'_key_index').val();
                        if ($('#wl'+i+'_ssid'+j+'_key'+keyid).val() == "") return false;
                        if (backup) wl_ssid_key[i][j]=new Array(4);
                        for (var k = 1; k < 5; k++) {
                            if (backup) {
                                wl_ssid_key[i][j][k-1]=$('#wl'+i+'_ssid'+j+'_key'+k).val();
                                $('#wl'+i+'_ssid'+j+'_key'+k).val(encode64($('#wl'+i+'_ssid'+j+'_key'+k).val()));
                            } else
                                $('#wl'+i+'_ssid'+j+'_key'+k).val(wl_ssid_key[i][j][k-1]);

                        }
                    }
                    if (secmode == 'psk' || secmode == 'psk2') {
                        if ($('#wl'+i+'_ssid'+j+'_key').val() == "") return false;
                        if (backup){
                            wl_ssid_key[0]=$('#wl'+i+'_ssid'+j+'_key').val();
                            $('#wl'+i+'_ssid'+j+'_key').val(encode64($('#wl'+i+'_ssid'+j+'_key').val()));
                        }else
                            $('#wl'+i+'_ssid'+j+'_key').val(wl_ssid_key[0]);
                        
                    }
                    if (secmode == 'wpa' || secmode == 'wpa2') {
                        if ($('#wl'+i+'_ssid'+j+'_radius_key').val() == "") return false;
                        if(backup){
                            wl_ssid_radius_key=$('#wl'+i+'_ssid'+j+'_radius_key').val();
                            $('#wl'+i+'_ssid'+j+'_radius_key').val(encode64($('#wl'+i+'_ssid'+j+'_radius_key').val()));
                        }else
                           $('#wl'+i+'_ssid'+j+'_radius_key').val($('#wl'+i+'_ssid'+j+'_radius_key').val());
                    }
                }
            }
            return true;
        }

        function to_submit(F) {
            $("#submit_button").val("wl_basic");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);

            /*
            ajax_preaction(F,"apply.cgi",6000);
            display_mask(F);
            */
        }

        function SelGuest(enable) {
            var i = <%nv_get("wlv_rule_num");%> - 1 ;
            var appmgr = <%nv_attr_get("appmgr_rule", 0, "guest_lan");%>;
            if (appmgr) {
                sel_change(enable, 1, guest_lan_enable_object);
                if (enable=="1") { 
                   var enable = $("input[name=guest_lan_enable]:checked").val();
                   //var enable = (EN("guest_lan_enable")[0].checked == true) ? 1 : 0;
                }
                sel_change(enable, 1, guest_lan_objects);
            }
        }
        function SelWL(F) {
            F = document.getElementsByName("form")[0];
            F.submit_button.value = "wl_basic";
            F.action.value = "Gozila";
            F.submit();
        }

        function init() {
            //F = EN("form")[0];

            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                var enable = $("input[name=wl"+i+"_radio_enable]:checked").val();
                //var enable = (EN("wl"+i+"_radio_enable")[0].checked == true) ? 1 : 0;
                sel_change(enable, 1, wl_basic_objects[i]);
                for (var j = 0; j < <%nv_get("wlv_rule_num");%>; j++) {
                    sel_change(enable, 1, wlv_basic_objects[i][j]);
                    if (enable == 1) {
                        wl_ssid_enable_disable(F, i, j);
                        //var secmode = N(F, 'wl'+i+'_ssid'+j+'_secmode').value;
                        var secmode = $('#wl'+i+'_ssid'+j+'_secmode').val();
                        if (secmode == 'wpa' || secmode == 'wpa2') {
                            wl_rekey_interval_sel(F, i, j);
                        }
                    } else if (j ==  <%nv_get("wlv_rule_num");%> - 1 ) SelGuest(enable);
                }
            }
        }

        function wl_ssid_enable_disable(F, i, j) {
            var enable = $("input[name=wl"+i+"_ssid"+j+"_enable]:checked").val();
            sel_change(enable, 1, wlv_local_basic_objects[i][j]);
            if(j == <%nv_get("wlv_rule_num");%> - 1) SelGuest(enable);
        }

        function wl_rekey_interval_sel(F, i, j) {
            var mode = $("#wl"+i+"_ssid"+j+"_rekey_mode").val();
            if (mode == 'disable') {
                $("#wl"+i+"_ssid"+j+"_rekey_time_interval").attr("disabled",true);
                $("#wl"+i+"_ssid"+j+"_rekey_pkt_interval").attr("disabled",true);
            } else if (mode == 'time') {
                $("#wl"+i+"_ssid"+j+"_rekey_time_interval").attr("disabled",false);
                $("#wl"+i+"_ssid"+j+"_rekey_pkt_interval").attr("disabled",true);
            } else if (mode == 'pkt') {
                $("#wl"+i+"_ssid"+j+"_rekey_time_interval").attr("disabled",true);
                $("#wl"+i+"_ssid"+j+"_rekey_pkt_interval").attr("disabled",false);
            }
        }

        function wl_valid_ssid(F) {
        }

        function valid_wl_sec_wpa_radius_ipaddr(key, Message){
          if(!valid_ipaddr(key, Message)){
            return false;
          }
          return true;
        }

        function valid_wl_sec_wpa_radius_port(key, Message) {
          if(!valid_range(key, 1, 65535, Message)) {
            return false;
          }
          return true;
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
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window("wl_basic.asp");</script>

        <form class="form-horizontal" name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('wl_basic_rule');</script>
                <h2><%lang("Wireless");%>-<%lang("Basic");%></h2>

            <%wl_show_setting("wl_basic_setting.asp");%>

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
