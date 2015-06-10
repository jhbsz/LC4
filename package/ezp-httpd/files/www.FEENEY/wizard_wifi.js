      var wl_setting = new Ext.Panel({
            layout: 'form',
            x:160,
            y:80,
            height: 300,
            id:'wl_setting_pannel',
            width: 500,
            border: false,
            hidden: true,
            labelWidth: 150,
            items:[ {
                xtype: 'textfield',
                fieldLabel: '<%lang("Wireless SSID Name");%>',
                id: 'wizard_ssid',
                anchor: '50%',
                region: 'center',
                value: '<%nvg_attr_get("wl0_ssid","wl0_ssid_rule","0","ssid");%>'
            },{
                xtype: 'checkbox',
                id: 'wizard_wl_key_enable',
                fieldLabel: '<%lang("Wireless Security Enable");%>',
                boxLabel: '<%lang("Enable");%>',
                listeners:{'check':function(){
                       if(this.getValue()){
                            Ext.getCmp('wizard_wl_key').enable();
                            Ext.getCmp('wl_sec').enable();
                            Ext.getCmp('wl_sec_method').enable();
                            Ext.getCmp('wl_sec_tip').show();
                        } else {
                            Ext.getCmp('wizard_wl_key').disable();
                            Ext.getCmp('wl_sec').disable();
                            Ext.getCmp('wl_sec_method').disable();
                            Ext.getCmp('wl_sec_tip').hide();
                        }
                    }}
            },{
                xtype: 'combo',
                id: 'wl_sec',
                hiddenName: 'wizard_wl_sec_value',
                width: 100,
                displayField:'name',
                valueField:'id',
                mode: 'local',
                triggerAction: 'all',
                value: 0,
                editable: false,
                disabled: true,
                fieldLabel: '<%lang("Security Mode");%>',
                store: new Ext.data.Store({
                        reader: new Ext.data.ArrayReader({}, [
                        {name: 'id'},
                        {name: 'name'}
                        ]),
                        data: [[0, 'WEP'],[1, 'WPA'],[2, 'WPA2']]
                    }),
                listeners:{'select':function(){
                    var val =  this.getValue();
                    var cmp = Ext.getCmp('wl_sec_tip');
                    var method = Ext.getCmp('wl_sec_method');
                    switch (val) {
                        case 0:
                            cmp.setText('(<%lang("The WEP Keys are ASCII strings of 5/13 digits, or HEX strings of 10/26 digits.");%>)');
                            method.hide();
                            break;
                        case 1:
                        case 2:
                            cmp.setText('(<%lang("The Key is an ASCII string of 8-63 digits, or a HEX string of 64 digits.");%>)');
                            method.show();
                            break;
                    }
                }}
            },{
                xtype: 'combo',
                id: 'wl_sec_method',
                hiddenName: 'wizard_wl_sec_method',
                width: 100,
                displayField:'name',
                valueField:'id',
                mode: 'local',
                triggerAction: 'all',
                value: 'aes',
                editable: false,
                fieldLabel: '<%lang("Encryption Method");%>',
                hidden: true,
                store: new Ext.data.Store({
                        reader: new Ext.data.ArrayReader({}, [
                        {name: 'id'},
                        {name: 'name'}
                        ]),
                        data: [['aes', 'AES'],['tkip', 'TKIP']]
                    })
            },{
                xtype: 'textfield',
                inputType: 'password',
                fieldLabel: '<%lang("Key");%>',
                id: 'wizard_wl_key',
                width: 200,
                region: 'center',
                vtype: 'wireless',
                keytype: 'wl_sec',
                disabled: true
            },{
                xtype: 'label',
                id: 'wl_sec_tip',
                style: 'font-size:11px',
                hidden: true,
                text: '(<%lang("The WEP Keys are ASCII strings of 5/13 digits, or HEX strings of 10/26 digits.");%>)'
           }]
        });

       wifi = new Ext.Panel({
            title: '<%wizard_title("Wireless");%>',
            layout: 'absolute',
            border: false,
            items : [{
                xtype: 'panel',
                layout: 'form',
                x: 130,
                y: 40,
                border: false,
                items:[{
                    xtype: 'fieldset',
                    title: '<%lang("Wireless Connection");%>',
                    height: 50,
                    width: 400,
                    labelWidth: 90,
                    x: 160,
                    y: 10,
                    items: [{
                        xtype: 'radiogroup',
                        id: 'wifi_enable',
                        columns: 2,
                        items:[{
                            name: 'wizard_wl_enable',
                            boxLabel: '<%lang("Enable");%>',
                            inputValue: 1
                        },{
                            name: 'wizard_wl_enable',
                            boxLabel: '<%lang("Disable");%>',
                            checked: true,
                            inputValue: 0
                        }],
                        listeners:{'change':function(){
                            var value = this.getValue().getRawValue();
                            if(value == 1){
                                 Ext.getCmp('wl_setting_pannel').show();
                               } else {
                                Ext.getCmp('wl_setting_pannel').hide();
                            }
                        }}
                    }]
                    },wl_setting
                ]
            }]
        });
    wifi.on('show',function(){
        prev = page.time;
        next = page.sum;
    });
