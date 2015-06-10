<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Camera</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var camera_objects =
        [ { name: 'camera_port' },
          { name: 'camera_auth_enable' },
          { name: 'camera_resolution'}
        ];

        var F; 

        function to_submit(F) {
            $("#submit_button").val("camera");
            $("#action").val("Apply");

            sel_change(1, 1, camera_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }


        function init() {
            enable = $('input[name=camera_enable]:checked').val();
            sel_change(enable, 1, camera_objects);

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
 	          <script type="text/javascript">do_hidden_elements('camera_rule');</script>
                <h2><%lang("AirCloud");%>-<%lang("Camera");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Camera");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Camera"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="camera_enable" onclick="init()"
                                <%nvg_attr_match("camera_enable","camera_rule","0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="camera_enable" onclick="init()"
                                <%nvg_attr_match("camera_enable","camera_rule","0","enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><% lang("Camera Port"); %></div>
                    <div class="span8">
                        HTTP <input type="text" class="num" maxlength="5" size= "5" name="camera_port" id="camera_port" 
                                onblur="valid_range(this,1,65535,'Port number')" value="<%nv_attr_get("camera_rule", "0", "port");%>">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Camera Account Login"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="camera_auth_enable" id="camera_auth_enable" onclick="init()"
                                <%nvg_attr_match("camera_auth_enable","camera_rule","0","auth_enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="camera_auth_enable" id="camera_auth_enable" onclick="init()"
                                <%nvg_attr_match("camera_auth_enable","camera_rule","0","auth_enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Camera Resolution"); %></div>
                    <div class="span8">
                        <select name="camera_resolution" id="camera_resolution">
                        <option value="1280x720" <%nvg_attr_match("camera_resolution", "camera_rule", "$0", "resolution", "1280x720", "selected");%>><% lang("1280x720"); %></option>
                        <option value="640x480" <%nvg_attr_match("camera_resolution", "camera_rule", "$0", "resolution", "640x480", "selected");%>><% lang("640x480"); %></option>
                        <option value="320x240" <%nvg_attr_match("camera_resolution", "camera_rule", "$0", "resolution", "320x240", "selected");%>><% lang("320x240"); %></option>
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