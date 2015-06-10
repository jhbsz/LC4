var formPost;
var ds_cnt = 2;
var vlan_rule = 'vlan_rule';
var vlan_attr = [
    'name',
    'enable',
    'vid',
    'p1', 'p2', 'p3', 'p4', 'p5',
    's1', 's2', 
    'wds', 'ur'
    ];
var ds_array, rule_array, attr_array;
var modify_flag = 0;
Ext.onReady(function(){

   var data = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'enable'},
            {name: 'name'},
            {name: 'mac'},
            {name: 'act'},
            {name: 's_arp_enable'},
            {name: 's_dhcp_enable'},
            {name: 'ip'},
        ]),
        data: []
    });

    function renderEnable(value){
        if ( value ) {
            return "<img src='v_021.gif'/>";
        } else {
            return "<img src='x_011.gif'/>";
        }
    }
    function renderAC(value){
        if ( value == 0 ) {
            return 'Allow';
        } else {
            return 'Deny';
        }
    }

    var cm = new Ext.grid.ColumnModel([
        {header: 'Rule Enable', dataIndex: 'enable',renderer:renderEnable},
        {header: 'Rule Name', dataIndex: 'name'},
        {header: 'MAC', dataIndex: 'mac'},
        {header: 'Action', dataIndex: 'act', renderer:renderAC},
        {header: 'Static ARP Enable', dataIndex: 's_arp_enable', renderer:renderEnable},
        {header: 'Static DHCP Enable', dataIndex: 's_dhcp_enable',renderer:renderEnable},
        {header: 'IP', dataIndex: 'ip'},
        ]);


    var op_window = new Ext.Window({
        title: 'Add Rule',
        width: 320,
        height: 320,
        closable: false,
        modal: true,
        resizable: false,
        bodyStyle: 'padding:15px',
        items:[{
            xtype: 'form',
            border: false,
            bodyStyle:'background-color:#DFE8F6',
            items:[{
                xtype:'checkbox',
                id: 'acl_enable',
                fieldLabel:'Rule Enable',
            },{
                xtype:'textfield',
                id: 'acl_name',
                allowBlank: false,
                fieldLabel:'Rule Name',
            },{
                xtype:'textfield',
                id: 'acl_mac',
                maxLengthText: 17,
                allowBlank: false,
                fieldLabel:'MAC',
            },{
                xtype:'combo',
                id: 'acl_act',
                fieldLabel:'Action',
                displayField:'name',
                valueField: 'value',
                mode: 'local',
                triggerAction: 'all',
                fieldLabel: 'Mode',
                editable: false,
                emptyText:'Select ACL Action',
                store: new Ext.data.Store({
                    reader: new Ext.data.ArrayReader({}, [
                        {name: 'name'},
                        {name: 'value'},
                        ]),
                    data: [['Allow','0'], ['Deny','1']]
                    })

            },{
                xtype:'checkbox',
                id: 'acl_sarp',
                fieldLabel:'Static ARP',
            },{
                xtype:'checkbox',
                id: 'acl_sdhcp',
                fieldLabel:'Static DHCP',
            },{
                xtype:'textfield',
                id: 'acl_ip',
                allowBlank: false,
                fieldLabel:'IP',
            }],
        }],
        buttons: [{
                    text:'Submit',
                    handler: function(){
                        var en = Ext.getCmp('acl_enable');
                        var name = Ext.getCmp('acl_name');
                        var mac = Ext.getCmp('acl_mac');
                        var act = Ext.getCmp('acl_act');
                        var sarp = Ext.getCmp('acl_sarp');
                        var sdhcp = Ext.getCmp('acl_sdhcp');
                        var ip = Ext.getCmp('acl_ip');
                        if( !modify_flag ) {
                            data.insert(0, new Ext.data.Record({
                            enable:  en.getValue(),
                            name: name.getValue(),
                            mac: mac.getValue(),
                            act: act.getValue(),
                            s_arp_enable: sarp.getValue(),
                            s_dhcp_enable: sdhcp.getValue(),
                            ip: ip.getValue(),
                            }));
                        } else {
                            var s = contain.getSelectionModel().getSelections();
                            if(s[0] != undefined ) {
                    data.getAt(data.indexOf(s[0])).set('enable', en.getValue());
                    data.getAt(data.indexOf(s[0])).set('name', name.getValue());
                    data.getAt(data.indexOf(s[0])).set('act', act.getValue());
                    data.getAt(data.indexOf(s[0])).set('mac', mac.getValue());
                    data.getAt(data.indexOf(s[0])).set('s_arp_enable', sarp.getValue());
                    data.getAt(data.indexOf(s[0])).set('s_dhcp_enable', sdhcp.getValue());
                    data.getAt(data.indexOf(s[0])).set('ip', ip.getValue());
                    data.commitChanges();
                            }
                        }
                        op_window.hide();
                    },
                },{
                    text: 'Close',
                    handler: function(){
                        op_window.hide();
                        modify_flag = 0;
                    }
                }],
         listeners:{'beforeshow':function(){
                }},
    });

     var tb = new Ext.Toolbar({
        autoHeight: false,
        Height: 30,
     items:[{
        width: 50,
        text:'Add',
        listeners:{'click':function(){
                var en = Ext.getCmp('acl_enable');
                var name = Ext.getCmp('acl_name');
                var mac = Ext.getCmp('acl_mac');
                var act = Ext.getCmp('acl_act');
                var sarp = Ext.getCmp('acl_sarp');
                var sdhcp = Ext.getCmp('acl_sdhcp');
                var ip = Ext.getCmp('acl_ip');
                        en.setValue(0);
                        name.setValue('');
                        mac.setValue('');
                        act.setValue(0);
                        sarp.setValue(0);
                        sdhcp.setValue(0);
                        ip.setValue('');
                        modify_flag = 0;
            op_window.show()}}
        },'-',{
        width: 50,
        text:'Delete',
        listeners:{'click':function(){
            var s = contain.getSelectionModel().getSelections();
            if(s[0] != undefined ) {
                Ext.Msg.confirm('Message',
                    'Delete the select data?',function (btn) {
                if( btn == 'yes') {
                for(var i = 0, r; r = s[i]; i++){
                data.remove(r);
                }}
                });
            }
            }}
        },'-',{
        width: 50,
        text:'Modify',
        listeners:{'click':function(){
            var s = contain.getSelectionModel().getSelections();
            if(s[0] != undefined ) {
                var en = Ext.getCmp('acl_enable');
                var name = Ext.getCmp('acl_name');
                var mac = Ext.getCmp('acl_mac');
                var act = Ext.getCmp('acl_act');
                var sarp = Ext.getCmp('acl_sarp');
                var sdhcp = Ext.getCmp('acl_sdhcp');
                var ip = Ext.getCmp('acl_ip');
                en.setValue(s[0].get('enable'));
                name.setValue(s[0].get('name'));
                mac.setValue(s[0].get('mac'));
                act.setValue(s[0].get('act'));
                sarp.setValue(s[0].get('s_arp_enable'));
                sdhcp.setValue(s[0].get('s_dhcp_enable'));
                ip.setValue(s[0].get('ip'));
                modify_flag = 1;
                op_window.show();
            }
            }}
        },'-',{
        width: 50,
        text:'Up',
        listeners:{'click':function(){
            alert('up');
            }}
        },'-',{
        width: 50,
        text:'Down',
        listeners:{'click':function(){
            alert('down');
            }}
        }]
    });

      var contain = new Ext.grid.GridPanel({
            title: 'MAC ACL Table',
            height:400,
            cm: cm,
            ds: data,
            bbar: tb,
            renderTo: 'wl_acl_tbl',
            sm: new Ext.grid.RowSelectionModel({
                singleSelect:true,
                }),
            viewConfig: {
                forceFit: true,
            }
      });

});
