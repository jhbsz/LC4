<% do_pagehead1(); %>      
<html>
  <head>
    <title><% nv_get("model"); %> - Relay</title>
    <style type="text/css">
    .press{background-color:#00ff00}
    </style>
    <% do_pagehead2(); %>
    <script type="text/javascript">

    var box = ["sun","mon","tue","wed","thu","fri","sat"];
    var F;
    var weeks=<%als_get_week();%>;
    var hours=<%als_get_time();%>;
    
    function to_submit(F) {
    	var z , y;
    	var schedule = "";
    	
        F.submit_button.value = "relay";
        F.action.value = "Apply";
        
        if($("#relay_option").val() == 1)
        {
        	for (z = 1 ; z < $("#schedule_1_table tr").length;z++)
        	{
            		var table_td = $("#schedule_1_table tr").eq(z).find("td");
            		for (y = 1 ; y < table_td.length;y++)
            		{
            			if ( (table_td.eq(y - 1).attr("class") == "button" || table_td.eq(y - 1).attr("class") == "title")&& table_td.eq(y).attr("class") == "button press" && table_td.eq(y + 1).attr("class") == "button press")
            	    			schedule += y + "~";
            			else if (table_td.eq(y - 1).attr("class") == "button press" && table_td.eq(y).attr("class") == "button press" && (table_td.eq(y + 1).attr("class") == "button" || y == (table_td.length - 1 )))
            	    			schedule += y + ",";
            			else if ((table_td.eq(y - 1).attr("class") == "button" || table_td.eq(y - 1).attr("class") == "title") && table_td.eq(y).attr("class") == "button press" && (table_td.eq(y + 1).attr("class") == "button" || y == (table_td.length - 1 )))
            	    			schedule += y + "~" + y + ",";
    	    		}
            		$("[name='schedule_1_" + box[z - 1] +"']").val(schedule);
            		schedule = "";
        	}
        	for (z = 1 ; z < $("#schedule_2_table tr").length;z++)
        	{
            		var table_td = $("#schedule_2_table tr").eq(z).find("td");
            		for (y = 1 ; y < table_td.length;y++)
            		{
            			if ( (table_td.eq(y - 1).attr("class") == "button" || table_td.eq(y - 1).attr("class") == "title")&& table_td.eq(y).attr("class") == "button press" && table_td.eq(y + 1).attr("class") == "button press")
            	    			schedule += y + "~";
            			else if (table_td.eq(y - 1).attr("class") == "button press" && table_td.eq(y).attr("class") == "button press" && (table_td.eq(y + 1).attr("class") == "button" || y == (table_td.length - 1 )))
            	    			schedule += y + ",";
            			else if ((table_td.eq(y - 1).attr("class") == "button" || table_td.eq(y - 1).attr("class") == "title") && table_td.eq(y).attr("class") == "button press" && (table_td.eq(y + 1).attr("class") == "button" || y == (table_td.length - 1 )))
            	    			schedule += y + "~" + y + ",";
    	    		}
            		$("[name='schedule_2_" + box[z - 1] +"']").val(schedule);
            		schedule = "";
        	}
        	if($("#schedule_1_table tr").eq(weeks).find("td").eq(hours).attr("class") == "button press")
            		$("[name='relay_1_enable']").val("1");
        	else
            		$("[name='relay_1_enable']").val("0");
            		
        	if($("#schedule_2_table tr").eq(weeks).find("td").eq(hours).attr("class") == "button press")
            		$("[name='relay_2_enable']").val("1");
        	else
            		$("[name='relay_2_enable']").val("0");
        	
        }
        display_mask(F);
        ajax_preaction(F);
    }

    </script>
  </head>
  <body class="gui" >
    <div id="wrapper">
        <div id="header"> 
	    <script type="text/javascript">do_header_pre()</script>
        <% do_menu("Setup", "Relay"); %>
        <script type="text/javascript">do_header_post()</script>
        </div>
             
	  <script type="text/javascript">do_body_pre()</script>	
	  <script type="text/javascript">do_page_path('<%lang("Setup");%>','<%lang("Relay");%>')</script>
	  <script type="text/javascript">do_table_pre()</script>	
	  <script type="text/javascript">do_mainframe_pre()</script>	
          <div id="contents">
            <form name="form" action="apply.cgi" method="post">
	          <script type="text/javascript">do_hidden_elements('relay')</script>	
	          <input type="hidden" name="relay" value="1" />
              <fieldset>
                <legend><%lang("Status");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Relay 1");%></div>
                  <div class="label"><%nvg_attr_match("relay_1", "relay_rule", "0", "enable", "1",   "ON"); %><%nvg_attr_match("relay_1", "relay_rule", "0", "enable", "0",   "OFF"); %></div>
                </div>
                <div class="setting">
                  <div class="label"><%lang("Relay 2");%></div>
                  <div class="label"><%nvg_attr_match("", "relay_rule", "1", "enable", "1",   "ON");%><%nvg_attr_match("", "relay_rule", "1", "enable", "0",   "OFF"); %></div>
                </div>
              </fieldset>
              
              <fieldset>
                <legend><%lang("Control");%></legend>
                <div class="setting" align=center>
                  <div class="label">
                  	<input type="radio" name="relay" value="1" <%nvg_attr_match("", "relay_rule", "0", "option", "1", "checked"); %>><%lang("Schedule");%></input>
                  </div>
                  <div class="label">
                  	<input type="radio" name="relay" value="0" <%nvg_attr_match("", "relay_rule", "0", "option", "0", "checked"); %>><%lang("Manually");%></input>
                  </div>
                </div>
              </fieldset>

              <fieldset id="setting">
                <legend><%lang("Direct");%></legend>
                <div class="setting">
                  <div class="label"><%lang("Relay 1 :");%></div>
                  <div class="label">
                  	<input type="radio" name="relay_1" onclick="$('#relay_1_enable').val('1');" <%nvg_attr_match("", "relay_rule", "0", "enable", "1", "checked"); %>><%lang("ON");%></input>
                  	<input type="radio" name="relay_1" onclick="$('#relay_1_enable').val('0');" <%nvg_attr_match("", "relay_rule", "0", "enable", "0", "checked"); %>><%lang("OFF");%></input>
                  </div>
                </div>
                
                <div class="setting">
                  <div class="label"><%lang("Relay 2 :");%></div>
                  <div class="label">
                  	<input type="radio" name="relay_2" onclick="$('#relay_2_enable').val('1');" <%nvg_attr_match("", "relay_rule", "1", "enable", "1", "checked"); %>><%lang("ON");%></input>
                  	<input type="radio" name="relay_2" onclick="$('#relay_2_enable').val('0');" <%nvg_attr_match("", "relay_rule", "1", "enable", "0", "checked"); %>><%lang("OFF");%></input>
                  </div>
                </div>
                
                <input type="hidden" value="0" name="relay_enable" />
                <input type="hidden" value="<%nv_attr_get("relay_rule" ,"0", "option");%>" name="relay_option" id="relay_option"/>
                <input type="hidden" value="<%nv_attr_get("relay_rule" ,"0", "enable");%>" name="relay_1_enable" id="relay_1_enable"/>
                <input type="hidden" value="<%nv_attr_get("relay_rule" ,"1", "enable");%>" name="relay_2_enable" id="relay_2_enable"/>
               </fieldset>
              
              <fieldset id="schedule_1" >
                <legend><%lang(" Relay 1 Schedule");%></legend>
                <table border=1 id="schedule_1_table"></table>
                <input type="button" id="press_1" value="Select ALL" />
                <input type="button" id="clean_1" value="Clean ALL" />
                <input type="hidden" name="schedule_1_sun" value="<%nv_attr_get("relay_schedule_rule" ,"0", "sun");%>" />
                <input type="hidden" name="schedule_1_mon" value="<%nv_attr_get("relay_schedule_rule" ,"0", "mon");%>" />
                <input type="hidden" name="schedule_1_tue" value="<%nv_attr_get("relay_schedule_rule" ,"0", "tue");%>" />
                <input type="hidden" name="schedule_1_wed" value="<%nv_attr_get("relay_schedule_rule" ,"0", "wed");%>" />
                <input type="hidden" name="schedule_1_thu" value="<%nv_attr_get("relay_schedule_rule" ,"0", "thu");%>" />
                <input type="hidden" name="schedule_1_fri" value="<%nv_attr_get("relay_schedule_rule" ,"0", "fri");%>" />
                <input type="hidden" name="schedule_1_sat" value="<%nv_attr_get("relay_schedule_rule" ,"0", "sat");%>" />
              </fieldset><br/>
              
              <fieldset id="schedule_2" >
                <legend><%lang(" Relay 2 Schedule");%></legend>
                <table border=1 id="schedule_2_table"></table>
                <input type="button" id="press_2" value="Select ALL" />
                <input type="button" id="clean_2" value="Clean ALL" />
                <input type="hidden" name="schedule_2_sun" value="<%nv_attr_get("relay_schedule_rule" ,"1", "sun");%>" />
                <input type="hidden" name="schedule_2_mon" value="<%nv_attr_get("relay_schedule_rule" ,"1", "mon");%>" />
                <input type="hidden" name="schedule_2_tue" value="<%nv_attr_get("relay_schedule_rule" ,"1", "tue");%>" />
                <input type="hidden" name="schedule_2_wed" value="<%nv_attr_get("relay_schedule_rule" ,"1", "wed");%>" />
                <input type="hidden" name="schedule_2_thu" value="<%nv_attr_get("relay_schedule_rule" ,"1", "thu");%>" />
                <input type="hidden" name="schedule_2_fri" value="<%nv_attr_get("relay_schedule_rule" ,"1", "fri");%>" />
                <input type="hidden" name="schedule_2_sat" value="<%nv_attr_get("relay_schedule_rule" ,"1", "sat");%>" />
              </fieldset><br/>

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick='to_submit(this.form)' />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload();" />
              </div>
            </form>
        </div>
	  <script type="text/javascript">do_mainframe_post()</script>	
      <%widget_show();%>
	  <script type="text/javascript">do_table_post()</script>	
	  <script type="text/javascript">do_body_post()</script>	
	  <script type="text/javascript">do_others()</script>	
      <div id="floatKiller"></div>
    </div>
    <script type="text/javascript">create_background_cover()</script>
    <script type="text/javascript">create_waiting_window()</script>
    <script type="text/javascript">create_return_window()</script>
    <script type="text/javascript">
        var g,y,z;
        var weekly = ["","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"];
        var time = ["00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23"];
    	var box = ["sun","mon","tue","wed","thu","fri","sat"];
        var schedule_table = "";
	schedule_table += "<tr><td></td>";
	for (z = 0 ; z < time.length;z++)	
	    schedule_table += "<td colspan=2 align=center>" + time[z] + "</td>";
	schedule_table += "</tr>";
   	for(g = 1;g < 8;g++)
   	{ 
	    schedule_table += "<tr align=center><td class=title >";
	    schedule_table += weekly[g];
	    schedule_table += "</td>";
   	    for(y = 0;y < 48;y++)
   	    {
	        schedule_table += "<td class=button >";
	        schedule_table += "</td>";
   	    }
	    schedule_table += "</tr>";
    }
        $("#schedule_1_table").append(schedule_table);
        $("#schedule_2_table").append(schedule_table);
        
        $(function(){

        F = document.getElementsByName("form")[0];
        <%widget_start();%>
            creat_table("1");
            creat_table("2")
        
       	    $("#press_1").click(function(){
       	        $("#schedule_1_table td[class='button']").addClass("press");
       	    });
	        
       	    $("#clean_1").click(function(){
       	        $("#schedule_1_table td[class='button press']").removeClass("press");
            });

       	    $("#press_2").click(function(){
       	        $("#schedule_2_table td[class='button']").addClass("press");
       	    });
	        
       	    $("#clean_2").click(function(){
       	        $("#schedule_2_table td[class='button press']").removeClass("press");
            });
            
            var button_even = function(){
                if ($(this).attr("class") == "button press" )                                                                      
                    $(this).removeClass("press");                                                                                  
                else                                                                                                               
                    $(this).addClass("press");                                                                                     
            };

        var relay_even = function(){
          	$("#relay_option").val($(this).val());
                $("[class^='button']").unbind();
                if ($("#relay_option").val() == 1){
                    $("#setting input").attr("disabled",true);
                    $("#schedule_1 input").attr("disabled",false);
                    $("#schedule_2 input").attr("disabled",false);
                    $("[class^='button']").bind("click",button_even);                                                                          
                }else if ($("#relay_option").val() == 0){
                    $("#setting input").attr("disabled",false);
                    $("#schedule_1 input").attr("disabled",true);
                    $("#schedule_2 input").attr("disabled",true);
                }
            }
            $("input[name='relay']").bind("click",relay_even);
            $("input[name='relay'][value='<%nv_attr_get("relay_rule" ,"0", "option");%>']").click(); 
        }); 

        function creat_table(i){
            var w , y , z;
            var table_tr = $("#schedule_" + i + "_table tr");
            for (w = 0; w < box.length;w++)
            {
                var schedule = $("[name='schedule_" + i + "_" + box[w] +"']").val();
                if( schedule ){
                var hour = schedule.split(",");
                var table_td = table_tr.eq((w + 1));
                for(y = 0;y < hour.length;y++)
                {
                    var press = hour[y].split("~");
                    for ( z = Number(press[0]); z <= Number(press[1]); z++)
                    {
                   	var grid = table_td.find("td").eq(z);
                    	grid.addClass("press");
            	    }
                }
                }
            }
        }
</script>
  </body>
</html>
