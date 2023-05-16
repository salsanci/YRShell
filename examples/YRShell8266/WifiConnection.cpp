#include "WifiConnection.h"
#include <ESP8266WiFi.h>


StringArray::StringArray(DebugLog* log) {
  m_log = log;
  reset();
}
void StringArray::reset( ) {
  memset( m_s.m_index, 0xFFFF, sizeof( m_s.m_index));
  m_s.m_lastOffset = 1;
  m_s.m_lastIndex = 0;
  m_s.m_buf[0] = 0;
}
void StringArray::resetString(uint8_t index, const char* s) {
  const char* d = get(index);
  if( strlen(d) == strlen(s)) {
    strcpy( (char*) d, s);
  } else {
    if( m_log) {
      m_log->print( __FILE__, __LINE__, 1, index, s, "StringArray_resetString_Failed: index, string" );
    }
  }
}

void StringArray::dump() {
  if( m_log) {
    m_log->print( __FILE__, __LINE__, 1, m_s.m_lastIndex, m_s.m_lastOffset, "StringArray_dump: lastIndex, lastOffset" );
    for( uint8_t i = 0; i < m_s.m_lastIndex && i <  ( sizeof( m_s.m_index) / sizeof(m_s.m_index[0])); i++ ) {
      m_log->print( __FILE__, __LINE__, 1, i, m_s.m_index[i], &m_s.m_buf[ m_s.m_index[ i]], "StringArray_dump: i, index, string" );
      m_log->flush();
    }
  }
}
void StringArray::append( const char* s) {
  if( m_s.m_lastIndex < ( sizeof( m_s.m_index) / sizeof(m_s.m_index[0])) ) {
    set( m_s.m_lastIndex++, s);
    if( m_log) {
      m_log->print( __FILE__, __LINE__, 1, m_s.m_lastIndex, s, "StringArray_append: lastIndex, string" );
    }
  }
}
void StringArray::set( uint8_t index, const char* s) {
  if( *s == '\0') {
    m_s.m_index[ index] = 0;
  } else {
    size_t sz = strlen( s) + 1;
    if( index < ( sizeof( m_s.m_index) / sizeof(m_s.m_index[0])) && sz < (sizeof(m_s.m_buf) - m_s.m_lastOffset) ) {
      m_s.m_index[ index] = m_s.m_lastOffset;
      strcpy( &m_s.m_buf[ m_s.m_lastOffset], s );
      m_s.m_lastOffset += sz;
    } else {
      if( m_log) {
        m_log->print( __FILE__, __LINE__, 1, index, "StringArray_set_failed: index" );
      }
    }
  }
}
const char* StringArray::get( uint8_t index) {
  const char * rc = "";
  if( index < m_s.m_lastIndex && index < ( sizeof( m_s.m_index) / sizeof(m_s.m_index[0])) && m_s.m_index[index] < m_s.m_lastOffset ) {
    rc =  &m_s.m_buf[m_s.m_index[ index]];
  } else {
    if( m_log) {
      m_log->print( __FILE__, __LINE__, 1, index, "StringArray_get_failed: index" );
    }
  }
  return rc;
}

void StringArray::load(const char* fname) {
  uint16_t i;
  File file = LittleFS.open(fname, "r");
  if( !file) {
    if( m_log) {
      m_log->print( __FILE__, __LINE__, 1, fname, "StringArray_load_failed: fname" );
    }
    reset();
  } else {
    char* p = (char*) &m_s;
    for( i = 0; i < sizeof( m_s) && file.available(); i++) {
      *p++ = (char) file.read();
    }
    file.close();
    if( i  == sizeof(m_s)) {
      if( m_log) {
        m_log->print( __FILE__, __LINE__, 1, fname, "StringArray_load: fname" );
        dump();
      }
    } else {
      if( m_log) {
        m_log->print( __FILE__, __LINE__, 1, fname, "StringArray_load_failed: fname" );
      }
      reset();
    }
  }
}

void StringArray::save(const char* fname ) {
    File file = LittleFS.open(fname, "w");
    if( !file) {
      if( m_log) {
        m_log->print( __FILE__, __LINE__, 1, fname, "StringArray_save: fname" );
      }
    } else {
      file.write( (uint8_t*)  &m_s, sizeof(m_s));
      file.close();
      if( m_log) {
        m_log->print( __FILE__, __LINE__, 1, fname, "StringArray_save_done: fname" );
      }
    }
}

NetworkParameters::NetworkParameters( DebugLog* log) : StringArray( log) {
  reset();
  m_numberOfFixedParameters = 6;
  append( "");
  append( "");
  append( "0x00000000");
  append( "0x00000000");
  append( "0x00000000");
  append( "0x00000000");
}

void NetworkParameters::resetUint32(uint8_t index, uint32_t v ) {
  const char* d = get( index);
  char buf[ 11];
  YRShellInterpreter::unsignedToStringX( v, 8, buf);
  resetString( index, buf);
}

void NetworkParameters::setHost(const char* networkName, const char* networkPassword,  const char* ip,  const char* gateway,  const char* mask ) {
  set( 0, networkName);
  set( 1, networkPassword);
  set( 2, ip);
  set( 3, gateway);
  set( 4, mask);
}
void NetworkParameters::setNetworkIp( uint32_t ip) {
  resetUint32( 5, ip);
}

void NetworkParameters::addNetwork(const char* networkName, const char* networkPassword ) {
  append( networkName);
  append( networkPassword);
}

const char* NetworkParameters::getNetworkName( uint8_t index) {
  const char* rc = "";
  uint8_t idx = index * 2 + m_numberOfFixedParameters;
  if( idx < m_s.m_lastIndex) {
    rc = get( idx);
  }
  return rc;
}
const char* NetworkParameters::getNetworkPassword( uint8_t index) {
  const char* rc = "";
  uint8_t idx = index * 2 + m_numberOfFixedParameters + 1;
  if( idx < m_s.m_lastIndex) {
    rc = get( idx);
  }
  return rc;
}

#define BLINK_SPEED_CONNECTING_MS 200
#define BLINK_SPEED_SCANNING_MS 400

WifiConnection::WifiConnection( LedBlink* led, DebugLog* log, uint32_t connectTimeout) : networkParameters(log) {
  m_led = led;
  m_log = log;
  m_connectTimeout = connectTimeout;
  m_currentAp = 0;
  m_state = 0;
  m_enable = false;
  m_maxRssi = -1024;
  m_hostActive = false;
}

void WifiConnection::changeState( uint8_t state) {
  if( m_log) {
    m_log->print( __FILE__, __LINE__, 1, m_state, state, "WifiConnection::changeState: m_state, state" );
  }
  m_state = state;
}

int WifiConnection::getConnectedNetworkIndex( void) {
  return isNetworkConnected() ? m_currentAp : -1;
}

bool WifiConnection::isNetworkConnected( void) {
  return WiFi.status() == WL_CONNECTED;
}

void WifiConnection::slice( ) {
  const char* p;
  const char* q;
  int i, j, k, m;
  switch( m_state) {
    case 0:
      networkParameters.load();
      m_currentAp = 0;
      if( m_led) {
        m_led->off();
      }
      changeState( 7);
    break;

    case 1:
      if( m_led) {
        m_led->off();
      }
      if( m_enable && networkParameters.getNumberOfNetworks() >= 0) {
        m_timer.setInterval( m_connectTimeout);
        changeState( 2);
      }
    break;

    case 2:
      if( m_led) {
        m_led->blink( BLINK_SPEED_CONNECTING_MS);
      }
      p = networkParameters.getNetworkName( m_currentAp );
      q = networkParameters.getNetworkPassword( m_currentAp );
      m_log->print( __FILE__, __LINE__, 1, m_currentAp, "WifiConnection::slice_connecting: currentAp" );
      m_log->print( __FILE__, __LINE__, 1, p, q, "WifiConnection::slice_connecting: p, q" );
      if( *p == '\0' || *q == '\0') {
        changeState( 3);
      } else {
        WiFi.begin( (char*)p,  q);
        changeState( 3);
        if( m_log) {
          m_log->print( __FILE__, __LINE__, 1, p, "WifiConnection::slice_connecting: networkName" );
        }
      }
    break;

    case 3:
      if( WiFi.status() == WL_CONNECTED) {
        networkParameters.setNetworkIp((uint32_t) WiFi.localIP());
        changeState( 5);
      } else if( m_timer.hasIntervalElapsed()) {
        changeState( 4);
      } 
    break;

    case 4:
      m_currentAp++;
      if( m_currentAp >= networkParameters.getNumberOfNetworks() ) {
        m_currentAp = 0;
      }
      changeState( 1);
    break;

    case 5:
      p = networkParameters.getNetworkName( m_currentAp );
      if( m_log) {
        m_log->print( __FILE__, __LINE__, 1,  networkParameters.get( m_currentAp), "WifiConnection::slice_connected: networkName" );
      }
      m_timer.setInterval( 500);
      changeState( 6);
      if( m_led) {
        m_led->on();
      }
    break;

    case 6:
      if( m_timer.isNextInterval() ) {
        if(WiFi.status() != WL_CONNECTED) {
            if( m_log) {
              m_log->print( __FILE__, __LINE__, 1,  networkParameters.getNetworkName( m_currentAp ), "WifiConnection::slice_disconnected: networkName" );
            }
          changeState( 4);
        }
      }
    break;

    case 7:
      WiFi.mode(WIFI_STA);
      WiFi.disconnect(  );
      m_timer.setInterval( 100);
      changeState( 8);
      m_maxRssi = -1024;
      m_maxRssiIndex = 0;
      if( m_led) {
        m_led->blink( BLINK_SPEED_SCANNING_MS);
      }

    break;

    case 8:
      if( m_timer.hasIntervalElapsed() ) {
        WiFi.scanNetworks( true);
        m_timer.setInterval( m_connectTimeout * 2);
        if( m_log) {
          m_log->print( __FILE__, __LINE__, 1, "WifiConnection_slice_scanStarted: " );
        }
        changeState( 9);
      }
    break;

    case 9:
      i = WiFi.scanComplete();
      if( i > 0) {
        m = networkParameters.getNumberOfNetworks();
        if( m_log) {
          m_log->print( __FILE__, __LINE__, 1, i, m, "WifiConnection_slice_scan: numberOfNetworks, lastIndex" );
        }
        for( int j = 0; j < i; j++ ) {
          for( k = 0; k < m; k++ ) {
            if( !strcmp( WiFi.SSID( j).c_str(), networkParameters.getNetworkName( k)) ) {
              int32_t RSSI;
              RSSI = WiFi.RSSI( j);
              if( RSSI > m_maxRssi) {
                m_maxRssi = RSSI;
                m_maxRssiIndex = k;
              }
              if( m_log) {
                m_log->print( __FILE__, __LINE__, 1, m_maxRssiIndex, 0 - m_maxRssi, WiFi.SSID( j).c_str(), "WifiConnection::slice_scan: maxRssiIndex, maxRssi, networkName");
              }
            }
          }
        }
        WiFi.scanDelete( );
        changeState( 10);
      } else if( m_timer.hasIntervalElapsed()) {
        if( m_log) {
          m_log->print( __FILE__, __LINE__, 1, i, "WifiConnection::slice_scan_timeout:" );
        }
        WiFi.scanDelete( );
        changeState( 10);
      }
    break;

    case 10:
      p = networkParameters.getHostName( );
      q = networkParameters.getHostPassword(  );
      m_log->print( __FILE__, __LINE__, 1, p, q, "WifiConnection::slice_host: p, q" );
      if( *p == '\0' || *q == '\0') {
        if( m_log) {
          m_log->print( __FILE__, __LINE__, 1, p, q, "WifiConnection::slice_no_host_notConnecting: p, q" );
        }
      } else {
        hostConfig( );
        if( WiFi.softAP( p, q) ) {
            m_hostActive = true;
          if( m_log) {
            m_log->print( __FILE__, __LINE__, 1, p, q, "WifiConnection_slice_host_up: networkName, networkPassword" );
            m_log->print( __FILE__, __LINE__, 1, WiFi.softAPIP().toString().c_str(), WiFi.softAPmacAddress().c_str(), "WifiConnection_slice_host_up: softApIp, softApMac" );
          }
        } else {
          m_hostActive = false;
          if( m_log) {
            m_log->print( __FILE__, __LINE__, 1, p, q, "WifiConnection::slice_host_not_up: p, q" );
          }
        }
      }
      changeState( 11);
    break;

    case 11:
      m_currentAp = m_maxRssiIndex == 0 ? 0 : m_maxRssiIndex;
      m_log->print( __FILE__, __LINE__, 1, m_enable,  m_currentAp, networkParameters.getNumberOfNetworks(), "WifiConnection::slice_scan_done: enable, m_currentAp, numberOfNetworks" );
      changeState( 1);
      if( m_led) {
        m_led->off();
      }
    break;
  }
}
void WifiConnection::hostConfig( ) {
  const char* p;
  uint32_t v;
  uint32_t ip = 0;
  uint32_t gw = 0;
  uint32_t mask = 0;

  if( YRShellInterpreter::stringToUnsignedX( networkParameters.getHostIp( ), &v)) {
    ip = v;
  }
  if( YRShellInterpreter::stringToUnsignedX( networkParameters.getHostGateway( ), &v)) {
    gw = v;
  }
  if( YRShellInterpreter::stringToUnsignedX( networkParameters.getHostMask( ), &v)) {
    mask = v;
  }
  if( !WiFi.softAPConfig( IPAddress(ip), IPAddress(gw) , IPAddress( mask))) {
    if( m_log) {
      m_log->print( __FILE__, __LINE__, 1, "WifiConnection_hostConfig_failed" );
    }
  }
}
