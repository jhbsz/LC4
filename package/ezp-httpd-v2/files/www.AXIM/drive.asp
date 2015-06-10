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
        var drive_objects =
            [ { name: 'drive_guest' },
              { name: 'drive_ext_access' },
              { name: 'drive_port' }
            ];

        $(document).ready(function () {
            init();
        });

        function init() {
            enable = $('input[name=drive_enable]:checked').val();
            sel_change(enable, 1, drive_objects);
        }

        
        function to_submit(F) {
            $("#submit_button").val("drive");
            $("#action").val("Apply");
            sel_change(1, 1, drive_objects);
            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);
       }

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('drive_rule');</script>
                <h2><%lang("AirCloud");%>-<%lang("Drive");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Drive");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Drive"); %></div>
                    <div class="span8">

                        <label class="radio inline">
                            <input type="radio" value="1" name="drive_enable" id="drive_enable" onclick="init()" <%nvg_attr_match("drive_enable","drive_rule","0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="0" name="drive_enable" id="drive_enable" onclick="init()" <%nvg_attr_match("drive_enable","drive_rule","0","enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><% lang("Drive Allow Guest"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="drive_guest" id="drive_guest" onclick="init()" <%nvg_attr_match("drive_guest","drive_rule","0","guest","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="drive_guest" id="drive_guest" onclick="init()" <%nvg_attr_match("drive_guest","drive_rule","0","guest","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Drive External Access"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="drive_ext_access" id="drive_ext_access" onclick="init()"
                                <%nvg_attr_match("drive_ext_access","drive_rule","0","ext_access","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="drive_ext_access" id="drive_ext_access" onclick="init()"
                                <%nvg_attr_match("drive_ext_access","drive_rule","0","ext_access","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Drive Port"); %></div>
                    <div class="span8">HTTP <input type="text" maxlength="5" size="5" name="drive_port" id="drive_port" onblur="valid_range(this,1,65535,'Port number')" 
                                                value="<%nv_attr_get("drive_rule", "0", "port");%>">
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