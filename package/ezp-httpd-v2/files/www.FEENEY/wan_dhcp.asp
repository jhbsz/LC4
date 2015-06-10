<script language="JavaScript">
wan_bigpond_objects[$0] = [
  { name: 'wan$0_bigpond_server' },
  { name: 'wan$0_bigpond_username' },
  { name: 'wan$0_bigpond_passwd' }
];
</script>
<div>
  <div class="setting">
    <div class="label"><%lang("Host Name");%></div>
    <input name="wan$0_dhcp_hostname" size="25" maxLength="30" onBlur="valid_name(this,'Host Name')" 
        value='<%nvg_attr_get("wan$0_dhcp_hostname", "wan_dhcp_rule", "$0", "hostname");%>'
        onblur='valid_name(this, "Host Name")' />
  </div>

  <div class="setting">
    <div class="label"><% lang("MTU"); %></div>
    <input size="6" maxLength="4" name="wan$0_mtu" 
            value='<%nvg_attr_get("wan$0_mtu", "wan_main_rule", "$0", "mtu");%>'
            onblur='valid_number(this, "MTU")' /> <%lang("Bytes");%>
  </div>

  <div class="setting">
    <div class="label"></div>
    <input type="hidden" value="1" name="wan$0_bigpond_enable" id="wan$0_bigpond_enable"
      onclick='sel_bigpond_login("1","$0")'
      <%nvg_attr_match("wan$0_bigpond_enable","wan_bigpond_rule","$0","enable","1","checked");%>>
    </input>
    <input type="hidden" value="0" name="wan$0_bigpond_enable" id="wan$0_bigpond_enable"
      onclick='sel_bigpond_login("0","$0")'
      <%nvg_attr_match("wan$0_bigpond_enable","wan_bigpond_rule","$0","enable","0","checked");%>>
    </input>
  </div>

<!--
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
-->
  <input type="hidden" name="wan$0_dhcp_domain" value='<%nvg_attr_get("wan$0_dhcp_domain", "wan_dhcp_rule", "$0", "domain");%>' />
  <input type="hidden" name="wan$0_dhcp_lease" value=<%nvg_attr_get("wan$0_dhcp_lease","wan_dhcp_rule", "$0", "lease");%> />
</div>
