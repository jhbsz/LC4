<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - OpenDNS</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var opendns_objects = new Array(<%nv_get("wan_num");%>);
        function to_submit(F) {
            var enable_check = 0;
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
                var enable = $("[name=opendns"+i+"_enable]:checked").val();
                if ( enable == "1"  ){
                    enable_check = 1;
                }
            }
            if ( enable_check == "1" ){
                if ( confirm("<%lang("All traffic will be redirected to OpenDNS servers if this feature is enabled.");%>") ) {
                    $("#submit_button").val("opendns");
                    $("#action").val("Apply");

                    open_waiting_window();            
                    var ajaxData;
                    ajaxData = ajax_data( $("#form").serializeArray() );
                    do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
                } 
            }else{
                $("#submit_button").val("opendns");
                $("#action").val("Apply");

                open_waiting_window();            
                var ajaxData;
                ajaxData = ajax_data( $("#form").serializeArray() );
                do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
            }
        }


        function init() {
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
                enable_table = $("[name=opendns"+i+"_enable]:checked").val();
                sel_change(enable_table, 1, opendns_objects[i]);
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
 	          <script type="text/javascript">do_hidden_elements('wan_opendns_rule');</script>
                <h2><%lang("Security");%>-<%lang("OpenDNS");%></h2>

              <% opendns_show_setting(); %>

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