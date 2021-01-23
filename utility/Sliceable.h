#ifndef Sliceable_h
#define Sliceable_h
#include <stdint.h>
#include <stddef.h>

#include "HardwareSpecific.h"
#include "HiResTimer.h"

#define SLICE_SLOW_LIMIT 100


class Sliceable;
class SliceAllTimer;
class Sliceable {
protected:
    Sliceable*  m_Next;
    HiResTimer  m_timer;
    bool        m_timeSlice;
    bool        m_sliceEnabled;

    static Sliceable* s_First;
    static Sliceable* s_Current;
    static Sliceable* s_Last;
    static Sliceable* s_Priority;
    static SliceAllTimer* s_SliceAllTimer;
    static uint32_t s_slowCounter;

public:
    void setPriority( void);
    void resetPriority( void);
    static void slicePriority( void);
    static void sliceOne( void);
    static void sliceAll( void);
    static void sliceSlow( void);
    static Sliceable* getSlicePointer( uint16_t n);
    Sliceable( void);
    virtual ~Sliceable( void);
    virtual void slice( void);
    virtual const char* sliceName( void);
    void resetTimer( void);
    void startTimer( void);
    void stopTimer( void);
    uint32_t getTimerCount( void);
    uint32_t getTimerMin( void);
    uint32_t getTimerMax( void);
    uint32_t getTimerAverage( void);
    void sliceEnable( void) { m_sliceEnabled = true; }
    void sliceDisable( void) { m_sliceEnabled = false; }

};

class SliceAllTimer : public Sliceable {
public:
    SliceAllTimer( void) { m_timeSlice = false; }
    virtual ~SliceAllTimer( void) { }
    virtual const char* sliceName( void) { return "SliceAllTimer"; }
    virtual void slice( void) {  }
 };

#endif
