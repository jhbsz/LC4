<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - SNMP</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">

        var F;
        function to_submit(F) {
            $("#submit_button").val("fr_vnc");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);

            /*
            F.submit_button.value = "fr_vnc";
            F.action.value = "Apply";
            ajax_preaction(F, "apply.cgi", 6000);
            display_mask(F);
            */
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
 	          <script type="text/javascript">do_hidden_elements('vnc_rule');</script>
                <h2><%lang("Applications");%>-<%lang("VNC KVM");%></h2>


        <div class="row">
            <div class="span12">
                <legend><%lang("VNC KVM");%></legend>

                <div class="row show-grid">
                    <div class="span4"><%lang("VNC KVM");%></div>
                    <div class="span8">

                        <label class="radio inline">
                            <input type="radio" value="1" name="vnc_enable" id="vnc_enable" onclick="init()"
                                <% nvg_attr_match("vnc_enable", "vnc_rule", "0", "enable", "1", "checked"); %>>
                                <% lang("Enable"); %>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="vnc_enable" id="vnc_enable" onclick="init()"
                                <% nvg_attr_match("vnc_enable", "vnc_rule", "0", "enable", "0", "checked"); %>>
                                <% lang("Disable"); %>
                        </label>

                    </div>
                </div>

            </div>
        </div><!-- row -->

	        <input type="hidden" name="vnc_vport" id="vnc_vport" value="5900" />
	        <input type="hidden" name="vnc_sport" id="vnc_sport" value="5500" />
	        <input type="hidden" name="vnc_mode" id="vnc_mode" value="1" />
	        <input type="hidden" name="vnc_ipaddr" id="vnc_ipaddr" value="0.0.0.0" />
	        <input type="hidden" name="vnc_session" id="vnc_session" value="16" />
	        <input type="hidden" name="vnc_event_enable" id="vnc_event_enable" value="1" />
	        <input type="hidden" name="vnc_event_port" id="vnc_event_port" value="2002" />
	        <input type="hidden" name="vnc_event_ipaddr" id="vnc_event_ipaddr" value="127.0.0.1" />

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