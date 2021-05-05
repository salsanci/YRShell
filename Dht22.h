#ifndef Dht22_h
#define Dht22_h
#include <stdint.h>

class Dht22 {
  private:
  int  m_pin;
  uint8_t m_buf[ 5];
  static uint16_t m_rh, m_temp, m_rc;
  public:
  Dht22( int p) { m_pin = p; }
  void dhtRead( void);

  static uint16_t getRc( void) { return m_rc; }
  static uint16_t getRh( void) { return m_rh; }
  static uint16_t getTemp( void) { return m_temp; }
};

#endif
