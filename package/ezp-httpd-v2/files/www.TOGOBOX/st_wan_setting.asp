        <div class="row">
            <div class="span12">
                <legend><%lang("WAN");%> <%increase("$0", "1");%></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("MAC Address");%></div>
                    <div class="span8"><% show_hwaddr("wan$0"); %></div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><%lang("Connection Type");%></div>
                    <div class="span8"><% nv_get("wan$0_proto"); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("IP Address");%></div>
                    <div class="span8"><% nv_get("wan$0_ipaddr"); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Subnet Mask");%></div>
                    <div class="span8"><% nv_get("wan$0_mask"); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Gateway");%></div>
                    <div class="span8"><% nv_get("wan$0_gateway"); %></div>
                </div>

            </div>
        </div><!-- row -->
        <br>
