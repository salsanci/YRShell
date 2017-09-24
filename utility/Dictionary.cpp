#include "YRShellInterpreter.h"
#include "Dictionary.h"

Dictionary::Dictionary( ) {
    m_mask = 0;
    M_interpreter = NULL;
}
Dictionary::~Dictionary( ) {
    yrshellERROR(__FILE__, __LINE__);
}
uint16_t Dictionary::getFirstEntry( ) {
    yrshellERROR(__FILE__, __LINE__);
    return YRSHELL_DICTIONARY_INVALID;
}
uint16_t Dictionary::getNextEntry( uint16_t index){
    yrshellERROR(__FILE__, __LINE__);
    return YRSHELL_DICTIONARY_INVALID;
}
uint16_t Dictionary::getToken( uint16_t index){
    yrshellERROR(__FILE__, __LINE__);
    return YRSHELL_DICTIONARY_INVALID;
}
uint16_t Dictionary::getNameAddressToken( uint16_t index){
    yrshellERROR(__FILE__, __LINE__);
    return YRSHELL_DICTIONARY_INVALID;
}
uint16_t Dictionary::getWord( uint16_t index) {
    yrshellERROR(__FILE__, __LINE__);
    return YRSHELL_DICTIONARY_INVALID;
}
const char* Dictionary::getAddress( uint16_t index){
    yrshellERROR(__FILE__, __LINE__);
    return NULL;
}
uint16_t Dictionary::find( const char* name) {
    yrshellERROR(__FILE__, __LINE__);
    return YRSHELL_DICTIONARY_INVALID;
}
void Dictionary::yrshellERROR( const char* file, unsigned line) {
    if( M_interpreter != NULL) {
        M_interpreter->shellERROR(file, line);
    }
}
void Dictionary::yrshellERROR( const char* file, unsigned line, const char* message) {
    if( M_interpreter != NULL) {
        M_interpreter->shellERROR(file, line, message);
    }
}
void Dictionary::setInterpreter( YRShellInterpreter* shell) {
    M_interpreter = shell;
}



FunctionDictionary::FunctionDictionary( const FunctionEntry* dict, uint16_t mask) {
    M_dictionary = dict;
    m_mask = mask;
    for(  m_size = 0; dict->isValid() && m_size <= ((uint16_t) YRSHELL_DICTIONARY_MAX_ADDRESS); dict++, m_size++) {
    }
}
uint16_t FunctionDictionary::find( const char* name) {
    uint16_t rc = YRSHELL_DICTIONARY_INVALID;
    for( const FunctionEntry *S = &M_dictionary[ m_size-1]; S >= M_dictionary; S-- ) {
        if( S->isMatch(name)) {
            rc = S->getFunctionNumber();
            rc |= m_mask;
            break;
        }
    }
    return rc;
}

const char* FunctionDictionary::getAddress( uint16_t index) {
    return index < m_size ? (M_dictionary + index)->getName() : NULL;
}

uint16_t FunctionDictionary::getFirstEntry( void) {
    return m_size - 1;
}
uint16_t FunctionDictionary::getNextEntry( uint16_t index){
    return index - 1;
}
uint16_t FunctionDictionary::getToken( uint16_t index){
    return index < m_size ? m_mask | (M_dictionary+index)->getFunctionNumber() : YRSHELL_DICTIONARY_INVALID;
}
uint16_t FunctionDictionary::getNameAddressToken( uint16_t index) {
    return index < m_size ? m_mask | index : YRSHELL_DICTIONARY_INVALID;
}



CompiledDictionary::CompiledDictionary( ) {
    m_dictionary = NULL;
    m_lastWord = YRSHELL_DICTIONARY_INVALID;
    m_size = 0;
    m_mask = 0;
}
CompiledDictionary::CompiledDictionary( uint16_t* dict, uint16_t lastWord, uint16_t size, uint16_t mask) {
    m_dictionary = dict;
    m_lastWord = lastWord;
    m_size = size;
    m_mask = mask;
}

uint16_t CompiledDictionary::findInternal( const char* name, uint16_t link) {
    uint16_t rc = YRSHELL_DICTIONARY_INVALID;
    const char* P;
    while( link != YRSHELL_DICTIONARY_INVALID) {
        P = (const char*) &m_dictionary[ link +1];
        if( strcmp( P, name)) {
            link = m_dictionary[ link];
        } else {
            rc = m_mask | (link + 1 + Dictionary::nameLength(name));
            break;
        }
    }
    return rc;
}
uint16_t CompiledDictionary::find( const char* name) {
    uint16_t rc = YRSHELL_DICTIONARY_INVALID;
    if( m_lastWord != YRSHELL_DICTIONARY_INVALID) {
        uint16_t link =  m_lastWord;
        rc = findInternal( name, link);
    }
    return rc;
}
uint16_t CompiledDictionary::getFirstEntry( void) {
    return m_lastWord;
}
uint16_t CompiledDictionary::getNextEntry( uint16_t index){
    return getWord( index);
}
uint16_t CompiledDictionary::getToken( uint16_t index){
    return m_mask | (index + 1 + Dictionary::nameLength((const char*) &m_dictionary[ index + 1]));
}
uint16_t CompiledDictionary::getNameAddressToken( uint16_t index) {
    return m_mask |  (index + 1);
}
uint16_t CompiledDictionary::getWord( uint16_t index) {
    return index < m_size ? m_dictionary[ index] : YRSHELL_DICTIONARY_INVALID;
}
const char* CompiledDictionary::getAddress( uint16_t index) {
    return index < m_size ? (const char*) &m_dictionary[ index] : NULL;
}

uint16_t CompiledDictionary::getWordEnd( void) {
    return m_size;
}




/*
 DICTIONARY ENTRY
 uint16_t m_linkAddress;     A link to the previous entry, 0 is the last entry
 char m_name[ xxx];          this is really a string padded with '\0' to an even byte length - always a whole number of uint16_t
 uint16_t m_code[xxx]        this is the interpreter code for the word
 */
CurrentVariableDictionary::CurrentVariableDictionary() {
    m_mask = 0;
    m_size = 0;
    m_dictionary = NULL;
    m_dictionaryBackupWordEnd = 0;
    m_dictionaryBackupLastWord = 0;
    m_dictionaryCurrentWordEnd = 0;
    m_lastWord = YRSHELL_DICTIONARY_INVALID;
    
}
CurrentVariableDictionary::CurrentVariableDictionary( uint16_t* dict, uint16_t size)  {
    m_mask = YRSHELL_DICTIONARY_CURRENT;
    m_size = size;
    m_dictionary = dict;
    m_dictionaryBackupWordEnd = 0;
    m_dictionaryBackupLastWord = 0;
    m_dictionaryCurrentWordEnd = 0;
    m_lastWord = YRSHELL_DICTIONARY_INVALID;
}


bool CurrentVariableDictionary::setToken( uint16_t address, uint16_t token){
    bool rc = false;
    if( address < m_dictionaryCurrentWordEnd ) {
        m_dictionary[ address] = token;
        rc = true;
    }
    return rc;
}
bool CurrentVariableDictionary::addToken( uint16_t token) {
    bool rc = false;
    if( m_dictionaryCurrentWordEnd < (m_size - 2)) {
        m_dictionary[ m_dictionaryCurrentWordEnd++] = token;
        m_dictionary[ m_dictionaryCurrentWordEnd] = YRShellInterpreter::SI_CC_return;
        rc = true;
    }
    return rc;
}
bool CurrentVariableDictionary::newCompile( const char* name) {
    uint16_t len = Dictionary::nameLength(name);
    bool rc = false;
    if( m_dictionaryCurrentWordEnd < (m_size - 1 - len - 2) ) {
        m_dictionaryBackupWordEnd = m_dictionaryCurrentWordEnd;
        m_dictionaryBackupLastWord = m_lastWord;
        m_dictionary[ m_dictionaryCurrentWordEnd] = m_lastWord;
        m_lastWord = m_dictionaryCurrentWordEnd;
        m_dictionaryCurrentWordEnd++;
        m_dictionary[ m_dictionaryCurrentWordEnd + len - 1] = 0;
        strcpy( (char*) &m_dictionary[ m_dictionaryCurrentWordEnd], name);
        m_dictionaryCurrentWordEnd += len;
        m_dictionary[ m_dictionaryCurrentWordEnd] = YRShellInterpreter::SI_CC_return;
        rc = true;
    }
    return rc;
}

void CurrentVariableDictionary::rollBack() {
    m_dictionaryCurrentWordEnd = m_dictionaryBackupWordEnd;
    m_lastWord = m_dictionaryBackupLastWord;
}
void CurrentVariableDictionary::newCompileDone() {
    m_dictionaryBackupWordEnd = m_dictionaryCurrentWordEnd;
    m_dictionaryBackupLastWord = m_lastWord;
}
uint16_t CurrentVariableDictionary::find( const char* name) {
    uint16_t rc = YRSHELL_DICTIONARY_INVALID;
    if( m_lastWord != YRSHELL_DICTIONARY_INVALID) {
        uint16_t link = m_dictionary[ m_lastWord];
        rc = findInternal( name, link);
    }
    return rc;
}
uint16_t CurrentVariableDictionary::getFirstEntry( ) {
    return m_dictionaryBackupLastWord;
}
uint16_t CurrentVariableDictionary::getWord( uint16_t index) {
    return index < m_dictionaryCurrentWordEnd ? m_dictionary[ index] : YRSHELL_DICTIONARY_INVALID;
}
const char* CurrentVariableDictionary::getAddress( uint16_t index) {
    return index < m_dictionaryCurrentWordEnd ? (const char*) &m_dictionary[ index] : NULL;
}

CurrentDictionary::CurrentDictionary( )  {
    m_mask = YRSHELL_DICTIONARY_CURRENT;
    m_size = YRSHELL_DICTIONARY_SIZE;
    m_dictionary = m_dictionaryBuffer;
    m_dictionaryBackupWordEnd = 0;
    m_dictionaryBackupLastWord = 0;
    m_dictionaryCurrentWordEnd = 0;
    m_lastWord = YRSHELL_DICTIONARY_INVALID;
}



