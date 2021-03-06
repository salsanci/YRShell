
#include <YRShell.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

bool ledControl;

typedef enum {
    SE_CC_first = YRSHELL_DICTIONARY_EXTENSION_FUNCTION,
    SE_CC_ledControlShell,
    SE_CC_ledControlArduino,
    SE_CC_ledOn,
    SE_CC_ledOff,
    SE_CC_lcdReadButtons,
    SE_CC_lcdWrite,
    SE_CC_last
} SE_CC_functions;

static const FunctionEntry shellExtensionFunctions[] = {
    { SE_CC_ledControlShell,    "ledControlShell" },
    { SE_CC_ledControlArduino,  "ledControlArduino" },
    { SE_CC_ledOn,              "ledOn" },
    { SE_CC_ledOff,             "ledOff" },
    { SE_CC_lcdReadButtons,     "lcdReadButtons" },
    { SE_CC_lcdWrite,           "lcdWrite" },
    { 0, NULL}
};
static FunctionDictionary dictionaryExtensionFunction( shellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );


CompiledDictionary compiledExtensionDictionary( NULL, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_EXTENSION_COMPILED);


class MyYRShell : public YRShell {
protected:
    virtual void executeFunction( uint16_t n);

public:
    MyYRShell();
    void init(void);
};


MyYRShell::MyYRShell( ){
}
void MyYRShell::init() {
    YRShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &dictionaryExtensionFunction;
}
void MyYRShell::executeFunction( uint16_t n) {
    uint32_t lcdLine;
    char lcdBuf[ 17];
    if( n <= SE_CC_first || n >= SE_CC_last) {
        YRShell::executeFunction(n);
    } else {
        switch( n) {
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
            case SE_CC_lcdReadButtons:
                pushParameterStack( analogRead(0));
                break;
             case SE_CC_lcdWrite:
                lcdLine = popParameterStack() == 0 ? 0 : 1;
                lcd.setCursor(0,lcdLine);
                strncpy( lcdBuf, getAddressFromToken( popParameterStack()), 16 );
                lcdBuf[ 16] = '\0';
                for( uint8_t i = 0; i < 16; i++) {
                  if( lcdBuf == '\0') {
                    lcdBuf[ i] = ' ';
                  }
                }
                lcd.print( lcdBuf);
                break;
           default:
                shellERROR(__FILE__, __LINE__);
                break;
        }
    }
}


MyYRShell shell;
IntervalTimer it;

void setup()
{
  BSerial.begin(230400);
  pinMode(LED_BUILTIN, OUTPUT);
  it.setInterval( 0);
  BSerial.init( shell.getInq(), shell.getOutq());
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print( "WELCOME         ");
  lcd.setCursor(0, 1);
  lcd.print( "           READY");
 
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

/*

: lcdClear s'                 ' dup 0 lcdWrite 1 lcdWrite

: lcr4  75 > [  4 ][ 5 ]
: lcr3  dup 250 > [ drop 3 ][ lcr4 ]
: lcr2  dup 500 > [ drop 2 ][ lcr3 ]
: lcr1  dup 800 > [ drop 1 ][ lcr2 ]
: lcr lcdReadButtons dup 980 > [ drop 0 ][ lcr1 ]
// rb ( -- ) read lcd buttons
: rb 0 { lcr dup 0!= [ . cr ][ drop ] esc? } drop

: db3 16 spaces
: db1 0 0 c! textIO s'  value:' .str lcdReadButtons .w db3  mainIO 0 0 lcdWrite
: db2 s' button:' 0 str! textIO lcr .w db3 mainIO 0 1 lcdWrite 
// db ( -- ) read lcd buttons
: db { db1 db2 esc? }

// demo ( -- )
: _demo0 2dup c! 1 + swap 1 + swap
: _demo1 dup 0 { _demo0  dup 0x10 == } 2drop 0 16 c! 0 1 lcdWrite
: demo hex lcdClear 0 { 0 0 c! textIO dup .b mainIO 0 0 lcdWrite _demo1 3000 delay 0x10 + dup 0x100 & } drop

: _t0 micros 0 0 c! textIO . mainIO 0 0 lcdWrite
: _t1 millis 0 0 c! textIO . mainIO 0 1 lcdWrite
: t lcdClear millis { _t0 _t1 { 1 delay millis over - 1000 > } 1000 +  esc? }

: .nc .n 0x3A emit 
: _utd dup 86400000 / .nc
: _uth 86400000 % dup 3600000 / .nc
: _utm 3600000 % dup 60000 / .nc
: _uts  60000 % dup 1000 / .n
: uptime millis _utd _uth _utm _uts 0x2e emit 1000 % .n 

: _ut 0 0 c! textIO uptime mainIO 0 0 lcdWrite
: tt lcdClear millis { _ut { 1 delay millis over - 1000 > } 1000 +  esc? }


: b? lcr  [ 50 delay lcr ][ 0 ]
: ttt lcdClear millis { _ut { 1 delay millis over - 100 > } 100 + b? [ { b? } { b? 0== } { b? } { b? 0== } ] esc? }


*/