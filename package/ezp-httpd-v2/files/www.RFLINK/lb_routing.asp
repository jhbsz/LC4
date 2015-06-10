<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Routing Policy</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
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
    F.submit_button.value = "lb_routing";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "rt_table", "rt_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}

var F;
function init() {
    
    if ($('[name=rt_enable]:checked').val()==1){
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
    var Routing_rows = [];
    var Routing_data = {};
    
    for (var i=0; i<rt_rule.length; i++ )  {
        Routing_data = {};
        
        Routing_data["name"] = rt_rule[i][enum_lb_routing._NAME];
        Routing_data["enable"] = tbl_wrap_enable( rt_rule[i][enum_lb_routing._ENABLE] );
        Routing_data["internalip"] = "<%lang("From");%>:" + rt_rule[i][enum_lb_routing._IN_IPADDR_START] + "<br><%lang("To");%>:" + rt_rule[i][enum_lb_routing._IN_IPADDR_END];
        Routing_data["externalip"] = "<%lang("From");%>:" + rt_rule[i][enum_lb_routing._EXT_IPADDR_START] + "<br><%lang("To");%>:" + rt_rule[i][enum_lb_routing._EXT_IPADDR_END];
        Routing_data["protocol"] = tbl_wrap_proto(rt_rule[i][enum_lb_routing._PROTOCOL]);
        Routing_data["portrange"] = "<%lang("From");%>:" + rt_rule[i][enum_lb_routing._EXT_PORT_START] + "<br><%lang("To");%>:" + rt_rule[i][enum_lb_routing._EXT_PORT_END];
        Routing_data["interface"] = tbl_wrap_wan( rt_rule[i][enum_lb_routing._EXT_IF] );
        Routing_data["type"] = rt_rule[i][enum_lb_routing._ROUTING_TYPE];
        Routing_data["gateway"] = rt_rule[i][enum_lb_routing._GATEWAY_IPADDR];
        
        Routing_rows.push(Routing_data);
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
                                 "<%lang("From");%>:<input id='inipaddr_start' name='inipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range Start\")' />"+
                                 "<%lang("To");%>:<input id='inipaddr_end' name='inipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range End\")' />"+
                                 "</td></tr>");
    $("#RoutingDataForm").append("<tr><td align='right'><%lang("External IP Range");%>:</td><td>"+
                                 "<%lang("From");%>:<input id='extipaddr_start' name='extipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range Start\")' />"+
                                 "<%lang("To");%>:<input id='extipaddr_end' name='extipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"External IP Range End\")' />"+
                                 "</td></tr>");
    $("#RoutingDataForm").append("<tr><td align='right'><%lang("Protocol");%>:</td><td>"+
                                 "<select id='proto' name='proto'>"+
                                 "  <option value=''>*</option><option value='tcp'>TCP</option><option value='udp'>UDP</option><option value='both'>TCP/UDP</option>"+
                                 "</select>"+
                                 "</td></tr>");
    $("#RoutingDataForm").append("<tr><td align='right'><%lang("Service Port Range");%>:</td><td>"+
                                 "<%lang("From");%>:<input id='extport_start' name='extport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' disabled='true' />"+
                                 "<%lang("To");%>:<input id='extport_end' name='extport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' disabled='true' />"+
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
                                 "<input id='gateway_ipaddr' name='gateway_ipaddr' size='17' maxlength='15' disabled='true' onblur='valid_ipaddr(this, \"Gateway IP Address\")' /></td></tr>");

    for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
        $('#extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
    }
    for (var i = 1; i <= <%nv_get("lan_num");%>; i++) {
        $('#extif').append(new Option("LAN" + i , "lan" + (i-1), false, false));
    }

}

$(document).ready(function(){
    var Routing_rows = GetGridData();

    jQuery('#RoutingListData').jqGrid({
            data: Routing_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Name");%>","<%lang("Enable");%>","<%lang("Internal IP Range");%>","<%lang("External IP Range");%>","<%lang("Protocol");%>","<%lang("Service Port Range");%>","<%lang("External Interface");%>","<%lang("Routing Type");%>","<%lang("Gateway");%>"],
            colModel:[
                      {name:'name',index:'name', width:5, align:"center", sortable:false, editable:true},
                      {name:'enable',index:'enable', width:2, align:"center", sortable:false, editable:true},
                      {name:'internalip',index:'internalip', width:8, align:"left", sortable:false, editable:true},
                      {name:'externalip',index:'externalip', width:8, align:"left", sortable:false, editable:true},
                      {name:'protocol',index:'protocol', width:5, align:"center", sortable:false, editable:true},
                      {name:'portrange',index:'portrange', width:6, align:"left", sortable:false, editable:true},
                      {name:'interface',index:'interface', width:5, align:"center", sortable:false, editable:true},
                      {name:'type',index:'type', width:6, align:"center", sortable:false, editable:true},
                      {name:'gateway',index:'gateway', width:7, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#RoutingListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("Routing Rule");%>"
    });

    jQuery("#RoutingListData").jqGrid('navGrid','#RoutingListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#RoutingListData").navButtonAdd('#RoutingListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#RoutingDataForm").empty();
            CreateRoutingDataForm();

            $( "#RoutingListData-dialog" ).dialog({
                resizable: false,
                height:400,
                width:450,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule()
                        rt_rule.push(Array_Tmp);
                        var Routing_rows = GetGridData();

                        $("#RoutingListData").jqGrid("clearGridData", true);
                        jQuery("#RoutingListData").setGridParam({ 'data':  Routing_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            
       }
    });

    jQuery("#RoutingListData").navButtonAdd('#RoutingListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#RoutingDataForm").empty();
            var rowid = $("#RoutingListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                rt_rule.splice(DelItem,1);
            }
            var Routing_rows = GetGridData();
            $("#RoutingListData").jqGrid("clearGridData", true);
            jQuery("#RoutingListData").setGridParam({ 'data':  Routing_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#RoutingListData").navButtonAdd('#RoutingListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#RoutingDataForm").empty();
            CreateRoutingDataForm();
            var rowid = $("#RoutingListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            $("#name").val(rt_rule[ModifyIndex][enum_lb_routing._NAME]);
            if (rt_rule[ModifyIndex][enum_lb_routing._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("#inipaddr_start").val(rt_rule[ModifyIndex][enum_lb_routing._IN_IPADDR_START]);
            $("#inipaddr_end").val(rt_rule[ModifyIndex][enum_lb_routing._IN_IPADDR_END]);
            $("#extipaddr_start").val(rt_rule[ModifyIndex][enum_lb_routing._EXT_IPADDR_START]);
            $("#extipaddr_end").val(rt_rule[ModifyIndex][enum_lb_routing._EXT_IPADDR_END]);
            $("select[name='proto']").attr("value", rt_rule[ModifyIndex][enum_lb_routing._PROTOCOL]);
            if (rt_rule[ModifyIndex][enum_lb_routing._PROTOCOL] == ""){
                $("#extport_start").attr("disabled",true);
                $("#extport_end").attr("disabled",true);
            }else{
                $("#extport_start").attr("disabled",false);
                $("#extport_end").attr("disabled",false);
            }
            $("#extport_start").val(rt_rule[ModifyIndex][enum_lb_routing._EXT_PORT_START]);
            $("#extport_end").val(rt_rule[ModifyIndex][enum_lb_routing._EXT_PORT_END])
            $("select[name='extif']").attr("value", rt_rule[ModifyIndex][enum_lb_routing._EXT_IF]);
            $("select[name='routing_type']").attr("value", rt_rule[ModifyIndex][enum_lb_routing._ROUTING_TYPE]);
            if (rt_rule[ModifyIndex][enum_lb_routing._ROUTING_TYPE]=="default"){
                $("#gateway_ipaddr").attr("disabled",true);
            }else{
                $("#gateway_ipaddr").attr("disabled",false);
            }
            $("#gateway_ipaddr").val(rt_rule[ModifyIndex][enum_lb_routing._GATEWAY_IPADDR]);

            $( "#RoutingListData-dialog" ).dialog({
                resizable: false,
                height:400,
                width:450,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule()
                        rt_rule.splice(ModifyIndex,1, Array_Tmp);

                        var Routing_rows = GetGridData();
                        $("#RoutingListData").jqGrid("clearGridData", true);
                        jQuery("#RoutingListData").setGridParam({ 'data':  Routing_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });


    jQuery("#RoutingListData").navButtonAdd('#RoutingListDataPager',{
       caption:"<% lang("Up"); %>", 
       buttonicon:"ui-icon-circle-arrow-n",
       id:"ItemUp",
       onClickButton: function(){
            $("#RoutingDataForm").empty();
            var rowid = $("#RoutingListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var UpItem = rowid - 1;
                if (UpItem == 0){
                    return false;
                }
                var arr_tmp = rt_rule[UpItem];
                rt_rule[UpItem] = rt_rule[UpItem - 1];
                rt_rule[UpItem - 1] = arr_tmp;
            }
            var Routing_rows = GetGridData();
            $("#RoutingListData").jqGrid("clearGridData", true);
            $("#RoutingListData").setGridParam({ 'data':  Routing_rows }).trigger("reloadGrid");
            $("#RoutingListData").setSelection(parseInt(rowid)-1, true);

       }
    });

    jQuery("#RoutingListData").navButtonAdd('#RoutingListDataPager',{
       caption:"<% lang("Down"); %>", 
       buttonicon:"ui-icon-circle-arrow-s",
       id:"ItemDown",
      onClickButton: function(){
            $("#RoutingDataForm").empty();
            var rowid = $("#RoutingListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DownItem = rowid - 1;
                if (DownItem == rt_rule.length-1){
                    return false;
                }
                var arr_tmp = rt_rule[DownItem];
                rt_rule[DownItem] = rt_rule[DownItem + 1];
                rt_rule[DownItem + 1] = arr_tmp;
            }
            var Routing_rows = GetGridData();
            $("#RoutingListData").jqGrid("clearGridData", true);
            $("#RoutingListData").setGridParam({ 'data':  Routing_rows }).trigger("reloadGrid");
            $("#RoutingListData").setSelection(parseInt(rowid)+1, true);
       }
    });

});//End Ready

</script>
<% tbl_get("rt_rule"); %>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Setup", "Routing"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("Routing");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>	
              <fieldset>
                <legend><%lang("Routing");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Routing"); %></div>
                  <input type="radio" value="1" name="rt_enable" onclick="init()"
                        <%nv_match("rt_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="rt_enable" onclick="init()"
                        <%nv_match("rt_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />
              <div id="RoutingListData-dialog" title="Routing Data" style="display: none">
                <table id="RoutingDataForm"></table>
              </div>
              <table id="RoutingListData"></table>
              <div id="RoutingListDataPager"></div>
              <%tbl_create_table("rt_table", "rt_rule", "1");%>
              <br/>
              <div class="submitFooter">
                <input type="button" name="save_button" value=<%lang("Save Settings");%>
                         onClick="to_submit(this.form)" />
                <input type="reset" value=<%lang("Cancel Changes");%>
                         onClick='window.location.reload()'/>
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
    <script type="text/javascript">create_input_window("rt_table", "rt_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
