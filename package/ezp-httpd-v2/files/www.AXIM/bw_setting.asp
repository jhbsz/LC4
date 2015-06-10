<script language="JavaScript">
bw_objects[$0] =
[
  { name: 'wan$0_bw_mode' }, 
  { name: 'wan$0_bw_type' }, 
  { name: 'wan$0_bw_resv' }, 
  { name: 'wan$0_bw_dl' }, 
  { name: 'wan$0_bw_ul' },
  { name: 'wan$0_bw_expert_dl' }, 
  { name: 'wan$0_bw_expert_ul' }
];
wan_bw_objects[$0] =
[
  { name: 'wan$0_bw_dl' }, 
  { name: 'wan$0_bw_ul' }, 
  { name: 'wan$0_bw_expert_dl' }, 
  { name: 'wan$0_bw_expert_ul' }
];

</script>

        <div class="row">
            <div class="span12">
                <legend><%lang("DBM");%> - <%lang("WAN");%> <%increase("$0", "1");%></legend>
                <input type='hidden' name='wan$0_bw_mode' value='auto' />
                <div class="row show-grid">
                    <div class="span4"><% lang("Bandwidth Type (Download/Upload)" ); %></div>
                    <div class="span8">
                        <select name="wan$0_bw_type" id="wan$0_bw_type" 
                            onchange="sel_bw(this.form.wan$0_bw_type,this.form,$0,wan_bw_objects[$0]); bw_update_available_bw();">
                          <%bw_show_adv_bw_type("$0");%>
                        </select>
                    </div>
                </div>


                <div class="row show-grid">
                    <div class="span4"><% lang("Download Bandwidth"); %></div>
                    <div class="span8">
                        <input type="text" id="wan$0_bw_dl"  name="wan$0_bw_dl" size="7" maxlength="6"
                             value='<%nvg_attr_get("wan$0_bw_dl", "wan_bw_rule", "$0", "dl");%>'
                             onblur='valid_number(this, "Download Bandwidth");bw_update_available_bw();'/> K bps
                    </div>
                </div>


                <div class="row show-grid">
                    <div class="span4"><% lang("Upload Bandwidth"); %></div>
                    <div class="span8">
                        <input type="text" id="wan$0_bw_ul" name="wan$0_bw_ul" size="7" maxlength="6"
                            value='<%nvg_attr_get("wan$0_bw_ul", "wan_bw_rule", "$0", "ul");%>'
                            onblur='valid_number(this, "Upload Bandwidth");bw_update_available_bw();'/> K bps
                        </input>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><% lang("Router Reserved Bandwidth"); %></div>
                    <div class="span8">
                        <input type="text" id="wan$0_bw_resv"  name="wan$0_bw_resv" size="7" maxlength="6" 
                            value='<%bw_show_resv("$0");%>' onblur='bw_update_available_bw();' /> %
                    </div>
                </div>
<!--
                <div class="row show-grid">
                    <div class="span12">
                        <div style='font-size:12px;margin-left:10px;color:#000000'>
                          (<%lang("Too less reserved buffering bandwidth might cause congestion in a unstable network.");%>)
                        </div>
                    </div>
                </div>

                <div class="row show-grid">
                    <div class="span4"><%lang("Available Bandwidth");%></div>
                    <div class="span8">
                        <div id='wan$0_totall_available_bw' class='wan$0_totall_available_bw'></div>
                    </div>
                </div>
-->
            </div>
        </div><!-- row -->
        <br>
