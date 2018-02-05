#ifndef IntervalTimer_h
#define IntervalTimer_h

#include "HardwareSpecific.h"

class IntervalTimer {
private:
#if defined(PLATFORM_LUA)
    double m_start, m_interval;
#else
   unsigned m_start, m_interval;
#endif
public:
    IntervalTimer( unsigned intervalInMilliSeconds = 0);
    void setInterval( unsigned intervalInMilliSeconds);
    bool hasIntervalElapsed( void);
    bool isNextInterval(void);
};

#endif

