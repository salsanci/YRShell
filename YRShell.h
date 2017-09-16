#ifndef Shell_h
#define Shell_h

#ifdef NOTARDUINO_AC6
#include "processorGlobal.h"
#endif

#include "utility/YRShellInterpreter.h"



/** \brief YRShell - interactive
 
 Details on whatYRShell is
 
 
 */

class YRShell : public YRShellInterpreter{
public:
/** \brief Used by YRShell to map to the native functions.

Used by YRShell to map to the native functions.
*/
enum S_CC_functions {
    S_CC_first = YRSHELL_DICTIONARY_FUNCTION,
    S_CC_zero,
    S_CC_plusOne,
    S_CC_plusTwo,
    S_CC_plusFour,
    S_CC_minusOne,
    S_CC_minusTwo,
    S_CC_minusFour,
    S_CC_last
};

protected:    

    virtual void executeFunction( uint16_t n);
    virtual uint16_t find( const char* name);

    
public:
    YRShell( ) { }
    virtual ~YRShell( ) { }
    void init( void);
    
};


#endif

