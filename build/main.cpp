//
//  main.cpp
//
//  Created by Sal Sanci on 2017-07-31.
//  Copyright © 2017 Sal Sanci. All rights reserved.
//


#include <iostream>
#include <string>
#include "../YRShell.h"
#include <pthread.h>
#include "unistd.h"

CircularQ<char, 16000> keyQ;

class StdoutQ : public Sliceable {
private:
    CircularQBase<char>* m_q;
public:
    StdoutQ( ) { }
    virtual ~StdoutQ( void) { }
    void init( CircularQBase<char>& q);
    void slice( void);
    virtual const char* sliceName( void) { return "StdoutQ"; }
};
void StdoutQ::init(CircularQBase<char>& q) {
    m_q = &q;
}
void StdoutQ::slice() {
    if( m_q->valueAvailable()) {
        for(int i = 0; i < 256 && m_q->valueAvailable(); i++) {
            std::cout << m_q->get();
        }
    }
}

StdoutQ out1;
StdoutQ out2;

IntervalTimer iTimer;
class derror : public DictionaryError {
public:
    derror() { }
    virtual void shellERROR( const char* name, unsigned line);
};
void derror::shellERROR( const char* name, unsigned line) {
    fprintf(stdout, "Dictionary ERROR [%s][%u]\n", name, line);
}
derror errd;

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

class SmallYRShell : public virtual YRShellBase<128, 64, 16, 16, 16, 8, 64, 256, 64, 64>, public virtual CommonShell {
public:
    /** \brief Used by SmallYRShell to map to the native functions.
     
     Used by SmallYRShell to map to the native functions.
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
    SmallYRShell() { }
    void init(void);
    virtual const char* shellClass( void) { return "SmallYRShell"; }
};

class BigYRShell : public virtual YRShellBase<8192, 256, 16, 16, 16, 8, 16384, 16384, 4096, 4096>, public virtual CommonShell {
public:
    /** \brief Used by SmallYRShell to map to the native functions.
     
     Used by SmallYRShell to map to the native functions.
     */
    enum SE_CC_functions {
        SE_CC_first = YRSHELL_DICTIONARY_EXTENSION_FUNCTION,
        SE_CC_ledControlShell,
        SE_CC_ledControlArduino,
        SE_CC_ledOn,
        SE_CC_ledOff,
        SE_CC_setBaud,
        SE_CC_last
    };
protected:
    virtual void executeFunction( uint16_t n);
    
public:
    BigYRShell() { }
    void init(void);
    virtual const char* shellClass( void) { return "BigYRShell"; }
};

BigYRShell shell0;
MyYRShell shell1;
SmallYRShell shell2;

YRShellInterpreter *shellVector[] = { &shell0, & shell1, &shell2};
uint8_t shellVectorIndex;
#define NUM_YRSHELLS (sizeof(shellVector)/sizeof( *shellVector))
YRShellInterpreter *currentYRShell = shellVector[ shellVectorIndex];

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
                if( ++shellVectorIndex >= NUM_YRSHELLS) {
                    shellVectorIndex = 0;
                }
                currentYRShell = shellVector[ shellVectorIndex];
                out1.init( currentYRShell->getOutq());
                out2.init(currentYRShell->getAuxOutq());
                break;
            default:
                shellERROR(__BASE_FILE__, __LINE__);
                break;
        }
    }
}

static const FunctionEntry shellExtensionFunctions[] = {
    { MyYRShell::SE_CC_timesTwo,       "2*"},
    { MyYRShell::SE_CC_timesFour,      "4*"},
 
    { 0, NULL}
};
static FunctionDictionary dictionaryExtensionFunction( shellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );
CompiledDictionary compiledExtensionDictionary( NULL, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_EXTENSION_COMPILED);

void MyYRShell::init() {
    CommonShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &dictionaryExtensionFunction;
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
                shellERROR(__BASE_FILE__, __LINE__);
                break;
        }
    }
}

void SmallYRShell::init() {
    CommonShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &dictionaryExtensionFunction;
}
void SmallYRShell::executeFunction( uint16_t n) {
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
                shellERROR(__BASE_FILE__, __LINE__);
                break;
        }
    }
}

static const FunctionEntry bigShellExtensionFunctions[] = {
    { BigYRShell::SE_CC_ledControlShell,    "ledControlShell" },
    { BigYRShell::SE_CC_ledControlArduino,  "ledControlArduino" },
    { BigYRShell::SE_CC_ledOn,              "ledOn" },
    { BigYRShell::SE_CC_ledOff,             "ledOff" },
    { BigYRShell::SE_CC_setBaud,            "setBaud" },
    { 0, NULL}
};
static FunctionDictionary bigShellDictionaryExtensionFunction( bigShellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );

void BigYRShell::init() {
    CommonShell::init();
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = &compiledExtensionDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &bigShellDictionaryExtensionFunction;
}
void BigYRShell::executeFunction( uint16_t n) {
    uint32_t v1, v2;
    static bool ledControl;
    if( n <= SE_CC_first || n >= SE_CC_last) {
        CommonShell::executeFunction(n);
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
#ifdef PLATFORM_AC6
                    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
#endif
#ifdef PLATFORM_ARDUINO
                    digitalWrite(LED_BUILTIN, HIGH);
#endif
                }
                break;
            case SE_CC_ledOff:
                if( ledControl) {
#ifdef PLATFORM_AC6
                    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
#endif
#ifdef PLATFORM_ARDUINO
                    digitalWrite(LED_BUILTIN, LOW);
#endif
                }
                break;
            case SE_CC_setBaud:
                v1 = popParameterStack();
                v2 = popParameterStack();
#ifdef PLATFORM_AC6
                STMSerial::setUartBaudRate( v1, v2);
#endif
#ifdef PLATFORM_ARDUINO
                //TODO FIXME
#endif
                break;
            default:
                shellERROR(__FILE__, __LINE__);
                break;
        }
    }
}




void *kscan(void *x_void_ptr) {
    int c;
    while( 1) {
        c = getchar();
        if( c != EOF) {
            while( !keyQ.spaceAvailable()) {
                usleep(1000);
            }
            keyQ.put( (char) c);
        }
    }
    return NULL;
}



int main(int argc, const char * argv[]) {
    char c;
    pthread_t kThread;
    uint16_t lastLen= 0;

    std::cout.setf( std::ios_base::unitbuf );

    if(pthread_create(&kThread, NULL, kscan, (void*) NULL)) {
        
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    Dictionary::s_DictionaryError =  &errd;

    shell0.setPrompt("bShell>");
    shell1.setPrompt("HELO>");
    shell2.setPrompt("OK>");
    iTimer.setInterval(1);
    out1.init( currentYRShell->getOutq());
    out2.init(currentYRShell->getAuxOutq());

    std::cout.setf( std::ios_base::unitbuf );
    while( 1) {
        if( iTimer.hasIntervalElapsed()) {
            for(int i = 0; i < 32 && keyQ.valueAvailable() && currentYRShell->getInq().spaceAvailable(); i++) {
                c = keyQ.get();
                currentYRShell->getInq().put( c);
            }
            iTimer.setInterval(1);
        }
        if( lastLen == keyQ.used()) {
            Sliceable::sliceAll( );
            lastLen = keyQ.used();
        } else {
            lastLen = keyQ.used();
            usleep( 5000);
        }

        usleep( 1);
    }
    return 0;
}
