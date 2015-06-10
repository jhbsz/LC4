<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Routing Policy</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

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

function routing_type_wrapper(v, check) {
    var F = document.getElementsByName("form")[0];
    if (v == check) {
        sel_enable_objs(F, routing_type_objects);
    } else {
        sel_disable_objs(F, routing_type_objects);
        E('gateway_ipaddr').value = "";
    }
}

var proto_objects =
[ { id: 'extport_start' },
  { id: 'extport_end' }
];

var rt_objects =
[ { id: 'rt_table_add' },
  { id: 'rt_table_delete' },
  { id: 'rt_table_modify' },
  { id: 'rt_table_up' },
  { id: 'rt_table_down' },
  { id: 'rt_table_element' }
];
 
var proto_item = [ '*', 'TCP', 'UDP', 'TCP/UDP' ];
var routing_type_objects = 
[ {id: 'gateway_ipaddr' }
];

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

    if (E('proto').value != '*') {
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

var default_rule = ['','1','','','','','','','','','','wan1','default',''];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<div class='iInputField'><input type='hidden' id='rt_table_old_seq' value='"+nth+"' />"
        + "<input id='rt_table_seq' size='4' maxlength='4' value='"+nth+"' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Name");%>' + "</div>"
        + "<div class='iInputField'><input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Enable");%>' + "</div>"
        + "<div class='iInputField'><input id='enable' name='enable' type='checkbox' value='0' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Internal IP Range");%>' + "</div>"
        + '<div class="iInputField"><%lang("From");%>' + ":&nbsp;"
        + "<input id='inipaddr_start' name='inipaddr_start' size='17' maxlength='15' "
        +       "onblur='valid_ipaddr(this, \"Internal IP Range Start\")' />"
        + '&nbsp;&nbsp;<%lang("To");%>' + ":&nbsp;"
        + "<input id='inipaddr_end' name='inipaddr_end' size='17' maxlength='15' "
        +       "onblur='valid_ipaddr(this, \"Internal IP Range End\")' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("External IP Range");%>' + "</div>"
        + '<div class="iInputField"><%lang("From");%>' + ":&nbsp;"
        + "<input id='extipaddr_start' name='extipaddr_start' size='17' maxlength='15' "
        +       "onblur='valid_ipaddr(this, \"External IP Range Start\")' />"
        + '&nbsp;&nbsp;<%lang("To");%>' + ":&nbsp;"
        + "<input id='extipaddr_end' name='extipaddr_end' size='17' maxlength='15' "
        +       "onblur='valid_ipaddr(this, \"External IP Range End\")' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Protocol");%>' + "</div>"
        + "<div class='iInputField'><select id='proto' name='proto' onchange=\"proto_sel_wrapper(value, '')\" >"
        + "<option value=''>*</option>"
        + "<option value='tcp'>TCP</option>"
        + "<option value='udp'>UDP</option>"
        + "<option value='both'>TCP/UDP</option>"
        + "</select></div>"
        + "</div><br/>";

    if (rule[enum_lb_routing._PROTOCOL] == '') {
        s += "<div class='isetting'>"
            + "<div class='ilabel'>"+'<%lang("Service Port Range");%>' + "</div>"
            + '<div class="iInputField"><%lang("From");%>' + ":&nbsp;"
            + "<input id='extport_start' name='extport_start' size='6' maxlength='5' "
            +           "onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' disabled />"
            + '&nbsp;&nbsp;<%lang("To");%>' + ":&nbsp;"
            + "<input id='extport_end' name='extport_end' size='6' maxlength='5' "
            +           "onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' disabled /></div>"
            +"</div><br/>";
    } else {
        s += "<div class='isetting'>"
            + "<div class='ilabel'>"+'<%lang("Service Port Range");%>' + "</div>"
            + '<div class="iInputField"><%lang("From");%>' + ":&nbsp;"
            + "<input id='extport_start' name='extport_start' size='6' maxlength='5' "
            +           "onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' />"
            + '&nbsp;&nbsp;<%lang("To");%>' + ":&nbsp;"
            + "<input id='extport_end' name='extport_end' size='6' maxlength='5' "
            +           "onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' /></div>"
            +"</div><br/>";
    }

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("External Interface");%>' + "</div>"
        + "<div class='iInputField'><select id='extif' name='extif' >";
    for (i = <%nv_get("wan_num");%>; i>=1; i--) {
        s += "<option value='wan" + (i-1) + "' >WAN" + get_wan_title(i-1) + "</option>";
    }
    for (i = 1; i <= <%nv_get("lan_num");%>; i++) {
        s += "<option value='lan" + (i-1) + "' >LAN</option>";
    }
    s +=  "</select></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Routing Gateway");%>' + "</div>"
        + "<div class='iInputField'><select id='routing_type' name='routing_type' "
        +       "onchange=\"routing_type_wrapper(value, 'static')\" >"
        + "<option value='default'>" + '<%lang("Default Gateway");%>' + "</option>"
        + "<option value='static'>" + '<%lang("Static Gateway");%>' + "</option>"
        + "</select></div>"
        + "</div><br/>";
    if (rule[enum_lb_routing._ROUTING_TYPE] == 'default') {
        s += "<div class='isetting'>"
            + "<div class='ilabel'>"+'<%lang("Gateway IP Address");%>' + "</div>"
            + "<div class='iInputField'><input id='gateway_ipaddr' name='gateway_ipaddr' size='17' "
            +       "maxlength='15' disabled='true' "
            +       "onblur='valid_ipaddr(this, \"Gateway IP Address\")' /></div>"
            +"</div>";
    } else {
        s += "<div class='isetting'>"
            + "<div class='ilabel'>"+'<%lang("Gateway IP Address");%>' + "</div>"
            + "<div class='iInputField'><input id='gateway_ipaddr' name='gateway_ipaddr' size='17' maxlength='15' "
            +       "onblur='valid_ipaddr(this, \"Gateway IP Address\")' /></div>"
            +"</div>";
    }
    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    E('name').value = rule[enum_lb_routing._NAME];
    tbl_assign_enable('enable', rule[enum_lb_routing._ENABLE]);
    E('inipaddr_start').value = rule[enum_lb_routing._IN_IPADDR_START];
    E('inipaddr_end').value = rule[enum_lb_routing._IN_IPADDR_END];
    E('extipaddr_start').value = rule[enum_lb_routing._EXT_IPADDR_START];
    E('extipaddr_end').value = rule[enum_lb_routing._EXT_IPADDR_END];
    E('proto').value = rule[enum_lb_routing._PROTOCOL];
    //E('inport_start').value = '';
    //E('inport_end').value = '';
    E('extport_start').value = rule[enum_lb_routing._EXT_PORT_START];
    E('extport_end').value = rule[enum_lb_routing._EXT_PORT_END];
    E('extif').value = rule[enum_lb_routing._EXT_IF];
    E('routing_type').value = rule[enum_lb_routing._ROUTING_TYPE];
    E('gateway_ipaddr').value = rule[enum_lb_routing._GATEWAY_IPADDR];
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Name");%></th>'
        + '<th><%lang("Enable");%></th>'
        + '<th><%lang("Internal IP Range");%></th>'
        + '<th><%lang("External IP Range");%></th>'
        + '<th><%lang("Protocol");%></th>'
        + '<th><%lang("Service Port Range");%></th>'
        + '<th><%lang("External Interface");%></th>'
        + '<th><%lang("Routing Type");%></th>'
        + '<th><%lang("Gateway");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + R[i][enum_lb_routing._NAME] + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_lb_routing._ENABLE]) + "</td>"
             + "<td>" + "<%lang("From");%>:" + R[i][enum_lb_routing._IN_IPADDR_START] + "<br/>"
                      + "<%lang("To");%>:" + R[i][enum_lb_routing._IN_IPADDR_END] + "</td>"
             + "<td>" + "<%lang("From");%>:" + R[i][enum_lb_routing._EXT_IPADDR_START] + "<br/>"
                      + "<%lang("To");%>:" + R[i][enum_lb_routing._EXT_IPADDR_END] + "</td>"
             + "<td>" + tbl_wrap_proto(R[i][enum_lb_routing._PROTOCOL]) + "</td>"
             + "<td>" + "<%lang("From");%>:" + R[i][enum_lb_routing._EXT_PORT_START] + "<br/>"
                      + "<%lang("To");%>:" + R[i][enum_lb_routing._EXT_PORT_END] + "</td>"
             + "<td>" + tbl_wrap_wan(R[i][enum_lb_routing._EXT_IF]) + "</td>"
             + "<td>" + R[i][enum_lb_routing._ROUTING_TYPE] + "</td>"
             + "<td>" + R[i][enum_lb_routing._GATEWAY_IPADDR] + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    tbl_append_element(F, 'rt_rule', '');
    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, P + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, P + i + '_name', rt_rule[i][enum_lb_routing._NAME]);
        tbl_append_element(F, P + i + '_enable', rt_rule[i][enum_lb_routing._ENABLE]);
        tbl_append_element(F, P + i + '_inipaddr_start', rt_rule[i][enum_lb_routing._IN_IPADDR_START]);
        tbl_append_element(F, P + i + '_inipaddr_end', rt_rule[i][enum_lb_routing._IN_IPADDR_END]);
        tbl_append_element(F, P + i + '_extipaddr_start', rt_rule[i][enum_lb_routing._EXT_IPADDR_START]);
        tbl_append_element(F, P + i + '_extipaddr_end', rt_rule[i][enum_lb_routing._EXT_IPADDR_END]);
        tbl_append_element(F, P + i + '_proto', rt_rule[i][enum_lb_routing._PROTOCOL]);
        tbl_append_element(F, P + i + '_inport_start', rt_rule[i][enum_lb_routing._IN_PORT_START]);
        tbl_append_element(F, P + i + '_inport_end', rt_rule[i][enum_lb_routing._IN_PORT_END]);
        tbl_append_element(F, P + i + '_extport_start', rt_rule[i][enum_lb_routing._EXT_PORT_START]);
        tbl_append_element(F, P + i + '_extport_end', rt_rule[i][enum_lb_routing._EXT_PORT_END]);
        tbl_append_element(F, P + i + '_extif', rt_rule[i][enum_lb_routing._EXT_IF]);
        tbl_append_element(F, P + i + '_routing_type', rt_rule[i][enum_lb_routing._ROUTING_TYPE]);
        tbl_append_element(F, P + i + '_gateway_ipaddr', rt_rule[i][enum_lb_routing._GATEWAY_IPADDR]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_lb_routing._NAME]             = E('name').value;
    rule_tmp[enum_lb_routing._ENABLE]           = tbl_dewrap_enable('enable');
    rule_tmp[enum_lb_routing._IN_IPADDR_START]  = E('inipaddr_start').value;
    rule_tmp[enum_lb_routing._IN_IPADDR_END]    = E('inipaddr_end').value;
    rule_tmp[enum_lb_routing._EXT_IPADDR_START] = E('extipaddr_start').value;
    rule_tmp[enum_lb_routing._EXT_IPADDR_END]   = E('extipaddr_end').value;
    rule_tmp[enum_lb_routing._PROTOCOL]         = E('proto').value;
    rule_tmp[enum_lb_routing._IN_PORT_START]    = '';
    rule_tmp[enum_lb_routing._IN_PORT_END]      = '';
    rule_tmp[enum_lb_routing._EXT_PORT_START]   = E('extport_start').value;
    rule_tmp[enum_lb_routing._EXT_PORT_END]     = E('extport_end').value;
    rule_tmp[enum_lb_routing._EXT_IF]           = E('extif').value;
    rule_tmp[enum_lb_routing._ROUTING_TYPE]   = E('routing_type').value;
    rule_tmp[enum_lb_routing._GATEWAY_IPADDR]     = E('gateway_ipaddr').value;

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
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("rt_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, rt_objects);
    if (enable == 1) {
        tbl_disable_row_edit('rt_table');
    }
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Setup", "Routing"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("Routing");%>')</script>
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>	
              <% tbl_get("rt_rule"); %>
              <fieldset>
                <legend><%lang("Routing");%></legend>
                <div class="setting">
                  <div class="label"><% lang("Routing"); %></div>
                  <div class='radiobutton'><input type="radio" value="1" name="rt_enable" onclick="init()"
                        <%nv_match("rt_enable","1","checked");%>><%lang("Enable");%>
                  </input></div>

                  <div class='radiobutton'><input type="radio" value="0" name="rt_enable" onclick="init()"
                        <%nv_match("rt_enable","0","checked");%>><%lang("Disable");%>
                  </input></div>
                </div>
              </fieldset><br />

              <fieldset>
                <legend><%lang("Routing Rule");%></legend>
                <%tbl_create_table("rt_table", "rt_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("rt_table", rt_rule);
                </script>
              </fieldset><br/>
              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>"
                         onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>"
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
