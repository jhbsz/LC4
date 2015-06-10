<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - FTP</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var ftpd_objects =
[ { name: 'ftpd_port' },
  { name: 'ftpd_bandwidth' },
  { name: 'ftpd_allow_ext_access' },
  { id: 'ftpd_table_add' },
  { id: 'ftpd_table_delete' },
  { id: 'ftpd_table_modify' },
  { id: 'ftpd_table_up' },
  { id: 'ftpd_table_down' },
  { id: 'ftpd_table_element' }
];
var F; 
function local_rule_check(T) {
    if ((!E('username').value)) {
        alert('User name is required.');
        return false;
    }

    if (!E('passwd').value) {
        alert('Password is required.');
        return false;
    }

    return true;
}
function local_table_check(T, R) {
    return rlist_check_dup_field(T, R, enum_ftp._NAME);
}

var enum_ftp = {                                                                                                          
    _ENABLE:0,
    _NAME:1,
    _PASSWD:2,
    _UPLOAD:3,
    _DOWNLOAD:4,
    _IPADDR:5
} 

var default_rule = ['1','','','','',''];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='ftpd_table_old_seq' value='"+nth+"' />"
        + "<input id='ftpd_table_seq' size='4' maxlength='4' value='"+nth+"' />"
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
        + "<input id='passwd' name='passwd' size='20' maxlength='20' onblur='valid_name(this, \"Password\", 1)'/>"
        +"</div>";
    
    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Per session upload rate");%>' + "</div>"
        + "<input id='upload' name='upload' size='20' maxlength='20' onblur='valid_number(this, \"Upload\", 1)'/>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Per session download rate");%>' + "</div>"
        + "<input id='download' name='download' size='20' maxlength='20' onblur='valid_number(this, \"Download\", 1)'/>"
        +"</div>";

    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    tbl_assign_enable('enable', rule[enum_ftp._ENABLE]);
    E('username').value = rule[enum_ftp._NAME];
    E('passwd').value = rule[enum_ftp._PASSWD];
    E('upload').value = rule[enum_ftp._UPLOAD];
    E('download').value = rule[enum_ftp._DOWNLOAD];
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("User Name");%></th>'
        + '<th><%lang("Password");%></th>'
        + '<th><%lang("Per Session Upload");%> (KB/s)</th>'
        + '<th><%lang("Per Session Download");%> (KB/s)</th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + tbl_wrap_enable(R[i][enum_ftp._ENABLE]) + "</td>"
             + "<td>" + R[i][enum_ftp._NAME] + "</td>"
             + "<td>" + R[i][enum_ftp._PASSWD] + "</td>"
             + "<td>" + R[i][enum_ftp._UPLOAD] + "</td>"
             + "<td>" + R[i][enum_ftp._DOWNLOAD] + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, prefix) {
    /* Important */
    tbl_append_element(F, 'ftpd_rule', '');
    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, prefix + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, prefix + i + '_enable', ftpd_user_rule[i][enum_ftp._ENABLE]);
        tbl_append_element(F, prefix + i + '_username', ftpd_user_rule[i][enum_ftp._NAME]);
        tbl_append_element(F, prefix + i + '_passwd', ftpd_user_rule[i][enum_ftp._PASSWD]);
        tbl_append_element(F, prefix + i + '_upload', ftpd_user_rule[i][enum_ftp._UPLOAD]);
        tbl_append_element(F, prefix + i + '_download', ftpd_user_rule[i][enum_ftp._DOWNLOAD]);
        tbl_append_element(F, prefix + i + '_ipaddr', ftpd_user_rule[i][enum_ftp._IPADDR]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_ftp._ENABLE]   = tbl_dewrap_enable('enable');
    rule_tmp[enum_ftp._NAME]     = E('username').value;
    rule_tmp[enum_ftp._PASSWD]   = E('passwd').value;
    rule_tmp[enum_ftp._UPLOAD]   = E('upload').value;
    rule_tmp[enum_ftp._DOWNLOAD]   = E('download').value;
    rule_tmp[enum_ftp._IPADDR]   = '';

    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "ftp";
	F.action.value = "Apply";
    if (local_table_check("ftpd_table", ftpd_user_rule) == false) {
        return;
    }
    local_pack_key_value_pair(F, "ftpd_table", "ftpd_user_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("ftpd_enable")[0].checked == true) ? 1 : 0;

    sel_change(enable, 1, F, ftpd_objects);
    if (enable == 1) {
        tbl_disable_row_edit('ftpd_table');
    }
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Storage", "FTP Server"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Storage");%>', '<%lang("FTP Server");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('ftpd_rule')</script>	

              <% tbl_get("ftpd_user_rule"); %>

              <fieldset>
                <legend><%lang("FTP Server");%></legend>
                <div class="setting">
                  <div class="label"><% lang("FTP Server"); %></div>
                  <input type="radio" value="1" name="ftpd_enable" onclick="init()"
                        <%nvg_attr_match("ftpd_enable","ftpd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="ftpd_enable" onclick="init()"
                        <%nvg_attr_match("ftpd_enable","ftpd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>
                <div class="setting">
                <div class="label"><%lang("FTP Server Port");%></div>
                <input name="ftpd_port" size="5" maxLength="5"
                value='<%nvg_attr_get("ftpd_port", "ftpd_rule",   "0", "port");%>' />
                </div>
                <div class="setting">
                <div class="label"><%lang("FTP Bandwidth");%></div>
                <input name="ftpd_bandwidth" size="5" maxLength="5"
                value='<%nvg_attr_get("ftpd_bandwidth", "ftpd_rule",   "0", "userbandwidth");%>' /> KB/s
                </div>

                <div class="setting">
                <div class="label"><% lang("FTP Server External Access"); %></div>
                <input type="radio" value="1" name="ftpd_allow_ext_access"
                <%nvg_attr_match("ftpd_allow_ext_access","ftpd_rule","0","allow_ext_access","1","checked");%>><%lang("Enable");%>
                </input>

                <input type="radio" value="0" name="ftpd_allow_ext_access"
                <%nvg_attr_match("ftpd_allow_ext_access","ftpd_rule","0","allow_ext_access","0","checked");%>><%lang("Disable");%>
                </input>
                </div>

              </fieldset><br />

              <fieldset>
                <legend><%lang("User Rule");%></legend>
                <%tbl_create_table("ftpd_table", "ftpd_user_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("ftpd_table", ftpd_user_rule);
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
    <script type="text/javascript">create_input_window("ftpd_table", "ftpd_user_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
