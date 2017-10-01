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
    
public:
    YRShellBase( ) { }
    virtual ~YRShellBase( ) { }
    void init( void);
    void setPrompt( const char* prompt);
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRShellBase"; }

};

template< unsigned DICTIONARY_SIZE, unsigned PAD_SIZE, unsigned NUM_REGISTERS,
    unsigned PARAMETER_STACK_SIZE, unsigned RETURN_STACK_SIZE, unsigned COMPILE_STACK_SIZE,
    unsigned INQ_SIZE, unsigned OUTQ_SIZE, unsigned AUX_INQ_SIZE, unsigned AUX_OUTQ_SIZE>
class YRShellTemplate : public YRShellBase{
protected:
    CurrentDictionary<DICTIONARY_SIZE> m_dictionaryCurrent;
    
    CircularQ<char, INQ_SIZE>       m_inq;
    CircularQ<char, AUX_INQ_SIZE>   m_auxInq;
    CircularQ<char, OUTQ_SIZE>      m_outq;
    CircularQ<char, AUX_OUTQ_SIZE>  m_auxOutq;
    
    char        m_pad[ PAD_SIZE];
    uint32_t    m_registers[ NUM_REGISTERS];
    
    uint32_t    m_parameterStack[ PARAMETER_STACK_SIZE];
    uint32_t    m_returnStack[ RETURN_STACK_SIZE];
    uint32_t    m_compileStack[ COMPILE_STACK_SIZE];
    

public:
    YRShellTemplate( ) {
        m_DictionaryCurrent = &m_dictionaryCurrent;
        m_Inq = &m_inq;
        m_AuxInq = &m_auxInq;
        m_Outq = &m_outq;
        m_AuxOutq = &m_auxOutq;
        m_Pad = m_pad;
        m_padSize = sizeof( m_pad);
        m_Registers = m_registers;
        m_numRegisters = sizeof(m_registers)/sizeof(m_registers[0]);
        m_ParameterStack = m_parameterStack;
        m_parameterStackSize = sizeof(m_parameterStack)/sizeof(m_parameterStack[0]);
        m_ReturnStack = m_returnStack;
        m_returnStackSize = sizeof(m_returnStack)/sizeof(m_returnStack[0]);
        m_CompileStack = m_compileStack;
        m_compileStackSize = sizeof(m_compileStack)/sizeof(m_compileStack[0]);
    }
    virtual ~YRShellTemplate( ) { }
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRShellTemplate"; }
};

class YRShell : public YRShellTemplate<2048, 128, 16, 16, 16, 8, 128, 256, 128, 256>{
protected:
public:
    YRShell( ) { }
    virtual ~YRShell( ) { }
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRShell"; }
};

#endif

