  <input type="hidden" name="wl$0_ssid$1_radius_ipaddr" id="wl$0_ssid$1_radius_ipaddr" value=
    '<% nvg_attr_get("wl$0_ssid$1_radius_ipaddr","wl$0_sec_wpa2_rule","$1","radius_ipaddr");%>'>
  <input type="hidden" name="wl$0_ssid$1_radius_port" id="wl$0_ssid$1_radius_port" value=
    '<% nvg_attr_get("wl$0_ssid$1_radius_port","wl$0_sec_wpa2_rule","$1","radius_port");%>'>
  <input type="hidden" name="wl$0_ssid$1_radius_key" id="wl$0_ssid$1_radius_key" value=
    '<% nvg_attr_get("wl$0_ssid$1_radius_key","wl$0_sec_wpa2_rule","$1","radius_key");%>'>
  <input type="hidden" name="wl$0_ssid$1_rekey_mode" id="wl$0_ssid$1_rekey_mode" value=
    '<% nvg_attr_get("wl$0_ssid$1_rekey_mode","wl$0_sec_wpa2_rule","$1","rekey_mode");%>'>
  <input type="hidden" name="wl$0_ssid$1_rekey_time_interval" id="wl$0_ssid$1_rekey_time_interval" value=
    '<% nvg_attr_get("wl$0_ssid$1_rekey_interval","wl$0_sec_wpa2_rule","$1","rekey_interval");%>'>
  <input type="hidden" name="wl$0_ssid$1_rekey_pkt_interval" id="wl$0_ssid$1_rekey_pkt_interval" value=
    '<% nvg_attr_get("wl$0_ssid$1_rekey_interval","wl$0_sec_wpa2_rule","$1","rekey_interval");%>'>
  <input type="hidden" name="wl$0_ssid$1_preauth" id="wl$0_ssid$1_preauth" value=
    '<% nvg_attr_get("wl$0_ssid$1_preauth","wl$0_sec_wpa2_rule","$1","preauth");%>'>

  <input type="hidden" name="wl$0_ssid$1_pmkperiod" id="wl$0_ssid$1_pmkperiod" value=
    '<% nvg_attr_get("wl$0_ssid$1_pmkperiod","wl$0_sec_wpa2_rule","$1","pmkperiod");%>'>
  <input type="hidden" name="wl$0_ssid$1_session_timeout" id="wl$0_ssid$1_session_timeout" value=
    '<% nvg_attr_get("wl$0_ssid$1_session_timeout","wl$0_sec_wpa2_rule","$1","session_timeout");%>'>
  <input type="hidden" name="wl$0_ssid$1_wpacap" id="wl$0_ssid$1_wpacap" value=
    '<% nvg_attr_get("wl$0_ssid$1_wpacap","wl$0_sec_wpa2_rule","$1","wpacap");%>'>

        <div class="row show-grid">
            <div class="span4"><% lang("Key"); %></div>
            <div class="span8">
                <script type="text/javascript">
                  document.write('<input type="text" name="wl$0_ssid$1_key" id="wl$0_ssid$1_key" size="17" maxlength="64"');
                  document.write('onChange="valid_wl_sec_wpa_psk_key(this,\'WPA2/PSK key\')"');
                  document.write('value="');
                  document.write(decode64('<%b64_nvg_attr_get("wl$0_ssid$1_key","wl$0_sec_wpa2_rule","$1","key");%>'));
                  document.write('">');
                </script>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Encryption Method"); %></div>
            <div class="span8">
              <select name="wl$0_ssid$1_crypto" id="wl$0_ssid$1_crypto">
              <option value="tkip" <%nvg_attr_match("wl$0_ssid$1_crypto", 
                "wl$0_sec_wpa2_rule", "$1", "crypto", "tkip", "selected");%>><% lang("TKIP"); %>
                </option>
              <option value="aes" <%nvg_attr_match("wl$0_ssid$1_crypto", 
                "wl$0_sec_wpa2_rule", "$1", "crypto", "aes", "selected");%>><% lang("AES"); %>
                </option>
              <option value="mixed" <%nvg_attr_match("wl$0_ssid$1_crypto", 
                "wl$0_sec_wpa2_rule", "$1", "crypto", "mixed", "selected");%>><% lang("Mixed (TKIP+AES)"); %>
                </option>
              </select>
              <br/>
              (<%lang("The Key is an ASCII string of 8-63 digits, or a HEX string of 64 digits.");%>)
            </div>
        </div>

<!--
  <div class="label"></div>
  <br/>

  <div class="label"></div>
  <br/>
