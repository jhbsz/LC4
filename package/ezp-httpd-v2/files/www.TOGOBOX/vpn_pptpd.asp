<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - VPN / PPTP</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <% tbl_get("pptpd_user_rule"); %>
    <script type="text/javascript">
        var pptpd_objects =
        [ { name: 'pptpd_mtu' },
          { name: 'pptpd_remoteip' },
          { name: 'pptpd_remoteip_range' },
          { name: 'pptpd_dns_auto_enable' },
          { name: 'pptpd_dns' },
          { name: 'pptpd_chap_enable' },
          { name: 'pptpd_mschap_enable' },
          { name: 'pptpd_mschapv2_enable' },
          { name: 'pptpd_mppe128_enable' },
          { name: 'pptpd_proxyarp_enable' },
          { name: 'pptpd_nat_enable' },
          { name: 'btn_add' },
          { name: 'btn_delete' },
          { name: 'btn_modify' }
        ];

        var F; 
        function local_rule_check(T) {
            if ((!$('#username').val())) {
                alert('User name is required.');
                return false;
            }

            if (!$('#passwd').val()) {
                alert('Password is required.');
                return false;
            }

            return true;
        }

        function UsernameCheck(Username) {
            for (var i=0; i<pptpd_user_rule.length; i++){
                if ( Username == pptpd_user_rule[i][enum_pptp._NAME] ){
                    alert("'"+Username +"' repeats.");
                    return false;
                }
            }
            return true
        }

        var enum_pptp = {                                                                                                          
            _ENABLE:0,                                                                                                             
            _NAME:1,                                                                                                               
            _PROVIDER:2,                                                                                                           
            _PASSWD:3,                                                                                                             
            _IPADDR:4                                                                                                              
        } 


        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = pptpd_user_rule.length ;
            $("<input>").attr({type: "hidden", id: "pptpd_rule", name: "pptpd_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: pptpd_user_rule[i][enum_pptp._ENABLE]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_username', name: P + i + '_username' , value: pptpd_user_rule[i][enum_pptp._NAME] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_provider', name: P + i + '_provider' , value: pptpd_user_rule[i][enum_pptp._PROVIDER] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_passwd', name: P + i + '_passwd' , value: pptpd_user_rule[i][enum_pptp._PASSWD] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_ipaddr', name: P + i + '_ipaddr' , value: pptpd_user_rule[i][enum_pptp._IPADDR] }).appendTo("form");
            }
        }

        function local_generate_rule(T) {
            var rule_tmp = Array();

            var enable = 0;
            if ($('input[name=enable]').is(':checked')){
                enable = $('[name=enable]:checked').val();
            }

            rule_tmp[enum_pptp._ENABLE]   = enable;
            rule_tmp[enum_pptp._NAME]     = $('#username').val();
            rule_tmp[enum_pptp._PROVIDER] = '';
            rule_tmp[enum_pptp._PASSWD]   = $('#passwd').val();
            rule_tmp[enum_pptp._IPADDR]   = '';

            return rule_tmp;
        }

        function to_submit(F) {
            $("#submit_button").val("vpn_pptpd");
            $("#action").val("Apply");
            local_pack_key_value_pair(F, "pptpd_table", "pptpd_user_rule");

            sel_change(1, 1, pptpd_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }


        function init() {
            enable_table = $('[name=pptpd_enable]:checked').val();
            sel_change(enable_table, 1, pptpd_objects);

            if ($('[name=pptpd_dns_auto_enable]:checked').val()==1 && enable_table == 1){
                $("#pptpd_dns").attr("disabled",false);
            }else{
                $("#pptpd_dns").attr("disabled",true);
            }
        }

        function GetGridData(){
            var PPTP_rows = [];
            var PPTP_data = [];

            for (var i=0; i<pptpd_user_rule.length; i++ )  {
                PPTP_data = [];
                PPTP_data[0] = tbl_wrap_enable( pptpd_user_rule[i][enum_pptp._ENABLE] );
                PPTP_data[1] = pptpd_user_rule[i][enum_pptp._NAME];
                PPTP_data[2] = pptpd_user_rule[i][enum_pptp._PASSWD];
                PPTP_rows[i] = PPTP_data;
            }
            return PPTP_rows;
        }

        function CreatePPTPDataForm(){
            $("#PPTPDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
            $("#PPTPDataForm").append("<tr><td align='right'><%lang("User Name");%>:</td><td><input id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/></td></tr>");
            $("#PPTPDataForm").append("<tr><td align='right'><%lang("Password");%>:</td><td><input id='passwd' name='passwd' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/></td></tr>");
        }

        var selectID=null;
        $(document).ready(function(){
            init();
            var PPTP_rows = GetGridData();
            DrawDataTables(PPTP_rows);

             $('#btn_add').click(function(){
                $("#PPTPDataForm").empty();
                CreatePPTPDataForm();

                 $("#add").show();
                 $("#modify").hide();
                 $('#PPTPListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                pptpd_user_rule.push(Array_Tmp);
                var PPTP_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(PPTP_rows);
                $('#PPTPListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#PPTPDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    pptpd_user_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });

             $("#btn_modify").click(function(){
                 $("#PPTPDataForm").empty();
                 CreatePPTPDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                if (pptpd_user_rule[selectID][enum_pptp._ENABLE] == 1){
                    $("#enable").attr('checked',true);
                }else{
                    $("#enable").attr('checked',false);
                }
                $("#username").val(pptpd_user_rule[selectID][enum_pptp._NAME]);
                $("#passwd").val(pptpd_user_rule[selectID][enum_pptp._PASSWD]);

                 $('#PPTPListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 pptpd_user_rule.splice(selectID,1, Array_Tmp);

                var PPTP_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(PPTP_rows);
                 $('#PPTPListData-modal').modal('hide');
             });

        });//End ready

        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }

        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#PPTPListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "<%lang("Rule Enable");%>", "sClass":"center" },
                    { "sTitle": "<%lang("User Name");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Password");%>", "sClass":"center" },
                ],
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

            $("#PPTPListData tbody tr").click( function() {  // selete a row data
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

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('pptpd_rule');</script>
                <h2><%lang("Security");%>-<%lang("VPN / PPTP");%></h2>
              <input type="hidden" name="pptpd_name" id="pptpd_name" 
                    value='<%nvg_attr_get("pptpd_name", "pptpd_rule","0","name");%>' />
              <input type="hidden" name="pptpd_redialperiod" id="pptpd_redialperiod" 
                    value='<%nvg_attr_get("pptpd_redialperiod", "pptpd_rule","0","redialperiod");%>' />
              <input type="hidden" name="pptpd_localip" id="pptpd_localip" 
                    value='<%nvg_attr_get("pptpd_localip", "pptpd_rule","0","localip");%>' />
              <input type="hidden" name="pptpd_localip_range" id="pptpd_localip_range" 
                    value='<%nvg_attr_get("pptpd_localip_range", "pptpd_rule","0","localip_range");%>' />
              <input type="hidden" name="pptpd_eap_enable" id="pptpd_eap_enable" 
                    value='<%nvg_attr_get("pptpd_eap_enable", "pptpd_rule","0","eap_enable");%>' />
              <input type="hidden" name="pptpd_pap_enable" id="pptpd_pap_enable" 
                    value='<%nvg_attr_get("pptpd_pap_enable", "pptpd_rule","0","pap_enable");%>' />


        <div class="row">
            <div class="span12">
                <legend><%lang("PPTP");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("PPTP"); %></div>
                    <div class="span8">

                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_enable" id="pptpd_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_enable" id="pptpd_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("MTU");%></div>
                    <div class="span8">
                        <input type="text" name="pptpd_mtu" id="pptpd_mtu" size="4" maxLength="4" 
                          value='<%nvg_attr_get("pptpd_mtu", "pptpd_rule", "0", "mtu");%>'
                          onblur="valid_number(this, 'PPTP MTU')" /> <%lang("Bytes");%>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("VPN Start IP Address");%></div>
                    <div class="span8">
                        <input type="text" name="pptpd_remoteip" id="pptpd_remoteip" size="20" maxLength="15" 
                          value='<%nvg_attr_get("pptpd_remoteip", "pptpd_rule", "0", "remoteip");%>'
                          onblur="valid_ipaddr(this, 'PPTP Local IP')" />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Max VPN Clients");%></div>
                    <div class="span8">
                        <input type="text" size="3" maxLength="3" name="pptpd_remoteip_range" id="pptpd_remoteip_range" 
                              value='<%nvg_attr_get("pptpd_remoteip_range", "pptpd_rule", "$0", "remoteip_range");%>'
                              onblur='valid_number(this, "PPTP Remote IP Range")' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Auto DNS"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_dns_auto_enable" id="pptpd_dns_auto_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","1","checked");%>>
                                <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_dns_auto_enable" id="pptpd_dns_auto_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","0","checked");%>>
                                <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("DNS");%></div>
                    <div class="span8">
                        <input type="text" maxLength="15" size="20" name="pptpd_dns" id="pptpd_dns"
                          value='<%nvg_attr_get("pptpd_dns", "pptpd_rule", "$0", "dns");%>'
                          onblur='valid_reserved_word(this, "PPTP DNS")' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("CHAP Enable"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_chap_enable" id="pptpd_chap_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","1","checked");%>>
                                <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_chap_enable" id="pptpd_chap_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","0","checked");%>>
                                <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("MSCHAP Enable"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_mschap_enable" id="pptpd_mschap_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","1","checked");%>>
                                <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_mschap_enable" id="pptpd_mschap_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","0","checked");%>>
                                <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("MSCHAP v2 Enable"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_mschapv2_enable" id="pptpd_mschapv2_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","1","checked");%>>
                                <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_mschapv2_enable" id="pptpd_mschapv2_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","0","checked");%>>
                                <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("MPPE128 Enable"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_mppe128_enable" id="pptpd_mppe128_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","1","checked");%>>
                                <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_mppe128_enable" id="pptpd_mppe128_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","0","checked");%>>
                                <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Proxy ARP Enable"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_proxyarp_enable" id="pptpd_proxyarp_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","1","checked");%>>
                                <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_proxyarp_enable" id="pptpd_proxyarp_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","0","checked");%>>
                                <%lang("Disable");%>
                        </label>
                    </div>
                </div>


                <div class="row show-grid">
                    <div class="span4"><% lang("NAT Enable"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="pptpd_nat_enable" id="pptpd_nat_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","1","checked");%>>
                                <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="pptpd_nat_enable" id="pptpd_nat_enable" onclick="init()"
                                <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","0","checked");%>>
                                <%lang("Disable");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="PPTPListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-body">
                        <table class="table" id="PPTPDataForm"></table>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>


                <table class="table table-bordered table-striped" id="PPTPListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                </div>

                <%tbl_create_table("wf_table", "wf_rule", "1");%>
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