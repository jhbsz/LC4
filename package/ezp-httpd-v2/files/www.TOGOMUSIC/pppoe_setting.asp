            <input type="hidden" name="wan$0_enable" id="wan$0_enable" value="1"/>
            <input type="hidden" name="wan$0_proto" id="wan$0_proto" value="pppoe"/>
            <input type="hidden" name="wan$0_ppp_auth" id="wan$0_ppp_auth" value="chap"/>
            <input type="hidden" name="wan$0_ppp_demand" id='wan$0_ppp_demand' value="0"/>
            <input type="hidden" name="wan$0_ppp_idletime" id='wan$0_ppp_idletime' value="300"/>
            <input type="hidden" name="wan$0_ppp_lcp_echo_interval" id="wan$0_ppp_lcp_echo_interval" value="20"/>
            <input type="hidden" name="wan$0_ppp_lcp_echo_failure" id="wan$0_ppp_lcp_echo_failure" value="20"/>
            <input type="hidden" name="wan$0_ppp_mtu" id="wan$0_ppp_mtu" value="1492"/>
            <input type="hidden" name="wan$0_mtu" id="wan$0_mtu" value="1500"/>
            <input type="hidden" name="wan$0_vpnc_enable" id="wan$0_vpnc_enable" value="0"/>
            <input type="hidden" name= "wanupnp_enable" id= "wanupnp_enable" value="0"/>

                <div class="row-fluid">
                    <label><%lang("User Name");%></label>
                        <input type="text" name="wan$0_ppp_username" id="wan$0_ppp_username" size="25" maxLength="36" onBlur="valid_pppoe(this,'User Name')" 
                        value='<%nvg_attr_get("wan$0_ppp_username", "wan_pppoe_rule", "$0", "username");%>' ></input>
                </div>

                <div class="row-fluid">
                    <label><%lang("Password");%></label>
                        <input name="wan$0_ppp_passwd" id="wan$0_ppp_passwd" size="25" maxLength="36" onBlur="valid_pppoe(this,'Password',SPACE_NO)" 
                            type="password" value="slide" />
                </div>

