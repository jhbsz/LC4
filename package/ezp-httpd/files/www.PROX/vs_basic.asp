<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Virtual Hosts</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var F;

function local_rule_check(T) {

    if (!E('wan_ipaddr').value) {
        alert('Please specify a External IP Address.');
        return false;
    }

    if (!E('mapped_ipaddr').value) {
        alert('Please specify a Mapped LAN IP Address.');
        return false;
    }

    return true;
}

var vs_objects =
[ { id: 'vs_table_add' },
  { id: 'vs_table_delete' },
  { id: 'vs_table_modify' },
  { id: 'vs_table_up' },
  { id: 'vs_table_down' },
  { id: 'vs_table_element' }
];
 
var enum_vs_basic = {
    _NAME:0,
    _ENABLE:1,
    _EXT_IF:2,
    _IF_INDEX:3,
    _WAN_ADDR:4,
    _MAPPED_ADDR:5
} 

var default_rule = ['','1','wan1','','',''];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<div class='iInputField'><input type='hidden' id='vs_table_old_seq' value='"+nth+"' />"
        + "<input id='vs_table_seq' size='4' maxlength='4' value='"+nth+"' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Name");%>' + "</div>"
        + "<div class='iInputField'><input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Enable");%>' + "</div>"
        + "<div class='iInputField'><input id='enable' name='enable' type='checkbox' value='1' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("External Interface");%>' + "</div>"
        + "<div class='iInputField'><select id='extif' name='extif' >";
    for (i = <%nv_get("wan_num");%>; i >= 1; i--) {
        s += "<option value='wan" + (i-1) + "' >WAN" + get_wan_title(i-1) + "</option>";
    }
    s +=  "</select></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("External IP Address");%>' + "</div>"
        + "<div class='iInputField'><input id='wan_ipaddr' name='wan_ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></div>"
        +"</div><br/>";

    s += "<div class='isetting'>"
        + "<div class='ilabel'>"+'<%lang("Mapped LAN IP Address");%>' + "</div>"
        + "<div class='iInputField'><input id='mapped_ipaddr' name='mapped_ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' /></div>"
        +"</div>";

    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    E('name').value          = rule[enum_vs_basic._NAME];
    tbl_assign_enable('enable', rule[enum_vs_basic._ENABLE]);
    E('extif').value         = rule[enum_vs_basic._EXT_IF];
    E('wan_ipaddr').value        = rule[enum_vs_basic._WAN_ADDR];
    E('mapped_ipaddr').value        = rule[enum_vs_basic._MAPPED_ADDR];
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Name");%></th>'
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("External Interface");%></th>'
        + '<th><%lang("External IP Address");%></th>'
        + '<th><%lang("Mapped LAN IP Address");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + R[i][enum_vs_basic._NAME] + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_vs_basic._ENABLE]) + "</td>"
             + "<td>" + tbl_wrap_wan(R[i][enum_vs_basic._EXT_IF]) + "</td>"
             + "<td>" + "<%lang("From");%>:" + R[i][enum_vs_basic._WAN_ADDR] + "</td>"
             + "<td>" + "<%lang("To");%>:" + R[i][enum_vs_basic._MAPPED_ADDR] + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    tbl_append_element(F, 'vs_rule', '');

    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, P + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, P + i + '_name', vs_rule[i][enum_vs_basic._NAME]);
        tbl_append_element(F, P + i + '_enable', vs_rule[i][enum_vs_basic._ENABLE]);
        tbl_append_element(F, P + i + '_extif', vs_rule[i][enum_vs_basic._EXT_IF]);
        tbl_append_element(F, P + i + '_wan_ipaddr', vs_rule[i][enum_vs_basic._WAN_ADDR]);
        tbl_append_element(F, P + i + '_mapped_ipaddr', vs_rule[i][enum_vs_basic._MAPPED_ADDR]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_vs_basic._NAME]           = E('name').value;
    rule_tmp[enum_vs_basic._ENABLE]         = tbl_dewrap_enable('enable');
    rule_tmp[enum_vs_basic._EXT_IF]         = E('extif').value;
    rule_tmp[enum_vs_basic._WAN_ADDR]        = E('wan_ipaddr').value;
    rule_tmp[enum_vs_basic._MAPPED_ADDR]        = E('mapped_ipaddr').value;

    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "vs_basic";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "vs_table", "vs_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("vs_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, vs_objects);
    if (enable == 1) {
        tbl_disable_row_edit('vs_table');
    }
    <%widget_start();%>
}
</script>
</head>
<body class="gui" onload="init();">
  <div id="wrapper">
      <div id="header">
      <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Applications", "Virtual Hosts"); %>
      <script type="text/javascript">do_header_post()</script>
      </div>

    <script type="text/javascript">do_body_pre()</script>	
    <script type="text/javascript">do_table_pre()</script>	
    <script type="text/javascript">do_mainframe_pre()</script>	
    <script type="text/javascript">do_page_path('<%lang("Applications");%>', '<%lang("Virtual Hosts");%>')</script>
        <div id="contentsInfo">
          <form name="form" action="apply.cgi" method="post">
            <script type="text/javascript">do_hidden_elements()</script>	
            <input type="hidden" name="fr_dmz_num" value=<%nv_get("wan_num");%>/>

            <% tbl_get("vs_rule"); %>

            <fieldset>
              <legend><%lang("Virtual Hosts");%></legend>
              <div class="setting">
                <div class="label"><% lang("Virtual Hosts"); %></div>
                <div class='radiobutton'><input type="radio" value="1" name="vs_enable" onclick="init()"
                      <%nv_match("vs_enable","1","checked");%>><%lang("Enable");%>
                </input></div>

                <div class='radiobutton'><input type="radio" value="0" name="vs_enable" onclick="init()"
                      <%nv_match("vs_enable","0","checked");%>><%lang("Disable");%>
                </input></div>
              </div>
            </fieldset><br />
            <fieldset>
              <legend><%lang("Virtual Host Rule");%></legend>
              <%tbl_create_table("vs_table", "vs_rule", "1");%>
              <script type="text/javascript">
                tbl_setup_table("vs_table", vs_rule);
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
  <script type="text/javascript">create_input_window("vs_table", "vs_rule", 1)</script>
  <script type="text/javascript">create_return_window()</script>
</body>
</html>
