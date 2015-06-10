<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - BWM</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var bw_objects = new Array(<%nv_get("wan_num");%>);
var wan_bw_objects = new Array(<%nv_get("wan_num");%>);
var F;

var bw_sbm_objects =
[ { id: 'sbm_table_add' },
  { id: 'sbm_table_delete' },
  { id: 'sbm_table_modify' },
  { id: 'sbm_table_up' },
  { id: 'sbm_table_element' }
];
 
var bw_dbm_objects =
[ { id: 'dbm_table_add' },
  { id: 'dbm_table_delete' },
  { id: 'dbm_table_modify' },
  { id: 'dbm_table_up' },
  { id: 'dbm_table_element' }
];
    
var proto_objects =
[ { id: 'sbm_table_extport_start' },
  { id: 'sbm_table_extport_end' },
 
];


function SelBWMode(F) {
    F.submit_button.value = "bw_basic";
    F.action.value = "Gozila";
    F.submit();
}

function bw_enable_disable(up,type) {
    if( type == "Custom") {
        choose_enable(F.elements[ul]);
    } else{
        choose_disable(F.elements[ul]);
    }
}

function proto_sel_wrapper(v, check) {
    var F = document.getElementsByName("form")[0];
    if (v != check) {
        sel_enable_objs(F, proto_objects);
    } else {
        sel_disable_objs(F, proto_objects);
        E('sbm_table_extport_start').value = "";
        E('sbm_table_extport_end').value = "";
    }
}                                                                                                                                         

function bw_update_total_available_bw(nth) {
    var ul = parseInt(E('wan'+nth+'_bw_ul').value) * (100 - parseInt(E('wan'+nth+'_bw_resv').value)) / 100;
    E('wan'+nth+'_totall_available_bw').innerHTML = ul.toFixed(1) + ' Kbps';
}

function sbm_update_used_bw(flag) {
    /* Initiate */
    for (j = 0; j < <%nv_get("wan_num");%>; j++) {
        E('wan'+j+'_used_bw_ul').value = '0';
    }

    for (var i = 0; i <  parseInt(E("sbm_table_cur_rule_num").value); i++) {
        if (bw_sbm_rule[i][enum_sbm._ENABLE] == '0') {
            continue; 
        }

        /* flag is used to identify this function is invoked when
         * sbm_input_field pops up.
         */
        if (flag == '1' && E('sbm_table_rule_flag').value == '0' && tbl_get_tr_index() == i) {
            /* Ignore the current editing rule. It would be calculated in
             * the input_field because it might be changed for editing.
             */
            continue;
        }

        var ul;
        for (j = 0; j < <%nv_get("wan_num");%>; j++) {
            if (bw_sbm_rule[i][enum_sbm._EXTIF] == 'wan'+j) {
                if (bw_sbm_rule[i][enum_sbm._BW_ALLOC] == 'ratio') {
                    ul = parseInt(E('wan'+j+'_bw_ul').value) * (100 - parseInt(E('wan'+j+'_bw_resv').value)) / 100;
                    var ratio = bw_sbm_rule[i][enum_sbm._BW_RATIO];
                    ratio = (ratio == '') ? 0 : ratio;
                    E('wan'+j+'_used_bw_ul').value = parseInt(E('wan'+j+'_used_bw_ul').value) + ul * parseInt(ratio) / 100;
                } else {
                    var used_ul = bw_sbm_rule[i][enum_sbm._BW_UPLOAD];
                    E('wan'+j+'_used_bw_ul').value = parseInt(E('wan'+j+'_used_bw_ul').value) + parseInt(used_ul);
                }
            }
        }
    }
}

function dbm_update_available_bw() {
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var ul = parseInt(E('wan'+i+'_bw_ul').value) * (100 - parseInt(E('wan'+i+'_bw_resv').value)) / 100;
        /* TODO: dirty hack. Don't know why E('wan'+i+'_available_bw') would be
         * undefined at loading the page.
         */

        if (E('wan'+i+'_available_bw') != undefined) {
            ul = ul - parseInt(E('wan'+i+'_used_bw_ul').value);
            if (ul < 0) {
                alert('<%lang("WAN");%> '+'<%increase("i", "1");%>:');
            }

            E('wan'+i+'_available_bw').innerHTML = ul.toFixed(1) + ' Kbps';
        }
    }
}

function bw_update_available_bw(flag) {
	sbm_update_used_bw(flag);
    dbm_update_available_bw();
}

function sbm_update_available_bw() {
    rule = local_generate_rule('sbm_table');

    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var ul = parseInt(E('wan'+i+'_bw_ul').value) * (100 - parseInt(E('wan'+i+'_bw_resv').value)) / 100;
        var b = (parseInt(ul) - parseInt(E('wan'+i+'_used_bw_ul').value));
        E('wan'+i+'_bw').innerHTML = b.toFixed(1) + ' Kbps';
    }

    if (rule[enum_sbm._ENABLE] == '0') {
        return;
    }

    bw_update_available_bw('1');
    if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var ul = parseInt(E('wan'+i+'_bw_ul').value) * (100 - parseInt(E('wan'+i+'_bw_resv').value)) / 100;
            if (rule[enum_sbm._EXTIF] == 'wan' + i) {
                var ratio = E('sbm_table_bw_ratio').value;
                ratio = (ratio == '') ? 0 : ratio;
                ul = ul * (100 - parseInt(ratio)) / 100;
                ul = ul - parseInt(E('wan'+i+'_used_bw_ul').value);
                if (ul < 0) {
                    alert('<%lang("WAN");%> '+'<%increase("i", "1");%>:');
                }
                E('wan'+i+'_bw').innerHTML = ul.toFixed(1) + ' Kbps';
            }
        }
    <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    } else if (rule[enum_sbm._BW_ALLOC] == 'group') {
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var groupid = parseInt(E('sbm_table_bw_group').value);
            var ul = parseInt(E('wan'+i+'_bw_ul').value) * (100 - parseInt(E('wan'+i+'_bw_resv').value)) / 100;
            if (rule[enum_sbm._EXTIF] == 'wan' + i) {
                var ratio = get_bw_group_rate_ratio(groupid);
                ratio = (ratio == '') ? 0 : ratio;
                ul = ul * (100 - parseInt(ratio)) / 100;
                ul = ul - parseInt(E('wan'+i+'_used_bw_ul').value);
                if (ul < 0) {
                    alert('<%lang("WAN");%> '+'<%increase("i", "1");%>:');
                }
                E('wan'+i+'_bw').innerHTML = ul.toFixed(1) + ' Kbps';
            }
        }
    <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
    } else {
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var ul = parseInt(E('wan'+i+'_bw_ul').value) * (100 - parseInt(E('wan'+i+'_bw_resv').value)) / 100;
            var b = (parseInt(ul) - parseInt(E('wan'+i+'_used_bw_ul').value));
            E('wan'+i+'_bw').innerHTML = b.toFixed(1) + ' Kbps';
            if (rule[enum_sbm._EXTIF] == 'wan' + i) {
                var used_ul = E('sbm_table_bw_upload').value;
                used_ul = (used_ul == '') ? 0 : used_ul;
                ul = ul - parseInt(used_ul) - parseInt(E('wan'+i+'_used_bw_ul').value);
                if (ul < 0) {
                    alert('<%lang("WAN");%> '+'<%increase("i", "1");%>:');
                }
                E('wan'+i+'_bw').innerHTML = ul.toFixed(1) + ' Kbps';
                <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
                if (E('wan'+i+'_available_bw') != undefined) {
                    E('wan'+i+'_available_bw').innerHTML = ul.toFixed(1) + ' Kbps';
                }
                <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
            }
        }
    }
}

function local_rule_check(T) {
    if (T == 'sbm_table') {
        if (_local_sbm_rule_check() == false) {
            return false;
        }
    }
    else if (T == 'bw_group_table') {
        if (_local_bw_group_rule_check() == false) {
            return false;
        }
    <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
    } else {
        if (_local_dbm_rule_check() == false) {
            return false;
        }
    <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    }
    return true;
}

function _local_bw_group_rule_check(T) {
    if (!E('bw_group_name').value) {
        alert('Please specify group name.');
        return false;
    }

    if (!E('bw_group_uprate').value) {
        alert('Please specify guaranteed rate ratio (1 ~ 100 %).');
        return false;
    }

    if (!E('bw_group_upceil').value) {
        alert('Please specify maximum rate ratio (1 ~ 100 %).');
        return false;
    }

    return true;
}

function _local_dbm_rule_check(T) {
    if ((E('dbm_table_inipaddr_start').value) && !E('dbm_table_inipaddr_end').value) {
        alert('DBM Internal IP End is needed.');
        return false;
    }

    if ((!E('dbm_table_inipaddr_start').value) && E('dbm_table_inipaddr_end').value) {
        alert('DBM Internal IP Start is needed.');
        return false;
    }

    if (!tbl_check_valid_ipaddr_range(E('dbm_table_inipaddr_start').value, 
                                      E('dbm_table_inipaddr_end').value, 'DBM Internal IP')) {
        return false;
    }

    return true;
}

function _local_sbm_rule_check(T) {
    if (!E('sbm_table_inipaddr_start').value && E('sbm_table_proto').value == '') {
        alert('<%lang("SBM internal IP or protocol/port");%>'+': '+'<%lang("not defined");%>');
        return false;
    }
    
    if (E('sbm_table_proto').value != '') {
        if (E('sbm_table_extport_start').value == '' || E('sbm_table_extport_end').value == '') {
            alert('<%lang("SBM port");%>'+': '+'<%lang("not defined");%>');
            return false;
        }
        
        if (!tbl_check_valid_port_range(E('sbm_table_extport_start').value, 
                                        E('sbm_table_extport_end').value, 'SBM Port')) {
            return false;
        }
    }

    if (E('sbm_table_bw_alloc').value == 'ratio') {
        if (E('sbm_table_bw_ratio').value == '') {
            alert('<%lang("SBM bandwidth ratio");%>'+': '+'<%lang("not defined");%>');
            return false;
        }
        if (valid_number(E('sbm_table_bw_ratio'), 'SBM bandwidth ratio') == false) {
            return false;
        }
    } else if (E('sbm_table_bw_alloc').value == 'bw') {
        if (E('sbm_table_bw_upload').value == '') {
            alert('<%lang("SBM bandwidth upload");%>'+': '+'<%lang("not defined");%>');
            return false;
        }
        if (valid_number(E('sbm_table_bw_upload'), 'Upload') == false) {
            return false;
        }
    }

    rule = local_generate_rule('sbm_table');
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var total_ul = parseInt(E('wan'+i+'_bw_ul').value) * (100 - parseInt(E('wan'+i+'_bw_resv').value)) / 100;
        if (rule[enum_sbm._EXTIF] == 'wan'+i) {
            if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
                var ratio = rule[enum_sbm._BW_RATIO];
                ul = parseInt(total_ul) * (100 - parseInt(ratio)) / 100;
            } else {
                ul = parseInt(total_ul) - parseInt(rule[enum_sbm._BW_UPLOAD]);
            }
            if (rule[enum_sbm._EXCESS] == '1') {
                if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
                    if (parseInt(rule[enum_sbm._EXCESS_RATIO]) > 100) {
                        var M = '<%lang("Use Maximal Ratio");%>' + ': '+ '<%lang("should be less than");%>' 
                                            + ' 100' + '<%lang(".");%>';
                        alert(M);
                        return false;
                    }
                    if (parseInt(rule[enum_sbm._EXCESS_RATIO]) < parseInt(rule[enum_sbm._BW_RATIO])) {
                        var M = '<%lang("Use Maximal Ratio");%>' + ': '+ '<%lang("should be larger than");%>' 
                                        + ' ' + rule[enum_sbm._BW_RATIO] + '<%lang(".");%>';
                        alert(M);
                        return false;
                    }
                } else {
                    if (parseInt(rule[enum_sbm._EXCESS_UPLOAD]) > parseInt(total_ul)) {
                        var M = '<%lang("Use Maximal Upload");%>' + ': '+ '<%lang("should be less than");%>' 
                                            + total_ul + '<%lang(".");%>';
                        alert(M);
                        return false;
                    }
                    if (parseInt(rule[enum_sbm._EXCESS_UPLOAD]) < parseInt(rule[enum_sbm._BW_UPLOAD])) {
                        var M = '<%lang("Use Maximal Upload");%>' + ': '+ '<%lang("should be larger than");%>' 
                                            + rule[enum_sbm._BW_UPLOAD] + '<%lang(".");%>';
                        alert(M);
                        return false;
                    }
                }
            }
        }
        ul = ul - parseInt(E('wan'+i+'_used_bw_ul').value);
        if (ul < 0) {
            alert('<%lang("WAN");%> '+'<%increase("i", "1");%>:' );
            return false;
        }
    }
    return true;
}

function local_pack_sbm_key_value_pair(F) {

    var row_num = parseInt(E('sbm_table_cur_rule_num').value);

    tbl_append_element(F, "bw_sbm_rule_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, 'bw_sbm_rule' + i + '_name', bw_sbm_rule[i][enum_sbm._NAME]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_enable', bw_sbm_rule[i][enum_sbm._ENABLE]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_inipaddr_start', bw_sbm_rule[i][enum_sbm._INIPADDR_START]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_inipaddr_end', bw_sbm_rule[i][enum_sbm._INIPADDR_END]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_proto', bw_sbm_rule[i][enum_sbm._PROTOCOL]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_extport_start', bw_sbm_rule[i][enum_sbm._PORT_START]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_extport_end', bw_sbm_rule[i][enum_sbm._PORT_END]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_extif', bw_sbm_rule[i][enum_sbm._EXTIF]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_bw_alloc', bw_sbm_rule[i][enum_sbm._BW_ALLOC]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_bw_ratio', bw_sbm_rule[i][enum_sbm._BW_RATIO]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_bw_upload', bw_sbm_rule[i][enum_sbm._BW_UPLOAD]);
        tbl_append_element(F, 'bw_sbm_rule' + i + '_excess', bw_sbm_rule[i][enum_sbm._EXCESS]); 
        tbl_append_element(F, 'bw_sbm_rule' + i + '_excess_ratio', bw_sbm_rule[i][enum_sbm._EXCESS_RATIO]); 
        tbl_append_element(F, 'bw_sbm_rule' + i + '_excess_upload', bw_sbm_rule[i][enum_sbm._EXCESS_UPLOAD]); 
        tbl_append_element(F, 'bw_sbm_rule' + i + '_groupid', bw_sbm_rule[i][enum_sbm._GROUPID]); 
        tbl_append_element(F, 'bw_sbm_rule' + i + '_dscpid', bw_sbm_rule[i][enum_sbm._DSCPID]); 
        tbl_append_element(F, 'bw_sbm_rule' + i + '_remark_dscpid', bw_sbm_rule[i][enum_sbm._REMARKDSCPID]); 
    }
}

function local_pack_dbm_key_value_pair(F) {

    var row_num = parseInt(E('dbm_table_cur_rule_num').value);

    tbl_append_element(F, "bw_dbm_rule_num", row_num);

    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, 'bw_dbm_rule' + i + '_name', bw_dbm_rule[i][enum_dbm._NAME]);
        tbl_append_element(F, 'bw_dbm_rule' + i + '_enable', bw_dbm_rule[i][enum_dbm._ENABLE]);
        tbl_append_element(F, 'bw_dbm_rule' + i + '_inipaddr_start', bw_dbm_rule[i][enum_dbm._INIPADDR_START]);
        tbl_append_element(F, 'bw_dbm_rule' + i + '_inipaddr_end', bw_dbm_rule[i][enum_dbm._INIPADDR_END]);
    }
}

function to_submit(F) {
    F.submit_button.value = "bw_basic";
	F.action.value = "Apply";
    local_pack_sbm_key_value_pair(F);
    <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    local_pack_bw_group_key_value_pair(F);
    <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
    <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
    local_pack_dbm_key_value_pair(F);
    <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    ajax_preaction(F);
    display_mask(F);
}
function init() {
    F = document.getElementsByName("form")[0];
    var enable = (document.getElementsByName("bw_enable")[0].checked == true) ? 1 : 0;
    sel_change(enable, 1, F, bw_sbm_objects);
    <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
    sel_change(enable, 1, F, bw_dbm_objects);
    <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        bw_update_total_available_bw(i);
        sel_change(enable, 1, F, bw_objects[i]);
        if (enable == 1) {
            if (N(F,"wan"+i+"_bw_type")) {
                sel_change(N(F,"wan"+i+"_bw_type").value,"Custom",F,wan_bw_objects[i]);
            }
        }
    }
    if (enable == 1) {
        tbl_disable_row_edit('sbm_table');
        <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
        tbl_disable_row_edit('dbm_table');
        <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    }
    <%widget_start();%>

    sbm_update_used_bw('0');
}
function do_sbm_used_bw() {
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        W('<input type="hidden" id="wan'+i+'_used_bw_ul" name="wan'+i+'_used_bw_ul" value="0" />');
    }
}
function do_dbm_available_bw() {
    W('<div class="setting">');
    W('<div class="label">'+'<%lang("DBM Available Bandwidth");%>'+'</div>&nbsp;');
    W('</div>');
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        W('<div class="setting">');
        W('<div class="label">'+'<%lang("WAN");%> '+ (i+1) +'</div>');
        W('<div id="wan' + i + '_available_bw">&nbsp;</div>');
        W('</div>');
    }

    bw_update_available_bw('0'); 
}
function sel_bw(element, F, i, objects) {
    if (element.value == 'A1M64K') {
        F.elements["wan"+i+"_bw_ul"].value = "64";
    } else if (element.value == 'A1.5M640K') {
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == 'A2M256K') {
        F.elements["wan"+i+"_bw_ul"].value = "256";
    } else if (element.value == 'A2M512K') {
        F.elements["wan"+i+"_bw_ul"].value = "512";
    } else if (element.value == 'A3M640K') {
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == 'A4M1M') {
        F.elements["wan"+i+"_bw_ul"].value = "1024";
    } else if (element.value == 'A6M256K') {
        F.elements["wan"+i+"_bw_ul"].value = "256";
    } else if (element.value == 'A6M640K') {
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == 'A8M256K') {
        F.elements["wan"+i+"_bw_ul"].value = "256";
    } else if (element.value == 'A8M640K') {
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == 'A8M800K') {
        F.elements["wan"+i+"_bw_ul"].value = "800";
    } else if (element.value == 'A10M2M') {
        F.elements["wan"+i+"_bw_ul"].value = "2048";
    } else if (element.value == 'A12M1M') {
        F.elements["wan"+i+"_bw_ul"].value = "1024";
    } else if (element.value == 'C1M128K') {
        F.elements["wan"+i+"_bw_ul"].value = "128";
    } else if (element.value == 'C2M128K') {
        F.elements["wan"+i+"_bw_ul"].value = "128";
    } else if (element.value == 'C2M384K') {
        F.elements["wan"+i+"_bw_ul"].value = "384";
    } else if (element.value == 'C3M384K') {
        F.elements["wan"+i+"_bw_ul"].value = "384";
    } else if (element.value == 'C6M640K') {
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == 'C8M640K') {
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == 'C10M1M') {
        F.elements["wan"+i+"_bw_ul"].value = "1024";
    } else if (element.value == 'V2M2M') {
        F.elements["wan"+i+"_bw_ul"].value = "2048";
    } else if (element.value == 'V4M4M') {
        F.elements["wan"+i+"_bw_ul"].value = "4096";
    } else if (element.value == 'V10M10M') {
        F.elements["wan"+i+"_bw_ul"].value = "10240";
    } else if (element.value == 'V25M25M') {
        F.elements["wan"+i+"_bw_ul"].value = "25600";
    } else if (element.value == 'V50M50M') {
        F.elements["wan"+i+"_bw_ul"].value = "51200";
    } else if (element.value == 'V100M100M') {
        F.elements["wan"+i+"_bw_ul"].value = "102400";
    } else if (element.value == "Custom") {
        F.elements["wan"+i+"_bw_ul"].value = "";
    }
            
    sel_change(element.value, "Custom", F, objects);
}

function local_create_table(T, R) {
    if (T == 'sbm_table') {
        return _local_create_sbm_table(T, R);
        <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    } else if (T == 'bw_group_table') {
        return bw_group_local_create_table(T, R);
    } else {
        <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
        <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
    } 
    if (T == 'dbm_table') {
        return _local_create_dbm_table(T, R);
    } else {
        <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    }
}

function wrap_sbm_mode(v) {
    if (v == 'ip') {
        return '<%lang("IP Address");%>';
    } else {
        return '<%lang("Application");%>';
    } 
}

function wrap_sbm_proto(v) {
    if (v == 'tcp') {
        return 'TCP';
    } else if (v == 'udp') {
        return 'UDP';
    } else {
        return '';
    }
}
function _local_create_sbm_table(T, R) {
    bw_update_available_bw('0');

    var c;
    c = "<tr>"
        + '<th><%lang("Rule Name");%></th>'
        + '<th><%lang("Enable");%></th>'
        + '<th><%lang("IP Address");%></th>'
        + '<th><%lang("Application");%></th>'
        + '<th><%lang("External Interface");%></th>'
        + '<th><%lang("Bandwidth");%></th>'
      + "</tr>";

    var cur_num = parseInt(E("sbm_table_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\"sbm_table\", this);'>"
             + "<td>" + R[i][enum_sbm._NAME] + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_sbm._ENABLE]) + "</td>"
             + "<td>" + R[i][enum_sbm._INIPADDR_START] + "</td>";
        if (R[i][enum_sbm._PROTOCOL] == '') {
            c += '<td></td>';
        } else {
            c += '<td>' + wrap_sbm_proto(R[i][enum_sbm._PROTOCOL]) + '/';
            if (R[i][enum_sbm._PORT_START] == R[i][enum_sbm._PORT_END]) {
                 c += R[i][enum_sbm._PORT_START];
            } else {
                 c += R[i][enum_sbm._PORT_START] + ':' + R[i][enum_sbm._PORT_END];
            }
            c += "</td>";
        }

        c += '<td>' + tbl_wrap_wan(R[i][enum_sbm._EXTIF]) + '</td>';
        if (R[i][enum_sbm._BW_ALLOC] == 'ratio') {
            c +=   "<td>" + R[i][enum_sbm._BW_RATIO] + " %</td>";
    <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
        } else if (R[i][enum_sbm._BW_ALLOC] == 'group') {
            var groupid = R[i][enum_sbm._GROUPID];
            c +=   "<td>" + get_bw_group_rate_ratio(groupid) + " %</td>";
    <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
        } else {
            c +=   "<td>" + R[i][enum_sbm._BW_UPLOAD] + "Kbps</td>";
        }
        c += "</tr>";
    }

    return c;
}

function _local_create_dbm_table(T, R) {
    var c;
    c = "<tr>"
        + '<th><%lang("Rule Name");%></th>'
        + '<th><%lang("Rule Enable");%></th>'
        + '<th><%lang("DBM IP");%></th>'
      + "</tr>";

    var cur_num = parseInt(E("dbm_table_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\"dbm_table\", this);'>"
             + "<td>" + R[i][enum_dbm._NAME] + "</td>"
             + "<td>" + tbl_wrap_enable(R[i][enum_dbm._ENABLE]) + "</td>";
		if (R[i][enum_dbm._INIPADDR_START] == R[i][enum_dbm._INIPADDR_END]) {			
             c += "<td>" + R[i][enum_dbm._INIPADDR_START] + "</td>";
		} else {
             c += "<td>" + '<%lang("From:");%>'+R[i][enum_dbm._INIPADDR_START] + "<br/>"
                      + '<%lang("To:  ");%>'+R[i][enum_dbm._INIPADDR_END] + "</td>";
		}
        c += "</tr>";
    }

    return c;
}

function local_create_input_field(T, nth, r) {
    if (T == 'sbm_table') {
        rule = (r != undefined) ? r : default_sbm_rule;
        <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    } else if (T == 'bw_group_table') {
        rule = (r != undefined) ? r : default_bw_group_rule;
        <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
        <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
    } else {
        rule = (r != undefined) ? r : default_dbm_rule;
        <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    }
    _local_create_input_field(T, nth);
} 

function _local_create_input_field(T, nth) {
    if (T == 'sbm_table') {
        _local_create_sbm_input_field(T, nth);
    } else if (T == 'bw_group_table') {
        _local_create_bw_group_input_field(T, nth);
        <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
    } else {
        _local_create_dbm_input_field(T, nth);
        <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    }
}
var enum_sbm = {
    _NAME: 0,
    _ENABLE: 1,
    _INIPADDR_START: 2,
    _INIPADDR_END: 3,
    _INIPADDR_NUM: 4,
    _PROTOCOL: 5,
    _PORT_START: 6,
    _PORT_END: 7,
    _EXTIF: 8,
    _BW_ALLOC: 9,
    _BW_RATIO: 10,
    _BW_UPLOAD: 11,
    _BW_DOWNLOAD: 12,
    _SCHED_IDX: 13,
    _EXCESS: 14,
    _EXCESS_RATIO: 15,
    _EXCESS_UPLOAD: 16,
    _EXCESS_DOWNLOAD: 17,
    _DSCPID: 18,
    _REMARKDSCPID: 19,
    _GROUPID: 20
}

var enum_dbm = {
    _NAME: 0,
    _ENABLE: 1,
    _INIPADDR_START: 2,
    _INIPADDR_END: 3,
    _INIPADDR_NUM: 4,
    _SCHED_IDX: 5 
}
var enum_bw_group = {
    _NAME:0,
    _ENABLE:1,
    _UPRATE:2,
    _UPCEIL:3,
    _PRIO:4
} 

var default_bw_group_rule = ['group','1','10','100',''];
var default_sbm_rule = ['','1','','','0','','','','wan0','ratio','','','','','0','100','','','disabled','disabled','0'];
var default_dbm_rule = ['','1','','',''];
var rule = default_sbm_rule;

function get_bw_group_rate_ratio(groupid) {
    return bw_group_rule[groupid][enum_bw_group._UPRATE];
}

function get_bw_group_ceil_ratio(groupid) {
    return bw_group_rule[groupid][enum_bw_group._UPCEIL];
}

function update_bw_group_ratio() {
    var groupid = parseInt(E('sbm_table_bw_group').value);
    E('sbm_table_bw_group_ratio').value = get_bw_group_rate_ratio(groupid);
    sbm_update_available_bw();
}

function _local_create_sbm_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='sbm_table_old_seq' value='"+nth+"' />"
        + "<input id='sbm_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Name");%>' + "</div>"
        + "<input id='sbm_table_name' name='sbm_table_name' size='20' maxlength='20' "
        +           "onblur='valid_name(this, \"Rule Name\", 1)' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Enable");%>' + "</div>"
        + "<input id='sbm_table_enable' name='sbm_table_enable' type='checkbox' value='1' "
        +           "onchange='sbm_update_available_bw()' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Internal IP Address");%>' + "</div>"
        + "<input id='sbm_table_inipaddr_start' name='sbm_table_inipaddr_start' size='17' maxlength='15' "
        +           "onblur='valid_ipaddr(this, \"Internal IP Range Start\")'/>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang(Protocol);%>' + "</div>"
        + "<select id='sbm_table_proto' name='sbm_table_proto' onchange=\"proto_sel_wrapper(value, '')\">"
        +   "<option value=''>*</option>"
        +   "<option value='tcp'>TCP</option>"
        +   "<option value='udp'>UDP</option>"
        + "</select>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Service Port Range");%>' + "</div>"
        + '<%lang("From");%>' + ":"
        + "<input id='sbm_table_extport_start' name='sbm_table_extport_start' size='6' maxlength='5' "
        +         "onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' />"
        + '<%lang("To");%>' + ":" 
        + "<input id='sbm_table_extport_end' name='sbm_table_extport_end' size='6' maxlength='5' "
        +         "onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' />"
        +"</div>";

    if (<%nv_get("wan_num");%> == 1) {
        s += "<input type='hidden' id='sbm_table_extif' name='sbm_table_extif' value='wan0' />";
    } else {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("External Interface");%>' + "</div>"
            + "<select id='sbm_table_extif' name='sbm_table_extif' onchange='sbm_update_available_bw()'/>";
        for (i = 1; i <= <%nv_get("wan_num");%>; i++) {
            s += "<option value='wan" + (i-1) + "' >WAN" + i + "</option>";
        }
        s +=  "</select>"
            +"</div>";
    }

    s += '<div class="setting">'
        + '<div class="ilabel">'+'<%lang("Available Bandwidth");%>'+"</div>"
        + '&nbsp;'
        +'</div>';

    for (i = 0; i < <%nv_get("wan_num");%>; i++) {
        s += "<div class='setting'>"
             + "<div class='ilabel'>WAN" + (i+1) + ":</div>"
             + "<div id='wan" + i + "_bw'></div>"
            +"</div>";
    }

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Bandwidth Allocation");%>' + "</div>"
        + "<select id='sbm_table_bw_alloc' name='sbm_table_bw_alloc' "
        +           "onchange='tbl_input_choose(\""+T+"\", this, enum_sbm._BW_ALLOC);"
        +                      "sbm_update_available_bw()' >"
        +  "<option value='ratio'>" + '<%lang("By Ratio");%>' + "</option>"
        <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
        +  "<option value='group'>" + '<%lang("By Group");%>' + "</option>"
        <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
        +  "<option value='bw'>" + '<%lang("By Bandwidth");%>' + "</option>"
        + "</select>"
        +"</div>";
    if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Ratio");%>' + "</div>"
            + "<input id='sbm_table_bw_ratio' name='sbm_table_bw_ratio' size='4' maxlength='2' "
            +       "onblur='sbm_update_available_bw()' /> %"
            +"</div>";
    /* group is a kind of predefined bandwidth setting. */
    <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    } else if (rule[enum_sbm._BW_ALLOC] == 'group') {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Group");%>' + "</div>"
            + "<select id='sbm_table_bw_group' name='sbm_table_bw_group' onchange='update_bw_group_ratio()' >\n"
/*            + "<select id='sbm_table_bw_group' name='sbm_table_bw_group' onblur='sbm_update_available_bw()'>\n"*/
            + "<option value='0'>" + bw_group_rule[0][enum_bw_group._NAME] + "</option>"
            + "<option value='1'>" + bw_group_rule[1][enum_bw_group._NAME] + "</option>"
            + "<option value='2'>" + bw_group_rule[2][enum_bw_group._NAME] + "</option>"
            + "<option value='3'>" + bw_group_rule[3][enum_bw_group._NAME] + "</option>"
            + "</select>"
            +"</div>";
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Group Ratio");%>' + "</div>"
            + "<input id='sbm_table_bw_group_ratio' name='sbm_table_bw_group_ratio' size='4' maxlength='4' disabled='true' /> %"
            +"</div>";
    <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
    } else {
        s += "<div class='setting'>"
            + "<div class='ilabel'>"+'<%lang("Upload");%>' + "</div>"
            + "<input id='sbm_table_bw_upload' name='sbm_table_bw_upload' size='4' maxlength='7' "
            +       "onblur='sbm_update_available_bw()' /> Kbps"
            +"</div>";
    }
    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Utilize Bandwidth More Than Guaranteed");%>' + "</div>"
        + "<input id='sbm_table_excess' name='sbm_table_excess' type='checkbox' value='1' "
        +           "onchange='tbl_input_choose(\""+T+"\", this, enum_sbm._EXCESS);'>"
        +"</div>";

    if (rule[enum_sbm._EXCESS] == '1') {
        if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
            s += "<div class='setting'>"
                + "<div class='ilabel'>"+'<%lang("Use Maximal Ratio");%>' + "</div>"
                + "<input id='sbm_table_excess_ratio' name='sbm_table_excess_ratio' size='4' maxlength='2' "
                +       "onblur='sbm_update_available_bw()' /> %"
                +"</div>";
        } else if (rule[enum_sbm._BW_ALLOC] == 'bw') {
            s += "<div class='setting'>"
                + "<div class='ilabel'>"+'<%lang("Use Maximal Upload");%>' + "</div>"
                + "<input id='sbm_table_excess_upload' name='sbm_table_excess_upload' size='4' maxlength='7' "
                +       "onblur='sbm_update_available_bw()' /> Kbps"
                +"</div>";
        }
    }

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("DSCP");%>' + "</div>"
        + "<select name='sbm_table_dscpid' id='sbm_table_dscpid' >"
        + "<option value='disabled'> Disabled </option>"
        + "<option value='CS0'> CS0 </option>"
        + "<option value='CS1'> CS1 </option>"
        + "<option value='CS2'> CS2 </option>"
        + "<option value='CS3'> CS3 </option>"
        + "<option value='CS4'> CS4 </option>"
        + "<option value='CS5'> CS5 </option>"
        + "<option value='CS6'> CS6 </option>"
        + "<option value='CS7'> CS7 </option>"
        + "<option value='BE'> BE </option>"
        + "<option value='AF11'> AF11 </option>"
        + "<option value='AF12'> AF12 </option>"
        + "<option value='AF13'> AF13 </option>"
        + "<option value='AF21'> AF21 </option>"
        + "<option value='AF22'> AF22 </option>"
        + "<option value='AF23'> AF23 </option>"
        + "<option value='AF31'> AF31 </option>"
        + "<option value='AF32'> AF32 </option>"
        + "<option value='AF33'> AF33 </option>"
        + "<option value='AF41'> AF41 </option>"
        + "<option value='AF42'> AF42 </option>"
        + "<option value='AF43'> AF43 </option>"
        + "<option value='EF'> EF </option>"
        + "</select>"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Remark DSCP");%>' + "</div>"
        + "<select name='sbm_table_remark_dscpid' id='sbm_table_remark_dscpid' >"
        + "<option value='disabled'> Disabled </option>"
        + "<option value='CS0'> CS0 </option>"
        + "<option value='CS1'> CS1 </option>"
        + "<option value='CS2'> CS2 </option>"
        + "<option value='CS3'> CS3 </option>"
        + "<option value='CS4'> CS4 </option>"
        + "<option value='CS5'> CS5 </option>"
        + "<option value='CS6'> CS6 </option>"
        + "<option value='CS7'> CS7 </option>"
        + "<option value='BE'> BE </option>"
        + "<option value='AF11'> AF11 </option>"
        + "<option value='AF12'> AF12 </option>"
        + "<option value='AF13'> AF13 </option>"
        + "<option value='AF21'> AF21 </option>"
        + "<option value='AF22'> AF22 </option>"
        + "<option value='AF23'> AF23 </option>"
        + "<option value='AF31'> AF31 </option>"
        + "<option value='AF32'> AF32 </option>"
        + "<option value='AF33'> AF33 </option>"
        + "<option value='AF41'> AF41 </option>"
        + "<option value='AF42'> AF42 </option>"
        + "<option value='AF43'> AF43 </option>"
        + "<option value='EF'> EF </option>"
        + "</select>"
        +"</div>";

    E(T+'_input_field').innerHTML = s;
   
    local_fill_sbm_input_field();
}

function _local_create_dbm_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='dbm_table_old_seq' value='"+nth+"' />"
        + "<input id='dbm_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Name");%>' + "</div>"
        + "<input id='dbm_table_name' name='dbm_table_name' size='20' maxlength='20' "
        +           "onblur='valid_name(this, \"Rule Name\", 1)' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Rule Enable");%>' + "</div>"
        + "<input id='dbm_table_enable' name='dbm_table_enable' type='checkbox' value='1' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Internal IP Range");%>' + "</div>"
        + '<%lang("From");%>' + ":"
        + "<input id='dbm_table_inipaddr_start' name='dbm_table_inipaddr_start' size='17' maxlength='15' "
        +           "onblur='valid_ipaddr(this, \"Internal IP Range Start\")'/>"
        + '<%lang("To");%>' + ":"
        + "<input id='dbm_table_inipaddr_end' name='dbm_table_inipaddr_end' size='17' maxlength='15' "
        +           "onblur='valid_ipaddr(this, \"Internal IP Range End\")' />"
        +"</div>";

    E(T+'_input_field').innerHTML = s;
   
    local_fill_dbm_input_field();
}

function local_fill_sbm_input_field() {
    E('sbm_table_name').value  = rule[enum_sbm._NAME];
    tbl_assign_enable('sbm_table_enable', rule[enum_sbm._ENABLE]);
    E('sbm_table_inipaddr_start').value = rule[enum_sbm._INIPADDR_START];
    E('sbm_table_proto').value = rule[enum_sbm._PROTOCOL];
    E('sbm_table_extport_start').value = rule[enum_sbm._PORT_START];
    E('sbm_table_extport_end').value = rule[enum_sbm._PORT_END];
    E('sbm_table_extif').value = rule[enum_sbm._EXTIF]; 
    E('sbm_table_bw_alloc').value = rule[enum_sbm._BW_ALLOC]; 
    if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
        E('sbm_table_bw_ratio').value = rule[enum_sbm._BW_RATIO];
        <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    } else if (rule[enum_sbm._BW_ALLOC] == 'group'){
        E('sbm_table_bw_group').value = rule[enum_sbm._GROUPID];
        E('sbm_table_bw_group_ratio').value =
            get_bw_group_rate_ratio(rule[enum_sbm._GROUPID]);
        <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
    } else {
        E('sbm_table_bw_upload').value = rule[enum_sbm._BW_UPLOAD];
    }
    if (E('sbm_table_proto').value == '') {
        E('sbm_table_extport_start').disabled = true;
        E('sbm_table_extport_end').disabled = true;
    }
    tbl_assign_enable('sbm_table_excess', rule[enum_sbm._EXCESS]);
    if (rule[enum_sbm._EXCESS] == '1') {
        if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
            E('sbm_table_excess_ratio').value = rule[enum_sbm._EXCESS_RATIO];
        } else {
            if (rule[enum_sbm._EXCESS_UPLOAD] != '') {
                E('sbm_table_excess_upload').value = rule[enum_sbm._EXCESS_UPLOAD];
            } else {
                for (i = 0; i < <%nv_get("wan_num");%>; i++) {
                    if (rule[enum_sbm._EXTIF] == 'wan'+i) {
                        var ul = parseInt(E('wan'+i+'_bw_ul').value) * (100 - parseInt(E('wan'+i+'_bw_resv').value)) / 100;
                        E('sbm_table_excess_upload').value = ul;
                    }
                }
            }
        }
    }

    E('sbm_table_dscpid').value = rule[enum_sbm._DSCPID]; 
    E('sbm_table_remark_dscpid').value = rule[enum_sbm._REMARKDSCPID]; 
    sbm_update_available_bw()
}

function local_fill_dbm_input_field() {
    E('dbm_table_name').value  = rule[enum_dbm._NAME];
    tbl_assign_enable('dbm_table_enable', rule[enum_dbm._ENABLE]);
    E('dbm_table_inipaddr_start').value = rule[enum_dbm._INIPADDR_START];
    E('dbm_table_inipaddr_end').value = rule[enum_dbm._INIPADDR_END];
}

function local_generate_rule(T) {
    if (T == 'sbm_table') {
        return _local_generate_sbm_rule();
        <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    } else if (T == 'bw_group_table') {
        return _local_generate_bw_group_rule();
        <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
        <%prod_comment_js_start("app_enable", "2", "2" , "dbm");%>
    } else {
        return _local_generate_dbm_rule();
        <%prod_comment_js_end("app_enable", "2", "2" , "dbm");%>
    }
}

function _local_generate_sbm_rule() {
    /* rule[] is existing. r[] is temporary used. Therefore, when we check the
     * original value of the input element, we need to use rule[] instead of
     * r[].
     */
    var r = Array();
    r[enum_sbm._NAME] = E('sbm_table_name').value;
    r[enum_sbm._ENABLE] = tbl_dewrap_enable('sbm_table_enable');
    r[enum_sbm._INIPADDR_START] = E('sbm_table_inipaddr_start').value;
    r[enum_sbm._INIPADDR_END] = E('sbm_table_inipaddr_start').value;
/*    r[enum_sbm._INIPADDR_END] = E('sbm_table_inipaddr_end').value; */
    r[enum_sbm._PROTOCOL] = E('sbm_table_proto').value;
    r[enum_sbm._PORT_START] = E('sbm_table_extport_start').value;
    r[enum_sbm._PORT_END] = E('sbm_table_extport_end').value;
    r[enum_sbm._EXTIF] = E('sbm_table_extif').value;
    r[enum_sbm._DSCPID] = E('sbm_table_dscpid').value;
    r[enum_sbm._REMARKDSCPID] = E('sbm_table_remark_dscpid').value;
    r[enum_sbm._BW_ALLOC] = E('sbm_table_bw_alloc').value;
    if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
        r[enum_sbm._BW_RATIO] = E('sbm_table_bw_ratio').value;
        r[enum_sbm._BW_UPLOAD] = rule[enum_sbm._BW_UPLOAD];
        r[enum_sbm._BW_DOWNLOAD] = rule[enum_sbm._BW_DOWNLOAD];
        r[enum_sbm._GROUPID] = rule[enum_sbm._GROUPID];
    <%prod_comment_js_start("app_enable", "2", "2" , "gbm");%>
    } else if (rule[enum_sbm._BW_ALLOC] == 'group') {
        r[enum_sbm._GROUPID] = E('sbm_table_bw_group').value;
        r[enum_sbm._BW_UPLOAD] = rule[enum_sbm._BW_UPLOAD];
        r[enum_sbm._BW_DOWNLOAD] = rule[enum_sbm._BW_DOWNLOAD];
        r[enum_sbm._BW_RATIO] = rule[enum_sbm._BW_RATIO];
    <%prod_comment_js_end("app_enable", "2", "2" , "gbm");%>
    } else {
        r[enum_sbm._BW_RATIO] = rule[enum_sbm._BW_RATIO];
        r[enum_sbm._GROUPID] = rule[enum_sbm._GROUPID];
        r[enum_sbm._BW_UPLOAD] = E('sbm_table_bw_upload').value;
    }
    r[enum_sbm._EXCESS] = tbl_dewrap_enable('sbm_table_excess');

    r[enum_sbm._EXCESS_RATIO] = rule[enum_sbm._EXCESS_RATIO];
    r[enum_sbm._EXCESS_UPLOAD] = rule[enum_sbm._EXCESS_UPLOAD];
    r[enum_sbm._EXCESS_DOWNLOAD] = rule[enum_sbm._EXCESS_DOWNLOAD];
    if (rule[enum_sbm._EXCESS] == '1') {
        if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
            r[enum_sbm._EXCESS_RATIO] = E('sbm_table_excess_ratio').value;
        } else {
            r[enum_sbm._EXCESS_UPLOAD] = E('sbm_table_excess_upload').value;
        }
    }
    return r;
}

function _local_generate_dbm_rule() {
    var r = Array();
    r[enum_dbm._NAME] = E('dbm_table_name').value;
    r[enum_dbm._ENABLE] = tbl_dewrap_enable('dbm_table_enable');
    r[enum_dbm._INIPADDR_START] = E('dbm_table_inipaddr_start').value;
    r[enum_dbm._INIPADDR_END] = E('dbm_table_inipaddr_end').value;

    return r;
}

function _local_create_bw_group_input_field(T, nth) {
    var s = '';

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Sequence Number");%>' + "</div>"
        + "<input type='hidden' id='bw_group_table_old_seq' value='"+nth+"' />"
        + "<input id='bw_group_table_seq' size='4' maxlength='4' value='"+nth+"' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Group Name");%>' + "</div>"
        + "<input id='bw_group_name' name='bw_group_name' size='20' maxlength='20' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Upload rate(\%)");%>' + "</div>"
        + "<input id='bw_group_uprate' name='bw_group_uprate' size='20' maxlength='2' />"
        +"</div>";

    s += "<div class='setting'>"
        + "<div class='ilabel'>"+'<%lang("Upload ceil(\%)");%>' + "</div>"
        + "<input id='bw_group_upceil' name='bw_group_upceil' size='20' maxlength='2' />"
        +"</div>";

    E(T+'_input_field').innerHTML = s;

    bw_group_fill_input_field();
}

function bw_group_fill_input_field() {
    E('bw_group_name').value   = rule[enum_bw_group._NAME];
    E('bw_group_uprate').value = rule[enum_bw_group._UPRATE];
    E('bw_group_upceil').value = rule[enum_bw_group._UPCEIL];
}

function bw_group_local_create_table(T, R) {
    var c;

    c = "<tr>"
        + '<th><%lang("Group Name");%></th>'
        + '<th><%lang("Upload Rate");%></th>'
        + '<th><%lang("Upload Ceil");%></th>'
      + "</tr>";

    var cur_num = parseInt(E(T+"_cur_rule_num").value);
    for (var i = 0; i < cur_num; i++) {
        c += "<tr onclick='tbl_focus_tr(\""+T+"\", this);'>"
             + "<td>" + R[i][enum_bw_group._NAME] + "</td>"
             + "<td>" + R[i][enum_bw_group._UPRATE] + "</td>"
             + "<td>" + R[i][enum_bw_group._UPCEIL] + "</td>"
           + "</tr>";
    }

    return c;
}
function local_pack_bw_group_key_value_pair(F) {
    tbl_append_element(F, 'bw_group_rule', '');

    var row_num = parseInt(E('bw_group_table_cur_rule_num').value);

    tbl_append_element(F, "bw_group_rule_num", row_num);
    for (var i = 0; i < row_num; i++) {
        tbl_append_element(F, "bw_group_rule" + i + '_name', bw_group_rule[i][enum_bw_group._NAME]);
        tbl_append_element(F, "bw_group_rule" + i + '_uprate', bw_group_rule[i][enum_bw_group._UPRATE]);
        tbl_append_element(F, "bw_group_rule" + i + '_upceil', bw_group_rule[i][enum_bw_group._UPCEIL]);
    }
}

function _local_generate_bw_group_rule(T) {
    var rule_tmp = Array();

    rule_tmp[enum_bw_group._NAME]      = E('bw_group_name').value;
    rule_tmp[enum_bw_group._UPRATE]      = E('bw_group_uprate').value;
    rule_tmp[enum_bw_group._UPCEIL]      = E('bw_group_upceil').value;

    return rule_tmp;
}
</script>
</head>
  <body class="gui" onload="init();"> <% showad(); %>
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Bandwidth", "iDBM"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("BWM");%>', '<%lang("QoS");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements("bw_rule")</script>
	          <script type="text/javascript">do_sbm_used_bw()</script>
              <% tbl_get("bw_group_rule"); %>
              <% tbl_get("bw_sbm_rule"); %>
              <% tbl_get("bw_dbm_rule"); %>
              <fieldset>
                <legend><%lang("Bandwidth Management");%></legend>
                <div class="setting">
                  <div class="label"><% lang("QoS"); %></div>
                  <input type="radio" value="1" name="bw_enable" onclick="init()" <%nvg_match("bw_enable","1","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="bw_enable" onclick="init()" <%nvg_match("bw_enable","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />

              <% bw_show_setting(); %>
              <fieldset>
                <legend><%lang("Bandwidth Management Group");%></legend>
                <%tbl_create_table("bw_group_table", "bw_group_rule", "0");%>
                <script type="text/javascript">
                  tbl_setup_table("bw_group_table", bw_group_rule);
                </script>
              </fieldset><br />

              <fieldset>
                <legend><%lang("Static Bandwidth Management");%> (SBM)</legend>
                <%tbl_create_table("sbm_table", "bw_sbm_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("sbm_table", bw_sbm_rule);
                </script>
              </fieldset><br />
              <%prod_comment_html_start("app_enable", "2", "2" , "dbm");%>
              <fieldset>
                <legend><%lang("Dynamic Bandwidth Management");%></legend>

                <div style='font-size:12px;margin-left:10px'>
                    <%lang("The rest bandwidth from setting SBM would be totally used for DBM.");%>
                </div>

                <br/>

                <script type="text/javascript">do_dbm_available_bw();</script>
                <%tbl_create_table("dbm_table", "bw_dbm_rule", "1");%>
                <script type="text/javascript">
                  tbl_setup_table("dbm_table", bw_dbm_rule);
                </script>
              </fieldset><br />
              <%prod_comment_html_end("app_enable", "2", "2" , "dbm");%>

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()"/>
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
    <script type="text/javascript">create_input_window("sbm_table", "bw_sbm_rule", 1)</script>
    <script type="text/javascript">create_input_window("dbm_table", "bw_dbm_rule", 1)</script>
    <script type="text/javascript">create_input_window("bw_group_table", "bw_group_rule", 1)</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
