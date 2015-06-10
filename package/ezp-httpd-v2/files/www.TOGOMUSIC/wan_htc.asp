    <div class="row show-grid">
        <div class="span4"><%lang("Host Name");%></div>
        <div class="span8">
            <input type="text" name="wan$0_htc_hostname" id="wan$0_htc_hostname" size="25" maxLength="30" onBlur="valid_name(this,'Host Name')" 
                value='<%nvg_attr_get("wan$0_htc_hostname", "wan_htc_rule", "$0", "hostname");%>'
                onblur='valid_name(this, "Host Name")' />
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("MTU"); %></div>
        <div class="span8">
            <input type="text" size="6" maxLength="4" name="wan$0_htc_mtu" id="wan$0_htc_mtu" 
                    value='<%nvg_attr_get("wan$0_htc_mtu", "wan_htc_rule", "$0", "mtu");%>'
                    onblur='valid_number(this, "MTU")' /> <%lang("Bytes");%>
        </div>
    </div>

  <input type="hidden" name="wan$0_htc_domain" value='<%nvg_attr_get("wan$0_htc_domain", "wan_htc_rule", "$0", "domain");%>' />
  <input type="hidden" name="wan$0_htc_lease" value=<%nvg_attr_get("wan$0_htc_lease","wan_htc_rule", "$0", "lease");%> />
 <input type="hidden" name="wan$0_mtu" value=<%nvg_attr_get("wan_main_mtu","wan_main_rule", "$0", "mtu");%> />

    <div class="row show-grid">
        <div class="span4"><%lang("TurboLink");%></div>
        <div class="span8">
            <label class="radio inline">
                <input type="radio" name="wan$0_htc_turbolink" id="wan$0_htc_turbolink"
                  value="1" <%nvg_attr_match("wan$0_htc_turbolink","wan_htc_rule","$0","turbolink","1","checked");%>/>
                  <%lang("Enable");%>
            </label>
            <label class="radio inline">
                <input type="radio" name="wan$0_htc_turbolink" id="wan$0_htc_turbolink"
                  value="0" <%nvg_attr_match("wan$0_htc_turbolink","wan_htc_rule","$0","turbolink","0","checked");%>/>
                  <%lang("Disable");%>
            </label>
        </div>
    </div>
