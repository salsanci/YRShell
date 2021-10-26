#include "Logger.h"
#include <math.h>

Logger* Logger::s_logger = NULL;

Logger::Logger() {
	m_count = 0;
	m_loggingMask = 1;
	m_timer.setInterval(LOGGER_TIMEOUT);
	if( s_logger == NULL) {
		s_logger = this;
	}
	m_loggerPrefix = "";
}
Logger::~Logger() {
}

void Logger::slice() {
	if(  m_timer.hasIntervalElapsed()) {
		if( m_count != 0 ) {
			outputBuf();
		}
		m_timer.setInterval(LOGGER_TIMEOUT);
	}
}

void Logger::appendDebug( const char* S) {
	if( (strlen(m_debugBuffer) + strlen(S)) < (sizeof(m_debugBuffer) - 1) ) {
		strcat( m_debugBuffer, S);
	}
}
void Logger::setDebug( const char* S) {
	memset( m_debugBuffer, 0, sizeof(m_debugBuffer));
	appendDebug( S);
}


void Logger::putLoggerUint32( const uint v) {
	char t[12];
	unsignedToString(v, 10, t);
	putString( t);
}
void Logger::putLoggerUint32X( const uint v) {
	char t[12];
	unsignedToStringX(v, 8, t);
	putString( t);
}
void Logger::putLoggerUint8X( const uint8_t v) {
	char t[12];
	unsignedToStringX(v, 2, t);
	putString( t);
}
void Logger::putLoggerUint16X( const uint16_t v) {
	char t[12];
	unsignedToStringX(v, 4, t);
	putString( t);
}
void Logger::putLoggerFloat( float v) {
	char suf[5];
	float mantissa, tmp;
	uint integerPart, fractionalPart;
	int8_t exponent, expRemainder;
	char t[10];
	char *P;
	bool neg = false;
	mantissa = 0.0f;
	
	
	if( !finitef( v)) {
		putString( "  NOT_FINITEF   ");
	} else {
		suf[ 0] = ' ';
		suf[ 1] = ' ';
		suf[ 2] = ' ';
		suf[ 3] = ' ';
		suf[ 4] = '\0';

		if( v < 0 ) {
			v = -v;
			neg = true;
		}
		if( v > 0.0f) {
			exponent = (int8_t) log10f( v);
			mantissa = v * powf( 10, ((float) (0 - exponent)) );
			expRemainder = exponent % 3;

			switch( expRemainder) {
			case -2:
				mantissa *= 10.0f;
				exponent -= 1;
			break;
			case -1:
				mantissa *= 100.0f;
				exponent -= 2;
			break;
			case 1:
				mantissa *= 10.0f;
				exponent -= 1;
			break;
			case 2:
				mantissa *= 100.0f;
				exponent -= 2;
			break;
			}
			if( exponent < 0) {
				suf[1] = '-';
				exponent = 0 - exponent;
			} else if( exponent > 0) {
				suf[1] = '+';
			}
			if(exponent > 0) {
				suf[3] = '0' + (exponent % 10);
				exponent /= 10;
				suf[2] = '0' + (exponent % 10);
				suf[0] = 'E';
			}
		}
		integerPart = (uint) mantissa;
		tmp =  (mantissa - ((float) integerPart));
		fractionalPart = (uint) (tmp * 1e6f);
		P = &t[5];
		*P-- = '\0';
		while( P >= t) {
			*P-- = '0' + (integerPart % 10);
			integerPart /= 10;
			if( integerPart == 0) {
				break;
			}
		}
		if( neg && P > t) {
			*P-- = '-';
		}
		while( P >= t) {
			*P-- = ' ';
		}
		putString( t);
		putString( ".");
		P = &t[6];
		*P-- = '\0';
		while( P >= t) {
			*P-- = '0' + (fractionalPart % 10);
			fractionalPart /= 10;
		}
		putString(t);
		putString( suf);
	}
}

void Logger::putChar( char c) {
	if( ((sizeof( m_buf) -1) - m_count) < 1) {
		outputBuf();
	}
	m_buf[ m_count++] = c;
	m_timer.setInterval(LOGGER_TIMEOUT);
}
void Logger::putString( const char* s) {
	uint16_t i, l = (uint16_t) strlen( s);
	if( l > 0) {
		const char* truncMessage = " LOGGER STRING TRUCATED\n";
		uint16_t lim = (sizeof( m_buf) - 1) - strlen(truncMessage);
		bool trunc = false;
		bool eol = false;
		char c;

		if( l > ((sizeof( m_buf) -1) - m_count) ) {
			outputBuf();
		}
		if( l > lim) {
			outputBuf();
			l = lim;
			trunc = true;
		}
		for( i = 0; i < l && m_count < (sizeof( m_buf) -1); i++, m_count++, s++) {
			c = *s;
			if( c == '\n') {
				eol = true;
			}
			m_buf[ m_count] = c;
		}
		if( trunc) {
			eol = true;
			for( ; *truncMessage != '\0'; truncMessage++ ) {
				c = *truncMessage;
				m_buf[ m_count] = c;
			}
		}
		if( eol) {
			outputBuf();
		}
		m_timer.setInterval(LOGGER_TIMEOUT);
	}
}
void Logger::dataDropped( CircularCharQ& q) {
	const char* s = "LOGGER DATA DROPPED\n";
	q.reset();
	while( *s != '\0') {
		q.put( *s++);
	}
}
void Logger::outputBuf() {
	if( m_loggerQ.spaceAvailable( m_count) ) {
		for( uint16_t i = 0; i < m_count; i++) {
			m_loggerQ.put( m_buf[i]);
		}
	} else {
		dataDropped(m_loggerQ);
	}
	m_count = 0;
	m_timer.setInterval(LOGGER_TIMEOUT);
}

void Logger::unsignedToStringZero(uint num, uint8_t numDigits, char *s) {
    char *P = s + numDigits;
    *P-- = '\0';
    while( P >= s) {
        *P-- = '0' + (num % 10);
        num /= 10;
    }
}
void Logger::unsignedToString(uint num, uint8_t numDigits, char *s) {
    unsignedToStringZero( num, numDigits, s);
    while( *s == '0' && numDigits-- > 1) {
        *s++ = ' ';
    }
}

void Logger::unsignedToStringX(uint num, uint8_t numDigits, char *s){
    *s++ = '0';
    *s++ = 'x';
    char *P = s + numDigits;
    char c;
    *P-- = '\0';
    while( P >= s) {
        c = num & 0xF;
        if( c > 9) {
            c += '7';
        } else {
            c += '0';
        }
        *P-- =  c;
        num >>= 4;
    }
}

const char* Logger::trimFileName( const char *P) {
	const char* N = P + strlen(P) - 1;
	while( N-- > P ) {
		if( *N == '/' || *N == '\\') {
			N++;
			break;
		}
	}
	return N;
}


void  Logger::__kwh( const char* file, uint line, uint ID) {
	static uint lastDebugTick;
	uint tick;
	char t[12];
//                              1         2         3
//                     1234567890123456789012345678901
	const char *pad = "                               ";
	const char *S;
	
	

	S = trimFileName(file);
	int ln = strlen(S);
	putString( m_loggerPrefix);

	unsignedToStringZero(Logger::m_seq++, 4, t);
	putString( t);
	putString( " MSG");
	unsignedToStringZero(ID, 4, t);
	putString( t);
	putString( " ");
	putString( S);
	ln++;
	putString( ":");
	unsignedToString(line, 10, t);
	for( S = t; *S == ' '; S++){ }
	ln += strlen(S);
	putString( S);
	if( ln < 31) {
		putString( pad+ln);
	}
	tick = HAL_GetTick();
	putLoggerUint32( tick);
	putString( ":");
	unsignedToString( tick - lastDebugTick, 10, t);
	for( S = t; *S == ' '; S++){ }
	ln = strlen(S);
	putString( S);
	putString( pad+21+ln);
	lastDebugTick = tick;
	putString( pad+21);
	putString("\"");
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint data) {
	
	
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( data);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, const char* m1, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putString( m1);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, const char* m1, const char* m2, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putString( m1);
		putChar( ' ');
		putString( m2);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, uint d4, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putLoggerUint32( d4);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, uint d4, uint d5, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putLoggerUint32( d4);
		putChar( ' ');
		putLoggerUint32( d5);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, uint d4, uint d5, uint d6, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putLoggerUint32( d4);
		putChar( ' ');
		putLoggerUint32( d5);
		putChar( ' ');
		putLoggerUint32( d6);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, uint d4, uint d5, uint d6, uint d7, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putLoggerUint32( d4);
		putChar( ' ');
		putLoggerUint32( d5);
		putChar( ' ');
		putLoggerUint32( d6);
		putChar( ' ');
		putLoggerUint32( d7);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, uint d4, uint d5, uint d6, uint d7, uint d8, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putLoggerUint32( d4);
		putChar( ' ');
		putLoggerUint32( d5);
		putChar( ' ');
		putLoggerUint32( d6);
		putChar( ' ');
		putLoggerUint32( d7);
		putChar( ' ');
		putLoggerUint32( d8);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0, const char* m1, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putString( m1);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0, uint d1, const char* m1, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putString( m1);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, const char* m1, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putString( m1);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, uint d0,  uint d1, uint d2, uint d3, uint d4, const char* m1, const char* msg)  {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( d0);
		putChar( ' ');
		putLoggerUint32( d1);
		putChar( ' ');
		putLoggerUint32( d2);
		putChar( ' ');
		putLoggerUint32( d3);
		putChar( ' ');
		putLoggerUint32( d4);
		putChar( ' ');
		putString( m1);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, float f3, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putLoggerFloat( f3);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, float f3, float f4, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putLoggerFloat( f3);
		putChar( ' ');
		putLoggerFloat( f4);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, float f3, float f4, float f5, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putLoggerFloat( f3);
		putChar( ' ');
		putLoggerFloat( f4);
		putChar( ' ');
		putLoggerFloat( f5);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, float f3, float f4, float f5, float f6, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putLoggerFloat( f3);
		putChar( ' ');
		putLoggerFloat( f4);
		putChar( ' ');
		putLoggerFloat( f5);
		putChar( ' ');
		putLoggerFloat( f6);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putLoggerFloat( f3);
		putChar( ' ');
		putLoggerFloat( f4);
		putChar( ' ');
		putLoggerFloat( f5);
		putChar( ' ');
		putLoggerFloat( f6);
		putChar( ' ');
		putLoggerFloat( f7);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}
void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putLoggerFloat( f3);
		putChar( ' ');
		putLoggerFloat( f4);
		putChar( ' ');
		putLoggerFloat( f5);
		putChar( ' ');
		putLoggerFloat( f6);
		putChar( ' ');
		putLoggerFloat( f7);
		putChar( ' ');
		putLoggerFloat( f8);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8, float f9, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerFloat( f0);
		putChar( ' ');
		putLoggerFloat( f1);
		putChar( ' ');
		putLoggerFloat( f2);
		putChar( ' ');
		putLoggerFloat( f3);
		putChar( ' ');
		putLoggerFloat( f4);
		putChar( ' ');
		putLoggerFloat( f5);
		putChar( ' ');
		putLoggerFloat( f6);
		putChar( ' ');
		putLoggerFloat( f7);
		putChar( ' ');
		putLoggerFloat( f8);
		putChar( ' ');
		putLoggerFloat( f9);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

void Logger::kwh( const char* file, uint line, uint ID, uint mask, bool d0, const char* msg) {
	if( m_loggingMask & mask) {
		__kwh( file, line, ID);
		putLoggerUint32( (uint) d0);
		putChar( ' ');
		putString( msg);
		putString( "\"\n");
	}
}

