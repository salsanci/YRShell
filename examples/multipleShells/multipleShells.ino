
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
    virtual const char* mainFileName( ) { return "examples/multipleShells/multipleShells.ino"; }
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

class LittleYRShell : public virtual YRShellBase<64, 64, 16, 16, 16, 16, 8, 64, 64, 64, 64, 0>, public virtual CommonShell {
public:
    LittleYRShell() { }
    virtual const char* shellClass( void) { return "LittleYRShell"; }
};

MyYRShell shell1;
LittleYRShell shell2;
MyYRShell shell3;
LittleYRShell shell4;

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
void CommonShell::executeFunction( uint16_t n ) {
    if( n <= SC_CC_first || n >= SC_CC_last) {
       YRShellInterpreter::executeFunction(n);
    } else {
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

void MyYRShell::init() {
    CommonShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &myYRDictionaryExtensionFunction;
}
void MyYRShell::executeFunction( uint16_t n) {
    if( n <= SE_CC_first || n >= SE_CC_last) {
       CommonShell::executeFunction(n);
    } else {
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

