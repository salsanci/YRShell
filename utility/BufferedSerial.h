#ifndef BufferedSerial_h
#define BufferedSerial_h

#include "HardwareSpecific.h"

#ifdef PLATFORM_ARDUINO

#include "CircularQ.h"

/** \brief BufferedSerial - wrapper arouns the Arduino HardwareSerial class to interface to queues

 Wrapper arouns the Arduino HardwareSerial class to interface to queues.

 */
class BufferedSerial : public Sliceable {
protected:
  HardwareSerial *m_hs; /**< Pointer to the HardwareSerial object */
  CircularQBase<char>* m_nextQ; /**< Pointer to the queue which will receive data from the HardwareSerial object */
  CircularQBase<char>* m_previousQ; /**< Pointer to the queue which will supply data to the HardwareSerial object */
  
public:
   virtual const char* sliceName( ) { return "BufferedSerial"; }
/** \brief BufferedSerial - constructor

 Constructor

 */
  BufferedSerial( HardwareSerial* hs);
/** \brief init - sets up the queues

 Sets up the queues. A null value is permissible to indicate there is no queue

 */
  void init(  CircularQBase<char>& nq, CircularQBase<char>& pq);
/** \brief slice - move data from the HardwareSerial object to / from the queues

 Move data from the HardwareSerial object to / from the queues

 */
  void slice( void);
  void begin( uint32_t baud);
  void end( void);
  void setBaud( uint32_t baud);
  
};



extern BufferedSerial BSerial;  
#ifdef ENABLE_SERIAL1
extern BufferedSerial BSerial1;  
#endif
#ifdef ENABLE_SERIAL2
extern BufferedSerial BSerial2;  
#endif
#ifdef ENABLE_SERIAL3
extern BufferedSerial BSerial3;  
#endif


#endif

#endif
