#ifndef UTILITY_BUFFEREDSERIAL_STM_H_
#define UTILITY_BUFFEREDSERIAL_STM_H_

#include "HardwareSpecific.h"

#ifdef PLATFORM_AC6

#include "CircularQ.h"
#include "main.h"
#include <YRShell.h>

#define BS_ST_TX_BUFFER_SIZE 256
#define BS_ST_RX_BUFFER_SIZE 256
#define BS_ST_TX_CHUNK_SIZE (BS_ST_TX_BUFFER_SIZE/4)

/** \brief BufferedSerial - provides interface to STM serial HAL using queues

 Provides interface to STM serial HAL using queues.

 */
class BufferedSerial : public Sliceable {
protected:
	UART_HandleTypeDef *m_phandler; /**< Pointer to the UART handler object */
	CircularQ<char, BS_ST_RX_BUFFER_SIZE> m_nextQ; /**< Pointer to the queue which will receive data from the HardwareSerial object */
	CircularQ<char, BS_ST_TX_BUFFER_SIZE> m_previousQ; /**< Pointer to the queue which will supply data to the HardwareSerial object */
public:
   virtual const char* sliceName( ) { return "BufferedSerial"; }
   /** \brief BufferedSerial - constructor

    Constructor

    */
     BufferedSerial( UART_HandleTypeDef* hs);
   /** \brief init - sets up the queues

    Sets up the queues. A null value is permissible to indicate there is no queue

    */
     void init(  CircularQBase<char>& nq, CircularQBase<char>& pq);
   /** \brief slice - move data from the HardwareSerial object to / from the queues

    Move data from the HardwareSerial object to / from the queues

    */
     void slice( void);
     void sliceFast(void);
     void begin( uint32_t baud);
     void end( void);
     void setBaud( uint32_t baud);
private:
     bool m_begun;
     uint16_t m_currentTXCount;

     void initUart( UART_HandleTypeDef* huart);
     void processRX( void);
     void processTX( void);
};

#ifdef ENABLE_SERIAL1
extern BufferedSerial BSerial1;
#endif
#ifdef ENABLE_SERIAL2
extern BufferedSerial BSerial2;
#endif
#ifdef ENABLE_SERIAL3
extern BufferedSerial BSerial3;
#endif
#ifdef ENABLE_SERIAL4
extern BufferedSerial BSerial4;
#endif
#ifdef ENABLE_SERIAL5
extern BufferedSerial BSerial5;
#endif
#ifdef ENABLE_SERIAL6
extern BufferedSerial BSerial6;
#endif

#endif
#endif /* UTILITY_BUFFEREDSERIAL_STM_H_ */
