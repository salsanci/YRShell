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
    if( !m_timerValid) {
        m_timer = getSysticks();
        m_timerValid = true;
    }
}
void HiResTimer::stop( ){
    if( m_timerValid) {
        m_count++;
        uint32_t elapsed =  getSysticks() - m_timer;
        if( elapsed < m_min) {
            m_min = elapsed;
        }
        if( elapsed > m_max) {
            m_max = elapsed;
        }
        m_accumulator += (uint64_t) elapsed;
        m_timerValid = false;
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

uint32_t HiResTimer::getSysticks() {
    return HW_getSysticks( );
}
uint32_t HiResTimer::getMicros() {
    return HW_getMicros( );
}
uint32_t HiResTimer::getMillis() {
    return HW_getMillis( );
}