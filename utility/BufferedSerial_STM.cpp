#include "BufferedSerial_STM.h"
#include "YRShellInterpreter.h"
#ifdef PLATFORM_AC6
BufferedSerial::BufferedSerial( UART_HandleTypeDef* handler) :
	m_phandler(handler),
	m_nextQ(),
	m_previousQ(),
	m_initialized(false),
	m_bytesTx(0)
{

}
uint8_t BufferedSerial::initUart() {
	if(m_phandler == NULL) return 0x01;
	HAL_StatusTypeDef handlerStatus;

	if( m_phandler->hdmarx) {
		handlerStatus = HAL_UART_Receive_DMA( m_phandler, (unsigned char*)m_nextQ.getBuffer(), m_nextQ.size());
		if( HAL_OK != handlerStatus) {
			return (uint8_t)handlerStatus;
		}
	} else {
		handlerStatus = HAL_UART_Receive_IT( m_phandler, (unsigned char*)m_nextQ.getLinearWriteBuffer(), 1);
		if( HAL_OK != handlerStatus) {
			return (uint8_t)handlerStatus;
		}
	}
	return 0x00;
}
void BufferedSerial::init(  CircularQBase<char>& nq, CircularQBase<char>& pq) {
	m_nextQ.setNextQ( nq);
	m_previousQ.setPreviousQ( pq);
}
void BufferedSerial::slice( void) {
	m_nextQ.slice();
	m_previousQ.slice();
}
// Call from interrupt context
void BufferedSerial::sliceInterrupt(void) {
	if( m_phandler->hdmarx) {
		processRX();
	}
	processTX();
}
uint8_t BufferedSerial::begin(uint32_t baud) {
	uint8_t result = 0x00;
	if(m_initialized) {
		result = setBaud(baud);
	} else {
		m_phandler->Init.BaudRate = baud;
		HAL_UART_Init(m_phandler);
		result = initUart();
		m_initialized = true;
	}
	return result;
}
void BufferedSerial::end( void) {
	if( m_initialized) {
		HAL_UART_DeInit(m_phandler);
		m_initialized = false;
	}
}
uint8_t BufferedSerial::setBaud(uint32_t baud) {
	end();
	return begin(baud);
}

void BufferedSerial::processRX(void) {
	if(m_phandler == NULL) return;
	uint16_t used;
	uint16_t prevUsed;
	uint16_t availableQ;
	uint16_t bytesInDMA;
	availableQ = m_nextQ.size();
	prevUsed = m_nextQ.used();

	if(m_phandler->hdmarx) {
		bytesInDMA = __HAL_DMA_GET_COUNTER( m_phandler->hdmarx);
		m_nextQ.setHead(availableQ - bytesInDMA);
	} else {
		return;
	}
	used = m_nextQ.used();
	// Catch overflow condition
	if(used < prevUsed) {
		m_nextQ.reset();
	}
}
void BufferedSerial::processTX( void) {
	if(m_phandler == NULL) return;
	if(m_phandler->gState != HAL_UART_STATE_READY) return;

	if(m_bytesTx > 0) {
		m_previousQ.drop(m_bytesTx);
		m_bytesTx = 0;
	}
	m_bytesTx = m_previousQ.getLinearReadBufferSize();
	if(m_bytesTx > 0) {
		m_bytesTx = m_bytesTx > SERIAL_TX_CHUNK_SIZE ? SERIAL_TX_CHUNK_SIZE : m_bytesTx;
		if( m_phandler->hdmatx) {
			// TODO: Add error handling?
			HAL_UART_Transmit_DMA( m_phandler, (unsigned char*)m_previousQ.getLinearReadBuffer(), m_bytesTx);
		}
	}
}

#ifdef ENABLE_SERIAL1
BufferedSerial BSerial1(usartHandler(1));
#endif
#ifdef ENABLE_SERIAL2
BufferedSerial BSerial2(usartHandler(2));
#endif
#ifdef ENABLE_SERIAL3
BufferedSerial BSerial3(usartHandler(3));
#endif
#ifdef ENABLE_SERIAL4
BufferedSerial BSerial4(usartHandler(4));
#endif
#ifdef ENABLE_SERIAL5
BufferedSerial BSerial5(usartHandler(5));
#endif
#ifdef ENABLE_SERIAL6
BufferedSerial BSerial6(usartHandler(6));
#endif



#endif




