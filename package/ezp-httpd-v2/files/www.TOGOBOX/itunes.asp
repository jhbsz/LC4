<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - ITUNES</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        itunes_objects =
        [
          { name: 'itunes_passwd' },
          { name: 'itunes_servername' },
        ];

        function to_submit(F) {
            $("#submit_button").val("itunes");
            $("#action").val("Apply");
            sel_change(1, 1, itunes_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function init() {
            enable = $('input[name=enable]:checked').val();
            sel_change(enable, 1, itunes_objects);
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
 	          <script type="text/javascript">do_hidden_elements('itunes_rule');</script>
                <h2><%lang("Storage");%>-<%lang("iTunes");%></h2>
              <% itunes_show_setting(); %>


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