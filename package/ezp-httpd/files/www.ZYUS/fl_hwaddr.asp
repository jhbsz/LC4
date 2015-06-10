<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - MAC Rule</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var action_item = [ '<%lang("ALLOW");%>', '<%lang("DENY");%>' ];
var F;

function local_rule_check(T) {
    if (!E('hwaddr').value) {
        alert('Please specify a MAC.');
        return false;
    }

    if ((E('arp_enable').checked == true) || (E('dhcp_enable').checked == true)) {
        if (!E('ipaddr').value) {
            alert('Please specify an IP.');
            return false;
        }
    }

    return true;
}

var fl_hwaddr_objects =
[ { id: 'fl_hwaddr_rule_default' },
  { id: 'fl_hwaddr_table_add' },
  { id: 'fl_hwaddr_table_delete' },
  { id: 'fl_hwaddr_table_modify' },
  { id: 'fl_hwaddr_table_up' },
  { id: 'fl_hwaddr_table_down' },
  { id: 'fl_hwaddr_table_element' }
];

var enum_fl_hwaddr = {
    _NAME:0,
    _HWADDR:1,
    _OP:2,
    _ACL_ENABLE:3,
    _ARP_ENABLE:4,
    _DHCP_ENABLE:5,
    _IPADDR:6
} 

var default_rule = ['','','1','1','1','1',''];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='fl_hwaddr_table_old_seq' value='"+nth+"' />"
        + "<input id='fl_hwaddr_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Name");%>' + "</div>"
        + "<input id='name' name='name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("MAC");%>' + "</div>"
        + "<input id='hwaddr' name='hwaddr' size='18' maxlength='17' onblur='valid_hwaddr(this, \"MAC\")' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Action");%>' + "</div>"
        + "<select id='op' name='op' >"
        + "<option value='1'>" + '<%lang("ALLOW");%>' + "</option>"
        + "<option value='0'>" + '<%lang("DENY");%>' + "</option>"
        + "</select>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("ACL Enable");%>' + "</div>"
        + "<input id='acl_enable' name='acl_enable' type='checkbox' size='10' value='1' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Static ARP Enable");%>' + "</div>"
        + "<input id='arp_enable' name='arp_enable' type='checkbox' size='10' value='1' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Static DHCP Enable");%>' + "</div>"
        + "<input id='dhcp_enable' name='dhcp_enable' type='checkbox' size='10' value='1' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("IP");%>' + "</div>"
        + "<input id='ipaddr' name='ipaddr' size='17' maxlength='15' onblur='valid_ipaddr(this, \"IP\")' />"
        +"</div>";

    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    E('name').value   = rule[enum_fl_hwaddr._NAME];
    E('hwaddr').value = rule[enum_fl_hwaddr._HWADDR];
    E('op').value = rule[enum_fl_hwaddr._OP];
    tbl_assign_enable('acl_enable', rule[enum_fl_hwaddr._ACL_ENABLE]);
    tbl_assign_enable('arp_enable', rule[enum_fl_hwaddr._ARP_ENABLE]);
    tbl_assign_enable('dhcp_enable', rule[enum_fl_hwaddr._DHCP_ENABLE]);
    E('ipaddr').value = rule[enum_fl_hwaddr._IPADDR];
}

function local_create_table(T, R) {
    var c;

    c = "<tr>"
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("Action");%></th>'
        + '<th><%lang("ACL Enable");%></th>'
        + '<th><%lang("Static DHCP Enable");%></th>'
        + '<th><%lang("IP");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + R[i][enum_fl_hwaddr._NAME] + "</td>"
             + "<td>" + tbl_wrap_action(R[i][enum_fl_hwaddr._OP]) + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_fl_hwaddr._ACL_ENABLE]) + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_fl_hwaddr._DHCP_ENABLE]) + "</td>"
             + "<td>" + R[i][enum_fl_hwaddr._IPADDR] + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    tbl_append_element(F, 'fl_hwaddr_rule', '');

    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, P + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, P + i + '_name', fl_hwaddr_rule[i][enum_fl_hwaddr._NAME]);
        tbl_append_element(F, P + i + '_hwaddr', fl_hwaddr_rule[i][enum_fl_hwaddr._HWADDR]);
        tbl_append_element(F, P + i + '_op', fl_hwaddr_rule[i][enum_fl_hwaddr._OP]);
        tbl_append_element(F, P + i + '_acl_enable', fl_hwaddr_rule[i][enum_fl_hwaddr._ACL_ENABLE]);
        tbl_append_element(F, P + i + '_arp_enable', fl_hwaddr_rule[i][enum_fl_hwaddr._ARP_ENABLE]);
        tbl_append_element(F, P + i + '_dhcp_enable', fl_hwaddr_rule[i][enum_fl_hwaddr._DHCP_ENABLE]);
        tbl_append_element(F, P + i + '_ipaddr', fl_hwaddr_rule[i][enum_fl_hwaddr._IPADDR]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_fl_hwaddr._NAME]        = E('name').value;
    rule_tmp[enum_fl_hwaddr._HWADDR]      = E('hwaddr').value;
    rule_tmp[enum_fl_hwaddr._OP]          = E('op').value;
    rule_tmp[enum_fl_hwaddr._ACL_ENABLE]  = tbl_dewrap_enable('acl_enable');
    rule_tmp[enum_fl_hwaddr._ARP_ENABLE]  = tbl_dewrap_enable('arp_enable');
    rule_tmp[enum_fl_hwaddr._DHCP_ENABLE] = tbl_dewrap_enable('dhcp_enable');
    rule_tmp[enum_fl_hwaddr._IPADDR]      = E('ipaddr').value;

    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "fl_hwaddr";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "fl_hwaddr_table", "fl_hwaddr_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("fl_hwaddr_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, fl_hwaddr_objects);
    if (enable == 1) {
        tbl_disable_row_edit('fl_hwaddr_table');
    }
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
         <% do_menu("Security", "MAC Access Control"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Security");%>', '<%lang("MAC Access Control");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>
              <% tbl_get("fl_hwaddr_rule"); %>
              <fieldset>
                <legend><%lang("MAC Access Control");%></legend>
                <div class="setting">
                  <div class="label"><% lang("MAC Access Control"); %></div>
                  <input type="radio" value="1" name="fl_hwaddr_enable" onclick="init()"
                        <%nv_match("fl_hwaddr_enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="fl_hwaddr_enable" onclick="init()"
                        <%nv_match("fl_hwaddr_enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                  <div class="label"><% lang("Default MAC Access Control Action"); %></div>
                  <input type="radio" value="1" name="fl_hwaddr_rule_default" id="fl_hwaddr_rule_default"
                        <%nv_match("fl_hwaddr_rule_default","1","checked");%>><%lang("ALLOW");%>
                  </input>

                  <input type="radio" value="0" name="fl_hwaddr_rule_default" id="fl_hwaddr_rule_default"
                        <%nv_match("fl_hwaddr_rule_default","0","checked");%>><%lang("DENY");%>
                  </input>
                </div>
              </fieldset><br />
              <fieldset>
                <legend><%lang("MAC Access Control Rule");%></legend>
                <%tbl_create_table("fl_hwaddr_table", "fl_hwaddr_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("fl_hwaddr_table", fl_hwaddr_rule);
                </script>
              </fieldset><br />

              <div class="submitFooter">
                <input type="button" name="save_button" value='<%lang("Save Settings");%>' onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick='window.location.reload()'/>
              </div>
            </form>

          </div>
        <div id="floatKiller"></div>
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
    <script type="text/javascript">create_input_window("fl_hwaddr_table", "fl_hwaddr_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
