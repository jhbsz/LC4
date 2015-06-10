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
    if ((!E('extport_start').value)) {
        alert('External Port Start is needed.');
        return false;
    }

    if (!E('extport_end').value) {
        alert('External Port End is needed.');
        return false;
    }

    if (!tbl_check_valid_port_range(E('extport_start').value, E('extport_end').value, 'External Port')) {
        return false;
    }

    if (!E('ipaddr').value) {
        alert('Please specify a IP.');
        return false;
    }
        
    return true;
}

var fr_objects =
[ { id: 'fr_table_add' },
  { id: 'fr_table_delete' },
  { id: 'fr_table_modify' },
  { id: 'fr_table_up' },
  { id: 'fr_table_down' },
  { id: 'fr_table_element' }
];
 
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

var default_rule = ['','0','wan0','tcp','','','','',''];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='fr_table_old_seq' value='"+nth+"' />"
        + "<input id='fr_table_seq' size='4' maxlength='4' value='"+nth+"' />"
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
        + "<select id='extif' name='extif' >";
    for (i = 1; i <= <%nv_get("wan_num");%>; i++) {
        s += "<option value='wan" + (i-1) + "' >WAN" + i + "</option>";
    }
    s +=  "</select>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Protocol");%>' + "</div>"
        + "<select id='proto' name='proto' >"
        + "<option value='tcp'>TCP</option>"
        + "<option value='udp'>UDP</option>"
        + "<option value='both'>TCP/UDP</option>"
        + "</select>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("External Port Range");%>' + "</div>"
        + '<%lang("From");%>' + ":"
        + "<input id='extport_start' name='extport_start' size='6' maxlength='5' "
        +       "onblur='valid_range(this, 1, 65535, \"External Port Range Start\")' />"
        + '<%lang("To");%>' + ":"
        + "<input id='extport_end' name='extport_end' size='6' maxlength='5' "
        +       "onblur='valid_range(this, 1, 65535, \"External Port Range End\")' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Internal IP");%>' + "</div>"
        + "<input id='ipaddr' name='ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Internal Port Range");%>' + "</div>"
        + '<%lang("From");%>' + ":"
        + "<input id='inport_start' name='inport_start' size='6' maxlength='5' "
        +       "onblur='valid_range(this, 1, 65535, \"Internal Port Range Start\")' />"
        + '<%lang("To");%>' + ":"
        + "<input id='inport_end' name='inport_end' size='6' maxlength='5' "
        +       "onblur='valid_range(this, 1, 65535, \"Internal Port Range End\")' />"
        +"</div>";

    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    E('name').value          = rule[enum_fr_basic._NAME];
    tbl_assign_enable('enable', rule[enum_fr_basic._ENABLE]);
    E('extif').value         = rule[enum_fr_basic._EXT_IF];
    E('proto').value         = rule[enum_fr_basic._PROTOCOL];
    E('extport_start').value = rule[enum_fr_basic._EXT_PORT_START];
    E('extport_end').value   = rule[enum_fr_basic._EXT_PORT_END];
    E('ipaddr').value        = rule[enum_fr_basic._IP_ADDR];
    E('inport_start').value  = rule[enum_fr_basic._IN_PORT_START];
    E('inport_end').value    = rule[enum_fr_basic._IN_PORT_END];
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Name");%></th>'
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("External Interface");%></th>'
        + '<th><%lang("Protocol");%></th>'
        + '<th><%lang("External Port Range");%></th>'
        + '<th><%lang("Internal IP");%></th>'
        + '<th><%lang("Internal Port Range");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + R[i][enum_fr_basic._NAME] + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_fr_basic._ENABLE]) + "</td>"
             + "<td>" + tbl_wrap_wan(R[i][enum_fr_basic._EXT_IF]) + "</td>"
             + "<td>" + tbl_wrap_proto(R[i][enum_fr_basic._PROTOCOL]) + "</td>"
             + "<td>" + "<%lang("From");%>:" + R[i][enum_fr_basic._EXT_PORT_START] + "<br/>"
                      + "<%lang("To");%>:" + R[i][enum_fr_basic._EXT_PORT_END] + "</td>"
             + "<td>" + R[i][enum_fr_basic._IP_ADDR] + "</td>"
             + "<td>" + "<%lang("From");%>:" + R[i][enum_fr_basic._IN_PORT_START] + "<br/>"
                      + "<%lang("To");%>:" + R[i][enum_fr_basic._IN_PORT_END] + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    tbl_append_element(F, 'fr_rule', '');

    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, P + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, P + i + '_name', fr_rule[i][enum_fr_basic._NAME]);
        tbl_append_element(F, P + i + '_enable', fr_rule[i][enum_fr_basic._ENABLE]);
        tbl_append_element(F, P + i + '_extif', fr_rule[i][enum_fr_basic._EXT_IF]);
        tbl_append_element(F, P + i + '_proto', fr_rule[i][enum_fr_basic._PROTOCOL]);
        tbl_append_element(F, P + i + '_extport_start', fr_rule[i][enum_fr_basic._EXT_PORT_START]);
        tbl_append_element(F, P + i + '_extport_end', fr_rule[i][enum_fr_basic._EXT_PORT_END]);
        tbl_append_element(F, P + i + '_ipaddr', fr_rule[i][enum_fr_basic._IP_ADDR]);
        tbl_append_element(F, P + i + '_inport_start', fr_rule[i][enum_fr_basic._IN_PORT_START]);
        tbl_append_element(F, P + i + '_inport_end', fr_rule[i][enum_fr_basic._IN_PORT_END]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_fr_basic._NAME]           = E('name').value;
    rule_tmp[enum_fr_basic._ENABLE]         = tbl_dewrap_enable('enable');
    rule_tmp[enum_fr_basic._EXT_IF]         = E('extif').value;
    rule_tmp[enum_fr_basic._PROTOCOL]       = E('proto').value;
    rule_tmp[enum_fr_basic._EXT_PORT_START] = E('extport_start').value;
    rule_tmp[enum_fr_basic._EXT_PORT_END]   = E('extport_end').value;
    rule_tmp[enum_fr_basic._IP_ADDR]        = E('ipaddr').value;
    rule_tmp[enum_fr_basic._IN_PORT_START]  = E('inport_start').value;
    rule_tmp[enum_fr_basic._IN_PORT_END]    = E('inport_end').value;

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
    F = document.getElementsByName("form")[0];
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var enable = (document.getElementsByName("fr_dmz"+i+"_enable")[0].checked == true) ? 1 : 0;
        sel_change(enable, 1, F, dmz_objects[i]);
    }
    var enable = (document.getElementsByName("fr_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, fr_objects);
    if (enable == 1) {
        tbl_disable_row_edit('fr_table');
    }
    <%widget_start();%>
}
</script>
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

              <% tbl_get("fr_rule"); %>

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
              <fieldset>
                <legend><%lang("Port Range Forwarding Rule");%></legend>
                <%tbl_create_table("fr_table", "fr_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("fr_table", fr_rule);
                </script>
              </fieldset><br />

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
