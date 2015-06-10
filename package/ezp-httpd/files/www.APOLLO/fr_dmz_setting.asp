<script language="JavaScript">
dmz_objects[$0] = [ { name: 'fr_dmz$0_ipaddr' } ];
</script>
<fieldset>
  <legend>DMZ - <%lang("WAN");%> <%increase_wan("$0", "1");%></legend>

  <div class="setting">
    <div class="label">DMZ</div>
    <div class='radiobutton'><input type="radio" value="1" name="fr_dmz$0_enable" onclick="init()"
        <% nvg_attr_match("fr_dmz$0_enable","fr_dmz_rule","$0","enable","1","checked");%>><%lang("Enable");%></input></div>
    <div class='radiobutton'><input type="radio" value="0" name="fr_dmz$0_enable" onclick="init()"
        <% nvg_attr_match("fr_dmz$0_enable","fr_dmz_rule","$0","enable","0","checked");%>><%lang("Disable");%></input></div>
  </div>

  <div class="setting">
    <div class="label"><% lang("DMZ IP Address"); %></div>
    <input class="num" name="fr_dmz$0_ipaddr" size="17" maxlength="15" value='<%nv_attr_get("fr_dmz_rule",$0,"ipaddr");%>'
        onblur='valid_ipaddr(this, "DMZ IP Address")' />
  </div>
</fieldset><br />
