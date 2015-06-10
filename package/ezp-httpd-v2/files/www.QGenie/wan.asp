<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - WAN</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <script type="text/javascript">
    var wan_objects = new Array(<%nv_get("wan_num");%>);
    var wan_bigpond_objects = new Array(<%nv_get("wan_num");%>);
    var F;
<%prod_show_js("start", "wisp");%>
    var wl_apcli_objects = new Array(<% nv_get("wl0_apcli_rule_num"); %>);
    function SelAPCLI(F) {
        F.submit_button.value = "wan";
        F.action.value = "Gozila";
        F.submit();
    }

    function enableDisableAPCLI(index) {
        var enable = $("input[name=wan"+index+"_enable]:checked").val();
        sel_change(enable, 1, wl_apcli_objects[index]);
    }
<%prod_show_js("end", "wisp");%>
    var wl_apcli_key=new Array(<%nv_get("wl_rule_num");%>);
    function encodekey(backup) {
        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            var apcli_num;
            if (i=="0") apcli_num = "<%nv_get("wl0_apcli_rule_num");%>";
            if (i=="1") apcli_num = "<%nv_get("wl1_apcli_rule_num");%>";
            if (backup) wl_apcli_key[i]=new Array(apcli_num);
            for (var j = 0; j < apcli_num; j++) {
                var secmode = $('#wl'+i+'_apcli'+j+'_secmode').val();
                if (secmode == 'wep') {
                    if ($('#wl'+i+'_apcli'+j+'_key1').val() == "") return false;
                    if (backup) wl_apcli_key[i][j]=new Array(4);
                    for (var k = 1; k < 5; k++) {
                        if(backup) {
                            wl_apcli_key[i][j][k-1]=$('#wl'+i+'_apcli'+j+'_key'+k).val();
                            $('#wl'+i+'_apcli'+j+'_key'+k).val(encode64($('#wl'+i+'_apcli'+j+'_key'+k).val()));
                        }else 
                            $('#wl'+i+'_apcli'+j+'_key'+k).val(wl_apcli_key[i][j][k-1]);
                    }
                }
                if (secmode == 'psk' || secmode == 'psk2') {
                    if ($('#wl'+i+'_apcli'+j+'_key').val() == "") return false;
                    if (backup) {
                        wl_apcli_key[0]=$('#wl'+i+'_apcli'+j+'_key').val();
                        $('#wl'+i+'_apcli'+j+'_key').val(encode64($('#wl'+i+'_apcli'+j+'_key').val()));
                    }else
                        $('#wl'+i+'_apcli'+j+'_key').val(wl_apcli_key[0]);
                }
            }
        }
        return true;
    }

    function to_submit(F) {
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            if ($("select[name=wan"+i+"_proto]").val() == "wisp") {
                if (!encodekey(1)) {
                    alert('<%lang("The Key must have value.");%>');
                    return;
                }
            }
        }

        $("#submit_button").val("wan");
        $("#action").val("Apply");

        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#form").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);

        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            if ($("select[name=wan"+i+"_proto]").val() == "wisp") {
                encodekey(0);
            }
        }
    }
    
    function SelWAN(F) {    //選擇連線類型
        F.submit_button.value = "wan";
        F.action.value = "Gozila";
        F.submit();
    }

    function sel_apn_type(I, nth, type) {
        var enable = $("input[name=wan"+nth+"_enable]:checked").val();
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            $('#wan'+nth+'_wwan_location').attr("disabled",true);
            $('#wan'+nth+'_wwan_isp').attr("disabled",true);
            $('#wan'+nth+'_wwan_apn').attr("disabled",false);
            if (type == 1) $('#wan'+nth+'_wwan_dialstr').attr("disabled",false);
        } else if (I == "2") {
            $('#wan'+nth+'_wwan_location').attr("disabled",true);
            $('#wan'+nth+'_wwan_isp').attr("disabled",true);
            $('#wan'+nth+'_wwan_apn').attr("disabled",true);
            if (type == 1) $('#wan'+nth+'_wwan_dialstr').attr("disabled",true);
        } else {
            $('#wan'+nth+'_wwan_location').attr("disabled",false);
            $('#wan'+nth+'_wwan_isp').attr("disabled",false);
            $('#wan'+nth+'_wwan_apn').attr("disabled",false);
            if (type == 1) $('#wan'+nth+'_wwan_dialstr').attr("disabled",false);

            sel_isp(nth, type);
        }
    }

    function sel_barry_apn_type(I, nth) {
        var enable = $("input[name=wan"+nth+"_enable]:checked").val();
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            $('#wan'+nth+'_barry_location').attr("disabled",true);
            $('#wan'+nth+'_barry_isp').attr("disabled",true);
        } else {
            $('#wan'+nth+'_barry_location').attr("disabled",false);
            $('#wan'+nth+'_barry_isp').attr("disabled",false);
        }
    }

    function sel_ppp_mode(I, nth) {
         var enable = $("input[name=wan"+nth+"_enable]:checked").val()
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            $('#wan'+nth+'_ppp_idletime').attr("disabled",false);
            $('#wan'+nth+'_ppp_lcp_echo_interval').attr("disabled",true);
            $('#wan'+nth+'_ppp_lcp_echo_failure').attr("disabled",true);
        } else {
            $('#wan'+nth+'_ppp_idletime').attr("disabled",true);
            $('#wan'+nth+'_ppp_lcp_echo_interval').attr("disabled",false);
            $('#wan'+nth+'_ppp_lcp_echo_failure').attr("disabled",false);
        }
    }
    function sel_wwan_ppp_mode(I, nth) {
         var enable = $("input[name=wan"+nth+"_enable]:checked").val()
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            E('wan'+nth+'_wwan_idletime').attr("disabled",false);
            E('wan'+nth+'_wwan_lcp_echo_interval').attr("disabled",true);
            E('wan'+nth+'_wwan_lcp_echo_failure').attr("disabled",true);
        } else {
            $('#wan'+nth+'_wwan_idletime').attr("disabled",true);
            $('#wan'+nth+'_wwan_lcp_echo_interval').attr("disabled",false);
            $('#wan'+nth+'_wwan_lcp_echo_failure').attr("disabled",false);
        }
    }
    function sel_bigpond_login(I, nth) {
        var enable = $("input[name=wan"+nth+"_enable]:checked").val()
        if (enable == 0) {
            return;
        }
        sel_change(I, 1, wan_bigpond_objects[nth]);
    }

    function alarm() {
        if (useIE6) {
            alert('<%lang("For the security and display reasons, IE7.0+ and FireFox are strongly suggested.");%>');
        }
    }

    function init() {
        var change_to_wwan = 0, change_to_wire_wan = 0;
        <%wan_create_proto_array();%>

        F = document.getElementsByName('form')[0];
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var enable = $("input[name=wan"+i+"_enable]:checked").val();
            var proto = $("select[name=wan"+i+"_proto]").val()
            sel_change(enable, 1, wan_objects[i]);
            
            if (proto == 'wisp')
                enableDisableAPCLI(i);
            if (enable == 0) {
                continue;
            }
            if (proto == 'wwan') {
                sel_location(i, 1);
                
                apn_type = $("input[name=wan"+i+"_wwan_apn_type]:checked").val();
                sel_apn_type(apn_type, i, "1");
                if (wan_proto[i] != proto) {
                    change_to_wwan = 1;
                }
            } else if (proto == 'directip') {
                sel_location(i, 2);
                apn_type = $("input[name=wan"+i+"_wwan_apn_type]:checked").val();
                sel_apn_type(apn_type, i, "2");
                if (wan_proto[i] != proto) {
                    change_to_wwan = 1;
                }
                var bp = $("input[name=wan"+i+"_bigpond_enable]:checked").val();
                sel_bigpond_login(bp, i);
            } else if (proto == 'pppoe') {
                if (wan_proto[i] != proto) {
                    change_to_wire_wan = 1;
                }
            } else if (proto == 'dhcp') {
                var bp = $("input[name=wan"+i+"_bigpond_enable]:checked").val();
                sel_bigpond_login(bp, i);
                if (wan_proto[i] != proto) {
                    change_to_wire_wan = 1;
                }
            } else if (proto == 'barry') {
                sel_barry_location(i, 1);
                var barry_apn_type = $("input[name=wan"+i+"_barry_apn_type]:checked").val();
                sel_barry_apn_type(barry_apn_type, i);
            } else if (proto == 'static') {
                if (wan_proto[i] != proto) {
                    change_to_wire_wan = 1;
                }
            }
        }
        
        if (change_to_wire_wan) {
        <%prod_show_js_start("portcfg", "1", "1" , "");%>
            alert('<%lang("The ethernet port will be configured as a WAN port.");%>');
        <%prod_show_js_end("portcfg", "1", "1" , "");%>
        }

        if (change_to_wwan == 1) {
            alert('<%lang("For the safe removal of the 3G USB Modem, please either press and hold down the WPS button for 3 seconds or click the safe removal icon available on the User Interface, and wait for the red status light to stop flashing.");%>');
        }
    }

    function sel_brand1(nth) {  // For 3G/4G
        var nbrand = parseInt($('#wan'+nth+'_wwan_brand').val());
        arr = modem_models[nbrand];
        var s = "<select name='wan"+nth+"_wwan_model' id='wwan"+nth+"_wwan_model'>";
        var selected;
        for (var i = 0; i < arr.length; i++) {
            if (arr[i] != undefined) {
                selected = (i == 0) ? "selected" : "";
                s += "<option value='"+i+"' "+selected+">"+arr[i]+"</option>";
            }
        }
        s += "</select>";
        
        $('#wan'+nth+'_wwan_model_wrap').empty();
        $('#wan'+nth+'_wwan_model_wrap').append(s);
        
    }

    function sel_brand2(nth) {  // For HSPA
        var nbrand = parseInt($('#wan'+nth+'_wwan_brand').val());
        arr = directip_modem_models[nbrand];
        
        var s = "<select name='wan"+nth+"_wwan_model' id='wwan"+nth+"_wwan_model'>";
        var selected;
        for (var i = 0; i < arr.length; i++) {
            if (arr[i] != undefined) {
                selected = (i == 0) ? "selected" : "";
                s += "<option value='"+i+"' "+selected+">"+arr[i]+"</option>";
            }
        }
        s += "</select>";

        $('#wan'+nth+'_wwan_model_wrap').empty();
        $('#wan'+nth+'_wwan_model_wrap').append(s);
    }
    
   
    function sel_location(nth, type) {
        var loc = parseInt($('#wan'+nth+'_wwan_location').val());
        var isp = parseInt($('#wan'+nth+'_wwan_isp').val());
        arr = isps[loc];
        
        var s = "<select name='wan"+nth+"_wwan_isp' id='wan"+nth+"_wwan_isp' value='";
        s += isp + "' onchange='sel_isp(" + nth + ", " + type +")'>";
        var selected;
        for (var i = 0; i < arr.length; i++) {
            selected = (i == isp) ? "selected" : "";
            s += "<option value='"+i+"' "+selected+">"+arr[i][0]+"</option>";
        }
        s += "</select>"
        
        $('#wan'+nth+'_wwan_isp_wrap').empty();
        $('#wan'+nth+'_wwan_isp_wrap').append(s);

        var apn_type = $("input[name=wan"+nth+"_wwan_apn_type]:checked").val();
        if (apn_type == '1') {
            sel_apn(nth);
            sel_isp(nth,type);
        }
    }
    
    function sel_barry_location(nth, type) {
        var loc = parseInt($('#wan'+nth+'_barry_location').val());
        var isp = parseInt($('#wan'+nth+'_barry_isp').val());
        arr = isps[loc];
        
        var s = "<select name='wan"+nth+"_barry_isp' id='wan"+nth+"_barry_isp' value='";
        s += isp + "' onchange='sel_barry_isp(" + nth + ", " + type +")'>";
        var selected;
        for (var i = 0; i < arr.length; i++) {
            selected = (i == isp) ? "selected" : "";
            s += "<option value='"+i+"' "+selected+">"+arr[i][0]+"</option>";
        }
        s += "</select>"

        $('#wan'+nth+'_barry_isp_wrap').empty();
        $('#wan'+nth+'_barry_isp_wrap').append(s);

        var barry_apn_type = $("input[name=wan"+nth+"_barry_apn_type]:checked").val();
        if (barry_apn_type == '1') {
            sel_barry_apn(nth);
            sel_barry_isp(nth,type);
        }
    }
    

    function sel_apn(nth) {
        var v1 = parseInt($('#wan'+nth+'_wwan_location').val());
        var v2 = parseInt($('#wan'+nth+'_wwan_isp').val());
        var isp = isps[v1][v2]

        $('#wan'+nth+'_wwan_apn').val(isp[1]);
   }

    function sel_isp(nth, type) {
        var v1 = parseInt($('#wan'+nth+'_wwan_location').val());
        var v2 = parseInt($('#wan'+nth+'_wwan_isp').val());
        var isp = isps[v1][v2]
        $('#wan'+nth+'_wwan_apn').val(isp[1]);
        if (type == 1) {
            /* type == 2: directip */
            $('#wan'+nth+'_wwan_auth').val(isp[3]);
            $('#wan'+nth+'_wwan_username').val(isp[4]);
            $('#wan'+nth+'_wwan_passwd').val(isp[5]);
            $('#wan'+nth+'_wwan_dialstr').val(isp[6]);
        }

        var enable = $("input[name=wan"+nth+"_enable]:checked").val();
        if (enable == 1) {
            $("input[name=wan"+nth+"_enable]").attr("disabled",false);
        } else {
            $("input[name=wan"+nth+"_enable]").attr("disabled",true);
        }
    }

    function sel_barry_apn(nth) {
        var v1 = parseInt($('#wan'+nth+'_barry_location').val());
        var v2 = parseInt($('#wan'+nth+'_barry_isp').val());
        var isp = isps[v1][v2]

        $('#wan'+nth+'_barry_apn').val(isp[1]);
    }

    function sel_barry_isp(nth, type) {
        var v1 = parseInt($('#wan'+nth+'_barry_location').val());
        var v2 = parseInt($('#wan'+nth+'_barry_isp').val());
        var isp = isps[v1][v2]

        $('#wan'+nth+'_barry_apn').val(isp[1]);
        if (type == 1) {
            $('#wan'+nth+'_barry_username').val(isp[4]);
            $('#wan'+nth+'_barry_passwd').val(isp[5]);
            $('#wan'+nth+'_barry_dialstr').val(isp[6]);
        }

        var enable = $("input[name=wan"+nth+"_enable]:checked").val()
        if (enable == 1) {

            $('#wan'+nth+'_barry_isp').attr("disabled",false);
        } else {
            $('#wan'+nth+'_barry_isp').attr("disabled",true);;
        }
    }

    <%wan_wwan_create_modem_array();%>

    <%wan_wwan_create_isp_array();%>

        $(document).ready(function(){
            init();
        });
    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window("wan.asp");</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('wan_rule');</script>
                <h2><%lang("Setup");%>-<%lang("WAN");%></h2>

<% wan_show_setting(); %>
<%prod_show_html_start("wwan", "0", "2");%>
        <div class="row">
            <div class="span12">
                <legend><% lang("WAN PnP"); %></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("WAN PnP"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name= "wanupnp_enable" id= "wanupnp_enable"
                            <% nvg_match("wanupnp_rule","1","checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name= "wanupnp_enable" id= "wanupnp_enable"  
                            <% nvg_match("wanupnp_rule","0","checked"); %>><% lang("Disable"); %></input>
                        </label>

                    </div>
                </div>
            </div>
        </div>
        <br>
<%prod_show_html_end("wwan", "0", "2");%>

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
