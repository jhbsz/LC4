        <div class="row">
            <div class="span12">
                <legend><%lang(LAN);%> <%increase("$0", "1");%></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("MAC Address");%></div>
                    <div class="span8"><% show_hwaddr("lan$0"); %></div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><%lang("IP Address");%></div>
                    <div class="span8"><% nv_get("lan$0_ipaddr"); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Subnet Mask");%></div>
                    <div class="span8"><% nv_get("lan$0_mask"); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("DHCP Service");%></div>
                    <div class="span8">
                        <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","1","Enabled");%>
                        <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","0","Disabled");%>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("DHCP Start IP Address");%></div>
                    <div class="span8"><%show_ipaddr_prefix24("lan$0_ipaddr");%><%nv_attr_get("lan_dhcps_rule", "$0", "start");%></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("DHCP End IP Address");%></div>
                    <div class="span8"><%show_ipaddr_prefix24_calc("lan$0_ipaddr", "lan_dhcps_rule", "$0");%></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Max DHCP Clients");%></div>
                    <div class="span8"><%nv_attr_get("lan_dhcps_rule", "$0", "num");%></div>
                </div>

            </div>
        </div><!-- row -->
        <br>