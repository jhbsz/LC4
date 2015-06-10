<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Expires" content="-1">
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<title>.::Welcome to ZyXEL NBG4615::.</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />

<style>
.on {display:on}
.off {display:none}
td {white-space: nowrap;}
</style>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("wireless");

var wirelessmode  = <% getCfgZero(1, "OP_Mode"); %>;
var MBSSID_MAX 				= 8;
var ACCESSPOLICYLIST_MAX	= 32;
var changed = 0;
var old_MBSSID;
var defaultShownMBSSID = 0;
var SSID = new Array();
var session_timeout_interval = new Array();
var AccessPolicy = new Array();
var AccessControlList = new Array();

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

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function checkMac(str){
	var len = str.length;
	if(len!=17)
		return false;

	for (var i=0; i<str.length; i++) {
		if((i%3) == 2){
			if(str.charAt(i) == ':')
				continue;
		}else{
			if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
					(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
					(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') )
			continue;
		}
		return false;
	}
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
	if(field.value == "")
		return false;

	if (checkAllNum(field.value) == 0)
		return false;

	if( (!checkRange(field.value,1,0,255)) ||
		(!checkRange(field.value,2,0,255)) ||
		(!checkRange(field.value,3,0,255)) ||
		(!checkRange(field.value,4,1,254)) ){
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

function checkHex(str){
	var len = str.length;

	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') ||
			(str.charAt(i) >= 'a' && str.charAt(i) <= 'f') ||
			(str.charAt(i) >= 'A' && str.charAt(i) <= 'F') ){
				continue;
		}else
	        return false;
	}
	return true;
}

function checkInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if ( str.charAt(i) == '\r' || str.charAt(i) == '\n'){
				return false;
		}else
	        continue;
	}
	return true;
}

function checkStrictInjection(str)
{
	var len = str.length;
	for (var i=0; i<str.length; i++) {
		if ( str.charAt(i) == ';' || str.charAt(i) == ',' ||
			 str.charAt(i) == '\r' || str.charAt(i) == '\n'){
				return false;
		}else
	        continue;
	}
	return true;
}

function checkAllNum(str)
{
	for (var i=0; i<str.length; i++){
		if((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return false;
	}
	return true;
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

var http_request = false;
function makeRequest(url, content, handler) {
	http_request = false;
	if (window.XMLHttpRequest) { // Mozilla, Safari,...
		http_request = new XMLHttpRequest();
		if (http_request.overrideMimeType) {
			http_request.overrideMimeType('text/xml');
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
		alert('Giving up :( Cannot create an XMLHTTP instance');
		return false;
	}
	http_request.onreadystatechange = handler;
	http_request.open('POST', url, true);
	http_request.send(content);
}

function securityHandler() {
	if (http_request.readyState == 4) {
		if (http_request.status == 200) {
			parseAllData(http_request.responseText);
			UpdateMBSSIDList();
			//LoadFields(defaultShownMBSSID);

			// load Access Policy for MBSSID[selected]
			LoadAP();
			ShowAP(defaultShownMBSSID);
		} else {
			alert('There was a problem with the request.');
		}
	}
	parent.adjustMyFrameHeight();
}

function deleteAccessPolicyListHandler()
{
	window.location.reload(false);
}


function parseAllData(str)
{
	var all_str = new Array();
	all_str = str.split("\n");

	defaultShownMBSSID = parseInt(all_str[0]);

	for (var i=0; i<all_str.length-2; i++) {
		var fields_str = new Array();
		fields_str = all_str[i+1].split("\r");

		SSID[i] = fields_str[0];
		session_timeout_interval[i] = fields_str[21];
		AccessPolicy[i] = fields_str[22];
		AccessControlList[i] = fields_str[23];
	}
}

function checkData()
{
	// check Access Policy
	for(i=0; i<MBSSID_MAX; i++){
		if( document.getElementById("newap_text_" + i).value != ""){
			if(!checkMac(document.getElementById("newap_text_" + i).value)){
				alert("The mac address in Access Policy form is invalid.\n");
				return false;
			}
			if (!check_same_mac(i, document.getElementById("newap_text_" + i).value))
				return false;
		}
	}
	return true;
}

function submit_apply()
{
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	    return false;
	}

	if (checkData() == true){
		changed = 0;
		
		showWebMessage(2, "");
		document.mfsecurity_form.submit();
	}
}

function ShowAP(MBSSID)
{
	var i;
	for(i=0; i<MBSSID_MAX; i++){
		document.getElementById("apselect_"+i).selectedIndex	= AccessPolicy[i];
		show_div(false, "AccessPolicy_"+i);
	}
	show_div(true, "AccessPolicy_"+MBSSID);
	parent.adjustMyFrameHeight();
}

function LoadAP()
{
	for(var i=0; i<SSID.length; i++){
		var j=0;
		var aplist = new Array;

		if(AccessControlList[i].length != 0){
			aplist = AccessControlList[i].split(";");
			for(j=0; j<aplist.length; j++){
				document.getElementById("newap_"+i+"_"+j).value = aplist[j];
			}

			// hide the lastest <td>
			if(j%2){
				document.getElementById("newap_td_"+i+"_"+j).style.visibility = "hidden";
				document.getElementById("newap_td_"+i+"_"+j).style.display = "none";
				document.getElementById("newap_"+i+"_"+j).style.visibility = "hidden";
				document.getElementById("newap_"+i+"_"+j).style.display = "none";
				j++;
			}
		}
		// hide <tr> left
		for(; j<ACCESSPOLICYLIST_MAX; j+=2){
			document.getElementById("id_"+i+"_"+j).style.visibility = "hidden";
			document.getElementById("id_"+i+"_"+j).style.display = "none";
		}
	}
}

function selectMBSSIDChanged()
{
	// check if any security settings changed
	if(changed){
		ret = confirm("Are you sure to ignore changed?");
		if(!ret){
			document.mfsecurity_form.ssidIndex.options.selectedIndex = old_MBSSID;
			return false;
		}
		else
			changed = 0;
	}

	var selected = document.mfsecurity_form.ssidIndex.options.selectedIndex;

	// backup for user cancel action
	old_MBSSID = selected;

	MBSSIDChange(selected);
}

function MBSSIDChange(selected)
{
	// update Access Policy for MBSSID[selected]
	ShowAP(selected);

	// clear all new access policy list field
	for(i=0; i<MBSSID_MAX; i++)
		document.getElementById("newap_text_"+i).value = "";

	parent.adjustMyFrameHeight();
	return true;
}

function delap(mbssid, num)
{
	makeRequest("/goform/APDeleteAccessPolicyList", mbssid+ "," +num, deleteAccessPolicyListHandler);
}

function initTranslation()
{
	var e = document.getElementById("secureSSIDChoice");
	e.innerHTML = _("secure ssid choice");

	e = document.getElementById("securityTitle");
	e.innerHTML = _("secure access policy");
	
	e = document.getElementById("GeneralApply");
	e.value = _("general apply");
	
	e = document.getElementById("GeneralReset");
	e.value = _("general reset");
}



function initAll()
{
	if ((wirelessmode == 2) || (wirelessmode == 6)){
	    alert("You CANNOT select WISP or Client mode to configure this page");	
	}
	
	initTranslation();	
	makeRequest("/goform/wirelessGetSecurity", "n/a", securityHandler);
}

function UpdateMBSSIDList()
{
	document.mfsecurity_form.ssidIndex.length = 0;

	for(var i=0; i<SSID.length; i++){
		var j = document.mfsecurity_form.ssidIndex.options.length;
		if(SSID[i] != ""){
		document.mfsecurity_form.ssidIndex.options[j] = new Option(SSID[i], i, false, false);
		}
	}
	
	document.mfsecurity_form.ssidIndex.options.selectedIndex = defaultShownMBSSID;
	old_MBSSID = defaultShownMBSSID;
}

function setChange(c){
	changed = c;
}

function check_same_mac(aptable,str)
{	
	var stillnotfull=false;
	for(i=0; i< ACCESSPOLICYLIST_MAX/2; i++){
		input_name = "newap_"+ aptable +"_" + (2*i);
		alert(document.getElementById(input_name).value);
		if (str==document.getElementById(input_name).value)
		{
			alert("MAC Address has existed.");
			return false;
		}
		else
		{
			if (document.getElementById(input_name).value=="")
				stillnotfull=true;
		}
		input_name = "newap_" + aptable + "_" + (2*i+1);
		if (str==document.getElementById(input_name).value)
		{
			alert("MAC Address has existed.");
			return false;
		}
		else
		{
			if (document.getElementById(input_name).value=="")
				stillnotfull=true;
		}
	}
	if (!stillnotfull)
	{
		alert("Table Full.");
		return false;
	}

	return true;
}
</script>
</head>
<body onload="initAll()">
<form method="post" name="mfsecurity_form" action="/goform/MultiFilterSecurity">
<div id="table">
<ul>
<li class="table_content">
<div class="data">
<ul>
<li class="title" id="securityTitle">Access Policy</li>

<span class='on'>                      
<li class="w_text">
<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td width="40%" id="secureSSIDChoice">SSID</td>
<td>
<select name="ssidIndex" size="1" onchange="selectMBSSIDChanged()">
<!-- ....Javascript will update options.... -->
</select>
</td>
</tr>
</table>
</li>
</span>


<!--AccessPolicy for mbssid-->
<script language="JavaScript" type="text/javascript">
var aptable;

for(aptable = 0; aptable < MBSSID_MAX; aptable++){
	document.write(" <span id=AccessPolicy_"+ aptable);
	document.write(" class='off'><li class='w_text'>");
	document.write(" <table width='95%' border='0' align='center' cellpadding='0' cellspacing='0'>");
	document.write(" <tr> <td width='40%'>"+_("secure access policy capable")+"</td>");
	document.write(" <td> <select name=apselect_"+ aptable + " id=apselect_"+aptable+" size=1 onchange=\"setChange(1)\">");
	document.write(" 			<option value=0 >"+_("wireless disable")+"</option> <option value=1 >"+_("wireless allow")+"</option><option value=2 >"+_("wireless reject")+"</option></select> </td></tr>");
	document.write(" </table></li>");

	document.write(" <li class='w_text'>");
	document.write(" <table width='95%' border='0' align='center' cellpadding='0' cellspacing='0'>");
	document.write("<tr><td width='40%'>"+_("secure access policy new")+"</td>");
	document.write("	<td>	<input name=newap_text_"+aptable+" id=newap_text_"+aptable+" size=16 maxlength=17></td>	</tr></table></li>");
	
	document.write(" <li class='w_text2'>");
	document.write(" <table width='95%' border='0' align='center' cellpadding='0' cellspacing='0'>");
	document.write(" <tr><td><table width='100%' cellpadding='0' cellspacing='0' >");
	document.write(" <tr><td height='26' colspan='4'><table width='100%' border='0' cellspacing='0' cellpadding='0'>");
	document.write(" <tr><td width='5' height='26'><img src='images/table_top_left.gif' width='5' height='26' /></td>");
	document.write(" <td background='images/table_top_center.gif' class='table_title'>"+_("secure mac filter summary")+"</td>");
	document.write(" <td width='5' height='26'><img src='images/table_top_right.gif' width='5' height='26' /></td>");
	document.write(" </tr></table></td></tr>");
	document.write(" <tr ><td width='10%' valign='top'><center>");
	document.write(" <span class='top_left'>"+_("secure mac delete")+"</span>");	
	document.write(" </center></td>");
	document.write(" <td width='40%'><center>");
	document.write(" <span class='top_font'>"+_("secure mac address")+"</span>");	
	document.write(" </center></td>");
	document.write(" <td width='10%'><center>");
	document.write(" <span class='top_font2'>"+_("secure mac delete")+"</span>");	
	document.write(" </center></td>");
	document.write(" <td width='40%'><center>");
	document.write(" <span class='top_right'>"+_("secure mac address")+"</span>");	
	document.write(" </center></td>");
	document.write(" </tr>");
	
	for(i=0; i< ACCESSPOLICYLIST_MAX/2; i++){
		input_name = "newap_"+ aptable +"_" + (2*i);
		td_name = "newap_td_"+ aptable +"_" + (2*i);
		del_name = "newap_del_"+ aptable +"_" + (2*i);

		document.write(" <tr id=id_"+aptable+"_");
		document.write(i*2);
		document.write("> <td width='10%' valign='top'");
		document.write("><center><span class='table_left' id=");
		document.write(td_name);
		document.write("><a href='javascript: delap("+aptable+", ");
		document.write(2*i);
		document.write(");'");
		document.write(" onmouseout=\" MM_swapImgRestore();\" onmouseover=\"MM_swapImage('"+del_name+"','','images/i_delete_on.gif',1);\"><img src='images/i_delete.gif' border='0' id='"+del_name+"' title='Delete' ></a");
		document.write("></span></center><td width='40%' valign='top'");
		document.write("><center><input id=");
		document.write(input_name);
		document.write(" size=16 maxlength=20 readonly></td></center>");

		input_name = "newap_" + aptable + "_" + (2*i+1);
		td_name = "newap_td_" + aptable + "_" + (2*i+1);
		del_name = "newap_del_"+ aptable +"_" + (2*i+1);
		document.write("<td width='10%' valign='top'");
		document.write("><center><span id=");
		document.write(td_name);
		document.write("><a href='javascript: delap("+aptable+", ");
		document.write(2*i+1);
		document.write(");'");
		document.write(" onmouseout=\" MM_swapImgRestore();\" onmouseover=\"MM_swapImage('"+del_name+"','','images/i_delete_on.gif',1);\"><img src='images/i_delete.gif' border='0' id='"+del_name+"' title='Delete' ></a");
		document.write("></center><td width='40%' valign='top' id=");
		document.write("><center><span class='table_right'><input id=");
		document.write(input_name);
		document.write(" size=16 maxlength=20 readonly></span></td></center></tr>");

	}
	
		document.write(" <tr>");
		document.write(" <td height='5' colspan='4'><table width='100%' height='5' border='0' cellpadding='0' cellspacing='0'>");
		document.write(" <tr>");
		document.write(" <td width='5' height='5'><img src='images/table_bottom_left.gif' width='5' height='5'></td>");
		document.write(" <td height='5' background='images/table_bottom_center.gif'><img src='images/table_bottom_center.gif' width='1' height='5'></td>");
		document.write(" <td width='5'><img src='images/table_bottom_right.gif' width='5' height='5'></td>");
		document.write(" </tr>");
		document.write(" </table></td></tr>");
		
	document.write(" </table></td></tr></li>");
	
	document.write(" <li class='w_text'>");
	document.write(" <table width='100%' border='0' align='center' cellpadding='0' cellspacing='0'>");
	document.write("<tr><td></td></tr></table></li>");
	
	document.write("</span>");
}
</script>
<li></li>
<li></li>
</ul>
</div>
</li>
<center>
<li class="table_button">
<input type=button value=Apply id='GeneralApply' onclick='submit_apply()'>&nbsp;&nbsp;
<input type=reset value=Reset id="GeneralReset" onClick="window.location.reload()">
</li>
</center>
</form>
</body>
<script>
function alert(str)
{
	showWebMessage(1, str);
}
showWebMessage(<% getWebMessageFlag(); %>, _("<% getWebMessage(); %>"));
</script>  
</html>
 