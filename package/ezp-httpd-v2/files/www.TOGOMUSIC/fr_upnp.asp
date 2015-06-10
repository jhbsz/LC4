<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - UPnP</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var upnp_objects =
        [ 
        { name: 'upnp_port' },
        { name: 'pmp_enable', type: 'radio' }
        ];                                    

        function onoffupnp() {
            enable_upnp = $('input[name=upnp_enable]:checked').val();
            sel_change(enable_upnp, 1, upnp_objects);
        }

        $(document).ready(function () {
            onoffupnp();
        });

        
        function to_submit(F) {
            $("#submit_button").val("fr_upnp");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
       }
        

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('upnp_rule');</script>
                <h2><%lang("Applications");%>-<%lang("UPnP");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("UPnP");%></legend>
                <div class="row show-grid">
                    <div class="span4">UPnP</div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="upnp_enable" id="upnp_enable" onclick="onoffupnp()"
                            <%nvg_attr_match("upnp_enable", "upnp_rule", "0", "enable", "1", "checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="upnp_enable" id="upnp_enable" onclick="onoffupnp()"
                            <%nvg_attr_match("upnp_enable", "upnp_rule", "0", "enable", "0", "checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>

                <input type="hidden" name="upnp_extif" value="wan0" />
                <input type="hidden" name="upnp_inif" value="lan0" />

                <div class="row show-grid">
                    <div class="span4"><% lang("UPnP Port"); %></div>
                    <div class="span8">
                        <input type="text" placeholder="" maxlength="5" size="5" name="upnp_port" id="upnp_port"  
                            onblur="valid_range(this,1,65535,'UPnP')" value="<%nvg_attr_get("upnp_port", "upnp_rule", "0", "port");%>">
                    </div>
                </div>
            </div>
        </div><!-- row -->

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
