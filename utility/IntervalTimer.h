#ifndef IntervalTimer_h
#define IntervalTimer_h

#ifdef NOTARDUINO_AC6
#include "processorGlobal.h"
#endif

class IntervalTimer {
private:
#if defined(__linux__) || defined(__APPLE__)
    double m_start, m_interval;
#else
   unsigned m_start, m_interval;
#endif
public:
    IntervalTimer( unsigned intervalInMilliSeconds = 0);
    void setInterval( unsigned intervalInMilliSeconds);
    bool hasIntervalElapsed( void);
};

#endif

