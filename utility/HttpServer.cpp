#include "HttpServer.h"

HttpServer::HttpServer( void) {
  m_server = NULL;
  m_client = NULL;

  m_log = NULL;
  m_led = NULL;
  
  m_state = 0;
  m_urlIndex = 0;
}

HttpServer::~HttpServer( void) {
  if( m_server != NULL) {
    delete m_server;
    m_server = NULL;
  }
  if( m_client != NULL) {
    delete m_client;
    m_client = NULL;
  }
  m_log = NULL;
  m_state = 0;
  m_responseCode = 0;
  m_urlIndex = 0;
}

void HttpServer::init(unsigned port, DebugLog* log, LedBlink* led) {
    m_server = new WiFiServer(port);
    m_log = log;
    m_led = led;
    m_server->begin();
    m_client = NULL;
    m_state = 0;
    m_urlIndex = 0;
}
unsigned HttpServer::readFile( char* P, unsigned len) {
  uint32_t start = HW_getMicros();
  unsigned rc =  m_sendFile.readBytes( m_buf, len);
  unsigned et =  HW_getMicros() - start;
  if( m_log != NULL && et > 900) {
    m_log->print( __FILE__, __LINE__, 0x010000, rc, len, et, "readFile: rc, len, time");
  }
  return rc;
}
int HttpServer::clientRead( char* P, unsigned len) {
  uint32_t start = HW_getMicros();
  int rc = m_client->read((uint8_t*) P, len);    
  unsigned et =  HW_getMicros() - start;
  if( m_log != NULL && rc  && et > 900) {
    m_log->print( __FILE__, __LINE__, 0x010000, rc, len, et, "clientRead: rc, len, time");
  }
  return rc;
}
void HttpServer::clientWrite( const char* P){
  clientWrite( P, strlen(P));
}
void HttpServer::clientWrite( const char* P, unsigned len){
  uint32_t start = HW_getMicros();
  m_client->write( P, len);
  unsigned et =  HW_getMicros() - start;
  if( m_log != NULL  && et > 900) {
    m_log->print( __FILE__, __LINE__, 0x010000, len, et, "clientWrite: len, time");
  }
}
char HttpServer::hexToAscii( const char* h) {
  char rc = YRShellInterpreter::charToHex( *h++);
  rc <<= 4;
  rc += YRShellInterpreter::charToHex( *h);
  return rc;
}
void HttpServer::sendExec(  uint8_t offset ) {
  uint16_t i;
  for( i = 0; i < (sizeof(m_url) - offset -1) && m_url[ offset + (i*2)] != '\0'; i++) {
    m_url[ offset + i] = hexToAscii( &m_url[ offset + (i *2)]);
  }
  m_url[ offset + i] = '\0';
  const char*p = &m_url[ offset];
  if( m_log != NULL) {
    m_log->print( __FILE__, __LINE__, 0x080000, p, "HttpServer_sendExec: exec");
  }
  startExec();
  exec( p);
}

void HttpServer::sendFile( const char* type) {
  m_sendFile = LittleFS.open(m_url, "r");
  if( !m_sendFile) {
    send404();
  } else if( m_client != NULL) {
    if( m_log != NULL) {
      m_log->print( __FILE__, __LINE__, 0x080000, m_url, "httpServer_sendFile: url");
    }
    clientWrite("HTTP/1.0 200 OK\r\nContent-type: ");
    clientWrite(type);
    clientWrite("\r\nCache-Control: max-age=3600\r\n\r\n");
    m_responseCode = 200;
  }
  changeState(6);
}

void HttpServer::send404(  ) {
  if( m_client != NULL) {
    if( m_log != NULL) {
      m_log->print( __FILE__, __LINE__, 0x080000, m_url, "httpServer_send404: url");
    }
    m_responseCode = 404;
    clientWrite("HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nCache-Control: no-cache\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><title>404 Error</title></head><body><h1>404 Error</h1></body></html>");
  }
  changeState( 3);
}

void HttpServer::changeState( uint8_t newState) {
  if( m_log != NULL) {
    m_log->print( __FILE__, __LINE__, 0x100000, (uint32_t) m_state, (uint32_t) newState, "httpServer_changeState: state, newState");
  }
  m_state = newState;
}

void HttpServer::slice() {
  uint32_t start = HW_getMicros();
  uint8_t startState = m_state;
  switch( m_state) {
    case 0:
      m_client = new WiFiClient;
      changeState( 1);
      m_responseCode = 0;
      m_urlIndex = 0;
      m_url[ 0] = '\0';
    break;
    case 1:
      *m_client = m_server->available();
      if( *m_client) {
        m_timer.setInterval( 100);
        if( m_log != NULL) {
          m_log->print( __FILE__, __LINE__, 0x020000, "HttpServer_slice_Connected:");
        }
        changeState( 20);
        if( m_led) {
          m_led->push();
          m_led->blink( 50);
        }
      }
    break;
    case 2:
    {
      m_timer.setInterval( 20000);
      if( m_log != NULL) {
        m_log->print( __FILE__, __LINE__, 0x040000, m_url, "HttpServer_slice: req");
      }

      if( strncmp( m_url, "GET ", 4) && strncmp( m_url, "GET ", 4)) {
        send404();
      } else {
        for( uint8_t i = 4; i < sizeof( m_url); i++) {
          char c = m_url[i];
          if( c == ' ' ) {
            m_url[ i - 4] = '\0';
            break;
          } else {
            m_url[ i - 4] = c;
          }
        }
        if( m_url[0] == '/' && m_url[ 1] == '\0') {
          strcpy( m_url, "/index.html");
        }
        if( strlen( m_url) <  4) {
          changeState( 3);
        } else if( !strncmp( m_url, "/exec/", 6)) {
          sendExec( 6);
          changeState( 7);
        } else if( !strncmp( m_url, "/cmd/", 5)) {
          sendExec( 5);
          changeState( 9);
        } else {
          const char* suffix = m_url + strlen( m_url);
          while( suffix > m_url && *suffix != '.') {
            suffix--;
          }
          if(!strcmp( suffix, ".html")) {
            sendFile( "text/html");
          } else if( !strcmp( suffix, ".css")) {
            sendFile( "text/css");
          } else if( !strcmp( suffix, ".js")) {
            sendFile( "text/javascript");
          } else if( !strcmp( suffix, ".ico")) {
            sendFile( "image/x-icon");
          } else if( !strcmp( suffix, ".gif")) {
            sendFile("image/gif");
          } else if( !strcmp( suffix, ".png")) {
            sendFile("image/png");
          } else if( !strcmp( suffix, ".jpg") || !strcmp( suffix, ".jpeg") || !strcmp( suffix, ".jpe")) {
            sendFile("image/jpeg");
          } else {
            send404();
          }
        }
      }
     }
    break;
    case 3:
      if( m_client != NULL) {
        delete m_client;
        m_client = NULL;
      }
      if( m_log != NULL) {
        m_log->print( __FILE__, __LINE__, 0x020000, "HttpServer_slice_Disconnected:");
      }
      m_timer.setInterval( 1);
      changeState( 4);
    break;
    case 4:
      if( m_log !=  NULL) {
        uint32_t et = HW_getMicros() - m_requestStart;
        et = (et + 500)/1000;
        m_log->printLog( __FILE__, __LINE__, 0x200000, m_responseCode, et, m_url);
        if( m_led) {
          m_led->pop();
        }
      }
      changeState( 5);
    break;
    case 5:
      changeState( 0);
    break;
    case 6:
      if( m_timer.hasIntervalElapsed()) {
        m_sendFile.close();
        changeState( 3);
      } else {
        if( m_client != NULL && m_client->availableForWrite() >= sizeof(m_buf)) {
          size_t br = readFile( m_buf, sizeof(m_buf)); 
          if( br > 0) {
            clientWrite( m_buf, br);
          } else {
            m_sendFile.close();
            changeState( 3);
          }
        }    
      }
    break;

    case 7:
      m_responseCode = 200;
      clientWrite( "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nCache-Control: no-cache\r\n\r\n");
      m_timer.setInterval( 10000);
      changeState( 8);
    break;
    case 8:
      if( sendExecReply() || m_timer.hasIntervalElapsed()) {
        endExec();
        changeState( 3);
      }
    break;
    case 9:
      m_responseCode = 200;
      clientWrite( "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nAccess-Control-Allow-Origin: *\r\nCache-Control: no-cache\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><title>Cmd</title></head><body><pre>\r\n");
      m_timer.setInterval( 10000);
      changeState( 10);
    break;
    case 10:
      if( sendExecReply() || m_timer.hasIntervalElapsed()) {
        clientWrite( "\r\n</pre></body></html>");
        endExec();
        changeState( 3);
      }
    break;

    case 20:
      m_requestStart = HW_getMicros();
      changeState( 21);
    break;
    case 21:
      if( m_client == NULL) {
        m_responseCode = 1;
        changeState( 3);
      } else if( m_timer.hasIntervalElapsed( )) {
        m_responseCode = 2;
        changeState( 3);        
      } else if( m_urlIndex >= (sizeof(m_url) - 1) ) {
        m_responseCode = 3;
        changeState( 3);
      } else {
        int nb = clientRead( &m_url[ m_urlIndex], sizeof(m_url) - m_urlIndex -1);
        if( nb > 0) {
          m_urlIndex += nb;
          m_url[ m_urlIndex ] = '\0';
          bool flag = true;
          for( uint16_t i = 0; flag && i < m_urlIndex; i++) {
            if( m_url[ i] == '\r' || m_url[i] == '\n') {
              m_url[ i] = '\0';
              m_client->flush();
              if( strlen( m_url) < 5 ) {
                changeState( 3);
              } else {
                changeState( 2);
              }
              flag = false;
            }
          }
        }
      }
    break;
  }
  unsigned et =  HW_getMicros() - start;
  if( m_log != NULL &&  et > 900) {
    m_log->print( __FILE__, __LINE__, 0x100000, startState, m_state, et, "HttpServer_slice: startState, m_state, time");
  }
}
