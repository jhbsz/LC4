Ext.grid.RadioColumn = function(config){
    Ext.apply(this, config);
    if(!this.id){
        this.id = Ext.id();
    }
    this.renderer = this.renderer.createDelegate(this);
};

Ext.grid.RadioColumn.prototype = {
    init : function(grid){
        this.grid = grid;
        this.grid.on('render', function(){
            var view = this.grid.getView();
            view.mainBody.on('mousedown', this.onMouseDown, this);
        }, this);
    },

    onMouseDown : function(e, t){
        if(t.className && t.className.indexOf('x-grid3-cc-'+this.id) != -1){
            e.stopEvent();
            var index = this.grid.getView().findRowIndex(t);
            var record = this.grid.store.getAt(index);
            var record_count = this.grid.store.getCount();
            for(var i = 0; i < record_count; i++) {
                record = this.grid.store.getAt(i);
                if(i == index) {
                    record.set(this.dataIndex, true);
                }
                else {
                    record.set(this.dataIndex, false);
                }
            }
        }
    },

    renderer : function(v, p, record){
        p.css += ' x-grid3-radio-col-td'; 
        return '<div class="x-grid3-radio-col'+(v?'-on':'')+' x-grid3-cc-'+this.id+'"> </div>';
    }
};
