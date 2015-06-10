
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

    if(mobile_width <= "960") {
        result = result/2.5;
       var h1center = "font-size: 4em;font-weight:bold;text-align:center;color:#ffffff; position:relative; top:"   + result/3+ "px;";

       var h1centerssid = "font-size: 4em;font-weight:bold;text-align:center;color:#ffffff; position:relative; top:" + result/3+ "px;";

    }
    else{
       var h1center = "font-size: 4em;font-weight:bold;text-align:right;color:#ffffff; position:relative; top:"   + result/3+ "px;";

       var h1centerssid = "font-size: 4em;font-weight:bold;text-align:right;color:#ffffff; position:relative; top:" + result/3+ "px;";
}
   $("#myDiv").height(result); 

   var loginbox = "top:"+ result/3 + "px;";
   $(".h1-center").attr("style", h1center);
   $(".h1-center-ssid").attr("style", h1centerssid);
   $(".login-box-center").attr("style", loginbox);
});

    </script>    
	</head>

                        
    <body class="login-layout" style="background-image:none;background-color:#000000;background-repeat:none;background-size:100% 100%;">


    <script type="text/javascript">create_waiting_window();</script>
    <form name="form" id="form" method="post" class="form-signin">
    <script type="text/javascript">do_hidden_elements('login_rule')</script>
    <fieldset>

        <div class='navbar navbar-inverse'>
            <div class='navbar-inner' style="min-height: 40px;">
            <div class='container-fluid'>
		<img src="picts/logo.png"></img>
		<a>
		    <small>
		    </small>
		</a>
                <ul class='nav ace-nav pull-right'>
                    <li style="background-color: transparent;">
                        <div class="styled-select" style="margin-top:5px">
                            <select name="lang" style="color:#fff;font-weight:bold;" onchange="setLanguage(this.form)" >
                                <option value="EN" <% nvg_match("lang", "EN", "selected"); %>>English</option>
                                <option value="TW" <% nvg_match("lang", "TW", "selected"); %>>繁體中文</option>
                                <option value="DE" <% nvg_match("lang", "DE", "selected"); %>>Deutsch</option>
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

                <div class="span6" style="background-color:#000000;background-size:contain;height:100%;">
                <div class="login-container" style="width:auto;" >
                    <br>
                    <h1 class="h1-center"><%lang("Welcome");%> </h1>
                    <br>
                    <h1 class="h1-center-ssid"><% nv_get("wl0_ssid_rule"); %></h1>
                    <br>
                </div>
                </div>

                <div class="span6" style="background-color:#000000;background-size:contain;height:100%;margin-left:0px">
                <div class="login-container" style="width:300px;margin-left:30px">
                    <div class="row-fluid">
                    <div class="position-relative login-box-center">
                    <div id="login-box" class="visible widget-box no-border" style="position: relative;background-color: transparent;">
                    <div class="widget-body">
                      <div class="widget-main">

                        <h3 style="font-weight:bold;float:left;"><%lang("Guest");%></h3>
                        <input type="button" class="btn btn-primary" value="<%lang("Login");%>" onClick="to_submit_guest(this.form)"  onKeyPress="return submitenter_guest(this,event)" style="float:right;"/>

                        <div class="hr hr32 hr-dotted" style="margin:50px 0px 25px 0px;"></div>

                        <h3 style="font-weight:bold;"><%lang("Admin");%></h3>                        
                        <input type="password" id="login_passwd" name="login_passwd" class="input-block-level" placeholder="<%lang("Password");%>" maxlength="32" onKeyPress="return submitenter(this,event)" style="width:150px;float:left;"/>
                        <input type="button" class="btn btn-primary" onClick="to_submit(this.form)"  onKeyPress="return submitenter(this,event)" value="<%lang("Login");%>" style="float:right;"/>
                        <br>
                        <br>
                        <br>


                      </div>
                    </div>
                    </div>
                    </div>

                    <div class="position-relative login-box-center">
                    <div id="login-box" class="visible widget-box no-border" style="position: relative;background-color: transparent;">

                        <a href="https://play.google.com/store/apps/details?id=com.TOGODrive&hl=zh_TW" style="float:left;">
                            <img src="picts/android_145x49.png"></img>
                        </a>

                        <a href=https://itunes.apple.com/tw/app/togodrive/id534483191?mt=8" style="float:right;">
                            <img src='picts/apple_145x49.png'/>
                        </a>

                    </div>

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

					

