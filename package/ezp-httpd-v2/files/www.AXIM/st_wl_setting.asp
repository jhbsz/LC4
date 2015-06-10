        <div class="row">
            <div class="span12">
                <legend><%lang("Wireless Network");%> <%increase("$0", "1");%></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("Wireless Channel");%></div>
                    <div class="span8"><%nv_attr_get("wl_basic_rule","$0","channel");%></div>
                </div>
                <% wlv_show_entry_setting("$0", "st_wlv_setting.asp");%>
            </div>
        </div><!-- row -->
        <br>


<!--
<fieldset>
  <legend>  </legend>
  <div class="setting">
     <div class="label"></div>
  </div>
</fieldset><br />
-->