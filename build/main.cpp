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

class MyYRShell : public YRShell {
public:
/** \brief Used by MyYRShell to map to the native functions.

Used by MyYRShell to map to the native functions.
*/
enum SE_CC_functions {
    SE_CC_first = YRSHELL_DICTIONARY_EXTENSION_FUNCTION,
    SE_CC_timesTwo,
    SE_CC_timesFour,
    SE_CC_nexYRShell,
    SE_CC_last
};
protected:
    virtual void executeFunction( uint16_t n);
    virtual uint16_t find( const char* name);

public:
    MyYRShell();
    void init(void);
};

static const FunctionEntry shellExtensionFunctions[] = {
    { MyYRShell::SE_CC_timesTwo,       "2*"},
    { MyYRShell::SE_CC_timesFour,      "4*"},
    { MyYRShell::SE_CC_nexYRShell,      ">>>" },

    { 0, NULL}
};
static FunctionDictionary dictionaryExtensionFunction( shellExtensionFunctions, YRSHELL_DICTIONARY_EXTENSION_FUNCTION );

CompiledDictionary compiledExtensionDictionary( NULL, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_EXTENSION_COMPILED);

MyYRShell shell1;
MyYRShell shell2;
MyYRShell *shellVector[] = {& shell1, &shell2};
uint8_t shellVectorIndex;
#define NUM_YRSHELLS (sizeof(shellVector)/sizeof( *shellVector))
MyYRShell *currenYRShell = shellVector[ shellVectorIndex];

#ifdef YRSHELL_DEBUG
static const char *YRShellExtensionDebugStrings[] = {
    "SE_CC_first",
    "SE_CC_timesTwo",
    "SE_CC_timesFour",
    "SE_CC_nexYRShell",
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
            outString(YRShellExtensionDebugStrings[n - SE_CC_first]);
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
            case SE_CC_nexYRShell:
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
            if( currenYRShell->getOutQ().valueAvailable()) {
                //std::cout << "<<";
                for(int i = 0; i < 256 && currenYRShell->getOutQ().valueAvailable(); i++) {
                    std::cout << currenYRShell->getOutQ().get();
                }
                //std::cout << ">>";
            }
        }
        if( currenYRShell->getAuxOutQ().valueAvailable()) {
            //std::cout << "<<";
            std::cout << "\r\n[AUX\r\n";
            for(int i = 0; i < 256 && currenYRShell->getAuxOutQ().valueAvailable(); i++) {
                std::cout << currenYRShell->getAuxOutQ().get();
            }
            //std::cout << ">>";
            std::cout << "\r\n]\r\n";
        }
    }
    return 0;
}
