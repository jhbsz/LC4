    <div class="row show-grid">
        <div class="span4"><% lang("Authentication"); %></div>
        <div class="span8">
            <select name="wan$0_ppp_auth" id='wan$0_ppp_auth' >
              <option value="chap" <%nvg_attr_match("wan$0_ppp_auth","wan_pppoe_rule","$0","auth","chap","selected"); %>>CHAP (<%lang("Auto");%>)</option>
              <option value="pap" <%nvg_attr_match("wan$0_ppp_auth","wan_pppoe_rule","$0","auth","pap","selected"); %>>PAP</option>
              <option value="none" <%nvg_attr_match("wan$0_ppp_auth","wan_pppoe_rule","$0","auth","none","selected"); %>><%lang("None");%></option>
            </select>
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("User Name");%></div>
        <div class="span8">
            <input type="text" name="wan$0_ppp_username" id="wan$0_ppp_username" size="25" maxLength="36" onBlur="valid_pppoe(this,'User Name')" 
                value='<%nvg_attr_get("wan$0_ppp_username", "wan_pppoe_rule", "$0", "username");%>' />
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("Password");%></div>
        <div class="span8">
            <input name="wan$0_ppp_passwd" id="wan$0_ppp_passwd" size="25" maxLength="36" onBlur="valid_pppoe(this,'Password',SPACE_NO)" 
                type="password" value="slide" />
        </div>
    </div>

      <input type='hidden' name='wan$0_ppp_demand' id='wan$0_ppp_demand' value='0'/>
      <input type='hidden' name='wan$0_ppp_idletime' id='wan$0_ppp_idletime' value='300'/>

    <div class="row show-grid">
        <div class="span4"><%lang("PPP Echo Interval");%></div>
        <div class="span8">
            <input type="text" size="4" maxLength="4" name="wan$0_ppp_lcp_echo_interval" 
                id="wan$0_ppp_lcp_echo_interval"
                value='<%nvg_attr_get("wan$0_ppp_lcp_echo_interval", "wan_pppoe_rule", "$0", "redialperiod");%>'
                onblur="valid_number(this, 'PPP Echo Interval')" /> <%lang("Seconds");%> (20 ~ 180)
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("PPP Retry Threshold");%></div>
        <div class="span8">
            <input type="text" size="4" maxLength="4" name="wan$0_ppp_lcp_echo_failure" 
                id="wan$0_ppp_lcp_echo_failure"
                value='<%nvg_attr_get("wan$0_ppp_lcp_echo_failure", "wan_pppoe_rule", "$0", "echo_failure");%>'
                onblur="valid_number(this, 'PPP Retry Threshold')" /> <%lang("Time(s)");%> (3 ~ 50)
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><%lang("PPP MTU");%></div>
        <div class="span8">
            <input type="text" name="wan$0_ppp_mtu" id="wan$0_ppp_mtu" size="4" maxLength="4"
                value='<%nvg_attr_get("wan$0_ppp_mtu", "wan_pppoe_rule", "$0", "mtu");%>'
                onblur="valid_number(this, 'PPP MTU')" /> <%lang("Bytes");%> (592-1492)
        </div>
    </div>

    <div class="row show-grid">
        <div class="span4"><% lang("MTU"); %></div>
        <div class="span8">
            <input type="text" size="6" maxLength="4" name="wan$0_mtu" id="wan$0_mtu" 
                    value='<%nvg_attr_get("wan$0_mtu", "wan_main_rule", "$0", "mtu");%>'
                    onblur='valid_number(this, "MTU")' /> <%lang("Bytes");%> (600~1500)
        </div>
    </div>

