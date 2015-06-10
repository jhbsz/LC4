<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VPN / PPTP</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">
  
var pptpd_objects =
[ { name: 'pptpd_mtu' },
  { name: 'pptpd_remoteip' },
  { name: 'pptpd_remoteip_range' },
  { name: 'pptpd_dns_auto_enable' },
  { name: 'pptpd_dns' },
  { name: 'pptpd_chap_enable' },
  { name: 'pptpd_mschap_enable' },
  { name: 'pptpd_mschapv2_enable' },
  { name: 'pptpd_mppe128_enable' },
  { name: 'pptpd_proxyarp_enable' },
  { name: 'pptpd_nat_enable' }
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
        if ( Username == pptpd_user_rule[i][enum_pptp._NAME] ){
            alert("'"+Username +"' repeats.");
            return false;
        }
    }
    return true
}

var enum_pptp = {                                                                                                          
    _ENABLE:0,                                                                                                             
    _NAME:1,                                                                                                               
    _PROVIDER:2,                                                                                                           
    _PASSWD:3,                                                                                                             
    _IPADDR:4                                                                                                              
} 


function local_pack_key_value_pair(F, T, P) {
    /* Important */
    var row_num = pptpd_user_rule.length ;
    $("<input>").attr({type: "hidden", id: "pptpd_rule", name: "pptpd_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: pptpd_user_rule[i][enum_pptp._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_username', name: P + i + '_username' , value: pptpd_user_rule[i][enum_pptp._NAME] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_provider', name: P + i + '_provider' , value: pptpd_user_rule[i][enum_pptp._PROVIDER] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_passwd', name: P + i + '_passwd' , value: pptpd_user_rule[i][enum_pptp._PASSWD] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_ipaddr', name: P + i + '_ipaddr' , value: pptpd_user_rule[i][enum_pptp._IPADDR] }).appendTo("form");
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    var enable = 0;
    if ($('input[name=enable]').is(':checked')){
        enable = $('[name=enable]:checked').val();
    }

    rule_tmp[enum_pptp._ENABLE]   = enable;
    rule_tmp[enum_pptp._NAME]     = $('#username').val();
    rule_tmp[enum_pptp._PROVIDER] = '';
    rule_tmp[enum_pptp._PASSWD]   = $('#passwd').val();
    rule_tmp[enum_pptp._IPADDR]   = '';

    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "vpn_pptpd";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "pptpd_table", "pptpd_user_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {

    if ($('[name=pptpd_enable]:checked').val()==1){
        for (var i=0; i<pptpd_objects.length; i++  ){
            $("input[name="+ pptpd_objects[i]["name"] +"]").each(function(){
                $(this).attr("disabled",false);
            });
        }
        $("#AddItem").removeClass('ui-state-disabled');
        $("#DelItem").removeClass('ui-state-disabled');
        $("#ModifyItem").removeClass('ui-state-disabled');
    }else{
        for (var i=0; i<pptpd_objects.length; i++  ){
            $("input[name="+ pptpd_objects[i]["name"] +"]").each(function(){
                $(this).attr("disabled",true);
            });
        }
        $("#AddItem").addClass('ui-state-disabled');
        $("#DelItem").addClass('ui-state-disabled');
        $("#ModifyItem").addClass('ui-state-disabled');
    }
    
    if ($('[name=pptpd_dns_auto_enable]:checked').val()==1){
        $("#pptpd_dns").attr("disabled",true);
    }else{
        $("#pptpd_dns").attr("disabled",false);
    }

    <%widget_start();%>
}

function GetGridData(){
    var PPTP_rows = [];
    var PPTP_data = {};

    for (var i=0; i<pptpd_user_rule.length; i++ )  {
        PPTP_data = {};
        PPTP_data["enable"] = tbl_wrap_enable( pptpd_user_rule[i][enum_pptp._ENABLE] );
        PPTP_data["username"] = pptpd_user_rule[i][enum_pptp._NAME];
        PPTP_data["passwd"] = pptpd_user_rule[i][enum_pptp._PASSWD];
        PPTP_rows.push(PPTP_data);
    }
    return PPTP_rows;
}

function CreatePPTPDataForm(){
    $("#PPTPDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='enable' name='enable' type='checkbox' value='1' /></td></tr>");
    $("#PPTPDataForm").append("<tr><td align='right'><%lang("User Name");%>:</td><td><input id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/></td></tr>");
    $("#PPTPDataForm").append("<tr><td align='right'><%lang("Password");%>:</td><td><input id='passwd' name='passwd' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/></td></tr>");
}

$(document).ready(function(){
    var PPTP_rows = GetGridData();

    jQuery('#PPTPListData').jqGrid({
            data: PPTP_rows,
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
            pager: '#PPTPListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("User Rule");%>"
    });

    jQuery("#PPTPListData").jqGrid('navGrid','#PPTPListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#PPTPListData").navButtonAdd('#PPTPListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"AddItem",
       onClickButton: function(){
            $("#PPTPDataForm").empty();
            CreatePPTPDataForm();

            $( "#PPTPListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        if (UsernameCheck($('#username').val())== false){
                            return false
                        }
                        var Array_Tmp = local_generate_rule();
                        pptpd_user_rule.push(Array_Tmp);
                        var PPTP_rows = GetGridData();
                        $("#PPTPListData").jqGrid("clearGridData", true);
                        jQuery("#PPTPListData").setGridParam({ 'data':  PPTP_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#PPTPListData").navButtonAdd('#PPTPListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DelItem",
       onClickButton: function(){
            $("#PPTPDataForm").empty();
            var rowid = $("#PPTPListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                pptpd_user_rule.splice(DelItem,1);
            }
            var PPTP_rows = GetGridData();
            $("#PPTPListData").jqGrid("clearGridData", true);
            jQuery("#PPTPListData").setGridParam({ 'data':  PPTP_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#PPTPListData").navButtonAdd('#PPTPListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"ModifyItem",
       onClickButton: function(){
            $("#PPTPDataForm").empty();
            CreatePPTPDataForm();
            var rowid = $("#PPTPListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            if (pptpd_user_rule[ModifyIndex][enum_pptp._ENABLE] == 1){
                $("#enable").attr('checked',true);
            }else{
                $("#enable").attr('checked',false);
            }
            $("#username").val(pptpd_user_rule[ModifyIndex][enum_pptp._NAME]);
            $("#passwd").val(pptpd_user_rule[ModifyIndex][enum_pptp._PASSWD]);

            $( "#PPTPListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("OK"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }

                        var Array_Tmp = local_generate_rule();
                        pptpd_user_rule.splice(ModifyIndex,1, Array_Tmp);

                        var PPTP_rows = GetGridData();
                        $("#PPTPListData").jqGrid("clearGridData", true);
                        jQuery("#PPTPListData").setGridParam({ 'data':  PPTP_rows }).trigger("reloadGrid");
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
          <% do_menu("Security", "VPN / PPTP"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '<%lang("VPN / PPTP");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('pptpd_rule')</script>	
              <input type="hidden" name="pptpd_name" 
                    value='<%nvg_attr_get("pptpd_name", "pptpd_rule","0","name");%>' />
              <input type="hidden" name="pptpd_redialperiod" 
                    value='<%nvg_attr_get("pptpd_redialperiod", "pptpd_rule","0","redialperiod");%>' />
              <input type="hidden" name="pptpd_localip" 
                    value='<%nvg_attr_get("pptpd_localip", "pptpd_rule","0","localip");%>' />
              <input type="hidden" name="pptpd_localip_range" 
                    value='<%nvg_attr_get("pptpd_localip_range", "pptpd_rule","0","localip_range");%>' />
              <input type="hidden" name="pptpd_eap_enable" 
                    value='<%nvg_attr_get("pptpd_eap_enable", "pptpd_rule","0","eap_enable");%>' />
              <input type="hidden" name="pptpd_pap_enable" 
                    value='<%nvg_attr_get("pptpd_pap_enable", "pptpd_rule","0","pap_enable");%>' />
              <fieldset>
                <legend><%lang("PPTP");%></legend>
                <div class="setting">
                  <div class="label"><% lang("PPTP"); %></div>
                  <input type="radio" value="1" name="pptpd_enable" id="pptpd_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="pptpd_enable" id="pptpd_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><%lang("MTU");%></div>
                  <input name="pptpd_mtu" id="pptpd_mtu" size="4" maxLength="4" 
                      value='<%nvg_attr_get("pptpd_mtu", "pptpd_rule", "0", "mtu");%>'
                      onblur="valid_number(this, 'PPTP MTU')" /> <%lang("Bytes");%>
                </div>

                <div class="setting">
                  <div class="label"><%lang("VPN Start IP Address");%></div>
                  <input name="pptpd_remoteip" id="pptpd_remoteip" size="20" maxLength="15" 
                      value='<%nvg_attr_get("pptpd_remoteip", "pptpd_rule", "0", "remoteip");%>'
                      onblur="valid_ipaddr(this, 'PPTP Local IP')" />
                </div>

                <div class="setting">
                  <div class="label"><%lang("Max VPN Clients");%></div>
                    <input size="3" maxLength="3" name="pptpd_remoteip_range" id="pptpd_remoteip_range" 
                          value='<%nvg_attr_get("pptpd_remoteip_range", "pptpd_rule", "$0", "remoteip_range");%>'
                          onblur='valid_number(this, "PPTP Remote IP Range")' />
                </div>

                <div class="setting">
                  <div class="label"><% lang("Auto DNS"); %></div>
                  <input type="radio" value="1" name="pptpd_dns_auto_enable" id="pptpd_dns_auto_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_dns_auto_enable" id="pptpd_dns_auto_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><%lang("DNS");%></div>
                  <input type="text" maxLength="15" size="20" name="pptpd_dns" id="pptpd_dns"
                      value='<%nvg_attr_get("pptpd_dns", "pptpd_rule", "$0", "dns");%>'
                      onblur='valid_reserved_word(this, "PPTP DNS")' />
                </div>

                <div class="setting">
                  <div class="label"><% lang("CHAP Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_chap_enable" id="pptpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_chap_enable" id="pptpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("MSCHAP Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_mschap_enable" id="pptpd_mschap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_mschap_enable" id="pptpd_mschap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("MSCHAP v2 Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_mschapv2_enable" id="pptpd_mschapv2_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_mschapv2_enable" id="pptpd_mschapv2_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("MPPE128 Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_mppe128_enable" id="pptpd_mppe128_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_mppe128_enable" id="pptpd_mppe128_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("Proxy ARP Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_proxyarp_enable" id="pptpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_proxyarp_enable" id="pptpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("NAT Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_nat_enable" id="pptpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_nat_enable" id="pptpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />
              <div id="PPTPListData-dialog" title="PPTP Data" style="display: none">
                <table id="PPTPDataForm"></table>
              </div>
              <table id="PPTPListData"></table>
              <div id="PPTPListDataPager"></div>
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
