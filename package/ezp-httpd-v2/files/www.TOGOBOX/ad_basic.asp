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

        $(document).ready(function () {
            $("#upgrade").attr("disabled", false);
            $("#import").attr("disabled", false);
        });

        function valid_password() {
            //if (F.http_passwd.value != F.http_passwd_confirm.value) {
            if ($("#http_passwd").val() != $("#http_passwd_confirm").val()){
                alert("Confirmed password did not match Entered Password.  Please re-enter password");
                $("#http_passwd").focus();
                $("#http_passwd").select();
                //F.http_passwd_confirm.focus();
                //F.http_passwd_confirm.select();
                return false;
            }
            return true;
        }


        function to_reboot(F) {
            $("#action").val("Reboot");
            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("reboot.cgi", "POST", "text", true, ajaxData, 3000);
            return true;
        }

        function to_export(F) {
            window.location.replace("<%nv_get("brand");%>.cfg");
        }

        function disp_confirm(){
            return confirm("<%lang("Are you sure to restore to the default configuration?");%>");
        }

        function to_default(F) {

            $("#submit_button").val("ad_basic");
            $("#action").val("Default");
            if (disp_confirm() == false) {
                return;
            }
            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("default.cgi", "POST", "text", true, ajaxData, 3000);
            return true;
        }

        function to_import(F) {
             $("#binfile").attr("disabled", true);
             name = $("#cfgfile").val();
            if (name.search(/\.(cfg)$/i) == -1) {
                alert('A ".cfg" file expected.');
                return;
            }
            if (!confirm('<%lang("Are you sure to import configuration by using");%> ' + name + '?')) 
                    return;

            F = document.getElementsByName("form")[0];
            F.getAttributeNode("action").value = "upgrade.cgi";
            F.encoding = "multipart/form-data";
            F.submit_button.value = "ad_basic";
            $("#import").attr("disabled", true);
            // TODO: Ajax support 
            F.submit();
            open_waiting_window();            
            return true;
        }


        function clock() {
            var t = ((new Date()).getTime() - startTime) / 1000;
            $("#upgrade-time").text(Math.floor(t / 60) + ':' + pad(Math.floor(t % 60)));
        }

        function to_upgrade(F) {
            $("#cfgfile").attr("disabled", true);
            name = $("#binfile").val();

            if (name.search(/\.(bin)$/i) == -1) {
                alert('A ".bin" file expected.');
                return;
            }
            if (!confirm('<%lang("Are you sure to upgrade by using");%> ' + name + '?')) 
                    return;

            startTime = (new Date()).getTime();
            setInterval('clock()', 1000);
            F = document.getElementsByName("form")[0];
            $("#upgrade").attr("disabled", true);
            F.getAttributeNode("action").value = "upgrade.cgi";
            F.encoding = "multipart/form-data";
            F.submit_button.value = "ad_basic";
            open_upgrade_window();
            F.submit();

        }

        function to_submit(F) {
            if ($("#http_passwd").val() != $("#http_passwd_confirm").val()){
                alert('Password confirmation is not matched.');
                return;
            }
            if(!valid_password(F)) return;
            var backup_password=$("#http_passwd").val();
            $("#http_passwd").val(encode64(F.http_passwd.value));
            $("#submit_button").val("ad_basic");
            $("#action").val("Apply");
            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
            $("#http_passwd").val(backup_password);
       }

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>
        <script type="text/javascript">create_upgrade_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('http_rule')</script>
               <input type="hidden" name= "reboot_button"/> 
                <h2><%lang("Admin");%>-<%lang("Management");%></h2>

            <% ad_x_mode_switch(); %>
            <% ad_vlanport_config(); %>

        <div class="row">
            <div class="span12">
                <legend><% lang("Administration Interface"); %></legend>
                <div class="row show-grid">
                    <div class="span4"><%lang("Language");%></div>
                    <div class="span8">
                        <select name="lang" id="lang">
                            <option value="EN" <% nvg_match("lang", "EN", "selected"); %>>English</option>
                            <option value="TW" <% nvg_match("lang", "TW", "selected"); %>>正體中文</option>
                            <option value="GE" <% nvg_match("lang", "GE", "selected"); %>>Deutsch</option>
                            <option value="GB" <% nvg_match("lang", "GB", "selected"); %>>簡體中文</option>
                        </select>
                    </div>
                </div>
                <div class="row show-grid">
                    <div class="span4"><% lang("Administrator Password"); %></div>
                    <div class="span8">
                        <input type="password" maxlength="32" size="30" value="slide" name="http_passwd" id="http_passwd">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Re-type Password"); %></div>
                    <div class="span8">
                        <input type="password" maxlength="32" size="30" value="slide" name="http_passwd_confirm" id="http_passwd_confirm">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Remote Management"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name= "http_rmgt_enable" id= "http_rmgt_enable" 
                                <% nvg_attr_match("http_rmgt_enable","http_rule","0","rmgt_enable","1","checked"); %>><% lang("Enable"); %></input>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name= "http_rmgt_enable" id= "http_rmgt_enable" 
                                <% nvg_attr_match("http_rmgt_enable","http_rule","0","rmgt_enable","0","checked"); %>><% lang("Disable"); %></input>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Management Port"); %></div>
                    <div class="span8">
                        HTTP <input type="text" class="num" maxlength="5" size="5" name="http_port" id="http_port" 
                                onblur="valid_range(this,1,65535,'Port number')" value="<%nv_attr_get("http_rule", "0", "port");%>">
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>

        <div class="row">
            <div class="span12">
                <legend><% lang("Reboot"); %></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Reboot"); %></div>
                    <div class="span8">
                        <input type="button" class="btn btn-small" value='<%lang("Reboot Router");%>' onclick= "to_reboot(this.form)">
                    </div>
                </div>
            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12">
                <legend><% lang("Configuration"); %></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Configuration Export"); %></div>
                    <div class="span8">
                        <input type="button" class="btn btn-small" value='<%lang("Export");%>' onclick="to_export(this.form)">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Default Configuration Restore"); %></div>
                    <div class="span8">
                        <input type="button" class="btn btn-small" value='<%lang("Default");%>' onclick="to_default(this.form)">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"> <% lang("Configuration Import"); %></div>
                    <div class="span8">
                        <div class="fileupload fileupload-new" data-provides="fileupload">
                          <span class="btn btn-file">
                            <span class="fileupload-new">Select file</span>
                            <span class="fileupload-exists">Change</span>
                            <input type="file" name="cfgfile" id="cfgfile" />
                          </span>
                          <span class="fileupload-preview"></span>
                          <a href="#" class="close fileupload-exists" data-dismiss="fileupload" style="float: none">×</a>
                          <input type="button" class="fileupload-exists" name="import" id="import" value='<%lang("Import");%>' onclick= "to_import(this.form)">
                        </div><!-- fileupload -->
                    </div>
                </div>

            </div>
        </div><!-- row -->

<%prod_show_html_start("firmware", "0", "100");%>
        <div class="row">
            <div class="span12">
                <legend><% lang("Firmware"); %></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Firmware Upgrade"); %></div>
                    <div class="span8">
                        <div class="fileupload fileupload-new" data-provides="fileupload">
                          <span class="btn btn-file">
                            <span class="fileupload-new">Select file</span>
                            <span class="fileupload-exists">Change</span>
                            <input type="file" name="binfile" id="binfile" />
                          </span>
                          <span class="fileupload-preview"></span>
                          <a href="#" class="close fileupload-exists" data-dismiss="fileupload" style="float: none">×</a>
                          <input type="button" class="fileupload-exists" name="upgrade" id="upgrade" value='<%lang("Upgrade");%>' onclick= "to_upgrade(this.form)">
                        </div><!-- fileupload -->

                      <!--
                      <input type="file" name="binfile" id="binfile" size="30" />
                      <input type="button" class="btn btn-small" name="upgrade" id="upgrade" value='<%lang("Upgrade");%>' onclick= "to_upgrade(this.form)">
                      -->
                    </div>
                </div>
            </div>
        </div><!-- row -->
        <br>
<%prod_show_html_end("firmware", "0", "100");%>

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
