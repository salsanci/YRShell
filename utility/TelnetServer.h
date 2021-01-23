#ifndef TelnetServer_h
#define TelnetServer_h

#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "DebugLog.h"
#include <YRShell8266.h>

class YRShell8266;

class TelnetServer : public Sliceable {
protected:

  WiFiServer* m_server;
  WiFiClient* m_client;

  CircularQBase<char>* m_fromTelnetQ;
  CircularQBase<char>* m_toTelnetQ; 
  DebugLog* m_log;

  IntervalTimer m_timer;
  uint8_t m_data0, m_data1, m_state;
  bool m_lastCharWasNull, m_flipFlop, m_lastConnected;

  void changeState( uint8_t newState);

public:
  TelnetServer(void);
  virtual ~TelnetServer();
  virtual const char* sliceName( ) { return "TelnetServer"; }
  void init( unsigned port, YRShell8266* shell, DebugLog* log );
  void slice( void);
};

class CQ1 : public CircularQ<char, 32> {
public:
  CQ1() {}
  virtual ~CQ1() {}
  virtual const char* sliceName( void) { return "CQ1"; } 
  virtual void slice( void) { }
};
class CQ2 : public CircularQ<char, 1024> {
public:
  CQ2() {}
  virtual ~CQ2() {}
  virtual const char* sliceName( void) { return "CQ2"; } 
  virtual void slice( void) { }
};

class TelnetLogServer : public TelnetServer {
protected:
  CQ1 m_fq;
  CQ2 m_tq;
public:
  TelnetLogServer(void) { }
  virtual ~TelnetLogServer() { }
  virtual const char* sliceName( ) { return "TelnetLogServer"; }
  void init( unsigned port);
  void put( char c ) { m_tq.put( c); }
  bool spaceAvailable( uint16_t n = 1) { return m_toTelnetQ->spaceAvailable( n); }
};

#endif