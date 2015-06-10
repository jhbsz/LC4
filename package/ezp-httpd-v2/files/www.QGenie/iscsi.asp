<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - iSCSI</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <% tbl_get("isuser_rule"); %>
    <script type="text/javascript">
        var isuser_objects =
        [ { name: 'iscsi_isns' },
          { name: 'iscsi_id' },
          { name: 'iscsi_alias' },
          { name: 'iscsi_max_conn' },
          { name: 'iscsi_max_tx' },
          { name: 'iscsi_max_rx' },
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

            if (!$('#password').val()) {
                alert('Password is required.');
                return false;
            }
            
            if ($('#password').val().length < 14) {
                    alert('Password length must > 13');
                            return false;
            }
                                
            return true;
        }
        function local_table_check(T, R) {
            return rlist_check_dup_field(T, R, enum_is._NAME);
        }

        var enum_is = {                                                                                                          
            _ENABLE:0,
            _NAME:1,
            _PASSWD:2
        } 

        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = isuser_rule.length ;
            $("<input>").attr({type: "hidden", id: "isuser_rule", name: "isuser_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: isuser_rule[i][enum_is._ENABLE]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_username', name: P + i + '_username' , value: isuser_rule[i][enum_is._NAME] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_password', name: P + i + '_password' , value: isuser_rule[i][enum_is._PASSWD] }).appendTo("form");
            }

        }

        function local_generate_rule(T) {
            var rule_tmp = Array();
            var enable = 0;
            if ($('input[name=enable]').is(':checked')){
                enable = $('[name=enable]:checked').val();
            }

            rule_tmp[enum_is._ENABLE]   = enable;
            rule_tmp[enum_is._NAME]     = $('#username').val();
            rule_tmp[enum_is._PASSWD]   = $('#password').val();

            return rule_tmp;
        }

        function to_submit(F) {
            $("#submit_button").val("iscsi");
            $("#action").val("Apply");
            if (local_table_check("isuser_table", isuser_rule) == false) {
                return;
            }
            local_pack_key_value_pair(F, "isuser_table", "isuser_rule");
            sel_change(1, 1, isuser_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        function init() {
            enable_table = $('[name=iscsi_enable]:checked').val();
            sel_change(enable_table, 1, isuser_objects);
        }

        function GetGridData(){
            var iSCSI_rows = [];
            var iSCSI_data = [];

            for (var i=0; i<isuser_rule.length; i++ )  {
                iSCSI_data = [];
                iSCSI_data[0] = tbl_wrap_enable( isuser_rule[i][enum_is._ENABLE] );
                iSCSI_data[1] = isuser_rule[i][enum_is._NAME];
                iSCSI_data[2] = isuser_rule[i][enum_is._PASSWD];
                iSCSI_rows[i] = iSCSI_data;
            }
            return iSCSI_rows;
        }

        function CreateiSCSIDataForm(){
            $("#iSCSIDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
            $("#iSCSIDataForm").append("<tr><td align='right'><%lang("User Name");%>:</td><td><input type='text' id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/></td></tr>");
            $("#iSCSIDataForm").append("<tr><td align='right'><%lang("Password");%>:</td><td><input type='text' id='password' name='password' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/></td></tr>");
        }

        var selectID=null;
        $(document).ready(function(){
            init();
            var iSCSI_rows = GetGridData();
            DrawDataTables(iSCSI_rows);

             $('#btn_add').click(function(){
                $("#iSCSIDataForm").empty();
                CreateiSCSIDataForm();

                 $("#add").show();
                 $("#modify").hide();
                 $('#iSCSIListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                isuser_rule.push(Array_Tmp);
                var iSCSI_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(iSCSI_rows);
                $('#iSCSIListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#iSCSIDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    isuser_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });

             $("#btn_modify").click(function(){
                 $("#iSCSIDataForm").empty();
                 CreateiSCSIDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                 if (isuser_rule[selectID][enum_is._ENABLE] == 1){
                     $("#enable").attr('checked',true);
                 }else{
                     $("#enable").attr('checked',false);
                 }
                 $("#username").val(isuser_rule[selectID][enum_is._NAME]);
                 $("#password").val(isuser_rule[selectID][enum_is._PASSWD]);

                 $('#iSCSIListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 isuser_rule.splice(selectID,1, Array_Tmp);

                var iSCSI_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(iSCSI_rows);
                 $('#iSCSIListData-modal').modal('hide');
             });

        });//End ready

        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }


        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#iSCSIListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "<%lang("Rule Enable");%>", "sClass":"center", "sWidth": "200px" },
                    { "sTitle": "<%lang("User Name");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Password");%>", "sClass":"center", "sWidth": "200px" }
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

            $("#iSCSIListData tbody tr").click( function() {  // selete a row data
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
 	          <script type="text/javascript">do_hidden_elements('is_rule');</script>
                <h2><%lang("Storage");%>-<%lang("iSCSI");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("iSCSI");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("iSCSI"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="iscsi_enable" id="iscsi_enable" onclick="init()"
                                <%nvg_attr_match("iscsi_enable","is_rule","0","enable","1","checked");%>><%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="iscsi_enable" id="iscsi_enable" onclick="init()"
                                <%nvg_attr_match("iscsi_enable","is_rule","0","enable","0","checked");%>><%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("iSNS Server");%></div>
                    <div class="span8">
                        <input type="text" name="iscsi_isns" id="iscsi_isns" size="20" maxLength="80"
                            value='<%nvg_attr_get("iscsi_isns", "is_rule", "0", "isns_serv");%>' /> (<%lang("IP address or domain name");%>)
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Target ID");%></div>
                    <div class="span8">
                        <input type="text" name="iscsi_id" id="iscsi_id" size="20" maxLength="80"
                            value='<%nvg_attr_get("iscsi_id", "is_rule", "0", "isid");%>' />
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("LUN Alias");%></div>
                    <div class="span8">
                        <input type="text" name="iscsi_alias" id="iscsi_alias" size="20" maxLength="20"
                            value='<%nvg_attr_get("iscsi_alias", "is_rule", "0", "alias");%>' /> (20 <%lang("Characters");%>)
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Max Connextion");%></div>
                    <div class="span8">
                        <input type="text" name="iscsi_max_conn" id="iscsi_max_conn" size="10" maxLength="10"
                            value='<%nvg_attr_get("iscsi_max_conn", "is_rule", "0", "max_conn");%>' /> <%lang("Connections");%> (1 ~ 10)
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Max Transmision Data Length");%></div>
                    <div class="span8">
                        <input type="text" name="iscsi_max_tx" id="iscsi_max_tx" size="10" maxLength="30"
                            value='<%nvg_attr_get("iscsi_max_tx", "is_rule", "0", "max_tx");%>' /> <%lang("Bytes");%> (512 ~ 16384)
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Max Receiving Data Length");%></div>
                    <div class="span8">
                        <input type="text" name="iscsi_max_rx" id="iscsi_max_rx" size="10" maxLength="30"
                        value='<%nvg_attr_get("iscsi_max_rx", "is_rule", "0", "max_rx");%>' /> <%lang("Bytes");%> (512 ~ 16384)
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="iSCSIListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-body">
                        <table class="table" id="iSCSIDataForm"></table>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>


                <table class="table table-bordered table-striped" id="iSCSIListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                </div>

              <%tbl_create_table("isuser_table", "isuser_rule", "1");%>
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