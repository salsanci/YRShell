#include "BufferedSerial.h"
#include "YRShellInterpreter.h"
#ifdef PLATFORM_ARDUINO
BufferedSerial::BufferedSerial( HardwareSerial* hs) {
	m_hs = hs; 
}
void BufferedSerial::init(  CircularQBase<char>& nq, CircularQBase<char>& pq) {
	m_nextQ = &nq;
	m_previousQ = &pq;
}
void BufferedSerial::slice( void) {
	int c;
	if( m_nextQ != NULL) {
		while( m_nextQ->spaceAvailable()) {
			c = m_hs->read();
			if( c == -1) {
				break;
			}
			m_nextQ->put( c);
		}
	}	
	if( m_previousQ != NULL) {
		while( m_previousQ->valueAvailable() && m_hs->availableForWrite() > 0) {
			m_hs->write( m_previousQ->get());
		}
	}
}
void BufferedSerial::begin( uint32_t baud) {
	m_hs->begin( baud);
}
void BufferedSerial::end( void) {
	m_hs->end( );
}
void BufferedSerial::setBaud( uint32_t baud) {
	m_hs->end();
	m_hs->begin( baud);
}

BufferedSerial BSerial( &Serial);  
#ifdef ENABLE_SERIAL1
BufferedSerial BSerial1( &Serial1);  
#endif
#ifdef ENABLE_SERIAL2
BufferedSerial BSerial2( &Serial2);  
#endif
#ifdef ENABLE_SERIAL3
BufferedSerial BSerial3( &Serial3);  
#endif

#endif
