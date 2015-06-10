
<!DOCTYPE html>
<html lang="en">
	<head>
		
		<meta charset="utf-8" />
		<title><% nv_get("model"); %> - myQNAPcloud</title>
		<%do_headmeta();%>
		<%do_basic_css();%>

		<%do_ace_font_css();%>
		<%do_ace_font_ie7_css();%>

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

		<!-- ace scripts -->
		<%do_ace_basic_js();%>


    <script type="text/javascript">
        function setLanguage(lang){
        /*  setCookie("language", lang);*/
            var data_array = {};
            data_array['action'] = 'lang';
            data_array['lang'] = lang;
            data_array['sub_button'] = 'exp_myQNAPcloud';
            do_ajax_action ("apply.cgi", "POST", "text", true, data_array, 3000);
        }

        var wan0_status = "<% nv_attr_get("","wan_status_rule", "0", "state");%>";
        var license_invalid = "";
        var qid_status = "";          
        var qcloud_qid = "";          
        var qcloud_dev_name = "";          
        var sign_in = "sign-in";
        var sign_out = "sign-out";
        var qcloud_get_web_mgmt_outport = "";
        var qcloud_get_file_station_outport = "";
        var qcloud_test_fetch_result_web_mgmt = "";
        var user_qcloud_site = "";
        var qcloud_test_fetch_result_file_station = "";
        var qid_last_update_status = "";
        var qid_last_update_time = "";
        var web_mgm_server = "<%lang("Management Server");%>";
        var file_station_server = "<%lang("File Server");%>";
        var file_station_upnpc_setting_inport = "<%nvg_attr_get("","upnpc_setting_rule","1", "inport");%>";
        var web_mgm_upnpc_setting_inport = "<%nvg_attr_get("","upnpc_setting_rule","0", "inport");%>";
        var get_myqnapcloud_id_qid = "<%nvg_attr_get("","myqnapcloud_id_rule","0", "qid");%>";
        var get_qcloud_singin_status = "<%nvg_attr_get("","myqnapcloud_id_rule", "0", "status");%>";
        var check_web_mgm_upnpc_status_rule = "<%nvg_attr_get("","upnpc_status_rule", "0", "fetch_result");%>";      
        var check_file_station_upnpc_status_rule = "<%nvg_attr_get("","upnpc_status_rule", "1", "fetch_result");%>";      
        var lighttpd_path = "/dav.asp ";
        var get_web_mgmt_url = "";
        var get_file_station_url = "";
        var submit_qcloud_qid = "";
        var submit_qcloud_pw = "";
        var submit_qcloud_device_name = "";
        var tmp_qcloud_qid = "";
        var tmp_qcloud_pw = "";
        var tmp_qcloud_device_name = "";
        var tag_qcloud_upnptest_action_localstorage = "";
        var tmp_qcloud_upnptest_action_localstorage = "0";

         $(document).ready(function(){
                <%show_myqnapcloud_status();%>
                //license_invalid=1:production, license_invalid=0:alpha 
                if (license_invalid == "1"){ 
                   $("#qcloud_device_name_href").text(".alpha-myqnapcloud.com");
                } else if (license_invalid == "0") { 
                   $("#qcloud_device_name_href").text(".myqnapcloud.com");
                }
                tmp_qcloud_qid = localStorage.getItem("submit_qcloud_qid");
                tmp_qcloud_pw = localStorage.getItem("submit_qcloud_pw");
                tmp_qcloud_device_name = localStorage.getItem("submit_qcloud_device_name");
                
                tmp_qcloud_upnptest_action_localstorage = localStorage.getItem("tag_qcloud_upnptest_action_localstorage");
                if (qid_status == sign_in) {
                    localStorage.removeItem("submit_qcloud_qid");
                    localStorage.removeItem("submit_qcloud_pw");
                    localStorage.removeItem("submit_qcloud_device_name");
                    $("#qcloud-signin-page").attr("style","display:none");
                    $("#sign-in-btn").attr("value", "<%lang("Sign Out");%>");
                    $("#manage-qcloud-account").attr("style","");
                    if (qcloud_dev_name != null) {
                    if (license_invalid == "1"){
                      user_qcloud_site = qcloud_dev_name + ".alpha-myqnapcloud.com";
                    $("#qcloud_info").val(user_qcloud_site);
                    } else if (license_invalid == "0") { 
                      user_qcloud_site = qcloud_dev_name + ".myqnapcloud.com";
                    $("#qcloud_info").val(user_qcloud_site);
                    }
                        var qcloud_info_len_size = $("#qcloud_info").val().length;
                        var qcloud_info_width_len_size = 100 + (qcloud_info_len_size / 15) * 100;
                        var body_width_len_size = 200 + (qcloud_info_len_size / 15) * 100;
                        var header_width_len_size = (200-12) + (qcloud_info_len_size / 15) * 100;
                        $("#qcloud_info").attr('style','width:' + qcloud_info_width_len_size + 'px');
                        $("div[class='widget-body']").attr('style','width:' + body_width_len_size + 'px');
                        $("div[class='widget-header']").attr('style','width:' + header_width_len_size + 'px');
                    }
                    if ((wan0_status == "4") && (check_web_mgm_upnpc_status_rule == "" || check_file_station_upnpc_status_rule == "" )) {
                        to_test_submit();
                    }
                        
                } 
                if (qid_status == sign_out) {
                    $("#qcloud-signout-page").attr("style","display:none");
                    $("#sign-in-btn").attr("value", "<%lang("Sign In");%>");
                    $('#qcloud_qid').val(tmp_qcloud_qid);
                    $('#qcloud_pw').val(tmp_qcloud_pw);
                    $('#qcloud_device_name').val(tmp_qcloud_device_name);
                    body_width_len_size = 330 + 153;
                    header_width_len_size = 330 + (153-12);
                    $("div[class='widget-body']").attr('style','width:' + body_width_len_size + 'px');
                    $("div[class='widget-header']").attr('style','width:' + header_width_len_size + 'px');
                    
                }
                
                if (qcloud_dev_name != null) {
                    if (license_invalid == "1"){
                       user_qcloud_site = qcloud_dev_name + ".alpha-myqnapcloud.com";
                        $("#qcloud_info").val(user_qcloud_site);
                    } else if (license_invalid == "0") { 
                       user_qcloud_site = qcloud_dev_name + ".myqnapcloud.com";
                       $("#qcloud_info").val(user_qcloud_site);
                    }
               }

                if (qcloud_qid != "(null)") {
                    var qcloud_qid_signed_len_size = $("#qcloud_qid_signed").val().length;
                    var qcloud_qid_signed_width_len_size = 150 + (qcloud_qid_signed_len_size / 15) * 100;
                    $("#qcloud_qid_signed").attr('style','width:' + qcloud_qid_signed_width_len_size + 'px');
                    $("#qcloud_qid_signed").val(qcloud_qid);
                }
                /******* test fetch result******/
                /* Initialize the hidden of test_status_icon */
                if (qcloud_test_fetch_result_file_station == "" && qcloud_test_fetch_result_web_mgmt == "")  {
                    $("#test_status_icon").attr("style","display:none");
                } else if (qid_status == sign_in){
                        var qcloud_info_len_size = $("#qcloud_info").val().length;
                        var qcloud_info_width_len_size = 100 + (qcloud_info_len_size / 15) * 100;
                        var body_width_len_size = 200 + (qcloud_info_len_size / 15) * 100;
                        var header_width_len_size = (200-12) + (qcloud_info_len_size / 15) * 100;
                        $("#qcloud_info").attr('style','width:' + qcloud_info_width_len_size + 'px');
                        $("div[class='widget-body']").attr('style','width:' + body_width_len_size + 'px');
                        $("div[class='widget-header']").attr('style','width:' + header_width_len_size + 'px');
                }

                /* test fetch result status for text**/
                if (qcloud_test_fetch_result_file_station == "1" && qcloud_test_fetch_result_web_mgmt =="1") { //ok
                    $("#qcloud_fetch_result").text("<%lang("Status");%>" + " : ");
                    $("#qcloud_fetch_status_result").text("<%lang("OK");%>");
                    $("#qcloud_fetch_result").attr("style","color:black");
                    $("#qcloud_fetch_status_result").attr("style","color:black");
                    $("#test_status_icon").removeClass("icon-remove-sign");
                    $("#test_status_icon").addClass("icon-ok-sign");
                    $("#test_status_icon").attr("style","color:green");
                    $("#qcloud_fetch_result").attr("style","display:none");
                    $("#qcloud_fetch_status_result").attr("style","display:none");
                    $("#test_status_icon").attr("style","display:none");
                } else if (qcloud_test_fetch_result_file_station == "") { //Initialize
                    $("#qcloud_fetch_result").text("");
                    $("#qcloud_fetch_result").attr("style","display:none");
                    $("#qcloud_fetch_status_result").attr("style","display:none");
                    $("#test_status_icon").attr("style","display:none");
                } else {   //failed
                    $("#qcloud_fetch_result").text("<%lang("Status");%>" + " : ");
                    $("#qcloud_fetch_status_result").text("<%lang("Failed");%>");
                    $("#qcloud_fetch_result").attr("style","color:black");
                    $("#qcloud_fetch_status_result").attr("style","color:black");
                    $("#test_status_icon").removeClass("icon-ok-sign");
                    $("#test_status_icon").addClass("icon-remove-sign");
                    $("#test_status_icon").attr("style","color:red");
                    $("#qcloud_fetch_result").attr("style","display:none");
                    $("#qcloud_fetch_status_result").attr("style","display:none");
                    $("#test_status_icon").attr("style","display:none");
                }

                /*sign status*/
                if (qid_status == sign_in) {
                    $("#qcloud_sign_status").text("<%lang("Status");%>" + " : ");
                    $("#qcloud_sign_status_result").text("<%lang("Signed In");%>");
                    $("#qcloud_sign_status").attr("style","color:black");
                    $("#qcloud_sign_status_result").attr("style","color:black");
                } else if (qid_status == sign_out) {
                    $("#qcloud_sign_status").text("<%lang("Status");%>" + " : ");
                    $("#qcloud_sign_status_result").text("<%lang("Signed Out");%>");
                    $("#qcloud_sign_status").attr("style","color:red");
                    $("#qcloud_sign_status_result").attr("style","color:red");
                }
                
                /*last update status*/
                if (qid_last_update_status == "success") {
                    $("#qcloud_last_update_status").text("<%lang("myQNAPcloud Link Status");%>" + " : ");
                    $("#qcloud_last_update_status").attr("style","color:black");
                    $("#last_update_status_icon").removeClass("icon-remove-sign");
                    $("#last_update_status_icon").addClass("icon-ok-sign");
                    $("#last_update_status_icon").attr("style","color:green");
                    $("#qcloud_last_update_ret").text("<%lang("OK");%>");
                    $("#qcloud_last_update_ret").attr("style","color:black");
                    $("#qcloud_last_update_ret_code").attr("style","display:none");
                } else if (qid_last_update_status == "failed") {
                    $("#qcloud_last_update_status").text("<%lang("myQNAPcloud Link Status");%>" + " : ");
                    $("#qcloud_last_update_status").attr("style","color:black");
                    $("#last_update_status_icon").removeClass("icon-ok-sign");
                    $("#last_update_status_icon").addClass("icon-remove-sign");
                    $("#last_update_status_icon").attr("style","color:red");
                    $("#qcloud_last_update_ret").text("<%lang("Failed");%>");
                    $("#qcloud_last_update_ret").attr("style","color:black");
                    $("#qcloud_last_update_ret_code").text("(" + qcloud_last_update_time_ret + ")");
                    $("#qcloud_last_update_ret_code").attr("style","color:black");
                }

                /*show test fetch outport of upnpc for web_mgmt port:80*/
                if (qcloud_get_web_mgmt_outport != "" && qcloud_test_fetch_result_web_mgmt == '0' && tmp_qcloud_upnptest_action_localstorage == '1') { //failed
                    get_web_mgmt_80_url = "http://" + user_qcloud_site;
                    get_web_mgmt_url = "http://" + user_qcloud_site + ":" + qcloud_get_web_mgmt_outport;
                    $("#qcloud_get_web_mgmt_outport").text("myQNAPcloud" + " [ " + web_mgm_server + " ] "+ "Port:" + web_mgm_upnpc_setting_inport);
                        if ( qcloud_get_web_mgmt_outport == web_mgm_upnpc_setting_inport) {
                            $("#show_qcloud_get_web_mgmt_outport").text(get_web_mgmt_80_url);
                        } else {
                            $("#show_qcloud_get_web_mgmt_outport").text(get_web_mgmt_url);
                        }
                    $("#qcloud_get_web_mgmt_outport").attr("style","color:black");
                    $("#qcloud_get_web_mgmt_outport_icon").removeClass("icon-ok-sign");
                    $("#qcloud_get_web_mgmt_outport_icon").addClass("icon-remove-sign");
                    $("#qcloud_get_web_mgmt_outport_icon").attr("style","color:red");
                    $("#qcloud_get_web_mgmt_outport_ret").text("<%lang("Failed");%>");
                    $("#qcloud_get_web_mgmt_outport_ret").attr("style","color:black");
                    var web_mgmt_outport_icon_string = "<%lang("The router does not support UPnP protocol or you have not enabled the UPnP function on the router.");%>";
                    document.getElementById('qcloud_get_web_mgmt_outport_icon').title = web_mgmt_outport_icon_string;
                } else if (qcloud_get_web_mgmt_outport != "" && qcloud_test_fetch_result_web_mgmt == '1' && tmp_qcloud_upnptest_action_localstorage == '1') {//ok
                    get_web_mgmt_80_url = "http://" + user_qcloud_site;
                    get_web_mgmt_url = "http://" + user_qcloud_site + ":" + qcloud_get_web_mgmt_outport;
                    $("#qcloud_get_web_mgmt_outport").text("myQNAPcloud" + " [ " + web_mgm_server + " ] " + "Port:" + web_mgm_upnpc_setting_inport);
                        if ( qcloud_get_web_mgmt_outport == web_mgm_upnpc_setting_inport) {
                            $("#show_qcloud_get_web_mgmt_outport").text(get_web_mgmt_80_url);
                        } else {
                            $("#show_qcloud_get_web_mgmt_outport").text(get_web_mgmt_url);
                        }
                    $("#qcloud_get_web_mgmt_outport").attr("style","color:black");
                    $("#qcloud_get_web_mgmt_outport_icon").removeClass("icon-remove-sign");
                    $("#qcloud_get_web_mgmt_outport_icon").addClass("icon-ok-sign");
                    $("#qcloud_get_web_mgmt_outport_icon").attr("style","color:green");
                    $("#qcloud_get_web_mgmt_outport_ret").text("<%lang("OK");%>");
                    $("#qcloud_get_web_mgmt_outport_ret").attr("style","color:black");
                    var web_mgmt_outport_icon_string = "<%lang("You may connect to your QGenie from anywhere over the network by myQNAPcloud services.");%>";
                    document.getElementById('qcloud_get_web_mgmt_outport_icon').title = web_mgmt_outport_icon_string;
                } else if (qcloud_test_fetch_result_file_station == "" || tmp_qcloud_upnptest_action_localstorage == null ) {
                    $("#qcloud_get_web_mgmt_outport").text("");
                    $("#qcloud_get_web_mgmt_outport_icon").attr("style","display:none");
                    $("qcloud_get_web_mgmt_outport_ret").text("");
                }
                
                /*show test fetch outport of upnpc for file_station*/
                if (qcloud_get_file_station_outport != "" && qcloud_test_fetch_result_file_station == '0' && tmp_qcloud_upnptest_action_localstorage == '1') {
                    get_file_station_url = "http://" + user_qcloud_site + ":" + qcloud_get_file_station_outport + lighttpd_path;
                    $("#qcloud_get_file_station_outport").text("myQNAPcloud" + " [ " + file_station_server + " ] " + "Port:"+ file_station_upnpc_setting_inport);
                    $("#show_qcloud_get_file_station_outport").text(get_file_station_url);
                    $("#qcloud_get_file_station_outport").attr("style","color:black");
                    $("#qcloud_get_file_station_outport_icon").removeClass("icon-ok-sign");
                    $("#qcloud_get_file_station_outport_icon").addClass("icon-remove-sign");
                    $("#qcloud_get_file_station_outport_icon").attr("style","color:red");
                    $("#qcloud_get_file_station_outport_ret").text("<%lang("Failed");%>");
                    $("#qcloud_get_file_station_outport_ret").attr("style","color:black");
                    var file_station_outport_icon_string = "<%lang("The router does not support UPnP protocol or you have not enabled the UPnP function on the router.");%>";
                    document.getElementById('qcloud_get_file_station_outport_icon').title = file_station_outport_icon_string;
                    localStorage.removeItem("tag_qcloud_upnptest_action_localstorage");
                } else if (qcloud_get_file_station_outport != "" && qcloud_test_fetch_result_file_station == '1' && tmp_qcloud_upnptest_action_localstorage == '1') { 
                   get_file_station_url = "http://" + user_qcloud_site + ":" + qcloud_get_file_station_outport + lighttpd_path;
                    $("#qcloud_get_file_station_outport").text("myQNAPcloud" + " [ " + file_station_server + " ] " + "Port:"+ file_station_upnpc_setting_inport);
                    $("#show_qcloud_get_file_station_outport").text(get_file_station_url);
                    $("#qcloud_get_file_station_outport").attr("style","color:black");
                    $("#qcloud_get_file_station_outport_icon").removeClass("icon-remove-sign");
                    $("#qcloud_get_file_station_outport_icon").addClass("icon-ok-sign");
                    $("#qcloud_get_file_station_outport_icon").attr("style","color:green");
                    $("#qcloud_get_file_station_outport_ret").text("<%lang("OK");%>");
                    $("#qcloud_get_file_station_outport_ret").attr("style","color:black");
                    var file_station_outport_icon_string = "<%lang("You may connect to your QGenie from anywhere over the network by myQNAPcloud services.");%>";
                    document.getElementById('qcloud_get_file_station_outport_icon').title = file_station_outport_icon_string;
                    localStorage.removeItem("tag_qcloud_upnptest_action_localstorage");
                } else if (qcloud_test_fetch_result_file_station == "" || tmp_qcloud_upnptest_action_localstorage == null) {
                    $("#qcloud_get_file_station_outport").text("");
                    $("#qcloud_get_file_station_outport_ret").text("");
                    $("#qcloud_get_file_station_outport_icon").attr("style","display:none");
                }
          
        });//---------------ready
         
        function to_signin_up_href() {
                if (license_invalid == "1"){
                   window.open('https://www.alpha-myqnapcloud.com/auth/signup?lang=en');
                } else if (license_invalid == "0") { 
                   window.open('https://www.myqnapcloud.com/auth/signup?lang=en');
                }
        }  
      
        function valid_device_mail_char(I) {
            var pattern = /^([a-zA-Z0-9_-])+([a-zA-Z0-9_+\.-])+@([a-zA-Z0-9_-])+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z]+$/;        
            if(!pattern.test(I))
            {
               alert("<%lang("Please enter a valid email address.");%>");
                return false;
            }
            return true;   
        }

        function valid_device_password_char(I) {
            if(I.length<6 || I.length>64) {
               alert("<%lang("The minimum length for password is 6.");%>");
               return false;
            }
               return true;
        }

        function valid_device_name_char(I) {
            if(I.length<3 || I.length>30) {
               alert("<%lang("The device name must be 3-30 characters long and can only contain letters (a-z or A-Z) or numbers (0-9).");%>");
               return false;
            }
            for(i=0 ; i<I.length; i++){
                ch = I.charAt(i);
                if((ch > '9' || ch < '0') && (ch > 'z' || ch < 'a') && (ch > 'Z' || ch < 'A')){
                alert("<%lang("The device name must be 3-30 characters long and can only contain letters (a-z or A-Z) or numbers (0-9).");%>");
                return false;
                }   
           }       
                
            return true;
        }   
        
        function qcloud_valid_check() {
            if ($("#qcloud_qid").val() == '') {
                alert("<%lang("Please enter your myQNAPcloud ID.");%>");
                $("#qcloud_qid").focus();
                $("#qcloud_qid").select();
                return false;

            }
            if ($("#qcloud_pw").val() == '') {
                alert("<%lang("Please enter your myQNAPcloud password.");%>");
                $("#qcloud_pw").focus();
                $("#qcloud_pw").select();
                return false;

            }
            if ($("#qcloud_device_name").val() == '') {
                alert("<%lang("Please enter your myQNAPcloud Device Name.");%>");
                $("#qcloud_device_name").focus();
                $("#qcloud_device_name").select();
                return false;

            }
            return true;
        }
        
        function redirect_myqnapcloud_service() {
            if ( license_invalid == '1') {
                window.open("https://www.alpha-myqnapcloud.com");
            } else {
                window.open("https://www.myqnapcloud.com");
            }
        }     

        function go_web_mgmt_url() {
                window.open(get_web_mgmt_url);
        }     

        function go_file_station_url() {
                window.open(get_file_station_url);
        }     

        function to_test_submit() {
            $("#submit_button").val("qcloud_test_portal");
            $("#action").val("Apply");
            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#qcloud_form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 50000);

        }

        function to_diagnostic_submit() {
            $("#submit_button").val("qcloud_diagnostic");
            $("#action").val("Apply");
            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#qcloud_form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 50000);

        }

        function disp_confirm(){
       return confirm("<%lang("After signing out the myQNAPcloud account, you will not be able to keep this device name anymore and the following link will become invalid:");%>" + "\n\n" + user_qcloud_site + "\n\n" + "<%lang("Are you sure you want to sign out the myQNAPcloud account?");%>");
        }

        function to_submit() {
            if (qid_status == sign_in) {
                if(disp_confirm() == false)	
                    return;
            } 
            
            
            SaveEXP();
        } //--------to_submit

        function SaveEXP(){
            localStorage.setItem("submit_qcloud_qid", $('#qcloud_qid').val());
            localStorage.setItem("submit_qcloud_pw", $('#qcloud_pw').val());
            localStorage.setItem("submit_qcloud_device_name",$('#qcloud_device_name').val());
            if (qid_status == sign_in) { //signin
                $("#submit_button").val("btn-to-signout");//to signin out
            } 
            if (qid_status == sign_out) {
                if(!qcloud_valid_check()) return;
                var get_device_name_val = $("#qcloud_device_name").val();
                if(!valid_device_mail_char($("#qcloud_qid").val())) return;
                if(!valid_device_password_char($("#qcloud_pw").val())) return;
                if(!valid_device_name_char(get_device_name_val)) return;
                $("#submit_button").val("btn-to-signin");
            }
            $("#action").val("Apply");
            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#qcloud_form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 50000);
    }
        
    </script>

	</head>

	<body>
	  <%do_bootstrap_navbar_menu();%>
        <script type="text/javascript">create_waiting_window();</script>
        <form class="form-horizontal" name="qcloud_form" id="qcloud_form" action="apply.cgi" method="post">
		<div class="main-container container-fluid" id="main-container">
            <a id="menu-toggler" class="menu-toggler" href="#"><span class="menu-text"></span></a>
			<div id="sidebar" class="sidebar">
			    <%do_bootstrap_menu();%>
                <div id="sidebar-collapse" class="sidebar-collapse"><i class="icon-double-angle-left" data-icon1="icon-double-angle-left" data-icon2="icon-double-angle-right" ></i></div>
			</div><!--/#sidebar-->
		
			<div id="main-content" class="main-content">
					
					<div id="breadcrumbs" class="breadcrumbs">
						<ul class="breadcrumb">
							<li><i class="icon-home"></i> Home<span class="divider"><i class="icon-angle-right"></i></span></li>
							<li class="active"><%lang("myQNAPcloud");%></li>
						</ul><!--.breadcrumb-->


					</div><!--breadcrumbs-->



					<div id="page-content" class="page-content">
						
						<div class="page-header position-relative">
							<h1><%lang("myQNAPcloud");%> <small><i class="icon-double-angle-right"></i> </small></h1>
							 <script type="text/javascript">do_hidden_elements('exp_myqnapcloud_rule');</script>
						</div><!--/page-header-->
						

						<div class="row-fluid">
        <!-- PAGE CONTENT BEGINS HERE -->

        <!--row1-->

        <div class="row-fluid">
            <div class="span4">
                <div class="widget-box">
                    <div class="widget-header">
                        <h4><i class='icon-myqnapcloud'></i>&nbsp;<%lang("myQNAPcloud");%></h4>
                    </div>
                    

                    <div class="widget-body" >
                            <div id="dialog-confirm" style="display:none">
                            </div>
                        <div id="qcloud-signout-page" class="widget-main" >
                            <div class="row-fluid">
                                <label for="qcloud_qid_signed"><%lang("myQNAPcloud ID");%> (<%lang("QID");%>)</label>
                                <input type="text" name="qcloud_qid_signed" id="qcloud_qid_signed" style="width: 280px;" readonly />
                            </div>
                            <div class="row-fluid">
                                <label for="qcloud_info"><% lang("myQNAPcloud Device Name"); %></label>
                                <input type="text" name="qcloud_info" id="qcloud_info" style="width:280px"readonly />
                            </div>
                            <div class="row-fluid">
                            <br>
                                <input type="button" name="qcloud_portal" id="qcloud_portal" class="btn btn-primary btn-small" onClick="to_test_submit();" value="<%lang("UPNP Test");%>" />
                                <input type="button" name="qcloud_diagnostic" id="qcloud_diagnostic" class="btn btn-primary btn-small" onClick="to_diagnostic_submit();" value="<%lang("Diagnostics");%>" />
                                &nbsp; &nbsp;
                                <br>
                                <a class="approx" id="qcloud_fetch_result" style="display:none"> </a>
                                <i id="test_status_icon" class='icon-ok-sign' style="display:none"></i>
                                <a class="approx" id="qcloud_fetch_status_result" style="display:none"> </a>
                                <br>       
                                <!--sign status-->
                                <a class="approx" id="qcloud_sign_status"> </a>
                                <a class="approx" id="qcloud_sign_status_result"> </a>
                                <br>
                                <!--last update status-->
                                <a class="approx" id="qcloud_last_update_status" > </a>
                                <i id="last_update_status_icon" class='icon-ok-sign'></i>
                                <a class="approx" id="qcloud_last_update_ret"> </a>
                                <a class="approx" id="qcloud_last_update_ret_code"> </a>
                                <br>
                                <br>
                                <!-- fetch mgmt outport -->
                                <a class="approx" id="qcloud_get_web_mgmt_outport"></a>
                                <br>
                                <a class="approx" id="show_qcloud_get_web_mgmt_outport" onclick="go_web_mgmt_url()"></a>
                                <i id="qcloud_get_web_mgmt_outport_icon" class='icon-ok-sign'></i>
                                <a class="approx" id="qcloud_get_web_mgmt_outport_ret"> </a>
                                <br>
                                <br>
                                <a class="approx" id="qcloud_get_file_station_outport"> </a>
                                <br>
                                <a class="approx" id="show_qcloud_get_file_station_outport" onclick="go_file_station_url()"> </a>
                                <i id="qcloud_get_file_station_outport_icon" class='icon-ok-sign'></i>
                                <a class="approx" id="qcloud_get_file_station_outport_ret"> </a>
                            </div>
                        </div><!--qcloud -signout-->
                        <div id="qcloud-signin-page" class="widget-main" style="" >
                            <div class="row-fluid">
                                <label for="qcloud_qid"><% lang("myQNAPcloud ID"); %>  <% lang("or"); %> 
                                <a href="javascript:to_signin_up_href()"><%lang("sign up now!");%></a>
                                </label>
                                <input type="text" name="qcloud_qid" id="qcloud_qid" placeholder="Email" style="width:280px;" />
                            </div>
                            <div class="row-fluid">
                                <label for="qcloud _pw"><% lang("Password"); %></label>
                                <input type="password" name="qcloud_pw" id="qcloud_pw" placeholder="Password" style="width:280px;"/>
                            </div>

                            <div class="row-fluid">
                                <label for="qcloud_device_name"><% lang("myQNAPcloud Device Name"); %></label>
                                <input type="text" name="qcloud_device_name" id="qcloud_device_name"   maxlength="30" style="width: 280px;" />
                                <a id="qcloud_device_name_href"> </a>
                            </div>
                            <br>
                        </div> <!--qcloud -signin-->
                    </div><!--widget-body-->
                </div><!--widget-box-->
            </div><!--/span-->

        </div>

        <!--row2-->
        <div class="row-fluid">
            <div class="span12">
                <div class="submitFooter">
                    <br>
                    <br>
                    <input type="button" id="sign-in-btn" class="btn btn-primary" onClick="to_submit()" value=" <%lang("Sign In"); %> ">
                    <input type="button" id="manage-qcloud-account"  class="btn btn-primary" onclick="redirect_myqnapcloud_service()" value="<%lang("Manage myQNAPcloud Account");%>" style="display:none">

                 </div>
            </div>
            <div class="span12">
                <br><br><script type="text/javascript"><%show_copyright();%></script>
            </div>
        </div><!-- row2 -->

<!-- PAGE CONTENT ENDS HERE -->
						 </div><!--/row-fluid-->
	
					</div><!--/#page-content-->
					  

					<!--<div id="ace-settings-container">
						<div class="btn btn-app btn-mini btn-warning" id="ace-settings-btn">
							<i class="icon-cog"></i>
						</div>
						<div id="ace-settings-box">
							<div>
								<div class="pull-left">
									<select id="skin-colorpicker" class="hidden">
                                        <option data-class="default" value="#F39770">#F39770</option>
                                        <option data-class="skin-1" value="#438EB9">#438EB9</option>
                                        <option data-class="skin-2" value="#C6487E">#C6487E</option>
									</select>
								</div>
								<span>&nbsp; Choose Skin</span>
							</div>
							<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-header" /><label class="lbl" for="ace-settings-header"> Fixed Header</label></div>
							<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-sidebar" /><label class="lbl" for="ace-settings-sidebar"> Fixed Sidebar</label></div>
						</div>
					</div>/#ace-settings-container-->


			</div><!-- #main-content -->


		</div><!--/.fluid-container#main-container-->




		<a href="#" id="btn-scroll-up" class="btn btn-small btn-inverse">
			<i class="icon-double-angle-up icon-only"></i>
		</a>

    </form>
	                    


		

	</body>
</html>
