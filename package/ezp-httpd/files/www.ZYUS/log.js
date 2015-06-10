
function get_log() {
    Ext.getCmp('log').getForm().submit({
        success:function(form, o){
            var msg = o.result.msg;
            Ext.getCmp('msg').setValue(msg);
        },
        failure:function(){
            alert('failure');
        }
    });
};


Ext.onReady(function(){

    var log_form = new Ext.FormPanel({
        renderTo: 'log_form',
        title: 'System Log',
        id: 'log',
        bodyStyle:'padding:5px 5px 0',
        height: 550,
        url: 'log.cgi',
        layout: 'anchor',
        items: [{
            xtype: 'panel',
            border: false,
            layout: 'column',
            style: 'margin:5px 5px 5px 5px',
            items: [{
                xtype: 'label',
                text: 'Log Class :',
                style: 'font-weight: bold;font-size: 12px;margin: 0px 5px;'
            },{
                xtype: 'combo',
                id: 'class',
                hiddenName: 'log_class',
                width: 100,
                displayField:'name',
                valueField:'id',
                mode: 'local',
                triggerAction: 'all',
                value: 1,
                editable: false,
                fieldLabel: '<%lang("Log Class");%>',
                store: new Ext.data.Store({
                        reader: new Ext.data.ArrayReader({}, [
                        {name: 'id'},
                        {name: 'name'}
                        ]),
                        data: [[0,'ALL'],[1,'SYS'],[2,'DHCP'],[3,'DNS'],
                               [4,'UPNP'],[5,'PPTP'],[6,'L2TP'],[7,'IPSEC']]
                    }),
                listeners:{'select':function(){
                    get_log();
                }}
            },{
                xtype: 'button',
                text: 'refresh',
                width: 50,
                handler: function(){
                    get_log();
                }
            }]
        }, {
           xtype: 'textarea',
           width: 700,
            id: 'msg',
           height: 470,
           flex: 1  // Take up all *remaining* vertical space
        }]
    });
    get_log();
});
