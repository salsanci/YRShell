#include "BufferedSerial_STM.h"
#include "YRShellInterpreter.h"
#ifdef PLATFORM_AC6
BufferedSerial::BufferedSerial( UART_HandleTypeDef* handler) :
	m_phandler(handler),
	m_nextQ(),
	m_previousQ(),
	m_initialized(false),
	m_bytesTx(0),
	m_error(ERROR::NONE)
{

}
BufferedSerial::ERROR BufferedSerial::initUart() {
	if(m_phandler == NULL) return ERROR::NULL_HANDLER;

	if( m_phandler->hdmarx) {
		HAL_StatusTypeDef handlerStatus = HAL_UART_Init(m_phandler);
		if(handlerStatus == HAL_OK) {
			handlerStatus = HAL_UART_Receive_DMA( m_phandler, (unsigned char*)m_nextQ.getBuffer(), m_nextQ.size());
		}
		return halStatusToError(handlerStatus);
	}
	// Only DMA is currently supported
	return ERROR::NO_DMA;
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
	processRX();
	processTX();
}
BufferedSerial::ERROR BufferedSerial::begin(uint32_t baud) {
	if(m_phandler == NULL) return ERROR::NULL_HANDLER;
	if(m_initialized) {
		end();
	}
	m_phandler->Init.BaudRate = baud;
	BufferedSerial::ERROR result = initUart();
	if(result == ERROR::NONE) {
		m_initialized = true;
	}
	return result;
}
void BufferedSerial::end(void) {
	if( m_initialized) {
		HAL_UART_DeInit(m_phandler);
		m_initialized = false;
	}
}
BufferedSerial::ERROR BufferedSerial::setBaud(uint32_t baud) {
	end();
	return begin(baud);
}

void BufferedSerial::processRX(void) {
	if(m_phandler == NULL) return;
	uint16_t used;
	uint16_t prevUsed    = m_nextQ.used();
	uint16_t availableQ  = m_nextQ.size();
	uint16_t bytesInDMA  = __HAL_DMA_GET_COUNTER( m_phandler->hdmarx);
	m_nextQ.setHead(availableQ - bytesInDMA);
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
			HAL_StatusTypeDef handlerStatus = HAL_UART_Transmit_DMA( m_phandler, (unsigned char*)m_previousQ.getLinearReadBuffer(), m_bytesTx);
			m_error = halStatusToError(handlerStatus);
		}
	}
}

BufferedSerial::ERROR BufferedSerial::halStatusToError(HAL_StatusTypeDef status) {
	switch(status) {
	case HAL_OK:
		return ERROR::NONE;
	case HAL_ERROR:
		return ERROR::HANDLER_ERROR;
	case HAL_BUSY:
		return ERROR::HANDLER_BUSY;
	case HAL_TIMEOUT:
		return ERROR::HANDLER_TIMEOUT;
	default:
		return ERROR::HANDLER_ERROR;
	}
}

#endif




