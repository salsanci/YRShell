#include "Dht22.h"
#include <ESP8266WiFi.h>

#define TIMEOUT_LIMIT 10000

uint16_t Dht22::m_rh = 0;
uint16_t Dht22::m_temp = 0;
uint16_t Dht22::m_rc = 0;


void Dht22::dhtRead( ) {
  m_rc = 0;
  uint8_t count = 7;
  uint8_t index = 0;
  
  for (int i=0; i< 5; i++){
    m_buf[i] = 0;
  }
  pinMode(m_pin, OUTPUT);
  digitalWrite(m_pin, LOW);
  delay(2);
  pinMode(m_pin, INPUT);
  delayMicroseconds(20);

  unsigned int timeoutCount = TIMEOUT_LIMIT;
  while( !m_rc && digitalRead(m_pin) == HIGH) {
    if( timeoutCount-- == 0) {
      m_rc = 1;
    }
  }
  timeoutCount = TIMEOUT_LIMIT;
  while( !m_rc && digitalRead(m_pin) == LOW) {
    if( timeoutCount-- == 0) {
      m_rc = 2;
    }
  }
  timeoutCount = TIMEOUT_LIMIT;
  while( !m_rc && digitalRead(m_pin) == HIGH) {
    if( timeoutCount-- == 0) {
      m_rc = 3;
    }
  }
  for( int i = 0; !m_rc && i < 40; i++) {
    timeoutCount = TIMEOUT_LIMIT;
    while( !m_rc && digitalRead(m_pin) == LOW) {
        if( timeoutCount-- == 0) {
        m_rc = 4;
      }
    }
    unsigned long t = micros();

    timeoutCount = TIMEOUT_LIMIT;
    while( !m_rc && digitalRead(m_pin) == HIGH) {
      if( timeoutCount-- == 0) {
        m_rc = 5;
      }
    }
    if( (micros() - t) > 40 ) {
      m_buf[index] |= (1 << count);
    } 
    if (count-- == 0) {
      count = 7;
      index++;
    }
  }

  uint8_t xcs = m_buf[0] + m_buf[1] + m_buf[2] + m_buf[3];
  if( m_buf[4] != xcs) {
    m_rc = 6;
  }
  m_rh = (((uint16_t) m_buf[0]) << 8)  + ((uint16_t) m_buf[1]);
  m_temp = (((uint16_t) m_buf[2]) << 8)  + ((uint16_t) m_buf[3]);
 
  return;
}
