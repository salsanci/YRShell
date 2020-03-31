#ifndef UTILITY_INTERRUPTSLICEABLE_H_
#define UTILITY_INTERRUPTSLICEABLE_H_

class InterruptSliceable {
protected:
	InterruptSliceable*  m_Next;
    bool                 m_sliceEnabled;

    static InterruptSliceable* s_First;
    static InterruptSliceable* s_Current;
    static InterruptSliceable* s_Last;
public:
	InterruptSliceable();
	virtual ~InterruptSliceable(void);
	static void sliceOne(void);
	virtual void sliceInterrupt(void) = 0;
};

#endif /* UTILITY_INTERRUPTSLICEABLE_H_ */
