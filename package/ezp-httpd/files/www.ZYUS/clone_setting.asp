<script language="JavaScript">
clone_objects[$2] =
[
  { name: '$1$2_hwaddr_clone_hwaddr' },
  { name: '$1$2_get_my_mac' }
];

</script>
  <fieldset>
    <legend><%lang("MAC Address Clone");%> - <%increase("$2", "1");%> <%lang("$0");%>
    </legend> <input type="hidden" name="$1$2_clone_http_client">

      <div class="setting">
        <div class="label">
          <% lang("Clone WAN MAC"); %>
        </div><input type="radio" value="1" name= "$1$2_hwaddr_clone_enable" onclick= "init()" 
        <%nvg_attr_match("$1$2_hwaddr_clone_enable","$1_hwaddr_clone_rule","$2","enable","1","checked");%>><%lang("Enable");%>
        <input type="radio" value="0" name= "$1$2_hwaddr_clone_enable" onclick= "init()" 
        <%nvg_attr_match("$1$2_hwaddr_clone_enable","$1_hwaddr_clone_rule","$2","enable","0","checked");%>>
        <%lang("Disable");%>
      </div>

      <div class="setting">
        <div class="label">
          <%lang("MAC Address");%>
        </div><input type="text" maxlength="17" size="17" name= "$1$2_hwaddr_clone_hwaddr" 
    value='<%nvg_attr_get("$1$2_hwaddr_clone_hwaddr","$1_hwaddr_clone_rule","$2","hwaddr");%>' onblur='valid_hwaddr(this, "MAC Address")'>
    <input type="button" name="$1$2_get_my_mac" id="$1$2_get_my_mac"
        value='<%lang("Get My MAC");%>' onclick= "to_get_mac(this.form, '$1$2_hwaddr_clone_hwaddr')">
           </div>
  </fieldset><br>
