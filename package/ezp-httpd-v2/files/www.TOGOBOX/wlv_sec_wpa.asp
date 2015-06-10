
  <input type="hidden" name="wl$0_ssid$1_key" id="wl$0_ssid$1_key" 
    value='<% nvg_attr_get("wl$0_ssid$1_key","wl$0_sec_wpa_rule","$1","key");%>'>

  <input type="hidden" name="wl$0_ssid$1_session_timeout" id="wl$0_ssid$1_session_timeout" 
    value='<% nvg_attr_get("wl$0_ssid$1_session_timeout","wl$0_sec_wpa_rule","$1","session_timeout");%>'>

        <div class="row show-grid">
            <div class="span4"><%lang("Radius Server IP Address");%></div>
            <div class="span8">
              <input type="text" name="wl$0_ssid$1_radius_ipaddr" id="wl$0_ssid$1_radius_ipaddr"
                onchange="valid_wl_sec_wpa_radius_ipaddr(this, 'WPA Radius IP')"
                value='<% nvg_attr_get("wl$0_ssid$1_radius_ipaddr", "wl$0_sec_wpa_rule","$1","radius_ipaddr");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Radius Server Port");%></div>
            <div class="span8">
              <input type="text" name="wl$0_ssid$1_radius_port" id="wl$0_ssid$1_radius_port"
                onchange="valid_wl_sec_wpa_radius_port(this, 'WPA Radius port')"
                value='<% nvg_attr_get("wl$0_ssid$1_radius_port",
                "wl$0_sec_wpa_rule","$1","radius_port");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Radius Key"); %></div>
            <div class="span8">
                <script type="text/javascript">
                  document.write('<input type="text" name="wl$0_ssid$1_radius_key" id="wl$0_ssid$1_radius_key" size="17" maxlength="64"');
                  document.write('onChange="valid_wl_sec_wpa_psk_key(this,\'WPA/PSK key\')"');
                  document.write('value="');
                  document.write(decode64('<%b64_nvg_attr_get("wl$0_ssid$1_radius_key","wl$0_sec_wpa_rule","$1","radius_key");%>'));
                  document.write('">');
                </script>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Encryption Method"); %></div>
            <div class="span8">
              <select name="wl$0_ssid$1_crypto" id="wl$0_ssid$1_crypto">
                  <option value="tkip" <%nvg_attr_match("wl$0_ssid$1_crypto", "wl$0_sec_wpa_rule", 
                    "$1", "crypto", "tkip", "selected");%>><% lang("TKIP"); %>
                    </option>
                  <option value="aes" <%nvg_attr_match("wl$0_ssid$1_crypto", "wl$0_sec_wpa_rule", 
                    "$1", "crypto", "aes", "selected");%>><% lang("AES"); %>
                    </option>
                  <option value="mixed" <%nvg_attr_match("wl$0_ssid$1_crypto", "wl$0_sec_wpa_rule", 
                    "$1", "crypto", "mixed", "selected");%>><% lang("Mixed (TKIP+AES)"); %>
                    </option>
              </select>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Rekey Method"); %></div>
            <div class="span8">
              <select name="wl$0_ssid$1_rekey_mode" id="wl$0_ssid$1_rekey_mode"  onchange="init(this.form, $0, $1)">
              <option value="disable" <%nvg_attr_match("wl$0_ssid$1_rekey_mode", 
                "wl$0_sec_wpa_rule", "$1", "rekey_mode", "disable", "selected");%>><% lang("Disable"); %>
                </option>
              <option value="time" <%nvg_attr_match("wl$0_ssid$1_rekey_mode", 
                "wl$0_sec_wpa_rule", "$1", "rekey_mode", "time", "selected");%>><% lang("Time"); %>
                </option>
              <option value="pkt" <%nvg_attr_match("wl$0_ssid$1_rekey_mode", 
                "wl$0_sec_wpa_rule", "$1", "rekey_mode", "pkt", "selected");%>><% lang("Packet Number"); %>
                </option>
              </select>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Rekey Time Interval"); %></div>
            <div class="span8">
              <input type="text" name="wl$0_ssid$1_rekey_time_interval" id="wl$0_ssid$1_rekey_time_interval" 
                value='<% nvg_attr_get("wl$0_ssid$1_rekey_interval", "wl$0_sec_wpa_rule","$1","rekey_time_interval");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Rekey Packet Interval"); %></div>
            <div class="span8">
              <input type="text" name="wl$0_ssid$1_rekey_pkt_interval" id="wl$0_ssid$1_rekey_pkt_interval" 
                value='<% nvg_attr_get("wl$0_ssid$1_rekey_interval", "wl$0_sec_wpa_rule","$1","rekey_pkt_interval");%>'>
              <br/>
              (<%lang("The Key is an ASCII string of 8-63 digits");%>)
            </div>
        </div>
