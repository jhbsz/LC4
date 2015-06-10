<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Status</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        $(document).ready(function () {
            $("#refresh").click(function(){
                window.location.reload();
            });
        });

    </script>

    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('ad_basic_rule');</script>
                <h2><%lang("Status");%>-<%lang("Router");%></h2>


        <div class="row">
            <div class="span12">
                <legend><%lang("Router Information");%></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("Model Name");%></div>
                    <div class="span8"><% nv_get("model"); %></div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><%lang("Firmware Version");%></div>
                    <div class="span8"><% show_version(""); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("License");%></div>
                    <div class="span8"><% show_license_status(); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Current Time");%></div>
                    <div class="span8"><% show_localtime(); %></div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Running Time");%></div>
                    <div class="span8"><% show_uptime(); %></div>
                </div>

            </div>
        </div><!-- row -->
        <br>
        <%st_show_wan_setting();%>
        <%st_show_lan_setting();%>
        <%prod_show_html("start", "wlan");%>
        <%st_show_wl_setting();%>
        <%prod_show_html("end", "wlan");%>

        <div class="row">
            <div class="span12">
                <div class="submitFooter">
                    <input type="button" class="btn btn-primary" name="refresh" id="refresh" value="<%lang("Refresh");%>">
                </div>
            </div>
        </div><!-- row -->

        </form>
        
        <div class="row">
            <div class="span12">
                   <script type="text/javascript">
                        <%show_copyright();%>
                    </script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>