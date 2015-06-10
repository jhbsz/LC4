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
    'p1', 'p2', 'p3', 'p4', 'p5',
    's1', 's2', 
    'wds', 'ur'
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
    var checkColumn_p1 = new Ext.grid.CheckColumn({
        header: 'Port1(L4)',
        width: 40,
        dataIndex: 'p1'
    });
    var checkColumn_p2 = new Ext.grid.CheckColumn({
        header: 'Port2(L3)',
        width: 40,
        dataIndex: 'p2'
    });
    var checkColumn_p3 = new Ext.grid.CheckColumn({
        header: 'Port3(L2)',
        width: 40,
        dataIndex: 'p3'
    });
    var checkColumn_p4 = new Ext.grid.CheckColumn({
        header: 'Port4(L1)',
        width: 40,
        dataIndex: 'p4'
    });
    var checkColumn_p5 = new Ext.grid.CheckColumn({
        header: 'Port5(W1)',
        width: 40,
        dataIndex: 'p5'
    });
    var radioColumn_s1 = new Ext.grid.RadioColumn({
        header: 'SSID1',
        width: 30,
        dataIndex: 's1'
    });
    var radioColumn_s2 = new Ext.grid.RadioColumn({
        header: 'SSID2',
        width: 30,
        dataIndex: 's2'
    });
    var radioColumn_wds = new Ext.grid.RadioColumn({
        header: 'WDS',
        width: 30,
        dataIndex: 'wds'
    });
    var radioColumn_ur = new Ext.grid.RadioColumn({
        header: 'UR',
        width: 30,
        dataIndex: 'ur'
    });

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
            }
            , checkColumn_p1
            , checkColumn_p2
            , checkColumn_p3
            , checkColumn_p4
            , checkColumn_p5
            , radioColumn_s1
            , radioColumn_s2
            , radioColumn_wds
            , radioColumn_ur
        ]
    }); /* End of column model */

    vlan_ds = new Ext.data.Store({
       reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'enable', type: 'bool'},
            {name: 'vid', type: 'int'},
            {name: 'p1', type: 'bool'},
            {name: 'p2', type: 'bool'},
            {name: 'p3', type: 'bool'},
            {name: 'p4', type: 'bool'},
            {name: 'p5', type: 'bool'},
            {name: 's1', type: 'bool'},
            {name: 's2', type: 'bool'},
            {name: 'wds', type: 'bool'},
            {name: 'ur', type: 'bool'},
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
                  checkColumn_p1,
                  checkColumn_p2,
                  checkColumn_p3,
                  checkColumn_p4,
                  checkColumn_p5,
                  radioColumn_s1,
                  radioColumn_s2,
                  radioColumn_wds,
                  radioColumn_ur],
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
                p1:false,
                p2:false,
                p3:false,
                p4:false,
                p5:false,
                s1:false, 
                s2:false, 
                wds:false, 
                ur:false 
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
