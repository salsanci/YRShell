#ifndef Sliceable_h
#define Sliceable_h
#include <stdint.h>
#include <stddef.h>

#include "HiResTimer.h"

#define SLICE_SLOW_LIMIT 100


class Sliceable;
class Sliceable {
protected:
    Sliceable* m_Next;
    HiResTimer m_timer;
    uint64_t hiResAccum;
    int32_t  hiResTimer;
    uint32_t hiResTimerMin;
    uint32_t hiResTimerMax;
    uint32_t hiResCount;

    static Sliceable* s_First;
    static Sliceable* s_Current;
    static Sliceable* s_Last;
    static uint32_t s_slowCounter;
public:
    static void sliceOne( void);
    static void sliceAll( void);
    static void sliceSlow( void);
    static Sliceable* getSlicePointer( uint16_t n);
    Sliceable( void);
    virtual ~Sliceable( void);
    virtual void slice( void) = 0;
    virtual const char* sliceName( void);
    void resetTimer( void);
    void startTimer( void);
    void stopTimer( void);
    uint32_t getTimerCount( void);
    uint32_t getTimerMin( void);
    uint32_t getTimerMax( void);
    uint32_t getTimerAverage( void);

};

#endif
