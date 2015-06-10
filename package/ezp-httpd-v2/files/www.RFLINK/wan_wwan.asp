<div>

  <%wan_wwan_show_modem("$0", "1");%>

  <div class="setting">
    <div class="label"><%lang("APN Type");%></div>

    <input type="radio" value="2" name="wan$0_wwan_apn_type" id="wan$0_wwan_apn_type"
      onclick='sel_apn_type("2","$0","1")'
      <%nvg_attr_match("wan$0_wwan_apn_type","wan_wwan_rule","$0","apn_type","2","checked");%>>
      <%lang("Auto");%> 
    </input>
    <input type="radio" value="0" name="wan$0_wwan_apn_type" id="wan$0_wwan_apn_type"
      onclick='sel_apn_type("0","$0","1")'
      <%nvg_attr_match("wan$0_wwan_apn_type","wan_wwan_rule","$0","apn_type","0","checked");%>>
      <%lang("Service Provider");%> 
    </input>
    <input type="radio" value="1" name="wan$0_wwan_apn_type" id="wan$0_wwan_apn_type"
      onclick='sel_apn_type("1","$0","1")'
      <%nvg_attr_match("wan$0_wwan_apn_type","wan_wwan_rule","$0","apn_type","1","checked");%>>
      <%lang("Manual");%>
    </input>
  </div>

  <%wan_wwan_show_isp("$0");%>

  <div class="setting">
    <div class="label"><%lang("Access Point Name");%> (APN)</div>
    <input name="wan$0_wwan_apn" id='wan$0_wwan_apn' size="25" maxLength="36" onBlur="valid_name(this,'APN')" 
        value='<%nvg_attr_get("wan$0_wwan_apn", "wan_wwan_rule", "$0", "apn");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("Personal Identification Number");%> (PIN)</div>
    <input name="wan$0_wwan_pin" id='wan$0_wwan_pin' size="25" maxLength="8" onBlur="valid_name(this,'PIN')" 
        value='<%nvg_attr_get("wan$0_wwan_pin", "wan_wwan_rule", "$0", "pin");%>' />
  </div>

  <div class="setting">
    <div class="label"><% lang("Authentication"); %></div>
    <select name="wan$0_wwan_auth" id='wan$0_wwan_auth' >
      <option value="chap" <%nvg_attr_match("wan$0_wwan_auth","wan_wwan_rule","$0","auth","chap","selected");%>>CHAP (<%lang("Auto");%>)</option>
      <option value="pap" <%nvg_attr_match("wan$0_wwan_auth","wan_wwan_rule","$0","auth","pap","selected");%>>PAP</option>
      <option value="none" <%nvg_attr_match("wan$0_wwan_auth","wan_wwan_rule","$0","auth","none","selected");%>><%lang("None");%></option>
    </select>
  </div>

  <div class="setting">
    <div class="label"><%lang("User Name");%></div>
    <input name="wan$0_wwan_username" id='wan$0_wwan_username' size="25" maxLength="36" onBlur="valid_name(this,'User Name')" 
        value='<%nvg_attr_get("wan$0_wwan_username", "wan_wwan_rule", "$0", "username");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("Password");%></div>
    <input name="wan$0_wwan_passwd" id='wan$0_wwan_passwd' size="25" maxLength="36" onBlur="valid_name(this,'Password',SPACE_NO)" 
        type="password" value="slide" />
  </div>

  <div class="setting">
    <div class="label"><%lang("Dial Number");%></div>
    <input name="wan$0_wwan_dialstr" id='wan$0_wwan_dialstr' size="25" maxLength="30" onBlur="valid_name(this,'Dial Number')" 
        value='<%nvg_attr_get("wan$0_wwan_dialstr", "wan_wwan_rule", "$0", "dialstr");%>' />
  </div>

  <div class="setting">
    <div class="label"><% lang("Connection Mode"); %></div>
    <select name="wan$0_wwan_mode"> 
      <option value="auto" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "auto", "selected"); %>><%lang("Auto");%></option>
      <option value="hsdpa" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "hsdpa", "selected"); %>>HSDPA / 3.5G</option>
      <option value="umts" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "umts", "selected"); %>>UMTS / 3G</option>
      <option value="edge" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "edge", "selected"); %>>EDGE / 2.5G</option>
      <option value="gprs" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "gprs", "selected"); %>>GPRS / 2G</option>
    </select>
  </div>

  <input type='hidden' name='wan$0_wwan_demand' value='0'/>
  <input type='hidden' name='wan$0_wwan_idletime' value='300'/>

  <div class="setting">
    <div class="label"><%lang("PPP Echo Interval");%></div>
    <input class="num" size="4" maxLength="4" name="wan$0_wwan_lcp_echo_interval"
         id="wan$0_wwan_lcp_echo_interval"
        value='<%nvg_attr_get("wan$0_wwan_lcp_echo_interval", "wan_wwan_rule", "$0", "redialperiod");%>'
        onblur="valid_number(this, 'PPP Echo Interval')" /> <%lang("Seconds");%> (20 ~ 180)
  </div>

  <div class="setting">
    <div class="label"><%lang("PPP Retry Threshold");%></div>
    <input class="num" size="4" maxLength="4" name="wan$0_wwan_lcp_echo_failure" 
        id="wan$0_wwan_lcp_echo_failure"
        value='<%nvg_attr_get("wan$0_wwan_lcp_echo_failure", "wan_wwan_rule", "$0", "echo_failure");%>'
        onblur="valid_number(this, 'PPP Retry Threshold')" /> <%lang("Time(s)");%> (3 ~ 50)
  </div>

  <div class="setting">
    <div class="label"><%lang("Mobile WAN MTU");%></div>
    <input name="wan$0_wwan_mtu" size="4" maxLength="4"
        value='<%nvg_attr_get("wan$0_wwan_mtu", "wan_wwan_rule", "$0", "mtu");%>'
        onblur="valid_number(this, 'Mobile WAN MTU')" /> <%lang("Bytes");%> (68-1492)
  </div>


  <div class="setting">
    <div class="label"><%lang("TurboLink");%> </div>
    <input type="radio" name="wan$0_wwan_turbolink" id="wan$0_wwan_turbolink"
      value="1" <%nvg_attr_match("wan$0_wwan_turbolink","wan_wwan_rule","$0","turbolink","1","checked");%>/>
      <%lang("Enable");%>
    <input type="radio" name="wan$0_wwan_turbolink" id="wan$0_wwan_turbolink"
      value="0" <%nvg_attr_match("wan$0_wwan_turbolink","wan_wwan_rule","$0","turbolink","0","checked");%>/>
      <%lang("Disable");%>
  </div>


  <input type="hidden" name="wan$0_mtu" value="1500" />
</div>
