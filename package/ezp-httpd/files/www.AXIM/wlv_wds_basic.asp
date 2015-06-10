<fieldset>
  <legend><%lang("WDS");%> <%increase("$1","1");%></legend>
  <div class="setting">

  <div class="label"><% lang("WDS MAC Address"); %></div>
  <input type="text" name="wl$0_wds$1_hwaddr" value='<% nvg_attr_get("wl$0_wds$1_hwaddr","wl$0_wds_basic_rule","$1","hwaddr");%>'>
  <input type="button" id="wl$1_survey" name="survey" value='<%lang("Survey");%>'><br/>

  <div class="label"><% lang("Security Mode"); %></div>
  <select name="wl$0_wds$1_secmode" onchange="SelWDS(this.form)">
  <option value="disabled" <%nvg_attr_match("wl$0_wds$1_secmode", "wl$0_wds_basic_rule", "$1", "secmode", "disabled", "selected");%>><% lang("Disable"); %></option>
  <option value="wep" <%nvg_attr_match("wl$0_wds$1_secmode", "wl$0_wds_basic_rule", "$1", "secmode", "wep", "selected");%>><% lang("WEP"); %></option>
  <option value="psk" <%nvg_attr_match("wl$0_wds$1_secmode", "wl$0_wds_basic_rule", "$1", "secmode", "psk", "selected");%>><% lang("WPA PSK (Pre-Shared Key)"); %></option>
  </select>
  <br/>

  <% wlv_wds_sec_show_setting($0, $1); %>
  </div>
</fieldset>
<br/>
