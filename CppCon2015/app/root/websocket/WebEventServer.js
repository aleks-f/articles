function WebEventServer(address, onOpen, onError, onMessage, onClose, enableLogging) {
  if(typeof(address) === 'undefined') address = "ws://" + window.location.host + "/webevent";
  if (typeof console === "undefined" || typeof console.log === "undefined") console = {};
  
  if (typeof(enableLogging) === "undefined")
    this.logging = false;
  else
    this.logging = enableLogging;

  this.serverAlive = false;
  
  var _version = "1.0";
  var _ping = "NOTIFY system.ping WebEvent/" + _version;
  var _pong = "NOTIFY system.pong WebEvent/" + _version;
  var _wsParent = this;
  var _sendCallback;
  var _connected = false;
 
  this.log = function(msg) {
    if (this.logging) console.log(msg);
  }

  // +++ Begin WebSocket

  // Get working socket
  this.connect = function(address){
	//if (_wsParent.isConnected()) _wsParent.disconnect();
	
	var socket = new WebSocket(address);
	socket.onopen = function() {
	_wsParent.log('Connection opened.');
	_wsParent._connected = true;

	if(onOpen)
		onOpen();
	};
		
	socket.onerror = function (error) {
	_wsParent.log('Error');
	_wsParent.log(error);
	if(onError)
		onError(error);
	};

	socket.onmessage = function(evt) {
	_wsParent.log('Received: ' + evt.data);

	if(onMessage)
		onMessage(evt);
	};

	socket.onclose = function (error) {
	_wsParent.log('Connection closed.');
	_wsParent._connected = false;
	if(onClose)
		onClose(error);
	};
	
	return socket;
  }
  
  this.ws = this.connect(address);
  // --- End Websocket
  
  this.subscribe = function(evt){
    if(this.isConnected()) {
	  var msg = "SUBSCRIBE " + evt + " WebEvent/" + _version;
	  try {
		this.ws.send(msg);
		this.log("Sent: " + msg);
	  }
	  catch(e)
	  {
		  this.log("Error sending message: " + e.message);
		  return false;
	  } 
    }
    else { 
	  this.log ("Error: Not connected!");
	  return false;
    }

    return this.isConnected();
  },
  
  this.unsubscribe = function(evt){
    if(this.isConnected()) {
	  var msg = "UNSUBSCRIBE " + evt + " WebEvent/" + _version;
      this.ws.send(msg);
      this.log("Sent: " + msg);
    }
    else { 
	  this.log ("Error: Not connected!");
	  return false;
    }

    return this.isConnected();
  },
  
  this.send = function(evt, data, callback) {
    if(this.isConnected()) {
	  var msg = "NOTIFY " + evt + " WebEvent/" + _version;
	  if (!(typeof(data) === 'undefined')) msg += "\r\n" + data;
	  try {
		this.ws.send(msg);
		this.log("Sent: " + msg);
	  }
	  catch(e)
	  {
		  this.log("Error sending message: " + e.message);
		  return false;
	  }
	  
	  if (callback)
	  {
		if (!_sendCallback)
			_sendCallback = this.ws.onmessage;
		this.ws.onmessage = callback;
	  }
	  else
	  {
		if (_sendCallback)
			this.ws.onmessage = _sendCallback;
		_sendCallback = undefined;
	  }
    }
    else { 
      this.log ("Error: Not connected!");
      return false;
    }

    return this.isConnected();
  },
  
  this.ping = function() {
	var me = this;
    var success = _wsParent.send("system.ping", undefined, function(response){
		var response = response.data.substring(0, 31);
		_wsParent.log('Pong received: ' + response);
		var ok = (response == _pong);
		if (!ok) me.ws.close();
		
		// Return callback
		if (_sendCallback)
			me.ws.onmessage = _sendCallback;
		_sendCallback = undefined;
	});
	if (!success) 
	{
		if (_wsParent.isConnected())
			this.ws.close();

		this.ws = this.connect(address);
	}
	return success;
  },

  this.isConnected = function() {
    return this.ws.OPEN && _wsParent._connected;
  },

  this.disconnect = function() {
    this.log ("Closing.");
    this.ws.close();
  };
}
