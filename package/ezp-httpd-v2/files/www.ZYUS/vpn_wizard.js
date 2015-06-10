var formPost;

var vpn_type,vpn_pptp,vpn_ipsec,vpn_ipsec_n1,vpn_ipsec_n2,vpn_ipsec_n3;
var vpn_ipsec_r1,vpn_ipsec_r2,vpn_ipsec_r3,vpn_finish;
var now = 0;        
var vpn_type_data = [['PPTP',0],['IPSEC',1]];

var page = {
    type : 0,
    pptp : 1,
    ipsec : 2,
    ipsec_n1 : 3,
    ipsec_n2 : 4,
    ipsec_n3 : 5,
    ipsec_r1 : 6,
    ipsec_r2 : 7,
    ipsec_r3 : 8,
    finish : 9
}
var valid = 1;

Ext.apply(Ext.form.VTypes, {
    asciicheck : function(val, field) {
        var str1,str2,err,j;
        str1='0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        for(i=0;i<val.length;i++){
            j = i+1;
            str2 =val.substring(i,j);
            err = str1.indexOf(str2);
            if(err<0){
                valid=0;
                return false;
            }
        }
        return true;
    },
    asciicheckText : 'Please use (0-9), (a-z) or (A-Z).',
    asciicheckkey : function(val, field) {
        var str1,str2,err,j;
        if (val.length < 8 || val.length > 40) {
            valid=0;
            return false;
        }
        str1='0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
        for(i=0;i<val.length;i++){
            j = i+1;
            str2 =val.substring(i,j);
            err = str1.indexOf(str2);
            if(err<0){
                valid=0;
                return false;
            }
        }
        return true;
    },
    asciicheckkeyText : 'Please use (0-9), (a-z) or (A-Z), and key length should between 8 - 40',
    netmask : function(val) {
        var correct_range =
                 {128:1,192:1,224:1,240:1,248:1,252:1,254:1,255:1,0:1};
        var m = val.split('.');

        for (var i = 0; i <= 3; i ++) {
            if (!(m[i] in correct_range)) {
                valid=0;
                return false;
            }
        }
        if ((m[0] == 0) || (m[0] != 255 && m[1] != 0) || (m[1] != 255 && m[2] !=
    0) || (m[2] != 255 && m[3] != 0)) {
                valid=0;
                return false;
        }
        return true;
    },
   netmaskText : 'Netmask do not valid',
   ipaddr : function(val) {
       var ipaddr = val.replace( /\s/g, "");
       var re = /^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/;
       if (re.test(ipaddr)) {
           var parts = ipaddr.split(".");
           if (parseInt(parseFloat(parts[0])) == 0) {
               valid=0;
               return false;
           }
           if (parseInt(parseFloat(parts[3])) == 0) {
               valid=0;
               return false;
           }
           for (var i=0; i<parts.length; i++) {
               if (parseInt(parseFloat(parts[i])) > 255){
                   valid=0;
                   return false;
               }
           }
           return true;
       } else {
           valid=0;
           return false;
       }
       return true;
    },
    ipaddrText : 'IP Address do not valid'
});

var next = 0,prev = 0;

var navHandler = function(direction) {     
    var btnNext = Ext.getCmp('move-next');     
    var btnBack = Ext.getCmp('move-prev');     
    var btnFinish = Ext.getCmp('move-finish');     
    var contain = Ext.getCmp('wiarzd_contain');
    if (now == page.type) {     
        if (Ext.getCmp('type').getValue() == 0) {
            if (pptp_num >= pptp_max) {
                    Ext.MessageBox.alert('Alert', 'Can not add new pptp rule, the max pptp rule num is ' + pptp_max);
                    return;
                }
        } else {
            if (ipsec_num >= ipsec_max) {
                    Ext.MessageBox.alert('Alert', 'Can not add new ipsec rule, the max ipsec rule num is ' + ipsec_max);
                    return;
                }
        }
    }
    if (direction == -1) {     
        now = prev;
    } else if (direction == 0) {
        var F;
        F = document.getElementById('wizard_form');
        to_submit(F);
    } else if (direction == 2) {
       window.location.reload(); 
    } else {
        valid=1;
        if (now == page.pptp) {
            Ext.getCmp('pptp_user').validate();
            Ext.getCmp('pptp_pw').validate();
            if ((Ext.getCmp('pptp_user').getValue() == "") || (Ext.getCmp('pptp_pw').getValue() == "")) valid=0;
        }
        else if (now == page.ipsec) {
            Ext.getCmp('wizard_vpn_ipsec_name').validate();
            if (Ext.getCmp('wizard_vpn_ipsec_name').getValue() == "") valid=0;
            if(Ext.getCmp('ipsec_mode').getValue() == 'net2net') {
                next = page.ipsec_n1;
            } else {
                next = page.ipsec_r1;
            }
        }
        else if (now == page.ipsec_n1) {
            Ext.getCmp('nlocal_ip').validate();
            Ext.getCmp('wizard_vpn_ipsec_nlocal_mask').validate();
            Ext.getCmp('nremote_gw').validate();
            Ext.getCmp('nremote_ip').validate();
            Ext.getCmp('wizard_vpn_ipsec_nremote_mask').validate();
        }
        else if (now == page.ipsec_n2) {
            Ext.getCmp('npkey').validate();
            if (Ext.getCmp('npkey').getValue() == "") valid=0;
            if(Ext.getCmp('nadv_opt').getValue()) {
                next = page.ipsec_n3;
            } else {
                next = page.finish;
            }
        }
        else if (now == page.ipsec_r1) {
            Ext.getCmp('rlocal_ip').validate();
            Ext.getCmp('wizard_vpn_ipsec_rlocal_mask').validate();
        }
        else if (now == page.ipsec_r2) {
            Ext.getCmp('rpkey').validate();
            if (Ext.getCmp('rpkey').getValue() == "") valid=0;
            if(Ext.getCmp('radv_opt').getValue()) {
                next = page.ipsec_r3;
            } else {
                next = page.finish;
            }
        }
        if (valid==0) alert('The configuration values are incorrect. Please input the right values.');
        else now = next;
    }  
    if (now == page.type) {
        btnBack.hide();     
    } else {     
        btnBack.show();     
    }     
    if (now == page.finish) {
        btnNext.hide();
        btnFinish.show();
    } else {
        btnFinish.hide();
        btnNext.show();
    } 
    contain.getLayout().setActiveItem(now);     
}

Ext.onReady(function(){
    Ext.QuickTips.init();
    Ext.form.Field.prototype.msgTarget = 'side'

   var vpn_ds = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'value'},
        ]),
        data: vpn_type_data
    });

      vpn_type = new Ext.Panel({
            title: 'VPN Type',
            bodyStyle: 'padding:15px',
            layout: 'absolute',
                border: false,
            items: [{
                xtype: 'combo',
                id: 'type',
                displayField:'name',
                hiddenName: 'wizard_vpn_type',
                valueField:'value',
                mode: 'local',
                triggerAction: 'all',
                editable: false,
                emptyText:'Select VPN Type...',
                x: 150,
                y: 70,
                width: 300,
                store: vpn_ds,
                listeners:{'select':function(){
                    var type = this.getValue();
                    if (type == 0) {
                        prev = page.type;
                        next = page.pptp;
                    } else if ( type == 1) {
                        prev = page.type;
                        next = page.ipsec;
                    }
                }}
            }],
             html: 'Please select your VPN type<br><br><br><br><br> Please note that PPTP VPN cannot be used if Hardware Accelerator is enabled.',
            listeners:{'show':function(){
                prev = 0;
                next = page.pptp;
                Ext.getCmp('type').setValue(0);
            }}
      });
      vpn_type.on('show',function(){
              Ext.getCmp('move-prev').hide();
              Ext.getCmp('move-finish').hide();
      });

      vpn_pptp = new Ext.Panel({
            title: 'Step 1 of 1 - PPTP Setting',
            bodyStyle: 'padding:15px',
            layout: 'absolute',
                border: false,
            html: 'Please input ID and Password into textbox' + 
                  ' to create new vpn user',
            listeners:{'show':function(){
                prev = page.type;
                next = page.finish;
                }},
            items:[{
                xtype:'panel',
                layout: 'form',
                border: false,
                x: 150,
                y: 100,
                items:[{
                    xtype:'textfield',
                    id: 'pptp_user',
                    allowBlank: false,
                    vtype: 'asciicheck',
                    fieldLabel: 'User Name'
                },{
                    xtype:'textfield',
                    inputType: 'password',
                    id: 'pptp_pw',
                    allowBlank: false,
                    vtype: 'asciicheck',
                    fieldLabel: 'Password'
                }]
            }]
      });
      var config = new Ext.Panel({
            x: 150,
            y: 100,
            border: false,
            layout: 'form',
            items:[{
                xtype: 'combo',
                fieldLabel: 'IPSec Mode',
                id: 'ipsec_mode',
                displayField:'name',
                valueField:'value',
                mode: 'local',
                triggerAction: 'all',
                editable: false,
                hiddenName: 'wizard_vpn_ipsec_mode',
                emptyText:'Select IPSec Mode...',
                value: 'net2net',
                width: 100,
                store: new Ext.data.SimpleStore({
                   fields: ['name','value'], 
                    data: [['Site-to-Site','net2net'],['Remote User' ,'rw_server']]
                    }),
                listeners:{'select':function(){
                    if(this.getValue() == 0) {
                        next = page.ipsec_n1;
                    } else {
                        next = page.ipsec_r1;
                    }
                }}
           }, {
                xtype: 'textfield',
                id: 'wizard_vpn_ipsec_name',
                allowBlank: false,
                vtype: 'asciicheck',
                fieldLabel: 'Rule Name'
          }] 
      });
      vpn_ipsec = new Ext.Panel({
            title: 'Step 1 of 4 - IPSec Mode',
            layout: 'absolute',
            border: false,
            bodyStyle: 'padding:15px',
            html: 'Please select IPSec connection mode',
            listeners:{'show':function(){
                next = page.ipsec_n1;
                prev = page.type;
                }},
            items:[config]
      });

      vpn_ipsec_n1 = new Ext.Panel({
            title: 'Step 2 of 4 - IPSec Site to Site Setting',
            layout: 'absolute',
            border: false,
            bodyStyle: 'padding:15px',
            html: 'Please setup local and remote base setting',
            listeners:{'show':function(){
                prev = page.ipsec;
                next = page.ipsec_n2;
                }},
            items:[{
                x: 150,
                y: 100,
                border: false,
                items:[{
                    xtype: 'fieldset',
                    title: 'Local setting',
                    width: 400,
                    labelWidth: 150,
                    layout: 'form',
                    items:[{
                            xtype: 'combo',
                            fieldLabel: 'Local External Interface',
                            id: 'nif',
                            displayField:'name',
                            valueField:'value',
                            value: 'wan0',
                            mode: 'local',
                            triggerAction: 'all',
                            hiddenName: 'wizard_vpn_ipsec_nif',
                            editable: false,
                            emptyText:'Select IPSec Mode...',
                            width: 150,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: [['Ethernet WAN','wan0'],['Mobile WAN' ,'wan1']]
                            })
                        }, { 
                            xtype: 'textfield',
                            id: 'nlocal_ip',
                            fieldLabel: 'Local Internal IP Address',
                            value: '<%nv_attr_get("lan_static_rule", 0,"ipaddr");%>',
                            allowBlank: false,
                            vtype: 'ipaddr'
                        }, { 
                            xtype: 'textfield',
                            fieldLabel: 'Local Netmask',
                            id: 'wizard_vpn_ipsec_nlocal_mask',
                            value: '<%wizard_get_lan_mask();%>',
                            allowBlank: false,
                            vtype: 'netmask'
                        }]
                }, { 
                    xtype: 'fieldset',
                    title: 'Remote setting',
                    width: 400,
                    labelWidth: 150,
                    layout: 'form',
                    items:[{
                        xtype: 'textfield',
                        id: 'nremote_gw',
                        fieldLabel: 'Remote Gateway',
                        allowBlank: false,
                        vtype: 'ipaddr'
                        },{
                            xtype: 'textfield',
                            id: 'nremote_ip',
                            fieldLabel: 'Remote Subnet IP',
                            allowBlank: false,
                            vtype: 'ipaddr'
                       }, { 
                            xtype: 'textfield',
                            fieldLabel: 'Remote Netmask',
                            id: 'wizard_vpn_ipsec_nremote_mask',
                            allowBlank: false,
                            vtype: 'netmask'
                        }]
            }]
            }]
      });
      vpn_ipsec_n2 = new Ext.Panel({
            title: 'Step 3 of 4 - IPSec Site to Site Setting',
            layout: 'absolute',
            border: false,
            bodyStyle: 'padding:15px',
            html: 'Please setup connetion setting',
            listeners:{'show':function(){
                prev = page.ipsec_n1;
                next = page.finish;
                }},
            items:[{
                x: 150,
                y: 100,
                border: false,
                layout: 'form',
                labelWidth: 150,
                items:[{
                   xtype: 'textfield',
                   inputType: 'password',
                   id: 'npkey',
                   allowBlank: false,
                   maxLength: 40,
                   minLength: 8,
                   vtype: 'asciicheckkey',
                   fieldLabel: 'Preshared Key'
                },{
                   xtype: 'label',
                   style: 'font-size:11px',
                   text: 'Key length should between 8 - 40'
                },{ 
                  xtype: 'checkbox',
                  id: 'con_init',
                  fieldLabel: 'Connection Initiation'
                },{
                  xtype: 'checkbox',
                  fieldLabel: 'Advanced Options',
                    id: 'nadv_opt',
                    listeners:{'check':function(){
                         if(this.getValue()) {
                            next = page.ipsec_n3;
                            } else {
                            next = page.finish;
                            }
                        }}
                }]
            }]
      });
       var auth_data = [['MD5', 'md5'],['SHA1','sha1']];
       var ept_data = [['DES','des'],['3DES', '3des'],['AES','aes128']];
      var g1_key_data = [['DH1','dh1'],['DH2','dh2'],['DH5','dh5']];
      var g2_key_data = [['None', 'none'],['DH1','dh1'],['DH2','dh2'],['DH5','dh5']];
      vpn_ipsec_n3 = new Ext.Panel({
            title: 'Step 4 of 4 - IPSec Site to Site Advance Setting',
            layout: 'absolute',
            bodyStyle: 'padding:15px',
            html: 'Please setup connetion setting',
                border: false,
            listeners:{'show':function(){
                prev = page.ipsec_n2;
                next = page.finish;
                }},
            items:[{
                x:150,
                y:100,
                border: false,
                layout:'form',
                items:[{                
                xtype: 'fieldset',
                labelWidth: 200,
                layout: 'form',
                title: 'Phase 1 setting', 
                items:[{
                    xtype: 'textfield',
                    id: 'np1_left_id',
                    fieldLabel: 'Phase 1 Left ID',
                    value: '0.0.0.0'
                  },{  
                    xtype: 'textfield',
                    id: 'np1_right_id',
                    fieldLabel: 'Phase 1 Right ID',
                    value: '0.0.0.0'
                  },{  
                    xtype: 'combo',
                            fieldLabel: 'Phase 1 Authentication',
                            displayField:'name',
                            valueField:'value',
                            value: 'sha1',
                            id: 'np1_auth',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            hiddenName: 'wizard_vpn_ipsec_np1_auth',
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: auth_data
                                })
                    },{
                            xtype: 'combo',
                            fieldLabel: 'Phase 1 Encryption',
                            displayField:'name',
                            valueField:'value',
                            id: 'np1_ept',
                            value: '3des',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            hiddenName: 'wizard_vpn_ipsec_np1_ept',
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: ept_data
                                })
                    },{
                    xtype: 'combo',
                            fieldLabel: 'Phase 1 Group Key Management',
                            displayField:'name',
                            valueField:'value',
                            value: 'dh2',
                            id: 'np1_gkey',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            width: 100,
                            hiddenName: 'wizard_vpn_ipsec_np1_gkey',
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: g1_key_data
                                })
                }]
            },{
                xtype: 'fieldset',
                layout: 'form',
                labelWidth: 230,
                title: 'Phase 2 setting', 
                items:[{
                    xtype: 'combo',
                            fieldLabel: 'Phase 2 Authentication',
                            displayField:'name',
                            valueField:'value',
                            id: 'np2_auth',
                            value: 'sha1',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            width: 100,
                            hiddenName: 'wizard_vpn_ipsec_np2_auth',
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: auth_data
                                })
                    },{
                    xtype: 'combo',
                            fieldLabel: 'Phase 2 Encryption',
                            displayField:'name',
                            valueField:'value',
                            id: 'np2_ept',
                            value: '3des',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            hiddenName: 'wizard_vpn_ipsec_np2_ept',
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: ept_data
                                })
                    },{
                    xtype: 'combo',
                            fieldLabel: 'Phase 2 Group Key Management(PFS)',
                            displayField:'name',
                            valueField:'value',
                            id: 'np2_gkey',
                            value: 'none',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            width: 100,
                            hiddenName: 'wizard_vpn_ipsec_np2_gkey',
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: g2_key_data
                                })
                    
                }]
            }]
        }]
      });
      vpn_ipsec_r1 = new Ext.Panel({
            title: 'Step 2 of 4 - IPSec Remote User Setting',
            layout: 'absolute',
                border: false,
            bodyStyle: 'padding:15px',
            html: 'Please setup local base setting',
            listeners:{'show':function(){
                prev = page.ipsec;
                next = page.ipsec_r2;
                }},
            items:[{
                x: 150,
                y: 100,
                xtype: 'fieldset',
                title: 'Local setting',
                width: 400,
                labelWidth: 150,
                layout: 'form',
                items:[{
                    xtype: 'combo',
                    fieldLabel: 'Local External Interface',
                    id: 'rif',
                    displayField:'name',
                    valueField:'value',
                    value: 'wan0',
                    mode: 'local',
                    triggerAction: 'all',
                    hiddenName: 'wizard_vpn_ipsec_rif',
                    editable: false,
                    emptyText:'Select IPSec Mode...',
                    width: 100,
                    store: new Ext.data.SimpleStore({
                             fields: ['name','value'], 
                            data: [['Ethernet WAN','wan0'],['Mobile WAN' ,'wan1']]
                    })
                 }, { 
                    xtype: 'textfield',
                    id: 'rlocal_ip',
                    fieldLabel: 'Local Internal IP Address',
                    allowBlank: false,
                    vtype: 'ipaddr'
                 }, { 
                    xtype: 'textfield',
                    id: 'wizard_vpn_ipsec_rlocal_mask',
                    fieldLabel: 'Local Netmask',
                    allowBlank: false,
                    vtype: 'netmask'
                 }]
            }]
      });
      vpn_ipsec_r2 = new Ext.Panel({
            title: 'Step 3 of 4 - IPSec Remote User  Setting',
            layout: 'absolute',
                border: false,
            bodyStyle: 'padding:15px',
            html: 'Please setup connetion setting',
            listeners:{'show':function(){
                prev = page.ipsec_r1;
                next = page.finish;
                }},
            items:[{
                x: 150,
                y: 100,
                border: false,
                layout: 'form',
                labelWidth: 150,
                items:[{
                   xtype: 'textfield',
                   inputType: 'password',
                   id: 'rpkey',
                   allowBlank: false,
                   maxLength: 40,
                   minLength: 8,
                   vtype: 'asciicheckkey',
                   fieldLabel: 'Preshared Key'
                },{
                   xtype: 'label',
                   style: 'font-size:11px',
                   text: 'Key length should between 8 - 40'
                },{
                  xtype: 'checkbox',
                  fieldLabel: 'Advanced Options',
                    id: 'radv_opt',
                    listeners:{'check':function(){
                         if(this.getValue()) {
                            next = page.ipsec_r3;
                            } else {
                            next = page.finish;
                            }
                        }}
                }]
            }]
      });
      vpn_ipsec_r3 = new Ext.Panel({
            title: 'Step 4 of 4 - IPSec Remote User Advance Setting',
            layout: 'absolute',
                border: false,
            bodyStyle: 'padding:15px',
            html: 'Please setup connetion setting',
            listeners:{'show':function(){
                prev = page.ipsec_r2;
                next = page.finish;
                }},
            items:[{
                x:150,
                y:100,
                border: false,
                layout:'form',
                items:[{                
                xtype: 'fieldset',
                labelWidth: 200,
                layout: 'form',
                title: 'Phase 1 setting', 
                items:[{
                    xtype: 'textfield',
                    id: 'rp1_id',
                    fieldLabel: 'Phase 1 ID'
                  },{  
                    xtype: 'combo',
                   fieldLabel: 'Phase 1 Authentication',
                   displayField:'name',
                   id: 'rp1_auth',
                   valueField:'value',
                   value: 'md5',
                   mode: 'local',
                   triggerAction: 'all',
                   hiddenName: 'wizard_vpn_ipsec_rp1_auth',
                    editable: false,
                    width: 100,
                    store: new Ext.data.SimpleStore({
                        fields: ['name','value'], 
                        data: auth_data
                        })
                    },{
                    xtype: 'combo',
                            fieldLabel: 'Phase 1 Encryption',
                            displayField:'name',
                            valueField:'value',
                            value: '3des',
                            id: 'rp1_ept',
                            mode: 'local',
                   hiddenName: 'wizard_vpn_ipsec_rp1_ept',
                            triggerAction: 'all',
                            editable: false,
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: ept_data
                                })
                    },{
                    xtype: 'combo',
                            fieldLabel: 'Phase 1 Group Key Management',
                            displayField:'name',
                            valueField:'value',
                            id: 'rp1_gkey',
                   hiddenName: 'wizard_vpn_ipsec_rp1_gkey',
                            value: 'dh2',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: g1_key_data
                                })
                }]
            },{
                xtype: 'fieldset',
                layout: 'form',
                labelWidth: 230,
                title: 'Phase 2 setting', 
                items:[{
                    xtype: 'combo',
                            fieldLabel: 'Phase 2 Authentication',
                            displayField:'name',
                            valueField:'value',
                            value: 'md5',
                            mode: 'local',
                            triggerAction: 'all',
                            id: 'rp2_auth',
                   hiddenName: 'wizard_vpn_ipsec_rp2_auth',
                            editable: false,
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: auth_data
                                })
                    },{
                    xtype: 'combo',
                            fieldLabel: 'Phase 2 Encryption',
                            displayField:'name',
                            valueField:'value',
                            id: 'rp2_ept',
                   hiddenName: 'wizard_vpn_ipsec_rp2_ept',
                            value: '3des',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: ept_data
                                })
                    },{
                    xtype: 'combo',
                            fieldLabel: 'Phase 2 Group Key Management(PFS)',
                            displayField:'name',
                            valueField:'value',
                            id: 'rp2_gkey',
                   hiddenName: 'wizard_vpn_ipsec_rp2_gkey',
                            value: 'dh2',
                            mode: 'local',
                            triggerAction: 'all',
                            editable: false,
                            width: 100,
                            store: new Ext.data.SimpleStore({
                                fields: ['name','value'], 
                                data: g2_key_data
                                })
                }]
            }]
            }]
      });
   var sum_data = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'value'}
        ]),
        data: []
    });
  var cm = new Ext.grid.ColumnModel([
        {header: 'name', dataIndex: 'name'},
        {header: 'value', dataIndex: 'value',
                 renderer:function(val) {
                    if (val == '') {
                        return '<p style="color: rgb(255, 0, 0);">No select or input value !</p>';
                    }   
                    return val;
                }}
        ]);


      
      vpn_finish = new Ext.Panel({
            title: 'Finish - VPN Setting Completed',
            layout: 'absolute',
            enableHdMenu: false,
            border: false,
            html: '<br>Please check your setting',
            listeners:{'activate':function(){
                var type = Ext.getCmp('type').getValue();
                var mode = Ext.getCmp('ipsec_mode').getValue();
                var radv = Ext.getCmp('radv_opt').getValue();
                var nadv = Ext.getCmp('nadv_opt').getValue();
                sum_data.removeAll();
                if(type == 0) {
                    prev = page.pptp;
                    sum_data.loadData([['VPN Type','PPTP'],
                            ['User Name', Ext.getCmp('pptp_user').getValue()]
                 //           ['Password', Ext.getCmp('pptp_pw').getValue()]
                    ], true);
                } else if (mode == 'net2net') {
                    sum_data.loadData([['VPN Type','IPSec'],
             ['IPSec Mode','Site to Site'],
             ['VPN Rule Name', Ext.getCmp('wizard_vpn_ipsec_name').getValue()],
             ['Local External Interface', Ext.getCmp('nif').getRawValue()],
             ['Local Internal IP Address', Ext.getCmp('nlocal_ip').getValue()],
             ['Local Netmask', Ext.getCmp('wizard_vpn_ipsec_nlocal_mask').getRawValue()],
             ['Remote Gateway', Ext.getCmp('nremote_gw').getValue()],
             ['Remote Subnet IP',Ext.getCmp('nremote_ip').getValue()],
             ['Remote Netmask', Ext.getCmp('wizard_vpn_ipsec_nremote_mask').getRawValue()]],
            // ['Preshared Key', Ext.getCmp('npkey').getValue()]],
             true);
             if (Ext.getCmp('con_init').getValue() == 1) 
             sum_data.loadData([['Connection Initiation', 'on']], true);
                else 
             sum_data.loadData([['Connection Initiation', 'off']], true);

                    if ( nadv ){
                        prev = page.ipsec_n3;
                    sum_data.loadData([['Phase 1 Left ID',
                                Ext.getCmp('np1_left_id').getValue()],
                    ['Phase 1 Right ID',
                                Ext.getCmp('np1_right_id').getValue()],
                    ['Phase 1 Authentication',
                                Ext.getCmp('np1_auth').getRawValue()],
                    ['Phase 1 Encryption',
                                Ext.getCmp('np1_ept').getRawValue()],
                    ['Phase 1 Group Key Management',
                                Ext.getCmp('np1_gkey').getRawValue()],
                    ['Phase 2 Authentication',
                                Ext.getCmp('np2_auth').getRawValue()],
                    ['Phase 2 Encryption',
                                Ext.getCmp('np2_ept').getRawValue()],
                    ['Phase 2 Group Key Management (PFS)',
                                Ext.getCmp('np2_gkey').getRawValue()]], true);
                    }else{
                        prev = page.ipsec_n2;
                    }
                } else {
                    sum_data.loadData([['VPN Type','IPSec'],
             ['IPSec Mode','Remote User'],
             ['Local External Interface', Ext.getCmp('rif').getRawValue()],
             ['Local Internal IP Address', Ext.getCmp('rlocal_ip').getValue()],
             ['Local Netmask', Ext.getCmp('wizard_vpn_ipsec_rlocal_mask').getRawValue()]],
            // ['Preshared Key', Ext.getCmp('rpkey').getValue()]],
             true);
                    if ( radv ) {
                        prev = page.ipsec_r3;
                    sum_data.loadData([['Phase 1 ID',
                                Ext.getCmp('rp1_id').getValue()],
                    ['Phase 1 Authentication',
                                Ext.getCmp('rp1_auth').getRawValue()],
                    ['Phase 1 Encryption',
                                Ext.getCmp('rp1_ept').getRawValue()],
                    ['Phase 1 Group Key Management',
                                Ext.getCmp('rp1_gkey').getRawValue()],
                    ['Phase 2 Authentication',
                                Ext.getCmp('rp2_auth').getRawValue()],
                    ['Phase 2 Encryption',
                                Ext.getCmp('rp2_ept').getRawValue()],
                    ['Phase 2 Group Key Management (PFS)',
                                Ext.getCmp('rp2_gkey').getRawValue()]], true);
                    }else{
                        prev = page.ipsec_r2;
                    }
                }
            }},
            items:[{
                xtype: 'grid',
                y: 50,
                height: 320,
                ds: sum_data,
                border: false,
                cm: cm,
           viewConfig: {
                forceFit: true
            }
            }]
      });
      
   var user_data = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'enable'},
            {name: 'id'},
            {name: 'pw'}
        ]),
        data: []
    });


      var contain = new Ext.Panel({     
           x:0,
           y:0,
           id:'wiarzd_contain',
           width: 700,     
           height: 420,     
           layout: 'card',     
           activeItem: 0, 
           bodyBorder:0, 
                border: false,
           items:[vpn_type,vpn_pptp,vpn_ipsec,
                 vpn_ipsec_n1, vpn_ipsec_n2, vpn_ipsec_n3,
                 vpn_ipsec_r1, vpn_ipsec_r2, vpn_ipsec_r3,
                 vpn_finish]});     

        var wizard = new Ext.Panel({
            width: 700,
            height: 500,
            layout: 'absolute',
            renderTo: 'wizard_window',
            items: [{
                        xtype: 'button',
                        text: 'Next',
                        x: 620,
                        y: 430,
                        width: 50,
                        id: 'move-next',
                        listeners:{'click':function(){
                            navHandler(1);
                        }}
                   }, {
                        xtype: 'button',
                        text: 'Prev',
                        x: 540,
                        y: 430,
                        width: 50,
                        id: 'move-prev',
                        listeners:{'click':function(){
                            navHandler(-1);
                        }}
                   }, {
                        xtype: 'button',
                        text: 'Cancel',
                        x: 460,
                        y: 430,
                        width: 50,
                        id: 'move-cancel',
                        listeners:{'click':function(){
                            navHandler(2);
                        }}
                   }, {
                        xtype: 'button',
                        text: 'Finish',
                        x: 620,
                        y: 430,
                        width: 50,
                        id: 'move-finish',
                        listeners:{'click':function(){
                            navHandler(0);
                        }}
                   },contain 
                ],
            });

});
