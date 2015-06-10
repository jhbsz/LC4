            <fieldset>
            <legend><%lang("LAN");%> </legend>
                     <div class="setting">
                        <div class="label"><%lang("MAC Address");%></div><% show_hwaddr("lan$0"); %>
                     </div>
                     <div class="setting">
                        <div class="label"><%lang("IP Address");%></div><% nv_get("lan$0_ipaddr"); %>
                     </div>
                     <div class="setting">
                        <div class="label"><%lang("Subnet Mask");%></div><% nv_get("lan$0_mask"); %>
                     </div>
                     <div class="setting">
                        <div class="label"><%lang("DHCP Service");%></div>
                        <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","1","Enabled");%>
                        <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","0","Disabled");%>
                     </div>
                     <div class="setting">
                        <div class="label"><%lang("DHCP Start IP Address");%>
                        </div> 
                        <%show_ipaddr_prefix24("lan$0_ipaddr");%><%nv_attr_get("lan_dhcps_rule", "$0", "start");%>
                     </div>
                     <div class="setting">
                        <div class="label"><%lang("DHCP End IP Address");%>
                        </div>
                        <%show_ipaddr_prefix24_calc("lan$0_ipaddr", "lan_dhcps_rule", "$0");%>
                     </div>
                     <div class="setting">
                        <div class="label"><%lang("Max DHCP Clients");%>
                        </div>
                        <%nv_attr_get("lan_dhcps_rule", "$0", "num");%>
                     </div>
            </fieldset><br />
