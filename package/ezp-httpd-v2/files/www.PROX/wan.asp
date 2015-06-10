<% do_pagehead1(); %>
<html>
  <head>
    <title><% nv_get("model"); %> - WAN</title>
    <% do_pagehead2(); %>
    <link rel="stylesheet" type="text/css" href="ext/css/ext-all.css"/>
    <script language="JavaScript">
        var useIE6 = false;
    </script>
    <!--[if lte IE 6]>
        <script language="JavaScript">
            var useIE6 = true;
        </script>
    <![endif]-->
    <script language="JavaScript">
    
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

    function enableDisableAPCLI(F) {
        var enable = (EN("wan0_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, wl_apcli_objects[0]);
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
                var secmode = EN('wl'+i+'_apcli'+j+'_secmode')[0].value;
                if (secmode == 'wep') {
                    if (EN('wl'+i+'_apcli'+j+'_key1')[0].value == "") return false;
                    if (backup) wl_apcli_key[i][j]=new Array(4);
                    for (var k = 1; k < 5; k++) {
                        if(backup) {
                            wl_apcli_key[i][j][k-1]=EN('wl'+i+'_apcli'+j+'_key'+k)[0].value;
                            EN('wl'+i+'_apcli'+j+'_key'+k)[0].value=encode64(EN('wl'+i+'_apcli'+j+'_key'+k)[0].value);
                        }else 
                            EN('wl'+i+'_apcli'+j+'_key'+k)[0].value=wl_apcli_key[i][j][k-1];
                    }
                }
                if (secmode == 'psk' || secmode == 'psk2') {
                    if (EN('wl'+i+'_apcli'+j+'_key')[0].value == "") return false;
                    if (backup) {
                        wl_apcli_key[0]=EN('wl'+i+'_apcli'+j+'_key')[0].value;
                        EN('wl'+i+'_apcli'+j+'_key')[0].value=encode64(EN('wl'+i+'_apcli'+j+'_key')[0].value);
                    }else
                        EN('wl'+i+'_apcli'+j+'_key')[0].value=wl_apcli_key[0];
                }
            }
        }
        return true;
    }

    function to_submit(F) {
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
         if (EN('wan'+i+'_proto')[0].value == "wisp") {
          if (!encodekey(1)) {
            alert('The Key must have value.');
            return;
          }
         }
        }
	    F.action.value = "Apply";
        F.submit_button.value = "wan";
        ajax_preaction(F, "apply.cgi", 6000);
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
         if (EN('wan'+i+'_proto')[0].value == "wisp") {
          encodekey(0);
         }
        }
        display_mask(F);
    }

    function to_disconnect(F, num) {
        F.submit_button.value = "wan";
        F.type.value = "iface";
        F.num.value = num;
        ajax_preaction(F, "disconnect.cgi", 6000);
        display_mask(F);
    }

    function SelWAN(F) {
        F.submit_button.value = "wan";
        F.action.value = "Gozila";
        F.submit();
    }

    function sel_apn_type(I, nth, type) {
        var enable = (EN("wan"+nth+"_enable")[0].checked == true) ? 1 : 0;
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            E('wan'+nth+'_wwan_location').disabled = true;
            E('wan'+nth+'_wwan_isp').disabled = true;
            E('wan'+nth+'_wwan_apn').disabled = false;
            if (type == 1) E('wan'+nth+'_wwan_dialstr').disabled = false;
        } else if (I == "2") {
            E('wan'+nth+'_wwan_location').disabled = true;
            E('wan'+nth+'_wwan_isp').disabled = true;
            E('wan'+nth+'_wwan_apn').disabled = true;
            if (type == 1) E('wan'+nth+'_wwan_dialstr').disabled = true;
        } else {
            E('wan'+nth+'_wwan_location').disabled = false;
            E('wan'+nth+'_wwan_isp').disabled = false;
            E('wan'+nth+'_wwan_apn').disabled = false;
            if (type == 1) E('wan'+nth+'_wwan_dialstr').disabled = false;
            sel_isp(nth, type);
        }
    }

    function sel_barry_apn_type(I, nth) {
        var enable = (EN("wan"+nth+"_enable")[0].checked == true) ? 1 : 0;
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            E('wan'+nth+'_barry_location').disabled = true;
            E('wan'+nth+'_barry_isp').disabled = true;
        } else {
            E('wan'+nth+'_barry_location').disabled = false;
            E('wan'+nth+'_barry_isp').disabled = false;
        }
    }

    function sel_ppp_mode(I, nth) {
        var enable = (EN("wan"+nth+"_enable")[0].checked == true) ? 1 : 0;
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            E('wan'+nth+'_ppp_idletime').disabled = false;
            E('wan'+nth+'_ppp_lcp_echo_interval').disabled = true;
            E('wan'+nth+'_ppp_lcp_echo_failure').disabled = true;
        } else {
            E('wan'+nth+'_ppp_idletime').disabled = true;
            E('wan'+nth+'_ppp_lcp_echo_interval').disabled = false;
            E('wan'+nth+'_ppp_lcp_echo_failure').disabled = false;
        }
    }
    function sel_wwan_ppp_mode(I, nth) {
        var enable = (EN("wan"+nth+"_enable")[0].checked == true) ? 1 : 0;
        if (enable == 0) {
            return;
        }
        if (I == "1") {
            E('wan'+nth+'_wwan_idletime').disabled = false;
            E('wan'+nth+'_wwan_lcp_echo_interval').disabled = true;
            E('wan'+nth+'_wwan_lcp_echo_failure').disabled = true;
        } else {
            E('wan'+nth+'_wwan_idletime').disabled = true;
            E('wan'+nth+'_wwan_lcp_echo_interval').disabled = false;
            E('wan'+nth+'_wwan_lcp_echo_failure').disabled = false;
        }
    }
    function sel_bigpond_login(I, nth) {
        var enable = (EN("wan"+nth+"_enable")[0].checked == true) ? 1 : 0;
        if (enable == 0) {
            return;
        }
        sel_change(I, 1, F, wan_bigpond_objects[nth]);
    }

    function alarm() {
        if (useIE6) {
            alert('<%lang("For the security and display reasons, IE7.0+ and FireFox are strongly suggested.");%>');
        }
    }

    function init() {
        var change_to_wwan = 0, change_to_wire_wan = 0;

        <%wan_create_proto_array();%>

        F = EN('form')[0];
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var enable = (EN("wan"+i+"_enable")[0].checked == true) ? 1 : 0;
            var proto = E('wan'+i+'_proto').value;
            sel_change(enable, 1, F, wan_objects[i]);
            if (proto == 'wisp')
                enableDisableAPCLI(F);
            if (enable == 0) {
                continue;
            }
            if (proto == 'wwan') {
                sel_location(i, 1);
                var apn_type = 0;
                for (var j=0;j<3;j++){
                    if (EN('wan'+i+'_wwan_apn_type')[j].checked == true) {
                        apn_type = EN('wan'+i+'_wwan_apn_type')[j].value;
                    } 
                }
                sel_apn_type(apn_type, i, "1");
                if (wan_proto[i] != proto) {
                    change_to_wwan = 1;
                }
            } else if (proto == 'directip') {
                sel_location(i, 2);
                var apn_type = 0;
                for (var j=0;j<3;j++){
                    if (EN('wan'+i+'_wwan_apn_type')[j].checked == true) {
                        apn_type = EN('wan'+i+'_wwan_apn_type')[j].value;
                    } 
                }
                sel_apn_type(apn_type, i, "2");
                if (wan_proto[i] != proto) {
                    change_to_wwan = 1;
                }
                var bp = (EN('wan'+i+'_bigpond_enable')[0].checked == true) ? 1 : 0;
                sel_bigpond_login(bp, i);
            } else if (proto == 'pppoe') {
                if (wan_proto[i] != proto) {
                    change_to_wire_wan = 1;
                }
            } else if (proto == 'dhcp') {
                var bp = (EN('wan'+i+'_bigpond_enable')[0].checked == true) ? 1 : 0;
                sel_bigpond_login(bp, i);
                if (wan_proto[i] != proto) {
                    change_to_wire_wan = 1;
                }
            } else if (proto == 'barry') {
                sel_barry_location(i, 1);
                var barry_apn_type = (EN('wan'+i+'_barry_apn_type')[1].checked == true) ? 1 : 0;
                sel_barry_apn_type(barry_apn_type, i);
            } else if (proto == 'static') {
                if (wan_proto[i] != proto) {
                    change_to_wire_wan = 1;
                }
            }
        }
        <%widget_start();%>
        if (change_to_wire_wan) {
        <%prod_show_js_start("portcfg", "1", "1" , "");%>
            alert('<%lang("The ethernet port will be configured as a WAN port.");%>');
        <%prod_show_js_end("portcfg", "1", "1" , "");%>
        }

        if (change_to_wwan == 1) {
            alert('<%lang("For the safe removal of the 3G USB Modem, please either press and hold down the WPS button for 3 seconds or click the safe removal icon available on the User Interface, and wait for the red status light to stop flashing.");%>');
        }
    }

    function sel_brand1(nth) {
        var nbrand = parseInt(E('wan'+nth+'_wwan_brand').value);

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

        E('wan'+nth+'_wwan_model_wrap').innerHTML = s;
    }

    function sel_brand2(nth) {
        var nbrand = parseInt(E('wan'+nth+'_wwan_brand').value);

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

        E('wan'+nth+'_wwan_model_wrap').innerHTML = s;
    }

    function sel_location(nth, type) {
        var loc = parseInt(E('wan'+nth+'_wwan_location').value);
        var isp = parseInt(E('wan'+nth+'_wwan_isp').value);

        arr = isps[loc];

        var s = "<select name='wan"+nth+"_wwan_isp' id='wan"+nth+"_wwan_isp' value='";
        s += isp + "' onchange='sel_isp(" + nth + ", " + type +")'>";
        var selected;
        for (var i = 0; i < arr.length; i++) {
            selected = (i == isp) ? "selected" : "";
            s += "<option value='"+i+"' "+selected+">"+arr[i][0]+"</option>";
        }
        s += "</select>"

        E('wan'+nth+'_wwan_isp_wrap').innerHTML = s;
        var apn_type = (EN("wan"+nth+"_wwan_apn_type")[0].checked == true) ?  1 : 0;
        if (apn_type == '1') {
            sel_apn(nth);
            sel_isp(nth,type);
        }
    }

    function sel_barry_location(nth, type) {
        var loc = parseInt(E('wan'+nth+'_barry_location').value);
        var isp = parseInt(E('wan'+nth+'_barry_isp').value);

        arr = isps[loc];

        var s = "<select name='wan"+nth+"_barry_isp' id='wan"+nth+"_barry_isp' value='";
        s += isp + "' onchange='sel_barry_isp(" + nth + ", " + type +")'>";
        var selected;
        for (var i = 0; i < arr.length; i++) {
            selected = (i == isp) ? "selected" : "";
            s += "<option value='"+i+"' "+selected+">"+arr[i][0]+"</option>";
        }
        s += "</select>"

        E('wan'+nth+'_barry_isp_wrap').innerHTML = s;
        var barry_apn_type = (EN("wan"+nth+"_barry_apn_type")[0].checked == true) ?  1 : 0;
        if (barry_apn_type == '1') {
            sel_barry_apn(nth);
            sel_barry_isp(nth,type);
        }
    }

    function sel_apn(nth) {
        var v1 = parseInt(E('wan'+nth+'_wwan_location').value);
        var v2 = parseInt(E('wan'+nth+'_wwan_isp').value);
        var isp = isps[v1][v2]

        E('wan'+nth+'_wwan_apn').value = isp[1];
    }

    function sel_isp(nth, type) {
        var v1 = parseInt(E('wan'+nth+'_wwan_location').value);
        var v2 = parseInt(E('wan'+nth+'_wwan_isp').value);
        var isp = isps[v1][v2]

        E('wan'+nth+'_wwan_apn').value = isp[1];
        if (type == 1) {
            /* type == 2: directip */
            E('wan'+nth+'_wwan_auth').value = isp[3];
            E('wan'+nth+'_wwan_username').value = isp[4];
            E('wan'+nth+'_wwan_passwd').value = isp[5];
            E('wan'+nth+'_wwan_dialstr').value = isp[6];
        }

        var enable = (EN("wan"+nth+"_enable")[0].checked == true) ? 1 : 0;
        if (enable == 1) {
            E('wan'+nth+'_wwan_isp').disabled = false;
        } else {
            E('wan'+nth+'_wwan_isp').disabled = true;
        }
    }

    function sel_barry_apn(nth) {
        var v1 = parseInt(E('wan'+nth+'_barry_location').value);
        var v2 = parseInt(E('wan'+nth+'_barry_isp').value);
        var isp = isps[v1][v2]

        E('wan'+nth+'_barry_apn').value = isp[1];
    }

    function sel_barry_isp(nth, type) {
        var v1 = parseInt(E('wan'+nth+'_barry_location').value);
        var v2 = parseInt(E('wan'+nth+'_barry_isp').value);
        var isp = isps[v1][v2]

        E('wan'+nth+'_barry_apn').value = isp[1];
        if (type == 1) {
            E('wan'+nth+'_barry_username').value = isp[4];
            E('wan'+nth+'_barry_passwd').value = isp[5];
            E('wan'+nth+'_barry_dialstr').value = isp[6];
        }

        var enable = (EN("wan"+nth+"_enable")[0].checked == true) ? 1 : 0;
        if (enable == 1) {
            E('wan'+nth+'_barry_isp').disabled = false;
        } else {
            E('wan'+nth+'_barry_isp').disabled = true;
        }
    }

    <%wan_wwan_create_modem_array();%>

    <%wan_wwan_create_isp_array();%>
     
    </script>
  </head>
    
  <body class="gui" onload="alarm();init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Setup", "WAN"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("WAN");%>')</script>
        <div id="contentsInfo">
          <form name="form" id="form" action="apply.cgi" method="post" >
	        <script type="text/javascript">do_hidden_elements('wan_rule')</script>
            <% wan_show_setting(); %>
<%prod_show_html_start("wwan", "0", "2");%>
            <fieldset>
              <legend><% lang("WAN PnP"); %></legend>
              <div class="setting">
                <div class="label">
                  <% lang("WAN PnP"); %>
                </div>
                <div class='radiobutton'><input type="radio" value="1" name= "wanupnp_enable"
                <% nvg_match("wanupnp_rule","1","checked"); %>><% lang("Enable"); %></input></div>
                <div class='radiobutton'><input type="radio" value="0" name= "wanupnp_enable"  
                <% nvg_match("wanupnp_rule","0","checked"); %>><% lang("Disable"); %></input></div>
              </div>
            </fieldset><br>
<%prod_show_html_end("wwan", "0", "2");%>

            <div class="submitFooter">
              <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick="to_submit(this.form)" />
              <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.href='wan.asp'" />
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
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window("wan.asp")</script>
  </body>
</html>
