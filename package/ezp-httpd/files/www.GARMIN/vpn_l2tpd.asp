<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VPN / L2TP</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var l2tpd_objects =
[ { name: 'l2tpd_mtu' },
  { name: 'l2tpd_remoteip' },
  { name: 'l2tpd_remoteip_range' },
  { name: 'l2tpd_dns_auto_enable' },
  { name: 'l2tpd_dns' },
  { name: 'l2tpd_chap_enable' },
  { name: 'l2tpd_proxyarp_enable' },
  { name: 'l2tpd_nat_enable' },
  { id: 'pptpd_table_add' },
  { id: 'pptpd_table_delete' },
  { id: 'pptpd_table_modify' },
  { id: 'pptpd_table_up' },
  { id: 'pptpd_table_down' },
  { id: 'pptpd_table_element' }
];
var F; 
function local_rule_check(T) {
    if ((!$('#username').val())) {
        alert('User name is required.');
        return false;
    }

    if (!$('#passwd').val()) {
        alert('Password is required.');
        return false;
    }

    return true;
}

function UsernameCheck(Username) {
    for (var i=0; i<pptpd_user_rule.length; i++){
        if ( Username == pptpd_user_rule[i][enum_l2tp._NAME] ){
            alert("'"+Username +"' repeats.");
            return false;
        }
    }
    return true
}

var enum_l2tp = {                                                                                                          
    _ENABLE:0,                                                                                                             
    _NAME:1,                                                                                                               
    _PROVIDER:2,                                                                                                           
    _PASSWD:3,                                                                                                             
    _IPADDR:4                                                                                                              
} 


function local_pack_key_value_pair(F, T, P) {
    /* Important */
    var row_num = pptpd_user_rule.length ;
    $("<input>").attr({type: "hidden", id: "l2tpd_rule", name: "l2tpd_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: pptpd_user_rule[i][enum_l2tp._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_username', name: P + i + '_username' , value: pptpd_user_rule[i][enum_l2tp._NAME] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_provider', name: P + i + '_provider' , value: pptpd_user_rule[i][enum_l2tp._PROVIDER] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_passwd', name: P + i + '_passwd' , value: pptpd_user_rule[i][enum_l2tp._PASSWD] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_ipaddr', name: P + i + '_ipaddr' , value: pptpd_user_rule[i][enum_l2tp._IPADDR] }).appendTo("form");
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();
    var enable = 0;
    if ($('input[name=enable]').is(':checked')){
        enable = $('[name=enable]:checked').val();
    }

    rule_tmp[enum_l2tp._ENABLE]   = enable;
    rule_tmp[enum_l2tp._NAME]     = $('#username').val();
    rule_tmp[enum_l2tp._PROVIDER] = '';
    rule_tmp[enum_l2tp._PASSWD]   = $('#passwd').val();
    rule_tmp[enum_l2tp._IPADDR]   = '';

    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "vpn_l2tpd";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "pptpd_table", "pptpd_user_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}

function init() {
    if ($('[name=l2tpd_enable]:checked').val()==1){
        for (var i=0; i<l2tpd_objects.length; i++  ){
            $("input[name="+ l2tpd_objects[i]["name"] +"]").each(function(){
                $(this).attr("disabled",false);
            });
        }
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
    }else{
        for (var i=0; i<l2tpd_objects.length; i++  ){
            $("input[name="+ l2tpd_objects[i]["name"] +"]").each(function(){
                $(this).attr("disabled",true);
            });
        }
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
    }
    
    if ($('[name=l2tpd_dns_auto_enable]:checked').val()==1){
        $("#l2tpd_dns").attr("disabled",true);
    }else{
        $("#l2tpd_dns").attr("disabled",false);
    }
    <%widget_start();%>
}

function GetGridData(){
    var L2TP_rows = [];
    var L2TP_data = {};

    for (var i=0; i<pptpd_user_rule.length; i++ )  {
        L2TP_data = {};
        L2TP_data["enable"] = tbl_wrap_enable( pptpd_user_rule[i][enum_l2tp._ENABLE] );
        L2TP_data["username"] = pptpd_user_rule[i][enum_l2tp._NAME];
        L2TP_data["passwd"] = pptpd_user_rule[i][enum_l2tp._PASSWD];
        L2TP_rows.push(L2TP_data);
    }
    return L2TP_rows;
}

function CreateL2TPDataForm(){
    $("#L2TPDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
    $("#L2TPDataForm").append("<tr><td align='right'><%lang("User Name");%>:</td><td><input id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/></td></tr>");
    $("#L2TPDataForm").append("<tr><td align='right'><%lang("Password");%>:</td><td><input id='passwd' name='passwd' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/></td></tr>");
}

$(document).ready(function(){
    var L2TP_rows = GetGridData();

    jQuery('#L2TPListData').jqGrid({
            data: L2TP_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Enable");%>", "<%lang("User Name");%>", "<%lang("Password");%>"],
            colModel:[
                      {name:'enable',index:'enable', width:3, align:"center", sortable:false, editable:true},
                      {name:'username',index:'username', width:8, align:"center", sortable:false, editable:true},
                      {name:'passwd',index:'passwd', width:8, align:"center", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#L2TPListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("User Rule");%>"
    });

    jQuery("#L2TPListData").jqGrid('navGrid','#L2TPListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#L2TPListData").navButtonAdd('#L2TPListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#L2TPDataForm").empty();
            CreateL2TPDataForm();

            $( "#L2TPListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        if (UsernameCheck($('#username').val())== false){
                            return false
                        }
                        var Array_Tmp = local_generate_rule();
                        pptpd_user_rule.push(Array_Tmp);
                        var L2TP_rows = GetGridData();
                        $("#L2TPListData").jqGrid("clearGridData", true);
                        jQuery("#L2TPListData").setGridParam({ 'data':  L2TP_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#L2TPListData").navButtonAdd('#L2TPListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#L2TPDataForm").empty();
            var rowid = $("#L2TPListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                pptpd_user_rule.splice(DelItem,1);
            }
            var L2TP_rows = GetGridData();
            $("#L2TPListData").jqGrid("clearGridData", true);
            jQuery("#L2TPListData").setGridParam({ 'data':  L2TP_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#L2TPListData").navButtonAdd('#L2TPListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#L2TPDataForm").empty();
            CreateL2TPDataForm();
            var rowid = $("#L2TPListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            if (pptpd_user_rule[ModifyIndex][enum_l2tp._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("#username").val(pptpd_user_rule[ModifyIndex][enum_l2tp._NAME]);
            $("#passwd").val(pptpd_user_rule[ModifyIndex][enum_l2tp._PASSWD]);

            $( "#L2TPListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }

                        var Array_Tmp = local_generate_rule();
                        pptpd_user_rule.splice(ModifyIndex,1, Array_Tmp);

                        var L2TP_rows = GetGridData();
                        $("#L2TPListData").jqGrid("clearGridData", true);
                        jQuery("#L2TPListData").setGridParam({ 'data':  L2TP_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });


});//End ready

</script>
<% tbl_get("pptpd_user_rule"); %>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Security", "VPN / L2TP"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '<%lang("VPN / L2TP");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('l2tpd_rule')</script>	
              <input type="hidden" name="l2tpd_name" 
                    value='<%nvg_attr_get("l2tpd_name", "l2tpd_rule","0","name");%>' />
              <input type="hidden" name="l2tpd_localip" 
                    value='<%nvg_attr_get("l2tpd_localip", "l2tpd_rule","0","localip");%>' />
              <input type="hidden" name="l2tpd_localip_range" 
                    value='<%nvg_attr_get("l2tpd_localip_range", "l2tpd_rule","0","localip_range");%>' />
              <input type="hidden" name="l2tpd_pap_enable" 
                    value='<%nvg_attr_get("l2tpd_pap_enable", "l2tpd_rule","0","pap_enable");%>' />
              <fieldset>
                <legend><%lang("L2TP");%></legend>
                <div class="setting">
                  <div class="label"><% lang("L2TP"); %></div>
                  <input type="radio" value="1" name="l2tpd_enable" id="l2tpd_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_enable","l2tpd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="l2tpd_enable" id="l2tpd_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_enable","l2tpd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><%lang("MTU");%></div>
                  <input name="l2tpd_mtu" id="l2tpd_mtu" size="4" maxLength="4" 
                      value='<%nvg_attr_get("l2tpd_mtu", "l2tpd_rule", "0", "mtu");%>'
                      onblur="valid_number(this, 'L2TP MTU')" /> <%lang("Bytes");%>
                </div>

                <div class="setting">
                  <div class="label"><%lang("VPN Start IP Address");%></div>
                  <input name="l2tpd_remoteip" id="l2tpd_remoteip" size="20" maxLength="15" 
                      value='<%nvg_attr_get("l2tpd_remoteip", "l2tpd_rule", "0", "remoteip");%>'
                      onblur="valid_ipaddr(this, 'L2TP Local IP')" />
                </div>

                <div class="setting">
                  <div class="label"><%lang("Max VPN Clients");%></div>
                    <input size="3" maxLength="3" name="l2tpd_remoteip_range" id="l2tpd_remoteip_range" 
                          value='<%nvg_attr_get("l2tpd_remoteip_range", "l2tpd_rule", "$0", "remoteip_range");%>'
                          onblur='valid_number(this, "L2TP Remote IP Range")' />
                </div>

                <div class="setting">
                  <div class="label"><% lang("Auto DNS"); %></div>
                  <input type="radio" value="1" name="l2tpd_dns_auto_enable" id="l2tpd_dns_auto_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_dns_auto_enable","l2tpd_rule","0","dns_auto_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="l2tpd_dns_auto_enable" id="l2tpd_dns_auto_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_dns_auto_enable","l2tpd_rule","0","dns_auto_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><%lang("DNS");%></div>
                  <input type="text" maxLength="15" size="20" name="l2tpd_dns" id="l2tpd_dns"
                      value='<%nvg_attr_get("l2tpd_dns", "l2tpd_rule", "$0", "dns");%>'
                      onblur='valid_reserved_word(this, "L2TP DNS")' />
                </div>

                <div class="setting">
                  <div class="label"><% lang("CHAP Enable"); %></div>
                  <input type="radio" value="1" name="l2tpd_chap_enable" id="l2tpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_chap_enable","l2tpd_rule","0","chap_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="l2tpd_chap_enable" id="l2tpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_chap_enable","l2tpd_rule","0","chap_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("Proxy ARP Enable"); %></div>
                  <input type="radio" value="1" name="l2tpd_proxyarp_enable" id="l2tpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_proxyarp_enable","l2tpd_rule","0","proxyarp_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="l2tpd_proxyarp_enable" id="l2tpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_proxyarp_enable","l2tpd_rule","0","proxyarp_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("NAT Enable"); %></div>
                  <input type="radio" value="1" name="l2tpd_nat_enable" id="l2tpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_nat_enable","l2tpd_rule","0","nat_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="l2tpd_nat_enable" id="l2tpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("l2tpd_nat_enable","l2tpd_rule","0","nat_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />
              <div id="L2TPListData-dialog" title="L2TP Data" style="display: none">
                <table id="L2TPDataForm"></table>
              </div>
              <table id="L2TPListData"></table>
              <div id="L2TPListDataPager"></div>
                <%tbl_create_table("pptpd_table", "pptpd_user_rule", "1");%>
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
    <script type="text/javascript">create_input_window("pptpd_table", "pptpd_user_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
