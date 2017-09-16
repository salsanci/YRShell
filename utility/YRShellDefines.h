#ifndef YRShellDefines_h
#define YRShellDefines_h

#ifdef NOTARDUINO_AC6
#include "processorGlobal.h"
#endif


#define YRSHELL_DEBUG
#define YRSHELL_INTERPRETER_FLOATING_POINT


#ifndef YRSHELL_VERSION
#define YRSHELL_VERSION "\r\nYRShell Version 0.1\r\n" // make sure this string length is less than YRSHELL_OUTQ_SIZE
#endif

#ifndef YRSHELL_OUTPUT_TIMEOUT_IN_MILLISECONDS
#define YRSHELL_OUTPUT_TIMEOUT_IN_MILLISECONDS 1000
#endif
#if YRSHELL_OUTPUT_TIMEOUT_IN_MILLISECONDS < 10
#define YRSHELL_OUTPUT_TIMEOUT_IN_MILLISECONDS  10
#endif
#if YRSHELL_OUTPUT_TIMEOUT_IN_MILLISECONDS > 10000
#define YRSHELL_OUTPUT_TIMEOUT_IN_MILLISECONDS 10000
#endif


#ifndef YRSHELL_PARAMETER_STACK_SIZE
#define YRSHELL_PARAMETER_STACK_SIZE 16
#endif
#if YRSHELL_PARAMETER_STACK_SIZE < 8
#define YRSHELL_PARAMETER_STACK_SIZE  8
#endif
#if YRSHELL_PARAMETER_STACK_SIZE > 256
#define YRSHELL_PARAMETER_STACK_SIZE  256
#endif

#ifndef YRSHELL_RETURN_STACK_SIZE
#define YRSHELL_RETURN_STACK_SIZE 16
#endif
#if YRSHELL_RETURN_STACK_SIZE < 8
#define YRSHELL_RETURN_STACK_SIZE  8
#endif
#if YRSHELL_RETURN_STACK_SIZE > 256
#define YRSHELL_RETURN_STACK_SIZE  256
#endif

#ifndef YRSHELL_COMPILE_STACK_SIZE
#define YRSHELL_COMPILE_STACK_SIZE 16
#endif
#if YRSHELL_COMPILE_STACK_SIZE < 8
#define YRSHELL_COMPILE_STACK_SIZE  8
#endif
#if YRSHELL_COMPILE_STACK_SIZE > 64
#define YRSHELL_COMPILE_STACK_SIZE  64
#endif

#ifndef YRSHELL_DICTIONARY_SIZE
#define YRSHELL_DICTIONARY_SIZE 2048
#endif
#if YRSHELL_DICTIONARY_SIZE < 32
#define YRSHELL_DICTIONARY_SIZE  32
#endif
#if YRSHELL_DICTIONARY_SIZE > 8192
#define YRSHELL_DICTIONARY_SIZE  8192
#endif

#ifndef YRSHELL_NUM_REGISTERS
#define YRSHELL_NUM_REGISTERS 16
#endif
#if YRSHELL_NUM_REGISTERS < 8
#define YRSHELL_NUM_REGISTERS  8
#endif
#if YRSHELL_NUM_REGISTERS > 1024
#define YRSHELL_NUM_REGISTERS  1024
#endif

#ifndef YRSHELL_INQ_SIZE
#define YRSHELL_INQ_SIZE  128
#endif
#if YRSHELL_INQ_SIZE < 32
#define YRSHELL_INQ_SIZE  32
#endif
#if YRSHELL_INQ_SIZE > 8192
#define YRSHELL_INQ_SIZE  8192
#endif

#ifndef YRSHELL_AUX_INQ_SIZE
#define YRSHELL_AUX_INQ_SIZE  64
#endif
#if YRSHELL_AUX_INQ_SIZE < 32
#define YRSHELL_AUX_INQ_SIZE  32
#endif
#if YRSHELL_AUX_INQ_SIZE > 8192
#define YRSHELL_AUX_INQ_SIZE  8192
#endif

#ifndef YRSHELL_OUTQ_SIZE
#define YRSHELL_OUTQ_SIZE  256
#endif
#if YRSHELL_OUTQ_SIZE < 32
#define YRSHELL_OUTQ_SIZE  32
#endif
#if YRSHELL_OUTQ_SIZE > 8192
#define YRSHELL_OUTQ_SIZE  8192
#endif

#ifndef YRSHELL_AUX_OUTQ_SIZE
#define YRSHELL_AUX_OUTQ_SIZE  256
#endif
#if YRSHELL_AUX_OUTQ_SIZE < 32
#define YRSHELL_AUX_OUTQ_SIZE  32
#endif
#if YRSHELL_AUX_OUTQ_SIZE > 8192
#define YRSHELL_AUX_OUTQ_SIZE  8192
#endif

#ifndef YRSHELL_PAD_SIZE
#define YRSHELL_PAD_SIZE  128
#endif
#if YRSHELL_PAD_SIZE < 32
#define YRSHELL_PAD_SIZE  32
#endif
#if YRSHELL_PAD_SIZE > 8192
#define YRSHELL_PAD_SIZE  8192
#endif

#ifndef YRSHELL_COMMAND_ECHO
#define YRSHELL_COMMAND_ECHO  true
#endif

#ifndef YRSHELL_COMMAND_ECHO_EXPAND_CR
#define YRSHELL_COMMAND_ECHO_EXPAND_CR  true
#endif

typedef enum {
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
    YRSHELL_DICTIONARY_COMPILED_INDEX = 4,
    YRSHELL_DICTIONARY_FUNCTION_INDEX = 5,
    YRSHELL_DICTIONARY_INTERPRETER_FUNCTION_INDEX = 6,
    YRSHELL_DICTIONARY_LAST_INDEX = 7
}YRShellDictionaryIndex;


#define YRSHELL_DICTIONARY_MASK                               0x0000E000
#define YRSHELL_DICTIONARY_INTERPRETER_FUNCTION               (YRSHELL_DICTIONARY_INTERPRETER_FUNCTION_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_FUNCTION                           (YRSHELL_DICTIONARY_FUNCTION_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_EXTENSION_FUNCTION                 (YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_COMPILED                           (YRSHELL_DICTIONARY_COMPILED_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_EXTENSION_COMPILED                 (YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_CURRENT                            (YRSHELL_DICTIONARY_CURRENT_INDEX << YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS)
#define YRSHELL_DICTIONARY_RELATIVE                           YRSHELL_DICTIONARY_MASK

#define YRSHELL_DICTIONARY_COMPILED_RELATIVE                  (YRSHELL_DICTIONARY_COMPILED_INDEX << YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS)
#define YRSHELL_DICTIONARY_EXTENSION_COMPILED_RELATIVE        (YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX << YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS)
#define YRSHELL_DICTIONARY_CURRENT_RELATIVE                   (YRSHELL_DICTIONARY_CURRENT_INDEX << YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS)
#define YRSHELL_DICTIONARY_RELATIVE_MASK                      (YRSHELL_DICTIONARY_COMPILED_RELATIVE | YRSHELL_DICTIONARY_EXTENSION_COMPILED_RELATIVE | YRSHELL_DICTIONARY_CURRENT_RELATIVE)
#define YRSHELL_DICTIONARY_RELATIVE_ALL_MASK                  YRSHELL_DICTIONARY_RELATIVE_MASK
#define YRSHELL_DICTIONARY_INVALID                            0x0000FFFF
#define YRSHELL_DICTIONARY_ADDRESS_MASK                       ((~YRSHELL_DICTIONARY_MASK) & YRSHELL_DICTIONARY_ADDRESS_BITS_MASK)
#define YRSHELL_DICTIONARY_MAX_ADDRESS                        YRSHELL_DICTIONARY_ADDRESS_MASK

#endif
