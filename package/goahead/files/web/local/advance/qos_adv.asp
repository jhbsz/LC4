<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html>
<head>
<title>QoS/Bandwidth Management Settings</title>
<link href="images/inside.css" rel="stylesheet" type="text/css" />
<link href="images/tablet.css" rel="stylesheet" type="text/css" />
<link href="images/table.css" rel="stylesheet" type="text/css" />
<style>
td {white-space: nowrap;}
</style>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1">
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript" type="text/javascript">
Butterlate.setTextDomain("internet");

var OP_MODE=<% getCfgZero(1, "OP_Mode"); %>;

function doFindCurrentRule(CurrentIndex)
{
	num = parseInt(CurrentIndex);
	document.forms[0].BW_rtnCurIndex.value = num;
}
function doEditRule()
{
	document.forms[0].BW_rtnFlag.value = 1;
}
function doDeleteRule()
{
	document.forms[0].BW_rtnFlag.value = 2;
}
function doFindAppRule(CurrentIndex)
{
	num = parseInt(CurrentIndex);
	document.forms[0].BW_rtnAppIndex.value = num;
}
function doEditAppRule()
{
	document.forms[0].BW_rtnAppFlag.value = 1;
}
function atoi(str, num)
{
	i = 1;
	if (num != 1) {
		while (i != num && str.length != 0) {
			if (str.charAt(0) == '.') {
				i++;
			}
			str = str.substring(1);
		}
		if (i != num)
			return -1;
	}
	for (i=0; i<str.length; i++) {
		if (str.charAt(i) == '.') {
			str = str.substring(0, i);
			break;
		}
	}
	if (str.length == 0)
		return -1;
	return parseInt(str, 10);
}
function isAllNum(str)
{
	for (var i=0; i<str.length; i++) {
		if ((str.charAt(i) >= '0' && str.charAt(i) <= '9') || (str.charAt(i) == '.' ))
			continue;
		return 0;
	}
	return 1;
}

function initTranslation()
{
	var e;
	var i;

	e = document.getElementById("advTitle1");
	e.innerHTML = _("adv title1");
	e = document.getElementById("advTitle2");
	e.innerHTML = _("adv title2");
	e = document.getElementById("advTitle2_1");
	e.innerHTML = _("adv title2");
	e = document.getElementById("advTitle3");
	e.innerHTML = _("adv title3");
	e = document.getElementById("advTitle3_1");
	e.innerHTML = _("adv title3");
	e = document.getElementById("advUpBWStr");
	e.innerHTML = _("adv up bw");
	//e = document.getElementById("advUpReserved");
	//e.innerHTML = _("adv up reserved");
	e = document.getElementById("advDownBWStr");
	e.innerHTML = _("adv down bw");
	//e = document.getElementById("advDownReserved");
	//e.innerHTML = _("adv down reserved");
	e = document.getElementById("advAppPrio");
	e.innerHTML = _("adv app prio");
	e = document.getElementById("advAppCategory");
	e.innerHTML = _("adv app category");
	e = document.getElementById("advAppService");
	e.innerHTML = _("adv app service");
	e = document.getElementById("advAppSetting");
	e.innerHTML = _("adv app setting");
	e = document.getElementById("advUserEnable");
	e.innerHTML = _("adv user enable");
	e = document.getElementById("advUserDir");
	e.innerHTML = _("adv user dir");
	e = document.getElementById("advUserName");
	e.innerHTML = _("adv user name");
	e = document.getElementById("advUserCategory");
	e.innerHTML = _("adv user category");
	e = document.getElementById("advUserModify");
	e.innerHTML = _("adv user modify");


/*
	for (i=0; i<8; i++){
		e = document.getElementById("advUserToLan"+i);
		e.innerHTML = _("adv user to lan");
		e = document.getElementById("advUserToWan"+i);
		e.innerHTML = _("adv user to wan");

	}*/
/*
	for (i=0; i<11; i++){
		if(i!=1 && i!=2 && i!=3 && i!=8 && i!=9 ){
			e = document.getElementById("advAppSelectHigh"+i);
			e.innerHTML = _("adv select high");
			e = document.getElementById("advAppSelectMid"+i);
			e.innerHTML = _("adv select mid");
			e = document.getElementById("advAppSelectLow"+i);
			e.innerHTML = _("adv select low");
		}
	}
*/
	e = document.getElementById("advApply");
	e.value = _("adv apply");
	e = document.getElementById("advReset");
	e.value = _("adv reset");

}

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

function checkSubmit()
{
   //=====================================================
   if(document.QoSSetup.UploadBW_Vaule.value != ""){
		d1 = atoi(document.QoSSetup.UploadBW_Vaule.value, 1);
		if(isAllNum( document.QoSSetup.UploadBW_Vaule.value ) == 0){
			alert("The value of bandwitdth is not valid value.");
			document.QoSSetup.UploadBW_Vaule.focus();
			return false;
		}
		if(d1 > 32768 || d1 < 64){
			alert("Please set value between 64~32768");
			document.QoSSetup.UploadBW_Vaule.focus();
			return false;
		}
	}else{
		alert("The value of bandwitdth can not accept empty.");
		document.QoSSetup.UploadBW_Vaule.focus();
		return false;
	}
    //=====================================================
    if(document.QoSSetup.DownloadBW_Vaule.value != ""){
		d1 = atoi(document.QoSSetup.DownloadBW_Vaule.value, 1);
		if(isAllNum( document.QoSSetup.DownloadBW_Vaule.value ) == 0){
			alert("The value of bandwitdth is not valid value.");
			document.QoSSetup.DownloadBW_Vaule.focus();
			return false;
		}
		if(d1 > 32768 || d1 < 64){
			alert("Please set value between 64~32768");
			document.QoSSetup.DownloadBW_Vaule.focus();
			return false;
		}
	}else{
		alert("The value of bandwitdth can not accept empty.");
		document.QoSSetup.DownloadBW_Vaule.focus();
		return false;
	}
    
	for (i=0; i<8; i++){
		var e = document.getElementsByName("userEnable"+i)[0].checked;
		var v = document.getElementsByName("userTextField"+i)[0].value;
		//alert(e);
		if(e){
			if(v==""){
				alert("The value of service name can not accept empty.");
				document.getElementsByName("userTextField"+i)[0].focus();
				return false;
			}
			for(j=0;j<v.length;j++){
				if(v.charAt(j)==',' || v.charAt(j)==';' ){
					alert("The value of service name can not accept , and ;  .");
					document.getElementsByName("userTextField"+i)[0].value="";
					document.getElementsByName("userTextField"+i)[0].focus();
					return false;
				}
			}
		}
	}
	showWebMessage(2, "");
	return true;
}
var uploadBandwidth = "<% QoSGetInfo("RL_QoSUploadBw"); %>";
var downloadBandwidth = "<% QoSGetInfo("RL_QoSDownloadBw"); %>";

function onInit()
{
	var i;
	initTranslation();
	
	document.QoSSetup.UploadBandwidth.options.selectedIndex = 0;
	document.QoSSetup.UploadBW_Vaule.value = uploadBandwidth;
	document.QoSSetup.DownloadBandwidth.options.selectedIndex = 0;
	document.QoSSetup.DownloadBW_Vaule.value = downloadBandwidth;
	
	for(i=0; i< document.QoSSetup.UploadBandwidth.length; i++){
		if(document.QoSSetup.UploadBandwidth.options[i].value == uploadBandwidth){
			document.QoSSetup.UploadBandwidth.options.selectedIndex = i;
			document.QoSSetup.UploadBW_Vaule.value = uploadBandwidth;
			break;
		}
	}
	for(i=0; i< document.QoSSetup.DownloadBandwidth.length; i++){
		if(document.QoSSetup.DownloadBandwidth.options[i].value == downloadBandwidth){
			document.QoSSetup.DownloadBandwidth.options.selectedIndex = i;
			document.QoSSetup.DownloadBW_Vaule.value = downloadBandwidth;
			break;
		}
	}

}
function UpdoServiceChange(index){
   if(index==0){
   		document.QoSSetup.UploadBW_Vaule.value="";
   }else{
		document.QoSSetup.UploadBW_Vaule.value=document.QoSSetup.UploadBandwidth.options[index].value;
	}
}
function DowndoServiceChange(index){
   if(index==0){
   		document.QoSSetup.DownloadBW_Vaule.value="";
   }else{
		document.QoSSetup.DownloadBW_Vaule.value=document.QoSSetup.DownloadBandwidth.options[index].value;
	}
}

</script>
</head>
<!--     body      -->
<body onload="onInit();MM_preloadImages('images/i_edit_on.gif','images/i_delete_on.gif')">
<!--<h1 id="QoSTitleStr">Quality of Service Settings </h1>
<p id="QoSIntroStr"> You may setup rules to provide Quality of Service guarantees for specific applications.</p>
<hr>-->
<form method="post" name="QoSSetup" action="/goform/QoSAdvSetup">
<input type="hidden" name="BW_rtnCurIndex" value="0">
<input type="hidden" name="BW_rtnFlag" value="0">
<input type="hidden" name="BW_rtnAppFlag" value="0">
<input type="hidden" name="BW_rtnAppIndex" value="0">
<div id="table">
	<ul>
  	<!--<li class="table_top"></li>-->
    <li class="table_content">
    <!--<br />-->
    	<div class="data">
      	<ul>
      		<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="advTitle1">Management Bandwidth</td>
                   </tr></table>
      		</li>
			<li class="w_text">
          	<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            	<tr>
					<td width="30%" ><font id="advUpBWStr">Upstream Bandwidth</font>&nbsp;&nbsp;</td>
					<td width="15%"><input name="UploadBW_Vaule" size="6" maxlength="12" value=""  type="text"> &nbsp; (Kbps) &nbsp;</td>
					<td><select name="UploadBandwidth" id="UploadBandwidth" size="1" onchange="UpdoServiceChange(this.selectedIndex);">
					<option value="00">User Defined</option>
					<option value="64">64 Kbps</option>
					<option value="96">96 Kbps</option>
					<option value="128">128 Kbps</option>
					<option value="192">192 Kbps</option>
					<option value="256">256 Kbps</option>
					<option value="384">384 Kbps</option>
					<option value="512">512 Kbps</option>
					<option value="768">768 Kbps</option>
					<option value="1024">1M bps</option>
					<option value="2048">2M bps</option>
					<option value="4096">4M bps</option>
					<option value="8192">8M bps</option>
					<option value="10240">10M bps</option>
					<option value="12288">12M bps</option>
					<option value="16384">16M bps</option>
					<option value="20480">20M bps</option>
					<option value="24576">24M bps</option>
					<option value="32768">32M bps</option>
					<!--<option value="49152">48M bps</option>-->
					</select>
					</td>
              	</tr>
           	</table>
          	</li>
			<li class="w_text">
          	<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
            	<tr>
              		<td width="30%"><font id="advDownBWStr">Downstream Bandwidth</font>&nbsp;&nbsp;</td>
                    <td width="15%"><input name="DownloadBW_Vaule" size="6" maxlength="12" value=""  type="text"> &nbsp; (Kbps) &nbsp;</td>
                    <td><select name="DownloadBandwidth" id="DownloadBandwidth" size="1" onchange="DowndoServiceChange(this.selectedIndex);">
					<<option value="00">User Defined</option>
					<option value="64">64 Kbps</option>
					<option value="96">96 Kbps</option>
					<option value="128">128 Kbps</option>
					<option value="192">192 Kbps</option>
					<option value="256">256 Kbps</option>
					<option value="384">384 Kbps</option>
					<option value="512">512 Kbps</option>
					<option value="768">768 Kbps</option>
					<option value="1024">1M bps</option>
					<option value="2048">2M	bps</option>
					<option value="4096">4M bps</option>
					<option value="8192">8M bps</option>
					<option value="10240">10M bps</option>
					<option value="12288">12M bps</option>
					<option value="16384">16M bps</option>
					<option value="20480">20M bps</option>
					<option value="24576">24M bps</option>
					<option value="32768">32M bps</option>
					<!--<option value="49152">48M bps</option>-->
					</select>
					</td>
              </tr>
            </table>
	    	</li>
			<li class="space3"></li>
			<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="advTitle2">Application List</td>
                   </tr></table>
			</li>
			<li></li>
			<li class="w_text2">
    			<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="26" colspan="3"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title"><font id ="advTitle2_1">Application List</font></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>

      				<tr>
        				<td>
        						<table width="100%" cellpadding="0" cellspacing="0" >
							        <tr >
							          <td width="5%"><center>
							            <span class="top_left">#</span>
							          </center>                            </td>
							          <td width="15%"><center>
							            <span class="top_font" id="advAppPrio">Priority</span>
							          </center>                            </td>
							          <td width="35%"><center>
							            <span class="top_font" id="advAppCategory">Category</span>
							          </center>                            </td>
							          <td width="25%">
							            <span class="top_font" id="advAppService">Service</span>
							          			                            </td>
							          <td width="20%"><center>
							            <span class="top_right" id="advAppSetting">Advanced Setting</span>
							          </center>                            </td>
							        </tr>

        <% asp_QoS("app_list"); %>
        <tr>
         	<td height="5" colspan="9">
		         <table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
			         <tr>
				         <td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
				         <td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
				         <td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
				         </tr>
				         </table></td>
			         </tr>
		         </table>
		         </td>
         </tr>
        </table>
			    </li>
				<li></li>
				<li class="space3"></li>
				<li class="title">
                  <table border="0" cellpadding="0" cellspacing="0">
                   <tr><td nowrap id="advTitle3">User-defined Service</td>
                   </tr></table>
				</li>
				<li></li>
				<li class="w_text2">
        	<table width="95%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr>
<td height="26" colspan="6"><table width="100%" border="0" cellspacing="0" cellpadding="0">
<tr>
<td width="5" height="26"><img src="images/table_top_left.gif" width="5" height="26" /></td>
<td background="images/table_top_center.gif"class="table_title"><font id ="advTitle3_1">User-defined Service</font></td>
<td width="5" height="26"><img src="images/table_top_right.gif" width="5" height="26" /></td>
</tr>
</table></td>
</tr>
             <tr >
               <td width="5%"><center>
                 <span class="top_left">#</span>
               </center>                            </td>
               <td width="10%"><center>
                 <span class="top_font" id="advUserEnable">Enable</span>
               </center>                            </td>
               <td width="20%"><center>
                 <span class="top_font" id="advUserDir">Direction</span>
               </center>                            </td>
               <td width="30%"><center>
                 <span class="top_font" id="advUserName">Service Name</span>
               </center>                            </td>
               <td width="15%"><center>
                 <span class="top_font" id="advUserCategory">Category</span>
               </center>                            </td>
               <td width="15%"><center>
                 <span class="top_right" id="advUserModify">Modify</span>
               </center>                            </td>
             </tr>

             <% asp_QoS("user_define"); %>

             <tr>
             	<td height="5" colspan="10">
             		<table width="100%" height="5" border="0" cellpadding="0" cellspacing="0">
             			<tr>
             				<td width="5" height="5"><img src="images/table_bottom_left.gif" width="5" height="5" /></td>
                    <td height="5" background="images/table_bottom_center.gif"><img src="images/table_bottom_center.gif" width="1" height="5" /></td>
                    <td width="5"><img src="images/table_bottom_right.gif" width="5" height="5" /></td>
                  </tr>
                </table>
             	</td>
             </tr>
             </table></td>
             </tr>
            </table>
			    </li>
				<li></li>
				<li></li>
       </ul>
     </div>
    </li>
    <center>
        <li class="table_button">
		<input type="submit" value="Apply" id="advApply" name="BW_CCfg_Apply" onClick="return checkSubmit()"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		<input type="reset" value="Reset" id="advReset" name="reset" onClick="window.location.reload()" >
        </li>
        </center>
  </ul>
</div>
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

