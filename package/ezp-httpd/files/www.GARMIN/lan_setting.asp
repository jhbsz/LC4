<fieldset>
  <legend><%lang("LAN");%> <%increase("$0", "1");%></legend>

  <input type="hidden" value="1" name="lan$0_enable" />
    <div class="setting">
      <div class="label"><%lang("Internal IP Address");%></div>
      <input type="text" maxLength="15" size="16" name="lan$0_static_ipaddr"
          value='<%nv_attr_get("lan_static_rule", "$0", "ipaddr");%>' 
          onblur='valid_ipaddr(this, "Internal IP Address")' />
    </div>
  
      <div class="setting">
      <div class="label"><%lang("Netmask");%></div>
        <select name="lan$0_static_mask">
          <option value="16" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","16","selected");%>>
                255.255.0.0</option>
          <option value="17" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","17","selected");%>>
                255.255.128.0</option>
          <option value="18" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","18","selected");%>>
                255.255.192.0</option>
          <option value="19" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","19","selected");%>>
                255.255.224.0</option>
          <option value="20" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","20","selected");%>>
                255.255.240.0</option>
          <option value="21" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","21","selected");%>>
                255.255.248.0</option>
          <option value="22" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","22","selected");%>>
                255.255.252.0</option>
          <option value="23" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","23","selected");%>>
                255.255.254.0</option>
          <option value="24" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","24","selected");%>>
                255.255.255.0</option>
          <option value="25" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","25","selected");%>>
                255.255.255.128</option>
          <option value="26" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","26","selected");%>>
                255.255.255.192</option>
          <option value="27" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","27","selected");%>>
                255.255.255.224</option>
          <option value="28" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","28","selected");%>>
                255.255.255.240</option>
          <option value="29" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","29","selected");%>>
                255.255.255.248</option>
          <option value="30" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","30","selected");%>>
                255.255.255.252</option>
          <option value="31" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","31","selected");%>>
                255.255.255.254</option>
          <option value="32" <%nvg_attr_match("lan$0_static_mask","lan_static_rule","$0","mask","32","selected");%>>
                255.255.255.255</option>
        </select>
      </div>
    <input type="hidden" name="lan$0_static_gateway" value='<%nv_attr_get("lan_static_rule", "$0", "gateway");%>' />
    <input type="hidden" name="lan$0_static_dns" value='<%nv_attr_get("lan_static_rule", "$0", "dns");%>' />
    <input type="hidden" name="lan$0_static_wins" value='<%nv_attr_get("lan_static_rule", "$0", "wins");%>' />

  <div class="setting">
    <div class="label"> <%lang("Spanning Tree Protocol");%> (STP) </div>
    <input type="radio" value="1" name="lan$0_stp"
        <%nvg_attr_match("lan$0_stp","lan_main_rule","$0","stp","1","checked");%>><%lang("Enable");%>
    <input type="radio" value="0" name="lan$0_stp"
        <%nvg_attr_match("lan$0_stp","lan_main_rule","$0","stp","0","checked");%>><%lang("Disable");%>
  </div>

  <div class="setting">
    <div class="label"><%lang("MTU");%></div>
      <input size="6" maxLength="4" name="lan$0_mtu" 
            value='<%nvg_attr_get("lan$0_mtu", "lan_main_rule", "$0", "mtu");%>'
            onblur='valid_number(this, "MTU")' /> <%lang("Bytes");%>
  </div>

  <input type="hidden" name="lan$0_weight" value="1"/>

</fieldset><br />
