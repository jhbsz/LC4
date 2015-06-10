<script language="JavaScript">
wan_objects[$0] = 
[ { name: 'wan$0_proto' }, 
  { name: 'wan$0_mtu' }, 
  { name: 'wan$0_dhcp_hostname' }, 
  { name: 'wan$0_ppp_auth' },
  { name: 'wan$0_ppp_username' },
  { name: 'wan$0_ppp_passwd' },
  { name: 'wan$0_ppp_demand', type: 'radio' },
  { name: 'wan$0_ppp_idletime' }, 
  { name: 'wan$0_ppp_lcp_echo_interval' }, 
  { name: 'wan$0_ppp_lcp_echo_failure' }, 
  { name: 'wan$0_ppp_mtu' },
  { name: 'wan$0_wwan_brand' },
  { name: 'wan$0_wwan_model' },
  { name: 'wan$0_wwan_apn_type' },
  { name: 'wan$0_wwan_location' },
  { name: 'wan$0_wwan_isp' },
  { name: 'wan$0_wwan_apn' },
  { name: 'wan$0_wwan_pin' },
  { name: 'wan$0_wwan_ping' },
  { name: 'wan$0_wwan_mode' },
  { name: 'wan$0_wwan_auth' },
  { name: 'wan$0_wwan_username' },
  { name: 'wan$0_wwan_passwd' },
  { name: 'wan$0_wwan_dialstr' },
  { name: 'wan$0_wwan_demand', type: 'radio' },
  { name: 'wan$0_wwan_turbolink', type: 'radio' },
  { name: 'wan$0_htc_turbolink', type: 'radio' },
  { name: 'wan$0_directip_turbolink', type: 'radio' },
  { name: 'wan$0_wwan_idletime' }, 
  { name: 'wan$0_wwan_lcp_echo_interval' }, 
  { name: 'wan$0_wwan_lcp_echo_failure' }, 
  { name: 'wan$0_wwan_mtu' },
  { name: 'wan$0_static_ipaddr' }, 
  { name: 'wan$0_static_mask' }, 
  { name: 'wan$0_static_gateway' }, 
  { name: 'wan$0_static_dns1' }, 
  { name: 'wan$0_static_dns2' },
  { name: 'wan$0_status' },
  { name: 'wan$0_connect' },
  { name: 'wan$0_disconnect' },
  { name: 'wan$0_bigpond_enable', type: 'radio' },
  { name: 'wan$0_bigpond_server' },
  { name: 'wan$0_bigpond_username' },
  { name: 'wan$0_bigpond_passwd' }
];

</script>

<input type="hidden" name="wan$0_now_proto" id="wan$0_now_proto" value='<%nvg_get("wan$0_proto");%>' />
<fieldset>
  <legend><%increase("$0", "1");%> <% lang("WAN"); %></legend>
  <div class="setting">
    <div class="label"><% lang("WAN"); %></div>
    <input type="radio" value="1" name="wan$0_enable" onclick="init()"
          <%nvg_attr_match("wan$0_enable","wan_main_rule","$0","enable","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="wan$0_enable" onclick="init()"
          <%nvg_attr_match("wan$0_enable","wan_main_rule","$0","enable","0","checked");%>><%lang("Disable");%>
    </input>
  </div>

  <div class="setting">
    <div class="label"><% lang("Connection Type"); %></div>
    <select name="wan$0_proto" id='wan$0_proto' onchange="SelWAN(this.form)">
<%wan_show_wan_type_html_start("$0");%>
      <option value="dhcp" <% nvg_match("wan$0_proto", "dhcp", "selected"); %>>DHCP</option>
      <option value="static" <% nvg_match("wan$0_proto", "static", "selected"); %>>Static IP</option>
      <option value="pppoe" <% nvg_match("wan$0_proto", "pppoe", "selected"); %>>PPPoE</option>
<%wan_show_wan_type_html_end("$0");%>
<%wwan_show_wan_type_html_start("$0", "wwan");%>
      <option value="wwan" <% nvg_match("wan$0_proto", "wwan", "selected"); %>>3G Mobile Internet</option>
<%wwan_show_wan_type_html_end("$0", "wwan");%>
    </select>
  </div>
  <% wan_show_type_setting($0); %>
  <input type="hidden" name="wan$0_vpnc_enable" value="0"/>
  <input type="hidden" name="wan$0_stp" value="0"/>

</fieldset><br />
