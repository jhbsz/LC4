
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<title><% nv_get("model"); %> - Status</title>
		<meta name="description" content="overview & stats" />

		<meta name="viewport" content="width=device-width, initial-scale=1.0" />

		<!-- basic styles -->
		<link href="css/bootstrap.min.css" rel="stylesheet" />
		<link href="css/bootstrap-responsive.min.css" rel="stylesheet" />

		<link rel="stylesheet" href="font-awesome/css/font-awesome.css" />
		<!--[if IE 7]>
		  <link rel="stylesheet" href="font-awesome/css/font-awesome-ie7.min.css" />
		<![endif]-->


		<!-- page specific plugin styles -->
		

		<!-- ace styles -->
		<link rel="stylesheet" href="css/ace.min.css" />
		<link rel="stylesheet" href="css/ace-responsive.min.css" />
		<link rel="stylesheet" href="css/ace-skins.min.css" />
		<!--[if lt IE 9]>
		  <link rel="stylesheet" href="css/ace-ie.min.css" />
		<![endif]-->
		    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

		<!--[if lt IE 9]>
		  <link rel="stylesheet" href="css/ace-ie.min.css" />
		<![endif]-->
    <% tbl_get("cnnt_rule"); %>
    <script type="text/javascript">

          var enum_cnnt_basic = {
            _EXPAND:0,
            _NAME:1,
            _ENABLE:2,
            _CONNECTION:3,
            _PROTO:4,

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
                connect = '<% nvg_attr_get("","cnnt_rule", 0, "connection");%>';
                proto_connect = '<% nvg_attr_get("","cnnt_rule", 0, "proto");%>';
            }else if(name == "WISP") {
                connect = '<% nvg_attr_get("","cnnt_rule", 1, "connection");%>';
                proto_connect = '<% nvg_attr_get("","cnnt_rule", 1, "proto");%>';
            }else if(name == "USB") {
                connect = '<% nvg_attr_get("","cnnt_rule", 2, "connection");%>';
                proto_connect = '<% nvg_attr_get("","cnnt_rule", 2, "proto");%>';
            }
            var len = $("#CnntListData tbody tr").length;
            for(var i=0;i<len;i++) {
                if($("#CnntListData tbody tr:eq("+i+") td:eq(0)").text() == name) {
                    if(connect == "1") {
                        if(proto_connect != proto) {
                            var str="".concat('<td class="center">', btn_connect_enable(0, name), "</td>");
                            $("#CnntListData tbody tr:eq("+i+") td:eq(3)").replaceWith(str);
                        }
                        else {
                            var str="".concat('<td class="center">', btn_connect_enable(1, name), "</td>");
                            $("#CnntListData tbody tr:eq("+i+") td:eq(3)").replaceWith(str);
                        }
                    }
                }
            }

        }
        function Connection(index,action) {
            F = document.getElementsByName('wisp_form')[0];
            F.submit_button.value = action;
            F.action.value = "Apply";
            F.wisp_swap_rule.value = "";
            $('#SiteSurveyListData-modal').modal('hide');

            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#wisp_form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }
        function to_submit() {
            F = document.getElementsByName('form')[0];
            F.submit_button.value = "cnnt";
            F.action.value = "Apply";
            local_pack_key_value_pair(F, "cnnt_table", "cnnt_rule");

            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function plug(name) {
            if(name == "Ethernet") {
                ether_plug = '<%nvg_attr_get("","plug_rule", "0", "ethernet");%>';
                if(ether_plug == "0") {
                    return '<font color="red">unplug</font>';
                }else {
                    return '<font color="green">plug</font>';
                }
            }else if(name == "WISP") {
                wisp_plug = '<%nvg_attr_get("","plug_rule", "0", "wisp");%>';
                if(wisp_plug == "0" || wisp_plug == "1") {
                    return '<font color="red">no wifi link</font>';
                }else {
                    ssid = '<% nvg_attr_get("","wl0_apcli_rule", "0", "ssid");%>';
                    return '<font color="green">'+ssid+'</font>';
                }
            }else if(name == "USB") {
                usb_plug = '<%nvg_attr_get("","plug_rule", "0", "usb");%>';
                if(usb_plug == "0") {
                    return '<font color="red">unplug</font>';
                }else {
                    return '<font color="green">plug</font>';
                }
            }
        }

        function connect_status(name, connection) {
            if(connection == "1") {
                wan_status = '<% nvg_attr_get("","wan_status_rule", "0", "state");%>';
                if(name == "Ethernet" && '<%nvg_attr_get("","plug_rule", "0", "ethernet");%>' == "1") {
                    wan0_proto = '<%nvg_get("wan0_proto");%>';
                    if(wan_status == "0") {
                        return '<td class="center"><font color="red">disconnect</font></td>';
                    }else if(wan_status == "4" || wan0_proto == "static") {
                        return '<td class="center"><font color="green"><%nvg_get("wan0_ipaddr");%></font></td>';
                    }else {
                        return '<td class="center"><font color="yellow">connecting</font></td>';
                    }
                } else if(name == "WISP" && '<%nvg_attr_get("","plug_rule", "0", "wisp");%>' =="2") {
                    if(wan_status == "0") {
                        return '<td class="center"><font color="red">disconnect</font></td>';
                    }else if(wan_status == "4") {
                        return '<td class="center"><font color="green"><%nvg_get("wan0_ipaddr");%></font></td>';
                    }else {
                        return '<td class="center"><font color="yellow">connecting</font></td>';
                    }
                } else if(name == "USB" && '<%nvg_attr_get("","plug_rule", "0", "usb");%>' == "1") {
                    if(wan_status == "0") {
                        return '<td class="center"><font color="red">disconnect</font></td>';
                    }else if(wan_status == "4") {
                        return '<td class="center"><font color="green"><%nvg_get("wan0_ipaddr");%></font></td>';
                    }else {
                        return '<td class="center"><font color="yellow">connecting</font></td>';
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
                return "<span class='btn btn-warning' onclick='btn_sitesurvey_click()'><%lang("Search");%></span>";
            }
            if (v == 1) {
                return "<span class='btn btn-success' onClick='discnnt_press(\""+name+"\")'><%lang("Disconnect");%></span>";
            } else {
                return "<span class='btn btn-warning' onClick='cnnt_press(\""+name+"\")'><%lang("Connect");%></span>";	
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
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,                
                "aoColumns": [
                    { "sWidth": "70%","sTitle": "", "sClass": "left" },
                    { "sWidth": "10%","sTitle": "", "sClass": "center" },
                    { "sWidth": "10%","sTitle": "", "sClass": "center"},           
                    { "sWidth": "10%","sTitle": "", "sClass": "center"},
                    { "sWidth": "0%", "sTitle": "", "sClass": "center"}                    
                ],  
		        "bAutoWidth": true,
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
            var sOut = '<label class="radio inline"><input type="radio" value="2" name="ether_enable" id="ether_name" onclick="GetRadioValue(this.name)" <%nvg_attr_match("ether_enable", "cnnt_rule", "0", "proto", "dhcp", "checked"); %>><%lang("DHCP");%></label>';
            sOut += '<label class="radio inline"><input type="radio" value="1" name="ether_enable" id="ether_name" onclick="pop_pppoe()" <%nvg_attr_match("ether_enable", "cnnt_rule", "0", "proto", "pppoe", "checked"); %>><%lang("PPPoE");%></input></label>';
            sOut += '<label class="radio inline"><input type="radio" value="0" name="ether_enable" id="ether_name" onClick="pop_static()" <%nvg_attr_match("ether_enable", "cnnt_rule", "0", "proto", "static", "checked"); %>><%lang("Static IP");%></label></td></tr>';	   
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
            var sOut = '<label class="radio inline"><input type="radio" value="2" name="USB_enable" onclick="GetRadioValue(this.name)" <%nvg_attr_match("USB_enable", "cnnt_rule", "2", "proto", "iphone", "checked"); %>><%lang("Apple iphone");%></label>';
        sOut += '<label class="radio inline"><input type="radio" value="1" name="USB_enable" onclick="GetRadioValue(this.name)" <%nvg_attr_match("USB_enable", "cnnt_rule", "2", "proto", "htc", "checked"); %>><%lang("Google Android phone");%></label>';
        sOut += '<label class="radio inline"><input type="radio" value="0" name="USB_enable" onclick="dongle_modal_trigger()" <%nvg_attr_match("USB_enable", "cnnt_rule", "2", "proto", "wwan", "checked"); %>><%lang("3G/4G Mobile Internet");%></label></td></tr>';   
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
                    if(ether_enable_doc[0].checked) {/* DHCP */
                        cnnt_rule[i][enum_cnnt_basic._PROTO]="dhcp";
                        check_click("Ethernet", "dhcp");
                    }
                }
            }
        }
    
        function GetGridData(){
            var connect_rows = [];
            var connect_data = {};
            for (var i=0; i<cnnt_rule.length; i++ )  {
                connect_data = {};
                //connect_data[0] = icon_expand(cnnt_rule[i][enum_cnnt_basic._EXPAND]);
                connect_data[0] = cnnt_rule[i][enum_cnnt_basic._NAME];
                connect_data[1] = connect_status(cnnt_rule[i][enum_cnnt_basic._NAME], cnnt_rule[i][enum_cnnt_basic._CONNECTION]);
           //     connect_data[2] = btn_priority_enable( cnnt_rule[i][enum_cnnt_basic._ENABLE], cnnt_rule[i][enum_cnnt_basic._NAME] );
                connect_data[2] = plug(cnnt_rule[i][enum_cnnt_basic._NAME]);
                connect_data[3] = btn_connect_enable(cnnt_rule[i][enum_cnnt_basic._CONNECTION], cnnt_rule[i][enum_cnnt_basic._NAME]);
                connect_data[4] = text_hidden_proto(cnnt_rule[i][enum_cnnt_basic._PROTO]);

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
                "sDom": "<'row'<'span2'l><'span2'f>r>t<'row'<'span2'i><'span2'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "", "sClass":"center" },
                    { "sTitle": "<%lang("Channel");%>", "sClass":"center" ,"bVisible": false},
                    { "sTitle": "<%lang("SSID");%>", "sClass":"left" },
                    { "sTitle": "<%lang("BSSID");%>", "sClass":"center","bVisible": false },
                    { "sTitle": "<%lang("Security Mode");%>", "sClass":"center" ,"bVisible": false},
                    { "sTitle": "<%lang("Signal");%>","bVisible": false  },
                    { "sTitle": "<%lang("ExtCH");%>", "sClass":"center","bVisible": false },
                    { "sTitle": "<%lang("Wireless Mode");%>","bVisible": false },
                    { "sTitle": "<%lang("Security Mode");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Signal");%>" },
                    { "sTitle": "<%lang("Active");%>" ,"sClass":"center"},
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
            $("#SelectedWISPDataForm").append('<div class="row show-grid">');
            $("#SelectedWISPDataForm").append('<div class="span2"><%lang("Key");%></div>');
            $("#SelectedWISPDataForm").append('<div class="span4">');
            $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_key_index' id='wl"+i+"_apcli"+i+"_key_index' value='1'/>");   
            $("#SelectedWISPDataForm").append("<input type='text' name='wl"+i+"_apcli"+i+"_key1' id='wl"+i+"_apcli"+i+"_key1' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WEP Key 1\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key1','wl"+i+"_apcli_sec_wep_rule','i','key1');%>')/ >");
            $("#SelectedWISPDataForm").append('</div>');
            $("#SelectedWISPDataForm").append('</div>');

            $("#SelectedWISPDataForm").append('<div class="row show-grid">');
            $("#SelectedWISPDataForm").append('<div class="span6"><%lang("The Key is an ASCII string of 8-63 digits");%></div>');
            $("#SelectedWISPDataForm").append('</div>');
        }
}


        function CreateSelectedPSK2CryptDataForm(){
         for (var i = 0; i < <%nv_get("wan_num");%>; i++) {     
            $("#SelectedWISPDataForm").append('<div class="row show-grid">');
            $("#SelectedWISPDataForm").append('<div class="span2"><%lang("Key");%></div>');
            $("#SelectedWISPDataForm").append('<div class="span4">');

            $("#SelectedWISPDataForm").append("<input type='text' name='wl"+i+"_apcli"+i+"_key' id='wl"+i+"_apcli"+i+"_key' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WPA2/PSK key\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key','wl"+i+"_apcli_sec_wpa2_rule','i','key');%>') />");
            $("#SelectedWISPDataForm").append('</div>');
            $("#SelectedWISPDataForm").append('</div>');

            $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_crypto' id='wl"+i+"_apcli"+i+"_crypto'/>");

            $("#SelectedWISPDataForm").append('<div class="row show-grid">');
            $("#SelectedWISPDataForm").append('<div class="span6"><%lang("The Key is an ASCII string of 8-63 digits");%></div>');
            $("#SelectedWISPDataForm").append('</div>');
         }
        }
        function CreateSelectedPSKCryptDataForm(){
          for (var i = 0; i < <%nv_get("wan_num");%>; i++) { 
            $("#SelectedWISPDataForm").append('<div class="row show-grid">');
            $("#SelectedWISPDataForm").append('<div class="span2"><%lang("Key");%></div>');
            $("#SelectedWISPDataForm").append('<div class="span4">');

            $("#SelectedWISPDataForm").append("<input type='text' name='wl"+i+"_apcli"+i+"_key' id='wl"+i+"_apcli"+i+"_key' value='' size='20' maxlength='64' onChange='valid_wl_sec_wpa_psk_key(this,\"WPA/PSK key\")' decode64('<%b64_nvg_attr_get('wl"+i+"_apcli"+i+"_key','wl"+i+"_apcli_sec_wpa_rule','i','key');%>') />");
            $("#SelectedWISPDataForm").append('</div>');
            $("#SelectedWISPDataForm").append('</div>');

            $("#SelectedWISPDataForm").append('<div class="row show-grid">');
            $("#SelectedWISPDataForm").append('<div class="span2"><%lang("Encryption Method");%></div>');
            $("#SelectedWISPDataForm").append('<div class="span4">');

            $("#SelectedWISPDataForm").append("<select name='wl"+i+"_apcli"+i+"_crypto' id='wl"+i+"_apcli"+i+"_crypto'><option value='tkip' <%nvg_attr_match("wl$0_apcli$1_crypto", "wl$0_apcli_sec_wpa_rule", "$1", "crypto", "tkip", "selected");%> ><% lang("TKIP"); %></option><option value='aes' <%nvg_attr_match("wl$0_apcli$1_crypto","wl$0_apcli_sec_wpa_rule", "$1", "crypto", "aes", "selected");%>><% lang("AES"); %></option><option value='mixed' <%nvg_attr_match("wl$0_apcli$1_crypto","wl$0_apcli_sec_wpa_rule", "$1", "crypto", "mixed", "selected");%>><% lang("Mixed (TKIP+AES)"); %></option></select>");
            $("#SelectedWISPDataForm").append('</div>');
            $("#SelectedWISPDataForm").append('</div>');

            $("#SelectedWISPDataForm").append('<div class="row show-grid">');
            $("#SelectedWISPDataForm").append('<div class="span6"><%lang("The Key is an ASCII string of 8-63 digits");%></div>');
            $("#SelectedWISPDataForm").append('</div>');
          }
        }
      function CreateSelectedWISPDataForm(){
            for (var i = 0; i < <%nv_get("wan_num");%>; i++) { 
                $("#SelectedWISPDataForm").append('<div class="row">');
                $("#SelectedWISPDataForm").append('<div class="span6">');

                $("#SelectedWISPDataForm").append('<div class="row show-grid">');
                $("#SelectedWISPDataForm").append('<div class="span2"><%lang("Target SSID");%></div>');
                $("#SelectedWISPDataForm").append('<div class="span4">');
                $("#SelectedWISPDataForm").append("<input type='text' name='wl"+i+"_apcli"+i+"_ssid' id='wl"+i+"_apcli"+i+"_ssid' value='' size='20' maxlength='20' readonly onblur='valid_name(this, \"Target SSID\", 1)'/>");
                $("#SelectedWISPDataForm").append('</div>');
                $("#SelectedWISPDataForm").append('</div>');

                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_apcli"+i+"_bssid' id='wl"+i+"_apcli"+i+"_bssid' value='' size='20' maxlength='20'/>");
                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_channel' id='wl"+i+"_channel' value='' size='20' maxlength='20'/>");
                $("#SelectedWISPDataForm").append("<input type='hidden' name='wl"+i+"_extcha' id='wl"+i+"_extcha' value='' size='20' maxlength='20'/>");

                $("#SelectedWISPDataForm").append('<div class="row show-grid">');
                $("#SelectedWISPDataForm").append('<div class="span2"><%lang("Security Mode");%></div>');
                $("#SelectedWISPDataForm").append('<div class="span4">');
                $("#SelectedWISPDataForm").append("<input type='text' name='wl"+i+"_apcli"+i+"_secmode' id='wl"+i+"_apcli"+i+"_secmode' value='' size='20' maxlength='20' readonly/>");

                $("#SelectedWISPDataForm").append('</div>');
                $("#SelectedWISPDataForm").append('</div>');

                $("#SelectedWISPDataForm").append('</div>');
                $("#SelectedWISPDataForm").append('</div>');
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
                $("#SiteSurveyMSG").append("<br><br><br><br><br><br>Searching...");
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
                    for (var i=0; i<rowsArray.length; i++ ){
                                var index = i+1;

                        rows_dataArray = rowsArray[i].split("^");
                        rows_data = {};
                        ssid = rows_dataArray[1];
                        bssid = rows_dataArray[2];
                        if(ssid==CurrentSSID && bssid==CurrentBSSID){
                            rows_data[0]="<img src='picts/web_checkmark.png'/>";
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
                        
                        if(rows_data[4]!="NONE"){
                            rows_data[8]="<img src='picts/web_securitykey.png'/>";		               
                        }else{
                            rows_data[8]=" ";	
                        }
                        if(sig>=76 && sig<=100){
                            rows_data[9]="<img src='picts/web_wavelevel4.png'/>";
                        } else if(sig>=51 && sig<=75) {
                            rows_data[9]="<img src='picts/web_wavelevel3.png'/>";
                        } else if(sig>=26 && sig<=50) {
                            rows_data[9]="<img src='picts/web_wavelevel2.png'/>";
                        } else {
                            rows_data[9]="<img src='picts/web_wavelevel1.png'/>";
                        }

                        if(CheckSavedWispData(bssid) && bssid==CurrentBSSID){
                            rows_data[10]="<input type='button' class='btn' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' disabled>&nbsp;&nbsp;"+
                                "<input type='button' class='btn' id='wl0_Forget' value='<%lang("Forget");%>' >";
                           
                        } else if(CheckSavedWispData(bssid)){
                            rows_data[10]="<input type='button' class='btn' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' >&nbsp;&nbsp;"+
                                "<input type='button' class='btn' id='wl0_Forget' value='<%lang("Forget");%>' >";
                          
                        } else {
                            rows_data[10]="<input type='button' class='btn' id='wl0_Connect_"+index+"' value='<%lang("Connect");%>' >&nbsp;&nbsp;"+
                                "<input type='button' class='btn' id='wl0_Forget' value='<%lang("Forget");%>'  disabled>"		                  

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
            
            init();
            var cnnt_rows = GetGridData();
            DrawDataTables(cnnt_rows);
            $("#hide_wan_enable_div").hide();            
             
        
             
            $("#refresh").click(function(){
                $("#SiteSurveyMSG").append("<br><br><br><br><br><br>Searching...");
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
            var ether_expand = '<% nvg_attr_get("","cnnt_rule", "0", "expand");%>';
            var len = $("#CnntListData tbody tr").length;
            for(var i=0;i<len;i++) {
                if($("#CnntListData tbody tr:eq("+i+") td:eq(0)").text() == "Ethernet") {
                    if(ether_expand == "1") {
                        var nTr = $("#CnntListData tbody tr")[i];
                        DataTable.fnOpen( nTr, fnFormatDetailsEther(nTr), 'details' );
                    }
                    else {
                        
                    }
                }
            }
            var wisp_expand = '<% nvg_attr_get("","cnnt_rule", "1", "expand");%>';
            len = $("#CnntListData tbody tr").length;
            for(var i=0;i<len;i++) {
                if($("#CnntListData tbody tr:eq("+i+") td:eq(0)").text() == "WISP") {
                    if(wisp_expand == "1") {
                        var nTr = $("#CnntListData tbody tr")[i];
   //                     DataTable.fnOpen( nTr, fnFormatDetailsWISP(nTr), 'details' );
                    }
                    else {
                        
                    }
                }
            }
            var usb_expand = '<% nvg_attr_get("","cnnt_rule", "2", "expand");%>';
            len = $("#CnntListData tbody tr").length;
            for(var i=0;i<len;i++) {
                if($("#CnntListData tbody tr:eq("+i+") td:eq(0)").text() == "USB") {
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
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        $('#pppoeModal').modal('hide');
    }

    function submit_static(F) {
        F.submit_button.value = "wan";
        F.action.value = "Apply";

        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#form_static").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
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
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);
        $('#dongleModal').modal('hide');
    }

    function to_submit_wisp(F) {
        for (var i=0; i<cnnt_rule.length; i++ ) {
            if(cnnt_rule[i][enum_cnnt_basic._NAME] == "WISP" ) {
                cnnt_rule[i][enum_cnnt_basic._CONNECTION] = 1;
            }
        }
        if (!encodekey(1)) {
                    alert('<%lang("The Key must have value.");%>');
                    return;
                }
        F.submit_button.value = "save";
        F.action.value = "Apply";        
        F.wisp_swap_rule.value = "";
        $('#SelectedWISPDataForm-modal').modal('hide');
        open_waiting_window();            
        var ajaxData;
        ajaxData = ajax_data( $("#wisp_form").serializeArray() );
        do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 6000);
    }

    </script>
    <!-- WISP Modal -->
    <div id="SelectedWISPDataForm-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">x</button>
        <h3 id="WISPLabel">WISP</h3>
        </div>
        <form name="wisp_form" id="wisp_form" action="apply.cgi" method="post">
        <script type="text/javascript">do_hidden_elements('wisp_swap_rule')</script>

            <div class="modal-body">
            	<p id="SelectedWISPDataForm"> </p>
                <!--<table class="table" id="SelectedWISPDataForm"></table>-->
            </div>
            <div class="modal-footer">
                <input type="button" class="btn btn-primary" name="wisp_connect" id="wisp_connect" onClick="to_submit_wisp(this.form)" value="<% lang("Connect"); %>" >
                <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="wisp_close" id="wisp_close" value="<% lang("Cancel"); %>">
            </div>
        </form>
    </div>
    <!-- dongle Modal -->
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
                <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
            </div>
        
        </form>
        
    </div>

 	<div id="SiteSurveyListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
        <div class="modal-body" style="height:400px;">
            <div id="SiteSurveyMSG"></div>
            <table class="table" id="SiteSurveyListData"></table>
        </div>
        <div class="modal-footer">
            <input type="button" class="btn btn-primary" name="refresh" id="refresh" value="<% lang("Refresh"); %>" >
            <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
        </div>
    </div> 
  
    <% tbl_get("cnnt_rule"); %>


    <!-- Static Modal -->
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
        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close_static" id="close_static" value="<% lang("Cancel"); %>">
    </div>
    </form>
    </div> 
 
    <!-- PPPoE Modal --> 
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
            <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close_pppoe" id="close_pppoe" value="<% lang("Cancel"); %>">
        </div>
    </form>
    </div>

	</head>

	<body>
	    	 
	  <%do_bootstrap_navbar_menu();%>


		<div class="container-fluid" id="main-container">
			<a href="#" id="menu-toggler"><span></span></a><!-- menu toggler -->

			<div id="sidebar">
				
				<div id="sidebar-shortcuts">
					<div id="sidebar-shortcuts-large">
						<button class="btn btn-small btn-success"><i class="icon-signal"></i></button>
						<button class="btn btn-small btn-info"><i class="icon-pencil"></i></button>
						<button class="btn btn-small btn-warning"><i class="icon-group"></i></button>
						<button class="btn btn-small btn-danger"><i class="icon-cogs"></i></button>
					</div>
					<div id="sidebar-shortcuts-mini">
						<span class="btn btn-success"></span>
						<span class="btn btn-info"></span>
						<span class="btn btn-warning"></span>
						<span class="btn btn-danger"></span>
					</div>
				</div><!-- #sidebar-shortcuts -->
				
                <%do_bootstrap_menu();%>
 

				<div id="sidebar-collapse"><i class="icon-double-angle-left"></i></div>


			</div><!--/#sidebar-->

		
			<div id="main-content" class="clearfix">
					
					<div id="breadcrumbs">
						<ul class="breadcrumb">
							<li><i class="icon-home"></i> <a href="#"><%lang("Home");%></a><span class="divider"><i class="icon-angle-right"></i></span></li>
							<li class="active"><%lang("Internet");%></li>
						</ul><!--.breadcrumb-->

						<div id="nav-search">
							<form class="form-search">
									<span class="input-icon">
										<input autocomplete="off" id="nav-search-input" type="text" class="input-small search-query" placeholder="Search ..." />
										<i id="nav-search-icon" class="icon-search"></i>
									</span>
							</form>
						</div><!--#nav-search-->
					</div><!--#breadcrumbs-->



					<div id="page-content" class="clearfix">
						
						<div class="page-header position-relative">
							<h1><%lang("Internet");%> <small><i class="icon-double-angle-right"></i> </small></h1>
							
						</div><!--/page-header-->

						

						<div class="row-fluid">
<!-- PAGE CONTENT BEGINS HERE -->
        <script type="text/javascript">create_waiting_window();</script>
        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('cnnt_rule')</script>
 	          	<h3 class="header smaller lighter blue"><%lang("Internet");%></h3>
      
                <table id="CnntListData" class="table table-striped  table-hover"></table>	
                    
                   
                    <%tbl_create_table("cnnt_table", "cnnt_rule", "1");%>

        </form>
        
  
                    <script type="text/javascript"><%show_copyright();%></script>
  







<!-- PAGE CONTENT ENDS HERE -->
						 </div><!--/row-->
	
					</div><!--/#page-content-->
					  

					<div id="ace-settings-container">
						<div class="btn btn-app btn-mini btn-warning" id="ace-settings-btn">
							<i class="icon-cog"></i>
						</div>
						<div id="ace-settings-box">
							<div>
								<div class="pull-left">
									<select id="skin-colorpicker" class="hidden">
										<option data-class="default" value="#438EB9">#438EB9</option>
										<option data-class="skin-1" value="#222A2D">#222A2D</option>
										<option data-class="skin-2" value="#C6487E">#C6487E</option>
										<option data-class="skin-3" value="#D0D0D0">#D0D0D0</option>
									</select>
								</div>
								<span>&nbsp; Choose Skin</span>
							</div>
							<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-header" /><label class="lbl" for="ace-settings-header"> Fixed Header</label></div>
							<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-sidebar" /><label class="lbl" for="ace-settings-sidebar"> Fixed Sidebar</label></div>
						</div>
					</div><!--/#ace-settings-container-->


			</div><!-- #main-content -->


		</div><!--/.fluid-container#main-container-->




		<a href="#" id="btn-scroll-up" class="btn btn-small btn-inverse">
			<i class="icon-double-angle-up icon-only"></i>
		</a>




	</body>
</html>
