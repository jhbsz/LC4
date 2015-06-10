<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Wireless</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var F;

        var wl_adv_objects = new Array(<%nv_get("wl_rule_num");%>);

        function to_submit(F) {
            $("#submit_button").val("wl_adv");
            $("#action").val("Apply");

            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                sel_change(1, 1, wl_adv_objects[i]);
            }

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function SelWlAdv(F) {
            F.submit_button.value = "wl_adv";
            F.action.value = "Gozila";
            F.submit();
        }

        function init() {
            F = document.getElementsByName("form")[0];
            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                var enable = $("#input[name=wl"+i+"_enable]:checked").val();
                sel_change(enable, 1, wl_adv_objects[i]);
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
 	          <script type="text/javascript">do_hidden_elements('wl_advanced_rule');</script>
                <h2><%lang("Wireless");%>-<%lang("Advanced");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Region Setting");%></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("Region");%></div>
                    <div class="span8">
                        <select name="wl_region" id="wl_region">
                          <option value='0' <%nvg_match("wl_region", "0", "selected");%>>
                            <%lang("US, Canada and Taiwan");%> (<%lang("channel");%> 1 - 11)
                          </option>
                          <option value='1' <%nvg_match("wl_region", "1", "selected");%>>
                            <%lang("Europe, Australia and Hong Kong");%> (<%lang("channel");%> 1 - 13)
                          </option>
                          <option value='5' <%nvg_match("wl_region", "5", "selected");%>>
                            <%lang("Japan");%> (<%lang("channel");%> 1 - 14)
                          </option>
                        </select>
                    </div>
                </div>
            </div>
        </div><!-- row -->
        <% wl_show_setting("wl_adv_setting.asp"); %>

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