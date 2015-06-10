<input type="hidden" name="wl$0_ssid$1_keytype" id="wl$0_ssid$1_keytype" 
  value='<% nvg_attr_get("wl$0_ssid$1_keytype" ,"wl$0_sec_wep_rule", "$1", "keytype");%>'>
<input type="hidden" name="wl$0_ssid$1_encmode" id="wl$0_ssid$1_encmode" 
  value='<% nvg_attr_get("wl$0_ssid$1_encmode" ,"wl$0_sec_wep_rule", "$1", "encmode");%>'>

<div style="display: none;">
<div class="label"><% lang("Key Index"); %></div>
<select name="wl$0_ssid$1_key_index" id="wl$0_ssid$1_key_index">
<option value="1" <% nvg_attr_match("wl$0_ssid$1_key_index", 
    "wl$0_sec_wep_rule", "$1", "key_index", "1", "selected");%>>
  <% lang("1"); %></option>
<option value="2" <% nvg_attr_match("wl$0_ssid$1_key_index",
    "wl$0_sec_wep_rule", "$1", "key_index", "2", "selected");%>>
  <% lang("2"); %></option>
<option value="3" <% nvg_attr_match("wl$0_ssid$1_key_index",
    "wl$0_sec_wep_rule", "$1", "key_index", "3", "selected");%>>
  <% lang("3"); %></option>
<option value="4" <% nvg_attr_match("wl$0_ssid$1_key_index",
    "wl$0_sec_wep_rule", "$1", "key_index", "4", "selected");%>>
  <% lang("4"); %></option>
</select>
<br/>
</div>

<div class="label"><% lang("Password"); %></div> 
<script type="text/javascript">
  document.write('<input type="password" name="wl$0_ssid$1_key1" id="wl$0_ssid$1_key1" size="17" maxlength="32"');
  //document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Password\',0)"');
  document.write('value="');
  document.write(decode64('<% b64_nvg_attr_get("wl$0_ssid$1_key1","wl$0_sec_wep_rule","$1","key1");%>'));
  document.write('">');
</script>
<div id="WEPErrorMsg" style="display:inline;color:red;"></div>
<br/>

<div class="label"><% lang("Retype new password"); %></div> 
<script type="text/javascript">
  document.write('<input type="password" name="wl$0_ssid$1_key1c" id="wl$0_ssid$1_key1c" size="17" maxlength="32"');
  //document.write('onBlur="valid_wl_sec_wep_key(this,\'WEP Retype Password\',1)"');
  document.write('value="');
  document.write(decode64('<% b64_nvg_attr_get("wl$0_ssid$1_key1","wl$0_sec_wep_rule","$1","key1");%>'));
  document.write('">');
</script>
  <br/>
<div class="label">&nbsp</div>( <%lang("5 or 13 characters");%> )


<div style="display: none;">
<div class="label"><% lang("Key"); %> 2</div> 
<script type="text/javascript">
  document.write('<input type="text" name="wl$0_ssid$1_key2" id="wl$0_ssid$1_key2" size="17" maxlength="32"');
  document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Key 2\',0)"');
  document.write('value="');
  document.write(decode64('<% b64_nvg_attr_get("wl$0_ssid$1_key2","wl$0_sec_wep_rule","$1","key2");%>'));
  document.write('">');
</script>
  <br/>

<div class="label"><% lang("Key"); %> 3</div> 
<script type="text/javascript">
  document.write('<input type="text" name="wl$0_ssid$1_key3" id="wl$0_ssid$1_key3" size="17" maxlength="32"');
  document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Key 3\',0)"');
  document.write('value="');
  document.write(decode64('<% b64_nvg_attr_get("wl$0_ssid$1_key3","wl$0_sec_wep_rule","$1","key3");%>'));
  document.write('">');
</script>
  <br/>

<div class="label"><% lang("Key"); %> 4</div> 
<script type="text/javascript">
  document.write('<input type="text" name="wl$0_ssid$1_key4" id="wl$0_ssid$1_key4" size="17" maxlength="32"');
  document.write('onChange="valid_wl_sec_wep_key(this,\'WEP Key 4\',0)"');
  document.write('value="');
  document.write(decode64('<% b64_nvg_attr_get("wl$0_ssid$1_key4","wl$0_sec_wep_rule","$1","key4");%>'));
  document.write('">');
</script>
  <br/>
</div>

<br/>

