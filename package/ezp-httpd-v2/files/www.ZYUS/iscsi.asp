<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - iSCSI</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var isuser_objects =
[ { name: 'isuser_port' },
  { name: 'isuser_bandwidth' },
  { id: 'isuser_table_add' },
  { id: 'isuser_table_delete' },
  { id: 'isuser_table_modify' },
  { id: 'isuser_table_up' },
  { id: 'isuser_table_down' },
  { id: 'isuser_table_element' }
];
var F; 
function local_rule_check(T) {
    if ((!E('username').value)) {
        alert('User name is required.');
        return false;
    }

    if (!E('password').value) {
        alert('Password is required.');
        return false;
    }
    
    if (E('password').value.length < 14) {
            alert('Password length must > 13');
                    return false;
    }
                        
    return true;
}
function local_table_check(T, R) {
    return rlist_check_dup_field(T, R, enum_is._NAME);
}

var enum_is = {                                                                                                          
    _ENABLE:0,
    _NAME:1,
    _PASSWD:2
} 

var default_rule = ['1','',''];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='isuser_table_old_seq' value='"+nth+"' />"
        + "<input id='isuser_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Enable");%>' + "</div>"
        + "<input id='enable' name='enable' type='checkbox'/>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("User Name");%>' + "</div>"
        + "<input id='username' name='username' size='20' maxlength='20' onblur='valid_name(this, \"User Name\", 1)'/>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Password");%>' + "</div>"
        + "<input id='password' name='password' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/>"
        + ' (<%lang("More than 13 Characters");%>)'
        +"</div>";
                
    
    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    tbl_assign_enable('enable', rule[enum_is._ENABLE]);
    E('username').value = rule[enum_is._NAME];
    E('password').value = rule[enum_is._PASSWD];
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("User Name");%></th>'
        + '<th><%lang("Password");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + tbl_wrap_enable(R[i][enum_is._ENABLE]) + "</td>"
             + "<td>" + R[i][enum_is._NAME] + "</td>"
             + "<td>" + R[i][enum_is._PASSWD] + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, prefix) {
    /* Important */
    tbl_append_element(F, 'isuser_rule', '');
    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, prefix + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, prefix + i + '_enable', isuser_rule[i][enum_is._ENABLE]);
        tbl_append_element(F, prefix + i + '_username', isuser_rule[i][enum_is._NAME]);
        tbl_append_element(F, prefix + i + '_password', isuser_rule[i][enum_is._PASSWD]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_is._ENABLE]   = tbl_dewrap_enable('enable');
    rule_tmp[enum_is._NAME]     = E('username').value;
    rule_tmp[enum_is._PASSWD]   = E('password').value;

    return rule_tmp;
}

function to_submit(F) {
    F.submit_button.value = "iscsi";
	F.action.value = "Apply";
    if (local_table_check("isuser_table", isuser_rule) == false) {
        return;
    }
    local_pack_key_value_pair(F, "isuser_table", "isuser_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("iscsi_enable")[0].checked == true) ? 1 : 0;

    sel_change(enable, 1, F, isuser_objects);
    if (enable == 1) {
        tbl_disable_row_edit('isuser_table');
    }
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Storage", "iSCSI"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("iSCSI");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('is_rule')</script>	

              <% tbl_get("isuser_rule"); %>

              <fieldset>
                <legend><%lang("iSCSI");%></legend>
                <div class="setting">
                  <div class="label"><% lang("iSCSI"); %></div>
                  <input type="radio" value="1" name="iscsi_enable" onclick="init()"
                        <%nvg_attr_match("iscsi_enable","is_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="iscsi_enable" onclick="init()"
                        <%nvg_attr_match("iscsi_enable","is_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                <div class="label"><%lang("iSNS Server");%></div>
                <input name="iscsi_isns" size="20" maxLength="80"
                value='<%nvg_attr_get("iscsi_isns", "is_rule", "0", "isns_serv");%>' />
                (<%lang("IP address or domain name");%>)
                </div>
                <div class="setting">
                <div class="label"><%lang("Target ID");%></div>
                <input name="iscsi_id" size="20" maxLength="80"
                value='<%nvg_attr_get("iscsi_id", "is_rule", "0", "isid");%>' />
                </div>

                <div class="setting">
                <div class="label"><%lang("LUN Alias");%></div>
                <input name="iscsi_alias" size="20" maxLength="20"
                value='<%nvg_attr_get("iscsi_alias", "is_rule", "0", "alias");%>' />
                (20 <%lang("Characters");%>)
                </div>

                <div class="setting">
                <div class="label"><%lang("Max Connextion");%></div>
                <input name="iscsi_max_conn" size="10" maxLength="10"
                value='<%nvg_attr_get("iscsi_max_conn", "is_rule", "0", "max_conn");%>' />
                <%lang("Connections");%> (1 ~ 10)
                </div>

                <div class="setting">
                <div class="label"><%lang("Max Transmision Data Length");%></div>
                <input name="iscsi_max_tx" size="10" maxLength="30"
                value='<%nvg_attr_get("iscsi_max_tx", "is_rule", "0", "max_tx");%>' />
                <%lang("Bytes");%> (512 ~ 16384)
                </div>

                <div class="setting">
                <div class="label"><%lang("Max Receiving Data Length");%></div>
                <input name="iscsi_max_rx" size="10" maxLength="30"
                value='<%nvg_attr_get("iscsi_max_rx", "is_rule", "0", "max_rx");%>' />
                <%lang("Bytes");%> (512 ~ 16384)
                </div>

              </fieldset><br />

              <fieldset>
                <legend><%lang("User Rule");%></legend>
                <%tbl_create_table("isuser_table", "isuser_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("isuser_table", isuser_rule);
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
    <script type="text/javascript">create_input_window("isuser_table", "isuser_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
