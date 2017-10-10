#include "HiResTimer.h"
HiResTimer::HiResTimer( ){
    reset();
}
void HiResTimer::reset( ){
    m_accumulator = 0;
    m_timer = 0;
    m_min = 0xFFFFFFFF;
    m_max = 0;
    m_count = 0;
    m_timerValid = false;
}
void HiResTimer::start( ){
    m_timer = getSysticks();
    m_count++;
    m_timerValid = true;
}
void HiResTimer::stop( ){
    if( m_timerValid) {
        uint32_t elapsed =  getSysticks() - m_timer;
        if( elapsed < m_min) {
            m_min = elapsed;
        }
        if( elapsed > m_max) {
            m_max = elapsed;
        }
        m_accumulator += (uint64_t) elapsed;
    }
}
uint32_t HiResTimer::getCount( ) {
    return m_count;
}
uint32_t HiResTimer::getMin( ) {
    return m_min;
}
uint32_t HiResTimer::getMax( ) {
    return m_max;
}
uint32_t HiResTimer::getAverage( ) {
    return (uint32_t) (m_accumulator / ((uint64_t) m_count));
}

#if defined(__linux__) || defined(__APPLE__)
#include <time.h>
unsigned HiResTimer::getSysticks() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000000000.0;
    return (unsigned) end; 
}
unsigned HiResTimer::getMicros() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000000.0;
    return (unsigned) end; 
}
unsigned HiResTimer::getMillis() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000.0;
    return (unsigned) end; 
}
#else
#ifdef NOTARDUINO_AC6
#include "stm32l4xx_hal.h"

unsigned HiResTimer::getSysticks() {
    return getMicros();
}
unsigned HiResTimer::getMicros() {
	uint32_t m0 = HAL_GetTick();
	uint32_t u0 = SysTick->LOAD - SysTick->VAL;
	uint32_t m1 = HAL_GetTick();
	uint32_t u1 = SysTick->LOAD - SysTick->VAL;

	if (m1 > m0) {
		return ( m1 * 1000 + (u1 * 1000) / SysTick->LOAD);
	} else {
		return ( m0 * 1000 + (u0 * 1000) / SysTick->LOAD);
	}
}
unsigned HiResTimer::getMillis() {
    return HAL_GetTick();
}

#else
#include "Arduino.h"
unsigned HiResTimer::getSysticks() {
    return micros(); 
}
unsigned HiResTimer::getMicros() {
    return micros(); 
}
unsigned HiResTimer::getMillis() {
    return millis(); 
} 
#endif
#endif
