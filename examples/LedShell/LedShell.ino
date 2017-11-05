
#include <YRShell.h>

bool ledShellControl;

class LedShell : public YRShell {
public:
	enum SE_CC_functions {
	    SE_CC_first = YRSHELL_DICTIONARY_EXTENSION_FUNCTION,
	    SE_CC_setLedControl,
	    SE_CC_setLedState,
	    SE_CC_last
	};
protected:
    virtual void executeFunction( uint16_t n);
	virtual const char* shellClass( void) { return "LedShell"; }
    virtual const char* mainFileName( ) { return "examples/LedShell/LedShell.ino"; }
public:
	LedShell() {}
	virtual ~LedShell() {}
	virtual void init( void);

};
static const FunctionEntry ledShellFunctions[] = {
    { LedShell::SE_CC_setLedControl,      "setLedControl" },
    { LedShell::SE_CC_setLedState,        "setLedState" },
    { 0, NULL}
};
static FunctionDictionary ledDictionaryExtensionFunction( ledShellFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );

void LedShell::init() {
    YRShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = NULL;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &ledDictionaryExtensionFunction;
}

void LedShell::executeFunction( uint16_t n) {
    if( n <= SE_CC_first || n >= SE_CC_last) {
       YRShell::executeFunction(n);
    } else {
        switch( n) {
            case SE_CC_setLedControl:
            	ledShellControl = popParameterStack();
                 break;
            case SE_CC_setLedState:
                if( popParameterStack()) {
					digitalWrite(LED_BUILTIN, HIGH);
               	} else {
					digitalWrite(LED_BUILTIN, LOW);
               	}
                break;
            default:
                shellERROR(__FILE__, __LINE__);
                break;
        }
    }
}



LedShell shell;
IntervalTimer it;

void setup()
{
  BSerial.begin( 115200);
  BSerial.init( shell.getInq(), shell.getOutq());
  pinMode(LED_BUILTIN, OUTPUT);
  it.setInterval( 0);
}

void loop()
{
  static bool state = false;

  if( !ledShellControl && it.hasIntervalElapsed( ) ) {
    if( state) {
      digitalWrite(LED_BUILTIN, HIGH);
      it.setInterval( 100);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      it.setInterval( 100);
    }
    state = !state;
  }
  Sliceable::sliceAll( );
}
