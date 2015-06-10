<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=iso-8859-1"  />
<meta http-equiv="Pragma" content="no-cache">
<title>.::Welcome to ZyXEL NBG417N::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

<!--
function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}

function MM_showHideLayers() { //v6.0
  var i,p,v,obj,args=MM_showHideLayers.arguments;
  for (i=0; i<(args.length-2); i+=3) if ((obj=MM_findObj(args[i]))!=null) { v=args[i+2];
    if (obj.style) { obj=obj.style; v=(v=='show')?'visible':(v=='hide')?'hidden':v; }
    obj.visibility=v; }
}
//-->

var opmode = "<% getCfgZero(1, "OperationMode"); %>";

var destination = new Array();
var gateway = new Array();
var netmask = new Array();
var flags = new Array();
var metric = new Array();
var ref = new Array();
var use = new Array();
var true_interface = new Array();
var category = new Array();
var interface = new Array();
var idle = new Array();
var comment = new Array();
var routing_number_max = "<% getStaticRoutingNumbers(); %>";

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

function deleteClick()
{
	return true;
}

function checkRange(str, num, min, max)
{
    d = atoi(str,num);
    if(d > max || d < min)
        return false;
    return true;
}

function checkIpAddr(field)
{
    if(field.value == ""){
        field.focus();
        return false;
    }

    if ( isAllNum(field.value) == 0) {
        field.focus();
        return false;
    }

    if( (!checkRange(field.value,1,0,255)) ||
        (!checkRange(field.value,2,0,255)) ||
        (!checkRange(field.value,3,0,255)) ||
        (!checkRange(field.value,4,0,255)) ){
        field.focus();
        return false;
    }

   return true;
}


function atoi(str, num)
{
	i=1;
	if(num != 1 ){
		while (i != num && str.length != 0){
			if(str.charAt(0) == '.'){
				i++;
			}
			str = str.substring(1);
		}
	  	if(i != num )
			return -1;
	}
	
	for(i=0; i<str.length; i++){
		if(str.charAt(i) == '.'){
			str = str.substring(0, i);
			break;
		}
	}
	if(str.length == 0)
		return -1;
	return parseInt(str, 10);
}

function isAllNum(str)
{
	for (var i=0; i<str.length; i++){
	    if((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function formCheck()
{
	if( document.addrouting.dest.value != "" && !checkIpAddr(document.addrouting.dest )){
		alert("The destination has wrong format.");
		return false;
	}
	if( document.addrouting.netmask.value != "" && !checkIpAddr(document.addrouting.netmask )){
		alert("The netmask has wrong format.");
		return false;
	}
	if( document.addrouting.gateway.value != "" && !checkIpAddr(document.addrouting.gateway)){
		alert("The gateway has wrong format.");
		return false;
	}

	if(	document.addrouting.dest.value == ""){
		alert("please input the destination.");
		return false;
	}

    /*
    if( document.addrouting.hostnet.selectedIndex == 1 &&
		document.addrouting.netmask.value == ""){
		alert("please input the netmask.");
        return false;
    }

	if(document.addrouting.interface.value == "Custom" &&
		document.addrouting.custom_interface.value == ""){
		alert("please input custom interface name.");
		return false;
	}
    */
    if( document.addrouting.netmask.value == "")
    {
		alert("please input the netmask.");
        return false;
    }
    
    if( document.addrouting.gateway.value == "")
    {
		alert("please input the gateway.");
        return false;
    }

    //aron add for metric
    if(	document.addrouting.metric.value == ""){
		document.addrouting.metric.value = "0";
	}

    //Limit static route to 8 entries at max
    if(routing_number_max == "1" )
    {
    	alert("Static Route table is full (8 entries)!");
	    return false;
	}

	return true;
}


function display_on()
{
  if(window.XMLHttpRequest){ // Mozilla, Firefox, Safari,...
    return "table-row";
  } else if(window.ActiveXObject){ // IE
    return "block";
  }
}

function disableTextField (field)
{
  if(document.all || document.getElementById)
    field.disabled = true;
  else {
    field.oldOnFocus = field.onfocus;
    field.onfocus = skip;
  }
}

function enableTextField (field)
{
  if(document.all || document.getElementById)
    field.disabled = false;
  else {
    field.onfocus = field.oldOnFocus;
  }
}

function initTranslation()
{
	var e;
	e = document.getElementById("routingTitle");
	e.innerHTML = _("routing title");
	e = document.getElementById("routingDest");
	e.innerHTML = _("routing routing dest");
	//e = document.getElementById("routingRange");
	//e.innerHTML = _("routing range");
	e = document.getElementById("routingNetmask");
	e.innerHTML = _("routing netmask");
	e = document.getElementById("routingGateway");
	e.innerHTML = _("routing gateway");
	e = document.getElementById("routingInterface");
	e.innerHTML = _("routing interface");
	//e = document.getElementById("routingCustom");
	//e.innerHTML = _("routing custom");
	e = document.getElementById("routingComment");
	e.innerHTML = _("routing comment");
	e = document.getElementById("routingReset");
	e.value = _("routing reset");
	e = document.getElementById("routingAddRule");
	e.value = _("routing AddRule");
	e = document.getElementById("routingNo");
	e.innerHTML = _("routing Number");
	e = document.getElementById("routingDelDest");
	e.innerHTML = _("routing del dest");
	e = document.getElementById("routingDelGateway");
	e.innerHTML = _("routing del gateway");
	e = document.getElementById("routingDelMetric");
	e.innerHTML = _("routing del metric");
	e = document.getElementById("routingDelInterface");
	e.innerHTML = _("routing del interface");
	e = document.getElementById("routingDelDelete");
	e.innerHTML = _("routing del Delete");


	e = document.getElementById("routingDelComment");
	e.innerHTML = _("routing del comment");
	//e = document.getElementById("routing host");
	//e.innerHTML = _("routing host");
	//e = document.getElementById("routing net");
	//e.innerHTML = _("routing net");
	e = document.getElementById("routing LAN");
	e.innerHTML = _("routing LAN");
	e = document.getElementById("routingMetric");
	e.innerHTML = _("routing Metric");
	e = document.getElementById("routingtable");
	e.innerHTML = _("routing table");
		e = document.getElementById("routingActive");
	e.innerHTML = _("routing Active");



	if(document.getElementById("routing WAN")){
		e = document.getElementById("routing WAN");
		e.innerHTML = _("routing WAN");
	}
}

function onInit()
{
	initTranslation();

	//document.addrouting.hostnet.selectedIndex = 0;
	
	//document.addrouting.netmask.readOnly = true;
	//document.getElementById("routingNetmaskRow").style.visibility = "hidden";
	//document.getElementById("routingNetmaskRow").style.display = "none";
	//show_div(true, "routingNetmaskRow");

	document.addrouting.interface.selectedIndex = 0;
	//document.addrouting.custom_interface.value = "";
	//document.addrouting.custom_interface.readOnly = true;

	mydiv = document.getElementById("dynamicRoutingDiv");

//  parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
}

function wrapDel(str, idle)
{
	if(idle == 1){
		document.write("<del>" + str + "</del>");
	}else
		document.write(str);
}

function style_display_on()
{
	if (window.ActiveXObject) { // IE
		return "block";
	}
	else if (window.XMLHttpRequest) { // Mozilla, Safari,...
		return "table-row";
	}
}
/*
function hostnetChange()
{
	if(document.addrouting.hostnet.selectedIndex == 1){
		//document.getElementById("routingNetmaskRow").style.visibility = "visible";
		//document.getElementById("routingNetmaskRow").style.display = style_display_on();
		show_div(true, "routingNetmaskRow");
		document.addrouting.netmask.readOnly = false;
		document.addrouting.netmask.focus();

	}else{
		document.addrouting.netmask.value = "";
		document.addrouting.netmask.readOnly = true;
		//document.getElementById("routingNetmaskRow").style.visibility = "hidden";
		//document.getElementById("routingNetmaskRow").style.display = "none";
		show_div(false, "routingNetmaskRow");
	}
//  parent.getElement("myFrame").height = document.body.scrollHeight;
  parent.adjustMyFrameHeight();
}
*/
function interfaceChange()
{
	/*
	if(document.addrouting.interface.selectedIndex == 2){
		document.addrouting.custom_interface.readOnly = false;
		document.addrouting.custom_interface.focus();
	}else{
		document.addrouting.custom_interface.value = "";
		document.addrouting.custom_interface.readOnly = true;
	}
	*/
}

function onClickDel(item)
{
	item.checked=true;
	document.delRouting.submit();
}
</script>
<!--     body      -->
</head>
<body onload="onInit()">
    <div id="table">
      <ul>
        <li class="table_content">
            <div class="data">
              <ul>
<form method="post" name="addrouting" action="/goform/addRouting">
                <li class="title">
                 <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="routingTitle">Add a routing rule</td>
                   </tr></table>
                   </li>
                <li></li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
    									<td width="40%" id="routingComment">Route Name</td>
											<td>
											<input name="comment" size="16" maxlength="32" type="text">
											</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
    									<td width="40%" id="routingDest">Destination IP Address</td>
											<td>
										  <input size="16" name="dest" type="text">
											</td>
                    </tr>
                  </table>
                </li>
                <!--
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
    									<td width="40%" id="routingRange">Host/Net</td>
											<td>
											<select name="hostnet" onChange="hostnetChange()">
											<option select="" value="host" id="routing host">Host</option>
											<option value="net"  id="routing net">Net</option>
											</select>
											</td>
                    </tr>
                  </table>
                </li>
                -->
<!--<span id="routingNetmaskRow" class='on'>-->
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
    									<td width="40%" id="routingNetmask">IP Subnet Mask</td>
											<td>
								  		<input size="16" name="netmask" type="text">
											</td>
                    </tr>
                  </table>
                </li>
<!--</span>-->                
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
    									<td width="40%" id="routingGateway">Gateway IP Address</td>
											<td>
								  		<input size="16" name="gateway" type="text">
											</td>
                    </tr>
                  </table>
                </li>
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
    									<td width="40%" id="routingMetric">Metric</td>
											<td>
								  		<input maxlength="5" size="8" name="metric" type="text">
											</td>
                    </tr>
                  </table>
                </li>
                
                
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
    									<td width="40%" id="routingInterface">Interface</td>
											<td>
											<select name="interface" onChange="interfaceChange()">
											<option select="" value="LAN" id="routing LAN">LAN</option>
											<script language="JavaScript" type="text/javascript">
											//if(opmode == "1")
												document.write("<option value=\"WAN\" id=\"routing WAN\">WAN</option>");
											</script>
											<!--<option value="Custom" id="routingCustom">Custom</option>-->
											</select>
											<!--<input alias="right" size="16" name="custom_interface" type="text">-->
											</td>
                    </tr>
                  </table>
                </li>
                
                
                <li class="w_text">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
											<td colspan=2>
											<input id="routingAddRule" value="Add Rule" name="addFilterPort" onclick="return formCheck()" type="submit"> &nbsp;&nbsp;
											</td>
                    </tr>
                  </table>
                </li>
							<li></li>
</form>							
<form action="/goform/delRouting" method="post" name="delRouting">
                <li class="w_text2">
                  <table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
                    <tr>
                      <td><table width="100%" cellpadding="0" cellspacing="0" >
                          <tr>
                            <td height="26" colspan="8"><table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr>
                                  <td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
                                  <td background="images/table_top_center.gif"class="table_title" id="routingtable"> Application Rules Summary<span class="cTitle"></span></td>
                                  <td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
                                </tr>
                            </table></td>
                          </tr>
                          <tr >
                            <td width="5%" valign="top"><center>
                                <span class="top_left" id="routingNo">#</span>
                            </center></td>
                            <td width="10%"><center>
                                <span class="top_font" id="routingActive">Active</span>
                                                        </center></td>
                            <td width="13%"><center>
                                <span class="top_font" id="routingDelComment">Route Name</span>
                            </center></td>
                            <td width="13%"><center>
                                <span class="top_font" id="routingDelDest">Destination </span>
                            </center></td>
                            <td width="13%"><center>
                                <span class="top_font" id="routingDelGateway">Gateway </span>
                            </center></td>
                            <td width="13%"><center>
                                <span class="top_font" id="routingDelMetric">Metric </span>
                            </center></td>
                            <td width="13%"><center>
                                <span class="top_font" id="routingDelInterface">Interface </span>
                            </center></td>
                            <td width="13%"><center>
                                <span class="top_right" id="routingDelDelete">Delete</span>
                            </center></td>
                          </tr>
	<script language="JavaScript" type="text/javascript">
	var i;
	var entries = new Array();
        var all_str = <% getRoutingTable(); %>;
        
        entries = all_str.split("|");
        if(entries == " " || entries == "")
        entries.length=0;
        
	for(i=0; i<entries.length; i++){
		var one_entry = entries[i].split("^");
		
		comment[i] = one_entry[0]; //AboCom UI
		if(comment[i] == " " || comment[i] == "")
			comment[i] = "&nbsp";
	        enable = one_entry[1];
	        srcipaddr_start = one_entry[2];
	        srcipaddr_end = one_entry[3];
	        destination[i] = one_entry[4]; //AboCom UI
	        dstipaddr_end = one_entry[5];
	        proto = one_entry[6];
	        srcport_start = one_entry[7];
	        srcport_end = one_entry[8];
	        dstport_start = one_entry[9];
	        dstport_end = one_entry[10];
	        interface[i] = one_entry[11]; //AboCom UI
	        routing_type = one_entry[12];
		gateway[i] = one_entry[13]; //AboCom UI
		netmask[i] = one_entry[14]; //AboCom UI
		metric[i] = one_entry[15]; //AboCom UI
		category[i] = i;
		idle[i] = 0;
		true_interface[i] = interface[i]; //AboCom UI
	}

	for(i=0; i<entries.length; i++){
		if(category[i] > -1){
			document.write("<tr bgcolor=#F1F1FF>");
			document.write("<td><center><span class='table_left'>");
			document.write(i+1);
			document.write("</span>");
			document.write("<span class='off'><input type=checkbox name=DR"+ category[i] + " value=\""+ destination[i] + " " + netmask[i] + " " + true_interface[i] +"\">");
			document.write("</span></center></td>");
		}else{
			document.write("<tr>");
			document.write("<td><center><span class='table_left'>"); 	document.write(i+1);			 	document.write("</span></center></td>");
		}

		document.write("<td><center><span class='table_font'><img src='images/i_active_on.gif' width='18' height='18'  title='On'/></span></center></td>");
		if(category[i] > -1){
		document.write("<td><center><span class='table_font'>"); 	wrapDel(comment[i], idle[i]);		document.write("</span></center></td>");
		}else{
		document.write("<td><center><span class='table_font'>default</span></center></td>");
		}
		document.write("<td><center><span class='table_font'>"); 	wrapDel(destination[i], idle[i]); 	document.write("</span></center></td>");
		document.write("<td><center><span class='table_font'>"); 	wrapDel(gateway[i], idle[i]); 		document.write("</span></center></td>");
		document.write("<td><center><span class='table_font'>"); 	wrapDel(metric[i], idle[i]); 		document.write("</span></center></td>");
		
                if(interface[i] == "lan0")
			interface[i] = _("routing LAN");
			//interface[i] = "LAN";
		else if(interface[i] == "wan0")
			interface[i] = _("routing WAN");
			//interface[i] = "WAN";
		//else if(interface[i] == "Custom")
		//	interface[i] = _("routing custom");
		document.write("<td><center><span class='table_font'>"); 	wrapDel(interface[i], idle[i]); 	document.write("</span></center></td>");		
		
		document.write("<td><center><span class='table_right'>");
		if(category[i] > -1){
		//document.write("<a href='#'><img src='images/i_edit.gif' name='Image191' width='18' height='18' border='0' id='Image191'  title='Edit'></a>&nbsp;&nbsp;");
		document.write("<a href='javascript: onClickDel(document.delRouting.DR"+ category[i] + ");' onmouseout='MM_swapImgRestore()' onmouseover=\"MM_swapImage('Image141"+i+"','','images/i_delete_on.gif',1)\">");
		document.write("<img src='images/i_delete.gif' name='Image141' width='18' height='18' border='0' id='Image141"+i+"'  title='Delete'></a>");
		}else{
		}
		document.write("</span></center></td>");

		//if(interface[i] == "LAN")
		//	interface[i] = _("routing LAN");
		//else if(interface[i] == "WAN")
		//	interface[i] = _("routing WAN");
		//else if(interface[i] == "Custom")
		//	interface[i] = _("routing custom");
        //document.write("<td><center><span class='table_font'>"); 	wrapDel(interface[i], idle[i]); 		document.write("</span></center></td>");
		
		document.write("</tr>\n");
	}
	</script>
                          <tr>
                            <td height="5" colspan="8"><table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
                                <tr>
                                  <td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
                                  <td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
                                  <td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
                                </tr>
                            </table></td>
                          </tr>
                      </table></td>
                    </tr>
                  </table>
                </li>

							<li></li>
              </ul>
            </div>
        </li>
        <center>
        <li class="table_button">
        <input type=reset value=Reset id="routingReset" onClick="window.location.reload()">
        </li>
        </center>

</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script> 
</html>
