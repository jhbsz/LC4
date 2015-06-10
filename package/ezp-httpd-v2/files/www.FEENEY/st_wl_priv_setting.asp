<fieldset>
  <legend><%lang("Wireless Network");%></legend>
  <div class="setting">
       <div class="label"><%lang("Wireless SSID");%></div><%nv_attr_get("wl$0_ssid_rule","$1","ssid");%>
  </div>
  <div class="setting">
     <div class="label"><%lang("MAC Address");%></div><% show_wl_ifname_hwaddr_priv("$0", "$1"); %>
  </div>
</fieldset><br />
