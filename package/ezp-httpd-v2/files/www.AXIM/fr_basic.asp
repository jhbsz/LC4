<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Port Forwarding</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <script type="text/javascript">
        var dmz_objects = new Array(<%nv_get("wan_num");%>);
        fun_objects = [ { name: 'btn_add' },
                        { name: 'btn_delete' },
                        { name: 'btn_modify' },
                        { name: 'btn_up' },
                        { name: 'btn_down' } ];

        function local_rule_check(T) {
            if (!$('#name').val()) {
                alert('Name is needed.');
                return false;
            }

            if (!$('#extport_start').val()) {
                alert('External Port Start is needed.');
                return false;
            }

            if ( !$('#extport_end').val() ) {
                alert('External Port End is needed.');
                return false;
            }

            if (!tbl_check_valid_port_range($('#extport_start').val(), $('#extport_end').val(), 'External Port')) {
                return false;
            }

            if (!$('#ipaddr').val()) {
                alert('Please specify a IP.');
                return false;
            }
                
            return true;
        }

        var enum_fr_basic = {
            _NAME:0,
            _ENABLE:1,
            _EXT_IF:2,
            _PROTOCOL:3,
            _EXT_PORT_START:4,
            _EXT_PORT_END:5,
            _IP_ADDR:6,
            _IN_PORT_START:7,
            _IN_PORT_END:8
        } 

        var rule; 

        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = fr_rule.length ;
            $("<input>").attr({type: "hidden", id: "fr_rule", name: "fr_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: fr_rule[i][enum_fr_basic._NAME]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable' , value: fr_rule[i][enum_fr_basic._ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extif', name: P + i + '_extif', value: fr_rule[i][enum_fr_basic._EXT_IF] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_proto', name: P + i + '_proto', value: fr_rule[i][enum_fr_basic._PROTOCOL] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extport_start', name: P + i + '_extport_start', value: fr_rule[i][enum_fr_basic._EXT_PORT_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_extport_end', name: P + i + '_extport_end', value: fr_rule[i][enum_fr_basic._EXT_PORT_END] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_ipaddr', name: P + i + '_ipaddr', value: fr_rule[i][enum_fr_basic._IP_ADDR] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inport_start', name: P + i + '_inport_start', value: fr_rule[i][enum_fr_basic._IN_PORT_START] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_inport_end', name: P + i + '_inport_end', value: fr_rule[i][enum_fr_basic._IN_PORT_END] }).appendTo("form");
            }
        }

        function local_generate_rule(T) {

            var EnableVal = 0;
            if ($('input[name=enable]').is(':checked')){
                EnableVal = $('[name=enable]:checked').val();
            }

            var rule_tmp = Array();

            rule_tmp[enum_fr_basic._NAME]           = $('#name').val();
            rule_tmp[enum_fr_basic._ENABLE]         = EnableVal;
            rule_tmp[enum_fr_basic._EXT_IF]         = $('#extif').val();
            rule_tmp[enum_fr_basic._PROTOCOL]       = $('#proto').val();
            rule_tmp[enum_fr_basic._EXT_PORT_START] = $('#extport_start').val();
            rule_tmp[enum_fr_basic._EXT_PORT_END]   = $('#extport_end').val();
            rule_tmp[enum_fr_basic._IP_ADDR]        = $('#ipaddr').val();
            rule_tmp[enum_fr_basic._IN_PORT_START]  = $('#inport_start').val();
            rule_tmp[enum_fr_basic._IN_PORT_END]    = $('#inport_end').val();

            return rule_tmp;
        }

        var selectID=null;
        $(document).ready(function(){
            init();

            var Forward_rows = GetGridData();
            DrawDataTables(Forward_rows);
    
             $('#btn_add').click(function(){
                 $("#ForwardDataForm").empty();
                 CreateForwardDataForm();
                 $("#add").show();
                 $("#modify").hide();
                 $('#ForwardListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });
        
             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                fr_rule.push(Array_Tmp);
                var Forward_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(Forward_rows);
                $('#ForwardListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#ForwardDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    fr_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });


             $("#btn_modify").click(function(){
                 $("#ForwardDataForm").empty();
                 CreateForwardDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                 $("#name").val(fr_rule[selectID][enum_fr_basic._NAME]);
                 if (fr_rule[selectID][enum_fr_basic._ENABLE] == 1){
                     $("#enable").attr('checked',true);
                 }else{
                     $("#enable").attr('checked',false);
                 }
                 $("select[name='extif']").attr("value", fr_rule[selectID][enum_fr_basic._EXT_IF]);
                 $("select[name='proto']").attr("value", fr_rule[selectID][enum_fr_basic._PROTOCOL]);
                 $("#extport_start").val(fr_rule[selectID][enum_fr_basic._EXT_PORT_START]);
                 $("#extport_end").val(fr_rule[selectID][enum_fr_basic._EXT_PORT_END]);
                 $("#ipaddr").val(fr_rule[selectID][enum_fr_basic._IP_ADDR]);
                 $("#inport_start").val(fr_rule[selectID][enum_fr_basic._IN_PORT_START]);
                 $("#inport_end").val(fr_rule[selectID][enum_fr_basic._IN_PORT_END]);

                 $('#ForwardListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 fr_rule.splice(selectID,1, Array_Tmp);

                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);
                 $('#ForwardListData-modal').modal('hide');
             });


             $("#btn_up").click(function(){
                 $("#ForwardDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == 0){
                         return false;
                     }
                 }
                 var arr_tmp = fr_rule[selectID];
                 fr_rule[selectID] = fr_rule[selectID - 1];
                 fr_rule[selectID - 1] = arr_tmp;
                 selectID = selectID - 1;
                 if ( selectID<0 ){
                    selectID = 0;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });

             $("#btn_down").click(function(){
                 $("#ForwardDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == fr_rule.length-1){
                         return false;
                     }
                 }

                 var arr_tmp = fr_rule[selectID];
                 fr_rule[selectID] = fr_rule[selectID + 1];
                 fr_rule[selectID + 1] = arr_tmp;
                 selectID = selectID + 1;
                 if ( selectID>fr_rule.length-1 ){
                     selectID = fr_rule.length-1;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });


        });// ready end
        
        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }

        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#ForwardListData').dataTable( {
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
                    { "sTitle": "<%lang("Protocol");%>", "sClass":"center" },
                    { "sTitle": "<%lang("External Port Range");%>" },
                    { "sTitle": "<%lang("Internal IP");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Internal Port Range");%>" },
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

            $("#ForwardListData tbody tr").click( function() {  // selete a row data
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

        function GetGridData(){
            var Forward_rows = [];
            var Forward_data = {};

            for (var i=0; i<fr_rule.length; i++ )  {
                Forward_data = {};
                Forward_data[0] = fr_rule[i][enum_fr_basic._NAME];
                Forward_data[1] = tbl_wrap_enable( fr_rule[i][enum_fr_basic._ENABLE] );
                Forward_data[2] = tbl_wrap_wan( fr_rule[i][enum_fr_basic._EXT_IF] );
                Forward_data[3] = tbl_wrap_proto( fr_rule[i][enum_fr_basic._PROTOCOL] );
                Forward_data[4] = "<%lang("From");%>:" + fr_rule[i][enum_fr_basic._EXT_PORT_START] + "<br><%lang("To");%>:"+fr_rule[i][enum_fr_basic._EXT_PORT_END];
                Forward_data[5] = fr_rule[i][enum_fr_basic._IP_ADDR];
                Forward_data[6] = "<%lang("From");%>:" + fr_rule[i][enum_fr_basic._IN_PORT_START] + "<br/><%lang("To");%>:" + fr_rule[i][enum_fr_basic._IN_PORT_END];

                Forward_rows[i] = Forward_data;
            }

            return Forward_rows;
        }

        function to_submit(F) {
            $("#submit_button").val("fr_basic");
            $("#action").val("Apply");
            local_pack_key_value_pair(F, "fr_table", "fr_rule");

            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
            

        }

        function init() {
            for (var i=0; i<dmz_objects.length; i++ ){
                enable_dmz = $('[name=fr_dmz'+i+'_enable]:checked').val();
                sel_change(enable_dmz, 1, dmz_objects[i]);
            }

            enable_table = $('[name=fr_enable]:checked').val();
            sel_change(enable_table, 1, fun_objects);
        }


        function CreateForwardDataForm(){
            $("#ForwardDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input type='text' name='name' id='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)'></td></tr>");
            $("#ForwardDataForm").append("<tr><td align='right'><%lang("Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' checked /></td></tr>");
            $("#ForwardDataForm").append("<tr><td align='right'><%lang("External Interface");%>:</td><td>"+
                                         "<select id='extif' name='extif' >"+
                                         "</select>"+
                                         "<td></td></tr>");
            $("#ForwardDataForm").append("<tr><td align='right'><%lang("Protocol");%>:</td><td>"+
                                         "<select id='proto' name='proto'>"+
                                         "  <option value='tcp'>TCP</option><option value='udp'>UDP</option><option value='both'>TCP/UDP</option>"+
                                         "</select>"+
                                         "</td></tr>");
            $("#ForwardDataForm").append("<tr><td align='right'><%lang("External Port Range");%>:</td><td>"+
                                         "<%lang("From");%>: <input type='text' class='input-mini' id='extport_start' name='extport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"External Port Range Start\")' /> "+
                                         "<%lang("To");%>: <input type='text' class='input-mini' id='extport_end' name='extport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"External Port Range End\")' />"+
                                         "</td></tr>");
            $("#ForwardDataForm").append("<tr><td align='right'><%lang("Internal IP");%>:</td><td><input type='text' class='input-medium' id='ipaddr' name='ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");
            $("#ForwardDataForm").append("<tr><td align='right'><%lang("Internal Port Range");%>:</td><td>"+
                                         "<%lang("From");%>: <input type='text' class='input-mini' id='inport_start' name='inport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Internal Port Range Start\")' /> "+
                                         "<%lang("To");%>: <input type='text' class='input-mini' id='inport_end' name='inport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Internal Port Range End\")' />"+
                                         "</td></tr>");

            for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
                $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
            }

        }

    </script>
    <% tbl_get("fr_rule"); %>

    </head>
    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements()</script>
                <h2><%lang("Applications");%>-<%lang("Port Range Forward");%></h2>

              <input type="hidden" name="fr_dmz_num" value=<%nv_get("wan_num");%>/>
              <% fr_show_setting(); %>

            <div class="row">
                <div class="span12">
                    <legend><%lang("Port Range Forwarding");%></legend>

                    <div class="row show-grid">
                        <div class="span4"><% lang("Port Forwarding"); %></div>
                        <div class="span8">
                            <label class="radio inline">
                              <input type="radio" value="1" name="fr_enable" onclick="init()" <%nv_match("fr_enable","1","checked");%>><%lang("Enable");%>
                            </label>
                            <label class="radio inline">
                              <input type="radio" value="0" name="fr_enable" onclick="init()" <%nv_match("fr_enable","0","checked");%>><%lang("Disable");%>
                            </label>
                        </div>
                    </div>

                </div>
            </div><!-- row -->
                <br>
            <div class="row">
                <div class="span12" style="">
                    <div id="ForwardListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                        <div class="modal-body">
                            <table class="table" id="ForwardDataForm"></table>
                        </div>
                        <div class="modal-footer">
                            <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                            <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                            <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                        </div>
                    </div>

                    <table class="table table-bordered table-striped" id="ForwardListData"></table>
                    <div class="btn-group">
                        <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                        <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                        <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                        <input type="button" class="btn" id="btn_up" name="btn_up" value="<% lang("Up"); %>">
                        <input type="button" class="btn" id="btn_down" name="btn_down" value="<% lang("Down"); %>">
                    </div>
                    <%tbl_create_table("fr_table", "fr_rule", "1");%>
                </div>
            </div><!-- row -->

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
                    <script type="text/javascript"><%show_copyright();%></script>
            </div>
        </div><!-- row -->

    </div> <!-- /container -->

</body></html>