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

CircularQ<char, 4000> keyQ;

IntervalTimer iTimer;

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


MyYRShell shell1;
SmallYRShell shell2;

YRShellInterpreter *shellVector[] = {& shell1, &shell2};
uint8_t shellVectorIndex;
#define NUM_YRSHELLS (sizeof(shellVector)/sizeof( *shellVector))
YRShellInterpreter *currenYRShell = shellVector[ shellVectorIndex];


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
                currenYRShell = shellVector[ shellVectorIndex];
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
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = &dictionaryExtensionFunction;
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
                shellERROR(__BASE_FILE__, __LINE__);
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
                usleep( 1000);
            }
            keyQ.put( (char) c);
        }
        
    }
    return NULL;
}

class StdoutQ {
public:
    StdoutQ( ) { }
    virtual ~StdoutQ( void) { }
    void slice( CircularQBase<char>& q);
};
void StdoutQ::slice(CircularQBase<char>& q) {
    if( q.valueAvailable()) {
        for(int i = 0; i < 256 && q.valueAvailable(); i++) {
            std::cout << q.get();
        }
    }
}


StdoutQ out1;
StdoutQ out2;

int main(int argc, const char * argv[]) {
    pthread_t kThread;
    int slowDownFactor, slowDownCounter;
    
    if(pthread_create(&kThread, NULL, kscan, (void*) NULL)) {
        
        fprintf(stderr, "Error creating thread\n");
        return 1;
        
    }
    shell1.init();
    shell2.init();
    shell1.setPrompt("\r\nHELO>");
    shell2.setPrompt("\r\nOK>");
    iTimer.setInterval(10);
    
    std::cout.setf( std::ios_base::unitbuf );
    
    slowDownFactor = 1;
    slowDownCounter = 0;
    while( 1) {
        if( iTimer.hasIntervalElapsed()) {
            for(int i = 0; i < 32 && keyQ.valueAvailable() && currenYRShell->getInq().spaceAvailable(); i++) {
                currenYRShell->getInq().put( keyQ.get());
            }
            iTimer.setInterval(10);
        }
        shell1.slice();
        shell2.slice();
        usleep( 10);
        if( slowDownCounter++ > slowDownFactor) {
            slowDownCounter = 0;
            out1.slice( currenYRShell->getOutq());
            out2.slice(currenYRShell->getAuxOutq());
        }
    }
    return 0;
}
