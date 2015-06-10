        <div class="row show-grid">
            <div class="span4"><% lang("Encryption Method"); %></div>
            <div class="span8">
              <select name="wl$0_wds$1_crypto" id="wl$0_wds$1_crypto">
              <option value="tkip" <%nvg_attr_match("wl$0_wds$1_crypto", "wl$0_wds_sec_psk_rule", "$1", "crypto", "tkip", "selected");%>>TKIP</option>
              <option value="aes" <%nvg_attr_match("wl$0_wds$1_crypto", "wl$0_wds_sec_psk_rule", "$1", "crypto", "aes", "selected");%>>AES</option>
              <option value="mixed" <%nvg_attr_match("wl$0_wds$1_crypto", "wl$0_wds_sec_psk_rule", "$1", "crypto", "mixed", "selected");%>>Mixed (TKIP+AES)</option>
              </select>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Key"); %></div>
            <div class="span8">
                <script type="text/javascript">
                  document.write('<input type="text" name="wl$0_wds$1_key" id="wl$0_wds$1_key" size="17" maxlength="64"');
                  document.write('onChange="valid_wl_sec_wpa_psk_key(this,\'WDS Key\')"');
                  document.write('value="');
                  document.write(decode64('<%b64_nvg_attr_get("wl$0_wds$1_key","wl$0_wds_sec_wpa_rule","$1","key");%>'));
                  document.write('">');
                </script>
                  <br/>
                  (<%lang("The Key is an ASCII string of 8-63 digits");%>)
            </div>
        </div>
