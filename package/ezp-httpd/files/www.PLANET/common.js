/* For IE9 */
function getBrowserVersion() {  
    var browser = {};  
    var userAgent = navigator.userAgent.toLowerCase();  
    var s;  
    (s = userAgent.match(/msie ([\d.]+)/))  
        ? browser.ie = s[1]  
        : 0;  
    if (browser.ie == "9.0") return true;
    else return false;
}
if (getBrowserVersion())
    if (typeof Range.prototype.createContextualFragment == "undefined") {
        Range.prototype.createContextualFragment = function(html) {
            var doc = this.startContainer.ownerDocument;
            var container = doc.createElement("div");
            container.innerHTML = html;
            var frag = doc.createDocumentFragment(), n;
            while ( (n = container.firstChild) ) {
                frag.appendChild(n);
            }
            return frag;
        };
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

/* For Wireless encryption */
function valid_wl_sec_wep_key(key, Message) {
  switch(key.value.length) {
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
    alert(Message + " must be 5/13 ASCII characters or 10/26 hex digits");
    key.value = key.defaultValue;
    return false;
  }
  return true;
}

function valid_wl_sec_wpa_psk_key(key, Message) {
  if(key.value.length == 64) {
    if(!isxdigit(key, Message)) {
      key.value = key.defaultValue;
      return false;
    }
  } else if((key.value.length < 8) || (key.value.length > 64)) {
    alert("Invalid " + Message + ", must be between 8 and 63 ASCII characters or 64 hexadecimal digits");
    key.value = key.defaultValue;
    return false;
  } else {
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
  }
  return true;
}

Array.prototype.swap=function(a, b)
{
    var tmp=this[a];
    this[a]=this[b];
    this[b]=tmp;
}

Array.prototype.insert=function(pos, v) {
    if(pos < 0 || pos > this.length) {
        return;
    }

    for (var i = this.length - 1; i >= pos; i--) {
        this[i+1] = this[i];
    }

    this[pos] = v;
};

Array.prototype.move=function(from, to, v) {
    if (from < to) {
        for (var i = from + 1; i <= to; i++) {
            this[i-1] = this[i];
        }
    } else if (from > to) {
        for (var i = from - 1; i >= to; i--) {
            this[i+1] = this[i];
        }
    }
            
    this[to] = v;
};

ie4 = ((navigator.appName == "Microsoft Internet Explorer") && (parseInt(navigator.appVersion) >= 4 ))
ns4 = ((navigator.appName == "Netscape") && (parseInt(navigator.appVersion) < 6 ))
ns6 = ((navigator.appName == "Netscape") && (parseInt(navigator.appVersion) >= 6 ))

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

function setCookie(c_name,value,expiredays)
{
    var exdate=new Date();
    exdate.setDate(exdate.getDate()+expiredays);
    document.cookie=c_name+ "=" +escape(value)+
        ((expiredays==null) ? "" : ";expires="+exdate.toUTCString());
}

function choose_enable(en_object) {
	if(!en_object)	return;
	en_object.disabled = false;			// netscape 4.x can not work, but 6.x can work

	if(!ns4)
		en_object.style.backgroundColor = "";	// netscape 4.x have error
}
function choose_disable(dis_object) {
	if(!dis_object)	return;
	dis_object.disabled = true;

	if(!ns4)
		dis_object.style.backgroundColor = "#e0e0e0";
}
function ignoreSpaces(string) {
	var temp = "";

	string = '' + string;
	splitstring = string.split(" ");
	for(i = 0; i < splitstring.length; i++) {
		temp += splitstring[i];
	}

	return temp;
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
	for(i=0 ; i<I.value.length; i++){
		ch = I.value.charAt(i);
		if(ch < ' ' || ch > '~'){
			alert(M +' has illegal ascii code!');
			I.value = I.defaultValue;	
			return false;
		}
	}

	return true;
}

function isxdigit(I,M) {
	for(i=0 ; i<I.value.length; i++){
		ch = I.value.charAt(i).toLowerCase();
		if(ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'f') {
				
		} else {
			alert(M +' has illegal hexadecimal digits!');
			I.value = I.defaultValue;	
			return false;
		}
	}

	return true;
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
function valid_number(I, M) {
    I.value = trim_unused_ws(I.value);
    
	for(i = 0; i < I.value.length; i++){
		ch = I.value.charAt(i);
		if(ch < '0' || ch > '9') {
			alert(M +': ' + I.value + ' is not a number');
			I.value = '';
			return false;
		}
	}

	return true;
}

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

function valid_pppoe(I,M,flag) {
    I.value = trim_unused_ws(I.value);
	isascii(I,M);
    valid_reserved_word2(I, M);

	if(flag & SPACE_NO){
		check_space(I,M);
	}
}

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
/* Inconsistent code. Modify it when we rearrange the pages wl_*.asp. */
function valid_ip(F,N,M1,flag){
	var m = new Array(4);
	M = unescape(M1);

	for(i=0;i<4;i++)
		m[i] = eval(N+"_"+i).value

	if(m[0] == 127 || m[0] == 224){
		alert(M+" value is illegal!");
		return false;
	}

	if(m[0] == "0" && m[1] == "0" && m[2] == "0" && m[3] == "0"){
		if(flag & ZERO_NO){
			alert(M+' value is illegal!');
			return false;
		}
	}

	if((m[0] != "0" || m[1] != "0" || m[2] != "0") && m[3] == "0"){
		if(flag & MASK_NO){
			alert(M+' value is illegal!');
			return false;
		}
	}

	return true;
}

function valid_ip_gw(F,I,N,G) {
	var IP = new Array(4);
	var NM = new Array(4);
	var GW = new Array(4);
	
	for(i=0;i<4;i++)
		IP[i] = eval(I+"_"+i).value
	for(i=0;i<4;i++)
		NM[i] = eval(N+"_"+i).value
	for(i=0;i<4;i++)
		GW[i] = eval(G+"_"+i).value

	for(i=0;i<4;i++){
		if((IP[i] & NM[i]) != (GW[i] & NM[i])){
			alert("IP address and gateway are not at the same subnet!");
			return false;
		}
	}
	if((IP[0] == GW[0]) && (IP[1] == GW[1]) && (IP[2] == GW[2]) && (IP[3] == GW[3])){
		alert("IP address and gateway can't be same!");
		return false;
	}
	
	return true;
}

function W(s)
{
    document.write(s);
}

function EN(e)
{
    return (typeof(e) == 'string') ? document.getElementsByName(e) : e;
}

function E(e)
{
    return (typeof(e) == 'string') ? document.getElementById(e) : e;
}

function N(F, e) {
    return F.elements[e];
}

function pad(n)
{
    n = n.toString();
    while (n.length < 2) n = '0' + n;
    return n;
}

function sel_enable_objs(F, objs) {
    for (var i = 0; i < objs.length; i++) {
        if (N(F, objs[i].name)) {
            if (!N(F, objs[i].name).type && N(F, objs[i].name).length) {
                /* Other than that type is text. */
                for (var j = 0; j < N(F, objs[i].name).length; j++) {
                    N(F, objs[i].name)[j].disabled = false;
                }
            } else {
                N(F, objs[i].name).disabled = false;
            }
        } else if (E(objs[i].id)) {
            E(objs[i].id).disabled = false;
        }
    }
}

function sel_disable_objs(F, objs) {
    for (var i = 0; i < objs.length; i++) {
        if (N(F, objs[i].name)) {
            if (!N(F, objs[i].name).type && N(F, objs[i].name).length) {
                /* Other than that type is text. */
                for (var j = 0; j < N(F, objs[i].name).length; j++) {
                    N(F, objs[i].name)[j].disabled = true;
                }
            } else {
                N(F, objs[i].name).disabled = true;
            }
        } else if (E(objs[i].id)) {
            E(objs[i].id).disabled = true;
        }
    }
}

/* TODO: change naming as sel_change_show_objs */
function sel_change(v, check, F, objs) {
    if (v == check) {
        sel_enable_objs(F, objs);
    } else {
        sel_disable_objs(F, objs);
    }
}

/* TODO: change naming as nsel_change_show_objs */
function nsel_change(v, check, F, objs) {
    if (v != check) {
        sel_enable_objs(F, objs);
    } else {
        sel_disable_objs(F, objs);
    }
}
function get_page_width() {
    var pageWidth = 1280;
    if(document.body) {
        if (document.body.scrollWidth) {
            pageWidth = document.body.scrollWidth;
        } else if(document.body.offsetWidth ) {
            pageWidth = document.body.offsetWidth;
        }
    }
    return pageWidth;
}
function get_page_height() {
    var pageHeight = 1024;
    if(document.body) {
        if (document.body.scrollHeight) {
            pageHeight = document.body.scrollHeight;
        } else if(document.body.offsetHeight ) {
            pageHeight = document.body.offsetHeight;
        }
    }
    return pageHeight;
}

var http_request = false;
function ajax_request_post(url, parameters, postaction) {
    http_request = false;
    if (window.XMLHttpRequest) { // Mozilla, Safari,...
        http_request = new XMLHttpRequest();
        if (http_request.overrideMimeType) {
            http_request.overrideMimeType('text/html');
        }
    } else if (window.ActiveXObject) { // IE
        try {
            http_request = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e) {
            try {
                http_request = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e) {}
        }
    }
    if (!http_request) {
        alert('Cannot create XMLHTTP instance');
        return false;
    }
    if (postaction == undefined) {
        http_request.onreadystatechange = ajax_postaction;
    } else {
        http_request.onreadystatechange = postaction;
    }
    http_request.open('POST', url, true);
    http_request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    http_request.send(parameters);
}
var force_display_return_timer = null, reset_force_flag_timer = null;
function ajax_postaction() {
    if (http_request.readyState == 4) {
        if (http_request.status == 200) {
            var result = http_request.responseText;
            
            var ret = result.split('\n');
            var message = '', name = '';
            var i;
            for (i = 0; i < ret.length; i++) {
                var pair = ret[i].split('='); 
                var val = pair[1].split(','); 
                if (pair[0] == "message") {
                    message = pair[1];
                } else if (pair[0] == 'name') { 
                    name = val[0];
                } else if (pair[0] == "return_value") {
                    if (force_display_return_timer) {
                        clearTimeout(force_display_return_timer);
                        clearTimeout(reset_force_flag_timer);
                        }
                    if (val[0] == '1') {
                        var ip = "wait('"+val[1]+"');";
                        setTimeout(ip, 45000);
                    } else if (val[0] == '3') {
                        var ip = "wait('"+val[1]+"');";
                        setTimeout(ip, 3000);
                    } else if (val[0] == '4') {
                        var target  = document.getElementsByName(name)[0];
                        target.value = message; 
                    } else {
                        /* For IE6 problem, we postponed the display of
                         * wait-window by 0.5 second. It causes the error
                         * message on return-window be covered.
                         */
                        var command = "display_return(" + pair[1] + ",'" + message + "');";
                        setTimeout(command, 500);
                    }
                    return;
                }
            } 
        }
        display_return(1);
    }
}
function ajax_parse_parameters(N, str) {
    if (N.tagName != "INPUT" && N.tagName != "SELECT") {
        return str;
    }    
    if (N.tagName == "INPUT") {
        if (N.type == "hidden") {
            str += N.name + "=";
            str += (N.value == undefined) ? "" : N.value; 
            str += "&";
        }
        if (N.type == "text") {
            str += N.name + "=" + N.value + "&";
        }
        if (N.type == "password") {
            str += N.name + "=" + N.value + "&";
        }
        if (N.type == "file") {
            str += N.name + "=" + N.value + "&";
        }
        if (N.type == "checkbox") {
            if (N.checked) {
                str += N.name + "=" + N.value + "&";
            } else {
                str += N.name + "=&";
            }
        }
        if (N.type == "radio") {
            if (N.checked) {
                str += N.name + "=" + N.value + "&";
            }
        }
    }   
    if (N.tagName == "SELECT") {
        str += N.name + "=" + N.options[N.selectedIndex].value + "&";
    }
    return str;
}        
       
function ajax_recur(F, str) {
    if (F.tagName == undefined) {
        return str;
    }
    var i;
    if (F.childNodes != undefined) {
        for (i = 0; i < F.childNodes.length; i++) {
            str = ajax_recur(F.childNodes[i], str);
        }
    }
    str = ajax_parse_parameters(F, str);
    return str;
}    

function js_ajax_preaction(F, ds, attr, rule, array_cnt, action, postaction) {
    var getstr = "";
    /* For common parameter */
    getstr = ajax_recur(F, getstr);
    if (action == undefined) {
        action = "apply.cgi";
    }
    if ((ds != undefined) && (array_cnt != undefined) && (array_cnt > 0)) {
        if (array_cnt == 1) {
            for (var i =0;i < ds.getCount(); i++) {
                for(var j = 0;j < attr.length; j++) {
                    var out = ds.getAt(i).get(attr[j]);
                    if (Ext.type(out) == 'boolean') {
                        out = (out == true) ? 1 : 0;
                    }
                    getstr = getstr + rule + i + '_' + attr[j] + '=' + out + '&';
                } 
            }
            getstr += rule + '_num=' + ds.getCount();
        }
        else {
            /* We may include more than one rule and data store in a
             * request. It is required to use multiple data stores. */
            for (var array_idx = 0; array_idx < array_cnt; array_idx++) {
                if (array_idx != 0) {
                    getstr = getstr + '&';
                }
                for (var i =0;i < ds[array_idx].getCount(); i++) {
                    for(var j = 0;j < attr[array_idx].length; j++) {
                        var out = ds[array_idx].getAt(i).get(attr[array_idx][j]);
                        if (Ext.type(out) == 'boolean') {
                            out = (out == true) ? 1 : 0;
                        }
                        getstr = getstr + rule[array_idx] + i + '_' + attr[array_idx][j] + '=' + out + '&';
                    } 
                }
                getstr += rule[array_idx] + '_num=' + ds[array_idx].getCount();
            }
        }
    }
    ajax_request_post(action, getstr, postaction);
}

function ajax_preaction(F, action, timer, postaction) {
    var getstr = "";

    /* For common parameter */
    getstr = ajax_recur(F, getstr);
    if (timer) {
        force_display_return_timer = setTimeout("force_display_return();", timer);
        reset_force_flag_timer = setTimeout("reset_force_flag();", timer + 2000);
    }
    if (action == undefined) {
        action = "apply.cgi";
    }
    ajax_request_post(action, getstr, postaction);
}

var http_req;
function widget_query(F) {
    http_req = false;
    if (window.XMLHttpRequest) { // Mozilla, Safari,...
        http_req = new XMLHttpRequest();
        if (http_req.overrideMimeType) {
            http_req.overrideMimeType('text/html');
        }
    } else if (window.ActiveXObject) { // IE
        try {
            http_req = new ActiveXObject("Msxml2.XMLHTTP");
        } catch (e) {
            try {
                http_req = new ActiveXObject("Microsoft.XMLHTTP");
            } catch (e) {}
        }
    }
    if (!http_req) {
        alert('Cannot create XMLHTTP instance');
        return false;
    }
    http_req.onreadystatechange = widget_postaction;
    http_req.open('POST', "apply.cgi", true);
    http_req.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    http_req.send("action=Widget");
}

function to_update(num, type)
{
    F.type.value = type;
    F.num.value = num;
    ajax_preaction(F, "update.cgi", 6000);
    display_mask(F);
}
function to_action(num, type, action)
{
    var F = document.getElementsByName('form')[0];
    F.num.value = num;

    F.type.value = type;
    if (action == 'connect') {
        /* wanX_staus = 0: disconnected */
        ajax_preaction(F, "connect.cgi", 6000);
    } else if (action == 'disconnect') {
        /* wanX_staus = 0: connected */
        ajax_preaction(F, "disconnect.cgi", 6000);
    }
}

var num = <%nv_get("wan_num");%>;
var rx = new Array(num);
var tx = new Array(num);
var rx_max = new Array(num);
var tx_max = new Array(num);
function widget_postaction() {
    if (http_req.readyState == 4) {
        if (http_req.status == 200) {
            var result = http_req.responseText;
            
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
                    if (pair[0] == 'wan'+j+'_status') {
                        if (pair[1] == '0' || pair[1] == '1') {
                            /* disconnected */
                            E('wan'+j+'_light').innerHTML = '<img src="picts/link_down.gif" width=25 height=25>';
                            E('wan'+j+'_action').innerHTML = '<img src="picts/turn_up.gif" width=25 height=25>';
                            E('wan'+j+'_status').value = '0';
                        } else if (pair[1] == '2' || pair[1] == '3' || pair[1] == '4') {
                            /* connected */
                            E('wan'+j+'_light').innerHTML = '<img src="picts/link_up.gif" width=25 height=25>';
                            E('wan'+j+'_action').innerHTML = '<img src="picts/turn_down.gif" width=25 height=25>';
                            E('wan'+j+'_status').value = '1';
                        } else {
                            /* The setting protocol is STATIC. */
                            E('wan'+j+'_light').innerHTML = '';
                            E('wan'+j+'_action').innerHTML = '';
                            E('wan'+j+'_status').value = '2';
                        }
                    }
                    if (pair[0] == 'wan'+j+'_ipaddr') {
                        E('wan'+j+'_ipaddr').innerHTML = pair[1];
                    }
                    if (pair[0] == 'vpn'+j+'_ipaddr') {
                        E('vpn'+j+'_ipaddr').innerHTML = pair[1];
                    }
                    if (pair[0] == "Rx"+j) {
                        rx[j] = pair[1];
                    } 
                    if (pair[0] == "Tx"+j) {
                        tx[j] = pair[1];
                    } 
                    if (pair[0] == "Rx_Max"+j) {
                        rx_max[j] = pair[1];
                    } 
                    if (pair[0] == "Tx_Max"+j) {
                        tx_max[j] = pair[1];
                    } 
                }
                if (pair[0] == "Memory") {
                    memory = pair[1];
                } 
<%prod_show_js_start("wwan", "0", "7");%>
                if (pair[0] == "USB") {
                    if (E("usb_img") != undefined) {
                        usb = pair[1];
                        if (usb == "loaded") {
                            E("usb_img").innerHTML = '<img src="picts/remove.gif" width=25 height=25>';
                        } else {
                            E("usb_img").innerHTML = '&nbsp;';
                            E("wwan_signal").innerHTML = '&nbsp;';
                        }
                    }
                } 
                if (pair[0] == 'wwan_on' && pair[1] == '0') {
                    wwan_on = '0';
                    if (E('wwan_mf') != undefined) {
                        E('wwan_mf').innerHTML = '&nbsp;';
                    }
                    if (E('wwan_model') != undefined) {
                        E('wwan_model').innerHTML = '&nbsp;';
                    }
                    if (E('wwan_imei') != undefined) {
                        E('wwan_imei').innerHTML = '&nbsp;';
                    }
                    if (E('wwan_isp') != undefined) {
                        E('wwan_isp').innerHTML = '&nbsp;';
                    }
                    if (E('wwan_signal') != undefined) {
                        E('wwan_signal').innerHTML = '&nbsp;';
                    }
                }

                if (wwan_on == '1' && pair[0] == 'Manufacturer') {
                    if (E('wwan_mf') != undefined) {
                        E('wwan_mf').innerHTML = pair[1];
                    }
                }
                if (wwan_on == '1' && pair[0] == 'Model') {
                    if (E('wwan_model') != undefined) {
                        E('wwan_model').innerHTML = pair[1];
                    }
                }
                if (wwan_on == '1' && pair[0] == 'IMEI') {
                    if (E('wwan_imei') != undefined) {
                        E('wwan_imei').innerHTML = pair[1];
                    }
                }
                if (wwan_on == '1' && pair[0] == 'ISP') {
                    if (E('wwan_isp') != undefined) {
                        E('wwan_isp').innerHTML = pair[1];
                    }
                }
                if (wwan_on == '1' && pair[0] == 'Signal') {
                    if (E('wwan_signal') != undefined) {
                        var num = parseInt(pair[1]);
                        E('wwan_signal').innerHTML = '<span><img src="picts/sig0' + num + '.gif" width=25 height=25></span>';
                    }
                }
<%prod_show_js_end("wwan", "0", "7");%>
<%prod_show_js_start("PROD_CAT_C|PROD_CAT_T|PROD_CAT_D|PROD_CAT_K|PROD_CAT_G", "0", "7");%>
                var size, used;
                if (pair[0] == "USB") {
                    if (E("usb_img") != undefined) {
                        usb = pair[1];
                        if (usb == "loaded") {
                            E("usb_img").innerHTML = '<img src="picts/remove.gif" width=25 height=25>';
                        } else {
                            E("usb_img").innerHTML = '&nbsp;';
                        }
                    }
                } 
                if (pair[0] == 'storage_type') {
                    if (pair[1] == 'vfat') {
                    E('storage_type').innerHTML = 'VFAT';
                    } else if (pair[1] == 'ext3') {
                    E('storage_type').innerHTML = 'EXT3';
                    } else if (pair[1] == 'cifs') {
                    E('storage_type').innerHTML = 'CIFS';
                    } else if (pair[1] == '') {
                    E('storage_type').innerHTML = '&nbsp';
                    }
                }
                if (pair[0] == 'storage_size') {
                    E('storage_size').innerHTML = pair[1];
                }
                if (pair[0] == 'storage_info') {
                    E('storage_info').innerHTML = pair[1];
                }
                if (pair[0] == 'dev_msg') {
                    E('dev_msg').innerHTML = pair[1];
                }
                if (pair[0] == 'bt_on') {
                    if (pair[1] == '0') {
                        E('bt_on').value = '0';
                        E('bt_on').innerHTML = '<img src="picts/START.gif" width=25 height=25>';
                        E('bt_action').innerHTML = '<img src="picts/turn_down.gif" width=25 height=25>'
                    } else {
                        E('bt_on').value = '1';
                        E('bt_on').innerHTML = '<img src="picts/STOP.gif" width=25 height=25>'  ;
                        E('bt_action').innerHTML = '<img src="picts/turn_up.gif" width=25 height=25>'
                    }
                }
                if (pair[0] == 'ftp_on') {
                    if (pair[1] == '0') {
                        E('ftp_on').value = '0';
                        E('ftp_on').innerHTML = '<img src="picts/START.gif" width=25 height=25>';
                        E('ftp_action').innerHTML = '<img src="picts/turn_down.gif" width=25 height=25>'
                    } else {
                        E('ftp_on').value = '1';
                        E('ftp_on').innerHTML = '<img src="picts/STOP.gif" width=25 height=25>'; 
                        E('ftp_action').innerHTML = '<img src="picts/turn_up.gif" width=25 height=25>'
                    }
                }
                if (pair[0] == 'samba_on') {
                    if (pair[1] == '0') {
                        E('samba_on').value = '0';
                        E('samba_on').innerHTML = '<img src="picts/START.gif" width=25 height=25>';
                        E('samba_action').innerHTML = '<img src="picts/turn_down.gif" width=25 height=25>'
                    } else {
                        E('samba_on').value = '1';
                        E('samba_on').innerHTML = '<img src="picts/STOP.gif" width=25 height=25>'; 
                        E('samba_action').innerHTML = '<img src="picts/turn_up.gif" width=25 height=25>'
                    }
                }
                if (pair[0] == 'itunes_on') {
                    if (pair[1] == '0') {
                        E('itunes_on').value = '0';
                        E('itunes_on').innerHTML = '<img src="picts/START.gif" width=25 height=25>';
                        E('itunes_action').innerHTML = '<img src="picts/turn_down.gif" width=25 height=25>'
                    } else {
                        E('itunes_on').value = '1';
                        E('itunes_on').innerHTML = '<img src="picts/STOP.gif" width=25 height=25>'; 
                        E('itunes_action').innerHTML = '<img src="picts/turn_up.gif" width=25 height=25>'
                    }
                }
<%prod_show_js_end("PROD_CAT_C|PROD_CAT_T|PROD_CAT_D|PROD_CAT_K|PROD_CAT_G", "0", "7");%>
<%prod_show_js_start("PROD_CAT_C|PROD_CAT_T|PROD_CAT_D|PROD_CAT_K|PROD_CAT_G", "3", "7");%>
                if (pair[0] == 'iscsi_on') {
                    if (pair[1] == '0') {
                        E('iscsi_on').value = '0';
                        E('iscsi_on').innerHTML = '<img src="picts/START.gif" width=25 height=25>';
                        E('iscsi_action').innerHTML = '<img src="picts/turn_down.gif" width=25 height=25>'
                    } else {
                        E('iscsi_on').value = '1';
                        E('iscsi_on').innerHTML = '<img src="picts/STOP.gif" width=25 height=25>'; 
                        E('iscsi_action').innerHTML = '<img src="picts/turn_up.gif" width=25 height=25>'
                    }
                }
<%prod_show_js_end("PROD_CAT_C|PROD_CAT_T|PROD_CAT_D|PROD_CAT_K|PROD_CAT_G", "3", "7");%>
<%prod_show_js_start("PROD_CAT_C|PROD_CAT_T|PROD_CAT_D|PROD_CAT_K|PROD_CAT_G", "0", "7");%>
                if (pair[0] == 'swap_on') {
                    if (pair[1] == '0') {
                        E('swap_on').innerHTML = '<%lang("Used");%>';
                    } else {
                        E('swap_on').innerHTML = '<%lang("Unused");%>'; 
                    }
                }
<%prod_show_js_end("PROD_CAT_C|PROD_CAT_T|PROD_CAT_D|PROD_CAT_K|PROD_CAT_G", "0", "7");%>
            } 
            <%widget_execute();%>
            setTimeout('widget_query(F);', 3000);
        }
    }
}
function def_get_index_by_id(D, ID) {
    for (var i = 0; i < D.length; i++) {
        if (D[i].id != ID) {
            continue;
        }
        return i;
    }
    return -1;
}

function rlist_check_dup_field(T, R, index) {
    var row_num = parseInt(E(T+'_cur_rule_num').value);
    for (var i = 0; i < row_num; i++) {
        /* _ENABLE == 0 */
        if (R[i][0] == 0) {
            continue;
        }
        var name1 = R[i][index];
        for (var j = i+1; j < row_num; j++) {
            /* _ENABLE */
            if (R[j][0] == 0) {
                continue;
            }
            var name2 = R[j][index];
            if (name1 == name2) {
                alert('"'+name1+'" repeats.');
                return false;
            }
        }
    }
    return true;
}

function dlist_set_value_by_id(L, D, ID, V)
{
    var index = def_get_index_by_id(D, ID);
    L[index] = V;
}

function def_set_value_by_id(D, ID, V)
{
    var index = def_get_index_by_id(D, ID);
    D[index].value = V;
}
function logout()
{
    setCookie("auth", "");
    window.location.href=""
}
