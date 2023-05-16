#ifndef DebugLog_h
#define DebugLog_h

#ifndef DEBUG_LOG_BUFFER_SIZE
#define DEBUG_LOG_BUFFER_SIZE 8192
#endif

#include "YRShellInterpreter.h"

class DebugLog {
protected:
  char m_buf[24];
  char c_buf[24];
  uint32_t m_lastTime;
  uint32_t m_mask;
  CircularQ<char, DEBUG_LOG_BUFFER_SIZE> m_dq;  void printHexLine( const char* P, int len);
  void out( const char c);
  void out( const char* s);
  void out( uint32_t v, uint32_t n = 10);
  void outX( uint32_t v, uint32_t n = 8);
  void outPaddedStr( const char* p, uint32_t len);
  void printh( const char* file, uint32_t line);
  void printu( uint32_t v);
  void printx( uint32_t v);
  void printm( const char* m);
  void prints( const char* m);
  
public:
  DebugLog( ) { m_mask = 1; }
  void setMask( uint32_t m) { m_mask = m; }
  bool valueAvailable( uint16_t n = 1) { return m_dq.valueAvailable( n); }
  char get( void) { return m_dq.get(); }
  void flush( void);
  void print( const char* m);
  void print( const char* file, uint32_t line, uint32_t mask, const char* message);
  
  void print( const char* file, uint32_t line, uint32_t mask, const char* m, const char* message);
  void print( const char* file, uint32_t line, uint32_t mask, const char* m,  const char* m1, const char* message);
  
  void print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* m, const char* message);
  void print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* m, const char* message);
  void printLog( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* m);

  void print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* message);
  void print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* message);
  void print( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, uint32_t v3, const char* message);

  void printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* m, const char* message);
  void printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* m, const char* message);

  void printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, const char* message);
  void printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, const char* message);
  void printX( const char* file, uint32_t line, uint32_t mask, uint32_t v1, uint32_t v2, uint32_t v3, const char* message);
  
  void printHex( const char* P, int len);
  #ifdef PLATFORM_ARDUINO
  void printHex( String &s);
  #endif
};

#endif