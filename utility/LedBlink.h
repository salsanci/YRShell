#ifndef LedBlink_h
#define LedBlink_h

#include "utility/Sliceable.h"
#include "utility/IntervalTimer.h"

class LedBlink : public Sliceable {
protected:
  int8_t m_ledPin, m_tos;

  uint32_t m_stack[ 16], m_ledOnMs, m_ledOffMs;
  bool m_ledState;
  IntervalTimer m_ledTimer;

public:
  LedBlink();
  virtual ~LedBlink( ) { }
  virtual const char* sliceName( ) { return "LedBlink"; }
  virtual void slice( void);
  void setLedPin( int8_t p);
  void setLedOnOffMs( uint32_t on, uint32_t off);
  void blink( uint32_t timeMs) { setLedOnOffMs( timeMs, timeMs); }
  void on( void) { setLedOnOffMs( 0xFFFFFFF, 1); }
  void off( void) { setLedOnOffMs( 1, 0xFFFFFFF); }
  void push();
  void pop();

};

#endif
