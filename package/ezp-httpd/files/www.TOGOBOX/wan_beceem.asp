<div>
 <input type="hidden" name="wan$0_mtu" value=<%nvg_attr_get("wan_main_mtu","wan_main_rule", "$0", "mtu");%> />

  <div class="setting">
    <div class="label"><% lang("ISP"); %></div>
    <select name="wan$0_beceem_isp" id='wan$0_beceem_isp' >
      <option value="g1" <%nvg_attr_match("wan$0_beceem_isp","wan_beceem_rule","$0","isp","g1","selected"); %>><%lang("Global Mobile");%></option>
      <option value="vmax" <%nvg_attr_match("wan$0_beceem_isp","wan_beceem_rule","$0","isp","vmax","selected"); %>><%lang("VMAX");%></option>
    </select>
  </div>

  <div class="setting">
    <div class="label"><%lang("User Name");%></div>
    <input name="wan$0_beceem_username" size="25" maxLength="36" onBlur="valid_pppoe(this,'User Name')" 
        value='<%nvg_attr_get("wan$0_beceem_username", "wan_beceem_rule", "$0", "username");%>' />
  </div>

  <div class="setting">
    <div class="label"><%lang("Password");%></div>
    <input name="wan$0_beceem_passwd" size="25" maxLength="36" onBlur="valid_pppoe(this,'Password',SPACE_NO)" 
        type="password" value="slide" />
  </div>
</div>
