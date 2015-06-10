<% do_pagehead1(); %>      
<html>
<head>

<title><% nv_get("model"); %> - Login Page</title>
<script type="text/javascript" src="base64.js"></script>
<script type='text/javascript' src='common.js'></script>
<script type='text/javascript' src='code.js'></script>

<link type='text/css' rel='stylesheet' href='style/style.css' />
<link rel="stylesheet" type="text/css" href="style/stylelog.css"/>
<script type="text/javascript">

var F;
function to_submit(F) {
	F.action.value = "Login";
    var auth_value = base64encode(document.getElementById("user").value + ':' +
            document.getElementById("pw").value);
    setCookie("auth", auth_value);
    ajax_preaction(F);
//    display_mask(F);
}

function submitenter(F,e)
{
    var keycode;
    if (window.event) keycode = window.event.keyCode;
    else if (e) keycode = e.which;
    else return true;

    if (keycode == 13)
    {
        to_submit(F);
        return false;
    }
    else
        return true;
}
function init() {
    setCookie("auth", "");
    }
</script>

</head>

<body onload="init();">

<div id="wrapper11">
<table width="866" border="0" align="center">
   <tr>
     <td align="center" valign="top"><p>&nbsp; </p>
       <div id="wrapper11">
         <div id="layer01_holder">
           <div id="left"></div>
           <div id="center"></div>
           <div id="right"></div>
         </div>
         <div id="layer02_holder">
           <div id="left"></div>
           <div id="center"></div>
           <div id="right"></div>
         </div>
         <div id="layer03_holder">
           <div id="left"></div>
           <div id="center">
             <table width="100%" border="0" cellspacing="0" cellpadding="0">
              
               <tr>
                 <td><form id="form" name="form" method="post" >
	          <script type="text/javascript">do_hidden_elements('login_rule')</script>	
                   <label>Username
                     <input name="login_username" type="text" id="user" style="margin-top:3px;" onblur="valid_name(this,'User Name')" />
                   </label>
                   <label>Password
                     <input type="password" name="login_passwd" id="pw"
                        maxlength="32" style="margin-top:3px;" 
                        onKeyPress="return submitenter(this.form, event)" />
                   </label>
                   <label>
                     <input type="button" name="save_button" id="button" value="Login" onClick="to_submit(this.form)"/>
                   </label>
                 </form></td>
               </tr>
             </table>
           </div>
           <div id="right"></div>
         </div>
         <div id="layer04_holder">
           <div id="left"></div>
           <div id="center"></div>
           <div id="right"></div>
         </div>
         <div id="layer05_holder"></div>
       </div>
      </td>
   </tr>
</table>
</div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window()</script>
</body>
</html>
