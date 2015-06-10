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
  <fieldset>
    <legend><%lang("DBM");%> -
    <%increase("$0", "1");%> <%lang("WAN");%>
    </legend>

  <input type='hidden' name='wan$0_bw_mode' value='auto' />

<!--
  <div class="setting">
    <div class="label"><% lang("Bandwidth Type (Download/Upload)" ); %></div>
    <select name="wan$0_bw_type" 
        onchange="sel_bw(this.form.wan$0_bw_type,this.form,$0,wan_bw_objects[$0]);
                  bw_update_available_bw();bw_update_total_available_bw('$0')">
      <%bw_show_adv_bw_type("$0");%>
    </select>
  </div>
-->
  <div class="setting">
    <div class="label"><% lang("Download Bandwidth"); %></div>
    <select id="wan$0_bw_dl"  name="wan$0_bw_dl" 
        onchange="sel_bw(this,this.form,$0,'wan$0_bw_dl'); bw_update_available_bw();bw_update_total_available_bw('$0')">
    <%bw_show_rate_setting("dl", "$0");%>
    </select> K bps
  </div>

  <div class="setting" style='display:none;' id='wan$0_bw_dl_show'>
    <div class="label"><% lang("Custom Download Bandwidth"); %></div>
    <input id="wan$0_bw_dl_custom"  name="wan$0_bw_dl_custom" size="7" maxlength="6"
         value='<%nvg_attr_get("wan$0_bw_dl", "wan_bw_rule", "$0", "dl");%>'
         onblur='valid_number(this, "Download Bandwidth");bw_update_available_bw();bw_update_total_available_bw("$0")' /> K bps
    </input>
  </div>


  <div class="setting">
    <div class="label"><% lang("Upload Bandwidth"); %></div>
    <select id="wan$0_bw_ul" name="wan$0_bw_ul"
        value='<%nvg_attr_get("wan$0_bw_ul", "wan_bw_rule", "$0", "ul");%>'
        onchange="sel_bw(this,this.form,$0,'wan$0_bw_ul');bw_update_available_bw();bw_update_total_available_bw('$0')" />
    <%bw_show_rate_setting("ul", "$0");%>
    </select> K bps
  </div>

  <div class="setting" style='display:none;' id='wan$0_bw_ul_show'>
    <div class="label"><% lang("Custom Upload Bandwidth"); %></div>
    <input id="wan$0_bw_ul_custom"  name="wan$0_bw_ul_custom" size="7" maxlength="6"
         value='<%nvg_attr_get("wan$0_bw_ul", "wan_bw_rule", "$0", "ul");%>'
         onblur='valid_number(this, "Upload Bandwidth");bw_update_available_bw();bw_update_total_available_bw("$0")' /> K bps
    </input>
  </div>

  <div class="setting">
    <div class="label"><% lang("Reserved Buffering Bandwidth"); %></div>
    <input id="wan$0_bw_resv"  name="wan$0_bw_resv" size="7" maxlength="6" 
        value='<%bw_show_resv("$0");%>' onblur='bw_update_available_bw();bw_update_total_available_bw("$0")' /> %
    </input>
  </div>

  <div style='font-size:12px;margin-left:10px;color:#000000'>
      (<%lang("Too less reserved buffering bandwidth might cause congestion in a unstable network.");%>)
  </div>

  <div class="setting">
    <div class="label"><%lang("Available Bandwidth");%></div>
    <div id='wan$0_totall_available_bw' class='wan$0_totall_available_bw'></div>
  </div>

  </fieldset><br>
