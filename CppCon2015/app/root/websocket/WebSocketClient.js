// Create singleton
// This class will help to maintain a reliable connection to the web socket server
// and allow to send / receive data through it
/*********************************************************************************

Usage:
				WebSocketClient.initialize();

				For events
				WebSocketClient.subscribe(EventId);
				or
				For request/response type messages
				WebSocketClient.subscribe(EventId, true/false); 

 				WebSocketClient.connect(messageReceivedCallback);
 								
				For simple send
				WebSocketClient.send(EventId, Data)
				or
				For request response type messages
				WebSocketClient.send(EventId, Data, callback)

**********************************************************************************/
var WebSocketClient = {

    _ws: null,
    _subscriptions: '',
    _publishEvents: [],
    _instanceId: '',
    _taskRunning: false,
    _pingInterval: 5000,

    // Initializing variables
    initialize: function () {
        _subscriptions = '';
		_publishEvents = [];
        _taskRunning = false;
        _instanceId = newGuid();
    },

    // Connect
    // onMessageReceived = Callback
	// pingInterval = number of milliseconds between pings
    connect: function (onMessageReceived, pingInterval, enableLogging) {
        _ws = new WebEventServer(undefined, this.onOpen, this.onError, onMessageReceived ? onMessageReceived : this.onMessage, undefined, enableLogging);
		_pingInterval = 5000;
		if (!isNaN(parseFloat(pingInterval)) && isFinite(pingInterval))
		  _pingInterval = pingInterval;
	  
	  if (_ws.logging) console.log("Ping interval: " + (_pingInterval / 1000) + " [s]");
    },

    // Disconnect.
    disconnect: function () {
        if (_ws && _ws.disconnect) _ws.disconnect();
    },

    // Enables or disables logging.
    enableLogging: function (val) {
        if (typeof(val) === "undefined")
			_ws.logging = true;
		else
			_ws.logging = val;
    },

    // Return true if logging is enabled, false otherwise
    loggingEnabled: function () {
		if (typeof(_ws) === "undefined")
			return false;

        return _ws.logging;
    },

    // Returns true if web socket is connected or false if not. This value will be updated by web server ping method.
    connected: function () {
        return (_taskRunning);
    },

    // if callback left undefined this function will handle the callback
    onMessage: function (response) {
        if (_ws.logging) console.log(response.data);
    },

    // Start task and subscribe/publish every time you get connection
    onOpen: function () {
        if (!_taskRunning) {
            if (_ws.logging) console.log('Initializing ping, interval: [' + _pingInterval + '] ms');
			 _taskRunning = true;
			 setInterval(function(){
				 _taskRunning = _ws.ping();
			 }, _pingInterval);
        }

        if (_subscriptions !== '')
            _ws.subscribe(_subscriptions);

		for (var i = 0; i < _publishEvents.length; ++i)
		{
			try {
				_ws.send(_publishEvents[i].event, _publishEvents[i].data);
			} catch (e) { if (_ws.logging) console.log('WebSocket send error: ' + e.message); }
		}
    },

    // Error log 
    onError: function (message) {
        if (_ws.logging) console.log(message);
    },

    // Send
    // Callback optional, will be fired only if subscribed to response
    send: function (evt, data, callback) {
        if (callback) _ws.subscribe(_instanceId);
        try {
			_ws.send(evt + ((callback) ? '.:' + _instanceId : ''), data, function (response) {
				if (callback) callback(response);
			});
		} catch (e) { if (_ws.logging) console.log('WebSocket send error: ' + e.message); }
    },

    // Returns the WebEventServer
    server: function () {
        return _ws;
    },

    // Subscribe will add subscription to the subscription list, and subscribe only when connected on event.
    // useId = true indicates the event will be a request/response type
    // 	     = false subscribe to generic event
    subscribe: function (eventName, useId) {
        var subscriptionString = (!useId ? '' : _instanceId + '.') + eventName;

        if (_subscriptions === '')
            _subscriptions = subscriptionString;
        else
            _subscriptions += ',' + subscriptionString;

        if (_taskRunning) _ws.subscribe(subscriptionString);
    },

    // Publish will add events that need to be published on connect to the list;
	// these events will be published on every connect.
	// Typically, this is used to initially populate the data (otherwise, the 
	// client must wait until the data changes to get a first refresh batch)
    publish: function (eventName, data) {
        if (_taskRunning) _ws.send(eventName, data);
		else
		{
			var pubObject = eval('({"event":"' + eventName + '","data":"' + data + '"})');
			_publishEvents.push(pubObject);
		}
    },

    // Will remove subscription
    // useId = true indicates the event will be a request/response type
    // 	     = false subscribe to generic event
    unsubscribe: function (eventName, useId) {
        _ws.unsubscribe(eventName);
        if (_subscriptions !== '') {
            _subscriptions = _subscriptions.replace(',' + (!useId ? '' : _instanceId + '.') + eventName, '');
            _subscriptions = _subscriptions.replace((!useId ? '' : _instanceId + '.') + eventName, '');
        }
    }
};

function S4() {
    return (((1+Math.random())*0x10000)|0).toString(16).substring(1); 
};
 
// then to call it, plus stitch in '4' in the third group
function newGuid() {
	return (S4() + S4() + "-" + S4() + "-4" + S4().substr(0,3) + "-" + S4() + "-" + S4() + S4() + S4()).toLowerCase();
};