<fieldset>
  <legend><%lang("WLAN");%> </legend>
  <div class="setting">

  <div class="label"><% lang("WDS Mode"); %></div>
  <select name="wl$0_wds_mode" onchange="SelWDS(this.form)">
  <option value="disabled" <%nvg_attr_match("wl$0_wds_mode", "wl_wds_rule", "$0", "mode", "disabled", "selected");%>><% lang("Disabled"); %></option>
  <option value="repeater" <%nvg_attr_match("wl$0_wds_mode", "wl_wds_rule", "$0", "mode", "repeater", "selected");%>>
<% lang("Repeater (AP Enabled)"); %></option>
  <option value="bridge" <%nvg_attr_match("wl$0_wds_mode", "wl_wds_rule", "$0", "mode", "bridge", "selected");%>><% lang("Bridge (AP Disabled)"); %></option>
  </select>
  <br/>

  </div>
</fieldset>
<br/>
  <% wl_wds_show_setting("$0"); %>
