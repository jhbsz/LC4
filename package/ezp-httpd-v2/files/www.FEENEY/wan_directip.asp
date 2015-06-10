<script language="JavaScript">
wan_bigpond_objects[$0] = [
  { name: 'wan$0_bigpond_server' },
  { name: 'wan$0_bigpond_username' },
  { name: 'wan$0_bigpond_passwd' }
];
</script>

<div>
  <%wan_wwan_show_modem("$0", "2");%>

  <div class="setting">
    <div class="label"><%lang("APN Type");%></div>

    <input type="radio" value="0" name="wan$0_wwan_apn_type" id="wan$0_wwan_apn_type"
      onclick='sel_apn_type("0","$0")'
      <%nvg_attr_match("wan$0_wwan_apn_type","wan_wwan_rule","$0","apn_type","0","checked");%>>
      <%lang("Service Provider");%> 
    </input>
    <input type="radio" value="1" name="wan$0_wwan_apn_type" id="wan$0_wwan_apn_type"
      onclick='sel_apn_type("1","$0")'
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
    <input name="wan$0_wwan_pin" id='wan$0_wwan_pin' size="25" maxLength="4" onBlur="valid_name(this,'PIN')" 
        value='<%nvg_attr_get("wan$0_wwan_pin", "wan_wwan_rule", "$0", "pin");%>' />
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

  <div class="setting">
    <div class="label"><%lang("WAN MTU");%></div>
    <input name="wan$0_mtu" size="4" maxLength="4"
        value='<%nvg_attr_get("wan$0_mtu", "wan_main_rule", "$0", "mtu");%>'
        onblur="valid_number(this, 'WAN MTU')" /> <%lang("Bytes");%>
  </div>

  <div class="setting">
    <div class="label"><%lang("Bigpond Login");%></div>
    <input type="radio" value="1" name="wan$0_bigpond_enable" id="wan$0_bigpond_enable"
      onclick='sel_bigpond_login("1","$0")'
      <%nvg_attr_match("wan$0_bigpond_enable","wan_bigpond_rule","$0","enable","1","checked");%>>
      <%lang("Enable");%> 
    </input>
    <input type="radio" value="0" name="wan$0_bigpond_enable" id="wan$0_bigpond_enable"
      onclick='sel_bigpond_login("0","$0")'
      <%nvg_attr_match("wan$0_bigpond_enable","wan_bigpond_rule","$0","enable","0","checked");%>>
      <%lang("Disable");%> 
    </input>
  </div>

  <div class="setting">
    <div class="label"><%lang("Bigpond Login Server");%></div>
    <select name="wan$0_bigpond_server" id="wan$0_bigpond_server">
      <option value="61.9.208.13" <%nvg_attr_match("wan$0_bigpond_server", "wan_bigpond_rule", "$0", "server", "61.9.208.13", "selected"); %>><%lang("Queensland (61.9.208.13)");%></option>
      <option value="61.9.192.13" <%nvg_attr_match("wan$0_bigpond_server", "wan_bigpond_rule", "$0", "server", "61.9.192.13", "selected"); %>><%lang("New South Wales (61.9.192.13)");%></option>
      <option value="61.9.128.13" <%nvg_attr_match("wan$0_bigpond_server", "wan_bigpond_rule", "$0", "server", "61.9.128.13", "selected"); %>><%lang("Victoria (61.9.128.13)");%></option>
      <option value="61.9.224.13" <%nvg_attr_match("wan$0_bigpond_server", "wan_bigpond_rule", "$0", "server", "61.9.224.13", "selected"); %>><%lang("South Australia (61.9.224.13)");%></option>
      <option value="61.9.240.13" <%nvg_attr_match("wan$0_bigpond_server", "wan_bigpond_rule", "$0", "server", "61.9.240.13", "selected"); %>><%lang("Western Australi (61.9.240.13)");%></option>
    </select>
  </div>
  <div class="setting">
    <div class="label"><%lang("Bigpond Login User Name");%></div>
    <input name="wan$0_bigpond_username" id='wan$0_bigpond_username' size="25" maxLength="36"
        onBlur="valid_name(this,'Bigpond Login User Name')" 
        value='<%nvg_attr_get("wan$0_bigpond_username", "wan_bigpond_rule", "$0", "username");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("Bigpond Login Password");%></div>
    <input name="wan$0_bigpond_passwd" id='wan$0_bigpond_passwd' size="25" maxLength="36"
        onBlur="valid_name(this,'Bigpond Login Password',SPACE_NO)" 
        type="password" value="slide" />
  </div>

  <div class="setting">
    <div class="label"><%lang("TurboLink");%> (<%lang("Enable it might increase your 3G data charge");%>)</div>
    <input type="radio" name="wan$0_directip_turbolink" id="wan$0_directip_turbolink"
      value="1" <%nvg_attr_match("wan$0_directip_turbolink","wan_wwan_rule","$0","turbolink","1","checked");%>/>
      <%lang("Enable");%>
    <input type="radio" name="wan$0_directip_turbolink" id="wan$0_directip_turbolink"
      value="0" <%nvg_attr_match("wan$0_directip_turbolink","wan_wwan_rule","$0","turbolink","0","checked");%>/>
      <%lang("Disable");%>
  </div>
</div>
