#include "DebugLog.h"
#include "HiResTimer.h"

void DebugLog::printHexLine( const char* P, int len) {
    int i, j;
    memset(c_buf, ' ', sizeof(c_buf)-1);
    c_buf[ sizeof( c_buf) - 1] = '\0';
    for( j = i = 0; i < 16; P++, i++ ) {
        if( i >= len) {
            print( "   ");
        } else {
            c_buf[ j++] = *P > 0x20 && *P < 0x7E ? *P : '.';
            outX(*P, 2);
            print( " ");
        }
        if( i == 7) {
            print( "  ");
            c_buf[ j++] = ' ';
            c_buf[ j++] = ' ';
        } else if( i == 3 || i == 11) {
            print( " ");
            c_buf[ j++] = ' ';
        }
    }
    print( c_buf);      
    print( "\r\n");
}
  
void DebugLog::flush( ) {
    for( uint16_t i = 1; i && valueAvailable(); i++ ) {
    }
}  
void DebugLog::out( const char c) {
    if( m_dq.spaceAvailable( 24)) {
      m_dq.put( c);
    } else {
      m_dq.reset();
      out("\r\n\nLOG DATA DROPPED\r\n\n");
    }
}
void DebugLog::out( const char* s) {
    while( *s != '\0') {
      out( *s++);
    }
}
void DebugLog::out( uint32_t v, uint32_t n) {
    YRShellInterpreter::unsignedToString( v, n, m_buf);
    out( m_buf);
}
void DebugLog::outX( uint32_t v, uint32_t n) {
    YRShellInterpreter::unsignedToStringX( v, n, m_buf);
    out( m_buf);
  }
void DebugLog::outPaddedStr( const char* p, uint32_t len) {
    size_t sz = strlen( p);
    if( sz > len) {
      out( p + sz - len);
    }  else {
      len -= sz;
      while( len-- > 0) {
        out( ' ');
      }
      out( p);
    }  
}
void DebugLog::printh( const char* file, uint32_t line) {
    uint32_t t =  HiResTimer::getMillis();
    out( t);
    out( ' ');
    out( t - m_lastTime);
    out( ' ');
    m_lastTime = t;
    outPaddedStr( file, 20);
    out( ' ');
    out( line, 4);
    out( ' ');
}
void DebugLog::printu( uint32_t v) {
    out( v);
    out( ' ');
}
void DebugLog::printx( uint32_t v) {
    outX( v);
    out( ' ');
}
void DebugLog::printm( const char* m) {
    prints( m);
    out( "\r\n");
}
  void DebugLog::prints( const char* m) {
    out( '"');
    out( m);
    out( '"');
}
 
void DebugLog::print( const char* m) {
    out( m);
}
void DebugLog::print( const char* file, uint32_t line, uint32_t mask, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printm( message);
    }
}
void DebugLog::print( const char* file, uint32_t line, uint32_t mask, const char* m, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        prints( m);
        out( ' ');
        printm( message);
    }
}
void DebugLog::print( const char* file, uint32_t line, uint32_t mask, const char* m, const char* m1, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        prints( m);
        out( ' ');
        prints( m1);
        out( ' ');
        printm( message);
    }
}
void DebugLog::print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* m, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printu( v1);
        prints( m);
        out( ' ');
        printm( message);
    }
}
void DebugLog::print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* m, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printu( v1);
        printu( v2);
        prints( m);
        out( ' ');
        printm( message);
    }
}
void DebugLog::printLog( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* m) {
    if( m_mask & mask) {
        printh( file, line);
        printu( v1);
        printu( v2);
        outPaddedStr( m, 64);
        out( "\r\n");
    }
}

void DebugLog::print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printu(v1);
        printm( message);
    }
}
void DebugLog::print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printu(v1);
        printu(v2);
        printm( message);
    }
}
void DebugLog::print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, uint32_t v3, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printu(v1);
        printu(v2);
        printu(v3);
        printm( message);
    }
}

void DebugLog::printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* m, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printx( v1);
        print( m);
        out( ' ');
        printm( message);
    }
}
void DebugLog::printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* m, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printx( v1);
        printx( v2);
        print( m);
        out( ' ');
        printm( message);
    }
}

void DebugLog::printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printx(v1);
        printm( message);
    }
}
void DebugLog::printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printx(v1);
        printx(v2);
        printm( message);
    }
}
void DebugLog::printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, uint32_t v3, const char* message) {
    if( m_mask & mask) {
        printh( file, line);
        printx(v1);
        printx(v2);
        printx(v3);
        printm( message);
    }
}

void DebugLog::printHex( const char* P, int len) {
    for(  ; len > 0; P += 16, len -= 16) {
      printHexLine( P, len);
    }
}
