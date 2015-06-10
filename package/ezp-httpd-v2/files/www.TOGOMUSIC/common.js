
/* ##### Web UI ##### */

function refresh() {
    window.location.reload();
}

function swap_proto_name(name) {
    if(name == 'dhcp'){
        W("Wired Internet(DHCP)");
    }else if(name == 'pppoe') {
        W("Wired Internet(PPPoE)");
    }else if(name == 'static') {
        W("Wired Internet(Static IP)");
    }else if(name == 'wisp') {
        W("Wireless Internet");
    }else if(name == 'iphone') {
        W("USB Internet(Apple iPhone)");
    }else if(name == 'htc') {
        W("USB Internet(Android phone)");
    }else if(name == 'wwan') {
        W("USB Internet(3G/4G Mobile Internet)");
    }else {
        W("");
    }
}

function html_convert_char(str) {
    return str.replace(/&/g, "&amp;").replace(/>/g, "&gt;").replace(/</g, "&lt;"); 
}

function html_revert_char(str) {
    return str.replace(/&amp;/g, "&").replace(/&gt;/g, ">").replace(/&lt;/g, "<"); 
}

function W(s)
{
    document.write(s);
}

function do_hidden_elements(key) {
     if (key != undefined) {
         W("<input type='hidden' name='"+key+"' />")
    }
    W("<input type='hidden' name='submit_button' id='submit_button' />");
    W("<input type='hidden' name='action' id='action' />");
    W("<input type='hidden' name='type' id='type' />");
    W("<input type='hidden' name='num' id='num' />");
}

/* TODO: return the message of the status. */
function display_return(status, msg) {
    var message;
    if (status == '0') {
       message = '<%lang("Settings are saved successfully.");%><BR/><%lang("Settings take effect immediately."); %>';
    } else if (status == '-1') {
       message = '<%lang("No changes of the settings are made.");%>';
    } else if (status == '-2') {
       message = '<%lang("The values you entered are invalid.");%><BR/>';
       message += msg + '<BR/><%lang("Please try again.");%>';
    } else if (status == '-3') {
       message = msg + '<BR/>';
    } else if (status == '2') {
       message = '<%lang("Login Success.");%><BR/>';
    } else if (status == '9') {
        message = msg;
    } else {
        /* Undefined status */
    }
    return message;
}

/* TODO: open the bootstrap modal window of waiting. */
function open_waiting_window() {
    $('#waiting-window').modal({
        keyboard: false,
        backdrop: 'static'
    })
}

/* TODO: create the div html of the waiting window. */
var GV_reload_page;
function create_waiting_window(reload_page,redirect){
    GV_reload_page = reload_page;
    W("<div id='waiting-window' class='modal hide fade' tabindex='-1' role='dialog' aria-labelledby='myModalLabel' aria-hidden='true'>");
    W("  <div class='modal-body'>");
    W("  <span class='gery'>");
    W("     <div class='center'><i class='icon-spinner icon-spin icon-5x aximDarkOrange'></i></div>");
    W("  </span>");
    W("        <p class='lead' id='waiting-window-msg' style='text-align:center;height:auto;'></p>");
    W("        <div id='waiting-window-btn' style='text-align:center;display:none;'>");
    if (typeof(reload_page)=="undefined") {
        W("        <button class='btn btn-inverse' onClick='window.location.reload()'><%lang("Confirm");%></button>");
    } else {
        W("        <button class='btn btn-inverse' onClick='window.location.href=\"" + reload_page + "\";'><%lang("Confirm");%></button>");
    }
    W("        </div>");
    W("  </div>");
    W("</div>");
}

/* TODO: open the bootstrap modal window of upgrade. */
function open_upgrade_window() {
    $('#upgrade-window').modal({
        keyboard: false,
        backdrop: 'static'
    })
}

/* TODO: create the div html of the upgrade window. */
function create_upgrade_window(reload_page,redirect){
    W("<div id='upgrade-window' class='modal hide fade' tabindex='-1' role='dialog' aria-labelledby='myModalLabel' aria-hidden='true'>");
    W("  <div class='modal-body'>");
    W("  <span class='gery'>");
    W("     <div class='center'><i class='icon-spinner icon-spin icon-5x orange'></i></div>");
    W("  </span>");
    W("            <p class='lead' id='waiting-window-msg' style='text-align:center;height:auto;'>");
    W("                <br><span id='upgrade-time'>0:00</span><br>");
    W("                <%lang("Please wait about 7 minutes.");%><br>");
    W('                <%lang("The router reboots automatically after upgrade is completed.");%>');
    W("            </p><br>")
    W("  </div>");
    W("</div>");
}

function create_wisp_window(reload_page,redirect){
    GV_reload_page = reload_page;
    var model = "TOGOMUSIC";
    W("<div id='wisp-window' class='modal hide fade' tabindex='-1' role='dialog' aria-labelledby='myModalLabel' aria-hidden='true'>");
    W("  <div class='modal-body'>");
    W("  <span class='gery'>");
    W("     <div class='center'><i class='icon-spinner icon-spin icon-5x orange'></i></div>");
    W("  </span>");
    W("        <p class='lead' id='waiting-window-msg' style='text-align:center;height:auto;'>");
    W("            <br>");
    W("            "+model+" will reboot to connect to the selected network, please reconnect after rebooting.");
    W("            <br>");
    W("        </p>");
    W("        <div id='waiting-window-btn' style='text-align:center;display:none;'>");
    if (typeof(reload_page)=="undefined") {
        W("        <button class='btn btn-inverse' onClick='window.location.reload()'><%lang("Confirm");%></button>");
    } else {
        W("        <button class='btn btn-inverse' onClick='window.location.href=\"" + reload_page + "\";'><%lang("Confirm");%></button>");
    }
    W("        </div>");
    W("  </div>");
    W("</div>");
}

/* TODO: open the bootstrap modal window of wisp connect. */
function open_wisp_window() {
    $('#wisp-window').modal({
        keyboard: false,
        backdrop: 'static'
    })
}


function pad(n)
{
    n = n.toString();
    while (n.length < 2) n = '0' + n;
    return n;
}

function tbl_wrap_enable(v) {
    if (v == 1) {
        //return "<img src='v_021.gif'>";
        return "<span class='label label-success'>Active</span>";
    } else {
        //return "<img src='x_011.gif'>";
        return "<span class='label label-important'>Inactive</span>";
    }
}



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

/* ##### Function List ###### */
// 0.0.0.0
var ZERO_NO = 1;	// 0x0000 0001
var ZERO_OK = 2;	// 0x0000 0010
// x.x.x.0
var MASK_NO = 4;	// 0x0000 0100
var MASK_OK = 8;	// 0x0000 1000
// 255.255.255.255
var BCST_NO = 16;	// 0x0001 0000
var BCST_OK = 32;	// 0x0010 0000

var SPACE_NO = 1;
var SPACE_OK = 2;

/* TODO: Enable or Disable field */
function sel_change(v, check, objs) {
    var disable_flag = false;
    if (v == check) {
        disable_flag = false;
    } else {
        disable_flag = true;
    }
    for (var i=0; i<objs.length; i++ ) {
        //$("#"+objs[i]["name"]).attr("disabled",disable_flag);
        if ($("#"+objs[i]["name"]).is('input')){
            $("input[name="+objs[i]["name"]+"]").attr("disabled",disable_flag);
        } else if($("#"+objs[i]["name"]).is('select')) {
            $("select[name="+objs[i]["name"]+"]").attr("disabled",disable_flag);
        } else {
            $("#"+objs[i]["name"]).attr("disabled",disable_flag);
        }
        
    }
}

/* TODO: Produce the data for ajax function. */
function ajax_data(form_data){
    var data_array = {};
    for (var i=0; i< form_data.length; i++) {
        data_array[form_data[i]["name"]] = form_data[i]["value"];
    }
    return data_array;
}

/* TODO: ajax aciton */
function do_ajax_action (LV_url, LV_type, LV_dataType, LV_async, LV_data, LV_timer){
    
    if (LV_timer){
        waiting_timer = setTimeout(function(){
                            if (typeof(GV_reload_page)=="undefined"){
                                window.location.reload();
                            } else {
                                window.location.href = GV_reload_page;
                            }
                            //var display_msg = display_return(0) ;
                            //$("#waiting-window-btn").show();
                            //$("#waiting-window-msg").html(display_msg);
                        }, LV_timer + 500);
    }

    setTimeout(function(){
        $.ajax({
           url:        LV_url,
           type:       LV_type,
           dataType:   LV_dataType,
           async:	   LV_async,
           data:       LV_data,
           success: function(data){
                   if (LV_timer){
                       clearTimeout(waiting_timer);
                   }
                   var status;
                   var display_msg="";
                   var msg = "";
                   ret = data.split('\n');
                   for (var i=0; i<ret.length; i++ ) {
                      var data_tmp = ret[i].split("="); 
                      if (data_tmp[0]=="message") {
                          msg = data_tmp[1];
                      } else if (data_tmp[0]=="return_value") {
                          status_tmp = data_tmp[1].split(",");
                          status = status_tmp[0];
                          if (status == "1"){   
                             ip_url = status_tmp[1];
                          } else if (status == "9"){
                             msg = status_tmp[1];
                          }
                      }
                   }
                   display_msg = display_return(status, msg) ;
                   if (status=="0" || status=="2"){    // Save Successfully / reboot / Login Successfully
                       setTimeout(function(){
                           window.location.reload();
                       }, 1000);
                   } else if (status=="1") {     // reboot
                       setTimeout(function(){
                           window.location.href="http://"+ip_url;
                       }, 1000);
                   } else {
                       $("#waiting-window-btn").show();
                       $("#waiting-window-msg").html(display_msg);
                   }
           },
           error: function(xhr, textStatus, error){
               //console.log(xhr.statusText, textStatus, error);
               //alert("readyState: " + xhr.readyState + "\nstatus: " + xhr.status);
               //alert("responseText: " + xhr.responseText);
           }
        });
        
    }, 500);

}

function to_update(num, type)
{
    $("#type").val(type);
    $("#num").val(num);
    open_waiting_window();            
    var ajaxData;
    ajaxData = ajax_data( $("#form").serializeArray() );
    do_ajax_action ("update.cgi", "POST", "text", true, ajaxData, 3000);
}

function check_space(I,M1){
	M = unescape(M1);
	for(i=0 ; i<I.value.length; i++){
		ch = I.value.charAt(i);
		if(ch == ' ') {
			alert(M +' is not allowed to contain any space character!');
			I.value = I.defaultValue;	
			return false;
		}
	}

	return true;
}

function isascii(I,M) {
    for(i=0 ; i<I.length; i++){
        ch = I.charAt(i);
        if(ch < ' ' || ch > '~'){
            alert(M +' has illegal ascii code!');
            //I.value = I.defaultValue; 
            return false;
        }
    }

    return true;
}

function isxdigit(I,M) {
    for(i=0 ; i<I.length; i++){
        ch = I.charAt(i).toLowerCase();
        if(ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f') {

        } else {
            alert(M +' has illegal hexadecimal digits!');
            //I = I.defaultValue;   
            return false;
        }
    }

    return true;
}

function valid_hwaddr(I, M) {
    if (I.value == '') {
        return true;
    }

    var str = trim_unused_ws(I.value);
    I.value = str.toUpperCase();

    var mac_pat = /^([0-9A-F]{2}:){5}([0-9A-F]{2})$/
    if (!mac_pat.test(I.value)) {
        alert(M + ': ' + I.value + ' is not a MAC address');
        I.value = '';
        return false;
    }                     
}

function valid_name(I,M,flag) {
    I.value = trim_unused_ws(I.value);
	isascii(I,M);
    valid_reserved_word(I, M);

	if(flag & SPACE_NO){
		check_space(I,M);
	}
}

function valid_eng_num_char(I,M) {
    for(i=0 ; i<I.length; i++){
        ch = I.charAt(i);
        if((ch > '9' || ch < '0') && (ch > 'z' || ch < 'a') && (ch > 'Z' || ch < 'A')){
            alert(M);
            return false;
        }
    }
    return true;
}

function valid_pppoe(I,M,flag) {
    I.value = trim_unused_ws(I.value);
	isascii(I,M);
    valid_reserved_word2(I, M);

	if(flag & SPACE_NO){
		check_space(I,M);
	}
}

function trim_unused_ws(v) {
	var b = v.replace(/^\s*/,"");
    var c = b.replace(/\s*$/,"");
    return c;
}

function valid_reserved_word(I, M) {
    if (I.value.indexOf('^') != -1 || 
        I.value.indexOf('|') != -1 ||
        I.value.indexOf('\'') != -1 ||
        I.value.indexOf('"') != -1) {
        alert(M + ': ^, |, \' and " are illegal');
        I.value = '';
        return false;
    }
}

function valid_reserved_word2(I, M) {
    if (I.value.indexOf('|') != -1 ||
        I.value.indexOf('\'') != -1 ||
        I.value.indexOf('"') != -1) {
        alert(M + ': |, \' and " are illegal');
        I.value = '';
        return false;
    }
}


/* TODO: Check IP structure */
function valid_ipaddr(I, M) {
    I.value = trim_unused_ws(I.value);
    var ipaddr = I.value;
    var re = /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/;
    if (ipaddr == '') {
        return true;
    }
    if (re.test(ipaddr)) {
        var parts = ipaddr.split(".");
        if (parseInt(parseFloat(parts[0])) == 0) {
           
            alert(M + ': ' + ipaddr + ' is not an IP address');
            I.value = '';
            return false;
        }
        for (var i=0; i<parts.length; i++) {
            if (parseInt(parseFloat(parts[i])) > 255) {
                alert(M + ': ' + ipaddr + ' is not an IP address');
                I.value = '';
                return false;
            }
        }
        return true;
    } else {
        alert(M + ': ' + ipaddr + ' is not an IP address');
        I.value = '';
        return false;
    }
}

function valid_number(I, M) {

	if (typeof(I)=="object"){
		I.value = trim_unused_ws(I.value);
		for(i = 0; i < I.value.length; i++){
			ch = I.value.charAt(i);
			if(ch < '0' || ch > '9') {
				alert(M +': ' + I.value + ' is not a number');
				I.value = '';
				return false;
			}
		}
	}else {
		I = trim_unused_ws(I);
		for(i = 0; i < I.length; i++){
			ch = I.charAt(i);
			if(ch < '0' || ch > '9') {
				alert(M +': ' + I + ' is not a number');
				I = '';
				return false;
			}
		}
	}

	return true;
}

/* TODO: check the range of the number */
function valid_range(I, start, end, M) {
    if (I.value == '') {
        return true;
    }
    if (valid_number(I, M) == false) {
        return false;
    }
	d = parseInt(I.value);	
	if (d > end || d < start) {		
		alert(M + ': ' + d + ' is out of range ['+ start + ' - ' + end +']');
        I.value = '';
        return false;
	}
    return true;
}


/* For Base64 Encoding */
var b64keyStr = "ABCDEFGHIJKLMNOP" +
                "QRSTUVWXYZabcdef" +
                "ghijklmnopqrstuv" +
                "wxyz0123456789+/" +
                "=";
function encode64(input) {
    var output = "";
    var chr1, chr2, chr3 = "";
    var enc1, enc2, enc3, enc4 = "";
    var i = 0;

    do {
        chr1 = input.charCodeAt(i++);
        chr2 = input.charCodeAt(i++);
        chr3 = input.charCodeAt(i++);

        enc1 = chr1 >> 2;
        enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;

        if (isNaN(chr2)) {
            enc3 = enc4 = 64;
        } else if (isNaN(chr3)) {
            enc4 = 64;
        }

        output = output +
            b64keyStr.charAt(enc1) +
            b64keyStr.charAt(enc2) +
            b64keyStr.charAt(enc3) +
            b64keyStr.charAt(enc4);
        chr1 = chr2 = chr3 = "";
        enc1 = enc2 = enc3 = enc4 = "";
    } while (i < input.length);

    return output;
}

/* For Base64 Decoding */
function decode64(input) {
    var output = "";
    var chr1, chr2, chr3 = "";
    var enc1, enc2, enc3, enc4 = "";
    var i = 0;

    if (input == "AA==" || input == "") return ""
    // remove all characters that are not A-Z, a-z, 0-9, +, /, or =
    var base64test = /[^A-Za-z0-9\+\/\=]/g;
    if (base64test.exec(input)) {
        alert("There were invalid base64 characters in the input text.\n" +
              "Valid base64 characters are A-Z, a-z, 0-9, ´+´, ´/´, and ´=´\n" +
              "Expect errors in decoding.");
    }
    input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");

    do {
        enc1 = b64keyStr.indexOf(input.charAt(i++));
        enc2 = b64keyStr.indexOf(input.charAt(i++));
        enc3 = b64keyStr.indexOf(input.charAt(i++));
        enc4 = b64keyStr.indexOf(input.charAt(i++));

        chr1 = (enc1 << 2) | (enc2 >> 4);
        chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
        chr3 = ((enc3 & 3) << 6) | enc4;

        output = output + String.fromCharCode(chr1);

        if (enc3 != 64) {
            output = output + String.fromCharCode(chr2);
        }
        if (enc4 != 64) {
            output = output + String.fromCharCode(chr3);
        }

        chr1 = chr2 = chr3 = "";
        enc1 = enc2 = enc3 = enc4 = "";

    } while (i < input.length);

    return output;
}
function valid_wl_sec_wep_key(key, Message,reChk) {
  switch(key.length) {
  case 0:
    break;
  case 5:
  case 13:
    var test = /['"*]/g;
    if(test.exec(key.value)){
      alert(Message + 'has illegal ASCII Character * " and \'.');
      key.value = key.defaultValue;
      return false;
    }
    if(!isascii(key, Message)) {
      key.value = key.defaultValue;
      return false;
    }
    break;
  case 10:
  case 26:
    if(!isxdigit(key, Message)) {
      key.value = key.defaultValue;
      return false; 
    }
    break;
  default:
    $("#WEPErrorMsg").text("( <%lang("5 or 13 characters");%> )");
    $("#wl0_ssid0_key1").val("");
    $("#wl0_ssid0_key1c").val("");
    return false;
  }

  if (reChk) {
    $("#WEPErrorMsg").text("");
    if ($("#wl0_ssid0_key1").val()!=$("#wl0_ssid0_key1c").val()){
        $("#WEPErrorMsg").text("<%lang("Password is not same.");%>");
        $("#wl0_ssid0_key1").val("");
        $("#wl0_ssid0_key1c").val("");
        return false;
   }
  }
  return true;
}

function valid_wl_sec_wpa_psk_key(key, Message,reChk) {
  if(key.length == 64) {
    if(!isxdigit(key, Message)) {
      return false;
    }
  } else if((key.length < 8) || (key.length > 64)) {
    $("#WPAErrorMsg").text("( <%lang("8-63 characters");%> )");
    $("#wl0_ssid0_key").val("");
    $("#wl0_ssid0_keyc").val("");
    return false;
  } else {
    var test = /['"*]/g;
    if(test.exec(key.value)){
      alert(Message + 'has illegal ASCII Character * " and \'.');
      key.value = key.defaultValue;
      return false;
    }
    if(!isascii(key, Message)) {
      return false;
    }
  }

  if (reChk) {
    $("#WPAErrorMsg").text("");
    if ($("#wl0_ssid0_key").val()!=$("#wl0_ssid0_keyc").val()){
        $("#WPAErrorMsg").text("<%lang("Password is not same.");%>");
        $("#wl0_ssid0_key").val("");
        $("#wl0_ssid0_keyc").val("");
        return false;
   }

  }

  return true;
}


function logout()
{
    setCookie("auth", "");
    window.location.href=""
}

function setCookie(c_name,value,expiredays)
{
    var exdate=new Date();
    exdate.setDate(exdate.getDate()+expiredays);
    document.cookie=c_name+ "=" +escape(value)+
        ((expiredays==null) ? "" : ";expires="+exdate.toUTCString());
}


function x(n)
{
        return (n / 1024);
}

function E(e)
{
    return (typeof(e) == 'string') ? document.getElementById(e) : e;
}
function widget_query(F) {

    $.ajax({
	url:        "apply.cgi",
	type:       "POST",
	dataType:   "text",
	data:       { action:"Widget"},
	success: function(data){
	    widget_postaction(data);
	},
	error: function(xhr, textStatus, error){
	     setTimeout('widget_query(F);', 1000);
		   //console.log(xhr.statusText, textStatus, error);
	}
    });
}

var num = <%nv_get("wan_num");%>;
var rx = new Array(num);
var tx = new Array(num);
var rx_max = new Array(num);
var tx_max = new Array(num);
/* For short term, this parameter applied for lan(br0), so we keep it with a
 * number instead of an array. */
var thp_tx = 0;
var thp_rx = 0;
var thp_timestamp = 0;
var thp_last_timestamp = 0;
function widget_postaction(data) {
	result = data;
	var ret = result.split('\n');
	var memory = '';
	var usb = '';

	for (i = 0; i < num; i++) {
		/* Clean up the array in case no proper value returns.
		 * It happens when changing WAN proto to wwan from other
		 * protocols.
		 */
		rx[i] = 0;
		tx[i] = 0;
		rx_max[i] = 0;
		tx_max[i] = 0;
	}
	var i, j;
	var wwan_on = 1;
	for (i = 0; i < ret.length; i++) {
		var pair = ret[i].split('='); 
		for (j = 0; j < <%nv_get("wan_num");%>; j++) {
			
			if (pair[0] == 'wan'+j+'_ipaddr') {
				E('wan'+j+'_ipaddr').innerHTML = pair[1];
			}
			else if (pair[0] == 'vpn'+j+'_ipaddr') {
				E('vpn'+j+'_ipaddr').innerHTML = pair[1];
			}
			else if (pair[0] == "Rx"+j) {
             //   E('rx_'+j).innerHTML = pair[1];
				$("#rx_"+j).html(x(pair[1]));
				rx[j] = pair[1];
			} 
			else if (pair[0] == "Tx"+j) {
            
              //E('tx_'+j).innerHTML = pair[1];
				$("#tx_"+j).html(x(pair[1]));
				//$("#tx").html(x(pair[1]));
				tx[j] = pair[1];
			} 
			else if (pair[0] == "Rx_Max"+j) {
			    $("#rx_max").html(x(pair[1]));
				rx_max[j] = pair[1];
			} 
			else if (pair[0] == "Tx_Max"+j) {
			    $("#tx_max").html(pair[1]);
				tx_max[j] = pair[1];
			} 
		}
		if (pair[0] == "Memory") {
			memory = pair[1];			
			$("#memory").attr("data-percent", memory);
		} 
        else if (pair[0] == "dhcp_lease_num") {		
            dhcp_lease_num = pair[1];
            $("#dhcp_lease_num").html(dhcp_lease_num);
        }
        else if (pair[0] == "CPU_idle") {
            cpu_usage = 100 - pair[1];
            $("#cpu_usage").attr("data-percent", cpu_usage);
        }
        else if (pair[0] == "last_thp_timestamp") {
            thp_timestamp = pair[1];
        }
        else if (pair[0] == "last_thp_rx") {
            thp_rx = pair[1];
        }
        else if (pair[0] == "Battery") {
            battery = pair[1] + "%";
            $("#battery_status").html(battery);
        }
        else if (pair[0] == "charge_status") {
            charge = pair[1];
            $("#charge_status").html(charge);
        }
        else if (pair[0] == "SD") {
            SD = pair[1];
            $("#sd").html(SD);
        }
        else if (pair[0] == "USB") {
            USB = pair[1];
            $("#usb").html(USB);
        }
        else if (pair[0] == "connection_type") {
            swap_proto = pair[1];
            $("#connection_type").html(swap_proto);
        }
        else if (pair[0] == "link_state") {
            link_state = pair[1];
            if(link_state == "0") {
                link_state = "disconnected";
            }else if(link_state == "4") {
                link_state = "connected";
            }else {
                link_state = "connecting";
            }
            $("#link_state").html(link_state);
        }
        else if (pair[0] == "ip_addr") {
            ip_addr = pair[1];
            if(ip_addr == "(null)") {
                ip_addr = "";
            }else {
                $("#ip_addr").html(ip_addr);
            }
        }
        else if (pair[0] == "ssid") {
            ssid = pair[1];
            $("#ssid").html(ssid);
        }
        else if (pair[0] == "model") {
            model = pair[1];
            $("#model").html(model);
        }
        else if (pair[0] == "fw_version") {
            fw_version = pair[1];
            $("#fw_version").html(fw_version);
        }
        else if (pair[0] == "cur_time") {
            current_time = pair[1];
            $("#current_time").html(current_time);
        }
        else if (pair[0] == "up_time") {
            up_time = pair[1];
            $("#up_time").html(up_time);
        }
	}//End for 
    var hour = (new Date()).getHours();
    var minute = (new Date()).getMinutes();
    if(hour < 10) {
        hour = "0" + hour;
    }
    if(minute < 10) {
        minute = "0" + minute;
    }
    var t = hour + ":" + minute;
    $("#cur_time").html(t);

	setTimeout('widget_query(F);', 5000);
}
