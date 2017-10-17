
#include <YRShell.h>

bool ledControl;

class CommonShell : public virtual YRShellInterpreter {
public:
enum SC_CC_functions {
    SC_CC_first = YRSHELL_DICTIONARY_COMMON_FUNCTION,
    SC_CC_nextYRShell,
    SC_CC_last
};
protected:
    void executeFunction( uint16_t n);
 
public:
    CommonShell( void) {  }
    virtual ~CommonShell( void) { }
    void init(void);
};


class MyYRShell : public virtual YRShell, public virtual CommonShell {
public:
/** \brief Used by MyYRShell to map to the native functions.

Used by MyYRShell to map to the native functions.
*/
enum SE_CC_functions {
    SE_CC_first = YRSHELL_DICTIONARY_EXTENSION_FUNCTION,
    SE_CC_timesTwo,
    SE_CC_timesFour,
    SE_CC_last
};
protected:
    virtual void executeFunction( uint16_t n);
 
public:
    MyYRShell() { }
    void init(void);
    virtual const char* shellClass( void) { return "MyYRShell"; }
};

class SmallYRShell : public virtual YRShellBase<128, 64, 16, 16, 16, 8, 64, 64, 64, 64>, public virtual CommonShell {
public:
    SmallYRShell() { }
    virtual const char* shellClass( void) { return "SmallYRShell"; }
};

MyYRShell shell1;
SmallYRShell shell2;
MyYRShell shell3;
SmallYRShell shell4;

YRShellInterpreter *shellVector[] = {&shell1, &shell2, &shell3, &shell4};
uint8_t shellVectorIndex = 0;
#define NUM_SHELLS (sizeof(shellVector)/sizeof( *shellVector))
YRShellInterpreter *currentShell = shellVector[ shellVectorIndex];

static const FunctionEntry shellCommonFunctions[] = {
    { CommonShell::SC_CC_nextYRShell,      ">>>" },

    { 0, NULL}
};
static FunctionDictionary dictionaryCommonFunction( shellCommonFunctions, YRSHELL_DICTIONARY_COMMON_FUNCTION );
void CommonShell::init() {
    YRShellInterpreter::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_COMMON_FUNCTION_INDEX] = &dictionaryCommonFunction;
}
#ifdef YRSHELL_DEBUG
static const char *CommonShellDebugStrings[] = {
    "SC_CC_first",
    "SC_CC_nextYRShell",
    "SC_CC_last",
};
#endif
void CommonShell::executeFunction( uint16_t n ) {
    if( n <= SC_CC_first || n >= SC_CC_last) {
       YRShellInterpreter::executeFunction(n);
    } else {
#ifdef YRSHELL_DEBUG
        if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
            outString("[");
            outString(CommonShellDebugStrings[n - SC_CC_first]);
            outString("]");
        }
#endif
        switch( n) {
            case SC_CC_nextYRShell:
                if( ++shellVectorIndex >= NUM_SHELLS) {
                    shellVectorIndex = 0;
                }
                currentShell = shellVector[ shellVectorIndex];
                BSerial.init( currentShell->getInq(), currentShell->getOutq());

                break;
            default:
                shellERROR(__FILE__, __LINE__);
                break;
        }
    }
}




static const FunctionEntry myYRShellExtensionFunctions[] = {
    { MyYRShell::SE_CC_timesTwo,       "2*"},
    { MyYRShell::SE_CC_timesFour,      "4*"},
 
    { 0, NULL}
};
static FunctionDictionary myYRDictionaryExtensionFunction( myYRShellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );
CompiledDictionary compiledExtensionDictionary( NULL, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_EXTENSION_COMPILED);

#ifdef YRSHELL_DEBUG
static const char *MyYRShellExtensionDebugStrings[] = {
    "SE_CC_first",
    "SE_CC_timesTwo",
    "SE_CC_timesFour",
    "SE_CC_last",
};
#endif
void MyYRShell::init() {
    CommonShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &myYRDictionaryExtensionFunction;
}
void MyYRShell::executeFunction( uint16_t n) {
    if( n <= SE_CC_first || n >= SE_CC_last) {
       CommonShell::executeFunction(n);
    } else {
#ifdef YRSHELL_DEBUG
        if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
            outString("[");
            outString(MyYRShellExtensionDebugStrings[n - SE_CC_first]);
            outString("]");
        }
#endif
        switch( n) {
            case SE_CC_timesTwo:
                pushParameterStack(popParameterStack()*2);
                break;
            case SE_CC_timesFour:
                pushParameterStack(popParameterStack()*4);
                break;
            default:
                shellERROR(__FILE__, __LINE__);
                break;
        }
    }
}



IntervalTimer it;

void setup()
{
  BSerial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  it.setInterval( 0);
  shell1.init();
  shell2.init();
  shell3.init();
  shell4.init();
  shell1.setPrompt("s1>");
  shell2.setPrompt("s2>");
  shell3.setPrompt("s3>");
  shell4.setPrompt("s4>");
  BSerial.init( currentShell->getInq(), currentShell->getOutq());
}

void loop()
{
  static bool state = false;

  if( !ledControl && it.hasIntervalElapsed( ) ) {
    if( state) {
      digitalWrite(LED_BUILTIN, HIGH);
      it.setInterval( 500);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      it.setInterval( 1500);
    }
    state = !state;
  }
  Sliceable::sliceAll();
}
