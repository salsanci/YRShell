#include "YRShell8266.h"
#include <SPI.h>

#include <TelnetServer.h>

#define INITIAL_LOAD_FILE "/start.macro"

void HServer::exec( const char *p) {
  m_shell->execString( p);
  m_auxBufIndex = 0;
}

void HServer::startExec( ) {
  m_shell->startExec();
}
void HServer::endExec( ) {
  m_shell->endExec();
}

bool HServer::sendExecReply( void) {
  bool rc = true;
  if( m_shell->isAuxQueueInUse() && m_shell->isExec() ) {
    rc = false;
    CircularQBase<char>& q = m_shell->getAuxOutq();
    while( q.valueAvailable()) {
      char c = q.get();
      if( c != '\r' && c != '\n' ) {
        m_auxBuf[ m_auxBufIndex++] = c;    
      }
      if( c == '\r' || c == '\n' ||  m_auxBufIndex > (sizeof(m_auxBuf) - 2 ) ) {
        m_auxBuf[ m_auxBufIndex] = '\0';
        bool flag = true;
        for( const char* p = m_auxBuf; flag && *p != '\0'; p++) {
          if( *p != ' ' && *p != '\r' && *p != '\n' && *p != '\t') {
            flag = false;
          }
        }
        if( !flag && m_log != NULL) {
          m_log->print( __FILE__, __LINE__, 4, m_auxBuf, "HServer_sendExecReply: auxBuf");
        }
        if( c == '\r' || c == '\n' ) {
          m_auxBuf[ m_auxBufIndex++] = c;    
        }
        clientWrite( m_auxBuf, m_auxBufIndex);
        m_auxBufIndex = 0;
      }
    }
  }
  return rc;
}

static const FunctionEntry yr8266ShellExtensionFunctions[] = {
    { SE_CC_setPinIn,             "setPinIn" },
    { SE_CC_setPinIn,             "spi" },
    { SE_CC_setPinInPullup,       "setPinInPullup" },
    { SE_CC_setPinInPullup,       "spip" },
    { SE_CC_setPinOut,            "setPinOut" },
    { SE_CC_setPinOut,            "spo" },
    { SE_CC_setDigitalPin,        "setDigitalPin" },
    { SE_CC_setDigitalPin,        "sdp" },
    { SE_CC_setAnalogPin,         "setAnalogPin" },
    { SE_CC_setAnalogPin,         "sap" },
    { SE_CC_getDigitalPin,        "getDigitalPin" },
    { SE_CC_getAnalogPin,         "getAnalogPin" },
    { SE_CC_ledPush,              "ledPush" },
    { SE_CC_ledPop,               "ledPop" },
    { SE_CC_setLedOnOffMs,        "setLedOnOffMs" },
    { SE_CC_setLogMask,           "setLogMask" },   
    { SE_CC_execDone,             "execDone"},
    { SE_CC_hexModeQ,             "hexMode?"},
    { SE_CC_wifiConnected,        "wifiConnected"},

    { SE_CC_getHostName,          "getHostName" },
    { SE_CC_getHostPassword,      "getHostPassword" },
    { SE_CC_getHostIp,            "getHostIp" },
    { SE_CC_getHostGateway,       "getHostGateway" },
    { SE_CC_getHostMask,          "getHostMask" },
    { SE_CC_getHostMac,           "getHostMac" },
    { SE_CC_isHostActive,         "isHostActive" },

    { SE_CC_getNumberOfNetworks,  "getNumberOfNetworks" },
    { SE_CC_getConnectedNetwork,  "getConnectedNetwork" },
    { SE_CC_getNetworkIp,         "getNetworkIp" },
    { SE_CC_getNetworkMac,        "getNetworkMac" },
    { SE_CC_getNetworkName,       "getNetworkName" },
    { SE_CC_getNetworkPassword,   "getNetworkPassword" },

    { SE_CC_setHostName,          "setHostName" },
    { SE_CC_setHostPassword,      "setHostPassword" },
    { SE_CC_setHostIp,            "setHostIp" },
    { SE_CC_setHostGateway,       "setHostGateway" },
    { SE_CC_setHostMask,          "setHostMask" },
    { SE_CC_setNetworkName,       "setNetworkName" },
    { SE_CC_setNetworkPassword,   "setNetworkPassword" },

    { SE_CC_saveNetworkParameters,"saveNetworkParameters" },

    { SE_CC_loadFile,              "loadFile" },

    { SE_CC_dbgM,                  "logM" },
    { SE_CC_dbgDM,                 "logDM" }, 
    { SE_CC_dbgDDM,                "logDDM" }, 
    { SE_CC_dbgXM,                 "logXM" }, 
    { SE_CC_dbgXXM,                "logXXM" },  

    { SE_CC_hardReset,             "hardReset" },  

    { SE_CC_dotUb,                ".ub" },
    { SE_CC_strToInt,             "strToInt"},
    { SE_CC_textBufferSize,       "textBufferSize" },

    { SE_CC_spiStart,             "spiStart" },
    { SE_CC_spiTransfer,          "spiTransfer" },
    { SE_CC_spiEnd,               "spiEnd" },

    { SE_CC_spiTest,              "spiTest" },

    { SE_CC_connectQ,             "connectQ" },

    { SE_CC_cqDump,               "cqDump" },

    { SE_CC_echoLoop,             "echoLoop"},
  
    { 0, NULL}
};


static FunctionDictionary dictionaryExtensionFunction( yr8266ShellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );

CompiledDictionary compiledExtensionDictionary( NULL, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_EXTENSION_COMPILED);

YRShell8266::~YRShell8266() {
  if( m_httpServer != NULL) {
    delete m_httpServer;
    m_httpServer = NULL;
  }
  if( m_telnetConsoleServer != NULL) {
    delete m_telnetConsoleServer;
    m_telnetConsoleServer = NULL;
  }
  if( m_telnetLogServer != NULL) {
    delete m_telnetLogServer;
    m_telnetLogServer = NULL;
  }
}

void YRShell8266::init( unsigned httpPort, WifiConnection* wifiConnection, LedBlink* led, DebugLog* log) {
  YRShellBase::init();
  m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
  m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &dictionaryExtensionFunction;
  m_wifiConnection = wifiConnection;
  m_led = led;
  m_log = log;
  if( httpPort != 0) {
    m_httpServer = new HServer( this);
    m_httpServer->init( httpPort, m_log, m_led);
  }
  m_exec = false;
  m_initialized = true;
  m_echoLoop = false;
}

void YRShell8266::startExec( void) {
  m_lastPromptEnable = getPromptEnable();
  m_lastCommandEcho = getCommandEcho();
  setPromptEnable( false);
  setCommandEcho( false);
}
void YRShell8266::endExec( void) {
  setPromptEnable( m_lastPromptEnable);
  setCommandEcho( m_lastCommandEcho);
  requestUseMainQueues();
}
void YRShell8266::execString( const char* p) {
  if( m_exec) {
      m_log->print( __FILE__, __LINE__, 1, "YRShell8266_execString_failed: ");
  } else {
    requestUseAuxQueues();
    for( ; *p != '\0'; p++ ) {
      m_AuxInq->put( *p);
    }
    for( const char* p = " cr execDone\r"; *p != '\0'; p++ ) {
      m_AuxInq->put( *p);
    }
    m_exec = true;
    m_execTimer.setInterval( 5000);
  }
}

void YRShell8266::loadFile( const char* fname, bool exec) {
  if( m_fileOpen) {
    if( m_log != NULL) {
      m_log->print( __FILE__, __LINE__, 1, "YRShell8266_loadFile_Failed: ");
    }
  } else {
    if( fname == NULL || fname[0] == '\0') {
      m_log->print( __FILE__, __LINE__, 1, "YRShell8266_loadFile_no_valid_file: ");
    } else {
      m_file = LittleFS.open(fname, "r");
      if( !m_file) {
        if( m_log != NULL) {
          m_log->print( __FILE__, __LINE__, 1, fname, "YRShell8266_loadFile_Failed: fname");
        }
      } else {
        if( exec) {
          requestUseAuxQueues();
        }
        m_fileOpen = true;
        if( m_log != NULL) {
          m_log->print( __FILE__, __LINE__, 1, fname, "YRShell8266_loadFile_loading: fname");
        }
      }
    }
  }
}

void YRShell8266::slice() {
  if( m_echoLoop) {
    if( m_Inq->valueAvailable()) {
      if( m_Outq->spaceAvailable()) {
        char c = m_Inq->get();
        if( c == '\x1B') {
          m_echoLoop = false;
        } else {
          m_Outq->put( c);
        }
      }
    }
  } else {
    YRShellBase::slice();
    if( m_fileOpen && m_auxInq.spaceAvailable(10)) {
      int c = m_file.read();
      if( c != -1) {
        m_auxInq.put( c);   
      } else {
        m_file.close();
        m_fileOpen = false;
        m_log->print( __FILE__, __LINE__, 1, "YRShell8266_slice_closing_file");
      } 
    }

    if( m_exec && m_execTimer.hasIntervalElapsed()) {
      m_exec = false;
    }
    
    if( m_useAuxQueues && !m_exec ) {
      while( m_AuxOutq->valueAvailable()) {
        char c = m_AuxOutq->get();
        if( c != '\r' && c != '\n' ) {
          m_auxBuf[ m_auxBufIndex++] = c;    
        }
        if( c == '\r' || c == '\n' ||  m_auxBufIndex > (sizeof(m_auxBuf) - 2 ) ) {
          m_auxBuf[ m_auxBufIndex] = '\0';
          bool flag = true;
          for( const char* p = m_auxBuf; flag && *p != '\0'; p++) {
            if( *p != ' ' && *p != '\r' && *p != '\n' && *p != '\t') {
              flag = false;
            }
          }
          if( !flag && m_log != NULL) {
            m_log->print( __FILE__, __LINE__, 8, m_auxBuf, "YRShell8266_slice: auxBuf");
          }
          m_auxBufIndex = 0;
        }
      }
    } else if( m_auxBufIndex > 0) {
      if( m_log != NULL) {
        m_log->print( __FILE__, __LINE__, 8, m_auxBuf, "YRShell8266_slice: auxBuf");
      }
      m_auxBufIndex = 0;
    }

    if( !m_initialFileLoaded && m_initialized && isIdle() ) {
        m_initialFileLoaded = true;
        loadFile( INITIAL_LOAD_FILE);
    }
  }
} 

void YRShell8266::executeFunction( uint16_t n) {
  uint32_t t1, t2, t3;
  CircularQBase<char> *q1, *q2;
  
  if( n <= SE_CC_first || n >= SE_CC_last) {
      YRShellBase::executeFunction(n);
  } else {
      switch( n) {
          case SE_CC_setPinIn:
              pinMode(popParameterStack(), INPUT);
              break;
          case SE_CC_setPinInPullup:
              pinMode(popParameterStack(), INPUT_PULLUP);
              break;
          case SE_CC_setPinOut:
              pinMode(popParameterStack(), OUTPUT);
              break;
          case SE_CC_setDigitalPin:
              t1 = popParameterStack();
              t2 = popParameterStack();
              digitalWrite( t1, t2);
              break;
          case SE_CC_setAnalogPin:
              t1 = popParameterStack();
              t2 = popParameterStack();
              analogWrite( t1, t2);
              break;
          case SE_CC_getDigitalPin:
              pushParameterStack( digitalRead(popParameterStack()));
              break;
          case SE_CC_getAnalogPin:
              pushParameterStack( analogRead(A0));
              break;
          case SE_CC_ledPush:
              if( m_led) {
                m_led->push();
              }
          break;
          case SE_CC_ledPop:
              if( m_led) {
                m_led->pop();
              }
          break;
          case SE_CC_setLedOnOffMs:
              t1 = popParameterStack();
              t2 = popParameterStack();
              if( m_led) {
                m_led->setLedOnOffMs( t2, t1);
              }
              break;
          case SE_CC_execDone:
              m_exec = false;
              break;
          case SE_CC_setLogMask:
              t1 = popParameterStack();
              if( m_log != NULL) {
                m_log->setMask( t1);
              }
              break;
          case SE_CC_hexModeQ:
              pushParameterStack( m_hexMode);
              break;
          case SE_CC_wifiConnected:
              pushParameterStack(  WiFi.status() == WL_CONNECTED);
              break;

          case SE_CC_getHostName:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getHostName());
              }
              pushParameterStack( 0);
            break;
          case SE_CC_getHostPassword:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getHostPassword());
              }
              pushParameterStack( 0);
            break;
          case SE_CC_getHostIp:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getHostIp());
              }
              pushParameterStack( 0);
              break;
          case SE_CC_getHostGateway:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getHostGateway());
              }
              pushParameterStack( 0);
              break;
          case SE_CC_getHostMask:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getHostMask());
              }
              pushParameterStack( 0);
              break;
          case SE_CC_getHostMac:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                m_wifiConnection->networkParameters.getHostMac(m_textBuffer);
              }
              pushParameterStack( 0);
              break;

          case SE_CC_isHostActive:
              if( m_wifiConnection) {
                pushParameterStack( m_wifiConnection->isHostActive());
              } else {
                pushParameterStack( 0);
              }
              break;
              break;

          case SE_CC_getNumberOfNetworks:
              if( m_wifiConnection) {
                pushParameterStack( m_wifiConnection->networkParameters.getNumberOfNetworks());
              } else {
                pushParameterStack( 0);
              }
              break;
          case SE_CC_getConnectedNetwork:
              if( m_wifiConnection) {
                pushParameterStack( m_wifiConnection->getConnectedNetworkIndex());
              } else {
                pushParameterStack( -1);
              }
              break;
          case SE_CC_getNetworkIp:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getNetworkIp( ));
              }
              pushParameterStack( 0);
              break;
          case SE_CC_getNetworkMac:
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                m_wifiConnection->networkParameters.getNetworkMac( m_textBuffer );
              }
              pushParameterStack( 0);
              break;
          case SE_CC_getNetworkName:
              t1 = popParameterStack();
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getNetworkName( t1));
              }
              pushParameterStack( 0);
              break;
          case SE_CC_getNetworkPassword:
              t1 = popParameterStack();
              m_textBuffer[ 0] = '\0';
              if( m_wifiConnection) {
                strcpy( m_textBuffer, m_wifiConnection->networkParameters.getNetworkPassword( t1));
              }
              pushParameterStack( 0);
              break;
          case SE_CC_setHostName:
              t1 = popParameterStack();
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.setHostName( getAddressFromToken( t1));
              }
              break;
          case SE_CC_setHostPassword:     
              t1 = popParameterStack();
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.setHostPassword( getAddressFromToken( t1));
              }
              break;
          case SE_CC_setHostIp:          
              t1 = popParameterStack();
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.setHostIp( getAddressFromToken( t1));
              }
              break;
          case SE_CC_setHostGateway:     
              t1 = popParameterStack();
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.setHostGateway( getAddressFromToken( t1));
              }
              break;
          case SE_CC_setHostMask:        
              t1 = popParameterStack();
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.setHostMask( getAddressFromToken( t1));
              }
              break;
          case SE_CC_setNetworkName:     
              t1 = popParameterStack();
              t2 = popParameterStack();
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.setNetworkName( t1, getAddressFromToken( t2));
              }
              break;
          case SE_CC_setNetworkPassword: 
              t1 = popParameterStack();
              t2 = popParameterStack();
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.setNetworkPassword( t1, getAddressFromToken( t2));
              }
              break;
          case SE_CC_saveNetworkParameters: 
              if( m_wifiConnection) {
                  m_wifiConnection->networkParameters.save();
              }
              break;

          case SE_CC_loadFile:
              loadFile( getAddressFromToken(popParameterStack()), false );
              break;
          case SE_CC_dbgM:
              if( m_log) {
                m_log->print( __FILE__, __LINE__, 1, getAddressFromToken(popParameterStack()) );
              }
              break;
          case SE_CC_dbgDM:
              t1 = popParameterStack();
              if( m_log) {
                m_log->print( __FILE__, __LINE__, 1, t1, getAddressFromToken(popParameterStack()) );
              }
              break;
          case SE_CC_dbgDDM:
              t1 = popParameterStack();
              t2 = popParameterStack();
              if( m_log) {
                m_log->print( __FILE__, __LINE__, 1, t2, t1,  getAddressFromToken(popParameterStack()) );
              }
              break;
          case SE_CC_dbgXM:
              t1 = popParameterStack();
              if( m_log) {
                m_log->printX( __FILE__, __LINE__, 1, t1, getAddressFromToken(popParameterStack()) );
              }
              break;
          case SE_CC_dbgXXM:
              t1 = popParameterStack();
              t2 = popParameterStack();
              if( m_log) {
                m_log->printX( __FILE__, __LINE__, 1, t2, t1,  getAddressFromToken(popParameterStack()) );
              }
              break;

          case SE_CC_hardReset:
              LittleFS.remove( "/NetworkParameters");
              ESP.restart();
              break;

          case SE_CC_dotUb:
              outUint8( popParameterStack());
              break;

          case SE_CC_textBufferSize:
              pushParameterStack( m_textBufferSize);
          break;

          case SE_CC_strToInt:
              if( m_textBuffer[0] == '0' && m_textBuffer[ 1] == 'x') {
                stringToUnsignedX( m_textBuffer, &t1 );
              } else {
                stringToUnsigned( m_textBuffer, &t1 );
              }
              pushParameterStack( t1);
              break;

          case SE_CC_spiStart:
              t1 = popParameterStack(); // spiMode
              t2 = popParameterStack(); // LSBFIRST
              t3 = popParameterStack(); // CLOCK
              SPI.begin();
              SPI.beginTransaction( SPISettings(t3, t2 ? LSBFIRST : MSBFIRST, t1));
              break;

          case SE_CC_spiTransfer:
              memset( m_textBuffer, 0xC5, m_textBufferSize);
              t1 = popParameterStack(); // nBytes
              SPI.transfer( m_textBuffer, t1);
              break;

          case SE_CC_spiEnd:
              SPI.endTransaction();
              SPI.end();
              break;

          case SE_CC_spiTest:
              digitalWrite( 15, 0);
              m_textBuffer[ 0] = 0xc5;
              m_textBuffer[ 1] = 0xc5;
              SPI.transfer( m_textBuffer, 2);
              digitalWrite( 15, 1);
              m_textBuffer[ 0] = 0xc5;
              m_textBuffer[ 1] = 0xc5;
              SPI.transfer( m_textBuffer, 2);
              break;

          case SE_CC_connectQ:
              t1 = popParameterStack();
              t2 = popParameterStack();
              t3 = CharQ::connect(getAddressFromToken( t2), getAddressFromToken( t1));
              pushParameterStack( t3);
              break;

          case SE_CC_cqDump:
              m_log->print( __FILE__, __LINE__, -1, "ENTER" );
              CQitem::dump();
              m_log->print( __FILE__, __LINE__, -1, "EXIT" );
              break;

          case SE_CC_echoLoop:
              m_echoLoop = true;
              break;

          default:
              shellERROR(__FILE__, __LINE__);
              break;
      }
  }
}
void YRShell8266::outUInt8( int8_t v) {
    char buf[ 5];
    unsignedToString(v, 3, buf);
    outString( buf);
}
