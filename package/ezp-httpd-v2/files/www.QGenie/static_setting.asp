        <input type="hidden" name="wan$0_enable" id="wan$0_enable" value="1"/>
        <input type="hidden" name="wan$0_proto" id="wan$0_proto" value="static"/>
        <input type="hidden" name="wan$0_mtu" id="wan$0_mtu" value="1500"/>
        <input type="hidden" name="wan$0_vpnc_enable" id="wan$0_vpnc_enable" value="0"/>
        <input type="hidden" name= "wanupnp_enable" id= "wanupnp_enable" value="0"/>

                <div class="row-fluid">
                    <label><%lang("External IP Address");%></label>
                        <input type="text" maxLength="15" size="16" name="wan$0_static_ipaddr" id="wan$0_static_ipaddr"
                        value='<%nvg_attr_get("wan$0_static_ipaddr", "wan_static_rule", "$0", "ipaddr");%>' 
                        onblur='valid_ipaddr(this, "External IP Address")' />
                </div>

                <div class="row-fluid">
                    <label><%lang("Netmask");%></label>
                        <select name="wan$0_static_mask" id="wan$0_static_mask">
                        <option value="16" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","16","selected");%>>
                            255.255.0.0</option>
                        <option value="17" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","17","selected");%>>
                            255.255.128.0</option>
                        <option value="18" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","18","selected");%>>
                            255.255.192.0</option>
                        <option value="19" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","19","selected");%>>
                            255.255.224.0</option>
                        <option value="20" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","20","selected");%>>
                            255.255.240.0</option>
                        <option value="21" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","21","selected");%>>
                            255.255.248.0</option>
                        <option value="22" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","22","selected");%>>
                            255.255.252.0</option>
                        <option value="23" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","23","selected");%>>
                            255.255.254.0</option>
                        <option value="24" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","24","selected");%>>
                            255.255.255.0</option>
                        <option value="25" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","25","selected");%>>
                            255.255.255.128</option>
                        <option value="26" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","26","selected");%>>
                            255.255.255.192</option>
                        <option value="27" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","27","selected");%>>
                            255.255.255.224</option>
                        <option value="28" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","28","selected");%>>
                            255.255.255.240</option>
                        <option value="29" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","29","selected");%>>
                            255.255.255.248</option>
                        <option value="30" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","30","selected");%>>
                            255.255.255.252</option>
                        <option value="31" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","31","selected");%>>
                            255.255.255.254</option>
                        <option value="32" <%nvg_attr_match("wan$0_static_mask","wan_static_rule","$0","mask","32","selected");%>>
                            255.255.255.255</option>
                        </select>
                </div>

                <div class="row-fluid">
                    <label><%lang("Gateway");%></label>
                        <input type="text" maxLength="15" size="16" name="wan$0_static_gateway" id="wan$0_static_gateway"
                            value='<%nvg_attr_get("wan$0_static_gateway", "wan_static_rule", "$0", "gateway");%>'
                            onblur='valid_ipaddr(this, "Gateway")'/>
                </div>

                <div class="row-fluid">
                    <label><%lang("Static DNS 1");%></label>
                    <input type="text" maxLength="15" size="16" name="wan$0_static_dns1" id="wan$0_static_dns1"
                        value='<%nvg_attr_get("wan$0_static_dns1", "wan_static_rule", "$0", "dns1");%>'
                        onblur='valid_reserved_word(this, "Static DNS 1")' />
                </div>

                <div class="row-fluid">
                    <label><%lang("Static DNS 2");%></label>
                    <input type="text" maxLength="15" size="16" name="wan$0_static_dns2" id="wan$0_static_dns2"
                        value='<%nvg_attr_get("wan$0_static_dns2", "wan_static_rule", "$0", "dns2");%>' 
                        onblur='valid_reserved_word(this, "Static DNS 2")' />
                </div>

