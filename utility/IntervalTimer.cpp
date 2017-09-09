
#include "IntervalTimer.h"
#include <time.h>

#if defined(__linux__) || defined(__APPLE__)
IntervalTimer::IntervalTimer( unsigned intervalInMilliSeconds) {
    setInterval( intervalInMilliSeconds);
}
void IntervalTimer::setInterval( unsigned intervalInMilliSeconds) {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    m_start = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000.0;
    m_interval = intervalInMilliSeconds;
}
bool IntervalTimer::hasIntervalElapsed( void) {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000.0;    return  (end - m_start) >=  m_interval;
}

#else
#include "Arduino.h"
IntervalTimer::IntervalTimer( unsigned intervalInMilliSeconds) {
    setInterval( intervalInMilliSeconds);
}
void IntervalTimer::setInterval( unsigned intervalInMilliSeconds) {
    m_start = millis();
    m_interval = intervalInMilliSeconds;
}
bool IntervalTimer::hasIntervalElapsed( void) {
    return (millis() - m_start) >= m_interval;
}
#endif


