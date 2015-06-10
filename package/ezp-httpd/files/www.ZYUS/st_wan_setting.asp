<style type="text/css"> 
#st_con {
    width:120px;
    margin:0px 0px 0px 0px;
 }
</style>
            <fieldset>
            <legend><%increase("$0", "1");%> <%lang("WAN");%></legend>

            <div class="setting">
              <div class="label"><%lang("Connection Status");%></div>
              <div id='wan$0_light'>&nbsp;</div>
            </div>

<%wan_show_wan_type_html_start("$0");%>
            <div class="setting">
              <div class="label">
                <%lang("MAC Address");%>
              </div><% show_hwaddr("wan$0"); %>
            </div>
<%wan_show_wan_type_html_end("$0");%>

            <div class="setting">
                <div class="label"> <%lang("Connection Type");%></div>
                <% nv_get("wan$0_proto"); %>
            </div>

            <div class="setting">
                <div class="label"> <%lang("IP Address");%> </div>
                <div id='wan$0_ipaddr'>&nbsp;</div>
            </div>

            <div class="setting">
                <div class="label"> <%lang("Subnet Mask");%> </div>
                <div id='wan$0_mask'>&nbsp;</div>
            </div>

            <div class="setting">
                <div class="label"> <%lang("Gateway");%> </div>
                <div id='wan$0_gateway'>&nbsp;</div>
            </div>

            <div class="setting">
                <div class="label"><%lang("Download");%></div>
                <div id='wan$0_rx'>&nbsp;</div>
            </div>

            <div class="setting">
                <div class="label"><%lang("Upload");%></div>
                <div id='wan$0_tx'>&nbsp;</div>
            </div>

            <input type='hidden' id='wan$0_status' />
<%wwan_show_wan_type_html_start("$0", "wwan");%>
            <div class="setting">
              <div class="label"><%lang("Modem Brand");%></div>
              <div id='wwan_mf' width=20>&nbsp;</div>
            </div>
            <div class="setting">
              <div class="label"><%lang("Modem Model");%></div>
              <div id='wwan_model' width=20>&nbsp;</div>
            </div>

<%wwan_show_wan_type_html_end("$0", "wwan");%>

            </fieldset><br />
