        <script language="JavaScript">
        var wan_objects = new Array(<%nv_get("wan_num");%>);
        wan_objects[$0] = 
        [ { name: 'wan$0_proto' }, 
          { name: 'wan$0_mtu' }, 
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
          { name: 'wan$0_directip_turbolink', type: 'radio' },
          { name: 'wan$0_wwan_idletime' }, 
          { name: 'wan$0_wwan_lcp_echo_interval' }, 
          { name: 'wan$0_wwan_lcp_echo_failure' }, 
          { name: 'wan$0_wwan_mtu' },
          { name: 'wan$0_status' },
          { name: 'wan$0_connect' },
          { name: 'wan$0_disconnect' },
        ];

        </script>

        <input type="hidden" name="wan$0_now_proto" id="wan$0_now_proto" value='<%nvg_get("wan$0_proto");%>' />
        <input type="hidden" name="wan$0_proto" id="wan$0_proto" value="wwan" />
        <input type="hidden" name="wanupnp_enable" id="wanupnp_enable" value="1" />
        <input type="hidden" name="wan$0_vpnc_enable" id="wan$0_vpnc_enable" value="0" />
        <input type="hidden" name="wwan$0_wwan_brand" id="wan$0_wwan_brand" value="0" />
        <input type="hidden" name="wan$0_wwan_model" id="wan$0_wwan_model" value="0" />

                <div id="hide_wan_enable_div">
                            <input type="radio" value="1" name="wan$0_enable" id="wan$0_enable" onclick="init()"
                                  <%nvg_attr_match("wan$0_enable","wan_main_rule","$0","enable","1","checked");%>><%lang("Enable");%>
                </div>
            
                <div class="row-fluid">
                    <label><%lang("APN Type");%></label>
                        <div class="control-group">
                        <div class="controls" style="margin-left:0px;">
                        <label style="display:inline;">
                            <input type="radio" value="2" name="wan$0_wwan_apn_type" id="wan$0_wwan_apn_type"
                                onclick='sel_apn_type("2","$0","1")'
                                <%nvg_attr_match("wan$0_wwan_apn_type","wan_wwan_rule","$0","apn_type","2","checked");%>/>
                            <span class="lbl"><%lang("Auto");%></span>
                        </label>
                        <label style="display:inline;">

                            <input type="radio" value="1" name="wan$0_wwan_apn_type" id="wan$0_wwan_apn_type"
                                onclick='sel_apn_type("1","$0","1")'
                                <%nvg_attr_match("wan$0_wwan_apn_type","wan_wwan_rule","$0","apn_type","1","checked");%>/>
                            <span class="lbl"><%lang("Manual");%></span>
                        </label>
                        </div>
                        </div>
                </div>
                
                <input type="hidden" name='wan$0_wwan_location' id='wan$0_wwan_location' value='<%nvg_attr_get("wan$0_wwan_location", "wan_wwan_rule", "$0", "location");%>'/>
                <input type="hidden" name='wan$0_wwan_isp' id='wan$0_wwan_isp' value='<%nvg_attr_get("wan$0_wwan_isp", "wan_wwan_rule", "$0", "isp");%>'/>

                <div class="row-fluid">
                    <label><%lang("Access Point Name");%> (<%lang("APN");%>)</label>
                        <input type="text" name="wan$0_wwan_apn" id='wan$0_wwan_apn' size="25" maxLength="36" onBlur="valid_name(this,'APN')" 
                            value='<%nvg_attr_get("wan$0_wwan_apn", "wan_wwan_rule", "$0", "apn");%>' />
                </div>


                <div class="row-fluid">
                    <label><%lang("PIN");%> </label>
                        <input type="text" name="wan$0_wwan_pin" id='wan$0_wwan_pin' size="25" maxLength="8" onBlur="valid_name(this,'PIN')" 
                            value='<%nvg_attr_get("wan$0_wwan_pin", "wan_wwan_rule", "$0", "pin");%>' />
                </div>
                <!--
                <div class="row show-grid">
                    <div class="span2"><% lang("Authentication"); %></div>
                    <div class="span4">
                        <select name="wan$0_wwan_auth" id='wan$0_wwan_auth' >

                          <option value="chap" <%nvg_attr_match("wan$0_wwan_auth","wan_wwan_rule","$0","auth","chap","selected");%>>CHAP (<%lang("Auto");%>)</option>
                          <option value="pap" <%nvg_attr_match("wan$0_wwan_auth","wan_wwan_rule","$0","auth","pap","selected");%>>PAP</option>
                          <option value="none" <%nvg_attr_match("wan$0_wwan_auth","wan_wwan_rule","$0","auth","none","selected");%>><%lang("None");%></option>
                        </select>
                    </div>
                </div>
                -->

                <input type="hidden" name='wan$0_wwan_auth' id='wan$0_wwan_auth' value='<%nvg_attr_get("wan$0_wwan_auth", "wan_wwan_rule", "$0", "auth");%>'/>

                <div class="row-fluid">
                    <label><%lang("User Name");%></label>
                        <input type="text" name="wan$0_wwan_username" id='wan$0_wwan_username' size="25" maxLength="36" onBlur="valid_name(this,'User Name')" 
                            value='<%nvg_attr_get("wan$0_wwan_username", "wan_wwan_rule", "$0", "username");%>' />
                </div>

                <div class="row-fluid">
                    <label><%lang("Change Password");%></label>
                        <input name="wan$0_wwan_passwd" id='wan$0_wwan_passwd' size="25" maxLength="36" onBlur="valid_name(this,'Password',SPACE_NO)" 
                            type="password" value="slide" />
                </div>

                <div class="row-fluid">
                    <label><%lang("Dial Number");%></label>
                        <input type="text" name="wan$0_wwan_dialstr" id='wan$0_wwan_dialstr' size="25" maxLength="30" onBlur="valid_name(this,'Dial Number')" 
                            value='<%nvg_attr_get("wan$0_wwan_dialstr", "wan_wwan_rule", "$0", "dialstr");%>' />
                </div>
                <!--
                <div class="row show-grid">
                    <div class="span2"><% lang("Connection Mode"); %></div>
                    <div class="span4">
                        <select name="wan$0_wwan_mode" id="wan$0_wwan_mode"> 
                          <option value="auto" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "auto", "selected"); %>><%lang("Auto");%></option>
                          <option value="hsdpa" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "hsdpa", "selected"); %>>HSDPA / 3.5G</option>
                          <option value="umts" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "umts", "selected"); %>>UMTS / 3G</option>
                          <option value="edge" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "edge", "selected"); %>>EDGE / 2.5G</option>
                          <option value="gprs" <% nvg_attr_match("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode", "gprs", "selected"); %>>GPRS / 2G</option>
                        </select>
                    </div>
                </div>
                -->
                <input type="hidden" name='wan$0_wwan_mode' id='wan$0_wwan_mode' value='<%nvg_attr_get("wan$0_wwan_mode", "wan_wwan_rule", "$0", "mode");%>'/>

                <input type='hidden' name='wan$0_wwan_demand' value='0'/>
                <input type='hidden' name='wan$0_wwan_idletime' value='300'/>
                <!--
                <div class="row show-grid">
                    <div class="span2"><%lang("PPP Echo Interval");%></div>
                    <div class="span4">
                        <input type="text" class="num" size="4" maxLength="4" name="wan$0_wwan_lcp_echo_interval"
                             id="wan$0_wwan_lcp_echo_interval"
                            value='<%nvg_attr_get("wan$0_wwan_lcp_echo_interval", "wan_wwan_rule", "$0", "redialperiod");%>'
                            onblur="valid_number(this, 'PPP Echo Interval')" /> <%lang("Seconds");%> (20 ~ 180)
                    </div>
                </div>
                -->
                <input type="hidden" name='wan$0_wwan_lcp_echo_interval' id='wan$0_wwan_lcp_echo_interval' value='<%nvg_attr_get("wan$0_wwan_lcp_echo_interval", "wan_wwan_rule", "$0", "redialperiod");%>'/>
                <!--
                <div class="row show-grid">
                    <div class="span2"><%lang("PPP Retry Threshold");%></div>
                    <div class="span4">
                        <input type="text" class="num" size="4" maxLength="4" name="wan$0_wwan_lcp_echo_failure" 
                            id="wan$0_wwan_lcp_echo_failure"
                            value='<%nvg_attr_get("wan$0_wwan_lcp_echo_failure", "wan_wwan_rule", "$0", "echo_failure");%>'
                            onblur="valid_number(this, 'PPP Retry Threshold')" /> <%lang("Time(s)");%> (3 ~ 50)
                    </div>
                </div>
                -->
                <input type="hidden" name='wan$0_wwan_lcp_echo_failure' id='wan$0_wwan_lcp_echo_failure' value='<%nvg_attr_get("wan$0_wwan_lcp_echo_failure", "wan_wwan_rule", "$0", "echo_failure");%>'/>
                <!--
                <div class="row show-grid">
                    <div class="span2"><%lang("Mobile WAN MTU");%></div>
                    <div class="span4">
                        <input type="text" name="wan$0_wwan_mtu" id="wan$0_wwan_mtu" size="4" maxLength="4"
                            value='<%nvg_attr_get("wan$0_wwan_mtu", "wan_wwan_rule", "$0", "mtu");%>'
                            onblur="valid_number(this, 'Mobile WAN MTU')" /> <%lang("Bytes");%> (68-1492)
                    </div>
                </div>
                -->
                <input type="hidden" name='wan$0_wwan_mtu' id='wan$0_wwan_mtu' value='<%nvg_attr_get("wan$0_wwan_mtu", "wan_wwan_rule", "$0", "mtu");%>'/>

                <!--
                <div class="row show-grid">
                    <div class="span2"><%lang("TurboLink");%></div>
                    <div class="span4">
                        <label class="radio inline">
                        <input type="radio" name="wan$0_wwan_turbolink" id="wan$0_wwan_turbolink"
                          value="1" <%nvg_attr_match("wan$0_wwan_turbolink","wan_wwan_rule","$0","turbolink","1","checked");%>/>
                          <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                        <input type="radio" name="wan$0_wwan_turbolink" id="wan$0_wwan_turbolink"
                          value="0" <%nvg_attr_match("wan$0_wwan_turbolink","wan_wwan_rule","$0","turbolink","0","checked");%>/>
                          <%lang("Disable");%>
                        </label>
                    </div>
                </div>
                -->
                <input type="hidden" name='wan$0_wwan_turbolink' id='wan$0_wwan_turbolink' value='<%nvg_attr_get("wan$0_wwan_turbolink", "wan_wwan_rule", "$0", "turbolink");%>'/>
                <input type="hidden" name="wan$0_mtu" value="1500" />
                <input type="hidden" name="wan$0_stp" value="0"/>
        <br>
