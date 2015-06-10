        <div class="row-fluid">
            <label><% lang("Change Password"); %></label>
            <script type="text/javascript">
                document.write('<input type="password"  style="width: 175px;" name="wl0_ssid0_key" id="wl0_ssid0_key" size="17" maxlength="32"');
                document.write('value="');
                document.write(decode64('<% b64_nvg_attr_get("wl0_ssid0_key","wl0_sec_wep_rule","0","key1");%>'));
                document.write('">');
            </script>
            <div id="WEPErrorMsg" style="display:inline;color:red;"></div>
        </div>    
                   
		<div class="row-fluid">
			<label><% lang("Re-type New Password"); %></label>
            <script type="text/javascript">
                document.write('<input type="password" style="width: 175px;" name="wl0_ssid0_keyc" id="wl0_ssid0_keyc" size="17" maxlength="32"');
                document.write('value="');
                document.write(decode64('<% b64_nvg_attr_get("wl0_ssid0_key","wl0_sec_wep_rule","0","key1");%>'));
                document.write('">');
            </script>   
        </div>
        <span class="help-block">( <%lang("5 or 13 characters");%> )</span>   

