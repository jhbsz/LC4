  <input type="hidden" name="wl$0_ssid$1_key" 
    value='<% nvg_attr_get("wl$0_ssid$1_key",
    "wl$0_sec_wpa2_rule","$1","key");%>'>

  <input type="hidden" name="wl$0_ssid$1_pmkperiod" value=
    '<% nvg_attr_get("wl$0_ssid$1_pmkperiod","wl$0_sec_wpa2_rule","$1","pmkperiod");%>'>
  <input type="hidden" name="wl$0_ssid$1_session_timeout" value=
    '<% nvg_attr_get("wl$0_ssid$1_session_timeout","wl$0_sec_wpa2_rule","$1","session_timeout");%>'>
  <input type="hidden" name="wl$0_ssid$1_wpacap" value=
    '<% nvg_attr_get("wl$0_ssid$1_wpacap","wl$0_sec_wpa2_rule","$1","wpacap");%>'>


  <div class="label"><%lang("Radius Server IP Address");%></div>
  <input type="text" name="wl$0_ssid$1_radius_ipaddr" 
    value='<% nvg_attr_get("wl$0_ssid$1_radius_ipaddr",
    "wl$0_sec_wpa2_rule","$1","radius_ipaddr");%>'>
  <br/>

  <div><div class="label"><%lang("Radius Server Port");%></div>
  <input type="text" name="wl$0_ssid$1_radius_port"
    value='<% nvg_attr_get("wl$0_ssid$1_radius_port",
    "wl$0_sec_wpa2_rule","$1","radius_port");%>'>
   </div>

    <div> <div class="label"><% lang("Radius Key"); %></div>
<script type="text/javascript">
  document.write('<input type="text" name="wl$0_ssid$1_radius_key" size="17" maxlength="64"');
  document.write('onChange="valid_wl_sec_wpa_psk_key(this,\'WPA2/PSK key\')"');
  document.write('value="');
  document.write(decode64('<%b64_nvg_attr_get("wl$0_ssid$1_radius_key","wl$0_sec_wpa2_rule","$1","radius_key");%>'));
  document.write('">');
</script>
    </div>

  <div class="label"><% lang("Encryption Method"); %></div>
  <select name="wl$0_ssid$1_crypto">
  <option value="tkip" <%nvg_attr_match("wl$0_ssid$1_crypto", "wl$0_sec_wpa2_rule", 
    "$1", "crypto", "tkip", "selected");%>><% lang("TKIP"); %>
    </option>
  <option value="aes" <%nvg_attr_match("wl$0_ssid$1_crypto", "wl$0_sec_wpa2_rule", 
    "$1", "crypto", "aes", "selected");%>><% lang("AES"); %>
    </option>
  <option value="mixed" <%nvg_attr_match("wl$0_ssid$1_crypto", "wl$0_sec_wpa2_rule", 
    "$1", "crypto", "mixed", "selected");%>><% lang("Mixed (TKIP+AES)"); %>
    </option>
  </select>
  <br/>

  <div class="label"><% lang("Rekey Method"); %></div>
  <select name="wl$0_ssid$1_rekey_mode" onchange="init(this.form, $0, $1)">
  <option value="disable" <%nvg_attr_match("wl$0_ssid$1_rekey_mode", 
    "wl$0_sec_wpa2_rule", "$1", "rekey_mode", "disable", "selected");%>>
    <% lang("Disable"); %>
    </option>
  <option value="time" <%nvg_attr_match("wl$0_ssid$1_rekey_mode", 
    "wl$0_sec_wpa2_rule", "$1", "rekey_mode", "time", "selected");%>>
    <% lang("Time"); %>
    </option>
  <option value="pkt" <%nvg_attr_match("wl$0_ssid$1_rekey_mode", 
    "wl$0_sec_wpa2_rule", "$1", "rekey_mode", "pkt", "selected");%>>
    <% lang("Packet Number"); %>
    </option>
  </select>
  <br/>

  <div class="label"><% lang("Rekey Time Interval"); %></div>
  <input name="wl$0_ssid$1_rekey_time_interval" 
    value='<% nvg_attr_get("wl$0_ssid$1_rekey_interval", "wl$0_sec_wpa2_rule","$1","rekey_time_interval");%>'>
  <br/>

  <div class="label"><% lang("Rekey Packet Interval"); %></div>
  <input name="wl$0_ssid$1_rekey_pkt_interval" 
    value='<% nvg_attr_get("wl$0_ssid$1_rekey_interval", "wl$0_sec_wpa2_rule","$1","rekey_pkt_interval");%>'>
  <br/>

  <div class="label"><% lang("Pre-authentication"); %></div>
  <input type=radio name="wl$0_ssid$1_preauth" value="1" 
    <%nvg_attr_match("wl$0_ssid$1_preauth", "wl$0_sec_wpa2_rule", "$1", 
    "preauth", "1", "checked");%>><% lang("Enable"); %>
    </input>
  <input type=radio name="wl$0_ssid$1_preauth" value="0" 
    <%nvg_attr_match("wl$0_ssid$1_preauth", "wl$0_sec_wpa2_rule", "$1", 
    "preauth", "0", "checked");%>><% lang("Disable"); %>
    </input>
  <br/>
  <br/>
  (<%lang("The Key is an ASCII string of 8-63 digits, or a HEX string of 64 digits.");%>)
  <br/>



