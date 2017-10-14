#ifndef HIRESTIMER_h
#define HIRESTIMER_h

#include "HardwareSpecific.h"

#include <stdint.h>
#include <stddef.h>
class HiResTimer {
protected:
    uint64_t m_accumulator;
    uint32_t m_timer;
    uint32_t m_min;
    uint32_t m_max;
    uint32_t m_count;
    bool m_timerValid;
public:
    HiResTimer( void);
    void reset( void);
    void start( void);
    void stop( void);
 
    uint32_t getCount( void);
    uint32_t getMin( void);
    uint32_t getMax( void);
    uint32_t getAverage( void);

    static uint32_t getSysticks( void);
    static uint32_t getMicros( void);
    static uint32_t getMillis( void);
};

#endif