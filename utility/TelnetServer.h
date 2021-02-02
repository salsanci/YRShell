#ifndef TelnetServer_h
#define TelnetServer_h


#include <ESP8266WiFi.h>
#include "DebugLog.h"
#include "CharQ.h"

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
  void init( unsigned port, DebugLog* log );
  void slice( void);
};


class TelnetConsoleServer : public TelnetServer {
protected:
  CQ32 m_fq;
  CQ32 m_tq;
public:
  TelnetConsoleServer(const char* inQ, const char* outQ):m_fq{inQ}, m_tq{outQ} { }
  virtual ~TelnetConsoleServer() { }
  virtual const char* sliceName( ) { return "TelnetConsoleServer"; }
  void init( unsigned port, DebugLog* log = NULL);
};


class TelnetLogServer : public TelnetServer {
protected:
  CQ32 m_fq;
  CQ1024 m_tq;
public:
  TelnetLogServer(const char* inQ, const char* outQ):m_fq{inQ}, m_tq{outQ} { }
  virtual ~TelnetLogServer() { }
  virtual const char* sliceName( ) { return "TelnetLogServer"; }
  void init( unsigned port,  DebugLog* log = NULL);
};

#endif