#ifndef WifiConnection_h
#define WifiConnection_h

#include <LittleFS.h>
#include <ESP8266WiFi.h>

#include "utility/Sliceable.h"
#include "utility/IntervalTimer.h"
#include "utility/DebugLog.h"
#include "utility/LedBlink.h"

typedef struct {
  uint16_t m_index[ 32];
  uint16_t m_lastIndex;
  uint16_t m_lastOffset;
  char m_buf[ 512];
} StringArrayStruct;

class StringArray {
protected:
  StringArrayStruct m_s;
  DebugLog* m_log;
  void resetString(uint8_t index, const char* s);
public:
  StringArray( DebugLog* log = NULL);
  ~StringArray() { }
  void set( uint8_t index, const char* s);
  void append( const char* s);
  const char* get( uint8_t index);
  uint16_t getLastIndex( void) { return m_s.m_lastIndex; }
  void reset( void);
  void load( const char* fname);
  void save( const char* fname);
  void dump( void);
};

class NetworkParameters : public StringArray {
protected:
  uint8_t m_numberOfFixedParameters;
  void resetUint32(uint8_t index, uint32_t v );

public:
  NetworkParameters( DebugLog* log = NULL);
  ~NetworkParameters( ){}
  void setHost(const char* networkName, const char* networkPassword,  const char* ip,  const char* gateway,  const char* mask );
  void addNetwork(const char* networkName, const char* networkPassword );
  const char* fileName( void) { return "/NetworkParameters"; }
  const char* getHostName( void) { return get(0); }
  const char* getHostPassword( void) { return get(1); }
  const char* getHostIp( void) { return get(2); }
  const char* getHostGateway( void) { return get(3); }
  const char* getHostMask( void) { return get(4); }
  const char* getNetworkIp( void) { return get(5); }
  const char* getNetworkName( uint8_t index);
  const char* getNetworkPassword( uint8_t index);
  uint8_t getNumberOfNetworks( void) { return (m_s.m_lastIndex - m_numberOfFixedParameters)/2; }
  void save( void) { StringArray::save( fileName()); }
  void load( void) { StringArray::load( fileName()); }
  void getHostMac( char* buf) { WiFi.softAPmacAddress( (uint8_t*) buf); }
  void getNetworkMac( char* buf) { WiFi.macAddress( (uint8_t*) buf); }

  void setHostName( const char* networkName) { set( 0, networkName); }
  void setHostPassword( const char* networkPassword ) { set( 1, networkPassword); }
  void setHostIp( const char* ip ) { set( 2, ip); }
  void setHostGateway( const char* gateway) { set( 3, gateway); }
  void setHostMask( const char* mask) { set( 4, mask); }
  void setNetworkIp( uint32_t ip);
  void setNetworkMac( const char* mac);

  void setNetworkName( uint8_t index, const char* networkName) { set( index * 2 + m_numberOfFixedParameters, networkName); }
  void setNetworkPassword( uint8_t index, const char* networkPassword) { set( index * 2 + m_numberOfFixedParameters + 1, networkPassword); }

};



class WifiConnection : public Sliceable {
protected:
  uint8_t m_currentAp, m_state;
  uint32_t m_connectTimeout;
  int32_t m_maxRssi;
  uint8_t m_maxRssiIndex;
  DebugLog* m_log;
  LedBlink* m_led;
  bool m_enable;
  IntervalTimer m_timer;
  bool m_hostActive;
  void changeState( uint8_t state);
  void hostConfig( void);

public:
  WifiConnection( LedBlink* led, DebugLog* log = NULL, uint32_t connectTimeout = 5000); 
  virtual ~WifiConnection() { }
  virtual const char* sliceName( void) { return "WifiConnection"; }
  virtual void slice( void);
  int getConnectedNetworkIndex( void);
  bool isNetworkConnected( void);

  NetworkParameters networkParameters;

  bool isHostActive( void) { return m_hostActive; }
  void enable( void) { m_enable = true; }
  void disable( void) { m_enable = false; }
};

#endif
