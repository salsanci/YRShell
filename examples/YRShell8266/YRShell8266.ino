#include "YRShell8266.h"
#include "WifiConnection.h"

//  0x01 - setup log
//  0x02 - errors
//  0x04 - exec output
//  0x08 - file load output
 
//  0x0100 - telnet long slice
//  0x0200 - telnet connect / disconnect
//  0x0400 - telnet char received
//  0x0800 - telnet control
//  0x1000 - telnet state change

// 0x010000 - http long slice
// 0x020000 - http connect / disconnect
// 0x040000 - http request
// 0x080000 - http request process
// 0x100000 - http state change
// 0x200000 - http log

// 0x80000000 - YRShellInterpreter debug

#define LOG_MASK 0xFFFFFFFF

#define LED_PIN 16

DebugLog dbg;
YRShell8266 shell;
LedBlink onBoardLed;
WifiConnection wifiConnection(&onBoardLed, &dbg);


void setup(){
  dbg.setMask( LOG_MASK);

  BSerial.begin( 500000);

  analogWriteFreq( 100);
  analogWriteRange(1023);

  dbg.print( __FILE__, __LINE__, 1, "\r\n\n");

  if(!LittleFS.begin()) {
    dbg.print( __FILE__, __LINE__, 1, "setup_Could_not_mount_file_system:");
  } else {
    dbg.print( __FILE__, __LINE__, 1, "setup_Mounted_file_system:");
  }

  if( !LittleFS.exists( wifiConnection.networkParameters.fileName())) {
    // Gateway - 192.168.10.1
    // Ip - 192.168.10.2
    wifiConnection.networkParameters.setHost("esp8266", "esp8266password", "0x020AA8C0" , "0x010AA8C0", "0x00FFFFFF" );
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.addNetwork( "", "");
    wifiConnection.networkParameters.save();
    dbg.print( __FILE__, __LINE__, 1, "Network parameters have been re-initialized");
 }
  
  onBoardLed.setLedPin( LED_PIN); 
  wifiConnection.enable();

  shell.init( 80, 23, &wifiConnection, &onBoardLed, &dbg, 2023);
  dbg.print( __FILE__, __LINE__, 1, "setup_done:");
}

void loop() {
  Sliceable::sliceAll( );
  if( dbg.valueAvailable() && shell.telnetLogSpaceAvaliable( 32) && Serial.availableForWrite() > 32) {
    char c;
    for( uint8_t i = 0; i < 32 && dbg.valueAvailable(); i++) {
      c = dbg.get();
      shell.telnetLogPut( c);
      Serial.print( c );
    }
  }
}
