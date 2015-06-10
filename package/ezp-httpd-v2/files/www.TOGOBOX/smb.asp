<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Samba</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
        var smbd_objects =
        [ { name: 'smbd_share_name' },
          { name: 'smbd_read_only' },
          { name: 'smbd_guest_ok' },
          { name: 'smbd_ext_access' },
          { name: 'smbd_codepage' },
          { name: 'smbd_passwd' },
          { name: 'smbd_passwd_confirm' }
        ];
        var F; 

        function to_submit(F) {
            if ($("#smbd_passwd").val() != $("#smbd_passwd_confirm").val()) {
                alert("Confirmed password did not match Entered Password.  Please re-enter password");
                $("#smbd_passwd_confirm").focus();
                $("#smbd_passwd_confirm").select();
                return false;
            }

            $("#submit_button").val("samba");
            $("#action").val("Apply");
            sel_change(1, 1, smbd_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }


        function init() {
            enable = $('input[name=smbd_enable]:checked').val();
            sel_change(enable, 1, smbd_objects);
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
 	          <script type="text/javascript">do_hidden_elements('smbd_rule');</script>
                <h2><%lang("Storage");%>-<%lang("Samba Server");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Samba Server");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Samba Server"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="smbd_enable" id="smbd_enable" onclick="init()"
                                <%nvg_attr_match("smbd_enable","smbd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="smbd_enable" id="smbd_enable" onclick="init()"
                                <%nvg_attr_match("smbd_enable","smbd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Samba Server Share Name");%></div>
                    <div class="span8">
                        <input type="text" name="smbd_share_name" id="smbd_share_name" size="30" maxLength="30"
                            value='<%nvg_attr_get("smbd_share_name", "smbd_rule",   "0", "share_name");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Samba Server Allow Guest"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="smbd_guest_ok" id="smbd_guest_ok" onclick="init()"
                                <%nvg_attr_match("smbd_guest_ok","smbd_rule","0","guest_ok","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="smbd_guest_ok" id="smbd_guest_ok" onclick="init()"
                                <%nvg_attr_match("smbd_guest_ok","smbd_rule","0","guest_ok","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Samba Server Read Only"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="smbd_read_only" id="smbd_read_only" onclick="init()"
                                <%nvg_attr_match("smbd_read_only","smbd_rule","0","read_only","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="smbd_read_only" id="smbd_read_only" onclick="init()"
                                <%nvg_attr_match("smbd_read_only","smbd_rule","0","read_only","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Samba Server External Access"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="smbd_ext_access" id="smbd_ext_access" onclick="init()"
                                <%nvg_attr_match("smbd_ext_access","smbd_rule","0","ext_access","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="smbd_ext_access" id="smbd_ext_access" onclick="init()"
                                <%nvg_attr_match("smbd_ext_access","smbd_rule","0","ext_access","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Language");%></div>
                    <div class="span8">
                        <select name="smbd_codepage" id="smbd_codepage">
                            <option value="cp437"<%nvg_attr_match("smbd_codepage","smbd_rule", "0", "codepage", "cp437", "selected");%>>
                                <%lang("United States, Canada");%></option>
                            <option value="cp850" <% nvg_attr_match("smbd_codepage","smbd_rule", "0", "codepage", "cp850", "selected");%>>
                                <%lang("Europe");%></option>
                            <option value="cp936" <% nvg_attr_match("smbd_codepage","smbd_rule", "0", "codepage", "cp936", "selected");%>>
                                <%lang("Simplified Chinese");%></option>
                            <option value="cp950" <% nvg_attr_match("smbd_codepage","smbd_rule", "0", "codepage", "cp950", "selected");%>>
                                <%lang("Traditional Chinese");%></option>
                            <option value="cp932" <% nvg_attr_match("smbd_codepage","smbd_rule", "0", "codepage", "cp932", "selected");%>>
                                <%lang("Japanese");%></option>
                            <option value="cp949" <% nvg_attr_match("smbd_codepage","smbd_rule", "0", "codepage", "cp949", "selected");%>>
                                <%lang("Korean");%></option>
                            <option value="utf8" <% nvg_attr_match("smbd_codepage","smbd_rule", "0", "codepage", "utf8", "selected");%>>
                                <%lang("Unicode");%></option>
                        </select>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Samba Password"); %></div>
                    <div class="span8">
                        <input type="password" maxlength="32" size="30" value="slide" name="smbd_passwd" id="smbd_passwd" onblur="valid_name(this,'Password',SPACE_NO)">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Re-type Password"); %></div>
                    <div class="span8">
                        <input type="password" maxlength="32" size="30" value="slide" name="smbd_passwd_confirm" id="smbd_passwd_confirm" onblur="valid_name(this,'Password',SPACE_NO)">
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span12"><% lang("( Default user name is admin, default password is admin )"); %></div>
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