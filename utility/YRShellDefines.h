#ifndef YRShellDefines_h
#define YRShellDefines_h

#include "HardwareSpecific.h"

#define YRSHELL_DEBUG
#define YRSHELL_INTERPRETER_FLOATING_POINT


#ifndef YRSHELL_VERSION
#define YRSHELL_VERSION "\rYRShell Version 0.1\r" // make sure this string length is less than YRSHELL_OUTQ_SIZE
#endif

typedef enum {
	YRSHELL_INVALID_STATE = 0,
    YRSHELL_BEGINNING = 1,
    YRSHELL_INRESET,
    YRSHELL_BEGIN_IDLE,
    YRSHELL_IDLE,
    YRSHELL_FILLING_PAD,
    YRSHELL_FILLING_AUXPAD,
    YRSHELL_BEGIN_PARSING,
    YRSHELL_PARSING,
    YRSHELL_COMPILING,
    YRSHELL_EXECUTING,
    YRSHELL_WAIT_FOR_OUTPUT_SPACE,
    YRSHELL_WAIT_DELAY
} YRShellState;


#define YRSHELL_DEBUG_NONE    0x0000
#define YRSHELL_DEBUG_STATE   0x0001
#define YRSHELL_DEBUG_INPUT   0x0002
#define YRSHELL_DEBUG_TOKEN   0x0004
#define YRSHELL_DEBUG_EXECUTE 0x0008

// these defines and the order are critical, algorithms depend on them
#define YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS    13
#define YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS      16
#define YRSHELL_DICTIONARY_ADDRESS_BITS_MASK      0XFFFF
typedef enum {
    YRSHELL_DICTIONARY_DUMMY_INDEX = 0,
    YRSHELL_DICTIONARY_CURRENT_INDEX = 1,
    YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX = 2,
    YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX = 3,
    YRSHELL_DICTIONARY_INTERPRETER_COMPILED_INDEX = 4,
    YRSHELL_DICTIONARY_COMMON_FUNCTION_INDEX = 5,
    YRSHELL_DICTIONARY_INTERPRETER_FUNCTION_INDEX = 6,
    YRSHELL_DICTIONARY_LAST_INDEX = 7
}YRShellDictionaryIndex;


#define YRSHELL_DICTIONARY_MASK                               0x0000E000
#define YRSHELL_DICTIONARY_INTERPRETER_FUNCTION               (YRSHELL_DICTIONARY_INTERPRETER_FUNCTION_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_COMMON_FUNCTION                    (YRSHELL_DICTIONARY_COMMON_FUNCTION_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_EXTENSION_FUNCTION                 (YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_INTERPRETER_COMPILED               (YRSHELL_DICTIONARY_INTERPRETER_COMPILED_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_EXTENSION_COMPILED                 (YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_CURRENT                            (YRSHELL_DICTIONARY_CURRENT_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_RELATIVE                           YRSHELL_DICTIONARY_MASK

#define YRSHELL_DICTIONARY_INTERPRETER_COMPILED_RELATIVE      (((uint32_t)YRSHELL_DICTIONARY_INTERPRETER_COMPILED_INDEX) << YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS)
#define YRSHELL_DICTIONARY_EXTENSION_COMPILED_RELATIVE        (((uint32_t)YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX) << YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS)
#define YRSHELL_DICTIONARY_CURRENT_RELATIVE                   (((uint32_t)YRSHELL_DICTIONARY_CURRENT_INDEX) << YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS)
#define YRSHELL_DICTIONARY_RELATIVE_MASK                      (YRSHELL_DICTIONARY_INTERPRETER_COMPILED_RELATIVE | YRSHELL_DICTIONARY_EXTENSION_COMPILED_RELATIVE | YRSHELL_DICTIONARY_CURRENT_RELATIVE)
#define YRSHELL_DICTIONARY_RELATIVE_ALL_MASK                  YRSHELL_DICTIONARY_RELATIVE_MASK
#define YRSHELL_DICTIONARY_INVALID                            0x0000FFFF
#define YRSHELL_DICTIONARY_ADDRESS_MASK                       ((~YRSHELL_DICTIONARY_MASK) & YRSHELL_DICTIONARY_ADDRESS_BITS_MASK)
#define YRSHELL_DICTIONARY_MAX_ADDRESS                        YRSHELL_DICTIONARY_ADDRESS_MASK

#endif
