                <div class="row show-grid">
                    <div class="span4"><%lang("Wireless SSID");%> <%increase("$1", "1");%></div>
                    <div class="span8"><%nv_attr_get("wl$0_ssid_rule","$1","ssid");%></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("MAC Address");%></div>
                    <div class="span8"><% show_wl_ifname_hwaddr("$0", "$1"); %></div>
                </div>


<!--
<div class="setting">
     <div class="label"></div>
</div>
<div class="setting">
   <div class="label"></div>
</div>
-->
