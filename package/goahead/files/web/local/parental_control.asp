<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="Pragma" content="no-cache">
<title>Untitled Document</title>
<script type="text/javascript" src="js/jquery-1.2.6.js"></script>
<script type="text/javascript" src="js/parental_control.js"></script>
<script type="text/javascript" src="/lang/b28n.js"></script>
<link href="images/popup.css" rel="stylesheet" type="text/css" />
<style>
.on {display:on}
.off {display:none}
</style>

<script>
Butterlate.setTextDomain("main");
var s= new Array(D_NUM);
var D_NUM=16;
var D_table_index=0;
 
function show_div(show,id) {
	if(show)
		document.getElementById(id).className  = "on" ;
    else
    	document.getElementById(id).className  = "off" ;
}

function update_table_value_first()
{
 var all_str = "<% getCfgGeneral(1, "websHostFilters"); %>";    						
 var entries = new Array();
    						
 if(all_str.length){
	entries = all_str.split(";");
	
	for(i=0; i<entries.length; i++){                       
		s[i]=entries[i];  						
	}
  }
 D_table_index=entries.length;
  
 for(i=entries.length;i<D_NUM;i++)
 	s[i]="";
}

function update_table()
{	
 var kid;
 var tid;
 var cid;
 var e;
 var c;

for(i=0;i<D_NUM;i++){
    kid="k"+i;  
    tid="t"+i;
    cid="c"+i;
	if (s[i]!="") {
		e = document.getElementById(kid);
		e.innerHTML = s[i];
		c = document.getElementById(cid);
		c.checked = 0;
		show_div(true,tid);
	}
	else
	    show_div(false,tid);
}

}

function onAdd()
{
if(D_table_index!=D_NUM){
   
    var sss=document.parental_control.Add_Key_Word.value;
    
	if(checkValue(sss)==0){
    	if(sss!=""){
			s[D_table_index]=sss;
			D_table_index++;
			update_table();
			document.parental_control.Add_Key_Word.value="Add Key Word";//Arthur added
		}
	}else
		alert("Key word is exist"); 
}
else
	alert("The rule number is exceeded "+ D_NUM +"."); 
}

function checkValue(str)
{
   var flag=0;
	
	for(i=0;i<D_table_index;i++){
		if(s[i]== str){
		   flag=1;
		   break;	   
		}		  
	}
	if(flag == 1)
	   	return 1;
	else
    	return 0;
}

function onDelete()
{
	var tt= new Array(D_NUM);
	var cid;
	var e;
    var j=0;
    //alert(D_table_index); 
      
    for(i=0;i<D_table_index;i++){
      cid="c"+i;
	  e = document.getElementById(cid);
	  
	  if(e.checked ==0){
	  		tt[j]=s[i];
	  		j++; 
	  	}	  	    
	 }
	 
	 for(i=0;i<D_table_index;i++)
    	s[i]="";
	 
	 for(i=0;i<j;i++)
    	s[i]=tt[i]; 
    
     D_table_index=j;	
	 update_table();
}

function formCheck()
{
    //alert(D_table_index); 
    var str = "";
	for(var i=0; i < D_table_index; i++){
	       if(i!=(D_table_index - 1)) 
		   	{str += s[i] + ";";}
		   else
		   	{str += s[i];}
	}	
        
		document.parental_control.url_str.value=str;
}
function initTranslation()
{
	var e = document.getElementById("pctitle");
	e.innerHTML = _("pc title");
		e = document.getElementById("pcDelete");
	e.value = _("pc Delete");
		e = document.getElementById("pcAdd");
	e.value = _("pc Add");
  e = document.getElementById("pcCancel");
	e.innerHTML = _("pc Cancel");
	e = document.getElementById("pcApply");
	e.innerHTML = _("pc Apply");
  e = document.getElementById("pcCancelX");
	e.innerHTML = _("pc Cancel");
	e = document.getElementById("pcApplyX");
	e.innerHTML = _("pc Apply");
  e = document.getElementById("KeyWord");
	e.value = _("Key_Word");
  
}
</script>
</head>
<body class="bandwidth_bg" onLoad="initTranslation()">
<form method="post" name="parental_control" action="/goform/easy_parent_control">
<div class="wrapper">
<div class="bandwidth_d"><ul>
  <li id="pctitle">Any URLs that contains any of the following keywords will be blocked and cannot be browsed.</li>
</ul>
</div>
<div class="addkeyword"><ul><li><label><input name="Add_Key_Word"  class="input1" type="text" id="KeyWord" value="Add Key Word" size="25" /></label></li><li><input name="Add"  class="input2"type="button" id="pcAdd" value="Add" onclick="onAdd();"/></li><li class="pc_line"></li><li><input name="Delete" type="button" id="pcDelete" value="Delete" onclick="onDelete();" class="input2"/></li></ul>
</div>
<div class="table">
<span id=t0 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input0" id=c0 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k0></td></tr></table></a></div>
</span>
<span id=t1 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input1" id=c1 type="checkbox" value="" /></td> <td width="469" id=k1></td></tr></table></a></div>
</span>
<span id=t2 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input2" id=c2 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k2></td></tr></table></a></div>
</span>
<span id=t3 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input3" id=c3 type="checkbox" value="" /></td> <td width="469" id=k3></td></tr></table></a></div>
</span>
<span id=t4 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input4" id=c4 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k4></td></tr></table></a></div>
</span>
<span id=t5 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input5" id=c5 type="checkbox" value="" /></td> <td width="469" id=k5></td></tr></table></a></div>
</span>
<span id=t6 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input6" id=c6 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k6></td></tr></table></a></div>
</span>
<span id=t7 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input7" id=c7 type="checkbox" value="" /></td> <td width="469" id=k7></td></tr></table></a></div>
</span>
<span id=t8 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input8" id=c8 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k8></td></tr></table></a></div>
</span>
<span id=t9 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input9" id=c9 type="checkbox" value="" /></td> <td width="469" id=k9></td></tr></table></a></div>
</span>
<span id=t10 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input10" id=c10 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k10></td></tr></table></a></div>
</span>
<span id=t11 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input11" id=c11 type="checkbox" value="" /></td> <td width="469" id=k11></td></tr></table></a></div>
</span>
<span id=t12 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input12" id=c12 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k12></td></tr></table></a></div>
</span>
<span id=t13 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input13" id=c13 type="checkbox" value="" /></td> <td width="469" id=k13></td></tr></table></a></div>
</span>
<span id=t14 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input14" id=c14 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k14></td></tr></table></a></div>
</span>
<span id=t15 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input15" id=c15 type="checkbox" value="" /></td> <td width="469" id=k15></td></tr></table></a></div>
</span>
<span id=t16 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input16" id=c16 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k16></td></tr></table></a></div>
</span>
<span id=t17 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input17" id=c17 type="checkbox" value="" /></td> <td width="469" id=k17></td></tr></table></a></div>
</span>
<span id=t18 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input18" id=c18 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k18></td></tr></table></a></div>
</span>
<span id=t19 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input19" id=c19 type="checkbox" value="" /></td> <td width="469" id=k19></td></tr></table></a></div>
</span>
<span id=t20 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input20" id=c20 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k20></td></tr></table></a></div>
</span>
<span id=t21 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input21" id=c21 type="checkbox" value="" /></td> <td width="469" id=k21></td></tr></table></a></div>
</span>
<span id=t22 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input22" id=c22 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k22></td></tr></table></a></div>
</span>
<span id=t23 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input23" id=c23 type="checkbox" value="" /></td> <td width="469" id=k23></td></tr></table></a></div>
</span>
<span id=t24 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input24" id=c24 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k24></td></tr></table></a></div>
</span>
<span id=t25 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input25" id=c25 type="checkbox" value="" /></td> <td width="469" id=k25></td></tr></table></a></div>
</span>
<span id=t26 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input26" id=c26 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k26></td></tr></table></a></div>
</span>
<span id=t27 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input27" id=c27 type="checkbox" value="" /></td> <td width="469" id=k27></td></tr></table></a></div>
</span>
<span id=t28 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input28" id=c28 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k28></td></tr></table></a></div>
</span>
<span id=t29 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input29" id=c29 type="checkbox" value="" /></td> <td width="469" id=k29></td></tr></table></a></div>
</span>
<span id=t30 class="off">
<div class="list"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input30" id=c30 class="checkbox" type="checkbox" value="" /></td> <td width="469" id=k30></td></tr></table></a></div>
</span>
<span id=t31 class="off">
<div class="list2"><a href="#"><table width="100%" height="36" border="0" cellpadding="0" cellspacing="0"><tr> <td width="29"><input name="Input31" id=c31 type="checkbox" value="" /></td> <td width="469" id=k31></td></tr></table></a></div>
</span>
</div>

<span id="button_on" class="on">
<div class="button">
<ul>
<li><a href="empty.html" id="pcCancel">Cancel</a></li>
<li><a href="javascript: formCheck(); show_div(false, 'button_on'); show_div(true, 'button_off'); document.parental_control.submit();" id="pcApply">Apply</a></li>
<li><input type=hidden value="/local/parental_control.asp" name="epc_url"></li>
<li><input type=hidden value="" name="url_str"></li>
</ul></div>
</span>
<span id="button_off" class="off">
<div class="buttonX">
<ul>
<li><a id="pcCancelX">Cancel</a></li>
<li><a id="pcApplyX">Apply</a></li>
</ul></div>
</span>
</div>
</form>
</body>
<script>
update_table_value_first();
update_table();
</script>
</html>