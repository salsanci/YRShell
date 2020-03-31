#include <stdint.h>
#include <stddef.h>
#include <utility/InterruptSliceable.h>

InterruptSliceable* InterruptSliceable::s_First   = NULL;
InterruptSliceable* InterruptSliceable::s_Current = NULL;
InterruptSliceable* InterruptSliceable::s_Last    = NULL;

InterruptSliceable::InterruptSliceable() {
	m_Next = NULL;
	if( s_First == NULL) {
		s_Last = s_First = this;
	} else {
		s_Last->m_Next = this;
		s_Last = this;
	}
	m_sliceEnabled = true;
}
InterruptSliceable::~InterruptSliceable() {
}

void InterruptSliceable::sliceOne() {
	if(s_Current == NULL) {
		s_Current = s_First;
	}
	if(s_Current != NULL) {
		if(s_Current->m_sliceEnabled) {
			s_Current->sliceInterrupt();
		}
		s_Current = s_Current->m_Next;
	}
}
