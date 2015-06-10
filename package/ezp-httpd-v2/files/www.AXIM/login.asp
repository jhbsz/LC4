<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Login</title>
    <%do_headmeta();%>

    <style type="text/css">
      body {
        padding-top: 40px;
        padding-bottom: 40px;
        background-color: #f5f5f5;
      }

      .form-signin {
        max-width: 300px;
        padding: 19px 29px 29px;
        margin: 0 auto 20px;
        background-color: #fff;
        border: 1px solid #e5e5e5;
        -webkit-border-radius: 5px;
           -moz-border-radius: 5px;
                border-radius: 5px;
        -webkit-box-shadow: 0 1px 2px rgba(0,0,0,.05);
           -moz-box-shadow: 0 1px 2px rgba(0,0,0,.05);
                box-shadow: 0 1px 2px rgba(0,0,0,.05);
      }
      .form-signin .form-signin-heading,
      .form-signin .checkbox {
        margin-bottom: 10px;
      }
      .form-signin input[type="text"],
      .form-signin input[type="password"] {
        font-size: 16px;
        height: auto;
        margin-bottom: 15px;
        padding: 7px 9px;
      }

    </style>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">

        function setLanguage(lang){
          /*  setCookie("language", lang);*/
        }

        function to_submit(F) {
            $("#submit_button").val("login");
            $("#action").val("Login");

            var auth_value = encode64($("#login_username").val() + ':' + $("#login_passwd").val() );
            setCookie("auth", auth_value);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

        }

        function submitenter(f,e){
            var keycode;
            if (window.event) keycode = window.event.keyCode;
            else if (e) keycode = e.which;
            else return true;

            if (keycode == 13) {
                to_submit();
                return false;
            } else {
                return true;
            }
        }

        function init() {
         /*   setCookie("language", "EN"); */
            setCookie("auth", "");
        }

    </script>
    </head>

    <body>
        <div class="container">
            <script type="text/javascript">create_waiting_window();</script>

            <div class="row">
                <div class="span12">

                  <form name="form" id="form" method="post" class="form-signin">
           	          <script type="text/javascript">do_hidden_elements('login_rule')</script>	
                      <h2 class="form-signin-heading"><%lang("Login");%></h2>
                      
                      <input type="text" id="login_username" name="login_username" class="input-block-level" placeholder="<%lang("User Name");%>"
                       maxlength="32" onblur="valid_name(this,'User Name')" onKeyPress="return submitenter(this,event)">

                      <input type="password" id="login_passwd" name="login_passwd" class="input-block-level" placeholder="<%lang("Password");%>"
                       maxlength="32" onKeyPress="return submitenter(this,event)">
                      
                      <select name="lang" onchange="setLanguage(this.value)" >
                          <option value="EN" <% nvg_match("lang", "EN", "selected"); %>>English</option>
                          <option value="TW" <% nvg_match("lang", "TW", "selected"); %>>繁體中文</option>
                          <option value="GE" <% nvg_match("lang", "GE", "selected"); %>>Deutsch</option>
                          <option value="GB" <% nvg_match("lang", "GB", "selected"); %>>簡體中文</option>
                      </select>
                    <br>
                    <input type="button" class="btn btn-large btn-primary" name="save_button" value="<%lang("Login");%>" 
                      onClick="to_submit(this.form)"  onKeyPress="return submitenter(this,event)">
                  </form>

                </div>
            </div><!-- row -->

            <div class="row">
                <div class="span12">
                    <div class="submitFooter">
                       <script type="text/javascript">
                            <%show_copyright();%>
                        </script>
                    </div>
                </div>
            </div><!-- row -->

        </div> <!-- /container -->
    </body></html>