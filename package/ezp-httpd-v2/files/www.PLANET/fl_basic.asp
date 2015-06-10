<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - ACL</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var F;

var fl_objects =
[ { id: 'fl_rule_default' },
  { id: 'fl_table_add' },
  { id: 'fl_table_delete' },
  { id: 'fl_table_modify' },
  { id: 'fl_table_up' },
  { id: 'fl_table_down' },
  { id: 'fl_table_element' }
];
 
var proto_objects =
[ { id: 'inport_start' },
  { id: 'inport_end' },
  { id: 'extport_start' },
  { id: 'extport_end' }
 
];

<%prod_show_js_start("PROD_CAT_B|PROD_CAT_L|PROD_CAT_M", "1", "7");%>
var action_objects =
[ { id: 'dbm_enable' }
];
<%prod_show_js_end("PROD_CAT_B|PROD_CAT_L|PROD_CAT_M", "1", "7");%>

var dbm_valid_objects =
[ { id: 'inipaddr_start', title: 'Internal IP Range Start' },
  { id: 'inipaddr_end', title: 'Internal IP Range End' }
];

function tbl_wrap_dbm_enable(v) {
    if (<%nv_get("bw_enable");%> == "1") {
        if (v == 1) {
            return "<img src='v_021.gif'>";
        } else {
            return "<img src='x_011.gif'>";
        }
    } else {
        if (v == 1) {
            return "<img src='v_022.gif'>";
        } else {
            return "<img src='x_012.gif'>";
        }
    }
}

function proto_sel_wrapper(v, check) {
    var F = document.getElementsByName("form")[0];
    if (v != check) {
        sel_enable_objs(F, proto_objects);
    } else {
        sel_disable_objs(F, proto_objects);
        E('extport_start').value = "";
        E('extport_end').value = "";
    }
}                                                                                                                                         

function action_sel_wrapper(v, check) {
    var F = document.getElementsByName("form")[0];
    if (v != <%lang("check");%> && <%nv_get("bw_enable");%> == '1') {
        sel_enable_objs(F, action_objects);
    } else {
        sel_disable_objs(F, action_objects);
        E('dbm_enable').checked = false;
    }
}

function valid_dbm_action(item) {
    var objs = dbm_valid_objects;
    if (item.checked == true) {
        for (var i = 0; i < objs.length; i++) {
            if (E(objs[i].id).value == '') {
                alert('DBM only works with the value of ' + objs[i].title + ' specified.');
                item.checked = false;
            }
        }
    }
}

function local_rule_check(T) {
    if ((E('inipaddr_start').value) && !E('inipaddr_end').value) {
        alert('Internal IP End is needed.');
        return false;
    }

    if ((!E('inipaddr_start').value) && E('inipaddr_end').value) {
        alert('Internal IP Start is needed.');
        return false;
    }

    if (!tbl_check_valid_ipaddr_range(E('inipaddr_start').value, E('inipaddr_end').value, 'Internal IP')) {
        return false;
    }

    if ((E('extipaddr_start').value) && !E('extipaddr_end').value) {
        alert('External IP End is needed.');
        return false;
    }

    if ((!E('extipaddr_start').value) && E('extipaddr_end').value) {
        alert('External IP Start is needed.');
        return false;
    }

    if (!tbl_check_valid_ipaddr_range(E('extipaddr_start').value, E('extipaddr_end').value, 'External IP')) {
        return false;
    }

    /* For some case, browser caches the protocol value as a value but '*',
     * that causes some problem at saving.
     */
    if (E('proto').value == '') {
        if ((E('extport_start').value) && E('extport_end').value) {
            alert('Proto is needed if specifying ports.');
        }
    }

    if (E('proto').value != '') {
        if ((E('extport_start').value) && !E('extport_end').value) {
            alert('External Port End is needed.');
            return false;
        }

        if ((!E('extport_start').value) && E('extport_end').value) {
            alert('External Port Start is needed.');
            return false;
        }

        if (!tbl_check_valid_port_range(E('extport_start').value, E('extport_end').value, 'External Port')) {
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

var default_rule = ['','1','wan0','','','','','','','','','','','1','0','','','',''];
var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='fl_table_old_seq' value='"+nth+"' />"
        + "<input id='fl_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Name");%>' + "</div>"
        + "<input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Enable");%>' + "</div>"
        + "<input id='enable' name='enable' type='checkbox' value='1' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("External Interface");%>' + "</div>"
        + "<select id='extif' name='extif' >"
        + "<option value=''>*</option>";
    for (i = 1; i <= <%nv_get("wan_num");%>; i++) {
        s += "<option value='wan" + (i-1) + "' >WAN" + i + "</option>";
    }
    s +=  "</select>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Internal IP Range");%>' + "</div>"
        + '<%lang("From");%>' + ":"
        + "<input id='inipaddr_start' name='inipaddr_start' size='17' maxlength='15' "
        +           "onblur='valid_ipaddr(this, \"Internal IP Range Start\")'/>"
        + '<%lang("To");%>' + ":"
        + "<input id='inipaddr_end' name='inipaddr_end' size='17' maxlength='15' "
        +           "onblur='valid_ipaddr(this, \"Internal IP Range End\")' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("External IP Range");%>' + "</div>"
        + '<%lang("From");%>' + ":"
        + "<input id='extipaddr_start' name='extipaddr_start' size='17' maxlength='15' "
        +           "onblur='valid_ipaddr(this, \"External IP Range Start\")' />"
        + '<%lang("To");%>' + ":"
        + "<input id='extipaddr_end' name='extipaddr_end' size='17' maxlength='15' "
        +           "onblur='valid_ipaddr(this, \"External IP Range End\")' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang(Protocol);%>' + "</div>"
        + "<select id='proto' name='proto' onchange=\"proto_sel_wrapper(value, '*')\" >"
        + "<option value=''>*</option>"
        + "<option value='tcp'>TCP</option>"
        + "<option value='udp'>UDP</option>"
        + "<option value='both'>TCP/UDP</option>"
        + "</select>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Service Port Range");%>' + "</div>"
        + '<%lang("From");%>' + ":"
        + "<input id='extport_start' name='extport_start' size='6' maxlength='5' "
        +         "onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' />"
        + '<%lang("To");%>' + ":" 
        + "<input id='extport_end' name='extport_end' size='6' maxlength='5' "
        +         "onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang(Action);%>' + "</div>"
        + "<select id='op' name='op' onchange=\"action_sel_wrapper(value, '0')\" >"
        + "<option value='1'>" + '<%lang("ALLOW");%>' + "</option>"
        + "<option value='0'>" + '<%lang("DENY");%>' + "</option>"
        + "</select>"
        +"</div>";

    s += "<div type='hidden' id='dbm_enable' name='dbm_enable' value='0' />";
/*
    s += "<div class='setting'>"
        + "<div class='label'>"+'<%lang("DBM IP Enable");%>' + "</div>"
        + "<input id='dbm_enable' nam='dbm_enable' type='checkbox' value='1' onclick=\"valid_dbm_action(this)\" />"
        +"</div>";
*/


    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    E('name').value  = rule[enum_fl_basic._NAME];
    tbl_assign_enable('enable', rule[enum_fl_basic._ENABLE]);
    E('extif').value = rule[enum_fl_basic._EXT_IF]; 
    E('inipaddr_start').value = rule[enum_fl_basic._IN_IPADDR_START];
    E('inipaddr_end').value = rule[enum_fl_basic._IN_IPADDR_END];
    E('extipaddr_start').value = rule[enum_fl_basic._EXT_IPADDR_START];
    E('extipaddr_end').value = rule[enum_fl_basic._EXT_IPADDR_END];
    E('proto').value = rule[enum_fl_basic._PROTOCOL];
    E('extport_start').value = rule[enum_fl_basic._EXT_PORT_START];
    E('extport_end').value = rule[enum_fl_basic._EXT_PORT_END];
    E('op').value = rule[enum_fl_basic._ACTION];
    tbl_assign_enable('dbm_enable', rule[enum_fl_basic._DBM_ENABLE]);
    if (E('op').value == '0') {
        E('dbm_enable').disabled = true;
    }

    if (E('proto').value == '') {
        E('extport_start').disabled = true;
        E('extport_end').disabled = true;
    }
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Name");%></th>'
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("External Interface");%></th>'
        + '<th><%lang("Internal IP Range");%></th>'
        + '<th><%lang("Action");%></th>'
/*        + '<th><%lang("DBM IP Enable");%></th>' */
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + R[i][enum_fl_basic._NAME] + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_fl_basic._ENABLE]) + "</td>"
             + "<td>" + tbl_wrap_wan(R[i][enum_fl_basic._EXT_IF]) + "</td>"
             + "<td>" + "<%lang("From");%>:" + R[i][enum_fl_basic._IN_IPADDR_START] + "<br/>" 
                      + "<%lang("To");%>:" + R[i][enum_fl_basic._IN_IPADDR_END] + "</td>"
             + "<td>" + tbl_wrap_action(R[i][enum_fl_basic._ACTION]) + "</td>"
/*             + "<td>" + tbl_wrap_enable(R[i][enum_fl_basic._DBM_ENABLE]) + "</td>" */
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    tbl_append_element(F, 'fl_rule', '');

    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, P + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, P + i + '_name', fl_rule[i][enum_fl_basic._NAME]);
        tbl_append_element(F, P + i + '_enable', fl_rule[i][enum_fl_basic._ENABLE]);
        tbl_append_element(F, P + i + '_extif', fl_rule[i][enum_fl_basic._EXT_IF]);
        tbl_append_element(F, P + i + '_inif', fl_rule[i][enum_fl_basic._IN_IF]);
        tbl_append_element(F, P + i + '_inipaddr_start', fl_rule[i][enum_fl_basic._IN_IPADDR_START]);
        tbl_append_element(F, P + i + '_inipaddr_end', fl_rule[i][enum_fl_basic._IN_IPADDR_END]);
        tbl_append_element(F, P + i + '_extipaddr_start', fl_rule[i][enum_fl_basic._EXT_IPADDR_START]);
        tbl_append_element(F, P + i + '_extipaddr_end', fl_rule[i][enum_fl_basic._EXT_IPADDR_END]);
        tbl_append_element(F, P + i + '_proto', fl_rule[i][enum_fl_basic._PROTOCOL]);
        tbl_append_element(F, P + i + '_inport_start', fl_rule[i][enum_fl_basic._IN_PORT_START]);
        tbl_append_element(F, P + i + '_inport_end', fl_rule[i][enum_fl_basic._IN_PORT_END]);
        tbl_append_element(F, P + i + '_extport_start', fl_rule[i][enum_fl_basic._EXT_PORT_START]);
        tbl_append_element(F, P + i + '_extport_end', fl_rule[i][enum_fl_basic._EXT_PORT_END]);
        tbl_append_element(F, P + i + '_op', fl_rule[i][enum_fl_basic._ACTION]);
        tbl_append_element(F, P + i + '_dbm_enable', fl_rule[i][enum_fl_basic._DBM_ENABLE]);
        tbl_append_element(F, P + i + '_dbm_num', fl_rule[i][enum_fl_basic._DBM_NUM]);
        tbl_append_element(F, P + i + '_mark', fl_rule[i][enum_fl_basic._MARK]);
        tbl_append_element(F, P + i + '_bw_idx', fl_rule[i][enum_fl_basic._BW_IDX]);
        tbl_append_element(F, P + i + '_sched_idx', fl_rule[i][enum_fl_basic._SCHED_IDX]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_fl_basic._NAME]             = E('name').value;
    rule_tmp[enum_fl_basic._ENABLE]           = tbl_dewrap_enable('enable');
    rule_tmp[enum_fl_basic._EXT_IF]           = E('extif').value;
    rule_tmp[enum_fl_basic._IN_IF]            = '';
    rule_tmp[enum_fl_basic._IN_IPADDR_START]  = E('inipaddr_start').value;
    rule_tmp[enum_fl_basic._IN_IPADDR_END]    = E('inipaddr_end').value;
    rule_tmp[enum_fl_basic._EXT_IPADDR_START] = E('extipaddr_start').value;
    rule_tmp[enum_fl_basic._EXT_IPADDR_END]   = E('extipaddr_end').value;
    rule_tmp[enum_fl_basic._PROTOCOL]         = E('proto').value;
    rule_tmp[enum_fl_basic._IN_PORT_START]    = '';
    rule_tmp[enum_fl_basic._IN_PORT_END]      = '';
    rule_tmp[enum_fl_basic._EXT_PORT_START]   = E('extport_start').value;
    rule_tmp[enum_fl_basic._EXT_PORT_END]     = E('extport_end').value;
    rule_tmp[enum_fl_basic._ACTION]           = E('op').value;
    rule_tmp[enum_fl_basic._DBM_ENABLE]       = tbl_dewrap_enable('dbm_enable');
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
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("fl_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, fl_objects);
    if (enable == 1) {
        tbl_disable_row_edit('fl_table');
    }
    <%widget_start();%>
}
</script>
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
              <% tbl_get("fl_rule"); %>
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
              <fieldset>
                <legend><%lang("Access Control List");%> (ACL) <%lang("Rule");%></legend>
                <%tbl_create_table("fl_table", "fl_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("fl_table", fl_rule);
                </script>
              </fieldset><br />

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
