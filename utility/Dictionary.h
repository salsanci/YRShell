#ifndef Dictionary_h
#define Dictionary_h

#ifdef NOTARDUINO_AC6
#include "processorGlobal.h"
#endif


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "YRShellDefines.h"

class YRShellInterpreter;

/** \brief Base class for all the dictionaries used by the interpreter
 
This class is not meant to be instatiated. 
*/
class Dictionary {
protected:
    uint16_t                m_mask;             /**< The mask applied to the tokens in this dictionary. */
    YRShellInterpreter      *M_interpreter;     /**< The interpreter object this dictionary belongs to. */
    
public:
    /** \brief Constructor.

    Default constructor, no parameters are required.
    */
    Dictionary( void);
    /** \brief Destructor.

    Default destructor.
    */
    virtual ~Dictionary( void);
    /** \brief Returns the mask.

    Returns the mask value applied to the tokens in this dictionary.
    */
    inline uint16_t getMask( void) { return m_mask; }
    /** \brief Should not be called.

    Will be handled by subclasses as applicable.
    */
    virtual uint16_t getFirstEntry( void);
    /** \brief Should not be called.

    Will be handled by subclasses as applicable.
    */
    virtual uint16_t getNextEntry( uint16_t index);
    /** \brief Should not be called.

    Will be handled by subclasses as applicable.
    */
    virtual uint16_t getToken( uint16_t index);
    /** \brief Should not be called.

    Will be handled by subclasses as applicable.
    */
    virtual uint16_t getNameAddressToken( uint16_t index);
    /** \brief Should not be called.

    Will be handled by subclasses as applicable.
    */
    virtual uint16_t getWord( uint16_t index);
    /** \brief Should not be called.

    Will be handled by subclasses as applicable.
    */
    virtual const char* getAddress( uint16_t index);
    /** \brief Should not be called.

    Will be handled by subclasses as applicable.
    */
    virtual uint16_t find( const char* name) =0;
    /** \brief Should not be called.

    Dispatches an error to M_interpreter. This is a generalized error reporting call for all the dictionary classes. 
    */
    virtual void yrshellERROR( const char* file, unsigned line);
    /** \brief Should not be called.

    Dispatches an error to M_interpreter. This is a generalized error reporting call for all the dictionary classes. 
    */
    virtual void yrshellERROR( const char* file, unsigned line, const char* message);  
    /** \brief Sets the intrepreter for this dictionary object.

    Not necessary for main functionality. Used to provide an error reporting path.
    */
    virtual void setInterpreter( YRShellInterpreter* shell);   
    /** \brief Returns the length rquired to store ths string in uin16_t words.

    All strings (includes trminating 0) in the dictionaries are stored as a sequence uint16_t words. This returns the number of words necessary to store the string.
    */
    static uint16_t nameLength( const char* name) { size_t len = strlen( name) + 1; return (uint16_t) (len/2 + (len & 1)); }
};

/** \brief Maps from text to a function number.
 
The function numbers along with the dictionary mask are used as tokens by the interpreters to call C code. Staticall declared in most applications.
*/
typedef struct {
    const uint16_t m_functionNumber;    /**< The funtion number. One of the following enums: YRShellInterpreter::SI_CC_functions, YRShell::S_CC_functions, or MyYRShell::SE_CC_functions */
    const char *M_functionName;         /**< The text string which is the name of the function. */
    
    inline bool isValid( void) const { return M_functionName != NULL; }     /**< Whether the entry is a valid entry. The last entry in a list is an invalid entry. */
    inline bool isMatch( const char* name) const { return !strcmp(M_functionName, name); } /**< If the entry matches the name. */
    inline uint16_t getFunctionNumber( void) const { return m_functionNumber; } /**< The function token for this function. */
    inline const char* getName( void) const { return M_functionName; } /**< The text name for this function. */
}  FunctionEntry;

/** \brief Dictionary to map native functions.
 
Is initialized with an array of FunctionEntry and a size. Provides functionality for searching and listing.
*/
class FunctionDictionary : public Dictionary  {
protected:
    const       FunctionEntry* M_dictionary;    /**< Pointer to the array of FunctionEntry */
    uint16_t    m_size;                         /**< The size of the FunctionEntry array. */
public:
    FunctionDictionary( const FunctionEntry* dict, uint16_t mask);   
    /** \brief Returns an index pointing to the first dictionary entry.

    Returns an index  pointing to the first dictionary entry. Returns YRSHELL_DICTIONARY_INVALID if there is no valid first entry.
    */
    uint16_t getFirstEntry( void);
    /** \brief Given an index , returns an index pointing to the next dictionary entry.

    Given an index , returns an index pointing to the next dictionary entry. Returns YRSHELL_DICTIONARY_INVALID if there is no valid next entry.
    */
    uint16_t getNextEntry( uint16_t index);
    /** \brief Given an index, returns the token.

    Given an index, returns the token. Returns YRSHELL_DICTIONARY_INVALID for an invalid index.
    */
    uint16_t getToken( uint16_t index);
    /** \brief Given an index, returns the token for the address of the name.

    Given an index, returns the token for the address of the name. Returns YRSHELL_DICTIONARY_INVALID for an invalid index.
    */
    uint16_t getNameAddressToken( uint16_t index);
    /** \brief Given an index, returns the address for the address of the name.  Returns NULL for an invalid index.

    Given an index, returns the address for he name.
    */
    const char* getAddress( uint16_t index);
    /** \brief Given a name, returns a token for the corresponding entry.

    Given a name, returns a token for the corresponding entry. Returns YRSHELL_DICTIONARY_INVALID if a corresponding entry is not found.
    */
    uint16_t find( const char* name);
};

/** \brief A compiled dictionary.
 
A compiled dictionary.
*/
class CompiledDictionary  : public Dictionary  {
protected:
    uint16_t*   m_dictionary;       /**< Pointer to the array of uint16_t which is the compiled dictionary. */
    uint16_t    m_lastWord;         /**< Index of the last entry in the dictionary*/
    uint16_t    m_size;             /**< The size of the dictionary. */
    
    /** \brief Given a name, returns a token for the corresponding entry.

    Given a name, returns a token for the corresponding entry. Returns YRSHELL_DICTIONARY_INVALID if a corresponding entry is not found.
    */
    uint16_t findInternal( const char* name, uint16_t link);
public:
    CompiledDictionary( void);
    CompiledDictionary( uint16_t* dict, uint16_t lastWord, uint16_t size, uint16_t mask);
    
    /** \brief Returns an index pointing to the first dictionary entry.

    Returns an index  pointing to the first dictionary entry. Returns YRSHELL_DICTIONARY_INVALID if there is no valid first entry.
    */
    uint16_t getFirstEntry( void);
    /** \brief Given an index , returns an index pointing to the next dictionary entry.

    Given an index , returns an index pointing to the next dictionary entry. Returns YRSHELL_DICTIONARY_INVALID if there is no valid next entry.
    */
    uint16_t getNextEntry( uint16_t index);

    /** \brief Given an index, returns the token.

    Given an index, returns the token. Returns YRSHELL_DICTIONARY_INVALID for an invalid index.
    */
    uint16_t getToken( uint16_t index);
    /** \brief Given an index, returns the token for the address of the name.

    Given an index, returns the token for the address of the name. Returns YRSHELL_DICTIONARY_INVALID for an invalid index.
    */
    uint16_t getNameAddressToken( uint16_t index);
    /** \brief Given an index, returns the uint16_t word at the index.

    Given an index, returns the uint16_t word at the index. Returns YRSHELL_DICTIONARY_INVALID for an invalid index.
    */
    uint16_t getWord( uint16_t index);
    
    /** \brief Given an index, returns the address for the address of the name.  Returns NULL for an invalid index.

    Given an index, returns the address for he name.
    */
    const char* getAddress( uint16_t index);
    /** \brief Given a name, returns a token for the corresponding entry.

    Given a name, returns a token for the corresponding entry. Returns YRSHELL_DICTIONARY_INVALID if a corresponding entry is not found.
    */
    uint16_t find( const char* name);
    /** \brief Returns an index past the last entry in the dictionary.

    Returns an index past the last entry in the dictionary.
    */
    uint16_t getWordEnd( void);
};


/** \brief A compiled dictionary, which is dynamic.
 
A compiled dictionary, which is dynamic.
*/
class CurrentVariableDictionary  : public CompiledDictionary  {
protected:
    uint16_t    m_dictionaryBackupLastWord;
    uint16_t    m_dictionaryBackupWordEnd;
    uint16_t    m_dictionaryCurrentWordEnd;
    
public:
    CurrentVariableDictionary( void);
    CurrentVariableDictionary( uint16_t* dict, uint16_t size);
    
    /** \brief Returns an index pointing to the first dictionary entry.

    Returns an index  pointing to the first dictionary entry. Returns YRSHELL_DICTIONARY_INVALID if there is no valid first entry.
    */
    uint16_t getFirstEntry( void);
    uint16_t getWord( uint16_t index);
    
    const char* getAddress( uint16_t index);
    /** \brief Given a name, returns a token for the corresponding entry.

    Given a name, returns a token for the corresponding entry. Returns YRSHELL_DICTIONARY_INVALID if a corresponding entry is not found.
    */
    uint16_t find( const char* name);
    
    bool newCompile( const char* name);
    bool addToken( uint16_t token);
    bool setToken( uint16_t address, uint16_t token);
    void rollBack( void);
    void newCompileDone( void);
    
    inline uint16_t getWordEnd( void) { return m_dictionaryCurrentWordEnd; }
    inline uint16_t getBackupLastWord( void) { return m_dictionaryBackupLastWord; }
    inline uint16_t getBackupWordEnd( void) { return m_dictionaryBackupWordEnd; }
    
};

template<unsigned SHELL_DICTIONARY_SIZE>
class CurrentDictionary  : public CurrentVariableDictionary  {
protected:
    uint16_t    m_dictionaryBuffer[ SHELL_DICTIONARY_SIZE];
public:
    CurrentDictionary( void) {
        m_mask = YRSHELL_DICTIONARY_CURRENT;
        m_size = SHELL_DICTIONARY_SIZE;
        m_dictionary = m_dictionaryBuffer;
        m_dictionaryBackupWordEnd = 0;
        m_dictionaryBackupLastWord = 0;
        m_dictionaryCurrentWordEnd = 0;
        m_lastWord = YRSHELL_DICTIONARY_INVALID;
    }
    
};

#endif
