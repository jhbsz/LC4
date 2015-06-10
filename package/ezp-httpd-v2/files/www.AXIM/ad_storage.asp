<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Administration</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
  
        var F;
        var ext_storage_objects = new Array(1);
        ext_storage_objects[0] = [
        { name: 'storage_target_ip' },
        { name: 'storage_target_dir' },
        { name: 'storage_user' },
        { name: 'storage_passwd' },
        { name: 'storage_group' },
        { name: 'storage_ext_connect' }
        ];

        function to_format(F) {
            F = document.getElementsByName("form")[0];
            F.action.value='Format';
            if (!confirm('<%lang("Are you sure to format USB storage?\nAll data on it will be lost!");%>')) 
                return;
            $("#format").attr("disabled", true);

            F.getAttributeNode("action").value = "format.cgi";
            F.submit_button.value = "ad_storage";
            F.submit();
            open_waiting_window();            
            $("#waiting-window-msg").html('<%lang("Please note: Router will blink a red light during the re-formatting process. Do not remove USB device or turn off the router.");%>');
            return true;
        }


        function to_submit(F) {
            $("#submit_button").val("ad_storage");
            $("#action").val("Apply");
            sel_change(1, 1, ext_storage_objects[0]);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

        }

        function init() {
          /* for some browser reload issue ex:firefox */
          $("#format").attr("disabled", false);

          <%prod_comment_js_start("PROD_CAT_K", "0", "6", "");%>

          var enable = $("input[name=storage_ext_enable]:checked").val();
          sel_change(enable, 1, ext_storage_objects[0]);
          if (enable==1) {
              $("#format_type").attr("disabled", true);
              $("#format").attr("disabled", true);
          } else {
              $("#format_type").attr("disabled", false);
              $("#format").attr("disabled", false);
          }
          <%prod_comment_js_end("PROD_CAT_K", "0", "6", "");%>
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
 	          <script type="text/javascript">do_hidden_elements('storage_rule');</script>
                <h2><%lang("Storage");%>-<%lang("Storage Management");%></h2>


        <div class="row">
            <div class="span12">
                <legend><% lang("Storage Device"); %></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("Language");%></div>
                    <div class="span8">
                        <select name="storage_codepage" id="storage_codepage">
                            <option value="437"<%nvg_attr_match("storage_codepage","storage_rule", "0", "codepage", "437", "selected");%>>
                                <%lang("United States, Canada");%></option>
                            <option value="850" <% nvg_attr_match("storage_codepage","storage_rule", "0", "codepage", "850", "selected");%>>
                                <%lang("Europe");%></option>
                            <option value="936" <% nvg_attr_match("storage_codepage","storage_rule", "0", "codepage", "936", "selected");%>>
                                <%lang("Simplified Chinese");%></option>
                            <option value="950" <% nvg_attr_match("storage_codepage","storage_rule", "0", "codepage", "950", "selected");%>>
                                <%lang("Traditional Chinese");%></option>
                            <option value="932" <% nvg_attr_match("storage_codepage","storage_rule", "0", "codepage", "932", "selected");%>>
                                <%lang("Japanese");%></option>
                            <option value="949" <% nvg_attr_match("storage_codepage","storage_rule", "0", "codepage", "949", "selected");%>>
                                <%lang("Korean");%></option>
                        </select>
                    </div>
                </div>


                <div class="row show-grid">
                    <div class="span4"><%lang("Swap File Size");%></div>
                    <div class="span8">
                        <select name="storage_swap_size" id="storage_swap_size">
                            <option value="16384" <% nvg_attr_match("storage_swap_size","storage_rule", "0", "swap_size", "16384", "selected");%>>
                                <%lang("64MB");%></option>
                            <option value="32768" <% nvg_attr_match("storage_swap_size","storage_rule", "0", "swap_size", "32768", "selected");%>>
                                <%lang("128MB");%></option>
                            <option value="65535" <% nvg_attr_match("storage_swap_size","storage_rule", "0", "swap_size", "65535", "selected");%>>
                                <%lang("256MB");%></option>
                        </select>
                    </div>
                </div>
            </div>
        </div><!-- row -->
        <br>

<%prod_comment_html_start("PROD_CAT_K", "0", "7" , "");%>
        <div class="row">
            <div class="span12">
                <legend><% lang("External Storage"); %></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("External Storage"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="storage_ext_enable" id="storage_ext_enable" onclick="init()"
                                <%nvg_attr_match("External Storage","storage_rule","0","ext_enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="storage_ext_enable" id="storage_ext_enable" onclick="init()"
                                <%nvg_attr_match("External Storage","storage_rule","0","ext_enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Target IP");%></div>
                    <div class="span8">
                        <input type='text' name="storage_target_ip" id="storage_target_ip" size="16" maxLength="15"
                            value='<%nvg_attr_get("Target IP", "storage_rule", "0", "target_ip");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Target Directory");%></div>
                    <div class="span8">
                        <input type='text' name="storage_target_dir" id="storage_target_dir" size="21" maxLength="20"
                            value='<%nvg_attr_get("Target Diretcory", "storage_rule", "0", "target_dir");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("User Name");%></div>
                    <div class="span8">
                        <input type='text' name="storage_user" id="storage_user" size="21" maxLength="20"
                            value='<%nvg_attr_get("User Name", "storage_rule", "0", "user");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Password");%></div>
                    <div class="span8">
                        <input type='text' name="storage_passwd" id="storage_passwd" size="21" maxLength="20"
                            value='<%nvg_attr_get("Password", "storage_rule", "0", "pw");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Group");%></div>
                    <div class="span8">
                        <input type='text' name="storage_group" id="storage_group" size="21" maxLength="20"
                            value='<%nvg_attr_get("Group", "storage_rule", "0", "group");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Reconnect");%></div>
                    <div class="span8">
                        <input type="button" class="btn" name="storage_ext_connect" id="storage_ext_connect"
                            onclick="to_update('0', 'smbc')" value="<%lang("Connect");%>" />
                    </div>
                </div>

            </div>
        </div><!-- row -->
<%prod_comment_html_end("PROD_CAT_K", "0", "7" , "");%>
        <br>
        <div class="row">
            <div class="span12">
                <legend><% lang("Format"); %></legend>

                <div class="row show-grid">
                    <div class="span4"><%lang("File System Type");%></div>
                    <div class="span8">
                        <select name="format_type" id="format_type">
                            <option value="vfat"><%lang("FAT");%></option>
                            <option value="ext3"><%lang("EXT3");%></option>
                        </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Format USB HDD"); %></div>
                    <div class="span8">
                        <input type="button" class="btn" name="format" id="format" value='<%lang("Format");%>' onclick= "to_format(this.form)">
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