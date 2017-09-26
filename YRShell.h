#ifndef Shell_h
#define Shell_h

#ifdef NOTARDUINO_AC6
#include "processorGlobal.h"
#endif

#include "utility/YRShellInterpreter.h"



/** \brief YRShell - interactive
 
 Details on whatYRShell is
 
 
 */
class YRShellBase : public YRShellInterpreter{
public:
/** \brief Used by YRShell to map to the native functions.

Used by YRShell to map to the native functions.
*/
enum S_CC_functions {
    S_CC_first = YRSHELL_DICTIONARY_FUNCTION,
    S_CC_zero,
    S_CC_plusOne,
    S_CC_minusOne,
    S_CC_last
};

protected:    
    virtual void executeFunction( uint16_t n);
    virtual uint16_t find( const char* name);
    
public:
    YRShellBase( ) { }
    virtual ~YRShellBase( ) { }
    void init( void);
    void setPrompt( const char* prompt);

};

class YRShell : public YRShellBase{
protected:
    CurrentDictionary<YRSHELL_DICTIONARY_SIZE> m_dictionaryCurrent;
public:
    YRShell( ) {
        m_DictionaryCurrent = &m_dictionaryCurrent;
    }
    virtual ~YRShell( ) { }
};

class YRShellRun : public YRShellBase{
protected:
    CurrentDictionary<256> m_dictionaryCurrent;
public:
    YRShellRun( ) {
        m_DictionaryCurrent = &m_dictionaryCurrent;
    }
    virtual ~YRShellRun( ) { }
};

#endif

