#ifndef IntervalTimer_h
#define IntervalTimer_h

#ifdef NOTARDUINO_AC6
#include "processorGlobal.h"
#endif

#if defined(__linux__) || defined(__APPLE__)

class IntervalTimer {
private:
    double m_start, m_interval;
public:
    IntervalTimer( unsigned intervalInMilliSeconds = 0);
    void setInterval( unsigned intervalInMilliSeconds);
    bool hasIntervalElapsed( void);

};
#else
class IntervalTimer {
private:
    unsigned m_start, m_interval;
public:
    IntervalTimer( unsigned intervalInMilliSeconds = 0);
    void setInterval( unsigned intervalInMilliSeconds);
    bool hasIntervalElapsed( void);
    
};
#endif

#endif

