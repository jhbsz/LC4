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
  { name: 'wan$0_htc_hostname'},
  { name: 'wan$0_htc_mtu'},
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
  { name: 'wan$0_bigpond_passwd' },
  { name: 'wan$0_barry_barrypasswd' },
  { name: 'wan$0_barry_apn_type' },
  { name: 'wan$0_barry_location' },
  { name: 'wan$0_barry_isp' },
  { name: 'wan$0_barry_apn' },
  { name: 'wan$0_barry_username' },
  { name: 'wan$0_barry_passwd' },
  { name: 'wan$0_barry_dialstr' },
  { name: 'wan$0_barry_turbolink', type: 'radio' },
  { name: 'wan$0_iphone_hostname' }, 
  { name: 'wan$0_iphone_mtu' }, 
  { name: 'wan$0_iphone_turbolink', type: 'radio' },
  { name: 'wan$0_vpnc_enable' }, 
  { name: 'wan$0_vpnc_type' }, 
  { name: 'wan$0_vpnc_username' }, 
  { name: 'wan$0_vpnc_pw' }, 
  { name: 'wan$0_vpnc_host' }, 
  { name: 'wan$0_vpnc_encmode' }
];

</script>

<input type="hidden" name="wan$0_now_proto" id="wan$0_now_proto" value='<%nvg_get("wan$0_proto");%>' />
        <div class="row">
            <div class="span12">
                <legend>$1</legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("WAN"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="wan$0_enable" id="wan$0_enable" onclick="init()"
                                  <%nvg_attr_match("wan$0_enable","wan_main_rule","$0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="wan$0_enable" id="wan$0_enable" onclick="init()"
                                  <%nvg_attr_match("wan$0_enable","wan_main_rule","$0","enable","0","checked");%>><%lang("Disable");%>
                        </label>                        
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Connection Type"); %></div>
                    <div class="span8">
                        <select name="wan$0_proto" id='wan$0_proto' onchange="SelWAN(this.form)">
                          <% wan_show_proto_type($0);%>
                        </select>
                    </div>
                </div>
                <% wan_show_type_setting($0); %>

                <%prod_show_html_start("vpnc", "0", "7");%>
                <div class="row show-grid">
                    <div class="span4"><% lang("VPN Client"); %></div>
                    <div class="span8">
                         <label class="radio inline">
                            <input type="radio" value="1" name="wan$0_vpnc_enable" id="wan$0_vpnc_enable" onchange="SelWAN(this.form)"
                                  <%nvg_attr_match("wan$0_vpnc_enable","wan_pptp_l2tp_rule","$0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="wan$0_vpnc_enable" id="wan$0_vpnc_enable" onchange="SelWAN(this.form)"
                                  <%nvg_attr_match("wan$0_vpnc_enable","wan_pptp_l2tp_rule","$0","enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>
                 <% wan_show_vpnc_type_setting($0); %>
                <%prod_show_html_end("vpnc", "0", "7");%>
                  <input type="hidden" name="wan$0_stp" value="0"/>
        
            </div>
        </div><!-- row -->
        <br>
