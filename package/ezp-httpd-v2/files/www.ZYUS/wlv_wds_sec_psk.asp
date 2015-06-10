  <div class="label"><% lang("Encryption Method"); %></div>
  <select name="wl$0_wds$1_crypto">
  <option value="tkip" <%nvg_attr_match("wl$0_wds$1_crypto", "wl$0_wds_sec_psk_rule", "$1", "crypto", "tkip", "selected");%>>TKIP</option>
  <option value="aes" <%nvg_attr_match("wl$0_wds$1_crypto", "wl$0_wds_sec_psk_rule", "$1", "crypto", "aes", "selected");%>>AES</option>
  <option value="mixed" <%nvg_attr_match("wl$0_wds$1_crypto", "wl$0_wds_sec_psk_rule", "$1", "crypto", "mixed", "selected");%>>Mixed (TKIP+AES)</option>
  </select>
  <br/>

  <div class="label"><% lang("Key"); %></div>
<script type="text/javascript">
  document.write('<input type="password" name="wl$0_wds$1_key" size="17" maxlength="64"');
  document.write('onChange="valid_wl_sec_wpa_psk_key(this,\'WDS Key\')"');
  document.write('value="');
  document.write(decode64('<%b64_nvg_attr_get("wl$0_wds$1_key","wl$0_wds_sec_wpa_rule","$1","key");%>'));
  document.write('">');
</script>
  <br/>
  <br/>
  (<%lang("The Key is an ASCII string of 8-63 digits, or a HEX string of 64 digits.");%>)
  <br/>

