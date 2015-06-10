<div class="row">
    <div class="span12">
        <legend><%lang("WLAN");%> <%increase("$0","1");%></legend>
        <div class="row show-grid">
            <div class="span4"><% lang("WDS Mode"); %></div>
            <div class="span8">
              <select name="wl$0_wds_mode" id="wl$0_wds_mode" onchange="SelWDS(this.form)">
                  <option value="disabled" <%nvg_attr_match("wl$0_wds_mode", "wl_wds_rule", "$0", "mode", "disabled", "selected");%>>
                    <% lang("Disabled"); %></option>
                  <option value="repeater" <%nvg_attr_match("wl$0_wds_mode", "wl_wds_rule", "$0", "mode", "repeater", "selected");%>>
                    <% lang("Repeater (AP Enabled)"); %></option>
                  <option value="bridge" <%nvg_attr_match("wl$0_wds_mode", "wl_wds_rule", "$0", "mode", "bridge", "selected");%>>
                    <% lang("Bridge (AP Disabled)"); %></option>
              </select>
            </div>
        </div>
    </div>
</div><!-- row -->

<% wl_wds_show_setting("$0"); %>
