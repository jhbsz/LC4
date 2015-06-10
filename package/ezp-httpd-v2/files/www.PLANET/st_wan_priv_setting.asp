            <fieldset>
            <legend><%lang("WAN");%> <%increase("$0", "1");%></legend>

            <div class="setting">
              <div class="label">
                <%lang("MAC Address");%>
              </div><% show_hwaddr("wan$0"); %> (<%nv_attr_get("wan_hwaddr_rule_default", $0, "hwaddr");%>)
            </div>

            </fieldset><br />
