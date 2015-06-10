<!--
<div class="row show-grid">
    <div class="span4"></div>
    <div class="span8">

    </div>
</div>
-->

<div class="row show-grid">
    <div class="span4"><%lang("VPN Client Type");%></div>
    <div class="span8">
      <select name="wan$0_vpnc_type" id="wan$0_vpnc_type" onchange="init(this.form)">
        <option value="pptp" <%nvg_attr_match("wan$0_vpnc_type","wan_pptp_l2tp_rule","$0","vpn_type","pptp","selected");%>>
            PPTP</option>
        <option value="l2tp" <%nvg_attr_match("wan$0_vpnc_type","wan_pptp_l2tp_rule","$0","vpn_type","l2tp", "selected");%>>
            L2TP</option>
      </select>
    </div>
</div>

<div class="row show-grid">
    <div class="span4"><%lang("User Name");%></div>
    <div class="span8">
        <input type="text" maxLength="15" size="16" name="wan$0_vpnc_username" id="wan$0_vpnc_username"
        value='<%nvg_attr_get("wan$0_vpnc_username", "wan_pptp_l2tp_rule", "$0", "username");%>'/>
    </div>
</div>

<div class="row show-grid">
    <div class="span4"><%lang("Password");%></div>
    <div class="span8">
        <input type="text" maxLength="15" size="16" name="wan$0_vpnc_pw" id="wan$0_vpnc_pw"
        value='<%nvg_attr_get("wan$0_vpnc_pw", "wan_pptp_l2tp_rule", "$0", "passwd");%>'/>
    </div>
</div>

<div class="row show-grid">
    <div class="span4"><%lang("VPN Host");%></div>
    <div class="span8">
        <input type="text" size="25" name="wan$0_vpnc_host" id="wan$0_vpnc_host"
            value='<%nvg_attr_get("wan$0_vpnc_host", "wan_pptp_l2tp_rule", "$0", "host");%>'/>
    </div>
</div>

<div class="row show-grid">
    <div class="span4"><%lang("MPPE128");%></div>
    <div class="span8">
        <label class="radio inline">
           <input type="radio" value="mppe" name="wan$0_vpnc_encmode" id="wan$0_vpnc_encmode" 
            <%nvg_attr_match("wan$0_vpnc_encmode","wan_pptp_l2tp_rule", "$0","encmode", "mppe","checked");%>>
            <%lang("Enable");%>
        </label>
        <label class="radio inline">
           <input type="radio" value="" name="wan$0_vpnc_encmode" id="wan$0_vpnc_encmode" 
            <%nvg_attr_match("wan$0_vpnc_encmode","wan_pptp_l2tp_rule", "$0","encmode","", "checked");%>>
            <%lang("Disable");%></input>
        </label>
    </div>
</div>

<!--
<div>


    </div>
  <div class="setting">
  <div class="label"></div>
  </div>

</div>
-->