
#ifndef CircularQ_h
#define CircularQ_h
#include <stdint.h>

/** \brief CircularQ - a FIFO Queue meant to be used by one reader and one writer
 
CircularQ is a template class which provides threadsafe operation for one reader and one writer.

The template declaration has 2 parameters: TYPE and SIZE. This is to easily support static declaration. 


The minimum value for size is 2, and teh maximum is 65,535.

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

template <class TYPE, unsigned SIZE>
class CircularQ
{
protected:  
    uint16_t m_head; /**< Index to the next value that will be written to the queue. */
    uint16_t m_tail; /**< Index to the next value that will be read from the queue. If equal to m_head the queue is empty. */
    uint16_t m_size; /**< The size of the queue buffer in items. The number of values which can be stored is m_size - 1. */
    TYPE m_buf[ SIZE]; /**< Storage for the items in the queue. */
public:
    /** \brief Constructor.

    Default constructor, no parameters are required.
    */
    CircularQ() {
        reset();
    }
    /** \brief Deletes all items in the queue.

    Only the indices are updated. The buffer is not reset to a know value.
    */
    void reset() {
        m_size = SIZE;
        m_head = m_tail = 0;
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
        if( m_head >=  m_tail) {
            rc = m_head - m_tail;
        } else {
            rc = SIZE - m_tail + m_head;
        }
        return rc;
    }
    /** \brief Returns the number of items which the queue can accept.

    Returns the number of items which the queue can accept.
    */
    const uint16_t free( ) {
        return SIZE - used() - 1;
    }
    /** \brief Check for values in the queue.

    Returns true if an item (or if a n is specified, n items) are available.
    */
    const bool valueAvailable( uint16_t n = 1) {
        return used() >= n;
    }
    /** \brief Checks for space in the queue.

    Returns true if an item (or if a n is specified, n items) can be written to the queue.
    */
    const bool spaceAvailable(  uint16_t n = 1) {
        return free() >= 1;
    }
    /** \brief Get a value from the queue.
 
    Returns the next item in the queue. valueAvailable() should always be called first.
    */
    TYPE get() {
        TYPE rc = 0;
        uint16_t newTail;
        if( m_head != m_tail) {
            rc = m_buf[ m_tail];
            newTail = m_tail + 1;
            if( newTail  >= SIZE ) {
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
    bool put( TYPE v) {
        bool rc = false;
        uint16_t newHead;
        newHead = m_head + 1;
        if( newHead >=  SIZE ){
            newHead = 0;
        }
        if( newHead != m_tail) {
            m_buf[ m_head] = v;
            m_head = newHead;
            rc = true;
        }
        return rc;
    }
};


#endif

