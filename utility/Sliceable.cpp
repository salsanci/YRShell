#include "Sliceable.h"

Sliceable* Sliceable::s_First = NULL;
Sliceable* Sliceable::s_Current = NULL;
Sliceable* Sliceable::s_Last = NULL;
uint32_t Sliceable::s_slowCounter = SLICE_SLOW_LIMIT;

// Finish THis
class SliceAll : public Sliceable {
    SliceAll( void);
    ~SliceAll( void);
    void slice( void);
};
SliceAll::SliceAll( void) { }
SliceAll::~SliceAll( void) { }
void SliceAll::slice( void) { }

Sliceable::Sliceable() {
	m_Next = NULL;
	if( s_First == NULL) {
		s_Last = s_First = this;
	} else {
		s_Last->m_Next = this;
		s_Last = this;
    }
}
Sliceable::~Sliceable() {
}
void Sliceable::sliceOne() {
	if( s_Current == NULL) {
		s_Current = s_First;
	}
	if( s_Current != NULL) {
		s_Current->startTimer();
		s_Current->slice();
		s_Current->stopTimer();
		s_Current = s_Current->m_Next;
	}
}
void Sliceable::sliceAll() {
	sliceOne( );
    for( uint16_t i = 0; i < 0x200 && s_Current != NULL; i++) {
        sliceOne();
    }
}
void Sliceable::sliceSlow() {
    if( --s_slowCounter == 0) {
        s_slowCounter = SLICE_SLOW_LIMIT;
        sliceOne();
    }
}
const char* Sliceable::sliceName( ) {
	return "Sliceable";
}
void Sliceable::resetTimer( ) {
	m_timer.reset();
}
void Sliceable::startTimer( ) {
	m_timer.start();
}
void Sliceable::stopTimer( ) {
	m_timer.stop();
}
uint32_t Sliceable::getTimerCount( ) {
	return m_timer.getCount();
}
uint32_t Sliceable::getTimerMin( ) {
	return m_timer.getMin();
}
uint32_t Sliceable::getTimerMax( ) {
	return m_timer.getMax();
}
uint32_t Sliceable::getTimerAverage( ) {
	return m_timer.getAverage();
}
Sliceable* Sliceable::getSlicePointer( uint16_t n) {
	Sliceable* rc = s_First;
	while( rc != NULL ) {
		if( !n) {
			break;
		}
		n--;
		rc = rc->m_Next;
	}
	return rc;
}

