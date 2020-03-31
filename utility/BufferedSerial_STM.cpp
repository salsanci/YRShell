#include "BufferedSerial_STM.h"
#include "YRShellInterpreter.h"
#ifdef PLATFORM_AC6
BufferedSerial::BufferedSerial( UART_HandleTypeDef* handler) :
	m_phandler(handler),
	m_begun(false),
	m_currentTXCount(0)
{

}
void BufferedSerial::initUart( UART_HandleTypeDef* huart) {
	HAL_StatusTypeDef hrc;
	m_phandler = huart;

	if( m_phandler->hdmarx) {
		hrc = HAL_UART_Receive_DMA( m_phandler, (unsigned char*)m_nextQ.getBuffer(), m_nextQ.size());
		if( HAL_OK != hrc) {
			data_fatalERROR( __FILE__, __LINE__ , hrc);
		}
	} else {
		hrc = HAL_UART_Receive_IT( m_phandler, (unsigned char*)m_nextQ.getLinearWriteBuffer(), 1);
		if( HAL_OK != hrc) {
			data_fatalERROR( __FILE__, __LINE__ , hrc);
		}
	}

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
void BufferedSerial::sliceFast(void) {
	if( m_phandler->hdmarx) {
		processRX();
	}
	processTX();
}
void BufferedSerial::begin(uint32_t baud) {
	if(m_begun) {
		setBaud(baud);
	} else {
		m_begun = true;
		m_phandler->Init.BaudRate = baud;
		HAL_UART_Init(m_phandler);
		initUart(m_phandler);
	}
}
void BufferedSerial::end( void) {
	if( m_begun) {
		HAL_UART_DeInit( m_phandler);
		m_begun = false;
	}
}
void BufferedSerial::setBaud( uint32_t baud) {
	end();
	begin(baud);
}

void BufferedSerial::processRX( void) {
	uint16_t used, oldUsed, bs, dmac;
	bs = m_nextQ.size();
	oldUsed = m_nextQ.used();

	if( m_phandler->hdmarx) {
		dmac = 	__HAL_DMA_GET_COUNTER( m_phandler->hdmarx);
		m_nextQ.setHead( bs - dmac);
	} else {
		fatalERROR(__FILE__, __LINE__ );
	}
	used = m_nextQ.used();
	if( used < oldUsed) {
		//m_inputQOverrun = true;
		m_nextQ.reset();
	}
}
void BufferedSerial::processTX( void) {
	if( m_phandler->gState == HAL_UART_STATE_READY)
	{
		HAL_StatusTypeDef hrc;
		if( m_currentTXCount) {
			m_previousQ.drop(m_currentTXCount);
			m_currentTXCount = 0;
		}
		m_currentTXCount = m_previousQ.getLinearReadBufferSize( );
		if(m_currentTXCount > 0) {
			m_currentTXCount = m_currentTXCount > BS_ST_TX_CHUNK_SIZE ? BS_ST_TX_CHUNK_SIZE : m_currentTXCount;
			if( m_phandler->hdmatx) {
				if( HAL_OK != (hrc = HAL_UART_Transmit_DMA( m_phandler, (unsigned char*)m_previousQ.getLinearReadBuffer(), m_currentTXCount)) ) {
					data_fatalERROR(__FILE__, __LINE__ , hrc);
				}
			} else {
				if( HAL_OK != (hrc = HAL_UART_Transmit_IT( m_phandler, (unsigned char*)m_previousQ.getLinearReadBuffer(), m_currentTXCount)) ) {
					data_fatalERROR(__FILE__, __LINE__ , hrc);
				}
			}
		}
	}
}

#ifdef ENABLE_SERIAL1
extern UART_HandleTypeDef huart1;
BufferedSerial BSerial1(&huart1);
#endif
#ifdef ENABLE_SERIAL2
extern UART_HandleTypeDef huart2;
BufferedSerial BSerial2(&huart2);
#endif
#ifdef ENABLE_SERIAL3
extern UART_HandleTypeDef huart3;
BufferedSerial BSerial3(&huart3);
#endif
#ifdef ENABLE_SERIAL4
extern UART_HandleTypeDef huart4;
BufferedSerial BSerial4(&huart4);
#endif
#ifdef ENABLE_SERIAL5
extern UART_HandleTypeDef huart5;
BufferedSerial BSerial5(&huart5);
#endif
#ifdef ENABLE_SERIAL6
extern UART_HandleTypeDef huart6;
BufferedSerial BSerial6(&huart6);
#endif



#endif




