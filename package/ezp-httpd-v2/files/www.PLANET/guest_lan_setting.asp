<script language="JavaScript">
guest_lan_enable_object = 
[ { name: 'guest_lan_enable', type: 'radio' }
];

guest_lan_objects = 
[ { name: 'guest_lan_ipaddr' },
  { name: 'guest_lan_mask' }
];

</script>
<div class="label"><% lang("Guest LAN"); %></div>
<input type="radio" value="1" name="guest_lan_enable"
    onchange="SelGuest(1)"
    <%nvg_attr_match("guest_lan_enable","guest_lan_rule","0","enable","1","checked");%>> <%lang("Enable");%>
</input>
<input type="radio" value="0" name="guest_lan_enable" 
    onchange="SelGuest(1)"
    <%nvg_attr_match("guest_lan_enable","guest_lan_rule","0","enable","0","checked");%>> <%lang("Disable");%>
</input>
</br> 
<div class="label"><%lang("Guest LAN IP Address");%></div>
<input type="text" maxLength="15" size="16" name="guest_lan_ipaddr"
    value='<%nv_attr_get("guest_lan_rule", "0", "ipaddr");%>' 
    onblur='valid_ipaddr(this, "Internal IP Address")' />
</br> 
<div class="label"><%lang("Guest LAN Netmask");%></div>
<select name="guest_lan_mask">
<option value="16" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","16","selected");%>> 255.255.0.0</option>
<option value="17" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","17","selected");%>> 255.255.128.0</option>
<option value="18" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","18","selected");%>> 255.255.192.0</option>
<option value="19" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","19","selected");%>> 255.255.224.0</option>
<option value="20" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","20","selected");%>> 255.255.240.0</option>
<option value="21" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","21","selected");%>> 255.255.248.0</option>
<option value="22" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","22","selected");%>> 255.255.252.0</option>
<option value="23" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","23","selected");%>> 255.255.254.0</option>
<option value="24" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","24","selected");%>> 255.255.255.0</option>
<option value="25" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","25","selected");%>> 255.255.255.128</option>
<option value="26" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","26","selected");%>> 255.255.255.192</option>
<option value="27" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","27","selected");%>> 255.255.255.224</option>
<option value="28" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","28","selected");%>> 255.255.255.240</option>
<option value="29" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","29","selected");%>> 255.255.255.248</option>
<option value="30" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","30","selected");%>> 255.255.255.252</option>
<option value="31" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","31","selected");%>> 255.255.255.254</option>
<option value="32" <%nvg_attr_match("guest_lan_mask","guest_lan_rule","0","mask","32","selected");%>> 255.255.255.255</option>
</select>
