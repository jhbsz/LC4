<script language="JavaScript">
dhcp_objects[$0] =
[
  { name: 'dhcp$0_start' },
  { name: 'dhcp$0_num' },
  { name: 'dhcp$0_lease' },
  { name: 'dhcp$0_domain' },
  { name: 'dhcp$0_dnstype' },
];

dhcpdnsaddr_objects[$0] =
[
  { name: 'dhcp$0_dnsaddr1'},
  { name: 'dhcp$0_dnsaddr2'},
];

// Function to submit DHCP data
function dhcpFormSubmit() {
	$.ajax({
	   url:        "apply.cgi",
	   type:       "POST",
	   dataType:   "text",
	   async:      false,
	   data:       { action:"Apply",
					 submit_button:"dhcp",
					 dhcp_rule:"",
					 dhcp$0_dnstype:$("#dhcp$0_dnstype").val(),
					 dhcp$0_dnsaddr1:$("#dhcp$0_dnsaddr1").val(),
					 dhcp$0_dnsaddr2:$("#dhcp$0_dnsaddr2").val(),
					 dhcp$0_lease:$("#dhcp$0_lease").val(),
					 dhcp$0_start:$("#dhcp$0_start").val(),
					 dhcp$0_enable:$('input[name=dhcp$0_enable]:checked').val(),
					 dhcp$0_num:$("#dhcp$0_num").val(),
					 dhcp$0_domain:$("#dhcp$0_domain").val(),				 		
					 dhcp$0_wins:$("dhcp$0_wins").val()
				   },
	   success: function(data){
			  show_mask();
			  //var msg = "Setting: " + '<%lang("succeeded");%>';
              var msg = "<%lang("The Wireless LAN connection will end due to changes in the network address setting. Establish a Wireless LAN connection.");%>";
			  display_return(-3, msg);
	   },
	   error: function(xhr, textStatus, error){
		   //console.log(xhr.statusText, textStatus, error);
	   }
	});
}
// End

$(document).ready(function(){
    var curLease = "<%nvg_attr_get("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease");%>";
    var DisplayLease = curLease / 60 /60;
    $("#dhcp$0_lease_display").val(DisplayLease+" <%lang("hours");%>");

    var curStartIP = "<%nvg_attr_get("dhcp$0_start", "lan_dhcps_rule", "$0", "start");%>";
    for (var i=2; i<246; i++ ){
        var IPRange = i + 9;
        if (curStartIP == i){
            $("#dhcp$0_start").append("<option value='"+i+"' selected>"+ i + "-" +IPRange+"</option>");
        } else {
            $("#dhcp$0_start").append("<option value='"+i+"'>"+ i + "-" +IPRange+"</option>");
        }
    }
});

</script>

  <div class="setting">
    <div class="label"><%lang("DNS server address");%></div>

        <select ID="dhcp$0_dnstype" name="dhcp$0_dnstype" onChange="SelDNSType($0)" 
                value='<%nvg_attr_get("dhcp$0_dnstype", "lan_dhcps_rule", "$0", "dnstype");%>'>
        <option value="dnsrelay" 
                <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule", "$0", "dnstype", "dnsrelay", "selected"); %>>
                <%lang("Auto");%></option>
        <option value="custom"
                <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype", "custom","selected"); %>>
                <%lang("Manual");%></option>
        </select>

  </div>
 <div class="setting">
    <div class="label">&nbsp;</div>
      DNS1: <input size="20" maxLength="20" ID="dhcp$0_dnsaddr1" name="dhcp$0_dnsaddr1" 
            value='<%nvg_attr_get("dhcp$0_dnsaddr1", "lan_dhcps_rule", "$0", "dnsaddr1");%>' 
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","dnsrelay","disabled"); %>
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","ispdns","disabled");%>
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","opendns","disabled");%>
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","googledns","disabled");%>>
 </div>
 <div class="setting">
    <div class="label">&nbsp;</div> 
      DNS2: <input size="20" maxLength="20" ID="dhcp$0_dnsaddr2" name="dhcp$0_dnsaddr2" 
            value='<%nvg_attr_get("dhcp$0_dnsaddr2", "lan_dhcps_rule", "$0", "dnsaddr2");%>'
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","dnsrelay","disabled");%>
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","ispdns","disabled");%>
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","opendns","disabled");%>
            <%nvg_attr_match("dhcp$0_dnstype","lan_dhcps_rule","$0","dnstype","googledns","disabled");%>>
 </div>

  <div class="setting">
    <div class="label"><%lang("Address lease period");%></div>
      <input type="hidden" id="dhcp$0_lease" name="dhcp$0_lease" value='<%nvg_attr_get("dhcp$0_lease", "lan_dhcps_rule", "$0", "lease");%>' />
      <input type="text" name="dhcp$0_lease_display" id="dhcp$0_lease_display" size="15" value='' style="border: 0px" readonly />
  </div>

  <div class="setting">
    <div class="label"><%lang("IP Address pool");%></div>
      
      <input type="text" name="dhcp$0_display_start" id="dhcp$0_display_start" size="9" value='<%show_ipaddr_prefix24("lan$0_ipaddr");%>' style="border: 0px" readonly />
      <select id="dhcp$0_start" name="dhcp$0_start" value='<%nvg_attr_get("dhcp$0_start", "lan_dhcps_rule", "$0", "start");%>' />
      </select>

  </div>

	<div style="display: none;">
  <div class="setting">
    <div class="label"><%lang("DHCP Service");%></div>
    <input type="radio" value="1" name="dhcp$0_enable" onclick="init()"
          <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","1","checked");%>><%lang("Enable");%>
    </input>

    <input type="radio" value="0" name="dhcp$0_enable" onclick="init()"
          <%nvg_attr_match("dhcp$0_enable","lan_dhcps_rule","$0","enable","0","checked");%>><%lang("Disable");%>
    </input>
  </div>
  
  <div class="setting">
    <div class="label"><%lang("Max DHCP Clients");%></div>
      <input size="3" maxLength="3" id="dhcp$0_num" name="dhcp$0_num" 
            value='<%nvg_attr_get("dhcp$0_num", "lan_dhcps_rule", "$0", "num");%>'
            onblur='valid_number(this, "Max DHCP Clients")' />
  </div>

 <div class="setting">
    <div class="label"><%lang("Domain");%></div>
      <input size="25" maxLength="20" id="dhcp$0_domain" name="dhcp$0_domain" 
            value='<%nvg_attr_get("dhcp$0_domain", "lan_dhcps_rule", "$0", "domain");%>' onblur="valid_name(this, 'Domain')" />
  </div>
	</div>

  <input type="hidden" id="dhcp$0_wins" name="dhcp$0_wins" value='<%nvg_attr_get("dhcp$0_wins", "lan_dhcps_rule", "$0", "wins");%>' />

<br />
