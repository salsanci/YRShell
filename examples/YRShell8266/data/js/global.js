

let global_pinState = []

function toHex(d) {
  return  ("0"+(Number(d).toString(16))).slice(-2).toUpperCase()
}

function global_loadPinConfig( ) {
  logs_msg( "global_loadPinConfig")
  var t = localStorage.getItem( "global_pinState")
  if( t == null)  {
    global_resetPinConfig()
  } else {
    global_pinState = JSON.parse( t)
  }
}
function global_resetPinConfig( ) {
  logs_msg( "global_resetPinConfig")
  global_pinState = [
  {
    "Name": "A0",
    "Type": "INPUT_ANALOG",
    "Modifiable": false
  },
  {
    "Name": "Reserved0",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "Reserved1",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "GPIO10",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "GPIO9",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "SD1",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "CMD",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "SD0",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "CLK",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "GND-0",
    "Type": "GROUND",
    "Modifiable": false
  },
  {
    "Name": "3.3V-0",
    "Type": "POWER33",
    "Modifiable": false
  },
  {
    "Name": "EN",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "RST",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "GND-1",
    "Type": "GROUND",
    "Modifiable": false
  },
  {
    "Name": "Vin",
    "Type": "POWER50",
    "Modifiable": false
  },
  {
    "Name": "OnboardLed",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "GPIO5",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "GPIO4",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "GPIO0",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "GPIO2",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "3.3V-1",
    "Type": "POWER33",
    "Modifiable": false
  },
  {
    "Name": "GND-2",
    "Type": "GROUND",
    "Modifiable": false
  },
  {
    "Name": "GPIO14",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "GPIO12",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "GPIO13",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "GPIO15",
    "Type": "INPUT",
    "Modifiable": true
  },
  {
    "Name": "RX",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "TX",
    "Type": "RESERVED",
    "Modifiable": false
  },
  {
    "Name": "GND-3",
    "Type": "GROUND",
    "Modifiable": false
  },
  {
    "Name": "3.3V-2",
    "Type": "POWER33",
    "Modifiable": false
  }
  ]

  global_pinState.forEach( function( o) {
    o.UserName = ""
  })
  global_setPinsInitialSetting( )
}
function global_setPinsInitialSetting( ) {
  global_pinState.forEach( function( o) {
    o.Value = 0
    o.TypeDirty = true
    o.ValueDirty = true
    if( o.Name.indexOf("GPIO") == 0 ){
      o.PinNumber = o.Name.substr(4)
    } 
  })
}
function global_savePinConfig( ) {
  localStorage.setItem( "global_pinState",  JSON.stringify( global_pinState))
  logs_msg( "global_savePinConfig")
}

global_loadPinConfig()