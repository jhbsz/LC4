<div>
  <div class="setting">
    <div class="label"><% lang("MTU"); %></div>
    <input size="6" maxLength="4" name="wan$0_wimax_mtu" 
            value='<%nvg_attr_get("wan$0_wimax_mtu", "wan_wimax_rule", "$0", "mtu");%>'
            onblur='valid_number(this, "MTU")' /> <%lang("Bytes");%>
  </div>

 <input type="hidden" name="wan$0_mtu" value=<%nvg_attr_get("wan_main_mtu","wan_main_rule", "$0", "mtu");%> />

  <div class="setting">
    <div class="label"><%lang("TurboLink");%></div>
    <div class='radiobutton'><input type="radio" name="wan$0_wimax_turbolink" id="wan$0_wimax_turbolink"
      value="1" <%nvg_attr_match("wan$0_wimax_turbolink","wan_wimax_rule","$0","turbolink","1","checked");%>/>
      <%lang("Enable");%></div>
    <div class='radiobutton'><input type="radio" name="wan$0_wimax_turbolink" id="wan$0_wimax_turbolink"
      value="0" <%nvg_attr_match("wan$0_wimax_turbolink","wan_wimax_rule","$0","turbolink","0","checked");%>/>
      <%lang("Disable");%></div>
  </div>
</div>
