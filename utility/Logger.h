#ifndef LOGGER_H_
#define LOGGER_H_
#include <stdint.h>

#include "YRShellInterpreter.h"

#define LOGGER_TIMEOUT 500
#define LOGGER_QUEUE_SIZE 60000

class Logger;

class Logger : public Sliceable {
private:
	static Logger *s_logger;
	uint	m_seq, m_count, m_loggingMask;
	char 		m_buf[ 256];

	CircularQ<char, LOGGER_QUEUE_SIZE> m_loggerQ;
	IntervalTimer m_timer;

	char m_debugBuffer[ 256];
	char m_lastFatalError[ 256];
	uint m_uniqueId;

	void outputBuf( void);
	void dataDropped( CircularCharQ& q);

	void appendDebug( const char*);
	void setDebug( const char*);
    void putChar( char c);
    void putString( const char *s);
    const char* trimFileName( const char *P);

	void putLoggerString( const char* P);
	void putLoggerUint32( const uint v);
	void putLoggerUint32X( const uint v);
	void putLoggerUint8X( const uint8_t v);
	void putLoggerUint16X( const uint16_t v);
	void putLoggerFloat( const float data);

    void unsignedToStringZero(uint num, uint8_t numDigits, char *s);
    void unsignedToString(uint num, uint8_t numDigits, char *s);
    void unsignedToStringX(uint num, uint8_t numDigits, char *s);

	void  __kwh( const char* file, uint line, uint ID);

	const char* m_loggerPrefix;

public:
	Logger( void);
	virtual ~Logger( void);
	virtual void slice( void);
    virtual const char* sliceName( void) { return "logger"; }

	static Logger* logger( void) { return s_logger; }

	uint getLoggingMask( void) { return m_loggingMask; }
	void setLoggingMask( uint mask) { m_loggingMask = mask; }

    CircularCharQ& getLoggerQ( ) { return m_loggerQ; }

	void setLoggerPrefix( const char* p) { m_loggerPrefix = p; }

	void kwh( const char* file, uint line, uint ID, uint mask, uint data);
	void kwh( const char* file, uint line, uint ID, uint mask, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, const char* m1, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, const char* m1, const char* m2, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, uint d4, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, uint d4, uint d5, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, uint d4, uint d5, uint d6, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, uint d4, uint d5, uint d6, uint d7, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, uint d4, uint d5, uint d6, uint d7,  uint d8, const char* msg);

	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, const char* m1, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, const char* m1, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, const char* m1, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, uint d2, uint d3, uint d4, const char* m1, const char* msg);

	void kwh( const char* file, uint line, uint ID, uint mask, float d0, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, float d3, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, float d3, float d4, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, float d3, float d4, float d5, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, float d3, float d4, float d5, float d6, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8, const char* msg);
	void kwh( const char* file, uint line, uint ID, uint mask, float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7, float d8, float d9, const char* msg);

	void kwh( const char* file, uint line, uint ID, uint mask, bool d0, const char* msg);

};

#endif
