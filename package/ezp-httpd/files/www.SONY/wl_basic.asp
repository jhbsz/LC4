<% do_pagehead1(); %>
<html>
  <head>
    <title id="title"><% nv_get("model"); %> - <%lang("Security");%></title>
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
        if($("#wl0_ssid0_secmode").val()=="disabled"){
            display_mask(F);
            SaveNoSecurity();
            setTimeout("SaveSuccess();", 5000);

        }else if($("#wl0_ssid0_secmode").val()=="wep"){
            PasswdVal = $("#wl0_ssid0_key1").val();
            MsgVal = "WEP Password";
            if (PasswdVal.length==0){
                $("#WEPErrorMsg").text("( <%lang("5 or 13 characters");%> )");
                $("#wl0_ssid0_key1c").val("");
                return
            }
            if(valid_wl_sec_wep_key(PasswdVal, MsgVal, 0)){
                $("#WEPErrorMsg").text("");

                if($("#wl0_ssid0_key1").val()!= $("#wl0_ssid0_key1c").val()){
                    $("#WEPErrorMsg").text("<%lang("Password is not same.");%>");
                    $("#wl0_ssid0_key1").val("");
                    $("#wl0_ssid0_key1c").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key1").val("");
                $("#wl0_ssid0_key1c").val("");
                return
            }

            display_mask(F);
            SaveWepSecurity();
            setTimeout("SaveSuccess();", 5000);
            
        }else if($("#wl0_ssid0_secmode").val()=="psk"){
            PasswdVal = $("#wl0_ssid0_key").val();
            MsgVal = "WPA Password";
            if (PasswdVal.length==0){
                $("#WPAErrorMsg").text("( <%lang("8-63 characters");%> )");
                $("#wl0_ssid0_keyc").val("");
                return
            }
            if(valid_wl_sec_wpa_psk_key(PasswdVal, MsgVal, 0)){
                $("#WPAErrorMsg").text("");

                if($("#wl0_ssid0_key").val()!= $("#wl0_ssid0_keyc").val()){
                    $("#WPAErrorMsg").text("<%lang("Password is not same.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }

            display_mask(F);
            SaveWpaSecurity();
            setTimeout("SaveSuccess();", 5000);
            
        }else if($("#wl0_ssid0_secmode").val()=="psk2"){
            PasswdVal = $("#wl0_ssid0_key").val();
            MsgVal = "WPA Password";
            if (PasswdVal.length==0){
                $("#WPAErrorMsg").text("( <%lang("8-63 characters");%> )");
                $("#wl0_ssid0_keyc").val("");
                return
            }
            if(valid_wl_sec_wpa_psk_key(PasswdVal, MsgVal, 0)){
                $("#WPAErrorMsg").text("");

                if($("#wl0_ssid0_key").val()!= $("#wl0_ssid0_keyc").val()){
                    $("#WPAErrorMsg").text("<%lang("Password is not same.");%>");
                    $("#wl0_ssid0_key").val("");
                    $("#wl0_ssid0_keyc").val("");
                    return
                }
            } else {
                $("#wl0_ssid0_key").val("");
                $("#wl0_ssid0_keyc").val("");
                return
            }

            display_mask(F);
            SaveWpa2Security();
            setTimeout("SaveSuccess();", 5000);

        }
    }

    function SaveSuccess(){
          show_mask();
          var msg = "<%lang("The wireless LAN connection will end due to changes in the security settings. Establish a wireless LAN connection.");%>";
          display_return(-3, msg);
    }

    function SaveNoSecurity(){

        setTimeout(function(){
            $.ajax({
               url:        "apply.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   true,
               data:       { action:"Apply", 
                             submit_button:"wl_basic", 
                             wl_basic_rule:"",
                             type:$("#type").val(),
                             num:$("#num").val(),
                             wl0_radio_enable:$("input[name=wl0_radio_enable]:checked").val(),
                             wl0_net_mode:$("#wl0_net_mode").val(),
                             wl0_txpower:$("#wl0_txpower").val(),
                             wl0_channel:$("#wl0_channel").val(),
                             wl0_ssid0_txrate:$("#wl0_ssid0_txrate").val(),
                             wl0_ssid0_enable:$("input[name=wl0_ssid0_enable]:checked").val(),
                             wl0_ssid0_ssid:$("#wl0_ssid0_ssid").val(),
                             wl0_ssid0_hide:$("input[name=wl0_ssid0_hide]:checked").val(),
                             wl0_ssid0_wme_enable:$("input[name=wl0_ssid0_wme_enable]:checked").val(),
                             wl0_ssid0_isolation_enable:$("input[name=wl0_ssid0_isolation_enable]:checked").val(),
                             wl0_ssid0_secmode:$("#wl0_ssid0_secmode").val() },
               success: function(data){
                      show_mask();
                      var RetrunMsg = data.split("=");
                      display_return(RetrunMsg[1]);
               },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });

        }, 1000);
    }

    function SaveWepSecurity(){

        setTimeout(function(){
            $.ajax({
               url:        "apply.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   true,
               data:       { action:"Apply", 
                             submit_button:"wl_basic", 
                             wl_basic_rule:"",
                             type:$("#type").val(),
                             num:$("#num").val(),
                             wl0_radio_enable:$("input[name=wl0_radio_enable]:checked").val(),
                             wl0_net_mode:$("#wl0_net_mode").val(),
                             wl0_txpower:$("#wl0_txpower").val(),
                             wl0_channel:$("#wl0_channel").val(),
                             wl0_ssid0_txrate:$("#wl0_ssid0_txrate").val(),
                             wl0_ssid0_enable:$("input[name=wl0_ssid0_enable]:checked").val(),
                             wl0_ssid0_ssid:$("#wl0_ssid0_ssid").val(),
                             wl0_ssid0_hide:$("input[name=wl0_ssid0_hide]:checked").val(),
                             wl0_ssid0_wme_enable:$("input[name=wl0_ssid0_wme_enable]:checked").val(),
                             wl0_ssid0_isolation_enable:$("input[name=wl0_ssid0_isolation_enable]:checked").val(),
                             wl0_ssid0_secmode:$("#wl0_ssid0_secmode").val(),

                             wl0_ssid0_keytype:$("#wl0_ssid0_keytype").val(),
                             wl0_ssid0_encmode:$("#wl0_ssid0_encmode").val(),
                             wl0_ssid0_key_index:$("#wl0_ssid0_key_index").val(),
                             wl0_ssid0_key1:encode64($("#wl0_ssid0_key1").val()),
                             wl0_ssid0_key2:encode64($("#wl0_ssid0_key2").val()),
                             wl0_ssid0_key3:encode64($("#wl0_ssid0_key3").val()),
                             wl0_ssid0_key4:encode64($("#wl0_ssid0_key4").val()) },
               success: function(data){
                      show_mask();
                      var RetrunMsg = data.split("=");
                      display_return(RetrunMsg[1]);
              },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });

        }, 1000);
    }

    function SaveWpaSecurity(){

        setTimeout(function(){
            $.ajax({
               url:        "apply.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   true,
               data:       { action:"Apply", 
                             submit_button:"wl_basic", 
                             wl_basic_rule:"",
                             type:$("#type").val(),
                             num:$("#num").val(),
                             wl0_radio_enable:$("input[name=wl0_radio_enable]:checked").val(),
                             wl0_net_mode:$("#wl0_net_mode").val(),
                             wl0_txpower:$("#wl0_txpower").val(),
                             wl0_channel:$("#wl0_channel").val(),
                             wl0_ssid0_txrate:$("#wl0_ssid0_txrate").val(),
                             wl0_ssid0_enable:$("input[name=wl0_ssid0_enable]:checked").val(),
                             wl0_ssid0_ssid:$("#wl0_ssid0_ssid").val(),
                             wl0_ssid0_hide:$("input[name=wl0_ssid0_hide]:checked").val(),
                             wl0_ssid0_wme_enable:$("input[name=wl0_ssid0_wme_enable]:checked").val(),
                             wl0_ssid0_isolation_enable:$("input[name=wl0_ssid0_isolation_enable]:checked").val(),
                             wl0_ssid0_secmode:$("#wl0_ssid0_secmode").val(),

                             wl0_ssid0_radius_ipaddr:$("#wl0_ssid0_radius_ipaddr").val(),
                             wl0_ssid0_radius_port:$("#wl0_ssid0_radius_port").val(),
                             wl0_ssid0_radius_key:$("#wl0_ssid0_radius_key").val(),
                             wl0_ssid0_rekey_mode:$("#wl0_ssid0_rekey_mode").val(),
                             wl0_ssid0_rekey_time_interval:$("#wl0_ssid0_rekey_time_interval").val(),
                             wl0_ssid0_rekey_pkt_interval:$("#wl0_ssid0_rekey_pkt_interval").val(),
                             wl0_ssid0_session_timeout:$("#wl0_ssid0_session_timeout").val(),
                             wl0_ssid0_key:encode64($("#wl0_ssid0_key").val()),
                             wl0_ssid0_crypto:$("#wl0_ssid0_crypto").val() },
               success: function(data){
                      show_mask();
                      var RetrunMsg = data.split("=");
                      display_return(RetrunMsg[1]);
              },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });

        }, 1000);

    }

    function SaveWpa2Security(){

        setTimeout(function(){
            $.ajax({
               url:        "apply.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   true,
               data:       { action:"Apply", 
                             submit_button:"wl_basic", 
                             wl_basic_rule:"",
                             type:$("#type").val(),
                             num:$("#num").val(),
                             wl0_radio_enable:$("input[name=wl0_radio_enable]:checked").val(),
                             wl0_net_mode:$("#wl0_net_mode").val(),
                             wl0_txpower:$("#wl0_txpower").val(),
                             wl0_channel:$("#wl0_channel").val(),
                             wl0_ssid0_txrate:$("#wl0_ssid0_txrate").val(),
                             wl0_ssid0_enable:$("input[name=wl0_ssid0_enable]:checked").val(),
                             wl0_ssid0_ssid:$("#wl0_ssid0_ssid").val(),
                             wl0_ssid0_hide:$("input[name=wl0_ssid0_hide]:checked").val(),
                             wl0_ssid0_wme_enable:$("input[name=wl0_ssid0_wme_enable]:checked").val(),
                             wl0_ssid0_isolation_enable:$("input[name=wl0_ssid0_isolation_enable]:checked").val(),
                             wl0_ssid0_secmode:$("#wl0_ssid0_secmode").val(),

                             wl0_ssid0_radius_ipaddr:$("#wl0_ssid0_radius_ipaddr").val(),
                             wl0_ssid0_radius_port:$("#wl0_ssid0_radius_port").val(),
                             wl0_ssid0_radius_key:$("#wl0_ssid0_radius_key").val(),
                             wl0_ssid0_rekey_mode:$("#wl0_ssid0_rekey_mode").val(),
                             wl0_ssid0_rekey_time_interval:$("#wl0_ssid0_rekey_time_interval").val(),
                             wl0_ssid0_rekey_pkt_interval:$("#wl0_ssid0_rekey_pkt_interval").val(),
                             wl0_ssid0_preauth:$("#wl0_ssid0_preauth").val(),
                             wl0_ssid0_pmkperiod:$("#wl0_ssid0_pmkperiod").val(),
                             wl0_ssid0_session_timeout:$("#wl0_ssid0_session_timeout").val(),
                             wl0_ssid0_wpacap:$("#wl0_ssid0_wpacap").val(),
                             wl0_ssid0_key:encode64($("#wl0_ssid0_key").val()),
                             wl0_ssid0_crypto:$("#wl0_ssid0_crypto").val() },
               success: function(data){
                      show_mask();
                      var RetrunMsg = data.split("=");
                      display_return(RetrunMsg[1]);
              },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });

        }, 1000);

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
        F = EN("form")[1];

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

$(document).ready(function(){
    var lang = '<% nv_get("lang"); %>';
    if(lang == "AR") {
        $("#title").attr("dir", "RTL");
        $("#sec_wep").attr("dir", "RTL");
        $("#sec_psk").attr("dir", "RTL");
        $("#sec_psk2").attr("dir", "RTL");
        $("#sec_disable").attr("dir", "RTL");
        $("#wl_ssid_isolation_enable").attr("dir", "RTL");
        $("#wl_ssid_isolation_disable").attr("dir", "RTL");
        $("#wl_ssid_wme_disable").attr("dir", "RTL");
        $("#wl_ssid_wme_enable").attr("dir", "RTL");
        $("#des").attr("dir", "RTL");
        $("#set_up").attr("dir", "RTL");
        $("#set_up").attr("style", "text-align: right;");
        $("#ready").attr("dir", "RTL");
        $("#WEPErrorMsg").attr("dir", "RTL");
        $("#WPAErrorMsg").attr("dir", "RTL");
        $("#cancel_button").attr("dir", "RTL");
        $("#save_button").attr("dir", "RTL");
        $("#passwd").attr("dir", "RTL");
        $("#repasswd").attr("dir", "RTL");
        $("#recommend").attr("dir", "RTL");
        $("#characters").attr("dir", "RTL");
        $("#wifi_mul").attr("dir", "RTL");
        $("#save_buutton").attr("dir", "RTL");
    }
});

    </script>
  </head>
    
  <body class="gui" onload="init();menuFreezer();">
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Wireless", "Basic"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>

	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '')</script>
      <div id="description">
	  <font id="des"><%lang("You can set a password for wireless LAN connection.");%></font>
	  <br>
	  <table width="100%">
	  <tr>
	   <td width="5%">
		<img src="picts/sec_caution.png" border="0">
	   </td>
	   <td id="set_up">
	    <font color="red"><%lang("This set-up is very important to use a wireless LAN device.<br>Without this set-up, other compatible devices within transmission range can connect to your Portable Wireless Sever.<br>You must do this set-up to protect your security.");%></font>
	   </td>
        <td>
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
            &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
        </td>
	  </tr>
      </table>
	  </div>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post" id="wireless">
	        <script type="text/javascript">do_hidden_elements('wl_basic_rule')</script>	
            <%wl_show_setting("wl_basic_setting.asp");%>
            <div class="submitFooter">
              <input type="reset" id="cancel_button" value="<%lang("Cancel");%> "onClick="window.location.href='wl_basic.asp'" />
              <input type="button" name="save_button" id="save_buutton" value="<%lang("Save");%>" 
                    onClick="to_submit(this.form)" />
            </div>
          </form>
        </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
      <!--<%widget_show();%>-->
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
