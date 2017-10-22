#ifndef TestSupport_h
#define TestSupport_h
#include "../YRShell.h"

class ShellRunner {
protected:
    YRShell m_shell;
    char m_resultBuffer[ 4096];
    const char* m_result;
    uint32_t m_commandTimeout;
    const char* m_resultBegin = "<RESULTBEGIN>";
    const char* m_resultEnd = "<RESULTEND>";
public:
    ShellRunner( );
    bool runCommand( const char* Setup, const char* Command);
    int getIntResult( void);
    unsigned getUnsignedResult( void);
    float getFloatResult( void);
    const char* getStringResult( void) { return m_result; };
    
    inline bool stacksEmpty( void) { return m_shell.stacksEmpty(); }
};
#endif

