        <div class="row show-grid">
            <div class="span4"><% lang("Key"); %></div>
            <div class="span8">
                <script type="text/javascript">
                  document.write('<input type="text" name="wl$0_wds$1_key" id="wl$0_wds$1_key" size="17" maxlength="32"');
                  document.write('onChange="valid_wl_sec_wep_key(this,\'WDS Key\')"');
                  document.write('value="');
                  document.write(decode64('<%b64_nvg_attr_get("wl$0_wds$1_key","wl$0_wds_sec_wep_rule", "$1", "key");%>'));
                  document.write('">');
                </script>
                <br/>
                (<%lang("The WEP Keys are ASCII strings of 5/13 digits, or HEX strings of 10/26 digits.");%>)
            </div>
        </div>
