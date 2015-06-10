var page = {
    admin : 0,
    model : 1,
    wan_setting : 2,
    mobile_setting : 3,
    bandwidth : 4,
    time : 5,
    wifi : 6,
    sum : 7
}
var wan_type = {
    dhcp : 0,
    pppoe : 1,
    staticip : 2,
    wwan : 3,
    htc : 4,
    directip : 5
}
var valid = 1;
var now = 0,next = 0,prev = 0; /* for wizard page control */
var model_def = 'router'; /* machine model nas or router */
var wan_append = 0; /* for wan connect type */
var admin,model,wan_setting,mobile_setting,bandwidth,time,wifi; /* wizard page */
var page_wifi = <%nv_attr_get("wizard_rule", 0, "wifi");%>;

var navHandler = function(direction) {
    var btnNext = Ext.getCmp('move-next');
    var btnBack = Ext.getCmp('move-prev');
    var btnFinish = Ext.getCmp('move-finish');
    var contain = Ext.getCmp('wizard_contain');
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
        if (now == page.admin) {
            Ext.getCmp('adminpw').validate();
            Ext.getCmp('adminpw_confirm').validate();
            if ((Ext.getCmp('adminpw').getValue() != "") ^ (Ext.getCmp('adminpw_confirm').getValue() != "")) valid=0;
        }
        else if (now == page.wan_setting) {
            var conn_type = Ext.getCmp('wan_type').getValue();
            var wan_wan0_enable =  Ext.getCmp('wan_wan0_enable').getValue().getRawValue();
            if (wan_wan0_enable == 1) {
                if ( conn_type == wan_type.pppoe ) {
                    Ext.getCmp('wizard_wan0_ppp_username').validate();
                    Ext.getCmp('wizard_wan0_ppp_passwd').validate();
                    Ext.getCmp('wizard_wan0_ppp_passwd_confirm').validate();
                    if ((Ext.getCmp('wizard_wan0_ppp_username').getValue() == "") || (Ext.getCmp('wizard_wan0_ppp_passwd').getValue() == "") || (Ext.getCmp('wizard_wan0_ppp_passwd_confirm').getValue() == "")) valid=0;
                } else if ( conn_type == wan_type.staticip ) {
                    Ext.getCmp('wizard_wan0_static_ip').validate();
                    Ext.getCmp('wizard_wan0_static_mask').validate();
                    Ext.getCmp('wizard_wan0_static_gateway').validate();
                    Ext.getCmp('wizard_wan0_static_dns1').validate();
                    Ext.getCmp('wizard_wan0_static_dns2').validate();
                }
            }
        }
        if (valid==0) alert('The configuration values are incorrect. Please input the right values.');
        else now = next;
    }
    if (now == page.admin) {
        btnBack.hide();
    } else {
        btnBack.show();
    }
    if (now == page.sum) {
        btnNext.hide();
        btnFinish.show();
    } else {
        btnFinish.hide();
        btnNext.show();
    }
    contain.getLayout().setActiveItem(now);
}
function check_ascii(val)
{
    
    for (var i=0;i<val.length;i++) {
        var asciiNum = val.charCodeAt(i);
        if (asciiNum>126){
            return false;
        }
    }
    return true; 
}
function check_hex(val)
{
    for (var i=0;i<val.length;i++) {
        var asciiNum = val.charCodeAt(i);
        if (asciiNum<48 || (asciiNum>57 && asciiNum<65) ||
            (asciiNum>70 && asciiNum<97) || asciiNum >102 ){
            return false;
        }
    }
    return true;
}
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
    password : function(val, field) {
        if (field.initialPassField) {
            var pwd = Ext.getCmp(field.initialPassField);
            if (val !=  pwd.getValue()) valid=0;
            return (val == pwd.getValue());
        }
        return true;
    },
    passwordText : 'Passwords do not match',
    wireless : function(val, field) {
        if (field.keytype) {
            var type = Ext.getCmp(field.keytype);
            switch (type.getValue()) {
                case 0:
                    switch (val.length) {
                        case 5:
                        case 13:
                            return check_ascii(val);
                        case 10:
                        case 26:
                            return check_hex(val);
                        default :
                            return false;
                    }   
                    break;
                case 1:
                case 2:
                    var len = val.length;
                    if (len>=8 && len<=63)
                        return check_ascii(val);
                    else if (len == 64)
                        return check_hex(val);
                    else 
                        return false;
                    break;
            }
        }
        return true;
    },
    wirelessText : 'Wireless key is not valid',
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



Ext.onReady(function(){
    Ext.QuickTips.init();
    Ext.form.Field.prototype.msgTarget = 'side'
      var admin = new Ext.Panel({
            title: '<%wizard_title("Admin");%>',
            layout: 'absolute',
            border: false,
            items: [{
                xtype: 'fieldset',
                title: '<%lang("Change Administrator Password");%>',
                layout: 'form',
                x: 135,
                y: 70,
                height: 120,
                width: 400,
                items: [{
                    xtype: 'textfield',
                    inputType: 'password',
                    name: 'wizard_admin_pw',
                    id: 'adminpw',
                    labelStyle: 'width:135px',
                    fieldLabel: '<%lang("Administrator Password");%>',
                    width: 200,
                    vtype: 'asciicheck'
                    },{
                    xtype: 'textfield',
                    inputType: 'password',
                    name: 'wizard_admin_pw_confirm',
                    id: 'adminpw_confirm',
                    labelStyle: 'width:135px',
                    fieldLabel: '<%lang("Password Confirm");%>',
                    width: 200,
                    vtype: 'password',
                    initialPassField: 'adminpw'
                    },{
                    xtype: 'label',
                    style: 'font-size:11px',
                    text: '(<%lang("If you do not change admin password, let input box keep empty");%>)'
                    }
                ]
            }]
      });
      admin.on('show',function(){
        prev = page.admin;
        next = page.wan_setting;
        Ext.getCmp('move-prev').hide();
        Ext.getCmp('move-finish').hide();
      });

    var area_data = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'value'}
        ]),
        data: [['Automatic','pool.ntp.org'],
               ['Asia', 'asia.pool.ntp.org'],
               ['Europe','eurpo.pool.ntp.org'],
               ['North America','north-america.pool.ntp.org'],
               ['South America', 'south-america.pool.ntp.org'],
               ['Africa', 'africa.pool.ntp.org']]
    });

    var zone_data = new Ext.data.Store({
        reader: new Ext.data.ArrayReader({}, [
            {name: 'value'},
            {name: 'name'}
        ]),
        data: [
        ['UTC12','UTC-12:00 Kwajalein'],
        ['UTC11','UTC-11:00 Midway Island, Samoa'],
        ['UTC10','UTC-10:00 Hawaii'],
        ['NAST9NADT,M3.2.0/2,M11.1.0/2','UTC-09:00 Alaska'],
        ['PST8PDT,M3.2.0/2,M11.1.0/2','UTC-08:00 Pacific Time'],
        ['UTC7','UTC-07:00 Arizona'],
        ['MST7MDT,M3.2.0/2,M11.1.0/2','UTC-07:00 Mountain Time'],
        ['UTC6','UTC-06:00 Mexico'],
        ['CST6CDT,M3.2.0/2,M11.1.0/2','UTC-06:00 Central Time'],
        ['UTC5','UTC-05:00 Colombia, Panama'],
        ['EST5EDT,M3.2.0/2,M11.1.0/2','UTC-05:00 Eastern Time'],
        ['UTC4','UTC-04:00 Bolivia, Venezuela'],
        ['AST4ADT,M3.2.0/2,M11.1.0/2','UTC-04:00 Atlantic Time'],
        ['BRWST4BRWDT,M10.3.0/0,M2.5.0/0','UTC-04:00 Brazil West'],
        ['NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01','UTC-03:30 Newfoundland'],
        ['WGST3WGDT,M3.5.6/22,M10.5.6/23','UTC-03:00 Greenland'],
        ['BRST3BRDT,M10.3.0/0,M2.5.0/0','UTC-03:00 Brazil East'],
        ['UTC3','UTC-03:00 Guyana'],
        ['UTC2','UTC-02:00 Mid-Atlantic'],
        ['STD1DST,M3.5.0/2,M10.5.0/2','UTC-01:00 Azores'],
        ['UTC0','UTC+00:00 Gambia, Liberia, Morocco'],
        ['GMT0BST,M3.5.0/2,M10.5.0/2','UTC+00:00 England'],
        ['UTC-1','UTC+01:00 Tunisia'],
        ['STD-1DST,M3.5.0/2,M10.5.0/2','UTC+01:00 France, Germany, Italy'],
        ['UTC-2','UTC+02:00 South Africa'],
        ['STD-2DST,M3.5.0/2,M10.5.0/2','UTC+02:00 Greece, Ukraine, Romania, Turkey'],
        ['UTC-3','UTC+03:00 Iraq, Jordan, Kuwait'],
        ['UTC-4','UTC+04:00 Armenia'],
        ['UTC-5','UTC+05:00 Pakistan, Russia'],
        ['UTC-5:30','UTC+05:30 Bombay, Calcutta, Madras, New Delhi'],
        ['UTC-6','UTC+06:00 Bangladesh, Russia'],
        ['UTC-7','UTC+07:00 Thailand, Russia'],
        ['UTC-8','UTC+08:00 Taiwan, China, Hong Kong, Western Australia, Singapore'],
        ['UTC-9','UTC+09:00 Japan, Korea'],
        ['CST-9:30CST,M10.5.0/2,M3.5.0/3', 'UTC+09:30 South Australia'],
        ['UTC-10','UTC+10:00 Guam, Russia'],
        ['STD-10DST,M10.5.0/2,M3.5.0/2','UTC+10:00 Australia'],
        ['UTC-11','UTC+11:00 Solomon Islands'],
        ['UTC-12','UTC+12:00 Fiji'],
        ['STD-12DST,M10.5.0/2,M3.5.0/2','UTC+12:00 New Zealand']]

    });

      var time = new Ext.Panel({
            title: '<%wizard_title("Time");%>',
            layout: 'absolute',
            border: false,
            items: [{
                xtype: 'fieldset',
                title: 'Time',
                layout: 'form',
                x: 130,
                y: 40,
                height: 100,
                labelWidth: 100,
                width: 540,
                items: [{
                       xtype: 'combo',
                       width: 170,
                       displayField:'name',
                       id: 'time_area',
                       hiddenName: 'wizard_time_pool',
                       valueField: 'value', 
                       mode: 'local',
                       value: 'north-america.pool.ntp.org',
                       triggerAction: 'all',
                       editable: false,
                       emptyText:'Select a area ...',
                       fieldLabel:'Time Server Area',
                       store: area_data
                },{
                       xtype: 'combo',
                       width: 400,
                       displayField:'name',
                       id: 'time_zone',
                       hiddenName: 'wizard_time_zone',
                       valueField: 'value', 
                       mode: 'local',
                       triggerAction: 'all',
                       value: 'PST8PDT,M3.2.0/2,M11.1.0/2',
                       editable: false,
                       emptyText:'Select a time zone ...',
                       fieldLabel:'Time Zone',
                       store: zone_data
                },{
                       xtype: 'checkbox',
                       id: 'wizard_time_daylight',
                       checked: true,
                       fieldLabel: '<%lang("Daylight Saving");%>',
                       boxLabel: '<%lang("Enable");%>',
                }]
            }]
      });
      time.on('show',function(){
        prev = page.mobile_setting;
        if(page_wifi)
            next = page.wifi;
        else 
            next = page.sum;
      });

      var model = new Ext.Panel({
            title: '<font color=#FF0000><%lang("Mode");%>l</font> - ' +
                   '<%lang("WAN");%> - <%lang("Time");%>' +
                   ' - <%lang("Wireless");%>',
            layout: 'absolute',
            border: false,
            items: [{
                xtype: 'fieldset',
                title: 'Model select',
                layout: 'form',
                x: 200,
                y: 40,
                height: 100,
                width: 400,
                items: [{
                    xtype: 'radio',
                    name: 'model',
                    boxLabel: '3G/4G Mobile Router',
                    checked: true,
                    inputValue: 'router',
                    listeners:{'check':function(cb,checked){
                        if(checked) {
                            model_def = 'router';
                        } else {
                            model_def = 'nas';
                        }
                    }}
                }, {
                    xtype: 'radio',
                    name: 'model',
                    boxLabel: 'NAS',
                    inputValue: 'nas'
                }]
            }]
      });
      model.on('show',function(){
        prev = page.model;
        next = page.wan_setting;
      });

      sum_data = new Ext.data.Store({
           reader: new Ext.data.ArrayReader({}, [
            {name: 'name'},
            {name: 'value'}
        ]),
        data: []
      });

      cm = new Ext.grid.ColumnModel([
            {header:'<%lang("Setting Name");%>', dataIndex:'name'},
            {header:'<%lang("Setting Value");%>', dataIndex:'value',
                 renderer:function(val) {
                    if (val == '') {
                        return '<p style="color: rgb(255, 0, 0);">No select or input value !</p>';
                    }   
                    return val;
                }}
        ]);
    function display_wifi() {
        var wifi_enable =
                Ext.getCmp('wifi_enable').getValue().getRawValue();
        if( wifi_enable == 1) {
            sum_data.loadData([
            ['<%lang("Wireless Connection");%>',
                 '<%lang("Enabled");%>'],
            ['<%lang("Wireless SSID Name");%>',
                 Ext.getCmp('wizard_ssid').getValue()]
            ], true);
            if(Ext.getCmp('wizard_wl_key_enable').getValue()) {
                sum_data.loadData([
                ['<%lang("Security Mode");%>',
                     Ext.getCmp('wl_sec').getRawValue()]
             //   ['<%lang("Key");%>',
             //        Ext.getCmp('wizard_wl_key').getValue()]
                ], true);
            }
        } else {
            sum_data.loadData([
            ['<%lang("Wireless Connection");%>',
                 '<%lang("Disabled");%>']
            ], true);
        }
    }

      summarise = new Ext.Panel({
            title: '<%lang("Configuration Summary");%>',
            border: false,
            items:[{
                xtype: 'grid',
                enableHdMenu: false,
                border: false,
                ds: sum_data,
                cm: cm,
                height: 400,                
                viewConfig:{
                    forceFit: true
                }
            }],
            listeners:{'show':function(){
                if(page_wifi)
                    prev = page.wifi;
                else 
                    prev = page.time;
                next = page.sum;
                var conn_type = Ext.getCmp('wan_type').getValue();
                sum_data.removeAll();
          /*      if(Ext.getCmp('adminpw').getValue() == ""){
                    sum_data.loadData([['<%lang("Administrator Password");%>',
                                '<%lang("Not Change");%>']],true);
                } else {
                    sum_data.loadData([['<%lang("Administrator Password");%>',
                                '<%lang("Change!!!");%>']],true);
                } */
                var wan_wan0_enable =
                        Ext.getCmp('wan_wan0_enable').getValue().getRawValue();
                if (wan_wan0_enable == 1) {
                sum_data.loadData([['<%lang("Ethernet WAN");%>','Enabled']], true);
                sum_data.loadData([['<%lang("Ethernet WAN Connection Type");%>',Ext.getCmp('wan_type').getRawValue()]], true);
                if ( conn_type == wan_type.pppoe ) {
                    sum_data.loadData([
                    ['<%lang("User Name");%>',
                        Ext.getCmp('wizard_wan0_ppp_username').getValue()]
  //                  ['<%lang("Password");%>',
  //                     Ext.getCmp('wizard_wan0_ppp_passwd').getValue()]
                    ], true);
                } else if ( conn_type == wan_type.staticip ) {
                    sum_data.loadData([
                    ['<%lang("External IP Address");%>',
                        Ext.getCmp('wizard_wan0_static_ip').getValue()],
                    ['<%lang("Netmask");%>',
                        Ext.getCmp('wizard_wan0_static_mask').getValue()],
                    ['<%lang("Gateway");%>',
                        Ext.getCmp('wizard_wan0_static_gateway').getValue()],
                    ['<%lang("Static DNS 1");%>',
                        Ext.getCmp('wizard_wan0_static_dns1').getValue()],
                    ],true);
                    if (Ext.getCmp('wizard_wan0_static_dns2').getValue() != ''){
                    sum_data.loadData([
                    ['<%lang("Static DNS 2");%>', Ext.getCmp('wizard_wan0_static_dns2').getValue()]],true); }}
                } else {
                    sum_data.loadData([['<%lang("Ethernet WAN");%>','Disabled']], true);
                }
                
                var mobile_enable =
                        Ext.getCmp('mobile_enable').getValue().getRawValue();
                if ( mobile_enable == '1') {
                    sum_data.loadData([
                        ['Mobile WAN', 'Enabled']],true);
                    if (Ext.getCmp('apn_select')
                            .getValue().getRawValue() == 0 ) {
                        sum_data.loadData([
                        ['Mobile APN Provider',Ext.getCmp('apn_provider').getRawValue()]
                        ],true);
                    } else {
                        sum_data.loadData([
                        ['Mobile APN',Ext.getCmp('wizard_wwan_apn').getValue()],
                        ['Mobile Dial Number',Ext.getCmp('wizard_wwan_dialstr').getValue()]],true);
                    }
                    var value = Ext.getCmp('wizard_wwan_pin').getValue();
                    if ( value ) {
                        sum_data.loadData([['Mobile PIN Code',value]],true);
                    } else {
                        Ext.getCmp('wizard_wwan_pin').setRawValue('');
                    }
                    var value = Ext.getCmp('wizard_wwan_user').getValue();
                    if ( value ) {
                        sum_data.loadData([['Mobile User Name',value]],true);
                    } else {
                        Ext.getCmp('wizard_wwan_user').setRawValue('');
                    }
                    var value = Ext.getCmp('wizard_wwan_pw').getValue();
                    if ( value ) {
                        sum_data.loadData([['Mobile Password',value]],true);
                    } else {
                        Ext.getCmp('wizard_wwan_pw').setRawValue('');
                    }
                } else {
                    sum_data.loadData([
                        ['Mobile WAN', 'Disabled']],true);
                }
/* 
                var bandwidth_enable =
                        Ext.getCmp('wizard_bandwidth_enable').getValue().getRawValue();
                if (bandwidth_enable == '1') {
                    sum_data.loadData([
                    ['WAN Ethernet Bandwidth setting',Ext.getCmp('bandwidth_wan0').getRawValue()]
                    ], true);
                    if( Ext.getCmp('bandwidth_wan0').getValue() == 'Custom' ) {
                        sum_data.loadData([
                        ['WAN Ethernet Upload Rate (kbps)',Ext.getCmp('wizard_wan0_bw_ul').getValue()],
                        ['WAN Ethernet Download Rate (kbps)',Ext.getCmp('wizard_wan0_bw_dl').getValue()],
                        ], true);
                    }
                    sum_data.loadData([
                    ['WAN Mobile Bandwidth setting',Ext.getCmp('bandwidth_wan1').getRawValue()]
                    ], true);
                    if( Ext.getCmp('bandwidth_wan1').getValue() == 'Custom' ) {
                        sum_data.loadData([
                        ['WAN Mobile Upload Rate (kbps)',Ext.getCmp('wizard_wan1_bw_ul').getValue()],
                        ['WAN Mobile Download Rate (kbps)',Ext.getCmp('wizard_wan1_bw_dl').getValue()],
                        ], true);
                    }
                } else {
                    sum_data.loadData([
                    ['Bandwidth setting', 'Disabled']], true);
                }
*/
                sum_data.loadData([
                ['Time Server Area', Ext.getCmp('time_area').getRawValue()],
                ['Time Zone', Ext.getCmp('time_zone').getRawValue()],
                ], true);

                if(Ext.getCmp('wizard_time_daylight').getValue()) {
                    sum_data.loadData([
                     ['Daylight Saving', 'Enabled']],true);
                } else {
                    sum_data.loadData([
                     ['Daylight Saving', 'Disabled']],true);
                }
                
                if(page_wifi){
                    display_wifi();
                }
            }}
      });

      var contain = new Ext.Panel({     
           id: 'wizard_contain',
           x:0,
           y:0,
           width: 700,     
           height: 420,     
            border: false,
           layout: 'card',     
           activeItem: 0, // make sure the active item is set on the container config!     
           bodyBorder:0, 
           items: [admin,model,wan_setting,mobile_setting,
                    bandwidth,time, wifi,summarise]
       });     

        var wizard = new Ext.Panel({
            width: 700,
            height: 500,
            layout: 'absolute',
            renderTo: 'wizard_window',
            items: [{
                        xtype: 'button',
                        text: '<%lang("Next");%>',
                        x: 620,
                        y: 430,
                        width: 50,
                        id: 'move-next',
                        handler: navHandler.createDelegate(this, [1])
                   }, {
                        xtype: 'button',
                        text: '<%lang("Prev");%>',
                        x: 540,
                        y: 430,
                        width: 50,
                        id: 'move-prev',
                        handler: navHandler.createDelegate(this, [-1])
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
                        text: '<%lang("Finish");%>',
                        x: 620,
                        y: 430,
                        width: 50,
                        id: 'move-finish',
                        handler: navHandler.createDelegate(this, [0])
                   },contain 
                ]
            });
});
