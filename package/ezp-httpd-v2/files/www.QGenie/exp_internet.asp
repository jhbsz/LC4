
<!DOCTYPE html>
<html lang="en">
       <head>
		<meta charset="utf-8" />
		<title><% nv_get("model"); %> - Internet</title>
		
		<%do_headmeta();%>
		<%do_basic_css();%>

		<%do_ace_font_css();%>
		<%do_ace_font_ie7_css();%>

		<!-- page specific plugin styles -->

		<!-- ace styles -->
		<%do_ace_basic_css();%>
		<%do_ace_basic_ie9_css();%>
		<%do_custom_css();%>

        <link href='css/fontcustom.css' rel='stylesheet' type='text/css'>
        <%do_custom_js();%>
        <%do_basic_js();%>
        <%do_bootstrap_modal();%>

		<!-- page specific plugin scripts -->
		
		<!--[if lt IE 9]>
		<script type="text/javascript" src="js/excanvas.min.js"></script>
		<![endif]-->

                <%do_jquery_ui_js();%>
		<%do_datatables_js();%>
		<%do_ace_basic_js();%>

    <script type="text/javascript">
     var url_info = "exp_internet";
    function current_connection_val(index){
        var cn_proto = '<%nvg_get("wan0_proto");%>';
        var ether_connection = 0;
        var wisp_connection = 0;
        var usb_connection = 0;

        if(cn_proto == "dhcp" || cn_proto == "static" || cn_proto == "pppoe") {
            ether_connection = 1 ;
        } else if(cn_proto == "iphone" || cn_proto == "htc" || cn_proto =="wwan") {
            usb_connection = 1;
        } else {
            wisp_connection = 1 ;
        }
            
        if (index == "Ethernet"){
           return ether_connection;
        } else if (index == "USB"){
            return usb_connection;
        } else {
            return wisp_connection;
        }
    }
  
    function current_proto_val(index){
        var ether_proto = '<% nvg_attr_get("","cnnt_rule", 0, "proto");%>';
        var wisp_proto = '<% nvg_attr_get("","cnnt_rule", 1, "proto");%>';
        var usb_proto = '<% nvg_attr_get("","cnnt_rule", 2, "proto");%>';
            
        if (index == "Ethernet"){
           return ether_proto;
        } else if (index == "USB"){
            return usb_proto;
        } else {
            return wisp_proto;
        }
    }
  var cn_ether_connection = current_connection_val("EThernet");
  var cn_wisp_connection =  current_connection_val("WISP");
  var cn_usb_connection = current_connection_val("USB");
  var cn_ether_proto = current_proto_val("EThernet");
  var cn_wisp_proto =  current_proto_val("WISP");
  var cn_usb_proto = current_proto_val("USB");

  var cnnt_rule =[["1","Ethernet","0",cn_ether_connection,cn_ether_proto],["1","WISP","0",cn_wisp_connection,cn_wisp_proto],["1","USB","0",cn_usb_connection,cn_usb_proto]];
   
    <%widget_start();%>
    var ajax_ssid = "";
    var ajax_wan_status = 0;
    var ajax_ether_plug = 0;
    var ajax_usb_plug = 0;
    var ajax_ether_connect = 0;
    var ajax_wisp_connect = 0;
    var ajax_usb_connect = 0;
    function widget_postaction(data) {
        result = data;
        var ret = result.split('\n');
        var USB_usage, SD_usage, Qsync_usage;
        for (i = 0; i < ret.length; i++) {
            var pair = ret[i].split('=');
            if (pair[0] == "Battery") {
                var battery = pair[1] + "%";
                if(pair[1] < 20) {
                    $("#navbar_battery").attr("style", "display:inline;background-color: rgb(255,255,255);");
                    $("#nav_battery").html(battery);
                } else if(pair[1] >= 20){
                    $("#navbar_battery").attr("style", "display:none;");
                }
            } else if (pair[0] == "USB_usage") {
                USB_usage = pair[1];
                if(USB_usage == "N/A") {
                    USB_usage = 0;
                }
            } else if (pair[0] == "SD_usage") {
                SD_usage = pair[1];
                if(SD_usage == "N/A") {
                    SD_usage = 0;
                }
            } else if (pair[0] == "Qsync_usage") {
                Qsync_usage = pair[1];
                $("#qsync_usage").attr("data-percent", Qsync_usage);
                if(Qsync_usage == "N/A") {
                    Qsync_usage = -1;
                }
            } else if (pair[0] == "wan0_ipaddr") {
                ajax_wan0_ipaddr = pair[1];
            } else if (pair[0] == "ssid") {
                ajax_ssid = pair[1];
            } else if (pair[0] == "wan0_widget_proto") {
                wan0_proto_type = pair[1];
            } else if (pair[0] == "wan0_status") {
                ajax_wan_status = pair[1];
            } else if (pair[0] == "ether_plug") {
                ajax_ether_plug = pair[1];
            } else if (pair[0] == "usb_plug") {
                ajax_usb_plug = pair[1];
            } else if (pair[0] == "ether_connect") {
                ajax_ether_connect = pair[1];
            } else if (pair[0] == "wisp_connect") {
                ajax_wisp_connect = pair[1];
            } else if (pair[0] == "usb_connect") {
                ajax_usb_connect = pair[1];
            } else if (pair[0] == "connection_type") {
                    plug_state();
                //var swap_proto = pair[1];
                //if(swap_proto != null)   {
                 //   plug_state();
               // }
            }

        }
        if(USB_usage > 95 || SD_usage > 95 || (Qsync_usage > 95)) {
            var storage;
            if(Qsync_usage >= USB_usage &&  Qsync_usage >= SD_usage) {
                storage = Qsync_usage + "%";
            } else if (SD_usage >= USB_usage && SD_usage >= Qsync_usage) {
                storage = SD_usage + "%";
            } else if (USB_usage >= SD_usage && USB_usage >= Qsync_usage) {
                storage = USB_usage + "%";
            } 

            $("#navbar_storage").attr("style", "display:inline;background-color: rgb(255,255,255);");
            $("#nav_storage").html(storage);
        } else if((USB_usage <= 95) && (SD_usage <= 95) && (Qsync_usage <= 95)){
            $("#navbar_storage").attr("style", "display:none;");
        }

        if(USB_usage > 0 || SD_usage > 0 || Qsync_usage >= 0) {
            $('#4').attr("style","display:inline;");
        } else if((USB_usage == 0) && (SD_usage == 0) && (Qsync_usage == -1)){
            $('#4').attr("style","display:none;");
        }

        //setTimeout('widget_query(F,url_info);', 5000);
        setTimeout('widget_query(F);', 5000);
    }

          var enum_cnnt_basic = {
            _EXPAND:0,
            _NAME:1,
            _ENABLE:2,
            _CONNECTION:3,
            _PROTO:4,

          }                                 

function plug_state() {
    var ether_connect_enable = btn_connect_enable(ajax_ether_connect, 'Ethernet');
    var wisp_connect_enable = btn_connect_enable(ajax_wisp_connect, 'WISP');
    var usb_connect_enable = btn_connect_enable(ajax_usb_connect, 'USB');

    var ether_connect_state = ajax_connect_status('Ethernet', ajax_ether_connect);
    var ether_plug = ajax_plug('Ethernet');
    $("#Ethernet").parents('td').next('td').html(ether_connect_state + ether_plug);
    $("#Ethernet").parents('td').next('td').next('td').html(ether_connect_enable);

    var wisp_connect_state = ajax_connect_status('WISP', ajax_wisp_connect);
    var wisp_plug = ajax_plug('WISP');
    $("#WISP").parents('td').next('td').html(wisp_connect_state + wisp_plug);
    $("#WISP").parents('td').next('td').next('td').html(wisp_connect_enable);

    var usb_connect_state = ajax_connect_status('USB',ajax_usb_plug);  
    var usb_plug = ajax_plug('USB');
    $("#USB").parents('td').next('td').html(usb_connect_state + usb_plug);
    $("#USB").parents('td').next('td').next('td').html(usb_connect_enable);
}

        function init() {
	    }    

        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = cnnt_rule.length ;
            $("<input>").attr({type: "hidden", id: "cnnt_rule", name: "cnnt_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
            	$("<input>").attr({type: "hidden", id: P + i + '_expand', name: P + i + '_expand', value: cnnt_rule[i][enum_cnnt_basic._EXPAND]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: cnnt_rule[i][enum_cnnt_basic._NAME]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable' , value: cnnt_rule[i][enum_cnnt_basic._ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_connection', name: P + i + '_connection', value: cnnt_rule[i][enum_cnnt_basic._CONNECTION] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_proto', name: P + i + '_proto', value: cnnt_rule[i][enum_cnnt_basic._PROTO] }).appendTo("form");

            }
        }

        function check_click(name, proto) {
            var connect;
            var proto_connect;
            if(name == "Ethernet") {
                connect = current_connection_val("EThernet");
                proto_connect = '<% nvg_attr_get("","cnnt_rule", 0, "proto");%>';
            }else if(name == "WISP") {
                //connect = '<% nvg_attr_get("","cnnt_rule", 1, "connection");%>';
                connect = current_connection_val("WISP");
                proto_connect = '<% nvg_attr_get("","cnnt_rule", 1, "proto");%>';
            }else if(name == "USB") {
                //connect = '<% nvg_attr_get("","cnnt_rule", 2, "connection");%>';
                connect = current_connection_val("USB");
                proto_connect = '<% nvg_attr_get("","cnnt_rule", 2, "proto");%>';
            }
            if(connect == "1") {
                if(proto_connect != proto) {
                    var str = "cnnt_press(\""+name+"\")";
                    $("#"+name+"_connect").text("<%lang("Connect");%>");
                    $("#"+name+"_connect").attr("onClick", str);
                    $("#"+name+"_connect").attr("class", "btn btn-primary btn-small");
                }
                else {
                    var str = "discnnt_press(\""+name+"\")";
                    $("#"+name+"_connect").text("<%lang("Disconnect");%>");
                    $("#"+name+"_connect").attr("onClick", str);
                    $("#"+name+"_connect").attr("class", "btn btn-success btn-small");
                }
            }

        }
        function Connection(index,action) {
            F = document.getElementsByName('wisp_form')[0];
            F.submit_button.value = action;
            F.action.value = "Apply";
            F.wisp_swap_rule.value = "";
            $('#SiteSurveyListData-modal').modal('hide');

            open_wisp_window();
            var ajaxData;
            ajaxData = ajax_data( $("#wisp_form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 10000);
        }
        function to_submit() {
            F = document.getElementsByName('form')[0];
            F.submit_button.value = "cnnt";
            F.action.value = "Apply";
            local_pack_key_value_pair(F, "cnnt_table", "cnnt_rule");

            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 5000);
        }

        function input_name(name) {
            if(name == "Ethernet") {                
                return "<h4 name="+name+" id="+name+"><%lang("Wired Internet");%>  <i class='icon-code-fork'></i></h4>";                
            }else if(name == "WISP") {
               return "<h4 name="+name+" id="+name+"><%lang("Wireless Internet");%>  <i class='icon-rss'></i></h4>";                
            }else if(name == "USB") {
                return "<h4 name="+name+" id="+name+"><%lang("USB Internet");%>  <i class='icon-mobile-phone'></i></h4>";
            }
        }
        function ajax_plug(name) {
            if(name == "Ethernet") {
                //ether_plug = '<%nvg_attr_get("","plug_rule", "0", "ethernet");%>';
                if(ajax_ether_plug == "0") {
                    return '<div class="badge badge-fail"><%lang("Unplugged");%></div>';
                }else {
                    return '<div class="badge badge-success"><%lang("Plugged");%></div>';
                }
            }else if(name == "WISP") {
                if(ajax_wisp_connect == "1") {
                    return '<div class="badge badge-success">' + ajax_ssid +'</div>';
                }
                else {
                    return "";
                }
            }else if(name == "USB") {
                //ajax_usb_plug = '<%nvg_attr_get("","plug_rule", "0", "usb");%>';
                if(ajax_usb_plug == "0") {
                    return '<div class="badge badge-fail"><%lang("Unplugged");%></div>';
                }else {
                    return '<div class="badge badge-success"><%lang("Plugged");%></div>';
                }
            }
        }
        function plug(name) {
            if(name == "Ethernet") {
                ether_plug = '<%nvg_attr_get("","plug_rule", "0", "ethernet");%>';
                if(ether_plug == "0") {
                    return '<div class="badge badge-fail"><%lang("Unplugged");%></div>';
                }else {
                    return '<div class="badge badge-success"><%lang("Plugged");%></div>';
                }
            }else if(name == "WISP") {
                //connect = '<% nvg_attr_get("","cnnt_rule", 1, "connection");%>';
                connect = current_connection_val("WISP");
                if(connect == "1") {
                    ssid = '<% nvg_attr_get("","wl0_apcli_rule", "0", "ssid");%>';
                    return '<div class="badge badge-success">'+ssid+'</div>';
                }
                else {
                    return "";
                }
            }else if(name == "USB") {
                usb_plug = '<%nvg_attr_get("","plug_rule", "0", "usb");%>';
                if(usb_plug == "0") {
                    return '<div class="badge badge-fail"><%lang("Unplugged");%></div>';
                }else {
                    return '<div class="badge badge-success"><%lang("Plugged");%></div>';
                }
            }
        }

        function setLanguage(lang){
        /*  setCookie("language", lang);*/
            var data_array = {};
            data_array['action'] = 'lang';
            data_array['lang'] = lang;
            data_array['sub_button'] = 'exp_internet';
            do_ajax_action ("apply.cgi", "POST", "text", true, data_array, 3000);
        }


        function connect_status(name, connection) {
            if(connection == "1") {
                wan_status = '<% nvg_attr_get("","wan_status_rule", "0", "state");%>';
                if(name == "Ethernet" && '<%nvg_attr_get("","plug_rule", "0", "ethernet");%>' == "1") {
                    wan0_proto = '<%nvg_get("wan0_proto");%>';
                    if(wan_status == "0") {
                        return '<span class="badge badge-important"><%lang("Disconnected");%></span>';
                    }else if(wan_status == "4" || wan0_proto == "static") {
		        return '<span class="badge badge-success"><%lang("Connected");%>: <%nvg_get("wan0_ipaddr");%></span>';
                    }else {
                        return '<span class="badge badge-yellow"><%lang("Connecting");%></span>';
                    }
                } else if(name == "WISP") {
                    ssid = '<% nvg_attr_get("","wl0_apcli_rule", "0", "ssid");%>';
                    wisp_plug = '<%nvg_attr_get("","plug_rule", "0", "wisp");%>';
                    if(ssid != "") {
                        if(wan_status == "4") {
                            return '<span class="badge badge-success"><%lang("Connected");%>: <%nvg_get("wan0_ipaddr");%></span>';
                        /*
                        }else if(wisp_plug == "1") {
                            return '<span class="badge badge-important"><%lang("Error password");%></span>';
                        */
                        }else if(wan_status == "0") {
                            return '<span class="badge badge-important"><%lang("Disconnected");%></span>';
                        }else {
                            return '<span class="badge badge-yellow"><%lang("Connecting");%></span>';
                        }
                    } else{
                        return "";
                    }
                } else if(name == "USB" && '<%nvg_attr_get("","plug_rule", "0", "usb");%>' == "1") {
                    if(wan_status == "0") {
                        return '<span class="badge badge-important"><%lang("Disconnected");%></span>';
                    }else if(wan_status == "4") {
                        return '<span class="badge badge-success"><%lang("Connected");%>: <%nvg_get("wan0_ipaddr");%></span>';
                    }else {
                        return '<span class="badge badge-yellow"><%lang("Connecting");%></span>';
                    }
                } else{
                    return "";
                }
            }
            else {
                return "";
            } 
        }
        function ajax_connect_status(name, connection) {
            if(connection == "1") {
                //wan_status = '<% nvg_attr_get("","wan_status_rule", "0", "state");%>';
                if(name == "Ethernet" && ajax_ether_plug == "1" && ajax_ether_connect == "1") {
                    if(ajax_wan_status == "0") {
                        return '<span class="badge badge-important"><%lang("Disconnected");%></span>';
                    }else if(ajax_wan_status == "4") {
                  		return '<span class="badge badge-success">'+'<%lang("Connected");%>'+': '+ajax_wan0_ipaddr+'</span>';
                    }else {
                        return '<span class="badge badge-yellow"><%lang("Connecting");%></span>';
                    }
                } else if(name == "WISP" && ajax_wisp_connect == "1") {
                    ssid = ajax_ssid;
                    if(ssid != "") {
                        if(ajax_wan_status == "4") {
                            return '<span class="badge badge-success">'+'<%lang("Connected");%>'+': '+ajax_wan0_ipaddr+'</span>';
                        }else if(ajax_wan_status == "0") {
                            return '<span class="badge badge-important"><%lang("Disconnected");%></span>';
                        }else {
                            return '<span class="badge badge-yellow"><%lang("Connecting");%></span>';
                        }
                    } else{
                        return "";
                    }
                } else if(name == "USB" && ajax_usb_plug == "1" && ajax_usb_connect == "1") {
                    if(ajax_wan_status == "0") {
                        return '<span class="badge badge-important"><%lang("Disconnected");%></span>';
                    }else if(ajax_wan_status == "4") {
                        return '<span class="badge badge-success">'+'<%lang("Connected");%>'+': '+ajax_wan0_ipaddr+'</span>';
                    }else {
                        return '<span class="badge badge-yellow"><%lang("Connecting");%></span>';
                    }
                } else{
                    return "";
                }
            }
            else {
                return "";
            } 
        }

        function btn_connect_enable(v, name) {
            if(name == "WISP") {
                return "<span class='btn btn-primary btn-small' name='"+name+"_connect' id='"+name+"_connect' onclick='btn_sitesurvey_click()'><%lang("Search");%></span>";
            }
            if (v == 1) {
                return "<span class='btn btn-success btn-small' name='"+name+"_connect' id='"+name+"_connect' onClick='discnnt_press(\""+name+"\")'><%lang("Disconnect");%></span>";
            } else {
                return "<span class='btn btn-primary btn-small' name='"+name+"_connect' id='"+name+"_connect' onClick='cnnt_press(\""+name+"\")'><%lang("Connect");%></span>";	
            }
        }

        function text_hidden_proto(v) {
            return "<input type='hidden' name='choose_proto' id='choose_proto'></input>";
        }

        var selectID=null;
        var DataTable;
        var SelectItem;
        function DrawDataTables(LV_Data){
            DataTable = $('#CnntListData').dataTable( {
                "sDom": "",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,                
                "aoColumns": [
                    { "sWidth": "55%","sTitle": "", "sClass": "left"},
                    { "sWidth": "30%","sTitle": "", "sClass": "right"},           
                    { "sWidth": "15%","sTitle": "", "sClass": "center"},
                    { "sWidth": "0%", "sTitle": "", "sClass": "center"}                    
                ],  
		        "bAutoWidth": false,
                "bSort": false,
                "bPaginate": false,
                "bLengthChange": false,
                "bFilter": false,
                "bInfo": false,
                "fnRowCallback": function( nRow, aData, iDisplayIndex ) {
                   if (iDisplayIndex == selectID){
                       $(nRow).addClass("row_selected");
                       return nRow;
                   }
                },
            } );   

            $("#CnntListData tbody tr").click( function() {  // selete a row data
                if ( $(this).hasClass('row_selected') ) {
                    $(this).removeClass('row_selected');
                    selectID=null;
                }else {
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                    $(this).addClass('row_selected');
                    selectID = $(this).index();
                }
            });

        }
        /* Formating function for row details */

        function fnFormatDetailsEther ( nTr )
        {    
            var aData = DataTable.fnGetData( nTr );
            var sOut = '<div class="control-group"><label class="control-label"></label><div class="controls">';
            sOut += '<label><input type="radio" value="1" name="ether_enable" id="ether_name" onclick="pop_pppoe()" <%nvg_attr_match("ether_enable", "cnnt_rule", "0", "proto", "pppoe", "checked"); %>/><span class="lbl">&nbsp;&nbsp;<%lang("PPPoE");%> : <%lang("Select this if you have a username and password.");%></span></label><br>';
            sOut += '<label><input type="radio" value="0" name="ether_enable" id="ether_name" onClick="pop_static()" <%nvg_attr_match("ether_enable", "cnnt_rule", "0", "proto", "static", "checked"); %>/><span class="lbl">&nbsp;&nbsp;<%lang("Static IP");%> : <%lang("Select this if you know the IP address.");%></span></label><br>';   
            sOut += '<label><input type="radio" value="2" name="ether_enable" id="ether_name" onclick="GetRadioValue(this.name)" <%nvg_attr_match("ether_enable", "cnnt_rule", "0", "proto", "dhcp", "checked"); %>/><span class="lbl">&nbsp;&nbsp;<%lang("DHCP");%> : <%lang("Select this if you have none of the above.");%> </span></label>';          
            sOut += '</div></div>';
            return sOut;
        }	
        function fnFormatDetailsWISP ( nTr )
        {
            var sOut = '<input type="button" class="btn" name="btn_sitesurvey" id="btn_sitesurvey" onclick="btn_sitesurvey_click()"  value="<%lang("Survey");%>">';
            return sOut;
        }	

        function fnFormatDetailsUSB ( nTr )
        {

        var aData = DataTable.fnGetData( nTr );
        var sOut = '<div class="control-group"><label class="control-label"></label><div class="controls">';
        sOut = '<label><input type="radio" value="2" name="USB_enable" onclick="GetRadioValue(this.name)" <%nvg_attr_match("USB_enable", "cnnt_rule", "2", "proto", "iphone", "checked"); %> /><span class="lbl">&nbsp;&nbsp;<%lang("iPhone");%> : <%lang("Select this to tether via iPhone.");%></span></label><br>';
        sOut += '<label ><input type="radio" value="1" name="USB_enable" onclick="GetRadioValue(this.name)" <%nvg_attr_match("USB_enable", "cnnt_rule", "2", "proto", "htc", "checked"); %> /><span class="lbl">&nbsp;&nbsp;<%lang("Android phone");%> : <%lang("Select this to tether via Android.");%></span></label><br>';
        sOut += '<label ><input type="radio" value="0" name="USB_enable" onclick="dongle_modal_trigger()" <%nvg_attr_match("USB_enable", "cnnt_rule", "2", "proto", "wwan", "checked"); %> /><span class="lbl">&nbsp;&nbsp;<%lang("3G/4G Mobile Internet");%> : <%lang("Select this to tether via a 3G/4G dongle.");%></span></label>';   
        sOut += '</div></div>';
            return sOut;
        }	

        function GetRadioValue(radio_name) {
            var USB_enable_doc = document.getElementsByName('USB_enable');        
            var ether_enable_doc = document.getElementsByName('ether_enable');   
            for (var i=0; i<cnnt_rule.length; i++ )  {
                if(cnnt_rule[i][enum_cnnt_basic._NAME] == "USB" && radio_name == "USB_enable") {
                    var proto = '<% nvg_attr_get("","cnnt_rule", "2", "proto");%>';
                    if(USB_enable_doc[0].checked) {/* iphone */
                        cnnt_rule[i][enum_cnnt_basic._PROTO]="iphone";
                        check_click("USB", "iphone");
                    } else if(USB_enable_doc[1].checked) {/* android */
                        cnnt_rule[i][enum_cnnt_basic._PROTO]="htc";
                        check_click("USB", "htc");
                    }
                } else if( cnnt_rule[i][enum_cnnt_basic._NAME] == "Ethernet" && radio_name == "ether_enable") {
                    var proto = '<% nvg_attr_get("","cnnt_rule", "0", "proto");%>';
                    if(ether_enable_doc[2].checked) {/* DHCP */
                        cnnt_rule[i][enum_cnnt_basic._PROTO]="dhcp";
                        check_click("Ethernet", "dhcp");
                    }
                }
            }
        }
    
        function GetGridData(){
            var connect_rows = [];
            var connect_data = {};
            var wisp_info = "";
            for (var i=0; i<cnnt_rule.length; i++ )  {
                connect_data = {};
                
                connect_data[0] = input_name(cnnt_rule[i][enum_cnnt_basic._NAME]);
                connect_data[1] = connect_status(cnnt_rule[i][enum_cnnt_basic._NAME], cnnt_rule[i][enum_cnnt_basic._CONNECTION]);         
                connect_data[1] += plug(cnnt_rule[i][enum_cnnt_basic._NAME]);
                connect_data[2] = btn_connect_enable(cnnt_rule[i][enum_cnnt_basic._CONNECTION], cnnt_rule[i][enum_cnnt_basic._NAME]);
                connect_data[3] = text_hidden_proto(cnnt_rule[i][enum_cnnt_basic._PROTO]);

                connect_rows[i] = connect_data;
            }
            return connect_rows;
        }
    

        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }


        function local_generate_rule(ID) {
            var rule_tmp = Array();
            rule_tmp[enum_cnnt_basic._EXPAND]           = cnnt_rule[ID][enum_cnnt_basic._EXPAND];
            rule_tmp[enum_cnnt_basic._NAME]           = cnnt_rule[ID][enum_cnnt_basic._NAME];
            rule_tmp[enum_cnnt_basic._ENABLE]         = cnnt_rule[ID][enum_cnnt_basic._ENABLE];
            rule_tmp[enum_cnnt_basic._CONNECTION]         = cnnt_rule[ID][enum_cnnt_basic._CONNECTION];
            rule_tmp[enum_cnnt_basic._PROTO]         = cnnt_rule[ID][enum_cnnt_basic._PROTO];
  
            return rule_tmp;
        }

        var F;
        function discnnt_press(name) {
            for (var i=0; i<cnnt_rule.length; i++ )  {
                if(cnnt_rule[i][enum_cnnt_basic._NAME] == name )
                {
                    if(cnnt_rule[i][enum_cnnt_basic._CONNECTION] == 1)
                    {
                        cnnt_rule[i][enum_cnnt_basic._CONNECTION]=0;
                    }
                }
                else
                {
                    cnnt_rule[i][enum_cnnt_basic._CONNECTION]=0;
                }
            }
            var connect_rows = GetGridData();
            DataTable.fnDestroy();
            DrawDataTables(connect_rows);
            $('#SiteSurveyListData-modal').modal('hide');
            to_submit();
        }     
        function cnnt_press(name) {
            
            for (var i=0; i<cnnt_rule.length; i++ )  {
                if(cnnt_rule[i][enum_cnnt_basic._NAME] == name )
                {
                    if(cnnt_rule[i][enum_cnnt_basic._CONNECTION] == 0)
                    {
                        cnnt_rule[i][enum_cnnt_basic._CONNECTION]=1;
                    }
                }
                else
                {
                    cnnt_rule[i][enum_cnnt_basic._CONNECTION]=0;
                }

            }
            if(name == "WISP") {
               btn_sitesurvey_click(); 
            }else {
                var connect_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(connect_rows);
                $('#SiteSurveyListData-modal').modal('hide');
                
                to_submit();
            }
        }                        
        function CreateWISPSubmitForm(IndexID,LV_Data) {		
            if (IndexID ==null) {
                alert("Please, select row");
                return false;
            }
            
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {                    
          
                $("#wl"+i+"_apcli"+i+"_ssid").val(LV_Data[IndexID][2]);
                $("#wl"+i+"_apcli"+i+"_bssid").val(LV_Data[IndexID][3]);                  
                $("#wl"+i+"_channel").val(LV_Data[IndexID][1]);
                if (LV_Data[IndexID][6] == "ABOVE") {
                    $("#wl"+i+"_extcha").val('1');
                } else {
                    $("#wl"+i+"_extcha").val('0');
                }
                var sec = LV_Data[IndexID][4];
        						
                    if(sec == 'NONE') {
                        $("#wl"+i+"_apcli"+i+"_secmode").val('disabled');        
    
                    } else if(sec == 'WEP') {
                        CreateSelectedWEPCryptDataForm();
                        $("#wl"+i+"_apcli"+i+"_secmode").val('wep');   
               
        			
                    } else if(sec.match('WPA2')) {   
                        
                        CreateSelectedPSK2CryptDataForm();           			    
                        getWISPCrypto(sec,i);
                        $("#wl"+i+"_apcli"+i+"_secmode").val('psk2');
    
    
                    } else if(sec.match('WPA')) {     
                        CreateSelectedPSKCryptDataForm();                
                        getWISPCrypto(sec,i);
                        $("#wl"+i+"_apcli"+i+"_secmode").val('psk');
    
                    }
            }
        }
        var SSDataTable;
        function SiteSurveyDrawDataTables(LV_Data){
            SSDataTable = $('#SiteSurveyListData').dataTable( {
                "sDom": "",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sWidth": "8%", "sTitle": "", "sClass":"center" },
                    { "sTitle": "<%lang("Channel");%>", "sClass":"center" ,"bVisible": false},
                    { "sWidth": "22%", "sTitle": "<%lang("SSID");%>" ,"sClass":"center"},
                    { "sTitle": "<%lang("BSSID");%>", "sClass":"center","bVisible": false },
                    { "sTitle": "<%lang("Security Mode");%>", "sClass":"center","bVisible": false },
                    { "sTitle": "<%lang("Signal & Security");%>","bVisible": false  },
                    { "sTitle": "<%lang("ExtCH");%>", "sClass":"center","bVisible": false },
                    { "sTitle": "<%lang("Wireless Mode");%>","bVisible": false },
                    { "sTitle": "<%lang("Security Mode");%>", "sClass":"center","bVisible": false },
                    { "sWidth": "15%", "sTitle": "<%lang("Signal");%>", "sClass":"center"},
                    { "sWidth": "40%", "sTitle": "<%lang("Action");%>" ,"sClass":"center"},
                ],
                "bSort": false,
                "bPaginate": false,
                "bLengthChange": false,
                "bFilter": false,
                "bRetrieve": true,
                "bInfo": false
            } );   
            

            $("#SiteSurveyListData tbody td input").click( function () {
                var nTr = $(this).parents('tr')[0];
                var nTr_table = $(this).parent().parent();
                $(nTr_table).addClass('row_selected');
                var IndexID=$(nTr_table).index();
                
                if ($(this).attr("id") == "wl0_Forget" ) {
                    CreateSelectedHiddenWISPDataForm();
                    CreateWISPSubmitForm(IndexID,LV_Data); 
                    Connection(IndexID+1,"forget");
                     
                }else{
                    var bssid;
                    bssid = LV_Data[IndexID][3];
                    $("#SelectedWISPDataForm").empty();
                
                    if(CheckSavedWispData(bssid) && bssid==CurrentBSSID){
                    
                        CreateSelectedHiddenWISPDataForm();
                        CreateWISPSubmitForm(IndexID,LV_Data);          		
        
                        Connection(IndexID+1,"save");
                    } else if(CheckSavedWispData(bssid)){
            			
                        CreateSelectedHiddenWISPDataForm();
                        CreateWISPSubmitForm(IndexID,LV_Data);          		
        
                        Connection(IndexID+1,"save");
                    } else {
                        CreateSelectedWISPDataForm();
                        CreateWISPSubmitForm(IndexID,LV_Data);
                        
                        $('#SiteSurveyListData-modal').modal('hide');
                        $('#SelectedWISPDataForm-modal').modal({
                            keyboard: false,
                            backdrop: 'static'
                         })  
                     }        

                }         

            });


  
        }
        function getWISPCrypto(sec,i) {
            var crypto = sec.split("/");
            if (crypto[1] == 'TKIP') {
                $("#wl"+i+"_apcli"+i+"_crypto").val("tkip");
            } else if (crypto[1] == 'AES') {
                $("#wl"+i+"_apcli"+i+"_crypto").val("aes");
            } else {
                $("#wl"+i+"_apcli"+i+"_crypto").val("mixed");
            }
        }
        function CreateSelectedWEPCryptDataForm(){
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {  
            $("#SelectedWISPDataForm").append('<div class="row-fluid">');
            $("#SelectedWISPDataForm").append('<label><%lang("Key");%></label>');
            $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_key_index' id='wl"+i+"_apcli"+i+"_key_index' value='1'/>");   
            $("#SelectedWISPDataForm").append("<input type='password' name='wl"+i+"_apcli"+i+"_key1' id='wl"+i+"_apcli"+i+"_key1' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WEP Key 1\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key1','wl"+i+"_apcli_sec_wep_rule','i','key1');%>')/ >");
            $("#SelectedWISPDataForm").append('<label><%lang("Re-type Key");%></label>');
            $("#SelectedWISPDataForm").append("<input type='password' name='wl"+i+"_apcli"+i+"_keyc' id='wl"+i+"_apcli"+i+"_keyc' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WEP Key 1\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key1','wl"+i+"_apcli_sec_wep_rule','i','key1');%>')/ >");
            $("#SelectedWISPDataForm").append('</div>');

            $("#SelectedWISPDataForm").append('<div class="row-fluid">');
            $("#SelectedWISPDataForm").append('<label><%lang("The Key is an ASCII string of 8-63 digits");%></label>');
            $("#SelectedWISPDataForm").append('</div>');
        }
}


        function CreateSelectedPSK2CryptDataForm(){
         for (var i = 0; i < <%nv_get("wan_num");%>; i++) {     
            $("#SelectedWISPDataForm").append('<div class="row-fluid">');
            $("#SelectedWISPDataForm").append('<label><%lang("Key");%></label>');

            $("#SelectedWISPDataForm").append("<input type='password' name='wl"+i+"_apcli"+i+"_key' id='wl"+i+"_apcli"+i+"_key' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WPA2/PSK key\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key','wl"+i+"_apcli_sec_wpa2_rule','i','key');%>') />");
            $("#SelectedWISPDataForm").append('<label><%lang("Re-type Key");%></label>');
            $("#SelectedWISPDataForm").append("<input type='password' name='wl"+i+"_apcli"+i+"_keyc' id='wl"+i+"_apcli"+i+"_keyc' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WPA2/PSK key\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key','wl"+i+"_apcli_sec_wpa2_rule','i','key');%>') />");
            $("#SelectedWISPDataForm").append('</div>');

            $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_crypto' id='wl"+i+"_apcli"+i+"_crypto'/>");

            $("#SelectedWISPDataForm").append('<div class="row-fluid">');
            $("#SelectedWISPDataForm").append('<label><%lang("The Key is an ASCII string of 8-63 digits");%></label>');
            $("#SelectedWISPDataForm").append('</div>');
         }
        }
        function CreateSelectedPSKCryptDataForm(){
          for (var i = 0; i < <%nv_get("wan_num");%>; i++) { 
            $("#SelectedWISPDataForm").append('<div class="row-fluid">');
            $("#SelectedWISPDataForm").append('<label><%lang("Key");%></label>');

            $("#SelectedWISPDataForm").append("<input type='password' name='wl"+i+"_apcli"+i+"_key' id='wl"+i+"_apcli"+i+"_key' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WPA/PSK key\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key','wl"+i+"_apcli_sec_wpa_rule','i','key');%>') />");
            $("#SelectedWISPDataForm").append('<label><%lang("Re-type Key");%></label>');
            $("#SelectedWISPDataForm").append("<input type='password' name='wl"+i+"_apcli"+i+"_keyc' id='wl"+i+"_apcli"+i+"_keyc' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WPA/PSK key\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key','wl"+i+"_apcli_sec_wpa_rule','i','key');%>') />");
            $("#SelectedWISPDataForm").append('</div>');

            $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_crypto' id='wl"+i+"_apcli"+i+"_crypto' />");

            $("#SelectedWISPDataForm").append('<div class="row-fluid">');
            $("#SelectedWISPDataForm").append('<label><%lang("The Key is an ASCII string of 8-63 digits");%></label>');
            $("#SelectedWISPDataForm").append('</div>');
          }
        }
      function CreateSelectedWISPDataForm(){
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) { 
                $("#SelectedWISPDataForm").append('<div class="row-fluid">');
                $("#SelectedWISPDataForm").append('<label><%lang("Target SSID");%></label>');
                $("#SelectedWISPDataForm").append("<input type='text' name='wl"+i+"_apcli"+i+"_ssid' id='wl"+i+"_apcli"+i+"_ssid' value='' size='20' maxlength='20' readonly onblur='valid_name(this, \"Target SSID\", 1)'/>");
                $("#SelectedWISPDataForm").append('</div>');

                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_bssid' id='wl"+i+"_apcli"+i+"_bssid' value='' size='20' maxlength='20'/>");
                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_channel' id='wl"+i+"_channel' value='' size='20' maxlength='20'/>");
                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_extcha' id='wl"+i+"_extcha' value='' size='20' maxlength='20'/>");

                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_secmode' id='wl"+i+"_apcli"+i+"_secmode' value='' size='20' maxlength='20' readonly/>");
            }
            for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
                $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
            }

        }
       function CreateSelectedHiddenWISPDataForm(){
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {     
                $("#SelectedWISPDataForm").append("<tr><td><input type='hidden' class='input-medium' name='wl"+i+"_apcli"+i+"_ssid' id='wl"+i+"_apcli"+i+"_ssid' value='' size='20' maxlength='20' onblur='valid_name(this, \"Target SSID\", 1)'>"+
                    "<input type='hidden' class='input-medium' name='wl"+i+"_apcli"+i+"_bssid' id='wl"+i+"_apcli"+i+"_bssid' value='' size='20' maxlength='20'>"+
                    "<input type='hidden' class='input-medium' name='wl"+i+"_channel' id='wl"+i+"_channel' value='' size='20' maxlength='20'>"+
                    "<input type='hidden' class='input-medium' name='wl"+i+"_extcha' id='wl"+i+"_extcha' value='' size='20' maxlength='20'></td></tr>");
                $("#SelectedWISPDataForm").append("<tr><td><input type='hidden' class='input-medium' name='wl"+i+"_apcli"+i+"_secmode' id='wl"+i+"_apcli"+i+"_secmode' value='' size='20' maxlength='20'></td></tr>");
                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_key_index' id='wl"+i+"_apcli"+i+"_key_index' value='1'/>");   
            	$("#SelectedWISPDataForm").append("<tr><td><input type='hidden' class='input-medium' name='wl"+i+"_apcli"+i+"_key1' id='wl"+i+"_apcli"+i+"_key1' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WEP Key 1\")'" +
                "decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key1','wl"+i+"_apcli_sec_wep_rule','i','key1');%>' ></td></tr>");
            $("#SelectedWISPDataForm").append("<tr><td><input type='hidden' class='input-medium' name='wl"+i+"_apcli"+i+"_key' id='wl"+i+"_apcli"+i+"_key' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WPA/PSK key\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key','wl"+i+"_apcli_sec_wpa_rule','i','key');%>' ></td></tr>");
            $("#SelectedWISPDataForm").append("<tr><td><<input type='hidden' name='wl"+i+"_apcli"+i+"_crypto' id='wl"+i+"_apcli"+i+"_crypto'></td></tr>");
             
            }

        }
   
        function btn_sitesurvey_click(){
                $("#SiteSurveyMSG").append("<br><br><br><br><br><br><div class='center'><i class='icon-spinner icon-spin icon-5x aximDarkOrange'></i></div>");
                $("#SiteSurveyListData").hide();
                $('#SiteSurveyListData-modal').modal({
                    keyboard: false,
                    backdrop: true
                });
                setTimeout(function(){
                           $("#SiteSurveyMSG").empty();
                           var WISPData = getWISPData(); 
                           SiteSurveyDrawDataTables(WISPData);
                           $("#SiteSurveyListData").show();
                      }, 500);
        
           
        }
        
        function getWISPData() {
            var WISPData_Tmp;
            $.ajax({
               url:        "survey.cgi",
               type:       "POST",
               dataType:   "text",
               async:	   false,
               data:       { action:"Update", submit_button:"wan"},
               success: function(data){
                   var obj = jQuery.parseJSON(data);
                   data = obj.msg
                    
                    var rows = [];
                    var rows_data = {};
                    var rowsArray = data.split("|");
                    var rows_dataArray;
                    var ssid,bssid;
                    var wan_proto = "<%nv_get("wan0_proto");%>";
                    if(data == "") {
                        WISPData_Tmp = {};
                        return;
                    }
                    for (var i=0; i<rowsArray.length; i++ ){
                                var index = i+1;

                        rows_dataArray = rowsArray[i].split("^");
                        rows_data = {};
                        ssid = rows_dataArray[1];
                        bssid = rows_dataArray[2];
                        if(wan_proto=="wisp" && ssid==CurrentSSID && bssid==CurrentBSSID){
                            rows_data[0]="<i class=\"icon-ok\"></i>";
                        }else{
                            rows_data[0]="";
                        }
                        rows_data[1] = rows_dataArray[0];
                        rows_data[2] = rows_dataArray[1];
                        rows_data[3] = rows_dataArray[2];
                        rows_data[4] = rows_dataArray[3];
                        rows_data[5] = rows_dataArray[4];
                        var sig=rows_data[5];
                        if (rows_dataArray[7] == '1') {
                            rows_data[6] = 'ABOVE';
                        } else {
                            rows_data[6] = 'BELOW';
                        }
                        rows_data[7] = rows_dataArray[5];
                       
                        /*	
                        if(rows_data[4]!="NONE"){
                            rows_data[8]="<img src='picts/web_securitykey.png'/>";		               
                        }else{
                            rows_data[8]=" ";	
                        }*/
                            rows_data[8]=" ";	
                        if(sig>=76 && sig<=100){
                            if(rows_data[4]!="NONE"){
                                rows_data[9]="<img src='picts/web_securitykey.png'/><img src='picts/web_wavelevel4.png'/>";
                            }else{
                                rows_data[9]="<img src='picts/web_wavelevel4.png'/>";
                            }
                        } else if(sig>=51 && sig<=75) {
                            if(rows_data[4]!="NONE"){
                                rows_data[9]="<img src='picts/web_securitykey.png'/><img src='picts/web_wavelevel3.png'/>";
                            }else{
                                rows_data[9]="<img src='picts/web_wavelevel3.png'/>";
                            }
                        } else if(sig>=26 && sig<=50) {
                            if(rows_data[4]!="NONE"){
                                rows_data[9]="<img src='picts/web_securitykey.png'/><img src='picts/web_wavelevel2.png'/>";
                            }else{
                                rows_data[9]="<img src='picts/web_wavelevel2.png'/>";
                            }
                        } else {
                            if(rows_data[4]!="NONE"){
                                rows_data[9]="<img src='picts/web_securitykey.png'/><img src='picts/web_wavelevel1.png'/>";
                            }else{
                                rows_data[9]="<img src='picts/web_wavelevel1.png'/>";
                            }
                        }

                        if(wan_proto=="wisp" && CheckSavedWispData(bssid) && bssid==CurrentBSSID){
                            rows_data[10]="<input type='button' class='btn btn-inverse' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' disabled>&nbsp;&nbsp;"+
                                "<input type='button' class='btn btn-inverse' id='wl0_Forget' value='<%lang("Forget");%>' >";
                           
                        } else if(CheckSavedWispData(bssid)){
                            rows_data[10]="<input type='button' class='btn btn-inverse' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' >&nbsp;&nbsp;"+
                                "<input type='button' class='btn btn-inverse' id='wl0_Forget' value='<%lang("Forget");%>' >";
                          
                        } else {
                            rows_data[10]="<input type='button' class='btn btn-inverse' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' >&nbsp;&nbsp;"+
                                "<input type='button' class='btn btn-inverse' id='wl0_Forget' value='<%lang("Forget");%>'  disabled>"		                  

                        }
                        
                        rows[i] = rows_data;
                    }
                    WISPData_Tmp = rows;
               },
               error: function(xhr, textStatus, error){
                   //console.log(xhr.statusText, textStatus, error);
               }
            });
        
            return WISPData_Tmp;
        
        }

        function CheckSavedWispData(LV_bssi){
            var bssid_flag = false;
            for (var i=0; i<SavedWispData.length; i++){
                if(SavedWispData[i]['BSSID']==LV_bssi){
                    bssid_flag = true;
                }
            }
            return bssid_flag;
        }



        
<% wan_show_wisp_swap_list(); %>
var CurrentSSID = "<%nvg_attr_get("", "wl0_apcli_rule", "0", "ssid");%>";     
var CurrentBSSID = "<%nvg_attr_get("", "wl0_apcli_rule", "0", "bssid");%>";
        $(document).ready(function(){
            var connection_state = 0;
            init();
            var cnnt_rows = GetGridData();
            DrawDataTables(cnnt_rows);
            $("#hide_wan_enable_div").hide();            
             GetRadioValue();
        
             
            $("#refresh").click(function(){
                $("#SiteSurveyMSG").append("<br><br><br><br><br><br><div class='center'><i class='icon-spinner icon-spin icon-5x aximDarkOrange'></i></div>");
                $("#SiteSurveyListData").hide();
                setTimeout(function(){
                           $("#SiteSurveyMSG").empty();
                           $("#SiteSurveyListData").show();
                           var WISPData = getWISPData();
                           SSDataTable.fnClearTable();
                           SSDataTable.fnDestroy();
                           SiteSurveyDrawDataTables(WISPData);
                      }, 500);
            });
        
        
            $("#close").click(function(){
                SSDataTable.fnClearTable();
                SSDataTable.fnDestroy();
            });
            $("#wisp_close").click(function(){
                SSDataTable.fnClearTable();
                SSDataTable.fnDestroy();
            });
            //var ether_expand = '<% nvg_attr_get("","cnnt_rule", "0", "expand");%>';
            var ether_expand = 1;
            var len = $("#CnntListData tbody tr").length;
            for(var i=0;i<len;i++) {
                if($("#CnntListData tbody tr:eq("+i+") td:eq(0)").text() == '<%lang("Wired Internet");%>  ') {
                    if(ether_expand == "1") {
                        var nTr = $("#CnntListData tbody tr")[i];
                        DataTable.fnOpen( nTr, fnFormatDetailsEther(nTr), 'details' );
                    }
                    else {
                        
                    }
                }
            }
            //var wisp_expand = '<% nvg_attr_get("","cnnt_rule", "1", "expand");%>';
            var wisp_expand = 1;
            len = $("#CnntListData tbody tr").length;
            for(var i=0;i<len;i++) {
                if($("#CnntListData tbody tr:eq("+i+") td:eq(0)").text() == '<%lang("Wireless Internet");%>  ') {
                    if(wisp_expand == "1") {
                        var nTr = $("#CnntListData tbody tr")[i];
   //                     DataTable.fnOpen( nTr, fnFormatDetailsWISP(nTr), 'details' );
                    }
                    else {
                        
                    }
                }
            }
            //var usb_expand = '<% nvg_attr_get("","cnnt_rule", "2", "expand");%>';
            var usb_expand = 1;
            len = $("#CnntListData tbody tr").length;
            for(var i=0;i<len;i++) {
            
                if($("#CnntListData tbody tr:eq("+i+") td:eq(0)").text() == '<%lang("USB Internet");%>  ') {
                    if(usb_expand == "1") {
                        var nTr = $("#CnntListData tbody tr")[i];
                        DataTable.fnOpen( nTr, fnFormatDetailsUSB(nTr), 'details' );
                    }
                    else {
                        
                    }
                }
            }


        });
        var wl_apcli_key=new Array(<%nv_get("wl_rule_num");%>);

        function encodekey(backup) {
            for (var i = 0; i < <%nv_get("wl_rule_num");%>; i++) {
                var apcli_num;
                if (i=="0") apcli_num = "<%nv_get("wl0_apcli_rule_num");%>";
                if (i=="1") apcli_num = "<%nv_get("wl1_apcli_rule_num");%>";
                if (backup) wl_apcli_key[i]=new Array(apcli_num);
                for (var j = 0; j < apcli_num; j++) {
                    var secmode = $('#wl'+i+'_apcli'+j+'_secmode').val();
                    if (secmode == 'wep') {
                        if ($('#wl'+i+'_apcli'+j+'_key1').val() == "") return false;
                        if (backup) wl_apcli_key[i][j]=new Array(4);
                        for (var k = 1; k < 2; k++) {
                            if(backup) {
                                wl_apcli_key[i][j][k-1]=$('#wl'+i+'_apcli'+j+'_key'+k).val();
                                $('#wl'+i+'_apcli'+j+'_key'+k).val(encode64($('#wl'+i+'_apcli'+j+'_key'+k).val()));
                            }else 
                                $('#wl'+i+'_apcli'+j+'_key'+k).val(wl_apcli_key[i][j][k-1]);
                        }
                    }
                    if (secmode == 'psk' || secmode == 'psk2') {
                        if ($('#wl'+i+'_apcli'+j+'_key').val() == "") return false;
                        if (backup) {
                            wl_apcli_key[0]=$('#wl'+i+'_apcli'+j+'_key').val();
                            $('#wl'+i+'_apcli'+j+'_key').val(encode64($('#wl'+i+'_apcli'+j+'_key').val()));
                        }else
                            $('#wl'+i+'_apcli'+j+'_key').val(wl_apcli_key[0]);
                    }   
                }
            }
            return true;
        }
        function dongle_modal_trigger() {
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
                apn_type = $("input[name=wan"+i+"_wwan_apn_type]:checked").val();
                sel_apn_type(apn_type, i, "1");
            }
            for (var i=0; i<cnnt_rule.length; i++ )  {
                if(cnnt_rule[i][enum_cnnt_basic._NAME] == "USB") {
                        cnnt_rule[i][enum_cnnt_basic._PROTO]="wwan";
                }
            }
            check_click("USB", "wwan");
            $('#dongleModal').modal('show');
        }   
    
        function pop_pppoe() {
            for (var i=0; i<cnnt_rule.length; i++ )  {
                if(cnnt_rule[i][enum_cnnt_basic._NAME] == "Ethernet") {
                        cnnt_rule[i][enum_cnnt_basic._PROTO]="pppoe";
                }
            }
            check_click("Ethernet", "pppoe"); 
            $('#pppoeModal').modal('show');
        }
		
        function pop_static() {
            for (var i=0; i<cnnt_rule.length; i++ )  {
                if(cnnt_rule[i][enum_cnnt_basic._NAME] == "Ethernet") {
                        cnnt_rule[i][enum_cnnt_basic._PROTO]="static";
                }
            }
            check_click("Ethernet", "static"); 
            $('#staticModal').modal('show');
        }

        function sel_apn_type(I, nth, type) {
            var enable = $("input[name=wan"+nth+"_enable]:checked").val();
            if (enable == 0) {
                return;
            }
            if (I == "1") {
                $('#wan'+nth+'_wwan_location').attr("disabled",true);
                $('#wan'+nth+'_wwan_isp').attr("disabled",true);
                $('#wan'+nth+'_wwan_apn').attr("disabled",false);
                if (type == 1) $('#wan'+nth+'_wwan_dialstr').attr("disabled",false);
            } else if (I == "2") {
                $('#wan'+nth+'_wwan_location').attr("disabled",true);
                $('#wan'+nth+'_wwan_isp').attr("disabled",true);
                $('#wan'+nth+'_wwan_apn').attr("disabled",true);
                if (type == 1) $('#wan'+nth+'_wwan_dialstr').attr("disabled",true);
            } else {
                $('#wan'+nth+'_wwan_location').attr("disabled",false);
                $('#wan'+nth+'_wwan_isp').attr("disabled",false);
                $('#wan'+nth+'_wwan_apn').attr("disabled",false);
                if (type == 1) $('#wan'+nth+'_wwan_dialstr').attr("disabled",false);

                sel_isp(nth, type);
            }
        }

    function submit_pppoe(F) {
        F.submit_button.value = "wan";
        F.action.value = "Apply";

        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#form_pppoe").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 5000);
        $('#pppoeModal').modal('hide');
    }

    function submit_static(F) {
        F.submit_button.value = "wan";
        F.action.value = "Apply";

        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#form_static").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 5000);
        $('#staticModal').modal('hide');
    }


    function to_submit_wwan(F) {
        for (var i=0; i<cnnt_rule.length; i++ ) {
            if(cnnt_rule[i][enum_cnnt_basic._NAME] == "USB" ) {
                cnnt_rule[i][enum_cnnt_basic._PROTO] = "wwan";
                cnnt_rule[i][enum_cnnt_basic._CONNECTION] = 1;
            }
        }
        F.submit_button.value = "wan";
        F.action.value = "Apply";

        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#wwan_form").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 5000);
        $('#dongleModal').modal('hide');
    }

    function valid_password() {
        if($("#wl0_apcli0_secmode").val() == "wep") {
            if ($("#wl0_apcli0_key1").val() != $("#wl0_apcli0_keyc").val()){
                alert('<%lang("Please enter the same password in both fields.");%>');
                $("#wl0_apcli0_key1").focus();
                $("#wl0_apcli0_key1").select();
                return false;
            }
        } else {
            if ($("#wl0_apcli0_key").val() != $("#wl0_apcli0_keyc").val()){
                alert('<%lang("Please enter the same password in both fields.");%>');
                $("#wl0_apcli0_key").focus();
                $("#wl0_apcli0_key").select();
                return false;
            }
        }
        return true;
    }

    function to_submit_wisp(F) {
        for (var i=0; i<cnnt_rule.length; i++ ) {
            if(cnnt_rule[i][enum_cnnt_basic._NAME] == "WISP" ) {
                cnnt_rule[i][enum_cnnt_basic._CONNECTION] = 1;
            }
        }
        if(!valid_password(F)) return;
        if (!encodekey(1)) {
                    alert('<%lang("The Key must have value.");%>');
                    return;
                }
        F.submit_button.value = "save";
        F.action.value = "Apply";        
        F.wisp_swap_rule.value = "";
        $('#SelectedWISPDataForm-modal').modal('hide');
        open_wisp_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#wisp_form").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 10000);
    }

$.fn.modal.defaults.maxHeight = function(){

    // subtract the height of the modal header and footer

    return $(window).height() - 165; 
}

    </script>
    
    <!-- WISP Modal -->
    <div class="page-container">
    <div id="SelectedWISPDataForm-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
        <h3 id="WISPLabel" ><%lang("WISP");%></h3>
        </div>
        <form name="wisp_form" id="wisp_form" action="apply.cgi" method="post">
        <script type="text/javascript">do_hidden_elements('wisp_swap_rule')</script>

            <div class="modal-body">
            	<p id="SelectedWISPDataForm"> </p>
                <!--<table class="table" id="SelectedWISPDataForm"></table>-->
            </div>
            <div class="modal-footer">
                <input type="button" class="btn btn-primary" name="wisp_connect" id="wisp_connect" onClick="to_submit_wisp(this.form)" value="<% lang("Connect"); %>" >
                <input type="button" class="btn btn-inverse" data-dismiss="modal" aria-hidden="true" name="wisp_close" id="wisp_close" value="<% lang("Cancel"); %>">
            </div>
        </form>
    </div>
    </div>

    <!-- dongle Modal -->
    <div class="page-container">
    <div id="dongleModal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
            <h3 id="myModalLabel"><%lang("3G/4G dongle");%></h3>
        </div>
        
        <form class="form-horizontal" name="wwan_form" id="wwan_form" action="apply.cgi" method="post">
            <script type="text/javascript">do_hidden_elements('wan_rule');</script>
                  
            <div class="modal-body">
                <% wwan_show_setting(); %>
            </div>
            <div class="modal-footer">
                <input type="button" class="btn btn-primary" name="add" id="add" onClick="to_submit_wwan(this.form)" value="<% lang("Connect"); %>" >
                <input type="button" class="btn btn-inverse" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
            </div>
        
        </form>
        
    </div>
    </div>

    <!-- SiteSurvey Modal -->
    <div class="page-container">
 	<div id="SiteSurveyListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        <div class="modal-body" style="height:400px;">
            <div id="SiteSurveyMSG"></div>
            <table class="table" id="SiteSurveyListData"></table>
        </div>
        <div class="modal-footer">
            <input type="button" class="btn btn-primary" name="refresh" id="refresh" value="<% lang("Refresh"); %>" >
            <input type="button" class="btn btn-inverse" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
        </div>
    </div> 
    </div>  

    <% tbl_get("cnnt_rule"); %>


    <!-- Static Modal -->
    <div class="page-container">
    <div id="staticModal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="StaticModalLabel" aria-hidden="itrue">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
            <h3 id="StaticModalLabel"><%lang("Static IP");%></h3>
        </div>
    <form class="form-horizontal" name="form_static" id="form_static" action="apply.cgi" method="post">
    <script type="text/javascript">do_hidden_elements('wan_rule');</script>
        <div class="modal-body">
            <% static_show_setting(); %>
        </div>

    <div class="modal-footer">
        <input type="button" class="btn btn-primary" name="refresh_static" id="refresh_static" onClick="submit_static(this.form)" value="<% lang("Connect"); %>" >
        <input type="button" class="btn btn-inverse" data-dismiss="modal" aria-hidden="true" name="close_static" id="close_static" value="<% lang("Cancel"); %>">
    </div>
    </form>
    </div> 
    </div>
 
    <!-- PPPoE Modal --> 
    <div class="page-container">
    <div id="pppoeModal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="PPPoEModalLabel" aria-hidden="true">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
            <h3 id="PPPoEModalLabel"><%lang("PPPoE");%></h3>
        </div>
    <form class="form-horizontal" name="form_pppoe" id="form_pppoe" action="apply.cgi" method="post">
    <script type="text/javascript">do_hidden_elements('wan_rule');</script>

        <div class="modal-body">
            <% pppoe_show_setting(); %>
        </div>
    
        <div class="modal-footer">
            <input type="button" class="btn btn-primary" name="connect_pppoe" id="connect_pppoe" onClick="submit_pppoe(this.form)" value="<% lang("Connect"); %>" >
            <input type="button" class="btn btn-inverse" data-dismiss="modal" aria-hidden="true" name="close_pppoe" id="close_pppoe" value="<% lang("Cancel"); %>">
        </div>
    </form>
    </div>
    </div>

	</head>

	<body>
	  <%do_bootstrap_navbar_menu();%>
		<div class="main-container container-fluid" id="main-container">
            <a id="menu-toggler" class="menu-toggler" href="#"><span class="menu-text"></span></a>
			<div id="sidebar" class="sidebar">
                <script type="text/javascript">
                    try{ace.settings.check('sidebar' , 'fixed')}catch(e){}
                </script>
                <%do_bootstrap_menu();%>
                <div id="sidebar-collapse" class="sidebar-collapse"><i class="icon-double-angle-left" data-icon1="icon-double-angle-left" data-icon2="icon-double-angle-right" ></i></div>
                <script type="text/javascript">
                    try{ace.settings.check('sidebar' , 'collapsed')}catch(e){}
                </script>
			</div><!--/#sidebar-->

		
			<div id="main-content" class="main-content">
					
					<div id="breadcrumbs" class="breadcrumbs">
						<ul class="breadcrumb">
							<li><i class="icon-home"></i> <%lang("Home");%><span class="divider"><i class="icon-angle-right"></i></span></li>
							<li class="active"><%lang("Internet");%></li>
						</ul><!--.breadcrumb-->


					</div><!--breadcrumbs-->



					<div id="page-content" class="page-content">
						
						<div class="page-header position-relative">
							<h1><%lang("Internet");%> <small><i class="icon-double-angle-right"></i> </small></h1>
							
						</div><!--/page-header-->

						

						<div class="row-fluid">
<!-- PAGE CONTENT BEGINS HERE -->


        <script type="text/javascript">create_waiting_window();</script>
        <script type="text/javascript">create_wisp_window();</script>
        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('cnnt_rule')</script>
      
                <table id="CnntListData" class="table table-striped  table-hover"></table>	
                <%tbl_create_table("cnnt_table", "cnnt_rule", "1");%>

        </form>
        
  
                    <script type="text/javascript"><%show_copyright();%></script>
  
<!-- PAGE CONTENT ENDS HERE -->
						 </div><!--/row-->
	
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




	</body>
</html>
