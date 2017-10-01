#ifndef YRShellInterpreter_h
#define YRShellInterpreter_h

#ifdef NOTARDUINO_AC6
#include "processorGlobal.h"
#endif

#include "CircularQ.h"
#include "IntervalTimer.h"
#include "Dictionary.h"

#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
#include "math.h"
#endif


/** \brief YRShellInterpreter - interactive
 
 Details on what YRShellInterpreter is
 
 */
class YRShellInterpreter {
public:
/** \brief Used by YRShellInterpreter to map to the native functions.

Used by YRShellInterpreter to map to the native functions.
*/
enum SI_CC_functions {
    SI_CC_first = YRSHELL_DICTIONARY_INTERPRETER_FUNCTION, /**< Used to provide a boundary only. */

    SI_CC_return, /**< Returns from an interpreted word to the interprted word which called it. */

    SI_CC_space, /**< Write a space to the current output queue. */
    
    SI_CC_cr,
    SI_CC_lf,
    SI_CC_crlf,
    SI_CC_reset,
    SI_CC_prompt,
    SI_CC_clearPad,
    SI_CC_uint16,
    SI_CC_uint32,
    SI_CC_nint16,
    SI_CC_dot,
    SI_CC_dotByte,
    SI_CC_dotWord,
    SI_CC_dotX,
    SI_CC_dotByteX,
    SI_CC_dotWordX,
    SI_CC_dotStr,
    SI_CC_hex,
    SI_CC_decimal,
    SI_CC_stringDef,
    SI_CC_string,
    SI_CC_if,
    SI_CC_else,
    SI_CC_then,
    SI_CC_begin,
    SI_CC_until,
    SI_CC_jmp,
    SI_CC_jmpz,
    SI_CC_dup,
    SI_CC_swap,
    SI_CC_nip,
    SI_CC_tuck,
    SI_CC_drop,
    SI_CC_rot,
    SI_CC_toR,
    SI_CC_fromR,
    SI_CC_negate,
    SI_CC_plus,
    SI_CC_minus,
    SI_CC_multiply,
    SI_CC_divide,
    SI_CC_mod,
    SI_CC_and,
    SI_CC_or,
    SI_CC_xor,
    SI_CC_lessThan,
    SI_CC_lessEqual,
    SI_CC_equal,
    SI_CC_greaterThan,
    SI_CC_greaterThanEqual,
    SI_CC_setDebug,
    SI_CC_toC,
    SI_CC_fromC,
    SI_CC_parameterStackDepth,
    SI_CC_returnStackDepth,
    SI_CC_compileStackDepth,
    SI_CC_parameterStackAt,
    SI_CC_returnStackAt,
    SI_CC_compileStackAt,
    SI_CC_notEqual,
    SI_CC_zeroEqual,
    SI_CC_zeroNotEqual,
    SI_CC_over,
    SI_CC_2dup,
    SI_CC_2drop,
    SI_CC_strlen,
    SI_CC_getCurrentDictionary,
    SI_CC_getCurrentDictionaryEnd,
    SI_CC_getCurrentDictionaryLastWord,

    SI_CC_delay,
    SI_CC_nextEntry,
    SI_CC_dotEntryName,
    SI_CC_entryToken,
    
    SI_CC_keyQ,
    SI_CC_auxKeyQ,

    SI_CC_auxIO,
    SI_CC_mainIO,
    SI_CC_emit,
    SI_CC_auxEmit,
    
    SI_CC_shellSize,
    SI_CC_printShellClass,
    
    SI_CC_dictionarySize,
    SI_CC_padSize,
    SI_CC_numRegisters,
    SI_CC_parameterStackSize,
    SI_CC_returnStackSize,
    SI_CC_compileStackSize,
    SI_CC_inqSize,
    SI_CC_auxInqSize,
    SI_CC_outqSize,
    SI_CC_auxOutqSize,

    SI_CC_dictionaryClear,

#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
    SI_CC_dotf,
    SI_CC_dote,
    SI_CC_fLessThan,
    SI_CC_fLessEqual,
    SI_CC_fEqual,
    SI_CC_fGreaterThan,
    SI_CC_fGreaterThanEqual,
    
    SI_CC_fPlus,
    SI_CC_fMinus,
    SI_CC_fMultiply,
    SI_CC_fDivide,
    SI_CC_fPI,
    SI_CC_fSin,
    SI_CC_fCos,
    SI_CC_fTan,
    SI_CC_faSin,
    SI_CC_faCos,
    SI_CC_faTan,
    SI_CC_fExp,
    SI_CC_fLog,
    SI_CC_fPow,
    SI_CC_fLog10,
    SI_CC_fRound,
    SI_CC_floatToInt,
    SI_CC_intToFloat,
#endif
    SI_CC_last
};

protected:
    YRShellState                            m_lastState, m_state;

    Dictionary *m_dictionaryList[ YRSHELL_DICTIONARY_LAST_INDEX];

    CircularQBase<char>                     *m_Inq, *m_AuxInq, *m_Outq, *m_AuxOutq;
    
#ifdef YRSHELL_DEBUG
    unsigned    m_debugFlags;
#endif
    bool        m_hexMode;
    bool        m_useAuxQueues;

    uint32_t    m_outputTimeoutInMilliseconds;
    IntervalTimer m_outputTimeout;
    IntervalTimer m_delayTimer;
    
    char        *m_Pad;
    uint16_t    m_padCount;
    uint16_t    m_padSize;

    uint32_t*   m_Registers;
    uint16_t    m_numRegisters;

    uint32_t    *m_ParameterStack, *m_ReturnStack, *m_CompileStack;
    uint8_t     m_parameterStackSize, m_returnStackSize, m_compileStackSize;
    
    uint8_t     m_topOfStack;
    uint8_t     m_returnTopOfStack;
    uint8_t     m_compileTopOfStack;
    
    char *m_token, *m_saveptr;
    uint32_t m_PC;
    const char* m_prompt = "\r\n>";
    
    CurrentVariableDictionary* m_DictionaryCurrent;
    
#ifdef YRSHELL_DEBUG
    void debugToken( void);
#endif
    virtual uint16_t find( const char* name);
    
    virtual bool isCompileToken( void);
    
    virtual void executeFunction( uint16_t n);
    virtual uint32_t shellSize( void) { return sizeof( *this); }
    virtual const char* shellClass( void) { return "YRShellInterpreter"; }
    
    void nextState(YRShellState n);
    void fillPad( char c);
    void interpretReset( void);
    void reset( void);
    void reset( const char* file, unsigned line);
    void reset( const char* file, unsigned line, const char* message);
    
    uint32_t popParameterStack( void);
    uint32_t popReturnStack( void);
    uint32_t popCompileStack( void);
    
    void pushParameterStack( uint32_t v);
    void pushReturnStack( uint32_t v);
    void pushCompileStack( uint32_t v);
#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
    float popFloat( void);
    void  pushFloat( float f);
#endif

    
    uint16_t getAbsoluteAddressToken( void);
    uint16_t fetchCurrentValueToken( void);
    const char* getAddressFromToken( uint16_t token);
    uint16_t fetchCurrentToken( void);
    void executeToken( uint16_t token);
    void beginParsing( void);
    void processToken( void);
    bool processLiteralToken( void);
    void executing( void);
    
    
    void CC_clearPad( void);
    void CC_stringDef( void);
    void CC_if( void);
    void CC_else( void);
    void CC_then( void);
    void CC_begin( void);
    void CC_until( void);
    void CC_nextEntry( void);

public:
    YRShellInterpreter( );
    virtual ~YRShellInterpreter();
    void init( void);
    
    virtual void shellERROR( const char* file, unsigned line);
    virtual void shellERROR( const char* file, unsigned line, const char* message);
    
    void setPrompt( const char* prompt );
    
    void slice( void);
    CircularQBase<char>& getInq( void);
    CircularQBase<char>& getAuxInq( void);
    CircularQBase<char>& getOutq(void);
    CircularQBase<char>& getAuxOutq(void);
    
    char charToHex( char c);
    void intToString(int32_t n, uint8_t numDigits, char *s);
    void unsignedToStringZero(uint32_t num, uint8_t numDigits, char *s);
    void unsignedToString(uint32_t num, uint8_t numDigits, char *s);
    void unsignedToStringX(uint32_t num, uint8_t numDigits, char *s);
    const char* stringToUnsignedInternal( const char* P, uint32_t* V);
    bool stringToUnsigned( const char* P, uint32_t* V);
    bool stringToUnsignedX( const char* P, uint32_t* V);
#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
    bool stringToFloat( const char* P, uint32_t* V);
    void floatToString( float num, char *s);
    void floatEngToString( float num, char *s);
    void outFloat( float num);
    void outEngFloat( float num);
#endif
    
    void outChar( char c);
    void outString( const char* S);

    void outUint8( uint8_t v);
    void outUint16( uint16_t v);
    void outUint32( uint32_t v);
    void outUint8X( uint8_t v);
    void outUint16X( uint16_t v);
    void outUint32X( uint32_t v);
    void outInt8( int8_t v);
    void outInt16( int16_t v);
    void outInt32( int32_t v);
    void outInt8X( int8_t v);
    void outInt16X( int16_t v);
    void outInt32X( int32_t v);
    
    inline bool stacksEmpty( void) { return m_topOfStack == '\0' &&  m_compileTopOfStack == '\0'; }
};


#endif

