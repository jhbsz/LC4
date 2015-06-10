<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - ACL</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
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
    F.submit_button.value = "fl_basic";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "fl_table", "fl_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {

    if ($('[name=fl_enable]:checked').val()==1){
        $("input[name=fl_rule_default][@type=radio]").attr("disabled",false);
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
        $("#ItemUp").removeClass('ui-state-disabled');
        $("#ItemDown").removeClass('ui-state-disabled');
    }else{
        $("input[name=fl_rule_default][@type=radio]").attr("disabled",true);
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
        $("#ItemUp").addClass('ui-state-disabled');
        $("#ItemDown").addClass('ui-state-disabled');
    }

    <%widget_start();%>
}

function GetGridData(){
    var ACL_rows = [];
    var ACL_data = {};
    
    for (var i=0; i<fl_rule.length; i++ )  {
        ACL_data = {};
        
        ACL_data["name"] = fl_rule[i][enum_fl_basic._NAME];
        ACL_data["enable"] = tbl_wrap_enable( fl_rule[i][enum_fl_basic._ENABLE] );
        ACL_data["interface"] = tbl_wrap_wan( fl_rule[i][enum_fl_basic._EXT_IF] );
        ACL_data["internalip"] = "<%lang("From");%>:" + fl_rule[i][enum_fl_basic._IN_IPADDR_START] + "<br><%lang("To");%>:" + fl_rule[i][enum_fl_basic._IN_IPADDR_END];
        ACL_data["externalip"] = "<%lang("From");%>:" + fl_rule[i][enum_fl_basic._EXT_IPADDR_START] + "<br><%lang("To");%>:" + fl_rule[i][enum_fl_basic._EXT_IPADDR_END];
        ACL_data["protocol"] = tbl_wrap_proto(fl_rule[i][enum_fl_basic._PROTOCOL]);
        ACL_data["portrange"] = "<%lang("From");%>:" + fl_rule[i][enum_fl_basic._EXT_PORT_START] + "<br><%lang("To");%>:" + fl_rule[i][enum_fl_basic._EXT_PORT_END];
        ACL_data["op"] = tbl_wrap_action(fl_rule[i][enum_fl_basic._ACTION]);
        
        ACL_rows.push(ACL_data);
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
                                 "<%lang("From");%>:<input id='inipaddr_start' name='inipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range Start\")' />"+
                                 "<%lang("To");%>:<input id='inipaddr_end' name='inipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range End\")' />"+
                                 "</td></tr>");
    $("#ACLDataForm").append("<tr><td align='right'><%lang("External IP Range");%>:</td><td>"+
                                 "<%lang("From");%>:<input id='extipaddr_start' name='extipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range Start\")' />"+
                                 "<%lang("To");%>:<input id='extipaddr_end' name='extipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range End\")' />"+
                                 "</td></tr>");
    $("#ACLDataForm").append("<tr><td align='right'><%lang("Protocol");%>:</td><td>"+
                                 "<select id='proto' name='proto'>"+
                                 "  <option value=''>*</option><option value='tcp'>TCP</option><option value='udp'>UDP</option><option value='both'>TCP/UDP</option>"+
                                 "</select>"+
                                 "</td></tr>");
    $("#ACLDataForm").append("<tr><td align='right'><%lang("Service Port Range");%>:</td><td>"+
                                 "<%lang("From");%>:<input id='extport_start' name='extport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' disabled='true' />"+
                                 "<%lang("To");%>:<input id='extport_end' name='extport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' disabled='true' />"+
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


$(document).ready(function(){
    var ACL_rows = GetGridData();

    jQuery('#ACLListData').jqGrid({
            data: ACL_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Name");%>", "<%lang("Rule Enable");%>", "<%lang("External Interface");%>", "<%lang("Internal IP Range");%>", "<%lang("External IP Range");%>","<%lang("Protocol");%>","<%lang("Service Port Range");%>", "<%lang("Action");%>"],
            colModel:[
                      {name:'name',index:'name', width:8, align:"center", sortable:false, editable:true},
                      {name:'enable',index:'enable', width:5, align:"center", sortable:false, editable:true},
                      {name:'interface',index:'interface', width:5, align:"center", sortable:false, editable:true},
                      {name:'internalip',index:'internalip', width:8, align:"left", sortable:false, editable:true},
                      {name:'externalip',index:'externalip', width:8, align:"left", sortable:false, editable:true},
                      {name:'protocol',index:'protocol', width:5, align:"center", sortable:false, editable:true},
                      {name:'portrange',index:'portrange', width:6, align:"left", sortable:false, editable:true},
                      {name:'op',index:'op', width:5, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#ACLListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("Access Control List");%> (ACL) <%lang("Rule");%>"
    });

    jQuery("#ACLListData").jqGrid('navGrid','#ACLListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#ACLListData").navButtonAdd('#ACLListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#ACLDataForm").empty();
            CreateACLDataForm();

            $( "#ACLListData-dialog" ).dialog({
                resizable: false,
                height:400,
                width:450,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        fl_rule.push(Array_Tmp);
                        var ACL_rows = GetGridData();

                        $("#ACLListData").jqGrid("clearGridData", true);
                        jQuery("#ACLListData").setGridParam({ 'data':  ACL_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#ACLListData").navButtonAdd('#ACLListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#ACLDataForm").empty();
            var rowid = $("#ACLListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                fl_rule.splice(DelItem,1);
            }
            var ACL_rows = GetGridData();
            $("#ACLListData").jqGrid("clearGridData", true);
            jQuery("#ACLListData").setGridParam({ 'data':  ACL_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#ACLListData").navButtonAdd('#ACLListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#ACLDataForm").empty();
            CreateACLDataForm();
            var rowid = $("#ACLListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            $("#name").val(fl_rule[ModifyIndex][enum_fl_basic._NAME]);
            if (fl_rule[ModifyIndex][enum_fl_basic._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("select[name='extif']").attr("value", fl_rule[ModifyIndex][enum_fl_basic._EXT_IF]);
            $("#inipaddr_start").val(fl_rule[ModifyIndex][enum_fl_basic._IN_IPADDR_START]);
            $("#inipaddr_end").val(fl_rule[ModifyIndex][enum_fl_basic._IN_IPADDR_END]);
            $("#extipaddr_start").val(fl_rule[ModifyIndex][enum_fl_basic._EXT_IPADDR_START]);
            $("#extipaddr_end").val(fl_rule[ModifyIndex][enum_fl_basic._EXT_IPADDR_END]);
            $("select[name='proto']").attr("value", fl_rule[ModifyIndex][enum_fl_basic._PROTOCOL]);
            if (fl_rule[ModifyIndex][enum_fl_basic._PROTOCOL] == ""){
                $("#extport_start").attr("disabled",true);
                $("#extport_end").attr("disabled",true);
            }else{
                $("#extport_start").attr("disabled",false);
                $("#extport_end").attr("disabled",false);
            }
            $("#extport_start").val(fl_rule[ModifyIndex][enum_fl_basic._EXT_PORT_START]);
            $("#extport_end").val(fl_rule[ModifyIndex][enum_fl_basic._EXT_PORT_END])
            $("select[name='op']").attr("value", fl_rule[ModifyIndex][enum_fl_basic._ACTION]);

            $( "#ACLListData-dialog" ).dialog({
                resizable: false,
                height:400,
                width:450,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        fl_rule.splice(ModifyIndex,1, Array_Tmp);

                        var ACL_rows = GetGridData();
                        $("#ACLListData").jqGrid("clearGridData", true);
                        jQuery("#ACLListData").setGridParam({ 'data':  ACL_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

    jQuery("#ACLListData").navButtonAdd('#ACLListDataPager',{
       caption:"<% lang("Up"); %>", 
       buttonicon:"ui-icon-circle-arrow-n",
       id:"ItemUp",
       onClickButton: function(){
            $("#ACLDataForm").empty();
            var rowid = $("#ACLListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var UpItem = rowid - 1;
                if (UpItem == 0){
                    return false;
                }
                var arr_tmp = fl_rule[UpItem];
                fl_rule[UpItem] = fl_rule[UpItem - 1];
                fl_rule[UpItem - 1] = arr_tmp;
            }
            var ACL_rows = GetGridData();
            $("#ACLListData").jqGrid("clearGridData", true);
            $("#ACLListData").setGridParam({ 'data':  ACL_rows }).trigger("reloadGrid");
            $("#ACLListData").setSelection(parseInt(rowid)-1, true);

       }
    });

    jQuery("#ACLListData").navButtonAdd('#ACLListDataPager',{
       caption:"<% lang("Down"); %>", 
       buttonicon:"ui-icon-circle-arrow-s",
       id:"ItemDown",
      onClickButton: function(){
            $("#ACLDataForm").empty();
            var rowid = $("#ACLListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DownItem = rowid - 1;
                if (DownItem == fl_rule.length-1){
                    return false;
                }
                var arr_tmp = fl_rule[DownItem];
                fl_rule[DownItem] = fl_rule[DownItem + 1];
                fl_rule[DownItem + 1] = arr_tmp;
            }
            var ACL_rows = GetGridData();
            $("#ACLListData").jqGrid("clearGridData", true);
            $("#ACLListData").setGridParam({ 'data':  ACL_rows }).trigger("reloadGrid");
            $("#ACLListData").setSelection(parseInt(rowid)+1, true);
       }
    });

});//End ready
</script>
<% tbl_get("fl_rule"); %>

</head>
  <body class="gui" onload="init();"> <% showad(); %>
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Security", "Access Control"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
          do_page_path('<%lang("Security");%>', '<%lang("Access Control");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>
              
              <fieldset>
                <legend><%lang("Access Control List");%> (ACL)</legend>
                <div class="setting">
                  <div class="label"><% lang("Access Control"); %></div>
                  <input type="radio" value="1" name="fl_enable" onclick="init()"
                        <%nv_match("fl_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="fl_enable" onclick="init()"
                        <%nv_match("fl_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                  <div class="label"><% lang("Default Access Control Action"); %></div>
                  <input type="radio" value="1" name="fl_rule_default" id="fl_rule_default"
                        <%nv_match("fl_rule_default","1","checked");%>><%lang("ALLOW");%>
                  </input>

                  <input type="radio" value="0" name="fl_rule_default" id="fl_rule_default"
                        <%nv_match("fl_rule_default","0","checked");%>><%lang("DENY");%>
                  </input>
                </div>
              </fieldset><br />
              <div id="ACLListData-dialog" title="ACL Data" style="display: none">
                <table id="ACLDataForm"></table>
              </div>
              <table id="ACLListData"></table>
              <div id="ACLListDataPager"></div>
                <%tbl_create_table("fl_table", "fl_rule", "1");%>
              <br />
              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick='window.location.reload()'/>
              </div>
            </form>
          </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
      <%widget_show();%>
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
    </div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_input_window("fl_table", "fl_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
