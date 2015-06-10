function do_header_pre() {
    W("<div class='hd1'>");
    W("<div class='hd2'>");
    W("<div class='hd3'>");
    W("<div class='hd4'>");
    W("<div id='orange-bar'></div>");
     
    W("<div id='header-body'>");
    <%show_logo();%>W("<a href='http://www.apollovideo.com'><div id='logo'></div></a><div id='Product-Name-Header'><% nv_get("get_model_only"); %></div><table class='MachineInfo'><tr><td class='MachineInfoCell'>Name:&nbsp;&nbsp;&nbsp;&nbsp;<span class='MachineInfoText'><% nv_attr_get("snmp_rule", "0", "sysname"); %></span></td></tr><tr><td class='MachineInfoCell'>Serial:&nbsp;&nbsp;&nbsp;&nbsp;<span class='MachineInfoText'><% nv_get("prox_serial"); %></span></td></tr></table>");
    
    W("<div class='nav'>");
    W("<ul class='navigation'>");

}
function do_header_post() {
    W("</ul>");
    W("</div>");
    W("</div>"); 
    W("</div>");
    W("</div>");
    W("</div>");      
    W("</div>");
}  
function do_body_pre() {
    W("<div id='mainbox'>");
}
function do_body_post() {
    W("</div>");
}
function do_page_path(main_menu, sub_menu, user) {
    W("<div id='page-path'>");
    W(main_menu + " - " + sub_menu);
    if (user) {
        W(user);
    }    
    W("</div>");
}
function do_table_pre() {
    W("<table cellpadding='0' cellspacing='0' id='mainblock'>");
    W("<tr valign='top'>");
}
function do_table_post() {
    W("</tr>");
    W("</table>");
}
function do_mainframe_pre() {
    W("<td id='main'>");
    W("<div class='padding'>");
    W("<div id='whitebox-padding'>");
    W("<div class='modulebox-white'>");
    W("<div class='bx1'>");
    W("<div class='bx2'>");
    W("<div class='bx3'>");
    W("<div class='bx4'>");
    W("<div class='bx5'>");
    W("<div class='padding'>");
}
function do_hidden_elements(key) {
     if (key != undefined) {
         W("<input type='hidden' name='"+key+"' />")
    }
    W("<input type='hidden' name='submit_button' />");
    W("<input type='hidden' name='action' />");
    W("<input type='hidden' name='type' />");
    W("<input type='hidden' name='num' />");
}
function do_mainframe_post() {
    W("<span class='article_seperator'>&nbsp;</span>");
    W("</div>");
    W("</div>");
    W("</div>");
    W("</div>");
    W("</div>");
    W("</div>");
    W("</div>");
    W("</div>");
    W("</div>");
    W("</td>");
}

function do_rightframe_pre() {
   W("<td id='sidenav' class='right'>");
   W("<div class='sidenav'>");
   W("<div class='padding'>");
   W("<div id='whitebox-padding'>");
}

function do_rightframe_post() {
   W("</div>");
   W("</div>");
   W("</div>");
   W("</td>");
}

function do_others() {
    W("<div id='bot-padding'>");
    W("<table cellpadding='0' cellspacing='0' id='botuserblock'>");
    W("<tr>");
    W("<td>");
    W("<div class='modulebox-silver'>");
    W("<div class='bx1'>");
    W("<div class='bx2'>");
    W("<div class='bx3 botbox-height'>");
    W("<div class='moduletable'>");
    <%show_copyright();%>
    W("</div>");
    W("</div>"); 
    W("</div>");
    W("</div>");
    W("</div>");
    W("</td>");
    W("</tr>");
    W("</table>");
    W("</div>");
     W("<div id='reflectbox'>");
     W("<div><div></div></div>");
     W("</div>")
    
     W("<script type='text/javascript'>");
     W("$(document).ready(function(){");
     W("       $(document).click(function(){");
     W("           $('li.top ul').removeClass('showMenu');");
     W("           $('li.top ul').addClass('sub');");
     W("       });");
     W("       $('li.top').click(function(event){");
     W("           $(this).siblings().not(this).find('ul').removeClass('showMenu');");
     W("           $(this).siblings().not(this).find('ul').addClass('sub');");
     W("           $(this).find('ul').toggleClass('sub');");
     W("           $(this).find('ul').toggleClass('showMenu');");
     W("           event.stopPropagation();");
     W("       });");
     W("   });");
     W("</script>");
}
function get_left_offset(width)
{
    var pageWidth = document.body.scrollWidth;
    var offset = (pageWidth - width) / 2;
    if (offset < 0) {
        return '0px';
    }
    return offset + 'px';
}
function create_background_cover() {
    var width = get_page_width() + 'px';
    var height = get_page_height() + 'px';
    var style = "display:none; width:" + width +"; height:" + height;

    W("<iframe id='background-cover' frameborder=0 scrolling=no ");
    W("style='"+style+"' src='empty.asp'></iframe>");
}
function create_waiting_window() {
    create_waiting_window_msg('<%lang("Please wait");%>...');
}
function create_waiting_window_msg(msg) {
    W("<div id='waiting-window-wrapper' style='display:none;'>");
    W("</div>");
    W("<div id='waiting-window' style='display:none;'>");
    
    W("<div class='widgetInfo'>");
    W("<br/>");
    W("<div id='radial'>&nbsp;</div><br>");
    W("<span id='waiting-window-msg'>" + msg + "</span>");
    W("<span id='fromnow' style='font-size:20px;'></span>");
    W("</div>");
    
    W("</div>");

    //E('waiting-window').style.top = '100px';
    /* TODO: do it when displaying */
    E('waiting-window').style.left =  get_left_offset(300);
}

function create_input_window(T, U, fixed, x) {
    if (fixed == 1) {
        W("<div id='"+T+"-input-window-wrapper-wrapper' class='input-window-wrapper-wrapper' style='display:none;'><div id='"+T+"-input-window-wrapper' class='input-window-wrapper' style='display:none;' onClick='window.location.reload()'></div><div id='"+T+"-input-window' class='input-window' style='display:none;'>");
    } else {
        W("<div id='"+T+"-input-window-wrapper-wrapper' class='input-window-wrapper-wrapper' style='display:none;'><div id='"+T+"-input-window-wrapper' class='input-window-wrapper' style='display:none;' onClick='window.location.reload()'></div><div id='"+T+"-input-window' class='input-window' style='display:none;'>");
    }
    
    W("<div class='widgetInfo'>");
    tbl_create_input_field(T);
    W("<div class='submitFooter'>");
    W("<input type='button' name='Add' value='" + '<%lang("Confirm");%>' + "' onClick='tbl_add_rule(\""+T+"\", "+U+")'/>");
    W("<input type='reset' value='" + '<%lang("Cancel Changes");%>' + "' onClick='tbl_cancel_input(\""+T+"\")'/>");
    W("</div>");
    W("</div>");
    
    W("</div>");
    W("</div>");
    
}
function create_return_window(reload_page) {
    W("<div id='return-window-wrapper' style='display:none;'>");
    W("</div>");
    W("<div id='return-window' style='display:none;'>");
    
    W("<div class='widgetInfo'>");
    W("<br/>");
    W("<div id='return-window-message'></div>");
    W("<br/><br/>");
    if (typeof(reload_page)=="undefined") {
        W("<input type='button' id='return-OK' name='OK' value='" + '<%lang("Confirm");%>' + "' onClick='window.location.reload()'/>");
    }
    else {
        W("<input type='button' id='return-OK' name='OK' value='" + '<%lang("Confirm");%>' + "' onClick='window.location.href=\"" + reload_page + "\";'/>");
    }
    W("</div>");
    
    W("</div>");
    W("</div>");

    //E('return-window').style.top = '288px';
    E('return-window').style.left =  get_left_offset(300);
}
function set_return_window_message(m) {
    E("return-window-message").innerHTML = m;
}
function create_upgrade_window() {
    W("<div id='upgrade-window-wrapper' style='display:none;'>");
    W("</div>");
    W("<div id='upgrade-window' style='display:none;'>");
    W("<div class='widgetInfo'>");
    W("<br/><br/><br/>");
    W("<div id='radial'>&nbsp;</div><br/>");
    W("<span id='upgrade-time'>0:00</span><br>");
    W("<br/>");
    W('<%lang("Please wait about 7 minutes.");%><br>');
    W('<%lang("The router reboots automatically after upgrade is completed.");%><br>');
    W("</div>");
    W("</div>");

    //E('upgrade-window').style.top = '100px';
    E('upgrade-window').style.left =  get_left_offset(300);
}

/* TODO: re-org it */
function show_mask() {
    E('background-cover').style.display = "block";
}

function display_mask(F, msg) {
    show_mask();
    var height = get_page_height() + 'px';
    E('background-cover').style.height= height;

    /* E('waiting-window').style.display = 'block';
     * it's for solving some IE6 bugs.
     */
    if (msg != undefined) {
        E('waiting-window-msg').innerHTML = msg;
    }
    setTimeout("E('waiting-window-wrapper').style.display = 'block';", 500);
    setTimeout("E('waiting-window').style.display = 'block';", 500);
}
function display_upgrade_mask(F) {
    show_mask();
    /* E('waiting-window').style.display = 'block';
     * it's for solving some IE6 bugs.
     */
    setTimeout("E('upgrade-window-wrapper').style.display = 'block';", 500);
    setTimeout("E('upgrade-window').style.display = 'block';", 500);
}
var force = false;
function wait(lanip) {
    var file = "http://" + lanip;
    var val = "wait('"+lanip+"');";
    location.replace(file);
    setTimeout(val, 3000);
}
function reset_force_flag() {
    force = false;
}
function force_display_return() {
    if (E('waiting-window').style.display == "block") {
        display_return('0');
        force = true;
    }
}
function display_return(status, msg) {
    
    var message;
    if (force) {
        force = false;
        return;
    }    
    E('waiting-window-wrapper').style.display = "none";
    E('waiting-window').style.display = "none";
    show_mask();
    /* XXX: re-org later */
    if (status == '0') {
        message = '<div class="return-msg-img-div"><img src="style/proxi/images/confirm.png" /></div><div><%lang("Settings are saved successfully.");%><BR/><%lang("Settings take effect immediately."); %></div>';
    } else if (status == '-1') {
        message = '<div class="return-msg-img-div"><img src="style/proxi/images/confirm.png" /></div><div><%lang("No changes of the settings are made.");%></div>';
    } else if (status == '-2') {
        message = '<div class="return-msg-img-div"><img src="style/proxi/images/error.png" /></div><div><%lang("The values you entered are invalid.");%><BR/>';
        if(msg!=undefined && msg!=""){
            message += msg +"<BR/>";
        }
        message += '<%lang("Please try again.");%></div>';
    } else if (status == '-3') {
        message = msg + '<BR/>';
    } else if (status == '-4') {
        message = '<div class="return-msg-img-div"><img src="style/proxi/images/error.png" /></div>';
        message += msg +"<BR/>";
    } else if (status == '2') {
        window.location.reload();	
        return;
    } else {
        /* Undefined status */
        hide_return_mask();
        return;
    }
    set_return_window_message(message);
    E('return-window-wrapper').style.display = "block";
    E('return-window').style.display = "block";
    E('return-OK').focus();
}
function hide_return_mask() {
    E('background-cover').style.display = "none";
    E('return-window-wrapper').style.display = "none";
    E('return-window').style.display = "none";
}

function act_for_connect(num) {
    var v = E('wan'+num+'_status').value;
    if (v == 0) {
        /* disconnected. Could execute 'connect' */
        if (!confirm('<div><img src="style/proxi/images/attention.png" /></div><div><%lang("Are you sure to connect?");%></div>')) {
            return;
        }
        to_action(num, 'iface', 'connect');
    } else if (v == 1) {
        /* connected. Could execute 'disconnect' */
        if (!confirm('<div class="return-msg-img-div"><img src="style/proxi/images/attention.png" /></div><div><%lang("Are you sure to disconnect?");%></div>')) {
            return;
        }
        to_action(num, 'iface', 'disconnect');
    }
}

function act_for_unload()
{
    if (!confirm('<div class="return-msg-img-div"><img src="style/proxi/images/attention.png" /></div><div><%lang("Are you sure to unload the device?");%></div>')) {
        return;
    }
    to_action(num, 'usb', 'disconnect');
}

function act_for_enable(serv) {
    var v = E(serv+'_on').value;

    if (v == 1) {
        /* disconnected. Could execute 'connect' */
        if (!confirm('<div class="return-msg-img-div"><img src="style/proxi/images/attention.png" /></div><div><%lang("Are you sure to start service?");%></div>')) {
            return;
        }
        to_action(num, serv, 'connect');
    } else if (v == 0) {
        /* connected. Could execute 'disconnect' */
        if (!confirm('<div class="return-msg-img-div"><img src="style/proxi/images/attention.png" /><%lang("Are you sure to stop service?");%></div>')) {
            return;
        }
        to_action(num, serv, 'disconnect');
    }
}

function IncludeJavaScript(jsFile)
{
  document.write('<script type="text/javascript" src="'
    + jsFile + '"></scr' + 'ipt>'); 
}

