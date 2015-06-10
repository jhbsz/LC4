	<script type="text/javascript">
		// Function to submit LAN data
		function lanFormSubmit() {
			$.ajax({
			   url:        "apply.cgi",
			   type:       "POST",
			   dataType:   "text",
			   async:      false,
			   data:       { action:"Apply",
							 submit_button:"lan",
							 lan_rule:"",
							 lan$0_enable:$("#lan$0_enable").val(),
							 lan$0_static_ipaddr:$("#lan$0_static_ipaddr").val(),
							 lan$0_static_mask:$("#lan$0_static_mask").val(),
							 lan$0_static_gateway:$("#lan$0_static_gateway").val(),
							 lan$0_static_dns:$("#lan$0_static_dns").val(),
							 lan$0_static_wins:$("#lan$0_static_wins").val(),
							 lan$0_stp:$('input[name=lan$0_stp]:checked').val(),
							 lan$0_mtu:$("#lan$0_mtu").val(),
							 lan$0_weight:$("#lan$0_weight").val()					 		
						   },
			   success: function(data){
					  show_mask();
					  var msg = "Lan Setting: " + '<%lang("succeeded");%>';
					  //display_return(-3, msg);
			   },
			   error: function(xhr, textStatus, error){
				   //console.log(xhr.statusText, textStatus, error);
			   }
			});
		}
		// End
$(document).ready(function(){
    var curIP = "<%nv_attr_get("lan_static_rule", "$0", "ipaddr");%>";
    //var IPArr = curIP.splite
    var IPArr = curIP.split(".");
    for (var i=0; i<255 ; i++ ){
        if (IPArr[2]==i) {
            $("#lan$0_static_ipclass").append("<option value='"+i+"' selected>"+i+"</option>");
        } else {
            $("#lan$0_static_ipclass").append("<option value='"+i+"'>"+i+"</option>");
        }
    }

    $("#lan$0_static_ipclass").change(function() {
        $("#lan$0_static_ipaddr").val("192.168." + $("#lan$0_static_ipclass").val() + ".1");
        $("#lan$0_static_ipaddr_t").val("192.168." + $("#lan$0_static_ipclass").val() + ".1");
        $("#dhcp$0_display_start").val("192.168." + $("#lan$0_static_ipclass").val() + ".");
    });

});

	</script>
  
  <input type="hidden" value="1" id="lan$0_enable" name="lan$0_enable" />
    <div class="setting">
      <div class="label"><font id="ip_addr"><%lang("Network address");%></font></div>
      <input type="hidden" maxLength="15" size="16" id="lan$0_static_ipaddr" name="lan$0_static_ipaddr"
          value='<%nv_attr_get("lan_static_rule", "$0", "ipaddr");%>' 
          onblur='valid_ipaddr(this, "Network address")'/>
        
        192.168.
        <select name="lan$0_static_ipclass" id="lan$0_static_ipclass">
        </select>
        .1

    </div>
  
      <div class="setting">
      <div class="label"><font id="mask"><%lang("Subnet Mask");%></font></div>
	  <input type="hidden" id="lan$0_static_mask" name="lan$0_static_mask" value="24">
      <input type="text" maxLength="15" size="16" value='255.255.255.0' style="border: 0px" readonly />
      </div>
    <input type="hidden" id="lan$0_static_gateway" name="lan$0_static_gateway" value='<%nv_attr_get("lan_static_rule", "$0", "gateway");%>' />
    <input type="hidden" id="lan$0_static_dns" name="lan$0_static_dns" value='<%nv_attr_get("lan_static_rule", "$0", "dns");%>' />
    <input type="hidden" id="lan$0_static_wins" name="lan$0_static_wins" value='<%nv_attr_get("lan_static_rule", "$0", "wins");%>' />

	<!-- Here to show default gateway -->
    <div class="setting">
      <div class="label"><font id="gateway"><%lang("Default Gateway");%></font></div>
      <input id="lan$0_static_ipaddr_t" type="text" maxLength="15" size="16" name="lan$0_static_ipaddr_t" style="border: 0px"
          value='<%nv_attr_get("lan_static_rule", "$0", "ipaddr");%>' readonly />
    </div>
	<!-- End -->

  <div style="display: none;">
  <div class="setting">
    <div class="label"> <%lang("Spanning Tree Protocol");%> (STP) </div>
    <input type="radio" value="1" name="lan$0_stp"
        <%nvg_attr_match("lan$0_stp","lan_main_rule","$0","stp","1","checked");%>><%lang("Enable");%>
    <input type="radio" value="0" name="lan$0_stp"
        <%nvg_attr_match("lan$0_stp","lan_main_rule","$0","stp","0","checked");%>><%lang("Disable");%>
  </div>

  <div class="setting">
    <div class="label"><%lang("MTU");%></div>
      <input size="6" maxLength="4" id="lan$0_mtu" name="lan$0_mtu" 
            value='<%nvg_attr_get("lan$0_mtu", "lan_main_rule", "$0", "mtu");%>'
            onblur='valid_number(this, "MTU")' /> <%lang("Bytes");%>
  </div>
  </div>

  <input type="hidden" id="lan$0_weight" name="lan$0_weight" value="1"/>

<br />
