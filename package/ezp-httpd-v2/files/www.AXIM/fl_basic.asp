<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - ACL</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <% tbl_get("fl_rule"); %>
    <script type="text/javascript">

        var F;

        <%prod_show_js_start("PROD_CAT_B|PROD_CAT_L|PROD_CAT_M", "1", "7");%>
        var action_objects =
        [ { id: 'dbm_enable' }
        ];
        <%prod_show_js_end("PROD_CAT_B|PROD_CAT_L|PROD_CAT_M", "1", "7");%>


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
                if ( !$('#extport_start').val() || !$('#extport_end').val() ) {
                    alert('External Port Start and End are needed.');
                    return false;
                }
            }
            
            return true;

        }

        var enum_fl_basic = {
            _NAME:0,
            _ENABLE:1,                                                                                                               
            _EXT_IF:2,                                                                                                           
            _IN_IF:3,                                                                                                             
            _IN_IPADDR_START:4,
            _IN_IPADDR_END:5,
            _EXT_IPADDR_START:6,
            _EXT_IPADDR_END:7,
            _PROTOCOL:8,
            _IN_PORT_START:9,
            _IN_PORT_END:10,
            _EXT_PORT_START:11,
            _EXT_PORT_END:12,
            _ACTION:13,
            _DBM_ENABLE:14,
            _DBM_NUM:15,
            _MARK:16,
            _BW_IDX:17,
            _SCHED_IDX:18
        } 

        //var default_rule = ['','1','wan0','','','','','','','','','','','1','0','','','',''];

        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = fl_rule.length ;
            $("<input>").attr({type: "hidden", id: "fl_rule", name: "fl_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: fl_rule[i][enum_fl_basic._NAME]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable' , value: fl_rule[i][enum_fl_basic._ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extif', name: P + i + '_extif', value: fl_rule[i][enum_fl_basic._EXT_IF] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inif', name: P + i + '_inif', value: fl_rule[i][enum_fl_basic._IN_IF] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_start', name: P + i + '_inipaddr_start', value: fl_rule[i][enum_fl_basic._IN_IPADDR_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_end', name: P + i + '_inipaddr_end', value: fl_rule[i][enum_fl_basic._IN_IPADDR_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extipaddr_start', name: P + i + '_extipaddr_start', value: fl_rule[i][enum_fl_basic._EXT_IPADDR_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extipaddr_end', name: P + i + '_extipaddr_end', value: fl_rule[i][enum_fl_basic._EXT_IPADDR_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_proto', name: P + i + '_proto', value: fl_rule[i][enum_fl_basic._PROTOCOL] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inport_start', name: P + i + '_inport_start', value: fl_rule[i][enum_fl_basic._IN_PORT_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inport_end', name: P + i + '_inport_end', value: fl_rule[i][enum_fl_basic._IN_PORT_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extport_start', name: P + i + '_extport_start', value: fl_rule[i][enum_fl_basic._EXT_PORT_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extport_end', name: P + i + '_extport_end', value: fl_rule[i][enum_fl_basic._EXT_PORT_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_op', name: P + i + '_op', value: fl_rule[i][enum_fl_basic._ACTION] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_dbm_enable', name: P + i + '_dbm_enable', value: fl_rule[i][enum_fl_basic._DBM_ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_dbm_num', name: P + i + '_dbm_num', value: fl_rule[i][enum_fl_basic._DBM_NUM] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_mark', name: P + i + '_mark', value: fl_rule[i][enum_fl_basic._MARK] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_bw_idx', name: P + i + '_bw_idx', value: fl_rule[i][enum_fl_basic._BW_IDX] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_sched_idx', name: P + i + '_sched_idx', value: fl_rule[i][enum_fl_basic._SCHED_IDX] }).appendTo("form");
            }
        }

        function local_generate_rule() {
            var EnableVal = 0;
            if ($('input[name=enable]').is(':checked')){
                EnableVal = $('[name=enable]:checked').val();
            }

            var rule_tmp = Array();
            rule_tmp[enum_fl_basic._NAME]             = $("#name").val();
            rule_tmp[enum_fl_basic._ENABLE]           = EnableVal;
            rule_tmp[enum_fl_basic._EXT_IF]           = $("select#extif option:selected").val();
            rule_tmp[enum_fl_basic._IN_IF]            = '';
            rule_tmp[enum_fl_basic._IN_IPADDR_START]  = $("#inipaddr_start").val();
            rule_tmp[enum_fl_basic._IN_IPADDR_END]    = $("#inipaddr_end").val();
            rule_tmp[enum_fl_basic._EXT_IPADDR_START] = $("#extipaddr_start").val();
            rule_tmp[enum_fl_basic._EXT_IPADDR_END]   = $("#extipaddr_end").val();
            rule_tmp[enum_fl_basic._PROTOCOL]         = $("select#proto option:selected").val();
            rule_tmp[enum_fl_basic._IN_PORT_START]    = '';
            rule_tmp[enum_fl_basic._IN_PORT_END]      = '';
            rule_tmp[enum_fl_basic._EXT_PORT_START]   = $("#extport_start").val();
            rule_tmp[enum_fl_basic._EXT_PORT_END]     = $("#extport_end").val();
            rule_tmp[enum_fl_basic._ACTION]           = $("select#op option:selected").val();
            rule_tmp[enum_fl_basic._DBM_ENABLE]       = $("#dbm_enable").val();
            rule_tmp[enum_fl_basic._DBM_NUM]          = '';
            rule_tmp[enum_fl_basic._MARK]             = '';
            rule_tmp[enum_fl_basic._BW_IDX]           = '';
            rule_tmp[enum_fl_basic._SCHED_IDX]        = '';

            return rule_tmp;
        }

        function to_submit(F) {
            $("#submit_button").val("fl_basic");
            $("#action").val("Apply");
            local_pack_key_value_pair(F, "fl_table", "fl_rule");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        fl_objects = [  { name: 'fl_rule_default' },
                        { name: 'btn_add' },
                        { name: 'btn_delete' },
                        { name: 'btn_modify' },
                        { name: 'btn_up' },
                        { name: 'btn_down' } ];


        function init() {
            enable_table = $('[name=fl_enable]:checked').val();
            sel_change(enable_table, 1, fl_objects);
        }

        function GetGridData(){
            var ACL_rows = [];
            var ACL_data = [];
            
            for (var i=0; i<fl_rule.length; i++ )  {
                ACL_data = [];
                ACL_data[0] = fl_rule[i][enum_fl_basic._NAME];
                ACL_data[1] = tbl_wrap_enable( fl_rule[i][enum_fl_basic._ENABLE] );
                ACL_data[2] = tbl_wrap_wan( fl_rule[i][enum_fl_basic._EXT_IF] );
                ACL_data[3] = "<%lang("From");%>:" + fl_rule[i][enum_fl_basic._IN_IPADDR_START] + "<br><%lang("To");%>:" + fl_rule[i][enum_fl_basic._IN_IPADDR_END];
                ACL_data[4] = "<%lang("From");%>:" + fl_rule[i][enum_fl_basic._EXT_IPADDR_START] + "<br><%lang("To");%>:" + fl_rule[i][enum_fl_basic._EXT_IPADDR_END];
                ACL_data[5] = tbl_wrap_proto(fl_rule[i][enum_fl_basic._PROTOCOL]);
                ACL_data[6] = "<%lang("From");%>:" + fl_rule[i][enum_fl_basic._EXT_PORT_START] + "<br><%lang("To");%>:" + fl_rule[i][enum_fl_basic._EXT_PORT_END];
                ACL_data[7] = tbl_wrap_action(fl_rule[i][enum_fl_basic._ACTION]);
                
                ACL_rows[i] = ACL_data;
            }
            return ACL_rows;
        }

        function CreateACLDataForm(){

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

            $("#ACLDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input type='text' name='name' id='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)'></td></tr>");
            $("#ACLDataForm").append("<tr><td align='right'><%lang("Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' checked /></td></tr>");
            $("#ACLDataForm").append("<tr><td align='right'><%lang("External Interface");%>:</td><td>"+
                                         "<select id='extif' name='extif' >"+
                                         "</select>"+
                                         "<td></td></tr>");
            $("#ACLDataForm").append("<tr><td align='right'><%lang("Internal IP Range");%>:</td><td>"+
                                         "<%lang("From");%>:<input type='text' class='input-medium' id='inipaddr_start' name='inipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range Start\")' />"+
                                         "<%lang("To");%>:<input type='text' class='input-medium' id='inipaddr_end' name='inipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range End\")' />"+
                                         "</td></tr>");
            $("#ACLDataForm").append("<tr><td align='right'><%lang("External IP Range");%>:</td><td>"+
                                         "<%lang("From");%>:<input type='text' class='input-medium' id='extipaddr_start' name='extipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range Start\")' />"+
                                         "<%lang("To");%>:<input type='text' class='input-medium' id='extipaddr_end' name='extipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range End\")' />"+
                                         "</td></tr>");
            $("#ACLDataForm").append("<tr><td align='right'><%lang("Protocol");%>:</td><td>"+
                                         "<select id='proto' name='proto'>"+
                                         "  <option value=''>*</option><option value='tcp'>TCP</option><option value='udp'>UDP</option><option value='both'>TCP/UDP</option>"+
                                         "</select>"+
                                         "</td></tr>");
            $("#ACLDataForm").append("<tr><td align='right'><%lang("Service Port Range");%>:</td><td>"+
                                         "<%lang("From");%>:<input type='text' class='input-medium' id='extport_start' name='extport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' disabled='true' />"+
                                         "<%lang("To");%>:<input type='text' class='input-medium' id='extport_end' name='extport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' disabled='true' />"+
                                         "</td></tr>");
            $("#ACLDataForm").append("<tr><td align='right'><%lang("Action");%>:</td><td>"+
                                         "<select id='op' name='op' \>"+
                                         "  <option value='1'><%lang("ALLOW");%></option>"+
                                         "  <option value='0'><%lang("DENY");%></option>"+
                                         "</select>"+
                                         "</td></tr>");
            $("#ACLDataForm").append("<input type='hidden' name='dbm_enable' id='dbm_enable' value='0'>");

            for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
                $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
            }

        }

        var selectID=null;
        $(document).ready(function(){
            init();
            var ACL_rows = GetGridData();
            DrawDataTables(ACL_rows);

             $('#btn_add').click(function(){
                $("#ACLDataForm").empty();
                CreateACLDataForm();

                 $("#add").show();
                 $("#modify").hide();
                 $('#ACLListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                fl_rule.push(Array_Tmp);
                var ACL_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(ACL_rows);
                $('#ACLListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#ACLDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    fl_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });


             $("#btn_modify").click(function(){
                 $("#ACLDataForm").empty();
                 CreateACLDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                 $("#name").val(fl_rule[selectID][enum_fl_basic._NAME]);
                 if (fl_rule[selectID][enum_fl_basic._ENABLE] == 1){
                     $("#enable").attr('checked',true);
                 }else{
                     $("#enable").attr('checked',false);
                 }
                 $("select[name='extif']").attr("value", fl_rule[selectID][enum_fl_basic._EXT_IF]);
                 $("#inipaddr_start").val(fl_rule[selectID][enum_fl_basic._IN_IPADDR_START]);
                 $("#inipaddr_end").val(fl_rule[selectID][enum_fl_basic._IN_IPADDR_END]);
                 $("#extipaddr_start").val(fl_rule[selectID][enum_fl_basic._EXT_IPADDR_START]);
                 $("#extipaddr_end").val(fl_rule[selectID][enum_fl_basic._EXT_IPADDR_END]);
                 $("select[name='proto']").attr("value", fl_rule[selectID][enum_fl_basic._PROTOCOL]);
                 if (fl_rule[selectID][enum_fl_basic._PROTOCOL] == ""){
                     $("#extport_start").attr("disabled",true);
                     $("#extport_end").attr("disabled",true);
                 }else{
                     $("#extport_start").attr("disabled",false);
                     $("#extport_end").attr("disabled",false);
                 }
                 $("#extport_start").val(fl_rule[selectID][enum_fl_basic._EXT_PORT_START]);
                 $("#extport_end").val(fl_rule[selectID][enum_fl_basic._EXT_PORT_END])
                 $("select[name='op']").attr("value", fl_rule[selectID][enum_fl_basic._ACTION]);

                 $('#ACLListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 fl_rule.splice(selectID,1, Array_Tmp);

                var ACL_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(ACL_rows);
                 $('#ACLListData-modal').modal('hide');
             });

             $("#btn_up").click(function(){
                 $("#ACLDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == 0){
                         return false;
                     }
                 }
                 var arr_tmp = fl_rule[selectID];
                 fl_rule[selectID] = fl_rule[selectID - 1];
                 fl_rule[selectID - 1] = arr_tmp;
                 selectID = selectID - 1;
                 if ( selectID<0 ){
                    selectID = 0;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });

             $("#btn_down").click(function(){
                 $("#ACLDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == fl_rule.length-1){
                         return false;
                     }
                 }

                 var arr_tmp = fl_rule[selectID];
                 fl_rule[selectID] = fl_rule[selectID + 1];
                 fl_rule[selectID + 1] = arr_tmp;
                 selectID = selectID + 1;
                 if ( selectID>fl_rule.length-1 ){
                     selectID = fl_rule.length-1;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });

        });//End ready

        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }

        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#ACLListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "<%lang("Rule Name");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Rule Enable");%>", "sClass":"center" },
                    { "sTitle": "<%lang("External Interface");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Internal IP Range");%>" },
                    { "sTitle": "<%lang("External IP Range");%>" },
                    { "sTitle": "<%lang("Protocol");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Service Port Range");%>" },
                    { "sTitle": "<%lang("Action");%>", "sClass":"center" }
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

            $("#ACLListData tbody tr").click( function() {  // selete a row data
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
 	          <script type="text/javascript">do_hidden_elements('');</script>
                <h2><%lang("Security");%>-<%lang("Access Control");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Access Control List");%> (ACL)</legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Access Control"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                          <input type="radio" value="1" name="fl_enable" onclick="init()"
                                <%nv_match("fl_enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                          <input type="radio" value="0" name="fl_enable" onclick="init()"
                                <%nv_match("fl_enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Default Access Control Action"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="fl_rule_default" id="fl_rule_default"
                                <%nv_match("fl_rule_default","1","checked");%>><%lang("ALLOW");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="fl_rule_default" id="fl_rule_default"
                                <%nv_match("fl_rule_default","0","checked");%>><%lang("DENY");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->

        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="ACLListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-body">
                        <table class="table" id="ACLDataForm"></table>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>


                <table class="table table-bordered table-striped" id="ACLListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                    <input type="button" class="btn" id="btn_up" name="btn_up" value="<% lang("Up"); %>">
                    <input type="button" class="btn" id="btn_down" name="btn_down" value="<% lang("Down"); %>">
                </div>

                <%tbl_create_table("fl_table", "fl_rule", "1");%>
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