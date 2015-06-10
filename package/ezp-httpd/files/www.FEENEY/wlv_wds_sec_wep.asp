  <div class="label"><% lang("Key"); %></div>
<script type="text/javascript">
  document.write('<input type="password" name="wl$0_wds$1_key" size="17" maxlength="32"');
  document.write('onChange="valid_wl_sec_wep_key(this,\'WDS Key\')"');
  document.write('value="');
  document.write(decode64('<%b64_nvg_attr_get("wl$0_wds$1_key","wl$0_wds_sec_wep_rule", "$1", "key");%>'));
  document.write('">');
</script>
  <br/>
<br/>
(<%lang("The WEP Keys are ASCII strings of 5/13 digits, or HEX strings of 10/26 digits.");%>)
<br/>

