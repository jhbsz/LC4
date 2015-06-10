<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> -  Dynamic DNS</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var ddns_objects = new Array(<%nv_get("wan_num");%>);
        var ddns_type_objects = new Array(<%nv_get("wan_num");%>);
        var ddns_update_objects = new Array(<%nv_get("wan_num");%>);
        var F;

        function to_submit(F) {
            $("#submit_button").val("ddns");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

        }
        function disable_update() {
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
                sel_change(0, 1, ddns_update_objects[i]);
            }
        }
        function init() {
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
                var enable = $("input[name=ddns"+i+"_enable]:checked").val();
                sel_change(enable, 1, ddns_objects[i]);
                
                if (enable==1) {
                    if ($("#ddns"+i+"_type").is(":disabled")==false) {
                        var type = ($("#ddns"+i+"_type").val() == "dyndns") ? 1 : 0;
                        sel_change(type, 1, ddns_type_objects[i]);
                    }

                }
                
                var type_check = ($("#ddns"+i+"_type").val() == $("#ddns"+i+"_type_org").val() ) ? 0 : 1;
                var username_check = ($("#ddns"+i+"_username").val() == $("#ddns"+i+"_username_org").val() ) ? 0 : 1;
                var passwd_check = ($("#ddns"+i+"_passwd").val() == $("#ddns"+i+"_passwd_org").val() ) ? 0 : 1;
                var hostname_check = ($("#ddns"+i+"_hostname").val() == $("#ddns"+i+"_hostname_org").val() ) ? 0 : 1;
                var server_check = ($("#ddns"+i+"_server").val() == $("#ddns"+i+"_server_org").val() ) ? 0 : 1;
                if (type_check || username_check || passwd_check || hostname_check || server_check) disable_update();
            }
        }

        $(document).ready(function () {
            init();
        });

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('ddns_rule');</script>
                <h2><%lang("Setup");%>-<%lang("DDNS");%></h2>

              <% ddns_show_setting(); %>

        <div class="row">
            <div class="span12">
                <div class="submitFooter">
                    <input type="button" class="btn btn-primary" onClick="to_submit(this.form)" value="<%lang("Save Settings");%>">
                    <input type="reset" class="btn" value="<%lang("Cancel Changes");%>">
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