    var mobile_conn_type = [[wan_type.wwan,'3G Mobile Internet'],[wan_type.directip,'HSPA+ Super Speed']];
    var def_conn_type = [[wan_type.dhcp,'DHCP'],[wan_type.pppoe,'PPPoE'],[wan_type.staticip,'STATIC IP']];
    var conn_type = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'id'},
            {name: 'type'}
        ]),
        data: []
    });

      wan_setting = new Ext.Panel({
            title: '<%wizard_title("Ethernet WAN");%>',
            layout: 'absolute',
            border: false,
            items: [{
                xtype: 'panel',
                x: 130,
                y: 40,
                layout: 'form',
                width: 470,
                border: false,
                items:[{
                    xtype: 'fieldset',
                    title: 'Ethernet WAN Enable',
                    items:[{
                        xtype: 'radiogroup',
                        id:'wan_wan0_enable',
                        layout: 'column',
                        items: [{
                            boxLabel: 'Enable', name: 'wizard_wan0_enable',
                            inputValue: 1, checked: true
                        },{
                            boxLabel: 'Disable', name: 'wizard_wan0_enable',
                            inputValue: 0
                        }],
                        listeners:{'change':function(val){
                            var value = this.getValue().getRawValue();
                            if(value == 1){
                                 Ext.getCmp('wan_type').show();
                                 Ext.getCmp('wan_type').setValue('DHCP');
                               } else {
                                Ext.getCmp('wan_type').hide();
                                Ext.getCmp('conn_set').getLayout().setActiveItem(0);
                            }
                        }}
                    }]
                    },{
                    xtype: 'combo',
                    id: 'wan_type',
                    labelWidth: 100,
                    hiddenName: 'wizard_wan_type_value',
                    allowBlank: false,
                    width: 250,
                    displayField:'type',
                    fieldLabel: '<%lang("Connection Type");%>',
                    valueField:'id',
                    mode: 'local',
                    triggerAction: 'all',
                    editable: false,
                    emptyText:'<%lang("Select a connectint type...");%>',
                    store: conn_type,
                    value: 'DHCP',
                    listeners:{'select':function(){
                        var type = this.getValue();
                        var temp = Ext.getCmp('conn_set');
                        switch (type) {
                        case wan_type.dhcp:
                            temp.getLayout().setActiveItem(0);
                            break;
                        case wan_type.pppoe:
                            temp.getLayout().setActiveItem(1);
                            break;
                        case wan_type.staticip:
                            temp.getLayout().setActiveItem(2);
                            break;
                        case wan_type.wwan:
                            temp.getLayout().setActiveItem(3);
                            break;
                        default:
                            temp.getLayout().setActiveItem(0);
                            break;
                        }
                    },
                    'focus':function(){ 
                            conn_type.removeAll();
                            conn_type.loadData(def_conn_type, true);
                    }
                    }
                }]
            },{
                xtype: 'panel',
                autoEl: 'div',
                x: 155,
                y: 160,
                border: false,
                width: 400,
                height: 150,
                layout: 'card',
                id: 'conn_set',
                activeItem: 0,
                items: [ {
                        xtype: 'panel',
                        layout: 'form',
                        border: false
                    }, {
                        xtype: 'panel',
                        layout: 'form',
                        border: false,
                        items: [{
                                xtype: 'textfield',
                                id: 'wizard_wan0_ppp_username',
                                fieldLabel: '<%lang("User Name");%>',
                                anchor: '80%',
                                allowBlank: false,
                                region: 'center'
                            },{
                                xtype: 'textfield',
                                inputType: 'password',
                                id: 'wizard_wan0_ppp_passwd',
                                fieldLabel: '<%lang("Password");%>',
                                anchor: '80%',
                                allowBlank: false,
                                region: 'center'
                            },{
                                xtype: 'textfield',
                                inputType: 'password',
                                id: 'wizard_wan0_ppp_passwd_confirm',
                                fieldLabel: '<%lang("Password Confirm");%>',
                                anchor: '80%',
                                allowBlank: false,
                                region: 'center',
                                vtype: 'password',
                                initialPassField: 'wizard_wan0_ppp_passwd'
                            }]
                    }, {
                        xtype: 'panel',
                        layout: 'form',
                        labelWidth: 150,
                        border: false,
                        items: [ {
                                xtype: 'textfield',
                                id: 'wizard_wan0_static_ip',
                                fieldLabel: '<%lang("External IP Address");%>',
                                anchor: '80%',
                                allowBlank: false,
                                region: 'center',
                                vtype: 'ipaddr'
                            }, {
                                xtype: 'textfield',
                                id: 'wizard_wan0_static_mask',
                                fieldLabel: '<%lang("Netmask");%>',
                                anchor: '80%',
                                allowBlank: false,
                                region: 'center',
                                vtype: 'netmask'
                            }, {
                                xtype: 'textfield',
                                id: 'wizard_wan0_static_gateway',
                                fieldLabel: '<%lang("Gateway");%>',
                                anchor: '80%',
                                allowBlank: false,
                                region: 'center',
                                vtype: 'ipaddr'
                            }, {
                                xtype: 'textfield',
                                id: 'wizard_wan0_static_dns1',
                                fieldLabel: '<%lang("Static DNS 1");%>',
                                anchor: '80%',
                                allowBlank: false,
                                region: 'center',
                                vtype: 'ipaddr'
                            }, {
                                xtype: 'textfield',
                                id: 'wizard_wan0_static_dns2',
                                fieldLabel: '<%lang("Static DNS 2");%>',
                                anchor: '80%',
                                region: 'center',
                                vtype: 'ipaddr'
                            } ]
                    }, {
                        xtype: 'panel',
                        layout: 'form',
                        border: false,
                        html: '<%lang("Press Next to configure mobile wan setting");%>'
                    }]
            }
        ]
      });
   wan_setting.on('show',function(){
     prev = page.admin;
     next = page.mobile_setting;
    });
