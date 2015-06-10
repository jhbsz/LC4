<% do_pagehead1(); %>
<html>
    <head>
    <title><% nv_get("model"); %> - Firewall</title>
    <%do_headmeta();%>

    <%do_basic_css();%>
    <%do_custom_css();%>

    <%do_custom_js();%>
    <%do_basic_js();%>

    <script type="text/javascript">
   	                     
        function to_submit(F) {
            $("#submit_button").val("fw_basic");
            $("#action").val("Apply");

            open_waiting_window();            
            var ajaxData;
            ajaxData = ajax_data( $("#form").serializeArray() );
            do_ajax_action ("apply.cgi", "POST", "text", true, ajaxData, 3000);
       }
       

    </script>
    </head>

    <body>
    <div class="container">
        <%do_bootstrap_menu();%>
        <script type="text/javascript">create_waiting_window();</script>

        <form name="form" id="form" action="apply.cgi" method="post">
 	          <script type="text/javascript">do_hidden_elements('fw_rule');</script>
 	          <input type="hidden" name="fw_enable" value="1" />
                <h2><%lang("Security");%>-<%lang("Firewall");%></h2>
                
        <div class="row">
            <div class="span12">
                <legend><%lang("Firewall Protection");%></legend>                
                <div class="row show-grid">
                    <div class="span4"><%lang("SPI Firewall Protection");%></div>
                    <div class="span8">
                    	<label class="radio inline">
			                  <input type="radio" value="1" name="fw_state_enable" id="fw_state_enable" 
			                        <% nvg_attr_match("state_enable", "fw_rule", "0", "state_enable", "1", "checked"); %>><% lang("Enable"); %></input>
		                  </label>
		                  <label class="radio inline">
			                  <input type="radio" value="0" name="fw_state_enable" id="fw_state_enable" 
			                        <% nvg_attr_match("state_enable", "fw_rule", "0", "state_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                    	</label>

                    </div>
                </div>                
                <div class="row show-grid">
                    <div class="span4"><%lang("TCP SYN DoS Protection");%></div>
                    <div class="span8">
                    	<label class="radio inline">
			                  <input type="radio" value="1" name="fw_syn_enable"  id="fw_syn_enable"
			                        <% nvg_attr_match("syn_enable", "fw_rule", "0", "syn_enable", "1", "checked"); %>><% lang("Enable"); %></input>
											</label>
											<label class="radio inline">
			                  <input type="radio" value="0" name="fw_syn_enable" id="fw_syn_enable" 
			                        <% nvg_attr_match("syn_enable", "fw_rule", "0", "syn_enable", "0", "checked"); %>><% lang("Disable"); %></input>
											</label>
                    </div>
                </div>                               
                <div class="row show-grid">
                    <div class="span4"><%lang("ICMP Broadcasting Protection");%></div>
                    <div class="span8">
                    	<label class="radio inline">
												<input type="radio" value="1" name="fw_icmp_enable"  id="fw_icmp_enable"
	                        	<% nvg_attr_match("icmp_enable", "fw_rule", "0", "icmp_enable", "1", "checked"); %>><% lang("Enable"); %></input>
	                  	</label>
	                  	<label class="radio inline">
	                  		<input type="radio" value="0" name="fw_icmp_enable" id="fw_icmp_enable" 
	                        	<% nvg_attr_match("icmp_enable", "fw_rule", "0", "icmp_enable", "0", "checked"); %>><% lang("Disable"); %></input>
                    	</label>
                    </div>
                </div>                
                <div class="row show-grid">
                    <div class="span4"><%lang("ICMP Redirect Protection");%></div>
                    <div class="span8">
                    	<label class="radio inline">
												<input type="radio" value="1" name="fw_icmp_redir_enable" id="fw_icmp_redir_enable" 
                        	<%nvg_attr_match("icmp_redir_enable", "fw_rule", "0", "icmp_redir_enable", "1", "checked");%>><%lang("Enable");%>
                  			</input>
                  		</label>
                  		<label class="radio inline">
                  				<input type="radio" value="0" name="fw_icmp_redir_enable"  id="fw_icmp_redir_enable" 
                        	<%nvg_attr_match("icmp_redir_enable", "fw_rule", "0", "icmp_redir_enable", "0", "checked");%>><%lang("Disable");%>
                  			</input>
                  		</label>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Broadcast Storming");%></div>
                    <div class="span8">
                    	<label class="radio inline">
			                  <input type="radio" value="1" name="bc_storm_enable"  id="bc_storm_enable"
			                        <%nvg_attr_match("bc_storm_enable", "fw_rule", "0", "bc_storm_enable", "1", "checked");%>><%lang("Enable");%>
			                  </input>
			                </label>
			                <label class="radio inline">
			                  <input type="radio" value="0" name="bc_storm_enable" id="bc_storm_enable"
			                        <%nvg_attr_match("bc_storm_enable", "fw_rule", "0", "bc_storm_enable", "0", "checked");%>><%lang("Disable");%>
			                  </input>
											</label>
                    </div>
                </div>
								<input type="hidden" value="0" name="fw_rpfilter_enable" id="fw_rpfilter_enable"/>
            </div>
        </div><!-- row -->
        <div class="row">
            <div class="span12">
                <div class="submitFooter">
                    <input type="button" class="btn btn-primary" onClick="to_submit(this.form)" value="<%lang("Save Settings");%>">
                    <input type="reset" class="btn" value="<%lang("Cancel Changes");%>">
                </div>
            </div>
        </div><!-- row -->
        </form>
        <div class="row">
            <div class="span12">
                   <script type="text/javascript">
                        <%show_copyright();%>
                    </script>
            </div>
        </div><!-- row -->
    </div> <!-- /container -->

</body></html>