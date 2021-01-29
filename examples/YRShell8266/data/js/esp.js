
var esp_baseUrl = "http://esp0/"

function esp_asciiToHex( s) {
	var arr = []
	for (var n = 0, l = s.length; n < l; n ++) {
		var hex = Number(s.charCodeAt(n)).toString(16)
		arr.push(hex)
  }
	return arr.join('')
}

function esp_sendConsole( url,  rf ) {
  fetch( new Request( esp_baseUrl + 'cmd/' + esp_asciiToHex( url)) )
  .then(response => response.text())
  .then(data => {
    var s = data.indexOf("<pre>")
    var e = data.indexOf("</pre>")
    if( s > 0 && e > s) {
      data = data.substr( s + 5, e - s - 5)
    }
    rf(data.trim())
  })
  .catch(console.error);  
}
function esp_getJson( url,  rf ) {
  fetch( new Request( esp_baseUrl + 'exec/' + esp_asciiToHex( url)) )
  .then(response => response.json())
  .then(data => {
    rf(data)
  })
  .catch(console.error);  
}

function esp_setBase( b) {
  esp_baseUrl = b
}

function esp_processPins( rf) {
  var cmd = ""
  cmd = esp_processPinsType( cmd);
  cmd = esp_processPinsOut( cmd);  
  cmd += " jsonPins"
  if( cmd.length > 0) {
    fetch( new Request( esp_baseUrl + 'exec/' + esp_asciiToHex( cmd)) )
    .then(response => response.json())
    .then(data => {
      esp_processPinsIn( data)
      rf()
    })
    .catch(console.error);  
  }
}

function esp_processPinsIn( o) {
  var k = Object.keys( o)
  k.forEach( function( obk) {
    var e = document.getElementById( "__id__pinProgress" + obk)
    if( e) {
      e.value = o[ obk]
    }
  })
}

function esp_processPinsOut( cmd) {
  for( var i = 0; i < global_pinState.length && cmd.length < 40; i++) {
    var o = global_pinState[i]
    if( o.hasOwnProperty("PinNumber")  &&  o.ValueDirty) {
      o.ValueDirty = false
      if( o.Type == "OUTPUT") {
        cmd += o.Value + " "
        cmd += o.PinNumber + " "
        cmd += "sdp "
      } else if( o.Type == "OUTPUT_ANALOG") {
        cmd += o.Value + " "
        cmd += o.PinNumber + " "
        cmd += "sap "
      }
    }
  }
  return cmd
}

function esp_processPinsType( cmd) {
  for( var i = 0; i < global_pinState.length && cmd.length < 40; i++) {
    var o = global_pinState[i]
    if( o.hasOwnProperty("PinNumber")  ) {
      if( o.TypeDirty) {
        flag = false
        o.TypeDirty = false
        cmd += o.PinNumber + " "
        if( o.Type == "INPUT") {
          cmd += "spi "
        } else if( o.Type == "INPUT_PULLUP") {
          cmd += "spip "
        } else {
          cmd += "spo "
        }
      }
    }
  }
  return cmd
}

