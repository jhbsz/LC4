
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<title><% nv_get("model"); %> - Login</title>

        <%do_headmeta();%>
        <%do_basic_css();%>

        <%do_ace_font_css();%>
        <%do_ace_font_ie7_css();%>

        <!-- page specific plugin styles -->

        <!-- ace styles -->
        <%do_ace_basic_css();%>
        <%do_ace_basic_ie9_css();%>
        <%do_custom_css();%>

        <%do_custom_js();%>
        <%do_basic_js();%>

        <!-- page specific plugin scripts -->
        
        <!--[if lt IE 9]>
        <script type="text/javascript" src="js/excanvas.min.js"></script>
        <![endif]-->

        <%do_jquery_ui_js();%>
        <%do_jquery_other_js();%>
        <%do_jquery_flot_js();%>
        <%do_datatables_js();%>
        <%do_ace_basic_js();%>

<style>
.styled-select select  {
    background: transparent;
    width: 150px;
    padding: 8px;
    font-size: 16px;
    border: 1px ;
    height: 100%;
    -webkit-appearance: none; /*for chrome*/
}
.styled-select {
    width: 110px;
    height: 44px;
    overflow: hidden;
    background: url(picts/bord.png) no-repeat right #fff;
    background-color: transparent;
}
</style>

    <script type="text/javascript">

        function setLanguage(lang){
          /*  setCookie("language", lang);*/
        }

        function to_submit_guest(F) {
            $("#submit_button").val("login");
            $("#action").val("Login");

            var auth_value = encode64("guest:guest");
            setCookie("auth", auth_value);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function to_submit(F) {
            $("#submit_button").val("login");
            $("#action").val("Login");

            var auth_value = encode64("admin:" + $("#login_passwd").val() );
            setCookie("auth", auth_value);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);

        }

        function submitenter_guest(f,e){
            var keycode;
            if (window.event) keycode = window.event.keyCode;
            else if (e) keycode = e.which;
            else return true;

            if (keycode == 13) {
                to_submit_guest();
                return false;
            } else {
                return true;
            }
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

                        
    <body class="login-layout" style="background-image:url('picts/TGB_BG.png');background-repeat:none;background-size:100% 100%;">


    <script type="text/javascript">create_waiting_window();</script>
    <form name="form" id="form" method="post" class="form-signin">
    <script type="text/javascript">do_hidden_elements('login_rule')</script>
    <fieldset>

        <div class='navbar navbar-inverse'>
            <div class='navbar-inner' style="min-height: 40px;">
            <div class='container-fluid'>
                <ul class='nav ace-nav pull-right'>
                    <li style="background-color: transparent;">
                        <div class="styled-select">
                            <select name="lang" style="color:#fff;font-weight:bold;" onchange="setLanguage(this.value)" >
                                <option value="EN" <% nvg_match("lang", "EN", "selected"); %>>English</option>
                            </select>              
                        </div>
                    </li>
                </ul>
            </div>
            </div>
        </div>

        <div class="container-fluid" id="main-container">
            <div id="main-content">
                <br>
                <br>
                <br>
                <br>
                <br>
                <br>
                <br>
                <br>
            <div class="row-fluid">

                <div class="span3"></div>
                <div class="span3"></div>

                <div class="span3">
                <div class="login-container" style="width:auto;" >
                    <br>
                    <h1 style="font-size: 4em;font-weight:bold;text-align:center;color:white;">Welcome</h1>
                    <br>
                    <h1 style="font-size: 2.5em;font-weight:bold;text-align:center;color:white;"><% nv_get("wl0_ssid_rule"); %></h1>
                    <br>
                </div>
                </div>

                <div class="span3">
                <div class="login-container" style="width:300px;">


                    <div class="row-fluid">
                    <div class="position-relative">
                    <div id="login-box" class="visible widget-box no-border" style="position: relative;background-color: transparent;">
                    <div class="widget-body">
                      <div class="widget-main">

                        <h3 style="font-weight:bold;float:left;">Guest</h3>
                        <input type="button" class="btn btn-primary" value="Login" onClick="to_submit_guest(this.form)"  onKeyPress="return submitenter_guest(this,event)" style="float:right;"/>

                        <div class="hr hr32 hr-dotted" style="margin:50px 0px 25px 0px;"></div>

                        <h3 style="font-weight:bold;">Admin</h3>                        
                        <input type="password" id="login_passwd" name="login_passwd" class="input-block-level" placeholder="<%lang("Password");%>" maxlength="32" onKeyPress="return submitenter(this,event)" style="width:150px;float:left;"/>
                        <input type="button" class="btn btn-primary" onClick="to_submit(this.form)"  onKeyPress="return submitenter(this,event)" value="<%lang("Login");%>" style="float:right;"/>
                        <br>
                        <br>
                        <br>


                      </div>
                    </div>
                    </div>
                    </div>

                    <div class="position-relative">
                    <div id="login-box" class="visible widget-box no-border" style="position: relative;background-color: transparent;">

                        <a href="https://play.google.com/store/apps/details?id=com.TOGODrive&hl=zh_TW" style="float:left;">
                            <img src="picts/android_145x49.png"></img>
                        </a>

                        <a href=https://itunes.apple.com/tw/app/togodrive/id534483191?mt=8" style="float:right;">
                            <img src='picts/apple_145x49.png'/>
                        </a>

                    </div>
                    </div>




                    </div>

                </div>
                </div>

            </div><!-- row -->
            </div><!-- main-content -->

        </div> <!-- /container -->
    </fieldset>
    </form>
    </body>

</html>

					

