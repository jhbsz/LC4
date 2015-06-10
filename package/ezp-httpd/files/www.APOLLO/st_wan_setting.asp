            <fieldset>
            <table border="0" cellpadding="0" cellspacing="0" class="box">
            <tr><td>
            <legend><%lang("WAN");%> <%increase_wan("$0", "1");%></legend>
            </td></tr>

            <tr><td>
            <div class="setting">
              <div class="label">
                <%lang("MAC Address");%>
              </div><% show_hwaddr("wan$0"); %>&nbsp;
            </div>
            </td></tr>
            
            <tr><td>
            <div class="setting">
              <div class="label">
                <%lang("Connection Type");%>
              </div><div id='wan$0_status_proto'>&nbsp;
            </div>
            </td></tr>

            <tr><td>
            <div class="setting">
              <div class="label">
                <%lang("IP Address");%>
              </div><div id='wan$0_status_ipaddr'>&nbsp;
            </div>
            </td></tr>

            <tr><td>
            <div class="setting">
              <div class="label">
                <%lang("Subnet Mask");%>
              </div><% nv_get("wan$0_mask"); %>&nbsp;
            </div>
            </td></tr>

            <tr><td>
            <div class="setting">
              <div class="label">
                <%lang("Gateway");%>
              </div><% nv_get("wan$0_gateway"); %>&nbsp;
            </div>
            </td></tr>
            </table>
            </fieldset><br />
