<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Web Filtering</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <% tbl_get("wf_rule"); %>
    <script type="text/javascript">
        var F;

        function local_rule_check() {

            if ((!$('#keyword').val())) {
                alert('Keyword is required for filtering.');
                return false;
            }
            
            return true;
        }

         
        var enum_wf_basic = {
            _ENABLE:0,
            _KEYWORD:1,
            _TYPE:2,
            _ACCESS:3
        }; 

        function local_pack_key_value_pair(F, T, P) {
            /* Important */
            var row_num = wf_rule.length ;
            $("<input>").attr({type: "hidden", id: "wf_rule", name: "wf_rule", value:""}).appendTo("form");
            $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
            for (var i = 0; i < row_num; i++) {
                $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: wf_rule[i][enum_wf_basic._ENABLE]}).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_keyword', name: P + i + '_keyword' , value: wf_rule[i][enum_wf_basic._KEYWORD] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_type', name: P + i + '_type' , value: wf_rule[i][enum_wf_basic._TYPE] }).appendTo("form");
                $("<input>").attr({type: "hidden", id: P + i + '_access', name: P + i + '_access' , value: wf_rule[i][enum_wf_basic._ACCESS] }).appendTo("form");
            }
        }

        function local_generate_rule(T) {
            var rule_tmp = Array();
            var enable = 0;
            if ($('input[name=enable]').is(':checked')){
                enable = $('[name=enable]:checked').val();
            }
            rule_tmp[enum_wf_basic._ENABLE]   = enable;
            rule_tmp[enum_wf_basic._KEYWORD]  = $('#keyword').val();
            rule_tmp[enum_wf_basic._TYPE]     = $('#filter_type').val();
            rule_tmp[enum_wf_basic._ACCESS]   = $('#access').val();
            return rule_tmp;
        }

        function to_submit(F) {
            $("#submit_button").val("wf_basic");
            $("#action").val("Apply");
            local_pack_key_value_pair(F, "wf_table", "wf_rule");
            sel_change(1, 1, wf_objects);

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
        }

        var wf_objects =
        [ 
        { name: 'activex_enable' },
        { name: 'java_enable' },
        { name: 'proxy_enable' },
        { name: 'btn_add' },
        { name: 'btn_delete' },
        { name: 'btn_modify' },
        { name: 'btn_up' },
        { name: 'btn_down' }
        ];                       
        function init() {
            enable_table = $('[name=wf_enable]:checked').val();
            sel_change(enable_table, 1, wf_objects);
        }

        function GetGridData(){
            var Page_rows = [];
            var Page_data = [];

            for (var i=0; i<wf_rule.length; i++ )  {
                Page_data = [];
                Page_data[0] = tbl_wrap_enable( wf_rule[i][enum_wf_basic._ENABLE] );
                Page_data[1] = wf_rule[i][enum_wf_basic._KEYWORD];
                Page_data[2] = wf_rule[i][enum_wf_basic._TYPE];
                Page_data[3] = tbl_wrap_action( wf_rule[i][enum_wf_basic._ACCESS] );
                Page_rows[i] = Page_data;
            }
            return Page_rows;
        }

        function CreatePageDataForm(){
            $("#PageDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
            $("#PageDataForm").append("<tr><td align='right'><%lang("Filter Keyword");%>:</td><td><input type='text' id='keyword' name='keyword' size='17' maxlength='15' /></td></tr>");
            $("#PageDataForm").append("<tr><td align='right'><%lang("Filter Type");%>:</td><td>"+
                                         "<select id='filter_type' name='filter_type'>"+
                                         "  <option value='url' selected><%lang("url");%></option>"+
                                         "  <option value='host'><%lang("host");%></option>"+
                                         "</select>"+
                                         "</td></tr>");
            $("#PageDataForm").append("<tr><td align='right'><%lang("Action");%>:</td><td>"+
                                         "<select id='access' name='access' \>"+
                                         "  <option value='1'><%lang("ALLOW");%></option>"+
                                         "  <option value='0'><%lang("DENY");%></option>"+
                                         "</select>"+
                                         "</td></tr>");

        }

        var selectID=null;
        $(document).ready(function(){
            init();
            var Page_rows = GetGridData();
            DrawDataTables(Page_rows);

             $('#btn_add').click(function(){
                $("#PageDataForm").empty();
                CreatePageDataForm();

                 $("#add").show();
                 $("#modify").hide();
                 $('#PageListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                wf_rule.push(Array_Tmp);
                var Page_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(Page_rows);
                $('#PageListData-modal').modal('hide');
             });

             $("#btn_delete").click(function(){
                $("#PageDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    wf_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });

             $("#btn_modify").click(function(){
                 $("#PageDataForm").empty();
                 CreatePageDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                if (wf_rule[selectID][enum_wf_basic._ENABLE] == 1){
                    $("#enable").attr('checked',true);
                }else{
                    $("#enable").attr('checked',false);
                }
                $("#keyword").val(wf_rule[selectID][enum_wf_basic._KEYWORD]);
                $("select[name='filter_type']").attr("value", wf_rule[selectID][enum_wf_basic._TYPE]);
                $("select[name='access']").attr("value", wf_rule[selectID][enum_wf_basic._ACCESS]);

                 $('#PageListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 wf_rule.splice(selectID,1, Array_Tmp);

                var Page_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(Page_rows);
                 $('#PageListData-modal').modal('hide');
             });

             $("#btn_up").click(function(){
                 $("#PageDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == 0){
                         return false;
                     }
                 }
                 var arr_tmp = wf_rule[selectID];
                 wf_rule[selectID] = wf_rule[selectID - 1];
                 wf_rule[selectID - 1] = arr_tmp;
                 selectID = selectID - 1;
                 if ( selectID<0 ){
                    selectID = 0;
                 }
                 var Forward_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(Forward_rows);

             });

             $("#btn_down").click(function(){
                 $("#PageDataForm").empty();

                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 } else {
                     if (selectID == wf_rule.length-1){
                         return false;
                     }
                 }

                 var arr_tmp = wf_rule[selectID];
                 wf_rule[selectID] = wf_rule[selectID + 1];
                 wf_rule[selectID + 1] = arr_tmp;
                 selectID = selectID + 1;
                 if ( selectID>wf_rule.length-1 ){
                     selectID = wf_rule.length-1;
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
            DataTable = $('#PageListData').dataTable( {
                "sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
                "sPaginationType": "bootstrap",
                "oLanguage": {
                    "sLengthMenu": "_MENU_ records per page"
                },
                "aaData": LV_Data,
                "aoColumns": [
                    { "sTitle": "<%lang("Rule Enable");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Filter Keyword");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Filter Type");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Action");%>", "sClass":"center" },
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

            $("#PageListData tbody tr").click( function() {  // selete a row data
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
                <h2><%lang("Security");%>-<%lang("Web Filtering");%></h2>

        <div class="row">
            <div class="span12">
                <legend><%lang("Web Filtering");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Web Filtering"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="wf_enable" id="wf_enable" onclick="init()"
                              <%nvg_match("wf_enable", "1", "checked");%>>
                              <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="wf_enable" id="wf_enable" onclick="init()"
                              <%nvg_match("wf_enable", "0", "checked");%>>
                              <%lang("Disable");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>

        <div class="row">
            <div class="span12">
                <legend><%lang("Web Content Filtering");%></legend>
                <div class="row show-grid">
                    <div class="span4"><% lang("Activex Filtering"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="activex_enable" id="activex_enable" onclick="init()"
                              <%nvg_attr_match("activex_enable", "wf_content_rule", "0", 
                                               "activex_enable", "1", "checked");%>>
                              <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="activex_enable" id="activex_enable" onclick="init()"
                              <%nvg_attr_match("activex_enable", "wf_content_rule", "0", 
                                               "activex_enable", "0", "checked");%>>
                              <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Java/JavaScript Filtering"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="java_enable" id="java_enable" onclick="init()"
                              <%nvg_attr_match("java_enable", "wf_content_rule", "0", 
                                               "java_enable", "1", "checked");%>>
                              <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="java_enable" id="java_enable" onclick="init()"
                              <%nvg_attr_match("java_enable", "wf_content_rule", "0", 
                                               "java_enable", "0", "checked");%>>
                              <%lang("Disable");%>
                        </label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Proxy Filtering"); %></div>
                    <div class="span8">
                        <label class="radio inline">
                            <input type="radio" value="1" name="proxy_enable" id="proxy_enable" onclick="init()"
                              <%nvg_attr_match("proxy_enable", "wf_content_rule", "0", 
                                               "proxy_enable", "1", "checked");%>>
                              <%lang("Enable");%>
                        </label>
                        <label class="radio inline">
                            <input type="radio" value="0" name="proxy_enable" id="proxy_enable" onclick="init()"
                              <%nvg_attr_match("proxy_enable", "wf_content_rule", "0", 
                                               "proxy_enable", "0", "checked");%>>
                              <%lang("Disable");%>
                        </label>
                    </div>
                </div>

            </div>
        </div><!-- row -->
        <br>
        <div class="row">
            <div class="span12" style="">
                <div id="PageListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                    <div class="modal-body">
                        <table class="table" id="PageDataForm"></table>
                    </div>
                    <div class="modal-footer">
                        <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                        <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                    </div>
                </div>


                <table class="table table-bordered table-striped" id="PageListData"></table>
                <div class="btn-group">
                    <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                    <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                    <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                    <input type="button" class="btn" id="btn_up" name="btn_up" value="<% lang("Up"); %>">
                    <input type="button" class="btn" id="btn_down" name="btn_down" value="<% lang("Down"); %>">
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