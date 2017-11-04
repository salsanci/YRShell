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
	uint16_t s1, s2;
	if( m_nextQ != NULL) {
		s1 = m_nextQ->getLinearWriteBufferSize();
		s2 = m_hs->available();
		s1 = s1 <= s2 ? s1 : s2;
		if( s1 > 0) {
			m_hs->readBytes( m_nextQ->getLinearWriteBuffer(), s1);
			m_nextQ->append( s1);
		}
	}
	if( m_previousQ != NULL) {
#ifdef ARDUINO_SAM_DUE
		if( m_previousQ->used() > 0) {
			m_hs->write( m_previousQ->get());
		}
#else
		s1 = m_previousQ->getLinearReadBufferSize();
		s2 = m_hs->availableForWrite();
		s1 = s1 <= s2 ? s1 : s2;
		if( s1 > 0) {
			m_hs->write( m_previousQ->getLinearReadBuffer(), s1);
			m_previousQ->drop( s1);
		}
#endif
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
