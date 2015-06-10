<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VPN / PPTP</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var autodns_objects =
[ { name: 'pptpd_dns' } ];
  
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
  { name: 'pptpd_nat_enable' },
  { id: 'pptpd_table_add' },
  { id: 'pptpd_table_delete' },
  { id: 'pptpd_table_modify' },
  { id: 'pptpd_table_up' },
  { id: 'pptpd_table_down' },
  { id: 'pptpd_table_element' }
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
    return rlist_check_dup_field(T, R, enum_pptp._NAME);
}

var enum_pptp = {                                                                                                          
    _ENABLE:0,                                                                                                             
    _NAME:1,                                                                                                               
    _PROVIDER:2,                                                                                                           
    _PASSWD:3,                                                                                                             
    _IPADDR:4                                                                                                              
} 

var default_rule = ['1','','','',''];

var rule; 
function local_create_input_field(T, nth, r) {
    rule = (r != undefined) ? r : default_rule;
        
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='pptpd_table_old_seq' value='"+nth+"' />"
        + "<input id='pptpd_table_seq' size='4' maxlength='4' value='"+nth+"' />"
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
    

    E(T+'_input_field').innerHTML = s;

    local_fill_input_field();
}

function local_fill_input_field() {
    tbl_assign_enable('enable', rule[enum_pptp._ENABLE]);
    E('username').value = rule[enum_pptp._NAME];
    E('passwd').value = rule[enum_pptp._PASSWD];
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
             + "<td>" + tbl_wrap_enable(R[i][enum_pptp._ENABLE]) + "</td>"
             + "<td>" + R[i][enum_pptp._NAME] + "</td>"
             + "<td>" + R[i][enum_pptp._PASSWD] + "</td>"
           + "</tr>";
    }

    return c;
}

function local_pack_key_value_pair(F, T, prefix) {
    /* Important */
    tbl_append_element(F, 'pptpd_rule', '');
    var row_num = parseInt(E(T+'_cur_rule_num').value);

    tbl_append_element(F, prefix + "_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, prefix + i + '_enable', pptpd_user_rule[i][enum_pptp._ENABLE]);
        tbl_append_element(F, prefix + i + '_username', pptpd_user_rule[i][enum_pptp._NAME]);
        tbl_append_element(F, prefix + i + '_provider', pptpd_user_rule[i][enum_pptp._PROVIDER]);
        tbl_append_element(F, prefix + i + '_passwd', pptpd_user_rule[i][enum_pptp._PASSWD]);
        tbl_append_element(F, prefix + i + '_ipaddr', pptpd_user_rule[i][enum_pptp._IPADDR]);
    }
}

function local_generate_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_pptp._ENABLE]   = tbl_dewrap_enable('enable');
    rule_tmp[enum_pptp._NAME]     = E('username').value;
    rule_tmp[enum_pptp._PROVIDER] = '';
    rule_tmp[enum_pptp._PASSWD]   = E('passwd').value;
    rule_tmp[enum_pptp._IPADDR]   = '';

    return rule_tmp;
}

function to_submit(F) {
	F.submit_button.value = "vpn_pptpd";
	F.action.value = "Apply";
    if (local_table_check("pptpd_table", pptpd_user_rule) == false) {
        return;
    }
    local_pack_key_value_pair(F, "pptpd_table", "pptpd_user_rule");
    ajax_preaction(F, "apply.cgi", 6000);
    display_mask(F);
}


function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("pptpd_enable")[0].checked == true) ? 1 : 0;
    var autodns_enable = (document.getElementsByName("pptpd_dns_auto_enable")[0].checked == true) ? 1 : 0;

    sel_change(enable, 1, F, pptpd_objects);
    if (enable == 1) {
        sel_change(autodns_enable, 0, F, autodns_objects);
        tbl_disable_row_edit('pptpd_table');
    }
    <%widget_start();%>
}
</script>
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

              <% tbl_get("pptpd_user_rule"); %>

              <fieldset>
                <legend><%lang("PPTP");%></legend>
                <div class="setting">
                  <div class="label"><% lang("PPTP"); %></div>
                  <input type="radio" value="1" name="pptpd_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","1","checked");%>><%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="pptpd_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_enable","pptpd_rule","0","enable","0","checked");%>><%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><%lang("MTU");%></div>
                  <input name="pptpd_mtu" size="4" maxLength="4" 
                      value='<%nvg_attr_get("pptpd_mtu", "pptpd_rule", "0", "mtu");%>'
                      onblur="valid_number(this, 'PPTP MTU')" /> <%lang("Bytes");%>
                </div>

                <div class="setting">
                  <div class="label"><%lang("VPN Start IP Address");%></div>
                  <input name="pptpd_remoteip" size="20" maxLength="15" 
                      value='<%nvg_attr_get("pptpd_remoteip", "pptpd_rule", "0", "remoteip");%>'
                      onblur="valid_ipaddr(this, 'PPTP Local IP')" />
                </div>

                <div class="setting">
                  <div class="label"><%lang("Max VPN Clients");%></div>
                    <input size="3" maxLength="3" name="pptpd_remoteip_range" 
                          value='<%nvg_attr_get("pptpd_remoteip_range", "pptpd_rule", "$0", "remoteip_range");%>'
                          onblur='valid_number(this, "PPTP Remote IP Range")' />
                </div>

                <div class="setting">
                  <div class="label"><% lang("Auto DNS"); %></div>
                  <input type="radio" value="1" name="pptpd_dns_auto_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_dns_auto_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_dns_auto_enable","pptpd_rule","0","dns_auto_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><%lang("DNS");%></div>
                  <input type="text" maxLength="15" size="20" name="pptpd_dns"
                      value='<%nvg_attr_get("pptpd_dns", "pptpd_rule", "$0", "dns");%>'
                      onblur='valid_reserved_word(this, "PPTP DNS")' />
                </div>

                <div class="setting">
                  <div class="label"><% lang("CHAP Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_chap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_chap_enable","pptpd_rule","0","chap_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("MSCHAP Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_mschap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_mschap_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschap_enable","pptpd_rule","0","mschap_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("MSCHAP v2 Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_mschapv2_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_mschapv2_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mschapv2_enable","pptpd_rule","0","mschapv2_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("MPPE128 Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_mppe128_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_mppe128_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_mppe128_enable","pptpd_rule","0","mppe128_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("Proxy ARP Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_proxyarp_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_proxyarp_enable","pptpd_rule","0","proxyarp_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>

                <div class="setting">
                  <div class="label"><% lang("NAT Enable"); %></div>
                  <input type="radio" value="1" name="pptpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","1","checked");%>>
                        <%lang("Enable");%>
                  </input>
                  <input type="radio" value="0" name="pptpd_nat_enable" onclick="init()"
                        <%nvg_attr_match("pptpd_nat_enable","pptpd_rule","0","nat_enable","0","checked");%>>
                        <%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />

              <fieldset>
                <legend><%lang("User Rule");%></legend>
                <%tbl_create_table("pptpd_table", "pptpd_user_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("pptpd_table", pptpd_user_rule);
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
    <script type="text/javascript">create_input_window("pptpd_table", "pptpd_user_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
