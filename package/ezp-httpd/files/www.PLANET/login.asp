<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - Login</title>
<% do_pagehead2(); %>
<script type="text/javascript" src="base64.js"></script>
<script type="text/javascript">

var F;

function setCookie(c_name,value,expiredays)
{
    var exdate=new Date();
    exdate.setDate(exdate.getDate()+expiredays);
    document.cookie=c_name+ "=" +escape(value)+
        ((expiredays==null) ? "" : ";expires="+exdate.toUTCString());
}

function setLanguage(lang){
  /*  setCookie("language", lang);*/
}

function to_submit(F) {
    F.submit_button.value = 'login';
	F.action.value = "Login";
    var auth_value = base64encode(document.getElementById("user").value + ':' +
            document.getElementById("pw").value);
    setCookie("auth", auth_value);
    ajax_preaction(F);
    display_mask(F);
}

function init() {
 /*   setCookie("language", "EN"); */
    setCookie("auth", "");
    }

</script>
</head>
  <body class="gui" onload="init();">
    <div id="wrapper">
        <div id="header">
	      <script type="text/javascript">do_header_pre()</script>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contentsInfo">

            <form name="form" method="post">
	          <script type="text/javascript">do_hidden_elements('login_rule')</script>	

  <fieldset>
    <legend><%lang("Login");%></legend> 

     <div class="setting">
        <div class="label">
          <%lang("User Name");%>
        </div><input id="user" name="login_username" size="25" maxlength=
        "32" onblur="valid_name(this,'User Name')">
      </div>

      <div class="setting">
        <div class="label">
          <%lang("Password");%>
        </div><input type="password" id="pw" name="login_passwd" size="25"
        maxlength="32">
      </div>

           <div class="setting">
                <div class="label">
                  <%lang("Language");%>
                </div><select name="lang" onchange="setLanguage(this.value)" >
                  <option value=
                  "EN" <% nvg_match("lang", "EN", "selected"); %>>
                  English
                  </option>

                  <option value=
                  "TW" <% nvg_match("lang", "TW", "selected"); %>>
                   繁體中文
                  </option>
                  <option value=
                  "GE" <% nvg_match("lang", "GE", "selected"); %>>
                  Deutsch 
                  </option>
                  <option value=
                  "GB" <% nvg_match("lang", "GB", "selected"); %>>
                  簡體中文
                  </option>
                </select>
             </div>
    </fieldset>
        <br><br>

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Login");%>" onClick="to_submit(this.form)" />
              </div>
            </form>
          </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
    </div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window()</script>
  </body>
</html>
