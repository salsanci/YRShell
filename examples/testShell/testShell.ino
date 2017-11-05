
#include <YRShell.h>

bool ledControl;

typedef enum {
    SE_CC_first = YRSHELL_DICTIONARY_EXTENSION_FUNCTION,
    SE_CC_timesTwo,
    SE_CC_timesFour,
    SE_CC_ledControlShell,
    SE_CC_ledControlArduino,
    SE_CC_ledOn,
    SE_CC_ledOff,
    SE_CC_last
} SE_CC_functions;

static const FunctionEntry shellExtensionFunctions[] = {
    { SE_CC_timesTwo,       "2*"},
    { SE_CC_timesFour,      "4*"},
    { SE_CC_ledControlShell,    "ledControlShell" },
    { SE_CC_ledControlArduino,  "ledControlArduino" },
    { SE_CC_ledOn,              "ledOn" },
    { SE_CC_ledOff,             "ledOff" },
    { 0, NULL}
};
static FunctionDictionary dictionaryExtensionFunction( shellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );

CompiledDictionary compiledExtensionDictionary( NULL, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_EXTENSION_COMPILED);

class MyYRShell : public YRShell {
protected:
    virtual void executeFunction( uint16_t n);
    virtual const char* shellClass( void) { return "MyYRShell"; }
    virtual const char* mainFileName( ) { return "examples/testShell/testShell.ino"; }

public:
    MyYRShell();
    void init(void);
};

#ifdef YRSHELL_DEBUG
static const char *YRShellExtensionDebugStrings[] = {
    "SE_CC_first",
    "SE_CC_timesTwo",
    "SE_CC_timesFour",
    "SE_CC_ledControlShell",
    "SE_CC_ledControlArduino",
    "SE_CC_ledOn",
    "SE_CC_ledOff",
    "SE_CC_last",
};
#endif

MyYRShell::MyYRShell( ){
}
void MyYRShell::init() {
    YRShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &dictionaryExtensionFunction;
}

void MyYRShell::executeFunction( uint16_t n) {
    if( n <= SE_CC_first || n >= SE_CC_last) {
        YRShell::executeFunction(n);
    } else {
#ifdef YRSHELL_DEBUG
        if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
            outString("[");
            outString( YRShellExtensionDebugStrings[n - SE_CC_first]);
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
            case SE_CC_ledControlShell:
                ledControl = true;
                break;
            case SE_CC_ledControlArduino:
                ledControl = false;
                break;
            case SE_CC_ledOn:
                if( ledControl) {
                    digitalWrite(LED_BUILTIN, HIGH);
                }
                break;
            case SE_CC_ledOff:
                if( ledControl) {
                    digitalWrite(LED_BUILTIN, LOW);
                }
                break;
            default:
                shellERROR(__FILE__, __LINE__);
                break;
        }
    }
}






MyYRShell myShell;
IntervalTimer it;

void setup()
{
  BSerial.begin( 115200);
  pinMode(LED_BUILTIN, OUTPUT);
  it.setInterval( 0);
  BSerial.init( myShell.getInq(), myShell.getOutq());
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
      it.setInterval( 100);
    }
    state = !state;
  }
  Sliceable::sliceAll( );
}
