#ifndef CircularQ_h
#define CircularQ_h

#include "HardwareSpecific.h"
#include "Sliceable.h"

#include <stdint.h>
#include <string.h>
#include <atomic>

/** \brief CircularQBase - abstract base class for a FIFO Queue meant to be used by one reader and one writer
 
 CircularQBase is a template class which provides threadsafe operation for one reader and one writer.

 The template declaration has 1 parameters: TYPE.  This is to easily support and abstraction for queues of the same type, but of different sizes.


 Common use is to typedef,
 This allows for more compact declaration, especially when passing as a reference as a parameter.

 Examples:
 typedef CircularQBase<char>         MyCharQ;
 MyCharQ         *InQ = & CircularQ<char, 23>;
 MyCharQ         *AuxInQ = & CircularQ<char, 23>;

 
 
 */
template<class TYPE>
class CircularQBase : public Sliceable {

public:
	CircularQBase() {
	}
	virtual ~CircularQBase() {
	}

	virtual void reset(void) = 0;
	virtual const uint16_t size(void) = 0;
	virtual const uint16_t used(void) = 0;
	virtual const uint16_t free(void) = 0;
	virtual const bool valueAvailable(uint16_t n = 1) = 0;
	virtual const bool spaceAvailable(uint16_t n = 1) = 0;
	virtual TYPE get(void) = 0;
	virtual bool put(TYPE v) = 0;
	virtual char* getBuffer(void) = 0;
	virtual char* getLinearReadBuffer(void) = 0;
	virtual uint16_t getLinearReadBufferSize(void) = 0;
	virtual void drop(uint16_t n) = 0;
	virtual char* getLinearWriteBuffer(void) = 0;
	virtual uint16_t getLinearWriteBufferSize(void) = 0;
	virtual void append(uint16_t n) = 0;
	virtual void setHead(uint16_t h) = 0;
	virtual void setNextQ( CircularQBase<TYPE>& q) = 0;
	virtual void setPreviousQ( CircularQBase<TYPE>& q) = 0;
	virtual void slice( void) = 0;
};

/** \brief CircularQ - a FIFO Queue meant to be used by one reader and one writer

 CircularQ is a template class which provides thread safe operation for one reader and one writer.

 The template declaration has 2 parameters: TYPE and SIZE. This is to easily support static declaration.


 The minimum value for size is 2, and the maximum is 65,535.

 Common use is to typedef,
 This allows for more compact declaration, especially when passing as a reference as a parameter.

 Examples:
 typedef CircularQ<char,     30>     MyInQ;
 typedef CircularQ<int,      22>     MyOutQ;
 typedef CircularQ<unsigend, 333>    InternalQ;

 MyInQ           inq;
 MyOutQ          outQ;
 MyInternalQ     iQ;



 */
template<class TYPE, unsigned SIZE>
class CircularQ: public CircularQBase<TYPE> {
protected:
	CircularQBase<TYPE>* m_nextQ; /**< Pointer to a queue which receives the output of this queue. */
	CircularQBase<TYPE>* m_previousQ; /**< Pointer to a queue which provides the input to this queue. */
	uint16_t m_head; /**< Index to the next value that will be written to the queue. */
	uint16_t m_tail; /**< Index to the next value that will be read from the queue. If equal to m_head the queue is empty. */
	TYPE m_buf[SIZE]; /**< Storage for the items in the queue. */
public:
	virtual const char* sliceName( ) { return "CircularQ"; }

	/** \brief Constructor.

	 Default constructor, no parameters are required.
	 */
	CircularQ() {
		m_nextQ = NULL;
		reset();
	}
	/** \brief Destructor.

	 Default destructor, no parameters are required.
	 */
	virtual ~CircularQ( ) {

	}
	/** \brief Clears Q.

	 Clears Q.
	 */
	void reset() {
		m_head = m_tail = 0;
	}
	/** \brief Sets the queue which receives the output of this Q.

	 Sets the queue which receives the output of this Q.
	 */
	void setNextQ( CircularQBase<TYPE>& q) {
		m_nextQ = &q;
	}
	/** \brief Sets the queue which receives the output of this Q.

	 Sets the queue which receives the output of this Q.
	 */
	void setPreviousQ( CircularQBase<TYPE>& q) {
		m_previousQ = &q;
	}
	/** \brief Pulls input from the queue Q, pushes output to the next queue.

	 Pulls input from the queue Q, pushes output to the next queue.
	 */
	void slice( ) {
		uint16_t s1, s2;
		if( m_nextQ != NULL) {
			s1 = getLinearReadBufferSize();
			s2 = m_nextQ->getLinearWriteBufferSize();
			s1 = s1 <= s2 ? s1 : s2;
			if( s1 > 0) {
				memcpy(m_nextQ->getLinearWriteBuffer(), getLinearReadBuffer(), s1);
                atomic_thread_fence( std::memory_order_release);
				drop( s1);
				m_nextQ->append( s1);
			}
		}
		if( m_previousQ != NULL) {
			s1 = getLinearWriteBufferSize();
			s2 = m_previousQ->getLinearReadBufferSize();
			s1 = s1 <= s2 ? s1 : s2;
			if( s1 > 0) {
				memcpy(getLinearWriteBuffer(), m_previousQ->getLinearReadBuffer(), s1);
                atomic_thread_fence( std::memory_order_release);
				m_previousQ->drop( s1);
				append( s1);
			}
		}
	}
	/** \brief Drops the next n items in the Q

	 Drops the next n items in the Q.
	 */
	void drop(uint16_t n) {
		uint16_t sz = used();
		if (n > sz) {
			n = sz;
		}
		if (n > 0) {
			sz = m_tail + n;
			if(sz >= SIZE) {
				sz -= SIZE;
			}
			m_tail = sz;
		}
	}
	/** \brief Appends the next n items in the Q

	 Appends the next n items in the Q.
	 */
	void append(uint16_t n) {
		uint16_t sz = free();
		if (n > sz) {
			n = sz;
		}
		if (n > 0) {
			sz = m_head + n;
			if(sz >= SIZE) {
				sz -= SIZE;
			}
			m_head = sz;
		}
	}
	/** \brief Sets the pointer to the next item for a put

	 Sets the pointer to the next item for a put.
	 */
	void setHead(uint16_t h) {
		if (h < SIZE) {
			m_head = h;
		} else {
			m_head = 0;
		}
	}
	/** \brief Returns a pointer to the buffer used by the queue for the next get operation

	 Returns a pointer to the buffer used by the queue for the next get operation.
	 */
	TYPE* getLinearReadBuffer() {
		return &m_buf[m_tail];
	}
	/** \brief Returns the size of the linear buffer

	 Returns the size of the linear buffer.
	 */
	uint16_t getLinearReadBufferSize() {
		uint16_t rc = 0;
		if (m_tail < m_head) {
			rc = m_head - m_tail;
		} else if (m_tail > m_head) {
			rc = SIZE - m_tail;
		}
		return rc;
	}
	/** \brief Returns a pointer to the buffer used by the queue for the next put operation

	 Returns a pointer to the buffer used by the queue for the next put operation.
	 */
	TYPE* getLinearWriteBuffer() {
		return &m_buf[m_head];
	}
	/** \brief Returns the size of the linear buffer

	 Returns the size of the linear buffer.
	 */
	uint16_t getLinearWriteBufferSize() {
		uint16_t rc = 0;
		if( m_head < m_tail) {
			rc = m_tail - 1 - m_head;
		} else if( m_head >= m_tail) {
			rc = SIZE - m_head;
			if( m_tail == 0 && rc > 0) {
				rc--;
			}
		}
		return rc;
	}
	/** \brief Returns a pointer to the buffer used by the queue

	 Returns a pointer to the buffer used by the queue.
	 */
	TYPE* getBuffer() {
		return m_buf;
	}
	/** \brief Returns number of items + 1 which can be stored in the queue

	 Returns the number of items + 1 which can be stored in the queue.
	 */
	const uint16_t size() {
		return SIZE;
	}
	/** \brief Returns the number of items in the queue.

	 Returns the number of items in the queue.
	 */
	const uint16_t used() {
		uint16_t rc;
		if (m_head >= m_tail) {
			rc = m_head - m_tail;
		} else {
			rc = SIZE - m_tail + m_head;
		}
		return rc;
	}
	/** \brief Returns the number of items which the queue can accept.

	 Returns the number of items which the queue can accept.
	 */
	const uint16_t free() {
		return SIZE - used() - 1;
	}
	/** \brief Check for values in the queue.

	 Returns true if an item (or if a n is specified, n items) are available.
	 */
	const bool valueAvailable(uint16_t n = 1) {
		return used() >= n;
	}
	/** \brief Checks for space in the queue.

	 Returns true if an item (or if a n is specified, n items) can be written to the queue.
	 */
	const bool spaceAvailable(uint16_t n = 1) {
		return free() >= 1;
	}
	/** \brief Get a value from the queue.

	 Returns the next item in the queue. valueAvailable() should always be called first.
	 */
	TYPE get(void) {
		TYPE rc = 0;
		uint16_t newTail;
		if (m_head != m_tail) {
			rc = m_buf[m_tail];
			newTail = m_tail + 1;
            atomic_thread_fence( std::memory_order_release);
			if (newTail >= SIZE) {
				m_tail = 0;
			} else {
				m_tail = newTail;
			}
		}
		return rc;
	}
	/** \brief Put a value in the queue.

	 Adds a value to the queue. Returns true if successful, false if there is no space in the queue.
	 */
	bool put(TYPE v) {
		bool rc = false;
		uint16_t newHead;
		newHead = m_head + 1;
		if (newHead >= SIZE) {
			newHead = 0;
		}
		if (newHead != m_tail) {
			m_buf[m_head] = v;
            atomic_thread_fence( std::memory_order_release);
			m_head = newHead;
			rc = true;
		}
		return rc;
	}
};

#endif

