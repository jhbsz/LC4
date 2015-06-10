    <%wizard_load_isp_data();%>
    var mobile_local = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'id'},
            {name: 'name'}
        ]),
        data: local_data
    });
    var mobile_provider = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'id'},
            {name: 'name'},
            {name: 'apn'},
            {name: 'dial'}
        ]),
        data: []
    });

    var mobile_enable = new Ext.form.FieldSet({
        title: 'Mobile WAN Enable',
        items:[{
            xtype:'radiogroup',
            id: 'mobile_enable',
            column: 2,
            items:[{
                boxLabel: 'Enable', name: 'wizard_mobile_enable', inputValue: 1
                },{
                boxLabel: 'Disable', name: 'wizard_mobile_enable', inputValue: 0              , checked: true
                }],
            listeners:{'change':function(val) {
                var value = this.getValue().getRawValue();
                if(value == 1){
                    Ext.getCmp('apn_select').show();
                    Ext.getCmp('apn_select_fieldset').show();
                    Ext.getCmp('wizard_wwan_apn').show();
                    Ext.getCmp('apn_provider').show();                    
                    Ext.getCmp('wizard_wwan_pin').show();
                    Ext.getCmp('wizard_wwan_user').show();
                    Ext.getCmp('wizard_wwan_pw').show();
                    Ext.getCmp('wizard_wwan_dialstr').show();
                } else {
                    Ext.getCmp('apn_select').hide();
                    Ext.getCmp('apn_select_fieldset').hide();
                    Ext.getCmp('wizard_wwan_apn').hide();
                    Ext.getCmp('apn_provider').hide();                    
                    Ext.getCmp('wizard_wwan_pin').hide();
                    Ext.getCmp('wizard_wwan_user').hide();
                    Ext.getCmp('wizard_wwan_pw').hide();
                    Ext.getCmp('wizard_wwan_dialstr').hide();
                }
            }}
        }]
    });
    var apn_select = new Ext.form.FieldSet({
        title: 'APN Type',
        id: 'apn_select_fieldset', 
        hidden: true,
        items:[{
           xtype: 'radiogroup',
           id: 'apn_select',
           columns: 2,
           items:[{
                boxLabel: 'Service Provider',
                name: 'wizard_wwan_apn_type',
                inputValue: 0,
                checked: true
                }, {
                 boxLabel: 'Manual',
                 name: 'wizard_wwan_apn_type',
                 inputValue: 1
                }],
            listeners:{'change':function(rg, check){
                var value = this.getValue().getRawValue();
                if(value == 0) {
                    Ext.getCmp('wizard_wwan_apn').disable();
                    Ext.getCmp('apn_provider').enable();                    
                    Ext.getCmp('wizard_wwan_dialstr').disable();
                } else {
                    Ext.getCmp('wizard_wwan_apn').enable();
                    Ext.getCmp('apn_provider').disable();                    
                    Ext.getCmp('wizard_wwan_dialstr').enable();                
                    Ext.getCmp('wizard_wwan_apn').setValue('');
                    Ext.getCmp('wizard_wwan_dialstr').setValue('');            
                }
            }}
        }]
    });

    mobile_setting  = new Ext.Panel({
        title: '<%wizard_title("Mobile WAN");%>',
        layout: 'absolute',
        border: false,
        items : [{
                xtype: 'panel',
                x: 130,
                y: 40,
                width: 470,
                height: 320 ,
                layout: 'form',
                border: false,
                labelWidth: 100,
                items: [
                        mobile_enable,
                        apn_select,
                     {
                       xtype: 'combo',
                       width: 170,
                       displayField:'name',
                       id: 'apn_provider',
                       hiddenName: 'wizard_wwan_isp',
                       valueField: 'id', 
                       mode: 'local',
                       triggerAction: 'all',
                       editable: false,
                       emptyText:'Select a Provider...',
                       fieldLabel:'Service Provider',
                       hidden: true,
                       store: mobile_provider,
                       listeners:{'select':function(){
                            var isp=this.getValue();
                            var apn=mobile_provider.getAt(isp).get('apn');
                            var dial=mobile_provider.getAt(isp).get('dial');
                            Ext.getCmp('wizard_wwan_apn').setValue(apn);
                            Ext.getCmp('wizard_wwan_dialstr').setValue(dial);
                        }} 
                    }, {
                        xtype: 'textfield',
                        id: 'wizard_wwan_apn',
                        fieldLabel: 'APN',
                        anchor: '50%',
                        region: 'center',
                        hidden: true,
                        disabled: true
                    }, {
                        xtype: 'textfield',
                        id: 'wizard_wwan_dialstr',
                        fieldLabel: 'Dial Number',
                        anchor: '50%',
                        region: 'center',
                        hidden: true,
                        disabled: true
                    }, {
                        xtype: 'textfield',
                        fieldLabel: 'PIN Code',
                        id: 'wizard_wwan_pin',
                        width:  250,
                       hidden: true,
                        emptyText:'If need PIN Code, please input here',
                        region: 'center'
                    }, {
                        xtype: 'textfield',
                        fieldLabel: 'User Name',
                        id: 'wizard_wwan_user',
                        width:  250,
                        emptyText:'If need username, please input here',
                        hidden: true,
                        region: 'center'
                    }, {
                        xtype: 'textfield',
                        fieldLabel: 'Password',
                        width:  250,
                        id: 'wizard_wwan_pw',
                        emptyText:'If need password, please input here',
                        hidden: true,
                        region: 'center'
                    }
                ]
            }]
      });
    mobile_setting.on('show',function(){
       mobile_provider.loadData(isp_data_12);
       prev = page.wan_setting;
       next = page.time;
    });
