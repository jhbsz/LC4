<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - TurboNAT</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var F;

        function to_submit(F) {
            $("#submit_button").val("turbonat");
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
 	          <script type="text/javascript">do_hidden_elements('turbonat_rule');</script>
                <h2><%lang("Bandwidth");%>-<%lang("TurboNAT");%></h2>

<!--
<%prod_show_html_end("hwnat", "0", "100");%>
<input type="hidden" name="hwnat_enable" value="0"> 
<%prod_show_html_start("hwnat", "0", "100");%>
-->
<%prod_show_html_start("hwnat", "0", "100");%>

        <div class="row">
            <div class="span12">
                <legend><%lang("Hardware NAT");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Hardware NAT"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="hwnat_enable" id="hwnat_enable" onclick="init()"
                                <%nvg_attr_match("hwnat_enable", "nat_rule","0","hw_enable","1","checked");%>>
                            <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="hwnat_enable" id="hwnat_enable" onclick="init()"
                                <%nvg_attr_match("hwnat_enable", "nat_rule","0","hw_enable","0","checked");%>>
                            <%lang("Disable");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->
<%prod_show_html_end("hwnat", "0", "100");%>

        <div class="row">
            <div class="span12">
                <legend><%lang("TurboNAT");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("TurboNAT"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="turbonat_enable" id="turbonat_enable" onclick="init()"
                                <%nvg_match("turbonat_enable","1","checked");%>>
                            <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="turbonat_enable" id="turbonat_enable" onclick="init()"
                                <%nvg_match("turbonat_enable","0","checked");%>>
                            <%lang("Disable");%>
                        </label>
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