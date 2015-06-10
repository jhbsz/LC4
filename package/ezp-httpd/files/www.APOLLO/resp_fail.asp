<% do_pagehead1(); %>
<html>
<head>
    <% do_pagehead2(); %>
<script language="JavaScript">
var submit_button = '<% show_next_page(); %>';
function to_submit()
{
		document.location.href = 'ad_basic.asp';

}

function rstyle()
{

    document.getElementById('return-window').style.left=get_left_offset(300);

}

</script>

<style>
#fail-wrapper {
    position:relative;
    text-align:center;
    width:100%;
    height:100%;
    display:block;
}
</style>

</head>
   <body onload='rstyle();'>
   
   
        <div id='fail-wrapper'>
        
            <div id='return-window-wrapper' style='display:block;'></div>
               
            <div id='return-window'>
                <div class='widgetInfo'>
                    <br/>
                    <div id='return-window-message'>
                        <div class="return-msg-img-div">
                            <img src="style/proxi/images/error.png" />
                        </div>
                        <%lang("Invalid config file format.");%><BR/>
                        <%lang("Please try again.");%>
                    </div>
                    
                        
                    <br/><% show_valid_msg(); %>
                    
                    <input type="button" name="action" value="<%lang("Continue");%>" OnClick=to_submit() />
                    <br/><br/>
                </div>
            </div>
        </div>
    
    </body>
</html>
