#include <YRShell8266.h>
#include <WifiConnection.h>
#include <SPI.h>
#include "Dht22.h"

IntervalTimer dhtInterval;

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

#define LOG_MASK 0xFFFFFBFF

#define LED_PIN 16
#define DHTPIN 10    // Digital pin connected to the DHT sensor

DebugLog dbg;
TelnetConsoleServer console( "ConsoleTelnetInQ", "ConsoleTelnetOutQ");
TelnetLogServer logConsole( "LogTelnetInQ",  "LogTelnetOutQ" );

CircularQBase<char>* telnetLogQ;

YRShell8266 shell("ShellMainInQ", "ShellMainOutQ", "ShellAuxInQ", "ShellAuxOutQ" );
LedBlink onBoardLed;
WifiConnection wifiConnection(&onBoardLed, &dbg);

Dht22 dht( DHTPIN);

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

  console.init( 23, &dbg);
  logConsole.init( 2023, &dbg);
  shell.init( 80, &wifiConnection, &onBoardLed, &dbg);

  CharQ::connect( "ConsoleTelnetInQ", "ShellMainInQ");
  CharQ::connect( "ShellMainOutQ", "ConsoleTelnetOutQ"); 
  telnetLogQ = CharQ::find( "LogTelnetOutQ"); 

  dhtInterval.setInterval( 3000);
  dbg.print( __FILE__, __LINE__, 1, "setup_done:");
}

void loop() {
  Sliceable::sliceAll( );
  if( dhtInterval.isNextInterval()) {
    dht.dhtRead();
    //dbg.print( __FILE__, __LINE__, 1, Dht22::getRc(),  Dht22::getRh(),  Dht22::getTemp(), "dhtRead: rc, rh, temp");
  }
  if( dbg.valueAvailable() && Serial.availableForWrite() > 32) {
    char c;
    for( uint8_t i = 0; i < 32 && dbg.valueAvailable(); i++) {
      c = dbg.get();
      if( telnetLogQ) {
        telnetLogQ->put( c);
      }
      Serial.print( c );
    }
  }
}
