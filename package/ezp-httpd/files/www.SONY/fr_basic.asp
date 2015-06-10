<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Port Forwarding</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var dmz_objects = new Array(<%nv_get("wan_num");%>);
var F;

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

function to_submit(F) {
	F.submit_button.value = "fr_basic";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "fr_table", "fr_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    for (var i=0; i<dmz_objects.length; i++ ){
        if ($('[name=fr_dmz'+i+'_enable]:checked').val()==1){
            for (var j=0; j<dmz_objects[i].length; j++ ){
                $("#"+dmz_objects[i][j].name).attr("disabled",false);
            }
        }else{
            for (var j=0; j<dmz_objects[i].length; j++ ){
                $("#"+dmz_objects[i][j].name).attr("disabled",true);
            }
        }
    }
    
    if ($('[name=fr_enable]:checked').val()==1){
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
        $("#ItemUp").removeClass('ui-state-disabled');
        $("#ItemDown").removeClass('ui-state-disabled');
    }else{
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
        $("#ItemUp").addClass('ui-state-disabled');
        $("#ItemDown").addClass('ui-state-disabled');
    }
    
    <%widget_start();%>
}

function GetGridData(){
    var Forward_rows = [];
    var Forward_data = {};
    
    for (var i=0; i<fr_rule.length; i++ )  {
        Forward_data = {};
        Forward_data["name"] = fr_rule[i][enum_fr_basic._NAME];
        Forward_data["enable"] = tbl_wrap_enable( fr_rule[i][enum_fr_basic._ENABLE] );
        Forward_data["extif"] = tbl_wrap_wan( fr_rule[i][enum_fr_basic._EXT_IF] );
        Forward_data["proto"] = tbl_wrap_proto( fr_rule[i][enum_fr_basic._PROTOCOL] );
        Forward_data["extport"] = "<%lang("From");%>:" + fr_rule[i][enum_fr_basic._EXT_PORT_START] + "<br><%lang("To");%>:"+fr_rule[i][enum_fr_basic._EXT_PORT_END];
        Forward_data["ipaddr"] = fr_rule[i][enum_fr_basic._IP_ADDR];
        Forward_data["inport"] = "<%lang("From");%>:" + fr_rule[i][enum_fr_basic._IN_PORT_START] + "<br/><%lang("To");%>:" + fr_rule[i][enum_fr_basic._IN_PORT_END];
        
        Forward_rows.push(Forward_data);
    }
    return Forward_rows;
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
                                 "<%lang("From");%>:<input id='extport_start' name='extport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"External Port Range Start\")' />"+
                                 "<%lang("To");%>:<input id='extport_end' name='extport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"External Port Range End\")' />"+
                                 "</td></tr>");
    $("#ForwardDataForm").append("<tr><td align='right'><%lang("Internal IP");%>:</td><td><input id='ipaddr' name='ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></td></tr>");
    $("#ForwardDataForm").append("<tr><td align='right'><%lang("Internal Port Range");%>:</td><td>"+
                                 "<%lang("From");%>:<input id='inport_start' name='inport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Internal Port Range Start\")' />"+
                                 "<%lang("To");%>:<input id='inport_end' name='inport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Internal Port Range End\")' />"+
                                 "</td></tr>");

    for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
        $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
    }

}


$(document).ready(function(){
    var Forward_rows = GetGridData();

    jQuery('#ForwardListData').jqGrid({
            data: Forward_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Name");%>", "<%lang("Rule Enable");%>", "<%lang("External Interface");%>", "<%lang("Protocol");%>", "<%lang("External Port Range");%>", "<%lang("Internal IP");%>", "<%lang("Internal Port Range");%>"],
            colModel:[
                      {name:'name',index:'name', width:5, align:"center", sortable:false, editable:true},
                      {name:'enable',index:'enable', width:5, align:"center", sortable:false, editable:true},
                      {name:'extif',index:'extif', width:5, align:"center", sortable:false, editable:true},
                      {name:'proto',index:'proto', width:5, align:"center", sortable:false, editable:true},
                      {name:'extport',index:'extport', width:5, align:"left", sortable:false, editable:true},
                      {name:'ipaddr',index:'ipaddr', width:6, align:"center", sortable:false, editable:true},
                      {name:'inport',index:'interface', width:5, align:"left", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#ForwardListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("Port Range Forwarding Rule");%>"
    });

    jQuery("#ForwardListData").jqGrid('navGrid','#ForwardListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#ForwardListData").navButtonAdd('#ForwardListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#ForwardDataForm").empty();
            CreateForwardDataForm();

            $( "#ForwardListData-dialog" ).dialog({
                resizable: false,
                height:350,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule()
                        fr_rule.push(Array_Tmp);
                        var Forward_rows = GetGridData();

                        $("#ForwardListData").jqGrid("clearGridData", true);
                        jQuery("#ForwardListData").setGridParam({ 'data':  Forward_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            
       }
    });

    jQuery("#ForwardListData").navButtonAdd('#ForwardListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#ForwardDataForm").empty();
            var rowid = $("#ForwardListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                fr_rule.splice(DelItem,1);
            }
            var Forward_rows = GetGridData();
            $("#ForwardListData").jqGrid("clearGridData", true);
            jQuery("#ForwardListData").setGridParam({ 'data':  Forward_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#ForwardListData").navButtonAdd('#ForwardListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#ForwardDataForm").empty();
            CreateForwardDataForm();
            var rowid = $("#ForwardListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            $("#name").val(fr_rule[ModifyIndex][enum_fr_basic._NAME]);
            if (fr_rule[ModifyIndex][enum_fr_basic._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("select[name='extif']").attr("value", fr_rule[ModifyIndex][enum_fr_basic._EXT_IF]);
            $("select[name='proto']").attr("value", fr_rule[ModifyIndex][enum_fr_basic._PROTOCOL]);
            $("#extport_start").val(fr_rule[ModifyIndex][enum_fr_basic._EXT_PORT_START]);
            $("#extport_end").val(fr_rule[ModifyIndex][enum_fr_basic._EXT_PORT_END]);
            $("#ipaddr").val(fr_rule[ModifyIndex][enum_fr_basic._IP_ADDR]);
            $("#inport_start").val(fr_rule[ModifyIndex][enum_fr_basic._IN_PORT_START]);
            $("#inport_end").val(fr_rule[ModifyIndex][enum_fr_basic._IN_PORT_END]);

            $( "#ForwardListData-dialog" ).dialog({
                resizable: false,
                height:350,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule()
                        fr_rule.splice(ModifyIndex,1, Array_Tmp);

                        var Forward_rows = GetGridData();
                        $("#ForwardListData").jqGrid("clearGridData", true);
                        jQuery("#ForwardListData").setGridParam({ 'data':  Forward_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

    jQuery("#ForwardListData").navButtonAdd('#ForwardListDataPager',{
       caption:"<% lang("Up"); %>", 
       buttonicon:"ui-icon-circle-arrow-n",
       id:"ItemUp",
       onClickButton: function(){
            $("#ForwardDataForm").empty();
            var rowid = $("#ForwardListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var UpItem = rowid - 1;
                if (UpItem == 0){
                    return false;
                }
                var arr_tmp = fr_rule[UpItem];
                fr_rule[UpItem] = fr_rule[UpItem - 1];
                fr_rule[UpItem - 1] = arr_tmp;
            }
            var Forward_rows = GetGridData();
            $("#ForwardListData").jqGrid("clearGridData", true);
            $("#ForwardListData").setGridParam({ 'data':  Forward_rows }).trigger("reloadGrid");
            $("#ForwardListData").setSelection(parseInt(rowid)-1, true);

       }
    });

    jQuery("#ForwardListData").navButtonAdd('#ForwardListDataPager',{
       caption:"<% lang("Down"); %>", 
       buttonicon:"ui-icon-circle-arrow-s",
       id:"ItemDown",
      onClickButton: function(){
            $("#ForwardDataForm").empty();
            var rowid = $("#ForwardListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DownItem = rowid - 1;
                if (DownItem == fr_rule.length-1){
                    return false;
                }
                var arr_tmp = fr_rule[DownItem];
                fr_rule[DownItem] = fr_rule[DownItem + 1];
                fr_rule[DownItem + 1] = arr_tmp;
            }
            var Forward_rows = GetGridData();
            $("#ForwardListData").jqGrid("clearGridData", true);
            $("#ForwardListData").setGridParam({ 'data':  Forward_rows }).trigger("reloadGrid");
            $("#ForwardListData").setSelection(parseInt(rowid)+1, true);
       }
    });

});//End ready

</script>
<% tbl_get("fr_rule"); %>

</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Applications", "Port Range Forward"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Applications");%>', '<%lang("Port Range Forward");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>	
              <input type="hidden" name="fr_dmz_num" value=<%nv_get("wan_num");%>/>
              <% fr_show_setting(); %>

              <fieldset>
                <legend><%lang("Port Range Forwarding");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Port Forwarding"); %></div>
                  <input type="radio" value="1" name="fr_enable" onclick="init()"
                        <%nv_match("fr_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="fr_enable" onclick="init()"
                        <%nv_match("fr_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />
              <div id="ForwardListData-dialog" title="Forward Data" style="display: none">
                <table id="ForwardDataForm"></table>
              </div>
              <table id="ForwardListData"></table>
              <div id="ForwardListDataPager"></div>
              <%tbl_create_table("fr_table", "fr_rule", "1");%>
              <br />
              <div class="submitFooter">
                <input type="button" name="save_button" value='<%lang("Save Settings");%>' onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%> "onClick="window.location.reload()" />
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
    <script type="text/javascript">create_input_window("fr_table", "fr_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
