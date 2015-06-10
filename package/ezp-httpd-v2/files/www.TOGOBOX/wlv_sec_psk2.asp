
        <div class="row-fluid">
            <label><% lang("Change Password"); %></label>
            <script type="text/javascript">
                document.write('<input type="password" style="width: 175px;" name="wl0_ssid0_key" id="wl0_ssid0_key" size="17" maxlength="64"');
                document.write('value="');
                document.write(decode64('<%b64_nvg_attr_get("wl0_ssid0_key","wl0_sec_wpa2_rule","0","key");%>'));
                document.write('">');
            </script>
            <div id="WPAErrorMsg" style="display:inline;color:red;"></div>    
        </div>    
                   
		<div class="row-fluid">
			<label><% lang("Re-type New Password"); %></label>
            <script type="text/javascript">
                document.write('<input type="password" style="width: 175px;" name="wl0_ssid0_keyc" id="wl0_ssid0_keyc" size="17" maxlength="64"');
                document.write('value="');
                document.write(decode64('<%b64_nvg_attr_get("wl0_ssid0_key","wl$0_sec_wpa2_rule","0","key");%>'));
                document.write('">');
            </script>     
        </div>
        <span class="help-block">( <%lang("8-63 characters");%> )</span>   

		


