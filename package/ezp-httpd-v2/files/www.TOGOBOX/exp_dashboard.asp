
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<title><% nv_get("model"); %> - Dashboard</title>
        
        <%do_headmeta();%>
        <%do_basic_css();%>

        <%do_ace_font_css();%>
        <%do_ace_font_ie7_css();%>

        <!-- page specific plugin styles -->

        <!-- ace styles -->
        <%do_ace_basic_css();%>
        <%do_ace_basic_ie9_css();%>
        <%do_custom_css();%>

        <link href='css/fontcustom.css' rel='stylesheet' type='text/css'>
        <%do_custom_js();%>
        <%do_basic_js();%>

        <!-- page specific plugin scripts -->
        
        <!--[if lt IE 9]>
        <script type="text/javascript" src="js/excanvas.min.js"></script>
        <![endif]-->

        <%do_jquery_ui_js();%>
        <%do_jquery_other_js();%>
        <%do_jquery_flot_js();%>
        <%do_datatables_js();%>
        <%do_ace_basic_js();%>

		
    <script type="text/javascript">

    var updateInterval = 6000;
    var now = new Date().getTime();
    var num = <%nv_get("wan_num");%>;
    var rx = new Array(num);
    var tx = new Array(num);
    var rx_max = new Array(num);
    var tx_max = new Array(num);
    /* For short term, this parameter applied for lan(br0), so we keep it with a
     * number instead of an array. */
    var thp_tx = 0;
    var thp_rx = 0;
    var thp_timestamp = 0;
    var thp_last_timestamp = 0;
    var USB_usage, SD_usage;
        $(function() {
        	$('.dialogs,.comments').slimScroll({
                height: '300px'
            });
        	
        	$('#tasks').sortable();
        	$('#tasks').disableSelection();
        	$('#tasks input:checkbox').removeAttr('checked').on('click', function(){
        		if(this.checked) $(this).closest('li').addClass('selected');
        		else $(this).closest('li').removeClass('selected');
        	});
        
        	var oldie = $.browser.msie && $.browser.version < 9;
        	$('.easy-pie-chart.percentage').each(function(){
        		var $box = $(this).closest('.infobox');
        		var barColor = $(this).data('color') || (!$box.hasClass('infobox-dark') ? $box.css('color') : 'rgba(255,255,255,0.95)');
        		var trackColor = barColor == 'rgba(255,255,255,0.95)' ? 'rgba(255,255,255,0.25)' : '#E2E2E2';
        		var size = parseInt($(this).data('size')) || 50;
        		$(this).easyPieChart({
        			barColor: barColor,
        			trackColor: trackColor,
        			scaleColor: false,
        			lineCap: 'butt',
        			lineWidth: parseInt(size/10),
        			animate: oldie ? false : 1000,
        			size: size
        		});
        	});
        
        	$('.sparkline').each(function(){
        		var $box = $(this).closest('.infobox');
        		var barColor = !$box.hasClass('infobox-dark') ? $box.css('color') : '#FFF';
        		$(this).sparkline('html', {tagValuesAttribute:'data-values', type: 'bar', barColor: barColor , chartRangeMin:$(this).data('min') || 0} );
        	});
        	
            F = document.getElementsByName("form")[0];        
            <%widget_start();%>
            var tx_data = [], rx_data = [], totalPoints = 120;
            <%st_mrtg_show("graph", "thp");%>
            var stats_ifname = '<% nvg_get("stats_ifname"); %>';
            var h = mrtg_data[stats_ifname];
            if(typeof mrtg_data[stats_ifname] == "undefined") {
                h = [];
                for(var i = 0;i < 120 ; i++)
                    h.push(0);
            }
            function getRandomData() {    
                var now = new Date().getTime();
                var res = [];
                for (var i = 0; i < h.points_0.length; ++i)
                    res.push([now - 60000*(h.points_0.length - i - 1), h.points_0[i] / 1000]);
                return res;
            }
            function getRxRandomData() {    
                var now = new Date().getTime();
                var res = [];
                for (var i = 0; i < h.points_1.length; ++i)
                    res.push([now - 60000*(h.points_1.length - i - 1), h.points_1[i] / 1000]);
                return res;
            }
           

            // setup plot
            var options = {
                series: { shadowSize: 0 }, // drawing is faster without shadows
                yaxis: { min: 0,
                        axisLabel: "KB/s",
                        axisLabelUseCanvas: true,
                        axisLabelFontSizePixels: 12,
                        axisLabelFontFamily: 'Verdana, Arial',
                        axisLabelPadding: 20 
                },
                xaxis: {
                    mode: "time",
                    tickSize: [1, "minute"],
                    tickFormatter: function (v, axis) {
                        var date = new Date(v);
                        if (date.getMinutes() % 10 == 0) {
                            var hours = date.getHours() < 10 ? "0" + date.getHours() : date.getHours();
                            var minutes = date.getMinutes() < 10 ? "0" + date.getMinutes() : date.getMinutes();
                            var seconds = date.getSeconds() < 10 ? "0" + date.getSeconds() : date.getSeconds();
                            return hours + ":" + minutes;
                        } else {
                            return "";
                        }
                    },
                    axisLabel: "<% lang("Time"); %>", 
                    axisLabelUseCanvas: true, 
                    axisLabelFontSizePixels: 12, 
                    axisLabelFontFamily: 'Verdana, Arial', 
                    axisLabelPadding: 10
               }
            };
            dataset = [
                {label: '<%lang("Upload");%>', data :getRandomData()}, 
                {label: '<%lang("Download");%>', data:getRxRandomData()}
            ];
 

            var plot = $.plot($("#placeholder"),  dataset, options);
     
        
            function update() {
                if(thp_last_timestamp != thp_timestamp) {
                    var h = mrtg_data[stats_ifname];
                    thp_last_timestamp = thp_timestamp;
                    for(var xxx = 0;xxx < (h.points_0.length - 1) ; xxx++) {
                        h.points_0[xxx] = h.points_0[xxx + 1];
                        h.points_1[xxx] = h.points_1[xxx + 1];
                    }
                    h.points_0[xxx] = thp_rx;
                    h.points_1[xxx] = thp_tx;
                }
                $.plot($("#placeholder"), [ {label:'<%lang("Upload");%>',data:getRandomData()}, {label:'<%lang("Download");%>',data:getRxRandomData()}], options);
				
				var progress = $('#memory').attr("data-percent");
                $('#memory').data('easyPieChart').update(parseInt(progress, 10));
				$('#memory').find('span').text(progress);

                var cpu_usage = $('#cpu_usage').attr("data-percent");
                $('#cpu_usage').data('easyPieChart').update(parseInt(cpu_usage, 10));
                $('#cpu_usage').find('span').text(cpu_usage);
				
                var usb_usage = $('#usb_usage').attr("data-percent");
                if(usb_usage == "N/A") {
                    $('#usb_usage').find('span').text(usb_usage);
                    $('#usb_link').removeAttr("href");
                }else {
                    if(usb_usage == undefined){
                        usb_usage = 0;
                        $('#usb_link').removeAttr("href");
                    }else {
                        var ip = "http://"+"<% nv_get("lan0_ipaddr"); %>"+":8081/USB/";
                        $('#usb_link').attr("href", ip);
                    }
                    $('#usb_usage').find('span').text(usb_usage+"%");
                }
                $('#usb_usage').data('easyPieChart').update(parseInt(usb_usage, 10));

                var sd_usage = $('#sd_usage').attr("data-percent");
                if(sd_usage == "N/A") {
                    $('#sd_usage').find('span').text(sd_usage);
                    $('#sd_link').removeAttr("href");
                }else {
                    if(sd_usage == undefined){
                        sd_usage = 0;
                        $('#sd_link').removeAttr("href");
                    }else {
                        var ip = "http://"+"<% nv_get("lan0_ipaddr"); %>"+":8081/SD/";
                        $('#sd_link').attr("href", ip);
                    }
                    $('#sd_usage').find('span').text(sd_usage+"%");
                }
                $('#sd_usage').data('easyPieChart').update(parseInt(sd_usage, 10));
                setTimeout(update, updateInterval);
            }	
            update();
        });

    function widget_postaction(data) {
	    result = data;
	    var ret = result.split('\n');
	    var memory = '';
	    var usb = '';

	    for (i = 0; i < num; i++) {
		    /* Clean up the array in case no proper value returns.
		     * It happens when changing WAN proto to wwan from other
		     * protocols.
		     */
		    rx[i] = 0;
		    tx[i] = 0;
		    rx_max[i] = 0;
		    tx_max[i] = 0;
    	}

    	var i, j;
	    var wwan_on = 1;
	    for (i = 0; i < ret.length; i++) {
	    	var pair = ret[i].split('='); 
		    for (j = 0; j < <%nv_get("wan_num");%>; j++) {
			
			    if (pair[0] == 'wan'+j+'_ipaddr') {
                    $("#wan"+j+"_ipaddr").innerHTML = pair[1];
			    } else if (pair[0] == 'vpn'+j+'_ipaddr') {
                    $("#vpn"+j+"ipaddr").innerHTML = pair[1];
			    } else if (pair[0] == "Rx"+j) {
				    $("#rx_"+j).html(x(pair[1]));
				    rx[j] = pair[1];
			    } else if (pair[0] == "Tx"+j) {
                    $("#tx_"+j).html(x(pair[1]));
                    tx[j] = pair[1];
			    } else if (pair[0] == "Rx_Max"+j) {
                    $("#rx_max").html(x(pair[1]));
                    rx_max[j] = pair[1];
                } else if (pair[0] == "Tx_Max"+j) {
                    $("#tx_max").html(pair[1]);
                    tx_max[j] = pair[1];
                } 
            }
            if (pair[0] == "Memory") {
                var memory = pair[1];			
                $("#memory").attr("data-percent", memory);
            } else if (pair[0] == "dhcp_lease_num") {		
                var dhcp_lease_num = pair[1];
                $("#dhcp_lease_num").html(dhcp_lease_num);
            } else if (pair[0] == "CPU_idle") {
                var cpu_usage = pair[1];
                $("#cpu_usage").attr("data-percent", cpu_usage);
            } else if (pair[0] == "last_thp_timestamp") {
                thp_timestamp = pair[1];
            } else if (pair[0] == "last_thp_rx") {
                thp_rx = pair[1];
            } else if (pair[0] == "last_thp_tx") {
                thp_tx = pair[1];
            } else if (pair[0] == "Battery") {
                var battery = pair[1] + "%";
                $("#battery_status").html(battery);
                if(pair[1] < 20) {
                    $("#navbar_battery").attr("style", "display:inline;background-color: rgb(255,255,255);");
                    $("#nav_battery").html(battery);
                } else if(pair[1] >= 20){
                    $("#navbar_battery").attr("style", "display:none;");
                }
            } else if (pair[0] == "charge_status") {
                var charge = pair[1];
                if(charge == "Charging") {
                    charge = '<%lang("Charging");%>';
                } else if(charge == "Not Charging") {
                    charge = '<%lang("Not Charging");%>';
                }
                $("#charge_status").html(charge);
            } else if (pair[0] == "SD_remaining") {
                var SD_remaining_status = pair[1];
                if (SD_remaining_status == "&nbsp;") {
                    $("#sd_box").html("Not Available");
                } else {
                    var SD_remaining = pair[1] + "<%lang("remaining");%>";
                    $("#sd_box").html(SD_remaining);
                }
            } else if (pair[0] == "SD") {
                var SD = pair[1];
		        if(SD == "Not Available") {
                        SD = '<%lang("Not Available");%>';
                } else if(SD == "Read Only") {
                        SD = '<%lang("Read Only");%>';      
                } else if(SD == "Read/Write") {
                        SD = '<%lang("Read/Write");%>';  
		        }	
                $("#sd").html(SD);
            } else if (pair[0] == "USB_remaining") {
                var USB_remaining_status = pair[1];
                if (USB_remaining_status == "&nbsp;") {
                    $("#usb_box").html("Not Available");
                } else {
                var USB_remaining = pair[1] + "<%lang("remaining");%>";
                $("#usb_box").html(USB_remaining);
                }
            } else if (pair[0] == "USB") {
                var USB = pair[1];
		        if(USB == "Not Available") {
                        USB = '<%lang("Not Available");%>';
                } else if(USB == "Read Only") {
                        USB = '<%lang("Read Only");%>';
                } else if(USB == "Read/Write") {
                        USB = '<%lang("Read/Write");%>';
		        }	
                $("#usb").html(USB);
            } else if (pair[0] == "connection_type") {
                var swap_proto = pair[1];
                if(swap_proto == "Wired Internet (DHCP)")	{
                    $("#tr_ssid").attr("style", "display:none;");
        	        swap_proto = '<%lang("Wired Internet");%> (<%lang("DHCP");%>)';
                } else if(swap_proto == "Wired Internet (PPPoE)") {
                    $("#tr_ssid").attr("style", "display:none;");
                    swap_proto = '<%lang("Wired Internet");%> (<%lang("PPPoE");%>)';	
                } else if(swap_proto == "Wired Internet (Static IP)") {
                    $("#tr_ssid").attr("style", "display:none;");
                    swap_proto = '<%lang("Wired Internet");%> (<%lang("Static IP");%>)';
                } else if(swap_proto == "Wireless Internet") {
                    var result_style = document.getElementById('tr_ssid').style;
                    result_style.display = 'table-row';
                    swap_proto = '<%lang("Wireless Internet");%>';
                } else if(swap_proto == "USB Internet (Apple iPhone)") {
                    $("#tr_ssid").attr("style", "display:none;");
                    swap_proto = '<%lang("USB Internet");%> (<%lang("Apple iPhone");%>)';	
                } else if(swap_proto == "USB Internet (Android phone)") {
                    $("#tr_ssid").attr("style", "display:none;");
                    swap_proto = '<%lang("USB Internet");%> (<%lang("Android phone");%>)';	
                } else if(swap_proto == "USB Internet (3G/4G Mobile Internet)") {
                    $("#tr_ssid").attr("style", "display:none;");
                    swap_proto = '<%lang("USB Internet");%> (<%lnag("3G/4G Mobile Interner");%>)';	       
                }
                $("#connection_type").html(swap_proto);
            } else if (pair[0] == "link_state") {
                var link_state = pair[1];
                if(link_state == "0") {
                    link_state = '<%lang("Disconnected");%>';
                    $("#ip_addr").html(" ");
                } else if(link_state == "4") {
                    link_state = '<%lang("Connected");%>';
                } else if(link_state == "-1") {
                    link_state = '<%lang("Connection Failed");%>';
                } else {
                    link_state = '<%lang("Connecting");%>';
                }
                $("#link_state").html(link_state);
            } else if (pair[0] == "ip_addr") {
                var ip_addr = pair[1];
                if(ip_addr == "(null)") {
                    ip_addr = "";
                } else {
                    $("#ip_addr").html(ip_addr);
                }
            } else if (pair[0] == "ssid") {
                var ssid = html_convert_char(pair[1]);
                $("#ssid").html(ssid);
            } else if (pair[0] == "model") {
                var model = pair[1];
                var lic = <% nvg_get("license_invalid"); %>;
                if(lic == 1) {
                    $("#model").html(model + " develop");
                    $("#model").attr("style", "color:red;");
                }
                else {
                    $("#model").html(model);
                }
            } else if (pair[0] == "fw_version") {
                var fw_version = pair[1];
                $("#fw_version").html(fw_version);
            } else if (pair[0] == "up_time") {
                var up_time = pair[1];
                $("#up_time").html(up_time);
            } else if (pair[0] == "USB_usage") {
                USB_usage = pair[1];
                $("#usb_usage").attr("data-percent", USB_usage);
                if(USB_usage == "N/A") {
                    USB_usage = -1;
                }
            } else if (pair[0] == "SD_usage") {
                SD_usage = pair[1];
                $("#sd_usage").attr("data-percent", SD_usage);
                if(SD_usage == "N/A") {
                    SD_usage = -1;
                }
            }
	}//End for
        if((USB_usage > 95) || (SD_usage > 95)) {
            var storage;
            if(SD_usage >= USB_usage) {
                storage = SD_usage + "%";
            } else {
                storage = USB_usage + "%";
            }
            $("#navbar_storage").attr("style", "display:inline;background-color: rgb(255,255,255);");
            $("#nav_storage").html(storage);
        } else if((USB_usage <= 95) && (SD_usage <= 95)){
            $("#navbar_storage").attr("style", "display:none;");
        }

        if(USB_usage >= 0 || SD_usage >= 0) {
            $('#3').attr("style","display:inline;");
        } else if(USB_usage == -1 && SD_usage == -1){
            $('#3').attr("style","display:none;");
        }

        var hour = (new Date()).getHours();
        var minute = (new Date()).getMinutes();
        if(hour < 10) {
            hour = "0" + hour;
        }
        if(minute < 10) {
            minute = "0" + minute;
        }
        var t = hour + ":" + minute;
        $("#cur_time").html(t);

	    setTimeout('widget_query(F);', 5000);
    }
    </script>

	</head>

	<body>
        <%do_bootstrap_navbar_menu();%>
        <form name="form" id="form" action="apply.cgi" method="post">
		<div class="main-container container-fluid" id="main-container">
        <a id="menu-toggler" class="menu-toggler" href="#"><span class="menu-text"></span></a>
			<div id="sidebar" class="sidebar">
                <script type="text/javascript">
                    try{ace.settings.check('sidebar' , 'fixed')}catch(e){}
                </script>
                <%do_bootstrap_menu();%>
				<div id="sidebar-collapse" class="sidebar-collapse"><i class="icon-double-angle-left" data-icon1="icon-double-angle-left" data-icon2="icon-double-angle-right" ></i></div>
                <script type="text/javascript">
                    try{ace.settings.check('sidebar' , 'collapsed')}catch(e){}
                </script>
			</div><!--/#sidebar-->		
			<div id="main-content" class="main-content">					
				<div id="breadcrumbs" class="breadcrumbs">
					<ul class="breadcrumb">
						<li><i class="icon-home"></i> <%lang("Home");%><span class="divider"><i class="icon-angle-right"></i></span></li>
						<li class="active"><%lang("Dashboard");%></li>
					</ul><!--.breadcrumb-->
				</div><!--breadcrumbs-->
				<div id="page-content" class="page-content">					
					<div class="page-header position-relative">
						<h1><%lang("Dashboard");%> <small><i class="icon-double-angle-right"></i> </small></h1>
					</div><!--/page-header-->						
					<div class="row-fluid">

<div class="space6"></div>
<div class="row-fluid">

                       <div class="space6"></div>
                        <div class="row-fluid">                        
                            <div class="span12 infobox-container">                           
                            	<div class="infobox infobox-green">
                            		<div class="infobox-progress">
                            			<div class="easy-pie-chart percentage" data-size="46" name="cpu_usage" id="cpu_usage"><span class="percent"></span>%
                            			</div>
                            		</div>
                            		
                            		<div class="infobox-data">
                            			<span class="infobox-text"><%lang("CPU");%></span>
                            			<span class="infobox-content"><span class="approx"></span> </span>
                            		</div>
                            	</div>

                            	<div class="infobox infobox-blue2">
                            		<div class="infobox-progress">
                          			<div class="easy-pie-chart percentage" data-size="46" name='memory' id='memory' ><span class="percent"></span>%
                            			</div>
                            		</div>
                            		
                            		<div class="infobox-data">
                            			<span class="infobox-text"><%lang("Memory");%></span>
                            			<span class="infobox-content"><span class="approx"></span> </span>
                            		</div>
                            	</div>
	
                            	<div class="infobox infobox-orange">
                            		<div class="infobox-icon"><i class="icon-desktop"></i></div>
                            		<div class="infobox-data">
                            			<span class="infobox-data-number" name="dhcp_lease_num" id="dhcp_lease_num"></span>
                            			<span class="infobox-content"><%lang("Device(s)");%></span>
                            		</div>
                            	
                            	</div>
                            	
                                <div class="infobox infobox-red">
                            		<div class="infobox-icon"><i class="icon-time"></i></div>
                            		<div class="infobox-data">
                            			<span class="infobox-data-number" name="cur_time" id="cur_time"></span>
                            			<span class="infobox-content"><%lang("Time");%></span>
                            		</div>                            		
                            	</div>



                                <a id="usb_link" name="usb_link">
                                <div class="infobox infobox-green">
                                    <div class="infobox-progress">
                                        <div class="easy-pie-chart percentage" data-size="46" name="usb_usage" id="usb_usage"><span class="percent"></span>
                                        </div>
                                    </div>

                                    <div class="infobox-data">
                                        <span class="infobox-text"><%lang("USB Used");%></span>
                                        <span class="infobox-content" id="usb_box"><span class="approx"></span> </span>
                                    </div>
                                </div>
                                </a>

                                <a id="sd_link" name="sd_link">
                                <div class="infobox infobox-blue">
                                    <div class="infobox-progress">
                                    <div class="easy-pie-chart percentage" data-size="46" name='sd_usage' id='sd_usage'><span class="percent"></span>
                                        </div>
                                    </div>
                                    <div class="infobox-data">
                                        <span class="infobox-text"><%lang("SD Used");%></span>
                                        <span class="infobox-content" id="sd_box"><span class="approx"></span> </span>
                                    </div>
                                </div>
                                </a>

                          </div>
                        </div>
</div>
</div><!--/row-->


                    	<input type='hidden'  name='tx_0' id='tx_0'  />
                	<input type='hidden'  name='rx_0' id='rx_0'  />
                	<input type='hidden'  name='tx_max' id='tx_max'  />
                	<input type='hidden'  name='rx_max' id='rx_max'  />
                        <input type='hidden'  name='wan0_ipaddr' id='wan0_ipaddr' value='' />
                        <div class="hr hr32 hr-dotted"></div>      
<div class="clearfix">                        
<div class="row-fluid">
                        <div class="row-fluid">
                        	<div class="span6">
                        		<div class="widget-box transparent">
                        			<div class="widget-header widget-header-flat">
                        				<h4 class="lighter"><i class="icon-battery"></i><%lang("Battery Status");%></h4>
                        				<div class="widget-toolbar">
                        					<a href="#" data-action="collapse"><i class="icon-chevron-up"></i></a>
                        				</div>
                        			</div>                       			
                        			<div class="widget-body">
                        			 <div class="widget-main no-padding">
                        			  <table id="table_bug_report" class="table table-bordered table-striped">
                        				<thead>
                        					<tr>
                        					</tr>
                        				</thead>
                        
                        				<tbody>
                        					<tr>
                        						<td class=""><%lang("Power Level");%></td>
                        						<td class="phone"><span name="battery_status" id="battery_status"></span></td>
                        					</tr>
                        					
                        					<tr>
                        						<td class=""><%lang("Charge Status");%></td>
                        						<td class="phone"><span name="charge_status" id="charge_status"></span></td>
                        					</tr>
                        					
                        				</tbody>
                        			  </table>
                        			 </div><!--/widget-main-->
                        			</div><!--/widget-body-->
                        		</div><!--/widget-box-->
                        	</div>
                        	
	
                    		<div class="span6">
                    		<div class="widget-box transparent">
                    			<div class="widget-header widget-header-flat">
                    				<h4 class="lighter"><i class="icon-storage"></i><%lang("Storage Status");%></h4>
                    				<div class="widget-toolbar">
                    					<a href="#" data-action="collapse"><i class="icon-chevron-up"></i></a>
                    				</div>
                    			</div>
                    			
                    			<div class="widget-body">
                    			 <div class="widget-main no-padding">
                    			  <table id="table_bug_report" class="table table-bordered table-striped">
                    				<thead>
                    					<tr>
                    					</tr>
                    				</thead>
                    
                    				<tbody>
                    					<tr>
                    						<td class=""><%lang("USB Disk");%></td>
                    						<td class="phone"><span name="usb" id="usb"></span></td>
                    					</tr>
                    					
                    					<tr>
                    						<td class=""><%lang("SD Card");%></td>
                    						<td class="phone"><span name="sd" id="sd"></span></td>
                    					</tr>
                    					
                    				</tbody>
                    			  </table>
                    			 </div><!--/widget-main-->
                    			</div><!--/widget-body-->
                    		</div><!--/widget-box-->
                    	</div>
                    </div>

                    <div class="hr hr32 hr-dotted"></div>      
                    <div class="row-fluid">

                        <div class="span6">
                            <div class="widget-box transparent">
                                <div class="widget-header widget-header-flat">
                                    <h4 class="lighter"><i class="icon-resize-horizontal"></i><%lang("Link Status");%></h4>
                                        <div class="widget-toolbar">
                                            <a href="#" data-action="collapse"><i class="icon-chevron-up"></i></a>
                                        </div>
                                </div>

                            <div class="widget-body">
                                <div class="widget-main no-padding">
                                    <table id="table_bug_report" class="table table-bordered table-striped">
                                    <tr>
                                        <td class=""><%lang("Connection Type");%></td>
                                        <td class="phone" name="connection_type" id="connection_type"></td>
                                    </tr>

                                    <tr>
                                        <td class=""><%lang("Link State");%></td>
                                        <td class="phone" name="link_state" id="link_state"></td>
                                    </tr>

                                    <tr id="tr_ssid" style="display: none;">
                                        <td class=""><%lang("SSID");%></td>
                                        <td class="phone" name="ssid" id="ssid"></td>
                                    </tr>

                                    <tr>
                                        <td class=""><%lang("IP Address");%></td>
                                        <td class="phone" name="ip_addr" id="ip_addr"></td>
                                    </tr>
                                    </table>
                                </div><!--/widget-main-->
                            </div><!--/widget-body-->
                            </div><!--/widget-box-->
                        </div>

                    	<div class="span6">
                    		<div class="widget-box transparent">
                    			<div class="widget-header widget-header-flat">
                    				<h4 class="lighter"><i class="icon-info"></i><%lang("Device Information");%></h4>
                    				<div class="widget-toolbar">
                    					<a href="#" data-action="collapse"><i class="icon-chevron-up"></i></a>
                    				</div>
                    			</div>
                    			
                    			<div class="widget-body">
                    			 <div class="widget-main no-padding">
                    			  <table id="table_bug_report" class="table table-bordered table-striped">
                    				<thead>
                    					<tr>
                    					</tr>
                    				</thead>
                    
                    				<tbody>
                    					<tr>
                    						<td class=""><%lang("Model Name");%></td>
                    						<td class="phone" name="model" id="model"></td>
                    					</tr>
                    					
                    					<tr>
                    						<td class=""><%lang("Firmware Version");%></td>
                    						<td class="phone" name="fw_version" id="fw_version"></td>
                    					</tr>
                    					
                    					<tr>
                    						<td class=""><%lang("Running Time");%></td>
                    						<td class="phone"><span name="up_time" id="up_time"></span></td>
                    					</tr>
                    					
                    				</tbody>
                    			  </table>
                    			 </div><!--/widget-main-->
                    			</div><!--/widget-body-->
                    		</div><!--/widget-box-->
                    	</div>
                    </div>


                        <div class="hr hr32 hr-dotted"></div>
                        <div class="row-fluid">
                            <div class="span6">
                              <div class="widget-box transparent">
                                <div class="widget-header widget-header-flat">
                                    <h4 class="lighter"><i class="icon-signal"></i><%lang("Traffic");%></h4>
                                    <div class="widget-toolbar">
                                        <a href="#" data-action="collapse"><i class="icon-chevron-up"></i></a>
                                    </div>
                                </div>

                                <div class="widget-body">
                                 <div class="widget-main padding-10">
                                     <div id="placeholder" name="placeholder" style="height:300px;"></div>
                                 </div><!--/widget-main-->
                                </div><!--/widget-body-->
                              </div><!--/widget-box-->
                            </div>

                        </div>

                    <br><br><script type="text/javascript"><%show_copyright();%></script>	

				</div><!--/row-->
            </div>
		    </div><!--/#page-content-->
					
</div>  
    		<!--<div id="ace-settings-container">
    			<div class="btn btn-app btn-mini btn-warning" id="ace-settings-btn">
    				<i class="icon-cog"></i>
    			</div>
    			<div id="ace-settings-box">
    				<div>
    					<div class="pull-left">
    						<select id="skin-colorpicker" class="hidden">
                                <option data-class="default" value="#F39770">#F39770</option>
                                <option data-class="skin-1" value="#438EB9">#438EB9</option>
                                <option data-class="skin-2" value="#C6487E">#C6487E</option>
    						</select>
    					</div>
    					<span>&nbsp; Choose Skin</span>
    				</div>
    				<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-header" /><label class="lbl" for="ace-settings-header"> Fixed Header</label></div>
    				<div><input type="checkbox" class="ace-checkbox-2" id="ace-settings-sidebar" /><label class="lbl" for="ace-settings-sidebar"> Fixed Sidebar</label></div>
    			</div>
    		</div>/#ace-settings-container-->
		</div><!-- #main-content -->
	</div><!--/.fluid-container#main-container-->
	 </form>
	<a href="#" id="btn-scroll-up" class="btn btn-small btn-inverse">
		<i class="icon-double-angle-up icon-only"></i>
	</a>


	</body>
</html>
