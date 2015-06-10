  <input type="hidden" name="wl$0_ssid$1_radius_ipaddr" id="wl$0_ssid$1_radius_ipaddr" 
    value='<% nvg_attr_get("wl$0_ssid$1_radius_ipaddr","wl$0_sec_wpa_rule","$1","radius_ipaddr");%>'>
  <input type="hidden" name="wl$0_ssid$1_radius_port" id="wl$0_ssid$1_radius_port" 
    value='<% nvg_attr_get("wl$0_ssid$1_radius_port","wl$0_sec_wpa_rule","$1","radius_port");%>'>
  <input type="hidden" name="wl$0_ssid$1_radius_key" id="wl$0_ssid$1_radius_key"
    value='<% nvg_attr_get("wl$0_ssid$1_radius_key","wl$0_sec_wpa_rule","$1","radius_key");%>'>
  <input type="hidden" name="wl$0_ssid$1_rekey_mode" id="wl$0_ssid$1_rekey_mode"
    value='<% nvg_attr_get("wl$0_ssid$1_rekey_mode","wl$0_sec_wpa_rule","$1","rekey_mode");%>'>
  <input type="hidden" name="wl$0_ssid$1_rekey_time_interval" id="wl$0_ssid$1_rekey_time_interval"
    value='<% nvg_attr_get("wl$0_ssid$1_rekey_interval","wl$0_sec_wpa_rule","$1","rekey_interval");%>'>
  <input type="hidden" name="wl$0_ssid$1_rekey_pkt_interval" id="wl$0_ssid$1_rekey_pkt_interval" 
    value='<% nvg_attr_get("wl$0_ssid$1_rekey_interval","wl$0_sec_wpa_rule","$1","rekey_interval");%>'>

  <input type="hidden" name="wl$0_ssid$1_session_timeout" id="wl$0_ssid$1_session_timeout" 
    value='<% nvg_attr_get("wl$0_ssid$1_session_timeout","wl$0_sec_wpa_rule","$1","session_timeout");%>'>

  <div class="label"><% lang("Password"); %></div>

<script type="text/javascript">
  document.write('<input type="password" name="wl$0_ssid$1_key" id="wl$0_ssid$1_key" size="17" maxlength="64"');
  //document.write('onChange="valid_wl_sec_wpa_psk_key(this,\'WPA Password\',0)"');
  document.write('value="');
  document.write(decode64('<%b64_nvg_attr_get("wl$0_ssid$1_key","wl$0_sec_wpa_rule","$1","key");%>'));
  document.write('">');
</script>
<div id="WPAErrorMsg" style="display:inline;color:red;"></div>
  <br/>

<div class="label"><% lang("Retype new password"); %></div> 
<script type="text/javascript">
  document.write('<input type="password" name="wl$0_ssid$1_keyc" id="wl$0_ssid$1_keyc" size="17" maxlength="64"');
  //document.write('onBlur="valid_wl_sec_wpa_psk_key(this,\'WEP Retype Password\',1)"');
  document.write('value="');
  document.write(decode64('<% b64_nvg_attr_get("wl$0_ssid$1_key","wl$0_sec_wpa_rule","$1","key");%>'));
  document.write('">');

  $(document).ready(function(){
    $("select[name='wl$0_ssid$1_crypto']").attr("value", "tkip");
  });
</script><br>
<div class="label">&nbsp</div>( <%lang("8-63 characters");%> )

  <br/>
  <br/>
  <div class="label" style='display:none;'><% lang("Encryption Method"); %></div>
  <div style='display:none;'>
  <select name="wl$0_ssid$1_crypto" id="wl$0_ssid$1_crypto">
  <option value="tkip" <%nvg_attr_match("wl$0_ssid$1_crypto", 
    "wl$0_sec_wpa_rule", "$1", "crypto", "tkip", "selected");%>><% lang("TKIP"); %>
    </option>
  <option value="aes" <%nvg_attr_match("wl$0_ssid$1_crypto", 
    "wl$0_sec_wpa_rule", "$1", "crypto", "aes", "selected");%>><% lang("AES"); %>
    </option>
  <option value="mixed" <%nvg_attr_match("wl$0_ssid$1_crypto", 
    "wl$0_sec_wpa_rule", "$1", "crypto", "mixed", "selected");%>><% lang("Mixed (TKIP+AES)"); %>
    </option>
  </select>
  <br/>
  <br/>
  </div>
  <br/>
