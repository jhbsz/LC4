
  <div class="setting">
    <div class="label"><% lang("IP Type"); %></div>
    <input type="radio" value="dhcp" name="wan$0_wisp_ip_type" onchange="SelWAN(this.form)"
          <%nvg_attr_match("wan$0_wisp_ip_type","wl0_apcli_rule","$0","ip_type","dhcp","checked");%>>
    <%lang("DHCP");%>
    </input>

    <input type="radio" value="static" name="wan$0_wisp_ip_type" onchange="SelWAN(this.form)"
          <%nvg_attr_match("wan$0_wisp_ip_type","wl0_apcli_rule","$0","ip_type","static","checked");%>>
    <%lang("Static IP");%>
    </input>
  </div>
 <% wan_show_wisp_type_setting($0); %>
