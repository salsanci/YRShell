
#include "testSupport.h"
#include <stdlib.h>
#include <stdio.h>

ShellRunner::ShellRunner( ) {
    m_commandTimeout = 1000;
    m_init = false;
}
bool ShellRunner::runCommand( const char* Setup,  const char *Command) {
    IntervalTimer timeout;
    char *P, *Lim, *Input, *FirstOutput = NULL, *SecondOutput = NULL;
    char buf[ 8192];
    bool inputDoneFlag = false;

    int c = 0;
    
    m_result = "";
    Input = buf;
    if( (strlen( Setup) +1 + strlen( Command) +1 + strlen(m_resultBegin) + 1 + strlen(m_resultEnd) + 1 ) < sizeof( buf)) {
        strcpy( buf, Setup);
        strcat( buf, "\rs' ");
        strcat( buf, m_resultBegin);
        strcat( buf, "' .str ");
        strcat( buf, Command);
        strcat( buf, " s' ");
        strcat( buf, m_resultEnd);
        strcat( buf, "' .str\r");
        
        for( int i = 0; i < 16; i++) {
            Sliceable::sliceAll();
        }
        if( !m_init) {
            m_init = true;
            while( m_shell.getOutq().valueAvailable()) {
                putchar( m_shell.getOutq().get());
                Sliceable::sliceAll();
            }
            putchar('\n');
            putchar('\n');
        }

        
        P = m_resultBuffer;
        Lim = m_resultBuffer + sizeof( m_resultBuffer) - 2;
        timeout.setInterval(m_commandTimeout);
    
        while( !timeout.hasIntervalElapsed()) {
            while( !inputDoneFlag && m_shell.getInq().spaceAvailable()) {
                if( *Input == '\0') {
                    inputDoneFlag = true;
                } else {
                    m_shell.getInq().put(*Input++);
                }
            }
            c++;
            Sliceable::sliceAll();
            while( (FirstOutput == NULL ||  SecondOutput == NULL)  && m_shell.getOutq().valueAvailable()) {
                *P = m_shell.getOutq().get();
                //putchar( *P);
                P++;
                *P = '\0';
            }
            if( P >= Lim) {
                break;
            }
            FirstOutput = strstr( m_resultBuffer, m_resultEnd);
            if( FirstOutput != NULL ) {
                SecondOutput = strstr( FirstOutput +1,  m_resultEnd);
                if( SecondOutput != NULL) {
                    *SecondOutput = '\0';
                    m_result = strstr( FirstOutput, m_resultBegin);
                    if( m_result != NULL) {
                        m_result += strlen( m_resultBegin);
                        break;
                    }
                }
            }
        }
    }
    return m_result != NULL;
}

int ShellRunner::getIntResult( ) {
    return atoi(m_result);
}
unsigned ShellRunner::getUnsignedResult( ) {
    return atoi(m_result);
}
float ShellRunner::getFloatResult( ) {
    return atof(m_result);
}
