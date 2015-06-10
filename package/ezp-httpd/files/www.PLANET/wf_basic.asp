<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Web Filtering</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var F;

function local_rule_check(T) {

    if ((!E('keyword').value)) {
        alert('Keyword is required for filtering.');
        return false;
    }
    
    return true;
}

var wf_rule_objects =
[ {name: 'activex_enable' },
  {name: 'java_enable' },
  {name: 'proxy_enable' },
];

var wf_objects =
[ { id: 'wf_table_add' },
  { id: 'wf_table_delete' },
  { id: 'wf_table_modify' },
  { id: 'wf_table_up' },
  { id: 'wf_table_down' },
  { id: 'wf_table_element' }
];
 
var enum_wf_basic = {
    _ENABLE:0,
    _KEYWORD:1,
    _TYPE:2,
    _ACCESS:3
}; 

var default_rule = ['0','web-page-name','url','allow'];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='label'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='wf_table_old_seq' value='"+nth+"' />"
        + "<input id='wf_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='label'>"+'<%lang("Rule Enable");%>' + "</div>"
        + "<input id='enable' name='enable' type='checkbox' value='1' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='label'>"+'<%lang("Filter Keyword");%>' + "</div>"
        + "<input id='keyword' name='keyword' size='17' maxlength='15' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='label'>"+'<%lang("Filter Type");%>'+"</div>"
        + "<select id='type' name='type' >"
          + "<option value='url' selected>"+'<%lang("url");%>'+"</option>"
          + "<option value='host'>"+'<%lang("host");%>'+"</option>"
        + "</select>"
        + "</div>"

    s += "<div class='setting'>"
        + "<div class='label'>"+'<%lang("Action");%>'+"</div>"
        + "<select id='access' name='access' >"
          + "<option value='0'>"+'<%lang("DENY");%>'+"</option>"
          + "<option value='1'>"+'<%lang("ALLOW");%>'+"</option>"
        + "</select>"
        + "</div>"

    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    tbl_assign_enable('enable', rule[enum_wf_basic._ENABLE]);
    E('keyword').value         = rule[enum_wf_basic._KEYWORD];
    E('type').value        = rule[enum_wf_basic._TYPE];
    E('access').value        = rule[enum_wf_basic._ACCESS];
}

function local_create_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("Filter Keyword");%></th>'
        + '<th><%lang("Filter Type");%></th>'
        + '<th><%lang("Action");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + tbl_wrap_enable(R[i][enum_wf_basic._ENABLE]) + "</td>"
             + "<td>" + R[i][enum_wf_basic._KEYWORD] + "</td>"
             + "<td>" + R[i][enum_wf_basic._TYPE] + "</td>"
             + "<td>" + tbl_wrap_action(R[i][enum_wf_basic._ACCESS]) + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, P) {
    /* Important */
    tbl_append_element(F, 'wf_rule', '');

    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, P + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, P + i + '_enable', wf_rule[i][enum_wf_basic._ENABLE]);
        tbl_append_element(F, P + i + '_keyword', wf_rule[i][enum_wf_basic._KEYWORD]);
        tbl_append_element(F, P + i + '_type', wf_rule[i][enum_wf_basic._TYPE]);
        tbl_append_element(F, P + i + '_access', wf_rule[i][enum_wf_basic._ACCESS]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_wf_basic._ENABLE]   = tbl_dewrap_enable('enable');
    rule_tmp[enum_wf_basic._KEYWORD]  = E('keyword').value;
    rule_tmp[enum_wf_basic._TYPE]     = E('type').value;
    rule_tmp[enum_wf_basic._ACCESS]   = E('access').value;
    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "wf_basic";
	F.action.value = "Apply";
    local_pack_key_value_pair(F, "wf_table", "wf_rule");
    ajax_preaction(F);
    display_mask(F);
}

function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("wf_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, wf_objects);
    sel_change(enable, 1, F, wf_rule_objects);

    if (enable == 1) {
        tbl_disable_row_edit('wf_table');
    }
    <%widget_start();%>
}
</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
          <% do_menu("Security", "Web Filtering"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Security");%>', '<%lang("Web Filtering");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements()</script>	

              <% tbl_get("wf_rule"); %>

              <fieldset>
                <legend><%lang("Web Filtering");%></legend>
                <div class="setting">
                  <!-- Enable/disable web filtering -->
                  <div class="label"><% lang("Web Filtering"); %></div>
                  <input type="radio" value="1" name="wf_enable" onclick="init()"
                      <%nvg_match("wf_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="wf_enable" onclick="init()"
                      <%nvg_match("wf_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br />
                </div>
              </fieldset><br />

              <fieldset>
                <legend><%lang("Web Content Filtering");%></legend>
                <div class="setting">
                  <!-- Enable/disable activex filtering -->
                  <div class="label"><% lang("Activex Filtering"); %></div>
                  <input type="radio" value="1" name="activex_enable" onclick="init()"
                      <%nvg_attr_match("activex_enable", "wf_content_rule", "0", 
                                       "activex_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="activex_enable" onclick="init()"
                      <%nvg_attr_match("activex_enable", "wf_content_rule", "0", 
                                       "activex_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br /> 
                  
                  <!-- Enable/disable java filtering -->
                  <div class="label"><% lang("Java/JavaScript Filtering"); %></div>
                  <input type="radio" value="1" name="java_enable" onclick="init()"
                      <%nvg_attr_match("java_enable", "wf_content_rule", "0", 
                                       "java_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="java_enable" onclick="init()"
                      <%nvg_attr_match("java_enable", "wf_content_rule", "0", 
                                       "java_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br /> 
                  
                  <!-- Enable/disable proxy filtering -->
                  <div class="label"><% lang("Proxy Filtering"); %></div>
                  <input type="radio" value="1" name="proxy_enable" onclick="init()"
                      <%nvg_attr_match("proxy_enable", "wf_content_rule", "0", 
                                       "proxy_enable", "1", "checked");%>>
                      <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="proxy_enable" onclick="init()"
                      <%nvg_attr_match("proxy_enable", "wf_content_rule", "0", 
                                       "proxy_enable", "0", "checked");%>>
                      <%lang("Disable");%>
                  </input>
                  <br /> 
                </div>
              </fieldset><br />
              <fieldset>
                <legend><%lang("Web Filtering Rule");%></legend>
                <%tbl_create_table("wf_table", "wf_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("wf_table", wf_rule);
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
    <script type="text/javascript">create_input_window("wf_table", "wf_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
