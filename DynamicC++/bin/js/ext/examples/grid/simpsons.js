Ext.require([
    'Ext.grid.*',
    'Ext.data.*',
    'Ext.panel.*',
    'Ext.layout.container.Border'
]);

Ext.onReady(function(){
    Ext.define('Book',{
        extend: 'Ext.data.Model',
        fields: [
            // set up the fields mapping into the xml doc
            // The first needs mapping, the others are very basic
            'Name',
            'Address',
            'Age',
            'DetailPageURL'
        ]
    });

    // create the Data Store
    var store = Ext.create('Ext.data.Store', {
        model: 'Book',
        proxy: {
            // load using HTTP
            type: 'ajax',
            url: 'http://localhost:8080/xml',
            // the return will be XML, so lets set up a reader
            reader: {
                type: 'xml',
                record: 'Item'/*,
                totalProperty  : 'total'*/
            }
        }
    });

    // create the grid
    var grid = Ext.create('Ext.grid.Panel', {
        store: store,
        columns: [
            {text: "Name", width: 120, dataIndex: 'Name', sortable: true},
            {text: "Address", flex: 1, dataIndex: 'Address', sortable: true},
            {text: "Age", width: 115, dataIndex: 'Age', sortable: true}
        ],
        forceFit: true,
        height:210,
        split: true,
        region: 'north'
    });
        
    // define a template to use for the detail view
    var bookTplMarkup = [
        'Address: <a href="{DetailPageURL}" target="_blank">{Address}</a><br/>',
        'Name: {Name}<br/>',
        'Age: {Age}<br/>'
    ];
    var bookTpl = Ext.create('Ext.Template', bookTplMarkup);

    Ext.create('Ext.Panel', {
        renderTo: 'binding-example',
        frame: true,
        title: 'Simpsons Family List',
        width: 540,
        height: 400,
        layout: 'border',
        items: [
            grid, {
                id: 'detailPanel',
                region: 'center',
                bodyPadding: 7,
                bodyStyle: "background: #ffffff;",
                html: 'Please select a person to see additional details.'
        }]
    });
    
    // update panel body on selection change
    grid.getSelectionModel().on('selectionchange', function(sm, selectedRecord) {
        if (selectedRecord.length) {
            var detailPanel = Ext.getCmp('detailPanel');
            bookTpl.overwrite(detailPanel.body, selectedRecord[0].data);
        }
    });

    store.load();
});