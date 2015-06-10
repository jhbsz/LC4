
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
html,body{
height: 100%;
}
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

        function setLanguage(F){
          /*  setCookie("language", lang);*/            
            var ajaxData;
            $("#submit_button").val("login");
            $("#action").val("lang");
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
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

$(document).ready(function(){
    var mobile_width = screen.width;
    var result = $(window).height();
    if(mobile_width <= "480") {
        $("#login_passwd").attr("style", "width:185px;float:left;background-color: transparent; margin-left: 20px; margin-top:17px; border-color: transparent; float: left;"); 
        $("#login_admin").attr("style", "font-weight:bold;margin-top:20px; margin-left: 20px; color: white;"); 
        $("#guest_login").attr("style", "float:left; line-height:16px; margin-top:17px; background-color: transparent; border-color: transparent; margin-left:5px; color: white;"); 
        $("#login_enter").attr("style", "padding: 10px 25px 0px; line-height:16px; float: right; margin-top:17px; background: url('picts/card_silver_enter.png') no-repeat; border-color: transparent;"); 
    }
    $("#myDiv").height(result); 

    var loginbox = "top:"+ result/10 + "px;";
    $(".login-box-center").attr("style", loginbox);
    to_update_time();
});

    </script>    
	</head>

                        
    <body class="login-layout" style="background-image:none;background-color:#000000;background-repeat:none;background-size:100% 100%;">


    <script type="text/javascript">create_waiting_window();</script>
    <form name="form" id="form" method="post" class="form-signin">
    <script type="text/javascript">do_hidden_elements('login_rule')</script>
    <fieldset>

        <div class='navbar'>
            <div class='navbar-inner' style="min-height: 40px;">
            <div class='container-fluid'>
		<img src="picts/logo.png" style="padding-top: 10px;"></img>
		<a>
		    <small>
		    </small>
		</a>
                <ul class='nav ace-nav pull-right'>
                    <li style="background-color: transparent;">
                        <div class="styled-select" style="margin-top:5px">
                            <select name="lang" style="color:#fff;font-weight:bold;" onchange="setLanguage(this.form)" >
                                <option value="EN" <% nvg_match("lang", "EN", "selected"); %> style='color: black;'>English</option>
                                <option value="CE" <% nvg_match("lang", "CE", "selected"); %> style='color: black;'>Čeština</option>
                                <option value="DA" <% nvg_match("lang", "DA", "selected"); %> style='color: black;'>Dansk</option>
                                <option value="DE" <% nvg_match("lang", "DE", "selected"); %> style='color: black;'>Deutsch</option>
                                <option value="EP" <% nvg_match("lang", "EP", "selected"); %> style='color: black;'>Español (España)</option>
                                <option value="ES" <% nvg_match("lang", "ES", "selected"); %> style='color: black;'>Español (Mexicano)</option>
                                <option value="EX" <% nvg_match("lang", "EX", "selected"); %> style='color: black;'>Ελληνικά</option>
                                <option value="FR" <% nvg_match("lang", "FR", "selected"); %> style='color: black;'>Français</option>
                                <option value="IT" <% nvg_match("lang", "IT", "selected"); %> style='color: black;'>Italiano</option>
                                <option value="MA" <% nvg_match("lang", "MA", "selected"); %> style='color: black;'>Magyar</option>
                                <option value="NE" <% nvg_match("lang", "NE", "selected"); %> style='color: black;'>Nederlands</option>
                                <option value="NO" <% nvg_match("lang", "NO", "selected"); %> style='color: black;'>Norsk</option>
                                <option value="PO" <% nvg_match("lang", "PO", "selected"); %> style='color: black;'>Polski</option>
                                <option value="PB" <% nvg_match("lang", "PB", "selected"); %> style='color: black;'>Português do Brasil</option>
                                <option value="RO" <% nvg_match("lang", "RO", "selected"); %> style='color: black;'>Română</option>
                                <option value="SU" <% nvg_match("lang", "SU", "selected"); %> style='color: black;'>Suomi</option>
                                <option value="SV" <% nvg_match("lang", "SV", "selected"); %> style='color: black;'>Svenska</option>
                                <option value="TU" <% nvg_match("lang", "TU", "selected"); %> style='color: black;'>Türkçe</option>
                                <option value="PY" <% nvg_match("lang", "PY", "selected"); %> style='color: black;'>Русский</option>
                                <option value="TA" <% nvg_match("lang", "TA", "selected"); %> style='color: black;'>ไทย</option>
                                <option value="JP" <% nvg_match("lang", "JP", "selected"); %> style='color: black;'>日本語</option>
                                <option value="KR" <% nvg_match("lang", "KR", "selected"); %> style='color: black;'>한국어</option>
                                <option value="SC" <% nvg_match("lang", "SC", "selected"); %> style='color: black;'>简体中文</option>
                                <option value="TW" <% nvg_match("lang", "TW", "selected"); %> style='color: black;'>繁體中文</option>
                            </select>              
                        </div>
                    </li>
                </ul>
            </div>
            </div>
        </div>

        <div class="container-fluid" id="main-container" style="padding-right:0px;padding-left:0px;width:100%;">
            <div id="main-content" style="padding-right:0px;padding-left:0px;width:100%;">

            <div id="myDiv" class="row-fluid">

                <div class="span12" style="background-color:transparent; background-size:cover; height:100%; background-image: url('picts/desktop-1.jpg');">
                <div class="login-container" style="width:276px;">
                    <div class="row-fluid">
                    <div class="position-relative login-box-center">
                    <div id="login-box" class="visible widget-box no-border" style="position: relative; background-color: transparent">
                    <div class="widget-body" style="background-color: transparent;">
                      <div class="widget-main" style="background: url('picts/card_silver.png'); background-color:transparent; background-repeat: no-repeat; padding-bottom: 150px; border-radius: 10px;">

                        <h3 style="font-weight:bold;margin-top:80px;text-align:center;" id="h1-center-ssid">QGenie</h3>

                        <br></br>

                        <h4 id="login_admin" style="font-weight:bold;margin-top:20px; margin-left: -10px; color: white;">admin</h3>                        
                        <input type="password" id="login_passwd" name="login_passwd" class="input-block-level" placeholder="<%lang("Password");%>" maxlength="32" onKeyPress="return submitenter(this,event)" style="width:185px;float:left;background-color: transparent; margin-left: -15px; margin-top:17px; border-color: transparent; float: left;"/>
                        <input id="login_enter" type="button" onClick="to_submit(this.form)"  onKeyPress="return submitenter(this,event)" style="padding: 10px 14px 0px; line-height:16px; float: right; margin-top:17px; background: url('picts/card_silver_enter.png') no-repeat; border-color: transparent;"/>

                        <br></br>
                        <input type="button" id="guest_login" style="float:left; line-height:16px; margin-top:17px; background-color: transparent; border-color: transparent; margin-left:-15px; color: white;" onclick="to_submit_guest(this.form)" value="<%lang("Guest Login");%>"/>
                        <br>
                        <br>


                      </div>
                    </div>
                    </div>
                    </div>


            <div class="position-relative login-box-center">


		    <div class="span12" style="color:#ffffff;top:5px; font-size:11px;">
		        <script type="text/javascript"><%show_copyright();%></script>
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

					

