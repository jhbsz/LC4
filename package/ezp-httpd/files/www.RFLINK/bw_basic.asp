<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Bandwidth</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="./table.js"></script>
<script type="text/javascript">

var bw_objects = new Array(<%nv_get("wan_num");%>);
var wan_bw_objects = new Array(<%nv_get("wan_num");%>);
var F;

function SelBWMode(F) {
    F.submit_button.value = "bw_basic";
    F.action.value = "Gozila";
    F.submit();
}

function bw_enable_disable(dl,up,type) {
    if( type == "Custom") {
        choose_enable(F.elements[dl]);
        choose_enable(F.elements[ul]);
    } else{
        choose_disable(F.elements[dl]);
        choose_disable(F.elements[ul]);
    }
}

/*
function bw_update_total_available_bw(nth) {
    var dl = parseInt(E('wan'+nth+'_bw_dl').value) * (100 - parseInt(E('wan'+nth+'_bw_resv').value)) / 100;
    var ul = parseInt(E('wan'+nth+'_bw_ul').value) * (100 - parseInt(E('wan'+nth+'_bw_resv').value)) / 100;
    E('wan'+nth+'_totall_available_bw').innerHTML = dl.toFixed(1) + '/' + ul.toFixed(1) + ' Kbps';
}
*/

function sbm_update_used_bw(flag) {
    /* Initiate */
    for (j = 0; j < <%nv_get("wan_num");%>; j++) {
        E('wan'+j+'_used_bw_dl').value = '0';
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

        var dl;
        var ul;
        for (j = 0; j < <%nv_get("wan_num");%>; j++) {
            if (bw_sbm_rule[i][enum_sbm._EXTIF] == 'wan'+j) {
                if (bw_sbm_rule[i][enum_sbm._BW_ALLOC] == 'ratio') {
                    dl = parseInt(E('wan'+j+'_bw_dl').value);
                    ul = parseInt(E('wan'+j+'_bw_ul').value);
                    var ratio = bw_sbm_rule[i][enum_sbm._BW_RATIO];
                    ratio = (ratio == '') ? 0 : ratio;
                    E('wan'+j+'_used_bw_ul').value = parseInt(E('wan'+j+'_used_bw_ul').value) + ul * parseInt(ratio) / 100;
                    E('wan'+j+'_used_bw_dl').value = parseInt(E('wan'+j+'_used_bw_dl').value) + dl * parseInt(ratio) / 100;
                } else {
                    var used_ul = bw_sbm_rule[i][enum_sbm._BW_UPLOAD];
                    var used_dl = bw_sbm_rule[i][enum_sbm._BW_DOWNLOAD];
                    E('wan'+j+'_used_bw_ul').value = parseInt(E('wan'+j+'_used_bw_ul').value) + parseInt(used_ul);
                    E('wan'+j+'_used_bw_dl').value = parseInt(E('wan'+j+'_used_bw_dl').value) + parseInt(used_dl);
                }
            }
        }
    }
}

function dbm_update_available_bw() {
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var dl = parseInt(E('wan'+i+'_bw_dl').value);
        var ul = parseInt(E('wan'+i+'_bw_ul').value);
        /* TODO: dirty hack. Don't know why E('wan'+i+'_available_bw') would be
         * undefined at loading the page.
         */

        if (E('wan'+i+'_available_bw') != undefined) {
            dl = dl - parseInt(E('wan'+i+'_used_bw_dl').value);
            ul = ul - parseInt(E('wan'+i+'_used_bw_ul').value);
            if (dl < 0 || ul < 0) {
                alert('<%lang("WAN");%> '+'<%increase("i", "1");%>: <%lang("insufficient bandwidth");%>');
            }

            E('wan'+i+'_available_bw').innerHTML = dl.toFixed(1) + '/' + ul.toFixed(1) + ' Kbps';
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
        var dl = parseInt(E('wan'+i+'_bw_dl').value);
        var ul = parseInt(E('wan'+i+'_bw_ul').value);
        var a = (parseInt(dl) - parseInt(E('wan'+i+'_used_bw_dl').value));
        var b = (parseInt(ul) - parseInt(E('wan'+i+'_used_bw_ul').value));
        $('#wan'+i+'_bw').html( a.toFixed(1) + '/' + b.toFixed(1) + ' Kbps');
    }

    if (rule[enum_sbm._ENABLE] == '0') {
        return;
    }
    bw_update_available_bw('1');

    if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var dl = parseInt(E('wan'+i+'_bw_dl').value);
            var ul = parseInt(E('wan'+i+'_bw_ul').value);
            if (rule[enum_sbm._EXTIF] == 'wan' + i) {
                var ratio = E('sbm_table_bw_ratio').value;
                ratio = (ratio == '') ? 0 : ratio;
                dl = dl * (100 - parseInt(ratio)) / 100;
                ul = ul * (100 - parseInt(ratio)) / 100;
                dl = dl - parseInt(E('wan'+i+'_used_bw_dl').value);
                ul = ul - parseInt(E('wan'+i+'_used_bw_ul').value);
                if (dl < 0 || ul < 0) {
                    alert('<%lang("WAN");%> '+'<%increase("i", "1");%>: <%lang("insufficient bandwidth");%>');
                }
                $('#wan'+i+'_bw').html( dl.toFixed(1) + '/' + ul.toFixed(1) + ' Kbps');
              <%prod_comment_js_start("PROD_CAT_T", "2", "2" , "PROD_SUBSUBCAT_L");%>
                $('#wan'+i+'_available_bw').html( dl.toFixed(1) + '/' + ul.toFixed(1) + ' Kbps');
              <%prod_comment_js_end("PROD_CAT_T", "2", "2" , "PROD_SUBSUBCAT_L");%>
            }
        }
            
    } else {
        for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
            var dl = parseInt(E('wan'+i+'_bw_dl').value);
            var ul = parseInt(E('wan'+i+'_bw_ul').value);
            var a = (parseInt(dl) - parseInt(E('wan'+i+'_used_bw_dl').value));
            var b = (parseInt(ul) - parseInt(E('wan'+i+'_used_bw_ul').value));
            $('#wan'+i+'_bw').html( a.toFixed(1) + '/' + b.toFixed(1) + ' Kbps');
            if (rule[enum_sbm._EXTIF] == 'wan' + i) {
                var used_dl = E('sbm_table_bw_download').value;
                var used_ul = E('sbm_table_bw_upload').value;
                used_dl = (used_dl == '') ? 0 : used_dl;
                used_ul = (used_ul == '') ? 0 : used_ul;
                dl = dl - parseInt(used_dl) - parseInt(E('wan'+i+'_used_bw_dl').value);
                ul = ul - parseInt(used_ul) - parseInt(E('wan'+i+'_used_bw_ul').value);
                if (dl < 0 || ul < 0) {
                    alert('<%lang("WAN");%> '+'<%increase("i", "1");%>: <%lang("insufficient bandwidth");%>');
                }
                $('#wan'+i+'_bw').html (dl.toFixed(1) + '/' + ul.toFixed(1) + ' Kbps');
                $('#wan'+i+'_available_bw').html( dl.toFixed(1) + '/' + ul.toFixed(1) + ' Kbps');
            }
        }
    }
}

function local_rule_check(T) {
    if (T == 'sbm_table') {
        if (_local_sbm_rule_check() == false) {
            return false;
        }
    } else {
        if (_local_dbm_rule_check() == false) {
            return false;
        }
    }
    return true;
}
function _local_dbm_rule_check(T) {
    if ( !$('#dbm_table_name').val()) {
        alert('Rule Name is needed.');
        return false;
    }

    if ( $('#dbm_table_inipaddr_start').val() && !$('#dbm_table_inipaddr_end').val()) {
        alert('DBM Internal IP End is needed.');
        return false;
    }

    if (!$('#dbm_table_inipaddr_start').val() && $('#dbm_table_inipaddr_end').val()) {
        alert('DBM Internal IP Start is needed.');
        return false;
    }

    if (!tbl_check_valid_ipaddr_range($('#dbm_table_inipaddr_start').val(), 
                                      $('#dbm_table_inipaddr_end').val(), 'DBM Internal IP')) {
        return false;
    }

    return true;
}

function _local_sbm_rule_check(T) {
    if (!$('#sbm_table_inipaddr_start').val() && $('#sbm_table_proto').val() == '') {
        alert('<%lang("SBM internal IP or protocol/port");%>'+': '+'<%lang("not defined");%>');
        return false;
    }
    
    if ($('#sbm_table_proto').val() != '') {
        if ($('#sbm_table_extport_start').val() == '' || $('#sbm_table_extport_end').val() == '') {
            alert('<%lang("SBM port");%>'+': '+'<%lang("not defined");%>');
            return false;
        }
        
        if (!tbl_check_valid_port_range($('#sbm_table_extport_start').val(), 
                                        $('#sbm_table_extport_end').val(), 'SBM Port')) {
            return false;
        }
    }

    if ($('#sbm_table_bw_alloc').val() == 'ratio') {
        if ($('#sbm_table_bw_ratio').val() == '') {
            alert('<%lang("SBM bandwidth ratio");%>'+': '+'<%lang("not defined");%>');
            return false;
        }

        if (valid_number($('#sbm_table_bw_ratio').val(), 'SBM bandwidth ratio') == false) {
            return false;
        }
    } else if ($('#sbm_table_bw_alloc').val() == 'bw') {
        if ($('#sbm_table_bw_upload').val() == '') {
            alert('<%lang("SBM bandwidth upload");%>'+': '+'<%lang("not defined");%>');
            return false;
        }
        if (valid_number($('#sbm_table_bw_upload').val(), 'Upload') == false) {
            return false;
        }
        if ($('#sbm_table_bw_download').val() == '') {
            alert('<%lang("SBM bandwidth download");%>'+': '+'<%lang("not defined");%>');
            return false;
        }
        if (valid_number($('#sbm_table_bw_download').val(), 'Download') == false) {
            return false;
        }

        if (parseInt($('#sbm_table_bw_download').val()) < 8) {
            alert('<%lang("SBM bandwidth download");%>'+': '+'<%lang("should be larger than");%>' + ' 8kpbs');
            return false;
        }
    }
    
    var dl;
    var ul;
    rule = local_generate_rule('sbm_table');
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        var total_dl = parseInt($('#wan'+i+'_bw_dl').val());
        var total_ul = parseInt($('#wan'+i+'_bw_ul').val());
        if (rule[enum_sbm._EXTIF] == 'wan'+i) {
            if (rule[enum_sbm._BW_ALLOC] == 'ratio') {
                var ratio = rule[enum_sbm._BW_RATIO];
                dl = parseInt(total_dl) * (100 - parseInt(ratio)) / 100;
                ul = parseInt(total_ul) * (100 - parseInt(ratio)) / 100;
            } else {
                ul = parseInt(total_dl) - parseInt(rule[enum_sbm._BW_UPLOAD]);
                dl = parseInt(total_dl) - parseInt(rule[enum_sbm._BW_DOWNLOAD]);
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
                    if (parseInt(rule[enum_sbm._EXCESS_DOWNLOAD]) > parseInt(total_dl)) {
                        var M = '<%lang("Use Maximal Download");%>' + ': '+ '<%lang("should be less than");%>' 
                                            + total_dl + '<%lang(".");%>';
                        alert(M);
                        return false;
                    }
                    if (parseInt(rule[enum_sbm._EXCESS_DOWNLOAD]) < parseInt(rule[enum_sbm._BW_DOWNLOAD])) {
                        var M = '<%lang("Use Maximal Download");%>' + ': '+ '<%lang("should be larger than");%>' 
                                            + rule[enum_sbm._BW_DOWNLOAD] + '<%lang(".");%>';
                        alert(M);
                        return false;
                    }

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
        dl = dl - parseInt($('#wan'+i+'_used_bw_dl').val());
        ul = ul - parseInt($('#wan'+i+'_used_bw_ul').val());
        if (dl < 0 || ul < 0) {
            alert('<%lang("WAN");%> '+'<%increase("i", "1");%>: <%lang("insufficient bandwidth");%>');
            return false;
        }
    }
    return true;
}

function local_pack_sbm_key_value_pair(F, T, P) {
    /* Important */
    var row_num = bw_sbm_rule.length ;
    $("<input>").attr({type: "hidden", id: "bw_sbm_rule_num", name: "bw_sbm_rule_num", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name', value: bw_sbm_rule[i][enum_sbm._NAME]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: bw_sbm_rule[i][enum_sbm._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_start', name: P + i + '_inipaddr_start', value: bw_sbm_rule[i][enum_sbm._INIPADDR_START]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_end', name: P + i + '_inipaddr_end', value: bw_sbm_rule[i][enum_sbm._INIPADDR_END]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_proto', name: P + i + '_proto', value: bw_sbm_rule[i][enum_sbm._PROTOCOL]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_extport_start', name: P + i + '_extport_start', value: bw_sbm_rule[i][enum_sbm._PORT_START]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_extport_end', name: P + i + '_extport_end', value: bw_sbm_rule[i][enum_sbm._PORT_END]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_extif', name: P + i + '_extif', value: bw_sbm_rule[i][enum_sbm._EXTIF]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_bw_alloc', name: P + i + '_bw_alloc', value: bw_sbm_rule[i][enum_sbm._BW_ALLOC]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_bw_ratio', name: P + i + '_bw_ratio', value: bw_sbm_rule[i][enum_sbm._BW_RATIO]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_bw_upload', name: P + i + '_bw_upload', value: bw_sbm_rule[i][enum_sbm._BW_UPLOAD]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_bw_download', name: P + i + '_bw_download', value: bw_sbm_rule[i][enum_sbm._BW_DOWNLOAD]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_excess', name: P + i + '_excess', value: bw_sbm_rule[i][enum_sbm._EXCESS]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_excess_ratio', name: P + i + '_excess_ratio', value: bw_sbm_rule[i][enum_sbm._EXCESS_RATIO]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_excess_upload', name: P + i + '_excess_upload', value: bw_sbm_rule[i][enum_sbm._EXCESS_UPLOAD]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_excess_download', name: P + i + '_excess_download', value: bw_sbm_rule[i][enum_sbm._EXCESS_DOWNLOAD]}).appendTo("form");
    
    }
}

function local_pack_dbm_key_value_pair(F, T, P) {
    /* Important */
    var row_num = bw_dbm_rule.length ;
    $("<input>").attr({type: "hidden", id: "bw_dbm_rule", name: "bw_dbm_rule", value:""}).appendTo("form");
    $("<input>").attr({type: "hidden", id: P+"_num", name: P+"_num", value: row_num}).appendTo("form");
    for (var i = 0; i < row_num; i++) {
        $("<input>").attr({type: "hidden", id: P + i + '_name', name: P + i + '_name' , value: bw_dbm_rule[i][enum_dbm._NAME] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_enable', name: P + i + '_enable', value: bw_dbm_rule[i][enum_dbm._ENABLE]}).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_start', name: P + i + '_inipaddr_start' , value: bw_dbm_rule[i][enum_dbm._INIPADDR_START] }).appendTo("form");
        $("<input>").attr({type: "hidden", id: P + i + '_inipaddr_end', name: P + i + '_inipaddr_end' , value: bw_dbm_rule[i][enum_dbm._INIPADDR_END] }).appendTo("form");
    }

}

function to_submit(F) {
    F.submit_button.value = "bw_basic";
	F.action.value = "Apply";
    local_pack_sbm_key_value_pair(F, "sbm_table", "bw_sbm_rule");
    <%prod_comment_js_start("PROD_CAT_T", "2", "2" , "PROD_SUBSUBCAT_L");%>
    local_pack_dbm_key_value_pair(F,"dbm_table", "bw_dbm_rule");
    <%prod_comment_js_end("PROD_CAT_T", "2", "2" , "PROD_SUBSUBCAT_L");%>
    ajax_preaction(F);
    display_mask(F);
}
function init() {
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        
        for (var j=0; j<bw_objects[i].length; j++  ){
            if ($('[name=bw_enable]:checked').val()==1){
                $("#"+ bw_objects[i][j]["name"]).attr("disabled",false);
                if ($("select#wan"+i+"_bw_type option:selected").val() !="Custom"){
                    $("#wan"+i+"_bw_dl").attr("disabled",true);
                    $("#wan"+i+"_bw_ul").attr("disabled",true);
                } else {
                    $("#wan"+i+"_bw_dl").attr("disabled",false);
                    $("#wan"+i+"_bw_ul").attr("disabled",false);
                }

            } else {
                $("#"+ bw_objects[i][j]["name"]).attr("disabled",true);
            }
        }

    }//End for

    if ($('[name=bw_enable]:checked').val()==1){
        $("#SBM_AddItem").removeClass('ui-state-disabled');
        $("#SBM_DelItem").removeClass('ui-state-disabled');
        $("#SBM_ModifyItem").removeClass('ui-state-disabled');
        $("#DBM_AddItem").removeClass('ui-state-disabled');
        $("#DBM_DelItem").removeClass('ui-state-disabled');
        $("#DBM_ModifyItem").removeClass('ui-state-disabled');
    }else{
        $("#SBM_AddItem").addClass('ui-state-disabled');
        $("#SBM_DelItem").addClass('ui-state-disabled');
        $("#SBM_ModifyItem").addClass('ui-state-disabled');
        $("#DBM_AddItem").addClass('ui-state-disabled');
        $("#DBM_DelItem").addClass('ui-state-disabled');
        $("#DBM_ModifyItem").addClass('ui-state-disabled');
    }

    <%widget_start();%>

    sbm_update_used_bw('0');
}
function do_sbm_used_bw() {
    for (var i = 0; i < <%nv_get("wan_num");%>; i++) {
        W('<input type="hidden" id="wan'+i+'_used_bw_ul" name="wan'+i+'_used_bw_ul" value="0" />');
        W('<input type="hidden" id="wan'+i+'_used_bw_dl" name="wan'+i+'_used_bw_dl" value="0" />');
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
    if (element.value == '1M64K') {
        F.elements["wan"+i+"_bw_dl"].value = "1024";
        F.elements["wan"+i+"_bw_ul"].value = "64";
    } else if (element.value == '1.5M640K') {
        F.elements["wan"+i+"_bw_dl"].value = "1536";
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == '1M128K') {
        F.elements["wan"+i+"_bw_dl"].value = "1024";
        F.elements["wan"+i+"_bw_ul"].value = "128";
    } else if (element.value == '2M256K') {
        F.elements["wan"+i+"_bw_dl"].value = "2048";
        F.elements["wan"+i+"_bw_ul"].value = "256";
    } else if (element.value == '2M512K') {
        F.elements["wan"+i+"_bw_dl"].value = "2048";
        F.elements["wan"+i+"_bw_ul"].value = "512";
    } else if (element.value == '2M128K') {
        F.elements["wan"+i+"_bw_dl"].value = "2048";
        F.elements["wan"+i+"_bw_ul"].value = "128";
    } else if (element.value == '2M384K') {
        F.elements["wan"+i+"_bw_dl"].value = "2048";
        F.elements["wan"+i+"_bw_ul"].value = "384";
    } else if (element.value == '3M640K') {
        F.elements["wan"+i+"_bw_dl"].value = "3072";
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == '3M384K') {
        F.elements["wan"+i+"_bw_dl"].value = "3072";
        F.elements["wan"+i+"_bw_ul"].value = "384";
    } else if (element.value == '4M1M') {
        F.elements["wan"+i+"_bw_dl"].value = "4096";
        F.elements["wan"+i+"_bw_ul"].value = "1024";
    } else if (element.value == '6M256K') {
        F.elements["wan"+i+"_bw_dl"].value = "6144";
        F.elements["wan"+i+"_bw_ul"].value = "256";
    } else if (element.value == '6M640K') {
        F.elements["wan"+i+"_bw_dl"].value = "6144";
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == '8M256K') {
        F.elements["wan"+i+"_bw_dl"].value = "8192";
        F.elements["wan"+i+"_bw_ul"].value = "256";
    } else if (element.value == '8M640K') {
        F.elements["wan"+i+"_bw_dl"].value = "8192";
        F.elements["wan"+i+"_bw_ul"].value = "640";
    } else if (element.value == '8M800K') {
        F.elements["wan"+i+"_bw_dl"].value = "8192";
        F.elements["wan"+i+"_bw_ul"].value = "800";
    } else if (element.value == '10M1M') {
        F.elements["wan"+i+"_bw_dl"].value = "10240";
        F.elements["wan"+i+"_bw_ul"].value = "1024";
    } else if (element.value == '10M2M') {
        F.elements["wan"+i+"_bw_dl"].value = "10240";
        F.elements["wan"+i+"_bw_ul"].value = "2048";
    } else if (element.value == '12M1M') {
        F.elements["wan"+i+"_bw_dl"].value = "12288";
        F.elements["wan"+i+"_bw_ul"].value = "1024";
    } else if (element.value == '20M2M') {
        F.elements["wan"+i+"_bw_dl"].value = "20480";
        F.elements["wan"+i+"_bw_ul"].value = "2048";
    } else if (element.value == '50M5M') {
        F.elements["wan"+i+"_bw_dl"].value = "51200";
        F.elements["wan"+i+"_bw_ul"].value = "5120";
    } else if (element.value == '100M10M') {
        F.elements["wan"+i+"_bw_dl"].value = "102400";
        F.elements["wan"+i+"_bw_ul"].value = "10240";
    } else if (element.value == '2M2M') {
        F.elements["wan"+i+"_bw_dl"].value = "2048";
        F.elements["wan"+i+"_bw_ul"].value = "2048";
    } else if (element.value == '4M4M') {
        F.elements["wan"+i+"_bw_dl"].value = "4096";
        F.elements["wan"+i+"_bw_ul"].value = "4096";
    } else if (element.value == '10M10M') {
        F.elements["wan"+i+"_bw_dl"].value = "10240";
        F.elements["wan"+i+"_bw_ul"].value = "10240";
    } else if (element.value == '25M25M') {
        F.elements["wan"+i+"_bw_dl"].value = "25600";
        F.elements["wan"+i+"_bw_ul"].value = "25600";
    } else if (element.value == '50M50M') {
        F.elements["wan"+i+"_bw_dl"].value = "51200";
        F.elements["wan"+i+"_bw_ul"].value = "51200";
    } else if (element.value == '100M100M') {
        F.elements["wan"+i+"_bw_dl"].value = "102400";
        F.elements["wan"+i+"_bw_ul"].value = "102400";
    } else if (element.value == "Custom") {
        F.elements["wan"+i+"_bw_dl"].value = "";
        F.elements["wan"+i+"_bw_ul"].value = "";
    }
            
    sel_change(element.value, "Custom", F, objects);
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
    _EXCESS_DOWNLOAD: 17
}

var enum_dbm = {
    _NAME: 0,
    _ENABLE: 1,
    _INIPADDR_START: 2,
    _INIPADDR_END: 3,
    _INIPADDR_NUM: 4,
    _SCHED_IDX: 5 
}

var default_sbm_rule = ['','1','','','0','','','','wan0','ratio','','','','','0','100','',''];
var rule;

function local_generate_rule(T) {
    if (T == 'sbm_table') {
        return _local_generate_sbm_rule();
    } else {
        return _local_generate_dbm_rule();
    }
}

function _local_generate_sbm_rule() {

    var EnableVal = 0;
    if ($('input[name=sbm_table_enable]').is(':checked')){
        EnableVal = $('[name=sbm_table_enable]:checked').val();
    }

    var rule_tmp = Array();
    rule_tmp[enum_sbm._NAME]             = $('#sbm_table_name').val();
    rule_tmp[enum_sbm._ENABLE]           = EnableVal;
    rule_tmp[enum_sbm._INIPADDR_START]  = $('#sbm_table_inipaddr_start').val();
    rule_tmp[enum_sbm._INIPADDR_END]    = $('#sbm_table_inipaddr_start').val();
    //rule_tmp[enum_sbm._INIPADDR_END] = $('#sbm_table_inipaddr_end').val();

    rule_tmp[enum_sbm._PROTOCOL] = $("select#sbm_table_proto option:selected").val();
    rule_tmp[enum_sbm._PORT_START] = $('#sbm_table_extport_start').val();
    rule_tmp[enum_sbm._PORT_END] = $('#sbm_table_extport_end').val();
    rule_tmp[enum_sbm._EXTIF] = $('#sbm_table_extif').val();
    rule_tmp[enum_sbm._BW_ALLOC] = $('#sbm_table_bw_alloc').val();

    if (rule_tmp[enum_sbm._BW_ALLOC] == 'ratio') {
        rule_tmp[enum_sbm._BW_RATIO] = $('#sbm_table_bw_ratio').val();
        rule_tmp[enum_sbm._BW_UPLOAD] = default_sbm_rule[enum_sbm._BW_UPLOAD];
        rule_tmp[enum_sbm._BW_DOWNLOAD] = default_sbm_rule[enum_sbm._BW_DOWNLOAD];
    } else {
        rule_tmp[enum_sbm._BW_RATIO] = default_sbm_rule[enum_sbm._BW_RATIO];
        rule_tmp[enum_sbm._BW_UPLOAD] = $('#sbm_table_bw_upload').val();
        rule_tmp[enum_sbm._BW_DOWNLOAD] = $('#sbm_table_bw_download').val();
    }

    var ExcessEnableVal = 0;
    if ($('input[name=sbm_table_excess]').is(':checked')){
        ExcessEnableVal = $('[name=sbm_table_excess]:checked').val();
    }

    rule_tmp[enum_sbm._EXCESS] = ExcessEnableVal;

    rule_tmp[enum_sbm._EXCESS_RATIO] = default_sbm_rule[enum_sbm._EXCESS_RATIO];
    rule_tmp[enum_sbm._EXCESS_UPLOAD] = default_sbm_rule[enum_sbm._EXCESS_UPLOAD];
    rule_tmp[enum_sbm._EXCESS_DOWNLOAD] = default_sbm_rule[enum_sbm._EXCESS_DOWNLOAD];
    if (rule_tmp[enum_sbm._EXCESS] == '1') {
        if (rule_tmp[enum_sbm._BW_ALLOC] == 'ratio') {
            rule_tmp[enum_sbm._EXCESS_RATIO] = $('#sbm_table_excess_ratio').val();
        } else {
            rule_tmp[enum_sbm._EXCESS_DOWNLOAD] = $('#sbm_table_excess_download').val();
            rule_tmp[enum_sbm._EXCESS_UPLOAD] = $('#sbm_table_excess_upload').val();
        }
    }
    return rule_tmp;
}

function _local_generate_dbm_rule() {
    var rule_tmp = Array();

    var enable = 0;
    if ($('input[name=dbm_table_enable]').is(':checked')){
        enable = $('[name=dbm_table_enable]:checked').val();
    }

    rule_tmp[enum_dbm._NAME]     = $('#dbm_table_name').val();
    rule_tmp[enum_dbm._ENABLE]   = enable;
    rule_tmp[enum_dbm._INIPADDR_START] = $('#dbm_table_inipaddr_start').val();
    rule_tmp[enum_dbm._INIPADDR_END] = $('#dbm_table_inipaddr_end').val();

    return rule_tmp;
}

function GetSMBGridData(){
    var SBM_rows = [];
    var SBM_data = {};
    
    for (var i=0; i<bw_sbm_rule.length; i++ )  {
        SBM_data = {};
        
        SBM_data["sbm_table_name"] = bw_sbm_rule[i][enum_sbm._NAME];
        SBM_data["sbm_table_enable"] = tbl_wrap_enable( bw_sbm_rule[i][enum_sbm._ENABLE] );
        SBM_data["sbm_table_inipaddr_start"] = bw_sbm_rule[i][enum_sbm._INIPADDR_START];
        if (bw_sbm_rule[i][enum_sbm._PROTOCOL] == '') {
            SBM_data["sbm_table_app"] = "";
        } else {
            SBM_data["sbm_table_app"] = wrap_sbm_proto(bw_sbm_rule[i][enum_sbm._PROTOCOL]) + "/";
            if (bw_sbm_rule[i][enum_sbm._PORT_START] == bw_sbm_rule[i][enum_sbm._PORT_END]) {
                 SBM_data["sbm_table_app"] += bw_sbm_rule[i][enum_sbm._PORT_START];
            } else {
                 SBM_data["sbm_table_app"] += bw_sbm_rule[i][enum_sbm._PORT_START] + ':' + bw_sbm_rule[i][enum_sbm._PORT_END];
            }
        }//End If

        SBM_data["sbm_table_extif"] = tbl_wrap_wan(bw_sbm_rule[i][enum_sbm._EXTIF]);

        if (bw_sbm_rule[i][enum_sbm._BW_ALLOC] == 'ratio') {
            SBM_data["sbm_table_bw"] = bw_sbm_rule[i][enum_sbm._BW_RATIO] + "%";
        } else {
            SBM_data["sbm_table_bw"] = bw_sbm_rule[i][enum_sbm._BW_DOWNLOAD] + "Kbps / " + bw_sbm_rule[i][enum_sbm._BW_UPLOAD] + "Kbps";
        }
        SBM_rows.push(SBM_data);
    }
    return SBM_rows;
}

function CreateSBMDataForm(){

    $("select#sbm_table_proto").live("change", function() {
        if ($(this).val()=="") {
            $("#sbm_table_extport_start").attr("disabled",true);
            $("#sbm_table_extport_start").attr("value", "");
            $("#sbm_table_extport_end").attr("disabled",true);
            $("#sbm_table_extport_end").attr("value", "");
        }else{
            $("#sbm_table_extport_start").attr("disabled",false);
            $("#sbm_table_extport_end").attr("disabled",false);
        }
    });

    $("select#sbm_table_bw_alloc").live("change", function() {
        if ($(this).val()=="ratio") {
            $("#sbm_table_bw_ratio").attr("value","");
            $("#sbm_table_bw_ratio").parents("tr").show();
            $("#sbm_table_bw_download").attr("value","");
            $("#sbm_table_bw_download").parents("tr").hide();
            $("#sbm_table_bw_upload").attr("value","");
            $("#sbm_table_bw_upload").parents("tr").hide();

            if ($('input[name=sbm_table_excess]').is(':checked')){
                $("#sbm_table_excess_ratio").attr("value","100");
                $("#sbm_table_excess_ratio").parents("tr").show();
                $("#sbm_table_excess_download").attr("value","");
                $("#sbm_table_excess_download").parents("tr").hide();
                $("#sbm_table_excess_upload").attr("value","");
                $("#sbm_table_excess_upload").parents("tr").hide();
            }
        }else{
            $("#sbm_table_bw_ratio").attr("value","");
            $("#sbm_table_bw_ratio").parents("tr").hide();
            $("#sbm_table_bw_download").attr("value","");
            $("#sbm_table_bw_download").parents("tr").show();
            $("#sbm_table_bw_upload").attr("value","");
            $("#sbm_table_bw_upload").parents("tr").show();

            if ($('input[name=sbm_table_excess]').is(':checked')){
                $("#sbm_table_excess_ratio").attr("value","");
                $("#sbm_table_excess_ratio").parents("tr").hide();
                $("#sbm_table_excess_download").attr("value","");
                $("#sbm_table_excess_download").parents("tr").show();
                $("#sbm_table_excess_upload").attr("value","");
                $("#sbm_table_excess_upload").parents("tr").show();
            }
        }
    });

    $("#sbm_table_excess").live("click", function() {
        if ($('input[name=sbm_table_excess]').is(':checked')){
            if ($("select#sbm_table_bw_alloc option:selected").val() == "ratio"){
                $("#sbm_table_excess_ratio").attr("value","100");
                $("#sbm_table_excess_ratio").parents("tr").show();
                $("#sbm_table_excess_download").attr("value","");
                $("#sbm_table_excess_download").parents("tr").hide();
                $("#sbm_table_excess_upload").attr("value","");
                $("#sbm_table_excess_upload").parents("tr").hide();
            } else {
                $("#sbm_table_excess_ratio").attr("value","");
                $("#sbm_table_excess_ratio").parents("tr").hide();
                $("#sbm_table_excess_download").attr("value","");
                $("#sbm_table_excess_download").parents("tr").show();
                $("#sbm_table_excess_upload").attr("value","");
                $("#sbm_table_excess_upload").parents("tr").show();
            }
        }else{
            $("#sbm_table_excess_ratio").attr("value","");
            $("#sbm_table_excess_ratio").parents("tr").hide();
            $("#sbm_table_excess_download").attr("value","");
            $("#sbm_table_excess_download").parents("tr").hide();
            $("#sbm_table_excess_upload").attr("value","");
            $("#sbm_table_excess_upload").parents("tr").hide();
        }
    });

    $("#SBMDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input id='sbm_table_name' name='sbm_table_name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' /></td></tr>");
    $("#SBMDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='sbm_table_enable' name='sbm_table_enable' type='checkbox' value='1' onchange='sbm_update_available_bw()' /></td></tr>");
    $("#SBMDataForm").append("<tr><td align='right'><%lang("Internal IP Address");%>:</td><td><input id='sbm_table_inipaddr_start' name='sbm_table_inipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range Start\")'/></td></tr>");
    $("#SBMDataForm").append("<tr><td align='right'><%lang(Protocol);%>:</td><td>"+
                             "<select id='sbm_table_proto' name='sbm_table_proto' \">"+
                             "  <option value=''>*</option>" +
                             "  <option value='tcp'>TCP</option>" +
                             "  <option value='udp'>UDP</option>" +
                             "</select>" +
                             "</td></tr>");
    $("#SBMDataForm").append("<tr><td align='right'><%lang("Service Port Range");%>:</td><td>"+
                                 "<%lang("From");%>:<input id='sbm_table_extport_start' name='sbm_table_extport_start' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range Start\")' disabled='true' />"+
                                 "<%lang("To");%>:<input id='sbm_table_extport_end' name='sbm_table_extport_end' size='6' maxlength='5' onblur='valid_range(this, 1, 65535, \"Service Port Range End\")' disabled='true' />"+
                              "</td></tr>");

    if (<%nv_get("wan_num");%> == 1) {  //Only One Wan
        $("#SBMDataForm").append("<tr style='display: none'><td align='right'><%lang("External Interface");%>:</td><td> <input type='hidden' id='sbm_table_extif' name='sbm_table_extif' value='wan0' /></td></tr>");
    } else {
        $("#SBMDataForm").append("<tr><td align='right'><%lang("External Interface");%>:</td><td>"+
                                 "  <select id='sbm_table_extif' name='sbm_table_extif' onchange='sbm_update_available_bw()'/></select>"+
                                 "</td></tr>");
        for (var i = 1; i <= <%nv_get("wan_num");%>; i++) {
            $('#sbm_table_extif').append(new Option("WAN" + i , "wan" + (i-1), false, false));
        }
    }

    $("#SBMDataForm").append("<tr><td align='center' colspan='2'><%lang("Available Bandwidth");%></td></tr>");
    for (i = 0; i < <%nv_get("wan_num");%>; i++) {
        $("#SBMDataForm").append("<tr><td align='right'><div class='ilabel'>WAN" + (i+1) + ":</div></td><td><div id='wan" + i + "_bw'></div></td></tr>");
    }

    $("#SBMDataForm").append("<tr><td align='right'><%lang("Bandwidth Allocation");%>:</td><td>"+ 
                             "<select id='sbm_table_bw_alloc' name='sbm_table_bw_alloc' onchange='sbm_update_available_bw()' >"+
                             "  <option value='ratio'><%lang("By Ratio");%></option>"+  
                             "  <option value='bw'><%lang("By Bandwidth");%></option>"+ 
                             "</select>"+ 
                             "</td></tr>");
    $("#SBMDataForm").append("<tr><td align='right'><%lang("Ratio");%>:</td><td><input id='sbm_table_bw_ratio' name='sbm_table_bw_ratio' size='4' maxlength='4' onblur='sbm_update_available_bw()' /> %</td></tr>");
    $("#SBMDataForm").append("<tr style='display: none'><td align='right'><%lang("Download");%>:</td><td><input id='sbm_table_bw_download' name='sbm_table_bw_download' size='7' maxlength='6' onblur='sbm_update_available_bw()' /> Kbps</td></tr>");
    $("#SBMDataForm").append("<tr style='display: none'><td align='right'><%lang("Upload");%>:</td><td><input id='sbm_table_bw_upload' name='sbm_table_bw_upload' size='7' maxlength='6' onblur='sbm_update_available_bw()' /> Kbps</td></tr>");
    $("#SBMDataForm").append("<tr><td align='right'><%lang("Utilize Bandwidth More Than Guaranteed");%>:</td><td><input id='sbm_table_excess' name='sbm_table_excess' type='checkbox' value='1' onchange=''></td></tr>");
    $("#SBMDataForm").append("<tr style='display: none'><td align='right'><%lang("Use Maximal Ratio");%>:</td><td><input id='sbm_table_excess_ratio' name='sbm_table_excess_ratio' size='4' maxlength='4' onblur='sbm_update_available_bw()' /> %</td></tr>");
    $("#SBMDataForm").append("<tr style='display: none'><td align='right'><%lang("Use Maximal Download");%>:</td><td><input id='sbm_table_excess_download' name='sbm_table_excess_download' size='7' maxlength='6' onblur='sbm_update_available_bw()' /> Kbps</td></tr>");
    $("#SBMDataForm").append("<tr style='display: none'><td align='right'><%lang("Use Maximal Upload");%>:</td><td><input id='sbm_table_excess_upload' name='sbm_table_excess_upload' size='7' maxlength='6' onblur='sbm_update_available_bw()' /> Kbps</td></tr>");
    sbm_update_available_bw();
}


/* ##### DBM ##### */
function GetDBMGridData(){
    var DBM_rows = [];
    var DBM_data = {};

    for (var i=0; i<bw_dbm_rule.length; i++ )  {
        DBM_data = {};
        DBM_data["dbm_table_name"] = bw_dbm_rule[i][enum_dbm._NAME];
        DBM_data["dbm_table_enable"] = tbl_wrap_enable( bw_dbm_rule[i][enum_dbm._ENABLE] );

		if (bw_dbm_rule[i][enum_dbm._INIPADDR_START] == bw_dbm_rule[i][enum_dbm._INIPADDR_END]) {			
            DBM_data["dbm_table_inipaddr"] = bw_dbm_rule[i][enum_dbm._INIPADDR_START];
		} else {
            DBM_data["dbm_table_inipaddr"] = "<%lang("From:");%>"+bw_dbm_rule[i][enum_dbm._INIPADDR_START] + "<br/><%lang("To:  ");%>"+bw_dbm_rule[i][enum_dbm._INIPADDR_END];
		}
        DBM_rows.push(DBM_data);
    }
    return DBM_rows;
}

function CreateDBMDataForm(){
    $("#DBMDataForm").append("<tr><td align='right'><%lang("Rule Name");%>:</td><td><input id='dbm_table_name' name='dbm_table_name' size='20' maxlength='20' onblur='valid_name(this, \"Rule Name\", 1)' /></td></tr>");
    $("#DBMDataForm").append("<tr><td align='right'><%lang("Rule Enable");%>:</td><td><input id='dbm_table_enable' name='dbm_table_enable' type='checkbox' value='1' /></td></tr>");
    $("#DBMDataForm").append("<tr><td align='right'><%lang("Internal IP Range");%>:</td><td>"+ 
                             "<%lang("From");%>: <input id='dbm_table_inipaddr_start' name='dbm_table_inipaddr_start' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range Start\")'/><br>"+
                             "<%lang("To");%>: <input id='dbm_table_inipaddr_end' name='dbm_table_inipaddr_end' size='17' maxlength='15' onblur='valid_ipaddr(this, \"Internal IP Range End\")' />"+
                             "</td></tr>");
}

/* ##### DBM End ##### */

$(document).ready(function(){
    /* ##### SBM ##### */
    var SBM_rows = GetSMBGridData();

    jQuery('#SBMListData').jqGrid({
            data: SBM_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Name");%>", "<%lang("Enable");%>", "<%lang("IP Address");%>", "<%lang("Application");%>", "<%lang("External Interface");%>", "<%lang("Bandwidth");%>"],
            colModel:[
                      {name:'sbm_table_name',index:'sbm_table_name', width:5, align:"center", sortable:false, editable:true},
                      {name:'sbm_table_enable',index:'sbm_table_enable', width:2, align:"center", sortable:false, editable:true},
                      {name:'sbm_table_inipaddr_start',index:'sbm_table_inipaddr_start', width:8, align:"left", sortable:false, editable:true},
                      {name:'sbm_table_app',index:'sbm_table_app', width:8, align:"left", sortable:false, editable:true},
                      {name:'sbm_table_extif',index:'sbm_table_extif', width:5, align:"center", sortable:false, editable:true},
                      {name:'sbm_table_bw',index:'sbm_table_bw', width:6, align:"left", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#SBMListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("Static Bandwidth Management");%> (SBM)"
    });

    jQuery("#SBMListData").jqGrid('navGrid','#SBMListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#SBMListData").navButtonAdd('#SBMListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"SBM_AddItem",
       onClickButton: function(){
            $("#SBMDataForm").empty();
            $("#DBMDataForm").empty();
            CreateSBMDataForm();
            $( "#SBMListData-dialog" ).dialog({
                resizable: false,
                height:570,
                width:350,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check("sbm_table")==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule('sbm_table');
                        bw_sbm_rule.push(Array_Tmp);
                        var SBM_rows = GetSMBGridData();

                        $("#SBMListData").jqGrid("clearGridData", true);
                        jQuery("#SBMListData").setGridParam({ 'data':  SBM_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            
       }
    });

    jQuery("#SBMListData").navButtonAdd('#SBMListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"SBM_DelItem",
       onClickButton: function(){
            $("#SBMDataForm").empty();
            $("#DBMDataForm").empty();
            var rowid = $("#SBMListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                bw_sbm_rule.splice(DelItem,1);
            }
            var SBM_rows = GetSMBGridData();
            $("#SBMListData").jqGrid("clearGridData", true);
            jQuery("#SBMListData").setGridParam({ 'data':  SBM_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#SBMListData").navButtonAdd('#SBMListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"SBM_ModifyItem",
       onClickButton: function(){
            $("#SBMDataForm").empty();
            $("#DBMDataForm").empty();
            CreateSBMDataForm();
            var rowid = $("#SBMListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            $("#sbm_table_name").val(bw_sbm_rule[ModifyIndex][enum_sbm._NAME]);

            if (bw_sbm_rule[ModifyIndex][enum_sbm._ENABLE] == 1){
                $("#sbm_table_enable").attr('checked',true);
            }else{
                $("#sbm_table_enable").attr('checked',false);
            }

            $("#sbm_table_inipaddr_start").val(bw_sbm_rule[ModifyIndex][enum_sbm._INIPADDR_START]);
            $("select[name='sbm_table_proto']").attr("value", bw_sbm_rule[ModifyIndex][enum_sbm._PROTOCOL]);
            if (bw_sbm_rule[ModifyIndex][enum_sbm._PROTOCOL]!=""){
                $("#sbm_table_extport_start").attr('disabled',false);
                $("#sbm_table_extport_end").attr('disabled',false);
            }
            $("#sbm_table_extport_start").val(bw_sbm_rule[ModifyIndex][enum_sbm._PORT_START]);
            $("#sbm_table_extport_end").val(bw_sbm_rule[ModifyIndex][enum_sbm._PORT_END]);

            $("select[name='sbm_table_extif']").attr("value", bw_sbm_rule[ModifyIndex][enum_sbm._EXTIF]);
            $("select[name='sbm_table_bw_alloc']").attr("value", bw_sbm_rule[ModifyIndex][enum_sbm._BW_ALLOC]);


            if (bw_sbm_rule[ModifyIndex][enum_sbm._BW_ALLOC] == 'ratio') {
                $('#sbm_table_bw_ratio').val(bw_sbm_rule[ModifyIndex][enum_sbm._BW_RATIO]);
                //$("#sbm_table_bw_ratio").parents("tr").show();
                //$("#sbm_table_bw_upload").parents("tr").hide();
                //$("#sbm_table_bw_download").parents("tr").hide();
            } else {
                $('#sbm_table_bw_upload').val(bw_sbm_rule[ModifyIndex][enum_sbm._BW_UPLOAD]);
                $('#sbm_table_bw_download').val(bw_sbm_rule[ModifyIndex][enum_sbm._BW_DOWNLOAD])
                $("#sbm_table_bw_ratio").parents("tr").hide();
                $("#sbm_table_bw_upload").parents("tr").show();
                $("#sbm_table_bw_download").parents("tr").show();
            }

            if (bw_sbm_rule[ModifyIndex][enum_sbm._EXCESS] == 1){
                $("#sbm_table_excess").attr('checked',true);
            }else{
                $("#sbm_table_excess").attr('checked',false);
            }

            if (bw_sbm_rule[ModifyIndex][enum_sbm._EXCESS] == 1 && bw_sbm_rule[ModifyIndex][enum_sbm._BW_ALLOC] == 'ratio'){
                $('#sbm_table_excess_ratio').val(bw_sbm_rule[ModifyIndex][enum_sbm._EXCESS_RATIO]);
                $("#sbm_table_excess_ratio").parents("tr").show();
                $("#sbm_table_excess_download").parents("tr").hide();
                $("#sbm_table_excess_upload").parents("tr").hide();
           } else if( bw_sbm_rule[ModifyIndex][enum_sbm._EXCESS] == 1 && bw_sbm_rule[ModifyIndex][enum_sbm._BW_ALLOC] == 'bw' ){
                $('#sbm_table_excess_download').val(bw_sbm_rule[ModifyIndex][enum_sbm._EXCESS_DOWNLOAD]);
                $('#sbm_table_excess_upload').val(bw_sbm_rule[ModifyIndex][enum_sbm._EXCESS_UPLOAD]);
                $("#sbm_table_excess_ratio").parents("tr").hide();
                $("#sbm_table_excess_download").parents("tr").show();
                $("#sbm_table_excess_upload").parents("tr").show();
           } /*else {
                $("#sbm_table_excess_ratio").parents("tr").hide();
                $("#sbm_table_excess_download").parents("tr").hide();
                $("#sbm_table_excess_upload").parents("tr").hide();
           }*/

            $( "#SBMListData-dialog" ).dialog({
                resizable: false,
                height:570,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check('sbm_table')==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule('sbm_table');
                        bw_sbm_rule.splice(ModifyIndex,1, Array_Tmp);

                        var SBM_rows = GetSMBGridData();
                        $("#SBMListData").jqGrid("clearGridData", true);
                        jQuery("#SBMListData").setGridParam({ 'data':  SBM_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });
    
    /* ##### SBM End ##### */

    /* ##### DBM ##### */
    var DBM_rows = GetDBMGridData();

    jQuery('#DBMListData').jqGrid({
            data: DBM_rows,
            datatype: 'local',
            colNames:["<%lang("Rule Name");%>", "<%lang("Rule Enable");%>", "<%lang("DBM IP");%>"],
            colModel:[
                      {name:'dbm_table_name',index:'enable', width:5, align:"center", sortable:false, editable:true},
                      {name:'dbm_table_enable',index:'username', width:3, align:"center", sortable:false, editable:true},
                      {name:'dbm_table_inipaddr',index:'passwd', width:8, align:"left", sortable:false, editable:true}
                     ],
            //autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            //sortname: 'Content',
            //sortorder: 'desc',
            pager: '#DBMListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "<%lang("Rule");%>"
    });

    jQuery("#DBMListData").jqGrid('navGrid','#DBMListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    jQuery("#DBMListData").navButtonAdd('#DBMListDataPager',{
       caption:"<% lang("Add"); %>", 
       buttonicon:"ui-icon-plusthick", 
       id:"DBM_AddItem",
       onClickButton: function(){
            $("#SBMDataForm").empty();
            $("#DBMDataForm").empty();
            CreateDBMDataForm();
            $( "#DBMListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }
                        var Array_Tmp = local_generate_rule();
                        bw_dbm_rule.push(Array_Tmp);
                        var DBM_rows = GetDBMGridData();
                        $("#DBMListData").jqGrid("clearGridData", true);
                        jQuery("#DBMListData").setGridParam({ 'data':  DBM_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
       }
    });

    jQuery("#DBMListData").navButtonAdd('#DBMListDataPager',{
       caption:"<% lang("Delete"); %>", 
       buttonicon:"ui-icon-trash",
       id:"DBM_DelItem",
       onClickButton: function(){
            $("#SBMDataForm").empty();
            $("#DBMDataForm").empty();
            var rowid = $("#DBMListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                bw_dbm_rule.splice(DelItem,1);
            }
            var DBM_rows = GetDBMGridData();
            $("#DBMListData").jqGrid("clearGridData", true);
            jQuery("#DBMListData").setGridParam({ 'data':  DBM_rows }).trigger("reloadGrid");
       }
    });

    jQuery("#DBMListData").navButtonAdd('#DBMListDataPager',{
       caption:"<% lang("Modify"); %>", 
       buttonicon:"ui-icon-pencil", 
       id:"DBM_ModifyItem",
       onClickButton: function(){
            $("#SBMDataForm").empty();
            $("#DBMDataForm").empty();
            CreateDBMDataForm();
            var rowid = $("#DBMListData").jqGrid('getGridParam','selrow');
            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var ModifyIndex = rowid - 1;
            }

            $("#dbm_table_name").val(bw_dbm_rule[ModifyIndex][enum_dbm._NAME]);
            if (bw_dbm_rule[ModifyIndex][enum_dbm._ENABLE] == 1){
                $("#dbm_table_enable").attr('checked',true);
            }else{
                $("#dbm_table_enable").attr('checked',false);
            }
            $("#dbm_table_inipaddr_start").val(bw_dbm_rule[ModifyIndex][enum_dbm._INIPADDR_START]);
            $("#dbm_table_inipaddr_end").val(bw_dbm_rule[ModifyIndex][enum_dbm._INIPADDR_END]);

            $( "#DBMListData-dialog" ).dialog({
                resizable: false,
                height:250,
                width:400,
                modal: true,
                buttons: {
                    "<% lang("Confirm"); %>": function() {
                        if (local_rule_check()==false){
                            return false;
                        }

                        var Array_Tmp = local_generate_rule();
                        bw_dbm_rule.splice(ModifyIndex,1, Array_Tmp);

                        var DBM_rows = GetDBMGridData();
                        $("#DBMListData").jqGrid("clearGridData", true);
                        jQuery("#DBMListData").setGridParam({ 'data':  DBM_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            

       }
    });

    /* ##### DBM End ##### */
});//End Ready

</script>
</head>
  <body class="gui" onload="init();"> <% showad(); %>
      <div id="SBMListData-dialog" title="SBM Data" style="display: none">
        <table id="SBMDataForm"></table>
      </div>

      <div id="DBMListData-dialog" title="DBM Data" style="display: none">
        <table id="DBMDataForm"></table>
      </div>
    <div id="wrapper">
      <div id="header">
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Bandwidth", "iDBM"); %>
        <script type="text/javascript">do_header_post()</script>
      </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">
        do_page_path('<%lang("Bandwidth");%>', '<%lang("iDBM");%>')
      </script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements("bw_rule")</script>
	          <script type="text/javascript">do_sbm_used_bw()</script>
              <% tbl_get("bw_sbm_rule"); %>
              <% tbl_get("bw_dbm_rule"); %>
              <fieldset>
                <legend><%lang("Intelligent Dynamic Bandwidth Management");%> (iDBM)</legend>
                <div class="setting">
                  <div class="label"><% lang("iDBM"); %></div>
                  <input type="radio" value="1" name="bw_enable" onclick="init()" <%nvg_match("bw_enable","1","checked");%>>
                    <%lang("Enable");%>
                  </input>

                  <input type="radio" value="0" name="bw_enable" onclick="init()" <%nvg_match("bw_enable","0","checked");%>>
                    <%lang("Disable");%>
                  </input>
                </div>
              </fieldset><br />
              <% bw_show_setting(); %>

              <table id="SBMListData"></table>
              <div id="SBMListDataPager"></div>
                <%tbl_create_table("sbm_table", "bw_sbm_rule", "1");%>
              <br />

              <%prod_comment_html_start("PROD_CAT_T", "2", "2" , "PROD_SUBSUBCAT_L");%>
              <fieldset>
                <legend><%lang("Dynamic Bandwidth Management");%> (DBM)</legend>

                <div style='font-size:12px;margin-left:10px'>
                    <%lang("The rest bandwidth from setting SBM would be totally used for DBM.");%>
                </div>
                <br/>
                <script type="text/javascript">do_dbm_available_bw();</script>
                <br/>
                  <table id="DBMListData"></table>
                  <div id="DBMListDataPager"></div>
                <%tbl_create_table("dbm_table", "bw_dbm_rule", "1");%>
              </fieldset><br />
              <%prod_comment_html_end("PROD_CAT_T", "2", "2" , "PROD_SUBSUBCAT_L");%>

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
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
