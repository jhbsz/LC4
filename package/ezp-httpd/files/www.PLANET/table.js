function tbl_wrap_wan(v) {
    if (v == '') {
        return '*';
    }
    var iface = 'wan';
    var idx = v.indexOf(iface);
    if (idx >= 0) {
        return iface.toUpperCase() + (parseInt(v.substring(iface.length)) + 1);
    }
    iface = 'lan';
    idx = v.indexOf(iface);
    if (idx >= 0) {
        return iface.toUpperCase() + (parseInt(v.substring(iface.length)) + 1);
    }
    return v;
}

function tbl_wrap_proto(v) {
    if (v == "both") {
        return "TCP/UDP";
    } else if (v == '') {
        return '*';
    } else {
        return v.toUpperCase();
    }
}


function tbl_dewrap_proto(id) {
    var v = E(id).value;
    if (v == "TCP/UDP") {
        return "both";
    } else if (v == "*") {
        return '';
    } else {
        return v.toLowerCase();
    }
}

function tbl_wrap(v) {
    return v.toUpperCase();
}

function tbl_dewrap(id) {
    var v = E(id).value;
    return v.toLowerCase();
}

function tbl_dewrap_radio(id) {
    var obj = document.getElementsByName(id);
    for (var i = 0; i < obj.length; i++) {
        if (obj[i].checked == true) {
            return obj[i].value.toLowerCase();
        }
    }
    return '';
}

function tbl_wrap_enable(v) {
    if (v == 1) {
        return "<img src='v_021.gif'>";
    } else {
        return "<img src='x_011.gif'>";
    }
}

function tbl_wrap_connection(v) {
    if (v == "1" || v == "Established") {
        return "<img src='green.gif'>";
    } else {
        return "<img src='red.gif'>";
    }
}

function tbl_dewrap_enable(id) {
    return (E(id).checked == true) ? 1 : 0;
}

function tbl_wrap_action(v) {
    return (v == 1) ? '<%lang("ALLOW");%>' : '<%lang("DENY");%>';
}

function tbl_dewrap_action(id) {
    return (E(id).value == '<%lang("ALLOW");%>') ? 1 : 0;
}

function tbl_assign_enable(id, v) {
    if (v == "1") {
        E(id).checked = true;
    } else {
        E(id).checked = false;
    }
}
        
function tbl_assign_radio(id, v, item) {
    var obj = document.getElementsByName(id);
    for (var i = 0; i < obj.length; i++) {
        if (v == item[i]) {
            obj[i].checked = true;
        } else {
            obj[i].checked = false;
        }
    }
}
        
function tbl_show_input(T, flag, R) {
    E(T+'_rule_flag').value = flag;
    if (E(T+'_rule_flag').value == '1') {
        if (parseInt(E(T+'_cur_rule_num').value) >= parseInt(E(T+"_max_rule_num").value)) {
            alert("The max number of the rules is "+E(T+"_max_rule_num").value+"!");
            return;
        }

        local_create_input_field(T, parseInt(E(T+'_cur_rule_num').value)+1);
    } else {
        local_create_input_field(T, parseInt(tbl_get_tr_index())+1, R[tbl_get_tr_index()]);
    }

    tbl_show_pop_window(T);
}

function tbl_cancel_input(T) {
    tbl_hide_pop_window(T);
}

function tbl_add_rule(T, R) {
    if (window.local_rule_check) {
        if (!local_rule_check(T)) {
            return;
        }
    }

    s = local_generate_rule(T);
    tbl_insert_tr(T, R, s, parseInt(E(T+'_old_seq').value) - 1, parseInt(E(T+'_seq').value) - 1);
    tbl_cancel_input(T);
}

function tbl_clear_focus(T, tbd) {
    document.currentTr = 0;
    var len = tbd.childNodes.length;
    for (var idx = 0; idx < len; idx++){
        tbd.childNodes[idx].style.background = "";
    }

    tbl_disable_row_edit(T);
}

function tbl_enable_row_edit(T) {
    E(T+"_delete").disabled = false;
    E(T+"_modify").disabled = false;
    if (E(T+"_enable_up_down").value == "true") {
        E(T+"_up").disabled = false;
        E(T+"_down").disabled = false;
    }
}

function tbl_disable_row_edit(T) {
    E(T+"_add").disabled = false;
    E(T+"_delete").disabled = true;
    E(T+"_modify").disabled = true;

    if (E(T+"_enable_up_down").value == "true") {
        E(T+"_up").disabled = true;
        E(T+"_down").disabled = true;
    }
}

function tbl_get_tr_index() {
    var tr = document.currentTr;
    if (!tr) {
        return 0;
    }

    var tbd = tr.parentNode;
    var idx;
    var len = tbd.childNodes.length;
    for (idx = 1; idx < len; idx++){
       if (tbd.childNodes[idx] == tr) {
           break;
       }
    }
    return idx - 1;
}

function tbl_focus_tr(T, TR) {
    if (document.currentTr == undefined) {
        document.currentTr = 0;   
    }
    
    var tbd = TR.parentNode;
    var table=tbd.parentNode;
    
    if (!table.disabled) {
        tbl_clear_focus(T, tbd);
    }
    
    document.currentTr = TR;
    TR.style.background = "#ccc";
    
    if (!table.disabled) {
    	tbl_enable_row_edit(T);
    }
}

function tbl_insert_tr(T, R, arr, from, to) {
    if (E(T+'_rule_flag').value == '1') {
        if (to < 0 || to > parseInt(E(T+'_cur_rule_num').value)) {
            alert("Inserted position out of boundary");
            return;
        }
    } else {
        if (to < 0 || to >= parseInt(E(T+'_cur_rule_num').value)) {
            alert("Inserted position out of boundary");
            return;
        }
    }

    if (E(T+'_rule_flag').value == '1') {
        var i = parseInt(E(T+'_cur_rule_num').value) + 1;
        E(T+'_cur_rule_num').value = i;
    }
    R.move(from, to, arr);
    tbl_setup_table(T, R);
}

function tbl_del_tr(T, R) {
    var idx = tbl_get_tr_index();
    var i = parseInt(E(T+'_cur_rule_num').value) - 1;
    E(T+'_cur_rule_num').value = i;
    R.splice(idx, 1);
    tbl_setup_table(T, R); 
}

function tbl_moveup_tr(T, R) {
    var idx = tbl_get_tr_index();
    R.swap(idx, idx - 1);
    tbl_setup_table(T, R); 
}

function tbl_movedown_tr(T, R) {
    var idx = tbl_get_tr_index();
    if (idx < parseInt(E(T+'_cur_rule_num').value) - 1) {
        R.swap(idx , idx+1);
    }
    tbl_setup_table(T, R); 
}

function tbl_append_element(F,N,V) {
    var e = document.createElement("input");
    e.type = "hidden";
    e.name = N;
    e.value = V;
    F.appendChild(e);
}

function tbl_setup_table(T, R) {
    var c = "<table id='"+T+"_element'>";

    c += local_create_table(T, R);
    E(T).innerHTML = c;

    tbl_disable_row_edit(T);
}

function tbl_create_input_field(T) {
    var s = "<fieldset id='"+T+"_input_field'>";
    s += "</fieldset>";
    W(s);
}

function tbl_hide_pop_window(T) {
    E('background-cover').style.display = 'none';
    E(T+'-input-window').style.display = 'none';
}

function tbl_show_pop_window(T) {
    E('background-cover').style.display = 'block';
    E('background-cover').style.width = get_page_width() + 'px';
    E('background-cover').style.height = get_page_height() + 'px';
    E(T+'-input-window').style.display = 'block';
}

function tbl_check_valid_port_range(v1, v2, M) {
    if (!v1 || !v2) {
        return true;
    }

    if (parseInt(v1) > parseInt(v2)) {
        alert(M + ': ' + v1 + ' and ' + v2 + ' not a valid range');
        return false;
    }
    return true; 
}
function tbl_check_valid_ipaddr_range(v1, v2, M) {
    if (!v1 || !v2) {
        return true;
    }

    var arr1 = v1.split('.');
    var arr2 = v2.split('.');

    var i;
    for (i = 0; i < 4; i++) {
        if (parseInt(arr1[i]) > parseInt(arr2[i])) {
            alert(M + ': ' + v1 + ' and ' + v2 + ' not a valid range');
            return false;
        }
    }
    return true; 
}

function tbl_input_choose(T, obj, index) {
    rule = local_generate_rule(T); 
    _local_create_input_field(T, E(T+'_seq').value);
}

// Function to combine 2 rules into one so that a table's content is not
// bounded with single rule
// For example : we can combine a setting rule with a status rule to display
// both static settings and run time status
function tbl_add_additional_rule_element(R1, idxR1_s,idxR1_e, R2, idxR2_s, idxR2_e) {
    var array_len = R1.length;
    /* If the element range of both array is not match */
    if ((idxR1_e < idxR1_s) || (idxR2_e < idxR2_s) || 
        ((idxR1_e - idxR1_s) != (idxR2_e - idxR2_s)) || 
        R1.length != R2.length) {
        return;
    }
    for (var idx = 0; idx < array_len; idx++) {
        for (var idx2 = 0; idx2 <= (idxR1_e - idxR1_s); idx2++) {
            R1[idx][idxR1_s + idx2] = R2[idx][idxR2_s + idx2];
        }
    }
}       
