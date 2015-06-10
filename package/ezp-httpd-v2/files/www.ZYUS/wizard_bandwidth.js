    <%wizard_load_bandwidth_data();%>
    var bw_type = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'type'},
            {name: 'u_rate'},
            {name: 'd_rate'}
        ]),
        data: bandwidth_data
    });

      bandwidth = new Ext.Panel({
            title: '<font size=4px><%lang("Admin");%> - <%lang("WAN Ethernet");%> - <%lang("WAN Mobile");%> - <font color=#FF0000><%lang("BandWidth");%></font> - <%lang("Wireless");%></font>',
            layout: 'absolute',
            border: false,
            items : [{
                xtype: 'panel',
                x: 130,
                y: 40,
                width: 470,
                height: 320 ,
                border: false,
                items: [{
                    xtype: 'fieldset',
                    title: 'Bandwidth Enable',
                    items:[{
                        xtype: 'radiogroup',
                        id:'wizard_bandwidth_enable',
                        layout: 'column',
                        items: [{
                            boxLabel: 'Enable', name: 'wizard_bandwidth_enable',
                            inputValue: 1
                        },{
                            boxLabel: 'Disable', name: 'wizard_bandwidth_enable',
                            inputValue: 0, checked: true
                        }],
                        listeners:{'change':function(val){
                            var value = this.getValue().getRawValue();
                            if(value == 1){
                                Ext.getCmp('wan_mobile_fieldset').show();
                                Ext.getCmp('wan_wan0_fieldset').show();
                               } else {
                                Ext.getCmp('wan_mobile_fieldset').hide();
                                Ext.getCmp('wan_wan0_fieldset').hide();
                            }
                        }}
                    }]
                    },{
                    xtype: 'fieldset',
                    title: 'WAN Etnernet',
                       hidden: true,
                    id: 'wan_wan0_fieldset',
                    labelWidth: 150,
                    layout: 'form',
                    items: [{
                        xtype: 'combo',
                        width: 170,
                        id: 'bandwidth_wan0',
                        hiddenName: 'wizard_wan0_bw_type',
                        fieldLabel:'Bandwitdh Type',
                        displayField:'name',
                        valueField: 'type', 
                        mode: 'local',
                        triggerAction: 'all',
                        editable: false,
                        emptyText:'Select a bandwidth type...',
                        store: bw_type,
                        listeners:{select:function(combo, record, index){
                            if( this.getValue() == 'Custom' ) {
                                Ext.getCmp('wizard_wan0_bw_ul').enable();
                                Ext.getCmp('wizard_wan0_bw_dl').enable();
                                Ext.getCmp('wizard_wan0_bw_dl').setValue('');
                                Ext.getCmp('wizard_wan0_bw_ul').setValue('');
                            } else {
                                Ext.getCmp('wizard_wan0_bw_ul').disable();
                                Ext.getCmp('wizard_wan0_bw_dl').disable();
                                Ext.getCmp('wizard_wan0_bw_ul').setValue(
                                        bw_type.getAt(index).get('u_rate'));
                                Ext.getCmp('wizard_wan0_bw_dl').setValue(
                                        bw_type.getAt(index).get('d_rate'));
                            }
                        }}
                     } , {
                        xtype: 'textfield',
                        fieldLabel: 'Upload Rate(Kbps)',
                        id: 'wizard_wan0_bw_ul',
                        anchor: '70%',
                        allowNegative: false,
                        region: 'center',
                        disabled: true
                     } , {
                        xtype: 'textfield',
                        fieldLabel: 'Download Rate(Kbps)',
                        id: 'wizard_wan0_bw_dl',
                        anchor: '70%',
                        allowNegative: false,
                        region: 'center',
                        disabled: true
                     }]
                    },{
                    xtype: 'fieldset',
                    title: 'WAN Mobile',
                    labelWidth: 150,
                    id: 'wan_mobile_fieldset',
                       hidden: true,
                    layout: 'form',
                    items: [{
                        xtype: 'combo',
                        width: 170,
                        id: 'bandwidth_wan1',
                        hiddenName: 'wizard_wan1_bw_type',
                        fieldLabel:'Bandwitdh Type',
                        displayField:'name',
                        valueField: 'type', 
                        mode: 'local',
                        triggerAction: 'all',
                        editable: false,
                        emptyText:'Select a bandwidth type...',
                        store: bw_type,
                        listeners:{select:function(combo, record, index){
                            if( this.getValue() == 'Custom' ) {
                                Ext.getCmp('wizard_wan1_bw_ul').enable();
                                Ext.getCmp('wizard_wan1_bw_dl').enable();
                                Ext.getCmp('wizard_wan1_bw_dl').setValue('');
                                Ext.getCmp('wizard_wan1_bw_ul').setValue('');
                            } else {
                                Ext.getCmp('wizard_wan1_bw_ul').disable();
                                Ext.getCmp('wizard_wan1_bw_dl').disable();
                                Ext.getCmp('wizard_wan1_bw_ul').setValue(
                                        bw_type.getAt(index).get('u_rate'));
                                Ext.getCmp('wizard_wan1_bw_dl').setValue(
                                        bw_type.getAt(index).get('d_rate'));
                            }
                        }}
                     } , {
                        xtype: 'textfield',
                        fieldLabel: 'Upload Rate(Kbps)',
                        id: 'wizard_wan1_bw_ul',
                        anchor: '70%',
                        allowNegative: false,
                        region: 'center',
                        disabled: true
                     } , {
                        xtype: 'textfield',
                        fieldLabel: 'Download Rate(Kbps)',
                        id: 'wizard_wan1_bw_dl',
                        anchor: '70%',
                        allowNegative: false,
                        region: 'center',
                        disabled: true
                     }]
                 }]
            }]
      });
    bandwidth.on('show',function(){
        prev = page.mobile_setting;
        next = page.wifi;
    });
