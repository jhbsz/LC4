/*
 * Ext JS Library 3.0 RC1
 * Copyright(c) 2006-2009, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://extjs.com/license
 */

var formPost;
var gp;

Ext.onReady(function(){
    function percent(val){
        if (val == '') {
            return;
        } else {
            return val + '%';
        }
    }

    function adj_size(val) {
        if (val == '') {
            return;
        }
        var v = parseInt(val) + ' B';
        if (val > 1024) {
            val = val / 1024;
            v = parseInt(val) + ' KB'
        }
        if (val > 1024) {
            val = val / 1024;
            v = parseInt(val) + ' MB'
        }
        return v;
    }

    function adj_speed(val) {
        if (val == '') {
            return;
        }
        var v = val + ' B/s';
        if (val > 1024) {
            val = val / 1024;
            v = parseInt(val) + ' KB/s'
        }
        if (val > 1024) {
            val = val / 1024;
            v = parseInt(val) + ' MB/s'
        }
        return v;
    }

    var formUpload = new Ext.FormPanel({
        fileUpload: true,
        baseCls: 'x-plain',
        bodyStyle: 'padding: 0px 10px 0px 10px;',
        labelWidth: 50,
        defaults: {
            anchor: '90%',
            allowBlank: false,
            msgTarget: 'side'
        },
        items: [{
            xtype: 'fileuploadfield',
            id: 'form-file',
            emptyText: 'Select a seed',
            fieldLabel: 'Seed',
            name: 'path',
            height: 30,
            buttonCfg: {
                text: '',
                iconCls: 'upload-icon'
            }
        }]
    });

    formPost = new Ext.FormPanel({
        renderTo: Ext.getBody(),
        baseCls: 'x-plain',
        bodyStyle: 'padding: 0px 10px 0px 10px;'
    });

    var window = new Ext.Window({
        title: 'Choose a seed to download or URL to fetch the seed',
        autoCreate : true,
        resizable: false,
        closable: false,
        modal: true,

        width: 500,
        autoHeight: true,
        layout: 'fit',
        plain:true,
        bodyStyle:'padding:5px;',
        buttonAlign:'center',
        items: formUpload,

        buttons: [{
            text: 'Send',
            handler: function() {
                if (formUpload.getForm().isValid()) {
                    formUpload.getForm().submit({
                        url: 'bt_upload.cgi',
                        waitMsg: 'Uploading',
                        success: function(formUpload, o) {
                            show_mask();
                            var msg = "Upload: " + '<%lang("succeeded");%>';
                            display_return(-3, msg);
                            window.hide();
                        },
                        failure: function(formUpload, o) {
                            show_mask();
                            if (o.result) {
                                display_return(-3, o.result.msg);
                            } else {
                                var msg = "Upload: " + '<%lang("failed");%>';
                                display_return(-3, msg);
                            }
                            window.hide();
                        }
                    });
                }
            }
        },{
            text: 'Cancel',
            handler: function() {
                window.hide();
            }
        }]
    });

    deleteButton = new Ext.Toolbar.Button({
            id: 'delete',
            iconCls: 'icon-delete',
        	text: 'Delete',
        	disabled: true,
            handler: function(r){
                if(formPost.getForm().isValid()) {
                    var s = gp.getSelectionModel().getSelections();
                    if (s == '') {
                        return;
                    }
                    var hash_val = s[0].get('hash');

                    formPost.getForm().submit({
                        url: 'bt.cgi',
                        waitMsg: 'Deleting',
                        params: {action: 'Delete', hash: hash_val},  
                        success: function(formPost, o) {
                            show_mask();
                            var msg = "Delete: " + '<%lang("succeeded");%>';
                            display_return(-3, o.result.msg);
                        },
                        failure: function(formPost, o) {
                            show_mask();
                            if (o.result) {
                                display_return(-3, o.result.msg);
                            } else {
                                var msg = "Delete: " + '<%lang("failed");%>';
                                display_return(-3, msg);
                            }
                        }
                    });
                }
            }
    	});
    startButton = new Ext.Toolbar.Button({
            id: 'start',
//            iconCls: 'icon-delete',
        	text: 'Start',
        	disabled: true,
            handler: function(r){
                if(formPost.getForm().isValid()) {
                    var s = gp.getSelectionModel().getSelections();
                    if (s == '') {
                        return;
                    }
                    var hash_val = s[0].get('hash');

                    formPost.getForm().submit({
                        url: 'bt.cgi',
                        waitMsg: 'Starting',
                        params: {action: 'Start', hash: hash_val},  
                        success: function(formPost, o) {
                            show_mask();
                            var msg = "Start: " + '<%lang("succeeded");%>';
                            display_return(-3, msg);
                        },
                        failure: function(formPost, o) {
                            show_mask();
                            if (o.result) {
                                display_return(-3, o.result.msg);
                            } else {
                                var msg = "Start: " + '<%lang("failed");%>';
                                display_return(-3, msg);
                            }
                        }
                    });
                }
            }
    	});
    stopButton = new Ext.Toolbar.Button({
            id: 'stop',
//            iconCls: 'icon-delete',
        	text: 'Stop',
        	disabled: true,
            handler: function(r){
                if(formPost.getForm().isValid()) {
                    var s = gp.getSelectionModel().getSelections();
                    if (s == '') {
                        return;
                    }
                    var hash_val = s[0].get('hash');
                    formPost.getForm().submit({
                        url: 'bt.cgi',
                        waitMsg: 'Stopping',
                        params: {action: 'Stop', hash: hash_val},
                        success: function(formPost, o) {
                            show_mask();
                            var msg = "Stop: " + '<%lang("succeeded");%>';
                            display_return(-3, msg);
                        },
                        failure: function(formPost, o) {
                            show_mask();
                            if (o.result) {
                                display_return(-3, o.result.msg);
                            } else {
                                var msg = "Stop: " + '<%lang("failed");%>';
                                display_return(-3, msg);
                            }
                        }
                    });
                }
            }
    	});
    deleteAllCompleted = new Ext.Toolbar.Button({
            id: 'delAllCpl',
            iconCls: 'icon-delete',
        	text: 'Delete All Completed',
        	disabled: false,
            handler: function(r){
                if(formPost.getForm().isValid()) {
                    formPost.getForm().submit({
                        url: 'bt.cgi',
                        waitMsg: 'Deleting',
                        params: {action: 'delallcpl', hash: '0'},
                        success: function(formPost, o) {
                            show_mask();
                            var msg = "Delete all completed torrent: " + '<%lang("succeeded");%>';
                            display_return(-3, msg);
                        },
                        failure: function(formPost, o) {
                            show_mask();
                            var msg = "Delete all completed torrent: " + '<%lang("failed");%>';
                            display_return(-3, msg);
                        }
                    });
                }
            }
    	});
    gp = new Ext.grid.GridPanel({
                    border:false,
                    ds: new Ext.data.Store({
                        reader: new Ext.data.ArrayReader({}, [
                           {name: 'name'},
                           {name: 'size', type: 'float'},
                           {name: 'downloadsize', type: 'float'},
                           {name: 'progress', type: 'float'},
                           {name: 'peer'},
                           {name: 'uploadspeed', type: 'float'},
                           {name: 'downloadspeed', type: 'float'},
                           {name: 'status'},
                           {name: 'st', type: 'int'}, 
                           {name: 'hash'}
                        ]),
                        data: Ext.grid.dummyData
                    }),
                    cm: new Ext.grid.ColumnModel([
                        {header: "File Name", width: 120, sortable: true, dataIndex: 'name'},
                        {header: "File Size", width: 70, sortable: true, renderer: adj_size, dataIndex: 'size'},
                        {header: "Downloaded", width: 70, sortable: true, renderer: adj_size, dataIndex: 'downloadsize'},
                        {header: "Progress", width: 50, sortable: true, renderer: percent,  dataIndex: 'progress'},
                        {header: "Peers", width: 60, sortable: true, dataIndex: 'peer'},
                        {header: "Upload Speed", width: 70, sortable: true, renderer: adj_speed, dataIndex: 'uploadspeed'},
                        {header: "Download Speed", width: 70, sortable: true, renderer: adj_speed,
                                 dataIndex: 'downloadspeed'},
                        {header: "Status", width: 70, sortable: true, dataIndex: 'status'}
                    ]),
                    
                  tbar: [{
                          id: 'add',
                          text:'Add',
                          iconCls: 'icon-add',  // <-- icon
                      	  disabled: false,
                          handler: function(){
                              window.show();
                          }
    	}, '-', deleteButton, '-', stopButton, '-', startButton, '-',
deleteAllCompleted],

                    viewConfig: {
                        forceFit:true
                    },
                    autoExpandColumn:'name',
                    height: 350
                });

    gp.render('gridpanel');                

    gp.getSelectionModel().on('selectionchange', function(sm){
            var s = gp.getSelectionModel().getSelections();
            var defstat = {
                downloading : 0,
                stopped : 1,
                paused : 2,
                waiting : 3,
                hashing : 4,
                completed : 5,
                stopping : 6,
                pausing : 7,
                starting : 8,
                deleting : 9,
                resuming : 10,
                hash_for_cpl : 11
                }

            if (s == '') {
                deleteButton.disable();
                startButton.disable();
                stopButton.disable();
                return;
            }
            var stat = s[0].get('st');
            switch (stat) {
                 case defstat.stopping:
                 case defstat.pausing:
                 case defstat.starting:
                 case defstat.deleting:
                 case defstat.hash_for_cpl:
                     deleteButton.disable();
                     startButton.disable();
                     stopButton.disable();
                     break;
                 case defstat.completed:
                     deleteButton.enable();
                     startButton.disable();
                     stopButton.disable();
                     break;
                 case defstat.downloading:
                     deleteButton.enable();
                     startButton.disable();
                     stopButton.enable();
                     break;
                 case defstat.stopped:
                     deleteButton.enable();
                     startButton.enable();
                     stopButton.disable();
                     break;
                 case defstat.paused:
                     deleteButton.enable();
                     startButton.enable();
                     stopButton.enable();
                     break;
                 case defstat.waiting:
                     deleteButton.enable();
                     startButton.disable();
                     stopButton.enable();
                     break;
                 case defstat.hashing:
                     deleteButton.enable();
                     startButton.disable();
                     stopButton.enable();
                     break;
            }
    });

});

function bt_update_status() {
    formPost.getForm().submit({
        url: 'bt.cgi',
        params: {action: 'Update'},
        success: function(formPost, o) {
            if (o.result == undefined) {
 /*       display_return(-3, "System busy. Please wait for a while."); */
                return;
            }
            var msg = o.result.msg;
            if (msg == '') {
                Ext.grid.dummyData = [];
                setTimeout('bt_update_status();', 15000);
                return;
            }

            var seeds = msg.split('|');
            for (var i = 0; seeds[i] != undefined; i++) {
                if (Ext.grid.dummyData[i] == undefined) {
                    Ext.grid.dummyData[i] = new Array();
                }

                /* Check the noise and filter it out. */
                var check = seeds[i].search('^');
                if (check == -1) {                                    
                    return;                                           
                }      

                var items = seeds[i].split('^');
                /* name size complete progress dl_rate ul_rate hash */
                for (var j = 0; items[j] != undefined; j++) {
                    Ext.grid.dummyData[i][j] = items[j];
                }
            }
            gp.store.loadData(Ext.grid.dummyData);
            setTimeout('bt_update_status();', 5000);
        },
        failure: function(formPost, o) {
            /* TODO */
        }
    });
}
