/*
 * Ext JS Library 3.0 RC1
 * Copyright(c) 2006-2009, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://extjs.com/license
 */

var formPost;
var gp;
var fm = Ext.form;
var ds_cnt = 2;
var vlan_ds;
var vlan_rule = 'vlan_rule';
var vlan_attr = [
    'name',
    'enable',
    'vid',
    <%vlan_js("element");%>
    ];

var vlanport_ds;
var vlanport_rule = 'vlanport_rule';
var vlanport_attr = [
    'name',
    'pvid',
    'tag'
    ];

var ds_array;
var rule_array;
var attr_array;

Ext.onReady(function(){

    var checkColumn_en = new Ext.grid.CheckColumn({
       header: 'Enable',
       width: 30,
       dataIndex: 'enable',
    });
   <%vlan_js("vlan_table");%> 

    var cm = new Ext.grid.ColumnModel({
        columns: [ 
            {
                header: 'Name',
                width: 50,
                dataIndex: 'name',
                editor: new fm.TextField({
                allowBlank: false
                })
            }
            , checkColumn_en
            , {
                header: 'VLAN ID',
                width: 40,
                dataIndex: 'vid',
                editor: new fm.NumberField({
                    allowBlank: false,
                    allowNegative: false,
                    minValue: 1,
                    maxValue: 4094
                })
            },
            <%vlan_js("cm");%>
        ]
    }); /* End of column model */

    vlan_ds = new Ext.data.Store({
       reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'enable', type: 'bool'},
            {name: 'vid', type: 'int'},
            <%vlan_js("store");%>
            ]),
        data: vlan_data
    }); /* End of data store */

    gp = new Ext.grid.EditorGridPanel({
        ds: vlan_ds,
        cm: cm,
        trackMouseOver: true,
        viewConfig: {
            forceFit:true
        },
        autoExpandColumn:'name',
        height: 350,
        renderTo: 'gridpanel',
        plugins: [checkColumn_en,
                  <%vlan_js("cm");%>
                  ],
        clicksToEdit: 1,
        sm: new Ext.grid.RowSelectionModel({singleSelect: true}),
        tbar: [{
            text: 'Add',
            handler : function(){
            var Plant = gp.getStore().recordType;
            var p = new Plant({
                name: '',
                enable: false,
                vid: vlan_ds.getCount() + 1,
                <%vlan_js("sm");%>
                });
            gp.stopEditing();
            if (vlan_ds.getCount() == rule_max) {
                alert('Max vlan rule is ' + rule_max);
                return;
                }
            vlan_ds.insert(vlan_ds.getCount(), p);
            gp.startEditing(vlan_ds.getCount() - 1, 0);
            vlan_ds.save();
            }
        }
        , '-'
        , {
            text: 'Remove',
            //disabled: true,
            handler: function(){
                var s = gp.getSelectionModel().getSelections();
                for(var i = 0, r; r = s[i]; i++){
                    vlan_ds.remove(r);
                }
            }
        }]
    });


    formPost = new Ext.FormPanel({
        renderTo: Ext.getBody(),
        baseCls: 'x-plain',
        bodyStyle: 'padding: 0px 10px 0px 10px;'
    });

    var checkColumn_porttag = new Ext.grid.CheckColumn({
        header: 'Port Tag',
        width: 1,
        dataIndex:'tag'
    });
    cm_tag = new Ext.grid.ColumnModel({
        columns: [ {
                header: 'Port',
                width: 1,
                editor: new fm.TextField({
                    allowBlank: false
                })
            }, {
                header: 'PVID',
                width: 1,
                dataIndex: 'pvid',
                editor: new fm.NumberField({
                    allowBlank: false,
                    allowNegative: false,
                    minValue: 1,
                    maxValue: 4094
                })
            }  
            , checkColumn_porttag
        ]
    });

    vlanport_ds = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'pvid', type: 'int'},
            {name: 'tag', type: 'bool'},
        ]),
        data: vlan_port_data
    });

    gp_tag = new Ext.grid.EditorGridPanel({
        ds: vlanport_ds,
        cm: cm_tag,
        trackMouseOver: true,
        title:'Tag Table',
        viewConfig: {
            forceFit:true
        },
        autoExpandColumn:'name',
        height: 200,
        renderTo: 'gridpanel1',
        plugins: [checkColumn_porttag]
    });
    ds_array = [vlan_ds, vlanport_ds];
    rule_array = [vlan_rule, vlanport_rule];
    attr_array = [vlan_attr, vlanport_attr];

});
