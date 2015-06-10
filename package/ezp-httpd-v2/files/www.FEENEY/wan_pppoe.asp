<div>
  <div class="setting">
    <div class="label"><% lang("Authentication"); %></div>
    <select name="wan$0_ppp_auth" id='wan$0_ppp_auth' >
      <option value="chap" <%nvg_attr_match("wan$0_ppp_auth","wan_pppoe_rule","$0","auth","chap","selected"); %>>CHAP (<%lang("Auto");%>)</option>
      <option value="pap" <%nvg_attr_match("wan$0_ppp_auth","wan_pppoe_rule","$0","auth","pap","selected"); %>>PAP</option>
      <option value="none" <%nvg_attr_match("wan$0_ppp_auth","wan_pppoe_rule","$0","auth","none","selected"); %>><%lang("None");%></option>
    </select>
  </div>

  <div class="setting">
    <div class="label"><%lang("User Name");%></div>
    <input name="wan$0_ppp_username" size="25" maxLength="36" onBlur="valid_pppoe(this,'User Name')" 
        value='<%nvg_attr_get("wan$0_ppp_username", "wan_pppoe_rule", "$0", "username");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("Password");%></div>
    <input name="wan$0_ppp_passwd" size="25" maxLength="36" onBlur="valid_pppoe(this,'Password',SPACE_NO)" 
        type="password" value="slide" />
  </div>


  <div class="setting">
    <div class="label"> <% lang("PPP Connection Type"); %></div>
    <input type="radio" name="wan$0_ppp_demand" id="wan$0_ppp_demand" value="0" 
      onclick="sel_ppp_mode('0', '$0')"
      <%nvg_attr_match("wan$0_ppp_demand","wan_pppoe_rule","$0","demand","0","checked");%> />
      <%lang("Always Connected");%>
    <input type="radio" name="wan$0_ppp_demand" id="wan$0_ppp_demand" value="1" 
      onclick="sel_ppp_mode('1', '$0')" 
      <%nvg_attr_match("wan$0_ppp_demand","wan_pppoe_rule","$0","demand","1","checked");%> />
      <%lang("On Demand");%>
  </div>

  <div class="setting">
    <div class="label"> <%lang("Max Idle Time");%> </div> 
    <input class="num" size="4" maxLength="4" name="wan$0_ppp_idletime" id="wan$0_ppp_idletime"
        value='<%nvg_attr_get("wan$0_ppp_idletime", "wan_pppoe_rule", "$0", "idletime");%>' 
        onblur="valid_number(this, 'Max Idle Time')" /> <%lang("Seconds");%> (60~3600)
  </div>

  <div class="setting">
    <div class="label"><%lang("PPP Echo Interval");%></div>
    <input class="num" size="4" maxLength="4" name="wan$0_ppp_lcp_echo_interval" 
        id="wan$0_ppp_lcp_echo_interval"
        value='<%nvg_attr_get("wan$0_ppp_lcp_echo_interval", "wan_pppoe_rule", "$0", "redialperiod");%>'
        onblur="valid_number(this, 'PPP Echo Interval')" /> <%lang("Seconds");%> (20 ~ 180)
    <br/>
    <div class="label"><%lang("PPP Retry Threshold");%></div>
    <input class="num" size="4" maxLength="4" name="wan$0_ppp_lcp_echo_failure" 
        id="wan$0_ppp_lcp_echo_failure"
        value='<%nvg_attr_get("wan$0_ppp_lcp_echo_failure", "wan_pppoe_rule", "$0", "echo_failure");%>'
        onblur="valid_number(this, 'PPP Retry Threshold')" /> <%lang("Time(s)");%> (3 ~ 50)
  </div>

  <div class="setting">
    <div class="label"><%lang("PPP MTU");%></div>
    <input name="wan$0_ppp_mtu" size="4" maxLength="4"
        value='<%nvg_attr_get("wan$0_ppp_mtu", "wan_pppoe_rule", "$0", "mtu");%>'
        onblur="valid_number(this, 'PPP MTU')" /> <%lang("Bytes");%> (592-1492)
  </div>

  <div class="setting">
    <div class="label"><% lang("MTU"); %></div>
    <input size="6" maxLength="4" name="wan$0_mtu" 
            value='<%nvg_attr_get("wan$0_mtu", "wan_main_rule", "$0", "mtu");%>'
            onblur='valid_number(this, "MTU")' /> <%lang("Bytes");%> (600~1500)
  </div>

</div>
