<div class="row">
    <div class="span12">
        <legend><%lang("WDS");%> <%increase("$1","1");%></legend>
        <div class="row show-grid">
            <div class="span4"><% lang("WDS MAC Address"); %></div>
            <div class="span8">
              <input type="text" name="wl$0_wds$1_hwaddr" id="wl$0_wds$1_hwaddr" value='<% nvg_attr_get("wl$0_wds$1_hwaddr","wl$0_wds_basic_rule","$1","hwaddr");%>'>
            </div>
        </div>

        <div class="row show-grid">
            <div class="span4"><% lang("Security Mode"); %></div>
            <div class="span8">
              <select name="wl$0_wds$1_secmode" id="wl$0_wds$1_secmode" onchange="SelWDS(this.form)">
              <option value="disabled" <%nvg_attr_match("wl$0_wds$1_secmode", "wl$0_wds_basic_rule", "$1", "secmode", "disabled", "selected");%>>
                <% lang("Disable"); %></option>
              <option value="wep" <%nvg_attr_match("wl$0_wds$1_secmode", "wl$0_wds_basic_rule", "$1", "secmode", "wep", "selected");%>>
                <% lang("WEP"); %></option>
              <option value="psk" <%nvg_attr_match("wl$0_wds$1_secmode", "wl$0_wds_basic_rule", "$1", "secmode", "psk", "selected");%>>
                <% lang("WPA PSK (Pre-Shared Key)"); %></option>
              </select>
            </div>
        </div>
      <% wlv_wds_sec_show_setting($0, $1); %>
    </div>
</div><!-- row -->
<br/>
