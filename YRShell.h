#ifndef YRShell_h
#define YRShell_h

#include "utility/HardwareSpecific.h"
#include "utility/BufferedSerial.h"

#include "utility/YRShellInterpreter.h"

/** \brief YRShellBase - interactive
 
 Details on whatYRShellBase is
 
 
 */
template< unsigned DICTIONARY_SIZE, unsigned PAD_SIZE, unsigned TEXT_BUFER_SIZE, unsigned NUM_REGISTERS,
    unsigned PARAMETER_STACK_SIZE, unsigned RETURN_STACK_SIZE, unsigned COMPILE_STACK_SIZE,
    unsigned INQ_SIZE, unsigned OUTQ_SIZE, unsigned AUX_INQ_SIZE, unsigned AUX_OUTQ_SIZE,
    unsigned LAST_BUFFER_SIZE >
class YRShellBase : public virtual YRShellInterpreter{
protected:
    CurrentDictionary<DICTIONARY_SIZE> m_dictionaryCurrent;
    
    CircularQ<char, INQ_SIZE>       m_inq;
    CircularQ<char, AUX_INQ_SIZE>   m_auxInq;
    CircularQ<char, OUTQ_SIZE>      m_outq;
    CircularQ<char, AUX_OUTQ_SIZE>  m_auxOutq;
    
    char        m_pad[ PAD_SIZE];
    char        m_textBuffer[ TEXT_BUFER_SIZE];
#ifdef INPUT_BUFFER_EDITING
    char        m_lastBufferArray[ LAST_BUFFER_SIZE];
#endif
    uint32_t    m_registers[ NUM_REGISTERS];
    uint32_t    m_parameterStack[ PARAMETER_STACK_SIZE];
    uint32_t    m_returnStack[ RETURN_STACK_SIZE];
    uint32_t    m_compileStack[ COMPILE_STACK_SIZE];
    
public:
    YRShellBase( ) {
        m_DictionaryCurrent = &m_dictionaryCurrent;
        m_Inq = &m_inq;
        m_AuxInq = &m_auxInq;
        m_Outq = &m_outq;
        m_AuxOutq = &m_auxOutq;
        m_Pad = m_pad;
        m_TextBuffer = m_textBuffer;
        m_padSize = sizeof( m_pad);
        m_textBufferSize = sizeof( m_textBuffer);
        m_Registers = m_registers;
        m_numRegisters = sizeof(m_registers)/sizeof(m_registers[0]);
        m_ParameterStack = m_parameterStack;
        m_parameterStackSize = sizeof(m_parameterStack)/sizeof(m_parameterStack[0]);
        m_ReturnStack = m_returnStack;
        m_returnStackSize = sizeof(m_returnStack)/sizeof(m_returnStack[0]);
        m_CompileStack = m_compileStack;
        m_compileStackSize = sizeof(m_compileStack)/sizeof(m_compileStack[0]);
#ifdef INPUT_BUFFER_EDITING
        m_lastBufferSize = LAST_BUFFER_SIZE;
        m_LastBuffer = m_lastBufferArray;
#else
        m_lastBufferSize = 0;
#endif
    }
    virtual ~YRShellBase( ) { }
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRShellBase"; }
};


/** \brief YRShell - interactive
 
 Details on whatYRShell is
 
 
 */
class YRShell : public virtual YRShellBase<2048, 128, 128, 16, 16, 16, 8, 128, 256, 128, 256, 256>{
protected:
public:
    YRShell( ) { }
    virtual ~YRShell( ) { }
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRShell"; }
};

/** \brief  interactive
 
 Details on what YRSmallShell is
 
 
 */
class YRMidShell : public virtual YRShellBase<256, 128, 128, 16, 16, 16, 8, 64, 128, 64, 64, 256>{
protected:
public:
    YRMidShell( ) { }
    virtual ~YRMidShell( ) { }
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRMidShell"; }
};
/** \brief  interactive
 
 Details on what YRSmallShell is
 
 
 */
class YRSmallShell : public virtual YRShellBase<64, 64, 64, 16, 16, 16, 8, 32, 32, 32, 32, 0>{
protected:
public:
    YRSmallShell( ) { }
    virtual ~YRSmallShell( ) { }
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRSmallShell"; }
};

#endif

