
#include "HardwareSpecific.h"
#include "IntervalTimer.h"
#include "HiResTimer.h"


IntervalTimer::IntervalTimer( unsigned intervalInMilliSeconds) {
    setInterval( intervalInMilliSeconds);
}

#ifdef PLATFORM_LA
#include <time.h>
void IntervalTimer::setInterval( unsigned intervalInMilliSeconds) {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    m_start = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000.0;
    m_interval = intervalInMilliSeconds;
}
bool IntervalTimer::hasIntervalElapsed( void) {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000.0;
    return  (end - m_start) >=  m_interval;
}
#else

void IntervalTimer::setInterval( unsigned intervalInMilliSeconds) {
    m_start = HiResTimer::getMillis();
    m_interval = intervalInMilliSeconds;
}
bool IntervalTimer::hasIntervalElapsed( void) {
    return (HiResTimer::getMillis() - m_start) >= m_interval;
}
#endif
