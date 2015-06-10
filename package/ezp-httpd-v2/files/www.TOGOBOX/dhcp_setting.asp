<script language="JavaScript">
dhcp_objects[$0] =
[
  { name: 'dhcp$0_start' },
  { name: 'dhcp$0_num' },
  { name: 'dhcp$0_lease' },
  { name: 'dhcp$0_domain' },
  { name: 'dhcp$0_dnstype' },
];

dhcpdnsaddr_objects[$0] =
[
  { name: 'dhcp$0_dnsaddr1'},
  { name: 'dhcp$0_dnsaddr2'},
];
</script>

<div class="row">
    <div class="span12">
        <legend><%lang("DHCP Server");%> - <%lang("LAN");%> <%increase("$0", "1");%></legend>

        <div class="row show-grid">
            <div class="span4"><%lang("DHCP Service");%></div>
            <div class="span8">
                <label class="radio inline">
                    <input type="radio" value="1" name="dhcp$0_enable" id="dhcp$0_enable" onclick="init()"
                          <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","1","checked");%>><%lang("Enable");%>
                </label>
                <label class="radio inline">
                    <input type="radio" value="0" name="dhcp$0_enable" id="dhcp$0_enable" onclick="init()"
                          <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","0","checked");%>><%lang("Disable");%>
                </label>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("DHCP Start IP Address");%></div>
            <div class="span8">
              <%show_ipaddr_prefix24("lan$0_ipaddr");%>
              <input type="text" size="3" maxLength="3" name="dhcp$0_start" id="dhcp$0_start" 
                    value='<%nvg_attr_get("dhcp$0_start", "lan_dhcps_rule", "$0", "start");%>'
                    onblur='valid_range(this, 1, 254, "DHCP Start IP Address")' />
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Max DHCP Clients");%></div>
            <div class="span8">
                  <input type="text" size="3" maxLength="3" name="dhcp$0_num" id="dhcp$0_num" 
                        value='<%nvg_attr_get("dhcp$0_num", "lan_dhcps_rule", "$0", "num");%>'
                        onblur='valid_number(this, "Max DHCP Clients")' />
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Lease");%></div>
            <div class="span8">
                  <select name="dhcp0_lease" id="dhcp0_lease" 
                        value='<%nvg_attr_get("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease");%>' />
                    <option value="3600" <%nvg_attr_match("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease", "3600", "selected"); %>>
                        1 <%lang("hour");%></option>
                    <option value="10800" <%nvg_attr_match("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease", "10800", "selected"); %>>
                        3 <%lang("hours");%></option>
                    <option value="21600" <%nvg_attr_match("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease", "21600", "selected"); %>>
                        6 <%lang("hours");%></option>
                    <option value="86400" <%nvg_attr_match("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease", "86400", "selected"); %>>
                        1 <%lang("day");%></option>
                    <option value="259200" <%nvg_attr_match("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease", "259200", "selected"); %>>
                        3 <%lang("days");%></option>
                    <option value="604800" <%nvg_attr_match("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease", "604800", "selected"); %>>
                        7 <%lang("days");%></option>
                  </select>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("Domain");%></div>
            <div class="span8">
              <input type="text" size="25" maxLength="20" name="dhcp$0_domain" id="dhcp$0_domain" 
                    value='<%nvg_attr_get("dhcp$0_domain", "lan_dhcps_rule", "$0", "domain");%>' onblur="valid_name(this, 'Domain')" />
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("DHCP DNS Server Type");%></div>
            <div class="span8">
                <select name="dhcp$0_dnstype" id="dhcp$0_dnstype" onChange="SelDNSType($0)" 
                        value='<%nvg_attr_get("dhcp$0_dnstype", "lan_dhcps_rule", "$0", "dnstype");%>'>
                <option value="dnsrelay" 
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule", "$0", "dnstype", "dnsrelay", "selected"); %>>
                        <%lang("DNS Relay");%></option>
                <option value="ispdns"
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule", "$0","dnstype", "ispdns", "selected"); %>>
                        <%lang("ISP DNS Server");%></option>
                <option value="opendns"
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule", "$0","dnstype", "opendns","selected"); %>>
                        <%lang("OpenDNS Server");%></option>
                <option value="googledns"
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule", "$0","dnstype", "googledns","selected"); %>>
                        <%lang("Google DNS Server");%></option>
                <option value="custom"
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype", "custom","selected"); %>>
                        <%lang("Custom");%></option>
                </select>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><%lang("DHCP DNS Server IP Address");%></div>
            <div class="span8">
                  <input type="text" size="20" maxLength="20" id="dhcp$0_dnsaddr1" name="dhcp$0_dnsaddr1" 
                        value='<%nvg_attr_get("dhcp$0_dnsaddr1", "lan_dhcps_rule", "$0", "dnsaddr1");%>' 
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","dnsrelay","disabled"); %>
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","ispdns","disabled");%>
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","opendns","disabled");%>
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","googledns","disabled");%>>
                  <br>
                  <input type="text" size="20" maxLength="20" ID="dhcp$0_dnsaddr2" name="dhcp$0_dnsaddr2" 
                        value='<%nvg_attr_get("dhcp$0_dnsaddr2", "lan_dhcps_rule", "$0", "dnsaddr2");%>'
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","dnsrelay","disabled");%>
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","ispdns","disabled");%>
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","opendns","disabled");%>
                        <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","googledns","disabled");%>>
            </div>
        </div>
        <input type="hidden" name="dhcp$0_wins" id="dhcp$0_wins" value='<%nvg_attr_get("dhcp$0_wins", "lan_dhcps_rule", "$0", "wins");%>' />

    </div>
</div><!-- row -->
<br>