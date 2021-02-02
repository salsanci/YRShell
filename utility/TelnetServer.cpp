#include "TelnetServer.h"

void TelnetConsoleServer::init( unsigned port, DebugLog* log) {
    m_fromTelnetQ = &m_fq;
    m_toTelnetQ = &m_tq;
    m_server = new WiFiServer( port);
    m_log = log;
    m_server->begin();
    m_client = new WiFiClient();
    m_timer.setInterval( 10);
}
void TelnetLogServer::init( unsigned port, DebugLog* log) {
    m_fromTelnetQ = &m_fq;
    m_toTelnetQ = &m_tq;
    m_server = new WiFiServer( port);
    m_log = log;
    m_server->begin();
    m_client = new WiFiClient();
    m_timer.setInterval( 10);
}

TelnetServer::TelnetServer() {
  m_fromTelnetQ = NULL;
  m_toTelnetQ = NULL;

  m_server = NULL;
  m_client = NULL;
  m_log = NULL;
  m_data0 = m_data1 = 0;
  m_state = 0;
  m_lastCharWasNull =  m_flipFlop = m_lastConnected =  false;
}

TelnetServer::~TelnetServer() {
  if( m_server == NULL) {
    delete m_server;
    m_server = NULL;
  }
  if( m_client == NULL) {
    delete m_client;
    m_client = NULL;
  }
  m_fromTelnetQ = NULL;
  m_toTelnetQ = NULL;
  m_log = NULL;
  m_server = NULL;
  m_client = NULL;
}

void TelnetServer::init( unsigned port, DebugLog* log) {
    m_server = new WiFiServer( port);
    m_log = log;
    m_server->begin();
    m_client = new WiFiClient();
    m_timer.setInterval( 10);
}

void TelnetServer::changeState( uint8_t newState) {
  if( m_log != NULL) {
    m_log->print( __FILE__, __LINE__, 0x1000, (uint32_t) m_state, (uint32_t) newState, "TelnetServer_changeState: state, newState");
  }
  m_state = newState;
}

void TelnetServer::slice() {
  if( m_timer.isNextInterval()) {
    uint32_t start = HW_getMicros();
    uint8_t startState = m_state;
    uint8_t data;

    if( m_lastConnected && !m_client->connected()) {
      if( m_log != NULL) {
        m_log->print( __FILE__, __LINE__, 0x0200, "TelnetServer_client_disconnected:");
      }
      m_lastConnected = false;
      changeState( 0);
    } else {
      switch( m_state) {
        case 0:
          *m_client = m_server->available();
          if( *m_client) {
            m_lastConnected = true;
            if( m_log != NULL) {
              m_log->print( __FILE__, __LINE__, 0x0200, "TelnetServer_client_connected:");
            }
            changeState( 1);
          }
        break;
        
        case 1:
          if( m_flipFlop) {
            m_flipFlop = false;
            if( m_fromTelnetQ) {
              if( m_client->available() && m_fromTelnetQ->spaceAvailable()) {
                data =  m_client->read( );
                if( m_log != NULL) {
                  m_log->print( __FILE__, __LINE__, 0x0400, data, "TelnetServer: charReceived");
                }
                if( data != 0xFF) {
                  if( data || m_lastCharWasNull ) {
                    m_fromTelnetQ->put( data);
                    m_lastCharWasNull = false;
                  } else {
                      m_lastCharWasNull = true;
                  }
                } else {
                  m_data0 = 0xFF;
                  changeState( 2);
                }
              }
            }
          } else {
            m_flipFlop = true;
            if( m_toTelnetQ) {
              const char* p = m_toTelnetQ->getLinearReadBuffer();
              size_t len = m_toTelnetQ->getLinearReadBufferSize();
              size_t aw = m_client->availableForWrite();
              if( len > 0 && aw > 0) {
                if( len > aw) {
                  len = aw;
                }
                size_t bw = m_client->write((uint8_t*) p , len );    
                if( bw > 0) {
                  m_toTelnetQ->drop( bw);
                }
              }
            }
          }
        break;
        
        case 2:
          if( m_client->available() && m_fromTelnetQ->spaceAvailable()) {
            data = m_client->read();
            if( data == 0xFF) {
              m_fromTelnetQ->put( data);
              changeState( 1);
            } else {
              m_data1 = data;
              changeState( 3);
            }
          }
        break;
        
        case 3:
          if( m_client->available() && m_fromTelnetQ->spaceAvailable()) {
            data = m_client->read();
            if( m_log != NULL) {
              m_log->printX( __FILE__, __LINE__, 0x0800, m_data0, m_data1, data,  "TelnetServer_slice_request: m_data0, m_data1, data");
            }
            if( m_data1 == 0xFB && data == 0x03) {
              m_data1 = 0xFD;
            } else if( m_data1 == 0xFD && data == 0x03) {
              m_data1 = 0xFB;
            } else if( m_data1 == 0xFD && data == 0x01) {
              m_data1 = 0xFB;
            } else if( m_data1 == 0xFC) {
              m_data1 = 0xFE;
            } else if( m_data1 == 0xFE) {
              m_data1 = 0xFB;
            } else {
              m_data0 = m_data1 = 0;
            }
            if( m_data0) {
              m_client->write( m_data0);
              m_client->write( m_data1);
              m_client->write( data );
              if( m_log != NULL) {
                m_log->printX( __FILE__, __LINE__, 0x0800, m_data0, m_data1, data,  "TelnetServer_slice_response: m_data0, m_data1, data");
              }
              m_data0 = m_data1 = 0;
            }
            changeState( 1);
          }
        break;
      }
    }
    unsigned et =  HW_getMicros() - start;

    if(  m_log != NULL && et > 900) {
      m_log->print( __FILE__, __LINE__, 0x0100, startState, m_state, et, "TelnetServer_slice: startState, m_state, time");
    }
  }
}
