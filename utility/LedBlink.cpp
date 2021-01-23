#include "LedBlink.h"


void LedBlink::setLedOnOffMs( uint32_t on, uint32_t off) {
  m_ledOnMs = on;
  m_ledOffMs = off;
  m_ledTimer.setInterval( m_ledOffMs);
  digitalWrite( m_ledPin, 1);
  m_ledState = false;
}

LedBlink::LedBlink( ) {
  m_ledPin = -1;
  m_ledState = false;
  m_ledOnMs = 1;
  m_ledOffMs = 10000;
  m_tos = 0;
}

void LedBlink::setLedPin( int8_t ledPin) {
  if( ledPin > 0) {
    m_ledPin = ledPin;
    pinMode( m_ledPin, OUTPUT);
  }
}

void LedBlink::slice() {
  if( m_ledPin > 0) {
    if( m_ledState) {
      if( m_ledTimer.hasIntervalElapsed()) {
        m_ledState = false;
        m_ledTimer.setInterval( m_ledOffMs);
        digitalWrite( m_ledPin, 1);
      }
    } else {
      if( m_ledTimer.hasIntervalElapsed()) {
        m_ledState = true;
        m_ledTimer.setInterval( m_ledOnMs);
        digitalWrite( m_ledPin, 0);
      }
    }
  }
}

void LedBlink::push() {
    if( m_tos <= ((sizeof(m_stack)/sizeof(m_stack[0]))-2) ) {
        m_stack[ m_tos++] = m_ledOnMs;
        m_stack[ m_tos++] = m_ledOffMs;
    }
}

void LedBlink::pop() {
    if( m_tos >= 2) {
        m_ledOffMs = m_stack[ --m_tos];
        m_ledOnMs = m_stack[ --m_tos];
    }
}
