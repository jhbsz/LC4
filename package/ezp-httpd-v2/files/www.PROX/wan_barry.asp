<div>

  <div class="setting">
    <div class="label"><%lang("BlackBerry Password");%></div>
    <input name="wan$0_barry_barrypasswd" id='wan$0_barry_barrypasswd' size="25" maxLength="36" onBlur="valid_name(this,'BarryPassword',SPACE_NO)" 
        type="password" value="slide" />
  </div>

  <div class="setting">
    <div class="label"><%lang("APN Type");%></div>

    <div class='radiobutton'><input type="radio" value="0" name="wan$0_barry_apn_type" id="wan$0_barry_apn_type"
      onclick='sel_barry_apn_type("0","$0")'
      <%nvg_attr_match("wan$0_barry_apn_type","wan_barry_rule","$0","apn_type","0","checked");%>>
      <%lang("Service Provider");%> 
    </input></div>
    <div class='radiobutton'><input type="radio" value="1" name="wan$0_barry_apn_type" id="wan$0_barry_apn_type"
      onclick='sel_barry_apn_type("1","$0")'
      <%nvg_attr_match("wan$0_barry_apn_type","wan_barry_rule","$0","apn_type","1","checked");%>>
      <%lang("Manual");%>
    </input></div>
  </div>

 <%wan_barry_show_isp("$0");%>

  <div class="setting">
    <div class="label"><%lang("Access Point Name");%> (APN)</div>
    <input name="wan$0_barry_apn" id='wan$0_barry_apn' size="25" maxLength="36" onBlur="valid_name(this,'APN')" 
        value='<%nvg_attr_get("wan$0_barry_apn", "wan_barry_rule", "$0", "apn");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("User Name");%></div>
    <input name="wan$0_barry_username" id='wan$0_barry_username' size="25" maxLength="36" onBlur="valid_name(this,'User Name')" 
        value='<%nvg_attr_get("wan$0_barry_username", "wan_barry_rule", "$0", "username");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("Password");%></div>
    <input name="wan$0_barry_passwd" id='wan$0_barry_passwd' size="25" maxLength="36" onBlur="valid_name(this,'Password',SPACE_NO)" 
        type="password" value="slide" />
  </div>

  <div class="setting">
    <div class="label"><%lang("Dial Number");%></div>
    <input name="wan$0_barry_dialstr" id='wan$0_barry_dialstr' size="25" maxLength="30" onBlur="valid_name(this,'Dial Number')" 
        value='<%nvg_attr_get("wan$0_barry_dialstr", "wan_barry_rule", "$0", "dialstr");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("TurboLink");%> (<%lang("This might increase data charge");%>)</div>
    <div class='radiobutton'><input type="radio" name="wan$0_barry_turbolink" id="wan$0_barry_turbolink"
      value="1" <%nvg_attr_match("wan$0_barry_turbolink","wan_barry_rule","$0","turbolink","1","checked");%>/>
      <%lang("Enable");%></div>
    <div class='radiobutton'><input type="radio" name="wan$0_barry_turbolink" id="wan$0_barry_turbolink"
      value="0" <%nvg_attr_match("wan$0_barry_turbolink","wan_barry_rule","$0","turbolink","0","checked");%>/>
      <%lang("Disable");%></div>
  </div>

  <input type="hidden" name="wan$0_mtu" value="1500" />
</div>
