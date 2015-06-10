
    <div class="row show-grid">
        <div class="span4"><% lang("Key Index"); %></div>
        <div class="span8">
            <select name="wl$0_apcli$1_key_index" id="wl$0_apcli$1_key_index">
            <option value="1" <% nvg_attr_match("wl$0_apcli$1_key_index", 
                "wl$0_apcli_sec_wep_rule", "$1", "key_index", "1", "selected");%>>
              <% lang("1"); %></option>
            <option value="2" <% nvg_attr_match("wl$0_apcli$1_key_index",
                "wl$0_apcli_sec_wep_rule", "$1", "key_index", "2", "selected");%>>
              <% lang("2"); %></option>
            <option value="3" <% nvg_attr_match("wl$0_apcli$1_key_index",
                "wl$0_apcli_sec_wep_rule", "$1", "key_index", "3", "selected");%>>
              <% lang("3"); %></option>
            <option value="4" <% nvg_attr_match("wl$0_apcli$1_key_index",
                "wl$0_apcli_sec_wep_rule", "$1", "key_index", "4", "selected");%>>
              <% lang("4"); %></option>
            </select>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Key"); %> 1</div>
        <div class="span8">
            <script type="text/javascript">
              document.write('<input type="text" name="wl$0_apcli$1_key1" id="wl$0_apcli$1_key1" size="17" maxlength="32"');
              document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Key 1\')"');
              document.write('value="');
              document.write(decode64('<% b64_nvg_attr_get("wl$0_apcli$1_key1","wl$0_apcli_sec_wep_rule","$1","key1");%>'));
              document.write('">');
            </script>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Key"); %> 2</div>
        <div class="span8">
            <script type="text/javascript">
              document.write('<input type="text" name="wl$0_apcli$1_key2" id="wl$0_apcli$1_key2" size="17" maxlength="32"');
              document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Key 2\')"');
              document.write('value="');
              document.write(decode64('<% b64_nvg_attr_get("wl$0_apcli$1_key2","wl$0_apcli_sec_wep_rule","$1","key2");%>'));
              document.write('">');
            </script>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Key"); %> 3</div>
        <div class="span8">
            <script type="text/javascript">
              document.write('<input type="text" name="wl$0_apcli$1_key3" id="wl$0_apcli$1_key3" size="17" maxlength="32"');
              document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Key 3\')"');
              document.write('value="');
              document.write(decode64('<% b64_nvg_attr_get("wl$0_apcli$1_key3","wl$0_apcli_sec_wep_rule","$1","key3");%>'));
              document.write('">');
            </script>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("Key"); %> 4</div>
        <div class="span8">
            <script type="text/javascript">
              document.write('<input type="text" name="wl$0_apcli$1_key4" id="wl$0_apcli$1_key4" size="17" maxlength="32"');
              document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Key 4\')"');
              document.write('value="');
              document.write(decode64('<% b64_nvg_attr_get("wl$0_apcli$1_key4","wl$0_apcli_sec_wep_rule","$1","key4");%>'));
              document.write('">');
            </script>
            <br/>
            (<%lang("The WEP Keys are ASCII strings of 5/13 digits, or HEX strings of 10/26 digits.");%>)
        </div>
    </div>
