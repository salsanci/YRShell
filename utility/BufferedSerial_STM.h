#ifndef UTILITY_BUFFEREDSERIAL_STM_H_
#define UTILITY_BUFFEREDSERIAL_STM_H_

#ifdef PLATFORM_AC6

#include "CircularQ.h"
#include "HardwareSpecific.h"
#include "InterruptSliceable.h"

#ifndef SERIAL_TX_BUFFER_SIZE
#define SERIAL_TX_BUFFER_SIZE 256
#endif
#ifndef SERIAL_RX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE 256
#endif
#ifndef SERIAL_TX_CHUNK_SIZE
#define SERIAL_TX_CHUNK_SIZE (SERIAL_TX_BUFFER_SIZE/4)
#endif

/** \brief BufferedSerial - provides interface to STM serial HAL using queues

 Provides interface to STM serial HAL using queues.

 */
class BufferedSerial : public Sliceable, public InterruptSliceable {
protected:
	UART_HandleTypeDef *m_phandler; /**< Pointer to the UART handler object */
	CircularQ<char, SERIAL_RX_BUFFER_SIZE> m_nextQ; /**< Pointer to the queue which will receive data from the Serial object */
	CircularQ<char, SERIAL_TX_BUFFER_SIZE> m_previousQ; /**< Pointer to the queue which will supply data to the Serial object */
public:

	enum ERROR {
		NONE = 0,
		NULL_HANDLER,
		NO_DMA,
		HANDLER_BUSY,
		HANDLER_ERROR,
		HANDLER_TIMEOUT,
		TX_FAILURE
	};

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
     ERROR begin( uint32_t baud);
     /** \brief end - Disable the Serial object

	Disable the Serial object

	*/
     void end( void);
     /** \brief setBaud - Change the baudrate of a Serial object

	Initialize and start the Serial object

	@param baud sets the baudrate of the serial object
	@return 0 = success, 1 = error, 2 = busy, 3 = timeout
	*/
     ERROR setBaud( uint32_t baud);

     ERROR error() const { return m_error; }
private:
     bool m_initialized;
     uint16_t m_bytesTx;
     ERROR m_error;

     ERROR initUart();
     void processRX( void);
     void processTX( void);

     static ERROR halStatusToError(HAL_StatusTypeDef status);
};

#endif
#endif /* UTILITY_BUFFEREDSERIAL_STM_H_ */
