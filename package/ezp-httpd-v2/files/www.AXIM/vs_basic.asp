<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Virtual Hosts</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>
    <%do_datatables_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>
    <%do_datatables_js();%>

    <script type="text/javascript">
        fun_objects = [ { name: 'btn_add' },
                        { name: 'btn_delete' },
                        { name: 'btn_modify' }];

	var enum_vs_basic = {
	    _NAME:0,
	    _ENABLE:1,
	    _EXT_IF:2,
	    _IF_INDEX:3,
	    _WAN_ADDR:4,
	    _MAPPED_ADDR:5
	} 

	function local_pack_key_value_pair(F, T, P) {
	    /* Important */
	    var row_num = vs_rule.length ;
	    $("<input>").attr({type: "hidden", id: "vs_rule", name: "vs_rule", value:""}).appendTo("form");
	    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
	    for (var i = 0; i < row_num; i++) {
	        $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name' , value: vs_rule[i][enum_vs_basic._NAME] }).appendTo("form");
	        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: vs_rule[i][enum_vs_basic._ENABLE]}).appendTo("form");
	        $("<input>").attr({type: "hidden", id: P + i + '_extif', name: P + i + '_extif' , value: vs_rule[i][enum_vs_basic._EXT_IF] }).appendTo("form");
	        $("<input>").attr({type: "hidden", id: P + i + '_wan_ipaddr', name: P + i + '_wan_ipaddr' , value: vs_rule[i][enum_vs_basic._WAN_ADDR] }).appendTo("form");
	        $("<input>").attr({type: "hidden", id: P + i + '_mapped_ipaddr', name: P + i + '_mapped_ipaddr' , value: vs_rule[i][enum_vs_basic._MAPPED_ADDR] }).appendTo("form");
	    }
	}

                                   
        function to_submit(F) {
            $("#submit_button").val("vs_basic");
            $("#action").val("Apply");
            local_pack_key_value_pair(F, "vs_table", "vs_rule");

            open_waiting_window();
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
            
        }
        
	function init() {
	    enable_table = $('[name=vs_enable]:checked').val();
	    sel_change(enable_table, 1, fun_objects);
	
	}    
	


                                              
	function local_rule_check(T) {
	
	    if ((!$('#name').val())) {
	        alert('Rule Name is required.');
	        return false;
	    }
	
	    if (!$('#wan_ipaddr').val()) {
	        alert('Please specify a External IP Address.');
	        return false;
	    }
	
	    if (!$('#mapped_ipaddr').val()) {
	        alert('Please specify a Mapped LAN IP Address.');
	        return false;
	    }
	
	    return true;
	}
	


	function local_generate_rule(T) {
	    var rule_tmp = Array();
	    var enable = 0;
	    if ($('input[name=enable]').is(':checked')){
	        enable = $('[name=enable]:checked').val();
	    }
	
	    rule_tmp[enum_vs_basic._NAME]   = $('#name').val();
	    rule_tmp[enum_vs_basic._ENABLE] = enable;
	    rule_tmp[enum_vs_basic._EXT_IF] = $('#extif').val();
	    rule_tmp[enum_vs_basic._WAN_ADDR] = $('#wan_ipaddr').val();
	    rule_tmp[enum_vs_basic._MAPPED_ADDR] = $('#mapped_ipaddr').val();
	
	    return rule_tmp;
	
	}
	




        var DataTable;
        function DrawDataTables(LV_Data){
            DataTable = $('#VSListData').dataTable( {
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
                    { "sTitle": "<%lang("External IP Address");%>", "sClass":"center" },
                    { "sTitle": "<%lang("Mapped LAN IP Address");%>", "sClass":"center"  },
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

            $("#VSListData tbody tr").click( function() {  // selete a row data
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
	    var VS_rows = [];
	    var VS_data = {};
	
	    for (var i=0; i<vs_rule.length; i++ )  {
	        VS_data = {};
	        VS_data[0] = vs_rule[i][enum_vs_basic._NAME];
	        VS_data[1] = tbl_wrap_enable( vs_rule[i][enum_vs_basic._ENABLE] );
	        VS_data[2] = tbl_wrap_wan(vs_rule[i][enum_vs_basic._EXT_IF]);
	        VS_data[3] = "<%lang("From");%>:" + vs_rule[i][enum_vs_basic._WAN_ADDR];
	        VS_data[4] = "<%lang("To");%>:" + vs_rule[i][enum_vs_basic._MAPPED_ADDR];
	        
	        VS_rows[i] = VS_data;
	    }
	    return VS_rows;
	}

        function fnGetSelected( oTableLocal ){
            return oTableLocal.$('tr.row_selected');
        }


        var selectID=null;
        $(document).ready(function(){
            init();
            var VS_rows = GetGridData();
            DrawDataTables(VS_rows);
            
            $('#btn_add').click(function(){
                 $("#VSDataForm").empty();
                 CreateVSDataForm();                
                 $("#add").show();
                 $("#modify").hide();
                 
                 $('#VSListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
                 
            });
            $("#add").click(function(){
                if (local_rule_check()==false){
                    return false;
                }
                var Array_Tmp = local_generate_rule()
                vs_rule.push(Array_Tmp);
                var VS_rows = GetGridData();
                DataTable.fnDestroy();
                DrawDataTables(VS_rows);
                $('#VSListData-modal').modal('hide');
             });
             
            $("#btn_delete").click(function(){
                $("#VSDataForm").empty();

                var anSelected = fnGetSelected( DataTable );
                if ( anSelected.length !== 0 ) {
                    DataTable.fnDeleteRow( anSelected[0] );
                    vs_rule.splice(selectID,1);
                    selectID = null;
                    DataTable.$('tr.row_selected').removeClass('row_selected');
                }else{
                    alert("Please, select row");
                    return false;
                }

             });


             $("#btn_modify").click(function(){
                 $("#VSDataForm").empty();
                 CreateVSDataForm();
                 $("#add").hide();
                 $("#modify").show();
                 if (selectID ==null) {
                     alert("Please, select row");
                     return false;
                 }

                 $("#name").val(vs_rule[selectID][enum_vs_basic._NAME]);
                 if (vs_rule[selectID][enum_vs_basic._ENABLE] == 1){
                     $("#enable").attr('checked',true);
                 }else{
                     $("#enable").attr('checked',false);
                 }
                 $("select[name='extif']").attr("value", vs_rule[selectID][enum_vs_basic._EXT_IF]);

                 $("#wan_ipaddr").val(vs_rule[selectID][enum_vs_basic._WAN_ADDR]);
                 $("#mapped_ipaddr").val(vs_rule[selectID][enum_vs_basic._MAPPED_ADDR]);

                 

                 $('#VSListData-modal').modal({
                    keyboard: false,
                    backdrop: 'static'
                 })
                 
                 
             });

             $("#modify").click(function(){
                 if (local_rule_check()==false){
                     return false;
                 }
                 var Array_Tmp = local_generate_rule()
                 vs_rule.splice(selectID,1, Array_Tmp);

                 var VS_rows = GetGridData();
                 DataTable.fnDestroy();
                 DrawDataTables(VS_rows);
                 $('#VSListData-modal').modal('hide');
             });

             
        })
        
	function CreateVSDataForm(){

	
	    $("#VSDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input type='text' id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' /></td></tr>");
	    $("#VSDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
	    $("#VSDataForm").append("<tr><td align='right'><%lang("External Interface");%>:</td><td>"+
	                                 "<select id='extif' name='extif' >"+
	                                 "</select>"+
	                                 "<td></td></tr>");
	    $("#VSDataForm").append("<tr><td align='right'><%lang("External IP Address");%>:</td><td><input type='text' id='wan_ipaddr' name='wan_ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");
	    $("#VSDataForm").append("<tr><td align='right'><%lang("Mapped LAN IP Address");%>:</td><td><input type='text' id='mapped_ipaddr' name='mapped_ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");
	
	    for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
	        $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
	    }
	
	}

    </script>
    <% tbl_get("vs_rule"); %>

    </head>
    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements()</script>
                <h2><%lang("Applications");%>-<%lang("Virtual Hosts");%></h2>
              <input type="hidden" name="fr_dmz_num" value=<%nv_get("wan_num");%>/>

            <div class="row">
                <div class="span12">
                    <legend><%lang("Virtual Hosts");%></legend>

                    <div class="row show-grid">
                        <div class="span4"><% lang("Virtual Hosts"); %></div>
                        <div class="span8">
                            <label class="radio inline">                		
                		<input type="radio" value="1" name="vs_enable" id="vs_enable" onclick="init()"
                      			<%nv_match("vs_enable","1","checked");%>><%lang("Enable");%>
                		</input>
                            </label>
                            <label class="radio inline">
                            	<input type="radio" value="0" name="vs_enable" id="vs_enable" onclick="init()"
                      	        	<%nv_match("vs_enable","0","checked");%>><%lang("Disable");%>
                		</input>
                            </label>
                        </div>
                    </div>

                </div>
            </div><!-- row -->
          

            
            <div class="row">
                <div class="span12" style="">
                    <div id="VSListData-modal" class="modal hide fade" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
                        <div class="modal-body">
                            <table class="table" id="VSDataForm"></table>
                        </div>
                        <div class="modal-footer">
                            <input type="button" class="btn btn-primary" name="add" id="add" value="<% lang("Confirm"); %>" >
                            <input type="button" class="btn btn-primary" style="display:none;" name="modify" id="modify" value="<% lang("Confirm"); %>" >
                            <input type="button" class="btn" data-dismiss="modal" aria-hidden="true" name="close" id="close" value="<% lang("Cancel"); %>">
                        </div>
                    </div>

                    <table class="table table-bordered table-striped" id="VSListData"></table>
                    <div class="btn-group">
                        <input type="button" class="btn" id="btn_add" name="btn_add" value="<% lang("Add"); %>">
                        <input type="button" class="btn" id="btn_delete" name="btn_delete" value="<% lang("Delete"); %>">
                        <input type="button" class="btn" id="btn_modify" name="btn_modify" value="<% lang("Modify"); %>">
                    </div>
                    <%tbl_create_table("vs_table", "vs_rule", "1");%>
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