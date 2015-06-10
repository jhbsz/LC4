<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>YUI Base Page</title>
   <style type="text/css" media="all">

</style>
<style>
.on {display:on}
.off {display:none}
</style>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" /></head>
<meta http-equiv="Pragma" content="no-cache">
<script type="text/javascript" src="js/jquery.js"></script>
<script type="text/javascript" src="js/interface.js"></script>
<script language="JavaScript" src="js/bandwidth.js"></script>
<script type="text/javascript" src="/lang/b28n.js"></script>
<script language="JavaScript">
Butterlate.setTextDomain("main");

function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
	else
		document.getElementById(id).className  = "off" ;
}

function record_pos(){


	var e = document.getElementById("bandtitle");
	e.innerHTML = _("band title");
		var e = document.getElementById("bandCancel");
	e.innerHTML = _("band Cancel");
		var e = document.getElementById("bandApply");
	e.innerHTML = _("band Apply");
		var e = document.getElementById("bandCancelX");
	e.innerHTML = _("band Cancel");
		var e = document.getElementById("bandApplyX");
	e.innerHTML = _("band Apply");

	for(i=0;i<6;i++){
		e = document.getElementById("p"+i);
		e.value=document.bwForm.elements[i].value
		//alert(e.value);
		}
}
function doSubmit(){
	document.bwForm.submit();
	//window.close();
}
</script>
<link href="images/popup.css" rel="stylesheet" type="text/css" />

<body class="bandwidth_bg" onload="record_pos();" >
<div class="wrapper">
<div class="bandwidth_d"><ul><li id="bandtitle">Drag the sliders to adjust the bandwidth for each network application.</li></ul></div>
<div class="bandwidth_high_low"></div>
<form method="post" name=bwForm action="/goform/EasyModeQoS">
	<!-- YOUR NAVIGATION GOES HERE -->
	<div id="sort1" class="groupWrapper">
		<% EasyModeShowSort(); %>
  </div>

<input type=hidden name=p0 id=p0>
<input type=hidden name=p1 id=p1>
<input type=hidden name=p2 id=p2>
<input type=hidden name=p3 id=p3>
<input type=hidden name=p4 id=p4>
<input type=hidden name=p5 id=p5>

<span id="button_on" class="on">
<div class="button">
	<ul>
		<li><a id="bandCancel" href="empty.html">Cancel</a></li>
		<li><a id="bandApply"  href="javascript: show_div(false, 'button_on'); show_div(true, 'button_off'); doSubmit();">Apply</a></li>
	</ul>
</div>
</span>
<span id="button_off" class="off">
<div class="buttonX">
	<ul>
		<li><a id="bandCancelX">Cancel</a></li>
		<li><a id="bandApplyX">Apply</a></li>
	</ul>
</div>
</span>

</form>
</div>
</body>
</html>