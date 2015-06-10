<% do_pagehead1(); %>
<html>
  <head>
    <title><% nv_get("model"); %> - Wireless</title>
    <% do_pagehead2(); %>
    <script language="JavaScript">
        var useIE6 = false;
    </script>
    <!--[if lte IE 6]>
        <script language="JavaScript">
            var useIE6 = true;
        </script>
    <![endif]-->
    <script language="JavaScript">
    
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
                if (N(F, 'wl'+i+'_ssid'+j+'_enable')[0].checked == true) {
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
                var secmode = N(F, 'wl'+i+'_ssid'+j+'_secmode').value;
                if (secmode == 'wep') {
                    var keyid = N(F, 'wl'+i+'_ssid'+j+'_key_index').value;
                    if (EN('wl'+i+'_ssid'+j+'_key'+keyid)[0].value == "") return false;
                    if (backup) wl_ssid_key[i][j]=new Array(4);
                    for (var k = 1; k < 5; k++) {
                        if (backup) {
                            wl_ssid_key[i][j][k-1]=EN('wl'+i+'_ssid'+j+'_key'+k)[0].value;
                            EN('wl'+i+'_ssid'+j+'_key'+k)[0].value=encode64(EN('wl'+i+'_ssid'+j+'_key'+k)[0].value);
                        } else
                            EN('wl'+i+'_ssid'+j+'_key'+k)[0].value=wl_ssid_key[i][j][k-1];

                    }
                }
                if (secmode == 'psk' || secmode == 'psk2') {
                    if (EN('wl'+i+'_ssid'+j+'_key')[0].value == "") return false;
                    if (backup){
                        wl_ssid_key[0]=EN('wl'+i+'_ssid'+j+'_key')[0].value;
                        EN('wl'+i+'_ssid'+j+'_key')[0].value=encode64(EN('wl'+i+'_ssid'+j+'_key')[0].value);
                    }else
                        EN('wl'+i+'_ssid'+j+'_key')[0].value=wl_ssid_key[0];
                    
                }
                if (secmode == 'wpa' || secmode == 'wpa2') {
                    if (EN('wl'+i+'_ssid'+j+'_radius_key')[0].value == "") return false;
                    if(backup){
                        wl_ssid_radius_key=EN('wl'+i+'_ssid'+j+'_radius_key')[0].value;
                        EN('wl'+i+'_ssid'+j+'_radius_key')[0].value=encode64(EN('wl'+i+'_ssid'+j+'_radius_key')[0].value);
                    }else
                       EN('wl'+i+'_ssid'+j+'_radius_key')[0].value=EN('wl'+i+'_ssid'+j+'_radius_key')[0].value;
                }
            }
        }
        return true;
    }

    function to_submit(F) {
        if (!encodekey(1)) {
            alert('The Key must have value.');
            return;
        }
        F.action.value = "Apply";
        F.submit_button.value = "wl_basic";
        if (local_table_check(F) == false) {
            return;
        }
        ajax_preaction(F,"apply.cgi",6000);
        encodekey(0);
        display_mask(F);
    }

    function SelGuest(enable) {
        var i = <%nv_get("wlv_rule_num");%> - 1 ;
        var appmgr = <%nv_attr_get("appmgr_rule", 0, "guest_lan");%>;
        if (appmgr) {
            sel_change(enable, 1, F, guest_lan_enable_object);
            if (enable) { 
               var enable = (EN("guest_lan_enable")[0].checked == true) ? 1 : 0;
            }
            sel_change(enable, 1, F, guest_lan_objects);
        }
    }
    function SelWL(F) {
        F.submit_button.value = "wl_basic";
        F.action.value = "Gozila";
        F.submit();
    }

    function init() {
        F = EN("form")[0];

        for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
            var enable = (EN("wl"+i+"_radio_enable")[0].checked == true) ? 1 : 0;
            sel_change(enable, 1, F, wl_basic_objects[i]);
            for (var j = 0; j < <%nv_get("wlv_rule_num");%>; j++) {
                sel_change(enable, 1, F, wlv_basic_objects[i][j]);
                if (enable == 1) {
                    wl_ssid_enable_disable(F, i, j);
                    var secmode = N(F, 'wl'+i+'_ssid'+j+'_secmode').value;
                    if (secmode == 'wpa' || secmode == 'wpa2') {
                        wl_rekey_interval_sel(F, i, j);
                    }
                } else if (j ==  <%nv_get("wlv_rule_num");%> - 1 ) SelGuest(enable);
            }
        }
	    <%widget_start();%>
    }

    function wl_ssid_enable_disable(F, i, j) {
        var enable = (EN("wl"+i+"_ssid"+j+"_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, wlv_local_basic_objects[i][j]);
        if(j == <%nv_get("wlv_rule_num");%> - 1) SelGuest(enable);
    }

    function wl_rekey_interval_sel(F, i, j) {
        var mode = EN("wl"+i+"_ssid"+j+"_rekey_mode")[0].value;
        if (mode == 'disable') {
            N(F, "wl"+i+"_ssid"+j+"_rekey_time_interval").disabled = true;
            N(F, "wl"+i+"_ssid"+j+"_rekey_pkt_interval").disabled = true;
        } else if (mode == 'time') {
            N(F, "wl"+i+"_ssid"+j+"_rekey_time_interval").disabled = false;
            N(F, "wl"+i+"_ssid"+j+"_rekey_pkt_interval").disabled = true;
        } else if (mode == 'pkt') {
            N(F, "wl"+i+"_ssid"+j+"_rekey_time_interval").disabled = true;
            N(F, "wl"+i+"_ssid"+j+"_rekey_pkt_interval").disabled = false;
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

    </script>
  </head>
    
  <body class="gui" onload="init();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Wireless", "Basic"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Wireless");%>', '<%lang("Basic");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id="wireless">
	        <script type="text/javascript">do_hidden_elements('wl_basic_rule')</script>	
            <%wl_show_setting("wl_basic_setting.asp");%>
            <div class="submitFooter">
              <input type="button" name="save_button" value="<%lang("Save Settings");%>" 
                    onClick="to_submit(this.form)" />
              <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.href='wl_basic.asp'" />
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
    <script type="text/javascript">create_return_window("wl_basic.asp")</script>
  </body>
</html>
