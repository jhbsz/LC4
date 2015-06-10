<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Routing Policy</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <% tbl_get("rt_rule"); %>
    <script type="text/javascript">

        function local_rule_check() {

            if (!$('#name').val()) {
                alert('Rule name is needed.');
                return false;
            }

            if ($('#inipaddr_start').val() && !$('#inipaddr_end').val() ) {
                alert('Internal IP End is needed.');
                return false;
            }

            if (!$('#inipaddr_start').val() && $('#inipaddr_end').val() ) {
                alert('Internal IP Start is needed.');
                return false;
            }

            if (!tbl_check_valid_ipaddr_range($('#inipaddr_start').val(), $('#inipaddr_end').val(), 'Internal IP')) {
                return false;
            }

            if ( $('#extipaddr_start').val() && !$('#extipaddr_end').val() ) {
                alert('External IP End is needed.');
                return false;
            }

            if ( !$('#extipaddr_start').val() && $('#extipaddr_end').val() ) {
                alert('External IP Start is needed.');
                return false;
            }

            if (!tbl_check_valid_ipaddr_range($('#extipaddr_start').val(), $('#extipaddr_end').val(), 'External IP')) {
                return false;
            }

            if ($('#proto').val() != '') {
                if ( $('#extport_start').val() && !$('#extport_end').val() ) {
                    alert('External Port End is needed.');
                    return false;
                }

                if ( !$('#extport_start').val() && $('#extport_end').val() ) {
                    alert('External Port Start is needed.');
                    return false;
                }

                if (!tbl_check_valid_port_range($('#extport_start').val(), $('#extport_end').val(), 'External Port') ) {
                    return false;
                }
            }

            if ($('#routing_type').val() != 'default') {
                if (!$("#gateway_ipaddr").val()){
                    alert('Gateway IP is needed.');
                    return false;
                }
            }
            
            return true;
        }

        var enum_lb_routing = {
            _NAME:0,
            _ENABLE:1,
            _IN_IPADDR_START:2,
            _IN_IPADDR_END:3,
            _EXT_IPADDR_START:4,
            _EXT_IPADDR_END:5,
            _PROTOCOL:6,
            _IN_PORT_START:7,
            _IN_PORT_END:8,
            _EXT_PORT_START:9,
            _EXT_PORT_END:10,
            _EXT_IF:11,
            _ROUTING_TYPE:12,
            _GATEWAY_IPADDR:13
        }; 


        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = rt_rule.length ;
            $("<input>").attr({type: "hidden", id: "rt_rule", name: "rt_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: rt_rule[i][enum_lb_routing._NAME]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable' , value: rt_rule[i][enum_lb_routing._ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_start', name: P + i + '_inipaddr_start', value: rt_rule[i][enum_lb_routing._IN_IPADDR_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_end', name: P + i + '_inipaddr_end', value: rt_rule[i][enum_lb_routing._IN_IPADDR_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extipaddr_start', name: P + i + '_extipaddr_start', value: rt_rule[i][enum_lb_routing._EXT_IPADDR_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extipaddr_end', name: P + i + '_extipaddr_end', value: rt_rule[i][enum_lb_routing._EXT_IPADDR_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_proto', name: P + i + '_proto', value: rt_rule[i][enum_lb_routing._PROTOCOL] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inport_start', name: P + i + '_inport_start', value: rt_rule[i][enum_lb_routing._IN_PORT_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inport_end', name: P + i + '_inport_end', value: rt_rule[i][enum_lb_routing._IN_PORT_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extport_start', name: P + i + '_extport_start', value: rt_rule[i][enum_lb_routing._EXT_PORT_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extport_end', name: P + i + '_extport_end', value: rt_rule[i][enum_lb_routing._EXT_PORT_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extif', name: P + i + '_extif', value: rt_rule[i][enum_lb_routing._EXT_IF] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_routing_type', name: P + i + '_routing_type', value: rt_rule[i][enum_lb_routing._ROUTING_TYPE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_gateway_ipaddr', name: P + i + '_gateway_ipaddr', value: rt_rule[i][enum_lb_routing._GATEWAY_IPADDR] }).appendTo("form");
            }

        }

        function local_generate_rule() {

            var EnableVal = 0;
            if ($('input[name=enable]').is(':checked')){
                EnableVal = $('[name=enable]:checked').val();
            }

            var rule_tmp = Array();
            rule_tmp[enum_lb_routing._NAME]             = $('#name').val();
            rule_tmp[enum_lb_routing._ENABLE]           = EnableVal;
            rule_tmp[enum_lb_routing._IN_IPADDR_START]  = $('#inipaddr_start').val();
            rule_tmp[enum_lb_routing._IN_IPADDR_END]    = $('#inipaddr_end').val();
            rule_tmp[enum_lb_routing._EXT_IPADDR_START] = $('#extipaddr_start').val();
            rule_tmp[enum_lb_routing._EXT_IPADDR_END]   = $('#extipaddr_end').val();
            rule_tmp[enum_lb_routing._PROTOCOL]         = $("select#proto option:selected").val();
            rule_tmp[enum_lb_routing._IN_PORT_START]    = '';
            rule_tmp[enum_lb_routing._IN_PORT_END]      = '';
            rule_tmp[enum_lb_routing._EXT_PORT_START]   = $('#extport_start').val();
            rule_tmp[enum_lb_routing._EXT_PORT_END]     = $('#extport_end').val();
            rule_tmp[enum_lb_routing._EXT_IF]           = $("select#extif option:selected").val();
            rule_tmp[enum_lb_routing._ROUTING_TYPE]     = $('#routing_type').val();
            rule_tmp[enum_lb_routing._GATEWAY_IPADDR]   = $('#gateway_ipaddr').val();

            return rule_tmp;
        }

        function to_submit(F) {
            $("#submit_button").val("lb_routing");
            $("#action").val("Apply");
            local_pack_key_value_pair(F, "rt_table", "rt_rule");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        lb_objects = [ { name: 'btn_add' },
                        { name: 'btn_delete' },
                        { name: 'btn_modify' },
                        { name: 'btn_up' },
                        { name: 'btn_down' } ];

        var F;
        function init() {            
            enable_table = $('[name=rt_enable]:checked').val();
            sel_change(enable_table, 1, lb_objects);
        }

        function GetGridData(){
            var Routing_rows = [];
            var Routing_data = [];
            
            for (var i=0; i<rt_rule.length; i++ )  {
                Routing_data = [];
                Routing_data[0] = rt_rule[i][enum_lb_routing._NAME];
                Routing_data[1] = tbl_wrap_enable( rt_rule[i][enum_lb_routing._ENABLE] );
                Routing_data[2] = "<%lang("From");%>:" + rt_rule[i][enum_lb_routing._IN_IPADDR_START] + "<br><%lang("To");%>:" + rt_rule[i][enum_lb_routing._IN_IPADDR_END];
                Routing_data[3] = "<%lang("From");%>:" + rt_rule[i][enum_lb_routing._EXT_IPADDR_START] + "<br><%lang("To");%>:" + rt_rule[i][enum_lb_routing._EXT_IPADDR_END];
                Routing_data[4] = tbl_wrap_proto(rt_rule[i][enum_lb_routing._PROTOCOL]);
                Routing_data[5] = "<%lang("From");%>:" + rt_rule[i][enum_lb_routing._EXT_PORT_START] + "<br><%lang("To");%>:" + rt_rule[i][enum_lb_routing._EXT_PORT_END];
                Routing_data[6] = tbl_wrap_wan( rt_rule[i][enum_lb_routing._EXT_IF] );
                Routing_data[7] = rt_rule[i][enum_lb_routing._ROUTING_TYPE];
                Routing_data[8] = rt_rule[i][enum_lb_routing._GATEWAY_IPADDR];
                
                Routing_rows[i] = Routing_data;
            }
            return Routing_rows;
        }

        function CreateRoutingDataForm(){
            $("select#routing_type").live("change", function() {
                if ($(this).val()=="default") {
                    $("#gateway_ipaddr").attr("disabled",true);
                    $("#gateway_ipaddr").attr("value", "");
                }else{
                    $("#gateway_ipaddr").attr("disabled",false);
                }
            });

            $("select#proto").live("change", function() {
                if ($(this).val()=="") {
                    $("#extport_start").attr("disabled",true);
                    $("#extport_start").attr("value", "");
                    $("#extport_end").attr("disabled",true);
                    $("#extport_end").attr("value", "");
                }else{
                    $("#extport_start").attr("disabled",false);
                    $("#extport_end").attr("disabled",false);
                }
            });

            $("#RoutingDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input type='text' name='name' id='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)'></td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' checked /></td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("Internal IP Range");%>:</td><td>"+
                                         "<%lang("From");%>:<input type='text' class='input-medium' id='inipaddr_start' name='inipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range Start\")' />"+
                                         "<%lang("To");%>:<input type='text' class='input-medium' id='inipaddr_end' name='inipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range End\")' />"+
                                         "</td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("External IP Range");%>:</td><td>"+
                                         "<%lang("From");%>:<input type='text' class='input-medium' id='extipaddr_start' name='extipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range Start\")' />"+
                                         "<%lang("To");%>:<input type='text' class='input-medium' id='extipaddr_end' name='extipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range End\")' />"+
                                         "</td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("Protocol");%>:</td><td>"+
                                         "<select id='proto' name='proto'>"+
                                         "  <option value=''>*</option><option value='tcp'>TCP</option><option value='udp'>UDP</option><option value='both'>TCP/UDP</option>"+
                                         "</select>"+
                                         "</td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("Service Port Range");%>:</td><td>"+
                                         "<%lang("From");%>:<input type='text' class='input-medium' id='extport_start' name='extport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' disabled='true' />"+
                                         "<%lang("To");%>:<input type='text' class='input-medium' id='extport_end' name='extport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' disabled='true' />"+
                                         "</td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("External Interface");%>:</td><td>"+
                                         "<select id='extif' name='extif' >"+
                                         "</select>"+
                                         "<td></td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("Routing Gateway");%>:</td><td>"+
                                         "<select id='routing_type' name='routing_type' \>"+
                                         "  <option value='default'><%lang("Default Gateway");%></option>"+
                                         "  <option value='static'><%lang("Static Gateway");%></option>"+
                                         "</select>"+
                                         "</td></tr>");
            $("#RoutingDataForm").append("<tr><td align='right'><%lang("Gateway IP Address");%>:</td><td>"+
                                         "<input type='text' id='gateway_ipaddr' name='gateway_ipaddr' size='17' maxlength='15' disabled='true' onblur='valid_ipaddr(this, \"Gateway IP Address\")' /></td></tr>");

            for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
                $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
            }
            for (var i = 1; i <= <%nv_get("lan_num");%>; i++) {
                $('#extif').append(new Option("LAN" + i , "lan" + (i-1), false, false));
            }

        }

        var selectID=null;
        $(document).ready(function(){
            init();
            var Routing_rows = GetGridData();
            DrawDataTables(Routing_rows);

             $('#btn_add').click(function(){
                $("#RoutingDataForm").empty();
                CreateRoutingDataForm();

                 $("#add").show();
                 $("#modify").hide();
                 $('#RoutingListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                rt_rule.push(Array_Tmp);
                var Routing_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(Routing_rows);
                $('#RoutingListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#RoutingDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    rt_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });


             $("#btn_modify").click(function(){
                 $("#RoutingDataForm").empty();
                 CreateRoutingDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                $("#name").val(rt_rule[selectID][enum_lb_routing._NAME]);
                if (rt_rule[selectID][enum_lb_routing._ENABLE] == 1){
                    $("#enable").attr('checked',true);
                }else{
                    $("#enable").attr('checked',false);
                }
                $("#inipaddr_start").val(rt_rule[selectID][enum_lb_routing._IN_IPADDR_START]);
                $("#inipaddr_end").val(rt_rule[selectID][enum_lb_routing._IN_IPADDR_END]);
                $("#extipaddr_start").val(rt_rule[selectID][enum_lb_routing._EXT_IPADDR_START]);
                $("#extipaddr_end").val(rt_rule[selectID][enum_lb_routing._EXT_IPADDR_END]);
                $("select[name='proto']").attr("value", rt_rule[selectID][enum_lb_routing._PROTOCOL]);
                if (rt_rule[selectID][enum_lb_routing._PROTOCOL] == ""){
                    $("#extport_start").attr("disabled",true);
                    $("#extport_end").attr("disabled",true);
                }else{
                    $("#extport_start").attr("disabled",false);
                    $("#extport_end").attr("disabled",false);
                }
                $("#extport_start").val(rt_rule[selectID][enum_lb_routing._EXT_PORT_START]);
                $("#extport_end").val(rt_rule[selectID][enum_lb_routing._EXT_PORT_END])
                $("select[name='extif']").attr("value", rt_rule[selectID][enum_lb_routing._EXT_IF]);
                $("select[name='routing_type']").attr("value", rt_rule[selectID][enum_lb_routing._ROUTING_TYPE]);
                if (rt_rule[selectID][enum_lb_routing._ROUTING_TYPE]=="default"){
                    $("#gateway_ipaddr").attr("disabled",true);
                }else{
                    $("#gateway_ipaddr").attr("disabled",false);
                }
                $("#gateway_ipaddr").val(rt_rule[selectID][enum_lb_routing._GATEWAY_IPADDR]);

                 $('#RoutingListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 rt_rule.splice(selectID,1, Array_Tmp);

                 var Routing_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Routing_rows);
                 $('#RoutingListData-modal').modal('hide');
             });

             $("#btn_up").click(function(){
                 $("#RoutingDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == 0){
                         return false;
                     }
                 }
                 var arr_tmp = rt_rule[selectID];
                 rt_rule[selectID] = rt_rule[selectID - 1];
                 rt_rule[selectID - 1] = arr_tmp;
                 selectID = selectID - 1;
                 if ( selectID<0 ){
                    selectID = 0;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });

             $("#btn_down").click(function(){
                 $("#RoutingDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == rt_rule.length-1){
                         return false;
                     }
                 }

                 var arr_tmp = rt_rule[selectID];
                 rt_rule[selectID] = rt_rule[selectID + 1];
                 rt_rule[selectID + 1] = arr_tmp;
                 selectID = selectID + 1;
                 if ( selectID>rt_rule.length-1 ){
                     selectID = rt_rule.length-1;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });

        });//End Ready


        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }


        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#RoutingListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "<%lang("Rule Name");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Enable");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Internal IP Range");%>" },
                    { "sTitle": "<%lang("External IP Range");%>" },
                    { "sTitle": "<%lang("Protocol");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Service Port Range");%>" },
                    { "sTitle": "<%lang("External Interface");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Routing Type");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Gateway");%>", "sClass":"center" },
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

            $("#RoutingListData tbody tr").click( function() {  // selete a row data
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
 	          <script type="text/javascript">do_hidden_elements();</script>
                <h2><%lang("Setup");%>-<%lang("Routing");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Routing");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Routing"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="rt_enable" onclick="init()"
                                  <%nv_match("rt_enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="rt_enable" onclick="init()"
                                  <%nv_match("rt_enable","0","checked");%>><%lang("Disable");%>
                        </label>

                    </div>
                </div>
            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="RoutingListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-body">
                        <table class="table" id="RoutingDataForm"></table>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>


                <table class="table table-bordered table-striped" id="RoutingListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                    <input type="button" class="btn" id="btn_up" name="btn_up" value="<% lang("Up"); %>">
                    <input type="button" class="btn" id="btn_down" name="btn_down" value="<% lang("Down"); %>">
                </div>

              <%tbl_create_table("rt_table", "rt_rule", "1");%>
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