  <div class="setting">
  <div class="label"><% lang("Bandwidth Type (Download/Upload)" ); %></div>
    <select name="bw$0_bw_type" onchange="sel_bw(this.form.bw$0_bw_type,this.form,$0,bw_bw_objects[$0])">
      <%bw_show_adv_bw_type("$0");%>
    </select>
  </div>

  <div class="setting">
  <div class="label"><% lang("Download Bandwidth"); %></div>
    <input  name="bw$0_bw_dl" size="6" maxlength="5"
         value='<%nvg_attr_get("bw$0_bw_dl", "wan_bw_rule", "$0", "dl");%>'
         onblur='valid_number(this, "Download Bandwidth")' /> K bps
    </input>
  </div>
  <div class="setting">
  <div class="label"><% lang("Upload Bandwidth"); %></div>
    <input name="bw$0_bw_ul" size="6" maxlength="5"
        value='<%nvg_attr_get("bw$0_bw_ul", "wan_bw_rule", "$0", "ul");%>'
        onblur='valid_number(this, "Upload Bandwidth")' /> K bps
    </input>
  </div>
