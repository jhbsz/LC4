            <fieldset>
            <legend><%lang("WAN");%> <%increase("$0", "1");%></legend>

            <div class="setting">
              <div class="label">
                <%lang("MAC Address");%>
              </div><% show_hwaddr("wan$0"); %>
            </div>

            <div class="setting">
              <div class="label">
                <%lang("Connection Type");%>
              </div><% nv_get("wan$0_proto"); %>
            </div>

            <div class="setting">
              <div class="label">
                <%lang("IP Address");%>
              </div><% nv_get("wan$0_ipaddr"); %>
            </div>

            <div class="setting">
              <div class="label">
                <%lang("Subnet Mask");%>
              </div><% nv_get("wan$0_mask"); %>
            </div>

            <div class="setting">
              <div class="label">
                <%lang("Gateway");%>
              </div><% nv_get("wan$0_gateway"); %>
            </div>

            </fieldset><br />
