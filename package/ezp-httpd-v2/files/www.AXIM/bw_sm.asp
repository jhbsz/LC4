<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Services</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var sm_objects =
        [ { name: 'sm_mode'  , type: 'radio' }
        ];
        function to_submit(F) {
            $("#submit_button").val("bw_sm_mode");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function init() {
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
 	          <script type="text/javascript">do_hidden_elements('sm_rule');</script>
                <h2><%lang("Bandwidth");%>-<%lang("Session Manager");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Session Manager");%></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("Recycle Mode");%></div>
                    <div class="span8">
                        <select name="bw_sm_mode" id="bw_sm_mode">
                            <option value="fast" <% nvg_match("sm_mode", "fast", "selected"); %>><%lang("Fast");%></option>
                            <option value="regular" <% nvg_match("sm_mode", "regular", "selected"); %>><%lang("Regular");%></option>
                            <option value="slow" <% nvg_match("sm_mode", "slow", "selected"); %>><%lang("Slow");%></option>
                        </select>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>
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