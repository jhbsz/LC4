<fieldset>
  <legend><%lang("Wireless Network");%> <%increase("$0", "1");%>  </legend>
  <div class="setting">
     <div class="label"><%lang("Wireless Channel");%></div><%nv_attr_get("wl_basic_rule","$0","channel");%>
  </div>
  <% wlv_show_entry_setting("$0", "st_wlv_setting.asp");%>
</fieldset><br />
