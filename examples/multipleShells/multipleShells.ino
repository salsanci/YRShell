
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
    SE_CC_nextShell,
    SE_CC_last
} SE_CC_functions;

static const FunctionEntry shellExtensionFunctions[] = {
    { SE_CC_timesTwo,       "2*"},
    { SE_CC_timesFour,      "4*"},
    { SE_CC_ledControlShell,    "ledControlShell" },
    { SE_CC_ledControlArduino,  "ledControlArduino" },
    { SE_CC_ledOn,              "ledOn" },
    { SE_CC_ledOff,             "ledOff" },
    { SE_CC_nextShell,          ">>>" },
    { 0, NULL}
};
static FunctionDictionary dictionaryExtensionFunction( shellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );

static uint16_t compiledExtensionDictionaryData[] = {
};
CompiledDictionary compiledExtensionDictionary( compiledExtensionDictionaryData, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_EXTENSION_COMPILED);


class MyYRShell : public YRShell {
protected:
    virtual void executeFunction( uint16_t n);
    virtual uint16_t find( const char* name);

public:
    MyYRShell();
    void init(void);
};

MyYRShell shell1;
MyYRShell shell2;
MyYRShell *shellVector[] = {& shell1, &shell2};
uint8_t shellVectorIndex;
#define NUM_SHELLS (sizeof(shellVector)/sizeof( *shellVector))
MyYRShell *currentShell = shellVector[ shellVectorIndex];


#ifdef YRSHELL_DEBUG
static const char *ShellExtensionDebugStrings[] = {
    "SE_CC_first",
    "SE_CC_timesTwo",
    "SE_CC_timesFour",
    "SE_CC_ledControlShell",
    "SE_CC_ledControlArduino",
    "SE_CC_ledOn",
    "SE_CC_ledOff",
    "SE_CC_nextShell",
    "SE_CC_last",
};
#endif

MyYRShell::MyYRShell( ){
}

void MyYRShell::init() {
    YRShell::init();
    compiledExtensionDictionary.setInterpreter(this);
    dictionaryExtensionFunction.setInterpreter(this);
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &dictionaryExtensionFunction;
}


uint16_t MyYRShell::find( const char* name) {
    return YRShell::find( name);
}
void MyYRShell::executeFunction( uint16_t n) {
    if( n <= SE_CC_first || n >= SE_CC_last) {
        YRShell::executeFunction(n);
    } else {
#ifdef YRSHELL_DEBUG
        if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
            outString("[");
            outString( ShellExtensionDebugStrings[n - SE_CC_first]);
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
             case SE_CC_nextShell:
                if( ++shellVectorIndex >= NUM_SHELLS) {
                    shellVectorIndex = 0;
                }
                currentShell = shellVector[ shellVectorIndex];
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
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  it.setInterval( 0);
  shell1.init();
  shell2.init();
  shell1.setPrompt("\r\nHELO>");
  shell2.setPrompt("\r\nOK>");
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
  for( int i = 0; i < 32 && Serial.available(); i++) {
    if( currentShell->getInq().spaceAvailable()) {
      currentShell->getInq().put( Serial.read());
    }
  }
  for( int i = 0; i < YRSHELL_OUTQ_SIZE && currentShell->getOutq().valueAvailable(); i++) {
    Serial.write( currentShell->getOutq().get());
  }
  
  shell1.slice();
  shell2.slice();
}
