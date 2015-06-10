<div>
  <div class="setting">
  <div class="label"><%lang("VPN Client Type");%></div>
  <select name="wan$0_vpnc_type" id="wan$0_vpnc_type" onchange="init(this.form)">
        <option value="pptp" <%nvg_attr_match("wan$0_vpnc_type","wan_pptp_l2tp_rule","$0","vpn_type","pptp","selected");%>>
        PPTP</option>
        <option value="l2tp" <%nvg_attr_match("wan$0_vpnc_type","wan_pptp_l2tp_rule","$0","vpn_type","l2tp", "selected");%>>
        L2TP</option>
      </select>
  </div>

  <div>
    <div class="setting">
      <div class="label"><%lang("User Name");%></div>
        <input type="text" maxLength="15" size="16" name="wan$0_vpnc_username"
        value='<%nvg_attr_get("wan$0_vpnc_username", "wan_pptp_l2tp_rule", "$0", "username");%>'/>
    </div>

    <div class="setting">
      <div class="label"><%lang("Password");%></div>
        <input type="text" maxLength="15" size="16" name="wan$0_vpnc_pw"
        value='<%nvg_attr_get("wan$0_vpnc_pw", "wan_pptp_l2tp_rule", "$0", "passwd");%>'/>
    </div>


  <div class="setting">
    <div class="label"><%lang("VPN Host");%></div>
    <input type="text" size="25" name="wan$0_vpnc_host"
        value='<%nvg_attr_get("wan$0_vpnc_host", "wan_pptp_l2tp_rule", "$0", "host");%>'/>
  </div>

    </div>
  <div class="setting">
  <div class="label"><%lang("MPPE128");%></div>
   <input type="radio" value="mppe" name="wan$0_vpnc_encmode" <%nvg_attr_match("wan$0_vpnc_encmode","wan_pptp_l2tp_rule", "$0","encmode", "mppe","checked");%>>
    <%lang("Enable");%></input>
   <input type="radio" value="" name="wan$0_vpnc_encmode" <%nvg_attr_match("wan$0_vpnc_encmode","wan_pptp_l2tp_rule", "$0","encmode","", "checked");%>>
    <%lang("Disable");%></input>
  </div>

</div>
