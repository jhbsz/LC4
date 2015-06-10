<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - WAN Advanced</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var detect_objects = new Array(<%nv_get("wan_num");%>);
        var lb_objects = new Array(<%nv_get("wan_num");%>);
        var host_objects = new Array(<%nv_get("wan_num");%>);
        var F;

        function to_submit(F) {
            $("#submit_button").val("lb_basic");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function available_update() {
            F.submit_button.value = "lb_basic";
            F.action.value = "Gozila";
            F.submit();
        }

        function init() {
            F = document.getElementsByName("form")[0];
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
                var detect_enable = $("input[name=wan"+i+"_detect_enable]:checked").val();
                sel_change(detect_enable, 1, lb_objects[i]);
                if(detect_enable){
                    var detect_enable = ($("select[name=wan"+i+"_detect_type]").val() == "custom" ) ? 1 : 0;
                    sel_change(detect_enable, 1, host_objects[i]);
                }
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
 	          <script type="text/javascript">do_hidden_elements('lb_rule');</script>
                <h2><%lang("Setup");%>-<%lang("WAN Advanced");%></h2>

        <% lb_show_setting(); %>
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