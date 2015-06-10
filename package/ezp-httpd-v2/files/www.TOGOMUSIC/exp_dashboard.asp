
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8" />
		<title><% nv_get("model"); %> - Dashboard</title>
        <meta charset="utf-8" />
        <title><% nv_get("model"); %> - Internet</title>
        
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
        	})
        
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
                var res = [];
                for (var i = 0; i < h.points_0.length; ++i)
                    res.push([i, h.points_0[i] / 1000]);
                return res;
            }
            function getRxRandomData() {    
                var res = [];
                for (var i = 0; i < h.points_1.length; ++i)
                    res.push([i, h.points_1[i] / 1000]);
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
                          tickSize: [60, "second"], 
               //           timeformat: "%M:%S",
                          timeformat: "",
                          axisLabel: "Total Time: 2 Hours", 
                          axisLabelUseCanvas: true, 
                          axisLabelFontSizePixels: 12, 
                          axisLabelFontFamily: 'Verdana, Arial', 
                          axisLabelPadding: 10
                        }
            };
            dataset = [
                {label: "Upload", data :getRandomData()}, 
                {label: "Download", data:getRxRandomData()}
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
                $.plot($("#placeholder"), [ {label:"Upload",data:getRandomData()}, {label:"Download",data:getRxRandomData()}], options);
				
				var progress = $('#memory').attr("data-percent");
                $('#memory').data('easyPieChart').update(parseInt(progress, 10));
				$('#memory').find('span').text(progress);

                var cpu_usage = $('#cpu_usage').attr("data-percent");
                $('#cpu_usage').data('easyPieChart').update(parseInt(cpu_usage, 10));
                $('#cpu_usage').find('span').text(cpu_usage);

                setTimeout(update, updateInterval);
            }	
            update();
        })


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
						<li><i class="icon-home"></i> Home<span class="divider"><i class="icon-angle-right"></i></span></li>
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

                    <div class="hr hr32 hr-dotted"></div>      
                    <div class="row-fluid">
                        <%st_show_wan_setting_bytype();%>
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
                                    <h4 class="lighter"><i class="icon-signal"></i><%lang("Internal Traffic");%></h4>
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
