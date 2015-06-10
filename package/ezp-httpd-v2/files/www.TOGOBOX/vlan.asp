<% do_pagehead1(); %>      
<html>
<head>
<title><% nv_get("model"); %> - VLAN</title>
<% do_pagehead2("no_table"); %>
<link rel="stylesheet" type="text/css" href="style/AXIM/table.css" />

<style type="text/css">
#container table {
    border-spacing: 0em;
    margin: 0 10px 0 10px;
    width: 0%;
    }
</style>

<script type="text/javascript">


var F;
function init() {
    F = document.getElementsByName('form');
    <%widget_start();%>
}

function to_submit(F) {

    if ( CheckAll_SSID_WDS_UR()==true ){
        F.submit_button.value = "vlan";
        F.action.value = "Apply";

        $("#VlanDataForm").empty();
        $("#PortDataForm").empty();

        Vlan_columnNames = $("#VlanListData").jqGrid('getGridParam','colModel');
        Vlan_rows = $("#VlanListData").jqGrid('getGridParam','data');
        VlanPort_columnNames = $("#PortListData").jqGrid('getGridParam','colModel');
        VlanPort_rows = $("#PortListData").jqGrid('getGridParam','data');
        js_ajax_preaction(F, Vlan_columnNames, Vlan_rows, VlanPort_columnNames, VlanPort_rows );
        display_mask(F);
    }//End if
}

//Source Data
<%vlan_show_content();%>

$(document).ready(function(){
    var intRegex = /^\d+$/;
    var floatRegex = /^((\d+(\.\d *)?)|((\d*\.)?\d+))$/;

    var colNames_List = ["Name","Enable","VLAN ID"];
    var colModel_List = [
                         {name:'name',index:'name', width:8, align:"center", editable:true},
                         {name:'enable',index:'enable', width:5, align:"center", edittype:'checkbox', editoptions: { value:"1:0"},formatter: "checkbox", formatoptions: {disabled : false}},
                         {name:'vid',index:'vid', width:5, align:"center", editable:true,formatter:'integer'}
                        ];

    <%vlan_js("vlan_table");%>

    /*##### VLAN List ##### */
    jQuery('#VlanListData').jqGrid({
            data: Vlan_rows,
            datatype: 'local',
            colNames: colNames_List ,
            colModel: colModel_List,
            autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            sortname: 'Content',
            sortorder: 'desc',
            pager: '#VlanListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "VLAN"
    });

    jQuery("#VlanListData").jqGrid('navGrid','#VlanListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );


       var Vlan_FieldNames = $("#VlanListData").jqGrid('getGridParam','colNames');
    var Vlan_columnNames = $("#VlanListData").jqGrid('getGridParam','colModel');

    var Vlan_rows = [];
    var Vlan_rows_data = {};
    
    var Vlan_rowsArray = vlan_data.split("|");
    var Vlan_rows_dataArray;

    for (var i=0; i< Vlan_rowsArray.length; i++ ){
        Vlan_rows_dataArray = Vlan_rowsArray[i].split("^");
        Vlan_rows_data = {};
        for (var j=0; j< Vlan_rows_dataArray.length; j++ ){
            var ColumName = Vlan_columnNames[j].name
            Vlan_rows_data[ ColumName ] = Vlan_rows_dataArray[j];
        }
        Vlan_rows[i] = Vlan_rows_data;
    }
    
    $("#VlanListData").jqGrid("clearGridData", true);
    jQuery("#VlanListData").setGridParam({ 'data':  Vlan_rows }).trigger("reloadGrid");

    
    $("#VlanListData td input:checkbox").live('change', function() {
        var TotalRecord = $("#VlanListData").getGridParam("reccount");
        var rowid = $(this).parent().parent().index();
        var colid = $(this).parent().index();

        if (Vlan_FieldNames[colid].indexOf("SSID")!=-1 || Vlan_FieldNames[colid].indexOf("WDS")!=-1 || Vlan_FieldNames[colid].indexOf("UR")!=-1){
            if ($(this).prop('checked') == false){
                $(this).attr('checked', true);
            }
        }

        Vlan_rows_data = {};
        for (var i=0; i<Vlan_columnNames.length; i++ ){
            var ColumName = Vlan_columnNames[i].name;
            Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', rowid, ColumName );
        }//End for
        Vlan_rows[rowid-1] = Vlan_rows_data;

        // Uncheck other SSID, WDS, UR checkbox
        for (var j=0; j<TotalRecord; j++ ){
            Vlan_rows_data = {};
            if (j!=rowid-1){
                for (var k=0; k<Vlan_columnNames.length; k++ ){
                    var ColumName = Vlan_columnNames[k].name;
                    Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', j+1, ColumName );

                    if (Vlan_FieldNames[k].indexOf("SSID")!=-1 || Vlan_FieldNames[k].indexOf("WDS")!=-1 || Vlan_FieldNames[k].indexOf("UR")!=-1){
                        if (eval("Vlan_rows[rowid-1]."+ColumName) == 1){
                            Vlan_rows_data[ ColumName ] = "0";
                        }else{
                            Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', j+1, ColumName );
                        }//End if
                    }else{
                        Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', j+1, ColumName );
                    }
                }
                Vlan_rows[j] = Vlan_rows_data;
            }//End if

        }//End for

        $("#VlanListData").jqGrid("clearGridData", true);
        jQuery("#VlanListData").setGridParam({ 'data':  Vlan_rows }).trigger("reloadGrid");

        $("#VlanListData").setSelection(rowid, true);
    });
 

    jQuery("#VlanListData").navButtonAdd('#VlanListDataPager',{
       caption:"Add", 
       buttonicon:"ui-icon-plusthick", 
       onClickButton: function(){
            $("#PortDataForm").empty();
            $("#VlanDataForm").empty();
            for (var i=0; i<Vlan_FieldNames.length; i++ ){
                if (Vlan_FieldNames[i]=="Name" || Vlan_FieldNames[i]=="VLAN ID"){
                    $("#VlanDataForm").append("<tr><td align='right'>"+Vlan_FieldNames[i]+":</td><td><input type='text' name='"+ Vlan_columnNames[i].name +"' id='"+ Vlan_columnNames[i].name +"' value=''></td></tr>");
                }else{
                    $("#VlanDataForm").append("<tr><td align='right'>"+Vlan_FieldNames[i]+":</td><td><input type='checkbox' name='"+ Vlan_columnNames[i].name +"' id='"+ Vlan_columnNames[i].name +"' value='1'></td></tr>");
                }
            }

            $( "#VlanListData-dialog" ).dialog({
                resizable: false,
                height:520,
                width:500,
                modal: true,
                buttons: {
                    "<% lang("Add"); %>": function() {
                        var TotalRecord = $("#VlanListData").getGridParam("reccount");
                        var New_Vlan_rows_data ={};

                        // Check VLAN ID is integer
                        if( intRegex.test($("#vid").val()) == false ) {
                            alert("VLAN ID must be a integer");
                            return false;
                        }

                        for (var i=0; i<Vlan_columnNames.length; i++ ){
                            if ($("#"+Vlan_columnNames[i].name).is('input:checkbox')){

                                // Enable, Port1 ~ Port5, SSID1~SSID2, WDS, UR
                                if ($('input[name='+ Vlan_columnNames[i].name +']').is(':checked')){
                                    
                                    // Uncheck other SSID, WDS, UR checkbox
                                    if (Vlan_FieldNames[i].indexOf("SSID")!=-1 || Vlan_FieldNames[i].indexOf("WDS")!=-1 || Vlan_FieldNames[i].indexOf("UR")!=-1){
                                        
                                        for (var j=0; j<TotalRecord; j++ ){
                                            Vlan_rows_data = {};
                                            for (var k=0; k<Vlan_columnNames.length; k++ ){
                                                var ColumName = Vlan_columnNames[k].name;
                                                Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', j+1, ColumName );

                                                if (Vlan_FieldNames[k].indexOf("SSID")!=-1 || Vlan_FieldNames[k].indexOf("WDS")!=-1 || Vlan_FieldNames[k].indexOf("UR")!=-1){
                                                    Vlan_rows_data[ ColumName ] = "0";
                                                }else{
                                                    Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', j+1, ColumName );
                                                }
                                            }
                                            Vlan_rows[j] = Vlan_rows_data;
                                        }//End for

                                    }//End if

                                    New_Vlan_rows_data[Vlan_columnNames[i].name] = "1";
                                } else {
                                    New_Vlan_rows_data[Vlan_columnNames[i].name] = "0";
                                }//End if

                            } else {// Name, VLANID
                                    New_Vlan_rows_data[Vlan_columnNames[i].name] = $("#"+Vlan_columnNames[i].name).val();
                            }//End if

                        }//End for

                        Vlan_rows.push(New_Vlan_rows_data);
                        $("#VlanListData").jqGrid("clearGridData", true);
                        jQuery("#VlanListData").setGridParam({ 'data':  Vlan_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });            
       }
    });


    jQuery("#VlanListData").navButtonAdd('#VlanListDataPager',{
       caption:"Del", 
       buttonicon:"ui-icon-trash", 
       onClickButton: function(){
               var rowid = $("#VlanListData").jqGrid('getGridParam','selrow');

            if ( rowid == null){
                alert("Please, select row");
                return false;
            } else {
                var DelItem = rowid - 1;
                Vlan_rows.remove(DelItem);
            }
            $("#VlanListData").jqGrid("clearGridData", true);
            jQuery("#VlanListData").setGridParam({ 'data':  Vlan_rows }).trigger("reloadGrid");
       }
    });

    //Remove Item from Array
    Array.prototype.remove = function(from, to) {
      var rest = this.slice((to || from) + 1 || this.length);
      this.length = from < 0 ? this.length + from : from;
      return this.push.apply(this, rest);
    };


    jQuery("#VlanListData").navButtonAdd('#VlanListDataPager',{
       caption:"Modify", 
       buttonicon:"ui-icon-pencil", 
       onClickButton: function(){
            var TotalRecord = $("#VlanListData").getGridParam("reccount");
            var rowid = $("#VlanListData").jqGrid('getGridParam','selrow');
            var getVlanFieldData;
            
            if (rowid == null){
                alert("Please, select row");
                return false;
            }

            $("#PortDataForm").empty();
            $("#VlanDataForm").empty();
            for (var i=0; i<Vlan_FieldNames.length; i++ ){
                getVlanFieldData = $('#VlanListData').jqGrid('getCell', rowid, Vlan_columnNames[i].name);

                if (Vlan_FieldNames[i]=="Name" || Vlan_FieldNames[i]=="VLAN ID"){
                    $("#VlanDataForm").append("<tr><td align='right'>"+Vlan_FieldNames[i]+":</td><td><input type='text' name='"+ Vlan_columnNames[i].name +"' id='"+ Vlan_columnNames[i].name +"' value='"+ getVlanFieldData +"'></td></tr>");
                }else{
                    if ( getVlanFieldData==1 ){
                        $("#VlanDataForm").append("<tr><td align='right'>"+Vlan_FieldNames[i]+":</td><td><input type='checkbox' name='"+ Vlan_columnNames[i].name +"' id='"+ Vlan_columnNames[i].name +"' value='1' checked></td></tr>");
                    }else{
                        $("#VlanDataForm").append("<tr><td align='right'>"+Vlan_FieldNames[i]+":</td><td><input type='checkbox' name='"+ Vlan_columnNames[i].name +"' id='"+ Vlan_columnNames[i].name +"' value='1'></td></tr>");
                    }
                }
            }
        
            $( "#VlanListData-dialog" ).dialog({
                resizable: false,
                height:520,
                width:500,
                modal: true,
                buttons: {
                    "<% lang("Modify"); %>": function() {
                        var Modify_Vlan_rows_data ={};

                        // Check VLAN ID is integer
                        if( intRegex.test($("#vid").val()) == false ) {
                            alert("VLAN ID must be a integer");
                            return false;
                        }

                        for (var i=0; i<Vlan_columnNames.length; i++ ){

                            if ($("#"+Vlan_columnNames[i].name).is('input:checkbox')){

                                if ($('input[name='+ Vlan_columnNames[i].name +']').is(':checked')){

                                    // Uncheck other SSID, WDS, UR checkbox
                                    if (Vlan_FieldNames[i].indexOf("SSID")!=-1 || Vlan_FieldNames[i].indexOf("WDS")!=-1 || Vlan_FieldNames[i].indexOf("UR")!=-1){
                                        
                                        for (var j=0; j<TotalRecord; j++ ){
                                            Vlan_rows_data = {};
                                            for (var k=0; k<Vlan_columnNames.length; k++ ){
                                                var ColumName = Vlan_columnNames[k].name;
                                                Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', j+1, ColumName );

                                                if (Vlan_FieldNames[k].indexOf("SSID")!=-1 || Vlan_FieldNames[k].indexOf("WDS")!=-1 || Vlan_FieldNames[k].indexOf("UR")!=-1){
                                                    Vlan_rows_data[ ColumName ] = "0";
                                                }else{
                                                    Vlan_rows_data[ ColumName ] = $('#VlanListData').jqGrid('getCell', j+1, ColumName );
                                                }
                                            }
                                            Vlan_rows[j] = Vlan_rows_data;
                                        }//End for

                                    }//End if

                                    Modify_Vlan_rows_data[Vlan_columnNames[i].name] = "1";
                                } else {
                                    Modify_Vlan_rows_data[Vlan_columnNames[i].name] = "0";
                                }

                            } else {
                                Modify_Vlan_rows_data[Vlan_columnNames[i].name] = $("#"+Vlan_columnNames[i].name).val()
                            }
                        }

                        Vlan_rows[rowid-1] = Modify_Vlan_rows_data;
                        $("#VlanListData").jqGrid("clearGridData", true);
                        jQuery("#VlanListData").setGridParam({ 'data':  Vlan_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                        
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
            
       }
    });

    /*##### Port Tag ##### */
    jQuery('#PortListData').jqGrid({
            data: VlanPort_rows,
            datatype: 'local',
            colNames: ["Port","PVID","Port Tag"] ,
            colModel: [{name:'name',index:'name', width:8, align:"center", editable:false},
                       {name:'pvid',index:'pvid', width:8, align:"center", editable:true,formatter:'integer'},
                       {name:'tag',index:'tag', width:5, align:"center", edittype:'checkbox', editoptions: { value:"1:0"},formatter: "checkbox", formatoptions: {disabled:0}}
                      ],
            autowidth: 'auto',
            width: 750,
            height: 'auto',
            rowNum: 50,
            //rowList: [20,50,100],
            sortname: 'Content',
            sortorder: 'desc',
            pager: '#PortListDataPager',
            viewrecords: false,
            pgbuttons: false,
            pgtext: null,
            caption: "Port Tag"
    });

    jQuery("#PortListData").jqGrid('navGrid','#PortListDataPager',
                                        {edit: false,add: false,del: false,search: false,refresh: false}
    );

    var VlanPort_FieldNames = $("#PortListData").jqGrid('getGridParam','colNames');
    var VlanPort_columnNames = $("#PortListData").jqGrid('getGridParam','colModel')

    var VlanPort_rows = [];
    var VlanPort_rows_data = {};
    
    var VlanPort_rowsArray = vlan_port_data.split("|");
    var VlanPort_rows_dataArray;

    for (var i=0; i< VlanPort_rowsArray.length; i++ ){
        VlanPort_rows_dataArray = VlanPort_rowsArray[i].split("^");
        VlanPort_rows_data = {};
        VlanPort_rows_data[ "name" ] = VlanPort_rows_dataArray[0];
        VlanPort_rows_data[ "pvid" ] = VlanPort_rows_dataArray[1];
        VlanPort_rows_data[ "tag" ] = VlanPort_rows_dataArray[2];
        VlanPort_rows[i] = VlanPort_rows_data;
    }

    $("#PortListData").jqGrid("clearGridData", true);
    jQuery("#PortListData").setGridParam({ 'data':  VlanPort_rows }).trigger("reloadGrid");



    $("#PortListData td").live('click', function(){
        var rowid = $(this).parent().index();
        var colid = $(this).index();
        var TotalRecord = $("#PortListData").getGridParam("reccount");

        VlanPort_rows_data = {};
        for (var i=0; i<VlanPort_columnNames.length; i++ ){
            var ColumName = VlanPort_columnNames[i].name;
            VlanPort_rows_data[ ColumName ] = $('#PortListData').jqGrid('getCell', rowid, ColumName );
        }//End for
        VlanPort_rows[rowid-1] = VlanPort_rows_data;

        $("#PortListData").jqGrid("clearGridData", true);
        jQuery("#PortListData").setGridParam({ 'data':  VlanPort_rows }).trigger("reloadGrid");

        $("#PortListData").setSelection(rowid, true);
    });


    jQuery("#PortListData").navButtonAdd('#PortListDataPager',{
       caption:"Modify", 
       buttonicon:"ui-icon-pencil", 
       onClickButton: function(){
            var rowid = $("#PortListData").jqGrid('getGridParam','selrow');
            var getPortFieldData;

            if (rowid == null){
                alert("Please, select row");
                return false;
            }

            $("#PortDataForm").empty();
            $("#VlanDataForm").empty();
            
            for (var i=0; i<VlanPort_FieldNames.length; i++ ){
                getPortFieldData = $('#PortListData').jqGrid('getCell', rowid, VlanPort_columnNames[i].name);
                if (VlanPort_FieldNames[i]=="Port"){
                    $("#PortDataForm").append("<tr><td align='right'>"+VlanPort_FieldNames[i]+":</td><td><input type='text' name='"+ VlanPort_columnNames[i].name +"' id='"+ VlanPort_columnNames[i].name +"' value='"+ getPortFieldData +"' readonly></td></tr>");
                }else if (VlanPort_FieldNames[i]=="PVID")    {
                    $("#PortDataForm").append("<tr><td align='right'>"+VlanPort_FieldNames[i]+":</td><td><input type='text' name='"+ VlanPort_columnNames[i].name +"' id='"+ VlanPort_columnNames[i].name +"' value='"+ getPortFieldData +"'></td></tr>");
                }else{
                    if ( getPortFieldData==1 ){
                        $("#PortDataForm").append("<tr><td align='right'>"+VlanPort_FieldNames[i]+":</td><td><input type='checkbox' name='"+ VlanPort_columnNames[i].name +"' id='"+ VlanPort_columnNames[i].name +"' value='1' checked></td></tr>");
                    }else{
                        $("#PortDataForm").append("<tr><td align='right'>"+VlanPort_FieldNames[i]+":</td><td><input type='checkbox' name='"+ VlanPort_columnNames[i].name +"' id='"+ VlanPort_columnNames[i].name +"' value='1'></td></tr>");
                    }
                }
            }
            
        
            $( "#PortListData-dialog" ).dialog({
                resizable: false,
                height:520,
                width:500,
                modal: true,
                buttons: {
                    "<% lang("Modify"); %>": function() {
                        var Modify_VlanPort_rows_data ={};
                        // Check PVID is integer
                        if( intRegex.test($("#pvid").val()) == false ) {
                            alert("PVID must be a integer");
                            return false;
                        }

                        for (var i=0; i<VlanPort_columnNames.length; i++ ){
                            if ($("#"+VlanPort_columnNames[i].name).is('input:checkbox')){
                                if ($('input[name='+ VlanPort_columnNames[i].name +']').is(':checked')){
                                    Modify_VlanPort_rows_data[VlanPort_columnNames[i].name] = "1";
                                } else {
                                    Modify_VlanPort_rows_data[VlanPort_columnNames[i].name] = "0";
                                }

                            } else {
                                Modify_VlanPort_rows_data[VlanPort_columnNames[i].name] = $("#"+VlanPort_columnNames[i].name).val()
                            }
                        }

                        VlanPort_rows[rowid-1] = Modify_VlanPort_rows_data;
                        $("#PortListData").jqGrid("clearGridData", true);
                        jQuery("#PortListData").setGridParam({ 'data':  VlanPort_rows }).trigger("reloadGrid");
                        $( this ).dialog( "close" );
                        
                    },
                    "<% lang("Cancel"); %>": function() {
                        $( this ).dialog( "close" );
                    }
                }
            });
            
       }
    });

});


function CheckAll_SSID_WDS_UR(){
      var Vlan_FieldNames = $("#VlanListData").jqGrid('getGridParam','colNames');
    var Vlan_columnNames = $("#VlanListData").jqGrid('getGridParam','colModel');
    var TotalRecord = $("#VlanListData").getGridParam("reccount");

    for (var i=0;i<TotalRecord; i++ ){
        for (var j=0; j<Vlan_columnNames.length; j++ ){
            if (Vlan_FieldNames[j].indexOf("SSID")!=-1 || Vlan_FieldNames[j].indexOf("WDS")!=-1){
                var FieldCheck=false;
                for (var k=0; k<TotalRecord; k++ ){
                    if ($('#VlanListData').jqGrid('getCell', k+1, j )==1){
                        FieldCheck=true;
                    }
                }
                if (FieldCheck==false){
                    alert("Please assign the "+Vlan_FieldNames[j]);
                    return false
                }
            }
        }
    }
    return true;
}


</script>

</head>
  <body class="gui" onload="init();">
    <div id="VlanListData-dialog" title="Add VLan" style="display: none">
        <table id="VlanDataForm"></table>
    </div>

    <div id="PortListData-dialog" title="Add VLan" style="display: none">
        <table id="PortDataForm"></table>
    </div>

    <div id="wrapper">
        <div id="header">
          <script type="text/javascript">do_header_pre()</script>
            <% do_menu("Setup", "VLAN"); %>
          <script type="text/javascript">do_header_post()</script>
        </div>
  
      <script type="text/javascript">do_body_pre()</script>    
      <script type="text/javascript">do_page_path('<%lang("Setup");%>', '<%lang("VLAN");%>')</script>    
      <script type="text/javascript">do_table_pre()</script>    
      <script type="text/javascript">do_mainframe_pre()</script>    
          <div id="contentsInfo">


              <table id="VlanListData"></table>
              <div id="VlanListDataPager"></div>
              <br>
              <table id="PortListData"></table>
              <div id="PortListDataPager"></div>

            <form name="form" action="apply.cgi" method="post">
              <script type="text/javascript">do_hidden_elements('vlan_rule')</script>

              <div class="submitFooter">
                <input type="button" name="save_button" value="<%lang("Save Settings");%>" onClick="to_submit(this.form)" />
                <input type="reset" value="<%lang("Cancel Changes");%>" onClick="window.location.reload()"/>
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
  </body>
</html>
