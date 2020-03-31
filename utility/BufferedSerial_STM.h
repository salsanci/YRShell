#ifndef UTILITY_BUFFEREDSERIAL_STM_H_
#define UTILITY_BUFFEREDSERIAL_STM_H_

#include "HardwareSpecific.h"

#ifdef PLATFORM_AC6

#include "CircularQ.h"
#include "main.h"
#include <YRShell.h>
#include <utility/InterruptSliceable.h>

/** \brief BufferedSerial - provides interface to STM serial HAL using queues

 Provides interface to STM serial HAL using queues.

 */
class BufferedSerial : public Sliceable, public InterruptSliceable {
protected:
	UART_HandleTypeDef *m_phandler; /**< Pointer to the UART handler object */
	CircularQ<char, SERIAL_RX_BUFFER_SIZE> m_nextQ; /**< Pointer to the queue which will receive data from the Serial object */
	CircularQ<char, SERIAL_TX_BUFFER_SIZE> m_previousQ; /**< Pointer to the queue which will supply data to the Serial object */
public:
   virtual const char* sliceName( ) { return "BufferedSerial"; }
   /** \brief BufferedSerial - constructor

    Constructor

    */
     BufferedSerial( UART_HandleTypeDef* phandler);
   /** \brief init - sets up the queues

    Sets up the queues. A null value is permissible to indicate there is no queue

    */
     void init(  CircularQBase<char>& nq, CircularQBase<char>& pq);
   /** \brief slice - process data in queues

    Process data in queues

    */
     void slice( void);
     /** \brief sliceFast - move data from the Serial object to / from the queues

	Move data from the Serial object to / from the queues.  Needs to be called ~1000 times per second,
	usually in an interrupt context.

	*/
     void sliceInterrupt(void);
     /** \brief begin - Initialize and start the Serial object

	Initialize and start the Serial object

	@param baud sets the baudrate of the serial object
	@return 0 = success, 1 = error, 2 = busy, 3 = timeout
	*/
     uint8_t begin( uint32_t baud);
     /** \brief end - Disable the Serial object

	Disable the Serial object

	*/
     void end( void);
     /** \brief setBaud - Change the baudrate of a Serial object

	Initialize and start the Serial object

	@param baud sets the baudrate of the serial object
	@return 0 = success, 1 = error, 2 = busy, 3 = timeout
	*/
     uint8_t setBaud( uint32_t baud);
private:
     bool m_initialized;
     uint16_t m_bytesTx;

     uint8_t initUart();
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
