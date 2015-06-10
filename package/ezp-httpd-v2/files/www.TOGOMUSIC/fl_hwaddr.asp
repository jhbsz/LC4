<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - MAC Rule</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <% tbl_get("fl_hwaddr_rule"); %>
    <script type="text/javascript">
        var F;

        function local_rule_check(T) {
            if (!$('#name').val()) {
                alert('Please enter the name.');
                return false;
            }

            if (!$('#hwaddr').val()) {
                alert('Please specify a MAC.');
                return false;
            }

            if ( $('input[name=arp_enable]').is(':checked') || $('input[name=dhcp_enable]').is(':checked') ) {
                if (!$('#ipaddr').val()) {
                    alert('Please specify an IP.');
                    return false;
                }
            }

            return true;
        }


        var enum_fl_hwaddr = {
            _NAME:0,
            _HWADDR:1,
            _OP:2,
            _ACL_ENABLE:3,
            _ARP_ENABLE:4,
            _DHCP_ENABLE:5,
            _IPADDR:6
        } 


        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = fl_hwaddr_rule.length ;
            $("<input>").attr({type: "hidden", id: "fl_hwaddr_rule", name: "fl_hwaddr_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: fl_hwaddr_rule[i][enum_fl_hwaddr._NAME]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_hwaddr', name: P + i + '_hwaddr' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._HWADDR] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_op', name: P + i + '_op' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._OP] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_acl_enable', name: P + i + '_acl_enable' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._ACL_ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_arp_enable', name: P + i + '_arp_enable' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._ARP_ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_dhcp_enable', name: P + i + '_dhcp_enable' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._DHCP_ENABLE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_ipaddr', name: P + i + '_ipaddr' , value: fl_hwaddr_rule[i][enum_fl_hwaddr._IPADDR] }).appendTo("form");
            }
        }

        function local_generate_rule() {
            var acl_enable = 0;
            if ($('input[name=acl_enable]').is(':checked')){
                acl_enable = $('[name=acl_enable]:checked').val();
            }

            var arp_enable = 0;
            if ($('input[name=arp_enable]').is(':checked')){
                arp_enable = $('[name=arp_enable]:checked').val();
            }
            var dhcp_enable = 0;
            if ($('input[name=dhcp_enable]').is(':checked')){
                dhcp_enable = $('[name=dhcp_enable]:checked').val();
            }

            var rule_tmp = Array();
            rule_tmp[enum_fl_hwaddr._NAME]        = $('#name').val();
            rule_tmp[enum_fl_hwaddr._HWADDR]      = $('#hwaddr').val();
            rule_tmp[enum_fl_hwaddr._OP]          = $("select#op option:selected").val();
            rule_tmp[enum_fl_hwaddr._ACL_ENABLE]  = acl_enable;
            rule_tmp[enum_fl_hwaddr._ARP_ENABLE]  = arp_enable;
            rule_tmp[enum_fl_hwaddr._DHCP_ENABLE] = dhcp_enable;
            rule_tmp[enum_fl_hwaddr._IPADDR]      = $('#ipaddr').val();
            return rule_tmp;
        }

        function to_submit(F) {
            $("#submit_button").val("fl_hwaddr");
            $("#action").val("Apply");
            local_pack_key_value_pair(F, "fl_hwaddr_table", "fl_hwaddr_rule");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        fl_hwaddr_objects = [  { name: 'fl_hwaddr_rule_default' },
                        { name: 'btn_add' },
                        { name: 'btn_delete' },
                        { name: 'btn_modify' },
                        { name: 'btn_up' },
                        { name: 'btn_down' } ];

        function init() {
            enable_table = $('[name=fl_hwaddr_enable]:checked').val();
            sel_change(enable_table, 1, fl_hwaddr_objects);
        }

        function GetGridData(){
            var MAC_rows = [];
            var MAC_data = [];

            for (var i=0; i<fl_hwaddr_rule.length; i++ )  {
                MAC_data = [];

                MAC_data[0] = fl_hwaddr_rule[i][enum_fl_hwaddr._NAME];
                MAC_data[1] = tbl_wrap_action( fl_hwaddr_rule[i][enum_fl_hwaddr._OP] );
                MAC_data[2] = fl_hwaddr_rule[i][enum_fl_hwaddr._HWADDR];
                MAC_data[3] = tbl_wrap_enable( fl_hwaddr_rule[i][enum_fl_hwaddr._ACL_ENABLE] );
                MAC_data[4] = tbl_wrap_enable( fl_hwaddr_rule[i][enum_fl_hwaddr._ARP_ENABLE] );
                MAC_data[5] = tbl_wrap_enable( fl_hwaddr_rule[i][enum_fl_hwaddr._DHCP_ENABLE] );
                MAC_data[6] = fl_hwaddr_rule[i][enum_fl_hwaddr._IPADDR];
                MAC_rows[i] = MAC_data;
                
            }
            return MAC_rows;
        }

        function CreateMACDataForm(){

            $("#MACDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input type='text' name='name' id='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)'></td></tr>");
            $("#MACDataForm").append("<tr><td align='right'><%lang("MAC");%>:</td><td><input type='text' id='hwaddr' name='hwaddr' size='18' maxlength='17' onblur='valid_hwaddr(this, \"MAC\")' /></td></tr>");
            $("#MACDataForm").append("<tr><td align='right'><%lang("Action");%>:</td><td>"+
                                         "<select id='op' name='op' \>"+
                                         "  <option value='1'><%lang("ALLOW");%></option>"+
                                         "  <option value='0'><%lang("DENY");%></option>"+
                                         "</select>"+
                                         "</td></tr>");
            $("#MACDataForm").append("<tr><td align='right'><%lang("ACL Enable");%>:</td><td><input id='acl_enable' name='acl_enable' type='checkbox' size='10' value='1' /></td></tr>");
            $("#MACDataForm").append("<tr><td align='right'><%lang("Static ARP Enable");%>:</td><td><input id='arp_enable' name='arp_enable' type='checkbox' size='10' value='1' /></td></tr>");
            $("#MACDataForm").append("<tr><td align='right'><%lang("Static DHCP Enable");%>:</td><td><input id='dhcp_enable' name='dhcp_enable' type='checkbox' size='10' value='1' /></td></tr>");
            $("#MACDataForm").append("<tr><td align='right'><%lang("IP");%>:</td><td><input type='text' id='ipaddr' name='ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");

        }

        var selectID=null;
        $(document).ready(function(){
            init();
            var MAC_rows = GetGridData();
            DrawDataTables(MAC_rows);

             $('#btn_add').click(function(){
                $("#MACDataForm").empty();
                CreateMACDataForm();

                 $("#add").show();
                 $("#modify").hide();
                 $('#MACListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                fl_hwaddr_rule.push(Array_Tmp);
                var MAC_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(MAC_rows);
                $('#MACListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#MACDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    fl_hwaddr_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });

             $("#btn_modify").click(function(){
                 $("#MACDataForm").empty();
                 CreateMACDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                 $("#name").val(fl_hwaddr_rule[selectID][enum_fl_hwaddr._NAME]);
                 $("#hwaddr").val(fl_hwaddr_rule[selectID][enum_fl_hwaddr._HWADDR]);
                 $("select[name='op']").attr("value", fl_hwaddr_rule[selectID][enum_fl_hwaddr._OP]);

                 if (fl_hwaddr_rule[selectID][enum_fl_hwaddr._ACL_ENABLE] == 1){
                     $("#acl_enable").attr('checked',true);
                 }else{
                     $("#acl_enable").attr('checked',false);
                 }
                 if (fl_hwaddr_rule[selectID][enum_fl_hwaddr._ARP_ENABLE] == 1){
                     $("#arp_enable").attr('checked',true);
                 }else{
                     $("#arp_enable").attr('checked',false);
                 }
                 if (fl_hwaddr_rule[selectID][enum_fl_hwaddr._DHCP_ENABLE] == 1){
                     $("#dhcp_enable").attr('checked',true);
                 }else{
                     $("#dhcp_enable").attr('checked',false);
                 }
                 $("#ipaddr").val(fl_hwaddr_rule[selectID][enum_fl_hwaddr._IPADDR]);

                 $('#MACListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 fl_hwaddr_rule.splice(selectID,1, Array_Tmp);

                var MAC_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(MAC_rows);
                 $('#MACListData-modal').modal('hide');
             });

             $("#btn_up").click(function(){
                 $("#MACDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == 0){
                         return false;
                     }
                 }
                 var arr_tmp = fl_hwaddr_rule[selectID];
                 fl_hwaddr_rule[selectID] = fl_hwaddr_rule[selectID - 1];
                 fl_hwaddr_rule[selectID - 1] = arr_tmp;
                 selectID = selectID - 1;
                 if ( selectID<0 ){
                    selectID = 0;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });

             $("#btn_down").click(function(){
                 $("#MACDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == fl_hwaddr_rule.length-1){
                         return false;
                     }
                 }

                 var arr_tmp = fl_hwaddr_rule[selectID];
                 fl_hwaddr_rule[selectID] = fl_hwaddr_rule[selectID + 1];
                 fl_hwaddr_rule[selectID + 1] = arr_tmp;
                 selectID = selectID + 1;
                 if ( selectID>fl_hwaddr_rule.length-1 ){
                     selectID = fl_hwaddr_rule.length-1;
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
            DataTable = $('#MACListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "<%lang("Rule Enable");%>", "sClass":"center", "sWidth": "200px" },
                    { "sTitle": "<%lang("Action");%>", "sClass":"center" },
                    { "sTitle": "<%lang("MAC");%>", "sClass":"center", "sWidth": "200px" },
                    { "sTitle": "<%lang("ACL Enable");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Static ARP Enable");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Static DHCP Enable");%>", "sClass":"center" },
                    { "sTitle": "<%lang("IP");%>", "sClass":"center", "sWidth": "200px" }
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

            $("#MACListData tbody tr").click( function() {  // selete a row data
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
                <h2><%lang("Security");%>-<%lang("MAC Access Control");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("MAC Access Control");%></legend>

                <div class="row show-grid">
                    <div class="span4"><% lang("MAC Access Control"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="fl_hwaddr_enable" id="fl_hwaddr_enable" onclick="init()"
                                <%nv_match("fl_hwaddr_enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="fl_hwaddr_enable" id="fl_hwaddr_enable" onclick="init()"
                                <%nv_match("fl_hwaddr_enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Default MAC Access Control Action"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="fl_hwaddr_rule_default" id="fl_hwaddr_rule_default"
                                <%nv_match("fl_hwaddr_rule_default","1","checked");%>><%lang("ALLOW");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="fl_hwaddr_rule_default" id="fl_hwaddr_rule_default"
                                <%nv_match("fl_hwaddr_rule_default","0","checked");%>><%lang("DENY");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->

        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="MACListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-body">
                        <table class="table" id="MACDataForm"></table>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>


                <table class="table table-bordered table-striped" id="MACListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                    <input type="button" class="btn" id="btn_up" name="btn_up" value="<% lang("Up"); %>">
                    <input type="button" class="btn" id="btn_down" name="btn_down" value="<% lang("Down"); %>">
                </div>

                <%tbl_create_table("fl_hwaddr_table", "fl_hwaddr_rule", "1");%>
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