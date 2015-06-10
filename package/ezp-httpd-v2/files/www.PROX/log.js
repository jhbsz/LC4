
function get_log() {
    Ext.getCmp('log').getForm().submit({
        success:function(form, o){
            var msg = o.result.msg;
            Ext.getCmp('msg').setValue(msg);
        },
        failure:function(){
            alert('Failure');
        }
    });
};
function clear_log() {
    Ext.getCmp('clear').getForm().submit({
        success: function(form, o){
            alert('Success');
            Ext.getCmp('msg').setValue('');
        },
        failure: function(){
            alert('Failure');
        }
    });
};


Ext.onReady(function(){

    var clear_form = new Ext.FormPanel({
        renderTo: 'log_form',
        id: 'clear',
        url: 'log.cgi',
        height: 0,
        items: [
        {
                xtype: 'hidden',
                name: 'log_class',
                value: 99
        }]
    });
    
    var log_form = new Ext.FormPanel({
        renderTo: 'log_form',
        title: 'System Log',
        id: 'log',
        bodyStyle:'padding:5px 5px 0',
        height: 550,
        url: 'log.cgi',
        layout: 'anchor',
        items: [
        
        {
            xtype: 'panel',
            border: false,
            layout: 'column',
            style: 'margin:5px 5px 5px 5px',
            items: [
            {
                xtype: 'label',
                text: 'Log Class :',
                style: 'font-weight: bold;font-size: 12px;margin: 0px 5px;'
            },
            {
                xtype: 'combo',
                id: 'class',
                hiddenName: 'log_class',
                width: 100,
                displayField:'name',
                valueField:'id',
                mode: 'local',
                triggerAction: 'all',
                value: 0,
                editable: false,
                fieldLabel: '<%lang("Log Class");%>',
                store: new Ext.data.Store({
                        reader: new Ext.data.ArrayReader({}, [
                        {name: 'id'},
                        {name: 'name'}
                        ]),
                        data: <% st_log_show_class();%>
                    }),
                listeners:{'select':function(){
                    get_log();
                }}
            },
            {
                xtype: 'button',
                text: 'refresh',
                width: 50,
                style:'padding:0px;margin:0px 0px 0px 10px;',
                handler: function(){
                    get_log();
                }
            },
            {
                xtype: 'button',
                text: 'clear',
                width: 50,
                style:'padding:0px;margin:0px 0px 0px 10px;',
                value:'1',
                handler: function(){
                    clear_log();
                }
            }]
        }, 
        {
           xtype: 'textarea',
           width: 614,
            id: 'msg',
           height: 470,
           flex: 1  // Take up all *remaining* vertical space
        }]
    });
    get_log();
});
