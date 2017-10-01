


#include "YRShell.h"

/*
 enum S_CC_functions {
 S_CC_first = YRSHELL_DICTIONARY_COMMON_FUNCTION,
 S_CC_zero,
 S_CC_plusOne,
 S_CC_minusOne,
 S_CC_last
 };
#ifdef YRSHELL_DEBUG
const char *YRShellDebugStrings[] = {
    "S_CC_first",
    "S_CC_zero",
    "S_CC_plusOne",
    "S_CC_minusOne",
    "S_CC_last",
};
#endif


static const FunctionEntry shellFunctions[] = {
    { (uint16_t)YRShellBase::S_CC_zero,        "zero" },
    { (uint16_t)YRShellBase::S_CC_plusOne,     "1+" },
    { (uint16_t)YRShellBase::S_CC_minusOne,    "1-" },
    { (uint16_t)0, NULL}
};
static FunctionDictionary dictionaryFunction( shellFunctions, YRSHELL_DICTIONARY_COMMON_FUNCTION );

void YRShellBase::init() {
    YRShellInterpreter::init( );
    //m_dictionaryList[ YRSHELL_DICTIONARY_COMMON_FUNCTION_INDEX] = &dictionaryFunction;
    
}
void YRShellBase::executeFunction( uint16_t n) {
    if( n <= S_CC_first || n >= S_CC_last) {
        YRShellInterpreter::executeFunction(n);
    } else {
#ifdef YRSHELL_DEBUG
        if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
            outString("[");
            outString(YRShellDebugStrings[n - S_CC_first]);
            outString("]");
        }
#endif
        switch( n) {
            case S_CC_zero:
                pushParameterStack(0);
                break;
            case S_CC_plusOne:
                pushParameterStack(popParameterStack()+1);
                break;
            case S_CC_minusOne:
                pushParameterStack(popParameterStack()-1);
                break;
            default:
                shellERROR( __BASE_FILE__, __LINE__);
                break;
        }
    }
}
*/
