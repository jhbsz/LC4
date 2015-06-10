  <div class="setting">
  <div class="label"><% lang("Download Bandwidth"); %></div>
    <input  name="bw$0_bw_expert_dl" size="5" maxlength="5"
         value='<%nvg_attr_get("bw$0_bw_expert_dl", "wan_bw_rule", "$0", "expert_dl");%>'
         onblur='valid_number(this, "Download Bandwidth")' /> K bps
    </input>
  </div>
  <div class="setting">
  <div class="label"><% lang("Upload Bandwidth"); %></div>
    <input name="bw$0_bw_expert_ul" size="5" maxlength="5"
        value='<%nvg_attr_get("bw$0_bw_expert_ul", "wan_bw_rule", "$0", "expert_ul");%>'
        onblur='valid_number(this, "Upload Bandwidth")' /> K bps
    </input>
  </div>

