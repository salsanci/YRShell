#ifndef YRShell8266_h
#define YRShell8266_h

#include "YRShell.h"
#include "WifiConnection.h"

#include "utility/DebugLog.h"

#include <ESP8266WiFi.h>
#include <LittleFS.h>

#include "utility/HttpServer.h"
#include "utility/TelnetServer.h"
#include "utility/LedBlink.h"

extern void setLedOnOffMs( uint32_t t, uint32_t u);

class YRShell8266;
class TelnetServer;
class TelnetLogServer;

class HServer : public HttpServer {
protected:
  YRShell8266* m_shell;
  char m_auxBuf[ 128];
  uint8_t m_auxBufIndex;

  bool m_lastPromptEnable, m_lastCommandEcho;

  virtual void exec( const char *p);
  virtual void startExec( void);
  virtual void endExec( void);
  virtual bool sendExecReply( void);
  
public:
  HServer( YRShell8266* s) { m_shell = s; } 
  virtual ~HServer() {}
  virtual const char* sliceName( ) { return "HServer"; }
};

typedef enum {
    SE_CC_first = YRSHELL_DICTIONARY_EXTENSION_FUNCTION,
    SE_CC_setPinIn,
    SE_CC_setPinInPullup,
    SE_CC_setPinOut,
    SE_CC_setDigitalPin,
    SE_CC_setAnalogPin,
    SE_CC_getDigitalPin,
    SE_CC_getAnalogPin,
    SE_CC_ledPush,
    SE_CC_ledPop,
    SE_CC_setLedOnOffMs,
    SE_CC_setLogMask,
    SE_CC_execDone,
    SE_CC_hexModeQ,
    SE_CC_wifiConnected,
    SE_CC_setTelnetLogEnable,

    SE_CC_getHostName,
    SE_CC_getHostIp,
    SE_CC_getHostGateway,
    SE_CC_getHostMask,
    SE_CC_getHostMac,

    SE_CC_getNumberOfNetworks,
    SE_CC_getNetworkName,

    SE_CC_setHostName,        
    SE_CC_setHostPassword,        
    SE_CC_setHostIp,          
    SE_CC_setHostGateway,     
    SE_CC_setHostMask,        
    SE_CC_setNetworkName,     
    SE_CC_setNetworkPassword, 

    SE_CC_loadFile,

    SE_CC_dbgM,
    SE_CC_dbgDM,
    SE_CC_dbgDDM,
    SE_CC_dbgXM,
    SE_CC_dbgXXM,

    SE_CC_hardReset,

    SE_CC_dotUb,
    SE_CC_strToInt,
    
    SE_CC_last
} SE_CC_functions;

class YRShell8266 : public virtual YRShellBase<2048, 128, 128, 16, 16, 16, 8, 256, 512, 256, 512, 128> {
protected:
  bool m_exec, m_initialized, m_telnetLogEnable;
  char m_auxBuf[ 128];
  uint8_t m_auxBufIndex;

  HServer* m_httpServer;
  TelnetServer* m_telnetServer;
  TelnetLogServer* m_telnetLogServer;
  LedBlink* m_led;
  WifiConnection* m_wifiConnection;
  IntervalTimer m_execTimer;
  bool m_fileOpen, m_initialFileLoaded, m_lastPromptEnable, m_lastCommandEcho;
  File m_file;

  virtual void executeFunction( uint16_t n);
  virtual const char* shellClass( void) { return "YRShell8266"; }
  virtual const char* mainFileName( ) { return "examples/YRShell8266/YRShell8266.ino"; }
  void outUInt8( int8_t v);

public:
  YRShell8266( );
  virtual ~YRShell8266( );
  void init( unsigned httpPort, unsigned telnetPort, WifiConnection* wifiConnection, LedBlink* led, DebugLog* log, unsigned debugTelnetPort = 0);
  virtual void slice( void);
  void loadFile( const char* fname, bool exec = true);
  void startExec( void);
  void endExec( void);
  void execString( const char* p);
  bool isExec( void) { return m_exec; }
  void telnetLogPut( char c);
  bool telnetLogSpaceAvaliable( uint16_t n = 1);
};

#endif