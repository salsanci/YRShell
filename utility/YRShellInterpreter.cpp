
#include "YRShellInterpreter.h"

uint8_t	YRShellInterpreter::s_shellNumber = 0;
/*
 

 : ps? s' PARAMETER STACK: ' .str psd? 0!= [ psd? { dup ps@ . 1 - dup 0== } drop ] cr
 : rs? s'    RETURN STACK: ' .str rsd? 0!= [ rsd? 1 - { dup rs@ . 1 - dup -1 == } drop ] cr
 : cs? s'   COMPILE STACK: ' .str csd? 0!= [ csd? 1 - { dup cs@ . 1 - dup -1 == } drop ] cr
 : st? ps? rs? cs?
 
 : _cd0 s' \rstatic const uint16_t ' .str
 : _cxc dup 0x20 < over 0x7E > | [ drop 0x2e ] emit
 : _cxb dup 0xFF & _cxc 8 >> _cxc 1 +
 : _cxa dup getCurrentDictionary _cxb
 : _cx0 _cxa _cxa _cxa _cxa _cxa _cxa _cxa _cxa drop
 : _cd1 _cd0 s' compiledDictionaryData[] = {\r' .str
 : _cx1 _cd0 s' compiledExtensionDictionaryData[] = {\r' .str
 : _cd2 dup 0x10 - 0xF & 0== [ s' // ' .str dup 0x10 - dup .wx space _cx0 cr ]
 : _cd3 0 { dup getCurrentDictionary .wx s' , ' .str 1 + _cd2 dup getCurrentDictionaryEnd >= } drop
 : _cd4 getCurrentDictionaryEnd 0!= [ _cd3 crlf ] s' };\r' .str
 : _cd5 s' static const CompiledDictionary ' .str
 : _cx5 s' static CompiledDictionary ' .str

 : _cd6 s' interpreterCompiledDictionary( compiledDictionaryData, ' .str
 : _cx6 s' compiledExtensionDictionary( compiledExtensionDictionaryData, ' .str
 : _cd7 getCurrentDictionaryLastWord .wx s' , ' .str getCurrentDictionaryEnd .wx
 : _cd8 _cd1 _cd4 _cd5 _cd6 _cd7  s' , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);\r' .str
 : _cx8 _cx1 _cd4 _cd5 _cx6 _cd7 s' , YRSHELL_DICTIONARY_EXTENSION_COMPILED);\r' .str
 : compileInterpreterDictionary _cd8
 : compileExtensionDictionary _cx8
 
 : spaces dup [ { space 1 - dup 0== } ] drop
 : dictInvalid 0xFFFF
 : _wl2 rot dup [ c> drop cr ][ c> spaces ] 0== rot rot dup dictInvalid ==
 : _wl3 .entryName 40 swap - >c
 : wl 0 0 dictInvalid { nextEntry over .bx dup .wx 2dup entryToken .wx 2dup _wl3 _wl2 } 2drop drop cr
 : _wls1 over .bx dup .wx  2dup entryToken .wx 2dup .entryName drop cr
 : wls 0 dictInvalid { nextEntry  isEntryMatch [ _wls1 ] dup dictInvalid == } 2drop drop
 
 : _w over .bx dup .wx  2dup entryToken .wx 2dup .entryName drop cr
 : w 0 dictInvalid { nextEntry  _w dup dictInvalid == } 2drop
 : x 0 dictInvalid { nextEntry  _w dup dictInvalid == } 2drop drop
 
 : _inf0 s'      DICTIONARY_SIZE: ' .str dictionarySize . cr
 : _inf1 s'             PAD_SIZE: ' .str padSize . cr
 : _inf2 s'        NUM_REGISTERS: ' .str numRegisters . cr
 : _inf3 s' PARAMETER_STACK_SIZE: ' .str parameterStackSize . cr
 : _inf4 s'    RETURN_STACK_SIZE: ' .str returnStackSize . cr
 : _inf5 s'   COMPILE_STACK_SIZE: ' .str compileStackSize . cr
 : _inf6 s'             INQ_SIZE: ' .str inqSize . cr

 : _inf7 s'         AUX_INQ_SIZE: ' .str auxInqSize . cr
 : _inf8 s'            OUTQ_SIZE: ' .str outqSize . cr
 : _inf9 s'        AUX_OUTQ_SIZE: ' .str auxOutqSize . cr
 : _infa s'     LAST_BUFFER_SIZE: ' .str lastBufferSize . cr
 : _infb s'           Shell Size: ' .str shellSize . cr
 : _infc s' Dictionary used free: ' .str dictionarySize getCurrentDictionaryEnd dup . - . cr
 : _inf _inf0 _inf1 _inf2 _inf3 _inf4 _inf5 _inf6 _inf7 _inf8 _inf9 _infa _infb _infc
 : info cr printShellClass 4 spaces printMainFileName cr cr _inf
 
 : _ts0 s'       COUNT' .str
 : _ts1 s'          MIN' .str
 : _ts2 s'         MAX' .str
 : _ts3 s'       AVERAGE' .str
 : _ts4 _ts0 _ts1 _ts2 _ts3 cr cr
 : _ts5 dup sliceStats dup >r [ . . . . ] r> dup [ over printSliceName ] cr
 : runStats _ts4 0 { _ts5 [ 1 + 0 ][ -1 ] } drop
 : resetStats 0 { dup clearStats [ 1 + 0 ][ -1 ] } drop

 : _dr0  dup numRegisters < [  dup @ . 1 + ]
 : _dr1 dup . s' : ' .str  dup @ . _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 cr
 : regs 0 { _dr1 dup numRegisters >= } drop
 
 
 
 
 //TODO fix dc iFUNCTION
 
 // resolveRelative ( token_addr token_addr -- token_addr token_addr )
 : resolveRelative dup v_dictionaryMask  & v_dictionaryRelative == [ over v_dictionaryMask & swap v_dictionaryMask ~ & | ]
 
 : _dc5 s'  ERROR[ ' .str . s' ] ' .str
 // _dc4 ( dict index  --  0 -1 | dict index 0)
 : _dc4 dup dictInvalid == [ 2drop 0 -1 ][ 0 ]
 // _dc3 ( dict index -- -1 -1 )
 : _dc3 space .entryName drop -1 -1
 // _dc2 ( dict index -- dict index token)
 : _dc2 nextEntry 2dup entryToken
 
 // dcwName ( token_addr token -- )
 : dcwName resolveRelative >c 0 dictInvalid { _dc2 c> dup >c  == [ _dc3 ][ _dc4 ] } c> swap 0== [ _dc5 ][ drop ]
 
 // dcwa ( token_addr token -- token_addr t/f )
 : dcwa dup v_string != [ -1 ][ drop 1 + dup s'  s\' ' .str .rawStr 0x27 emit dup strlen 2 / + 0 ]
 // dcw8 ( token_addr token -- token_addr t/f )
 : dcw8 dup v_else != [ dup v_then != [ dcwa ][ drop s'  ]' .str 0 ] ][ drop 1 + s'  ][' .str 0 ]
 // dcw6 ( token_addr token -- token_addr t/f )
 : dcw6 dup v_until != [ dup v_if != [ dcw8 ][ drop 1 + s'  [' .str 0 ] ][ drop 1 + s'  }' .str 0 ]
 // dcw4 ( token_addr token -- token_addr t/f )
 : dcw4 dup v_nint16 != [ dup v_begin != [ dcw6 ][ drop s'  {' .str 0 ] ][ drop 1 + dup fetchToken 0xFFFF0000 + space .n 0 ]
 // dcw3 ( token_addr token -- token_addr t/f )
 : dcw3 dup v_uint32 != [ dcw4 ][ drop 1 + dup fetchToken swap 1 + tuck fetchToken 16 << + space .n 0 ]
 // dcw1 ( token_addr token -- token_addr t/f )
 : dcw1 dup v_return != [ dup v_uint16 != [ dcw3 ][ drop 1 + dup fetchToken space .n 0 ] ][ 0 ]
 // dcw ( token_addr -- token_addr )
 : dcw dup fetchToken dcw1 [ dcwName ]
 
 // _dc0 ( token_addr -- )
 : _dc0 { dup fetchToken v_return == [ drop -1 ][ dcw 1 + 0 ] }
 // dc ( string -- )
 : dc s' : ' .str dup .str find dup 0xFFFF != [ _dc0 ][ _dc5 ] cr
 
 // duw2 ( token_addr+256 token_addr -- token_addr+256 token_addr t/f )
 : duw2 dup fetchToken dup .wx v_return == >r 1 + 2dup <= r> |
 // duw1 ( token_addr+256 token_addr -- token_addr+256 token_addr t/f )
 : duw1 dup isFunction [ s' iFUNCTION' .str ][ { duw2 } ]
 // duw ( string -- )
 : duw find dup 0xFFFF != [ dup .wx s' : ' .str dup 0x100 + swap duw1 2drop ][ drop ] cr
 
 // _dca ( dict index token --  dict index )
 : _dca dup isFunction [ s' // ' .str drop 2dup .entryName drop cr ][ s' : ' .str >c 2dup .entryName drop c> _dc0  cr ]
 
 //dAT ( n -- dict entry)
 : dAT1 rot 1 + dup >c rot rot c> c> dup >c ==
 : dAT >c 0 0 dictInvalid { nextEntry dup dictInvalid == [ -1 ][ dAT1 ] } c> drop rot drop
 
 // dcCount( -- count)
 : dcCount 0 0 dictInvalid { nextEntry dup dictInvalid == [ -1 ][ rot 1 + rot rot  0 ] } 2drop
 
 // dcAll ( -- )
 : dcAll dcCount { dup dAT 2dup entryToken _dca 2drop 1 - dup 0 == }
 
 // key ( -- c )
 : key { key? [ -1 ] }
 
 // esc?
 : esc? key? [ 0x1B == ]
 
 compileInterpreterDictionary
 
 
 
 
 
 
 root@5cc636be335b:/home/root/YRShell/build# ./yrshell
 
 
 YRShell Version 0.2  Commit: 1dfb4f0d6ed5f06caceb69e94d002e6d44c98118 [1dfb4f0]
 Compile Time: Jan 13 2018 20:20:34 Main File: build/main.cpp
 
 YR00>
 : ps? s' PARAMETER STACK: ' .str psd? 0!= [ psd? { dup ps@ . 1 - dup 0== } drop ] cr
 : rs? s'    RETURN STACK: ' .str rsd? 0!= [ rsd? 1 - { dup rs@ . 1 - dup -1 == } drop ] cr
 : cs? s'   COMPILE STACK: ' .str csd? 0!= [ csd? 1 - { dup cs@ . 1 - dup -1 == } drop ] cr
 : st? ps? rs? cs?
 
 : _cd0 s' \rstatic const uint16_t ' .str
 : _cxc dup 0x20 < over 0x7E > | [ drop 0x2e ] emit
 : _cxb dup 0xFF & _cxc 8 >> _cxc 1 +
 : _cxa dup getCurrentDictionary _cxb
 : _cx0 _cxa _cxa _cxa _cxa _cxa _cxa _cxa _cxa drop
 : _cd1 _cd0 s' compiledDictionaryData[] = {\r' .str
 : _cx1 _cd0 s' compiledExtensionDictionaryData[] = {\r' .str
 : _cd2 dup 0x10 - 0xF & 0== [ s' // ' .str dup 0x10 - dup .wx space _cx0 cr ]
 : _cd3 0 { dup getCurrentDictionary .wx s' , ' .str 1 + _cd2 dup getCurrentDictionaryEnd >= } drop
 : _cd4 getCurrentDictionaryEnd 0!= [ _cd3 crlf ] s' };\r' .str
 : _cd5 s' static const CompiledDictionary ' .str
 : _cx5 s' static CompiledDictionary ' .str
 
 : _cd6 s' interpreterCompiledDictionary( compiledDictionaryData, ' .str
 : _cx6 s' compiledExtensionDictionary( compiledExtensionDictionaryData, ' .str
 : _cd7 getCurrentDictionaryLastWord .wx s' , ' .str getCurrentDictionaryEnd .wx
 : _cd8 _cd1 _cd4 _cd5 _cd6 _cd7  s' , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);\r' .str
 : _cx8 _cx1 _cd4 _cd5 _cx6 _cd7 s' , YRSHELL_DICTIONARY_EXTENSION_COMPILED);\r' .str
 : compileInterpreterDictionary _cd8
 : compileExtensionDictionary _cx8
 
 : spaces dup [ { space 1 - dup 0== } ] drop
 : dictInvalid 0xFFFF
 : _wl2 rot dup [ c> drop cr ][ c> spaces ] 0== rot rot dup dictInvalid ==
 : _wl3 .entryName 40 swap - >c
 : wl 0 0 dictInvalid { nextEntry over .bx dup .wx 2dup entryToken .wx 2dup _wl3 _wl2 } 2drop drop cr
 
 : _inf0 s'      DICTIONARY_SIZE: ' .str dictionarySize . cr
 : _inf1 s'             PAD_SIZE: ' .str padSize . cr
 : _inf2 s'        NUM_REGISTERS: ' .str numRegisters . cr
 : _inf3 s' PARAMETER_STACK_SIZE: ' .str parameterStackSize . cr
 : _inf4 s'    RETURN_STACK_SIZE: ' .str returnStackSize . cr
 : _inf5 s'   COMPILE_STACK_SIZE: ' .str compileStackSize . cr
 : _inf6 s'             INQ_SIZE: ' .str inqSize . cr
 
 : _inf7 s'         AUX_INQ_SIZE: ' .str auxInqSize . cr
 : _inf8 s'            OUTQ_SIZE: ' .str outqSize . cr
 : _inf9 s'        AUX_OUTQ_SIZE: ' .str auxOutqSize . cr
 : _infa s'     LAST_BUFFER_SIZE: ' .str lastBufferSize . cr
 : _infb s'           Shell Size: ' .str shellSize . cr
 : _infc s' Dictionary used free: ' .str dictionarySize getCurrentDictionaryEnd dup . - . cr
 : _inf _inf0 _inf1 _inf2 _inf3 _inf4 _inf5 _inf6 _inf7 _inf8 _inf9 _infa _infb _infc
 : info cr printShellClass 4 spaces printMainFileName cr cr _inf
 
 : _ts0 s'       COUNT' .str
 : _ts1 s'          MIN' .str
 : _ts2 s'         MAX' .str
 : _ts3 s'       AVERAGE' .str
 : _ts4 _ts0 _ts1 _ts2 _ts3 cr cr
 : _ts5 dup sliceStats dup >r [ . . . . ] r> dup [ over printSliceName ] cr
 : runStats _ts4 0 { _ts5 [ 1 + 0 ][ -1 ] } drop
 : resetStats 0 { dup clearStats [ 1 + 0 ][ -1 ] } drop
 
 : _dr0  dup numRegisters < [  dup @ . 1 + ]
 : _dr1 dup . s' : ' .str  dup @ . _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 cr
 : regs 0 { _dr1 dup numRegisters >= } drop
 
 
 
 
 //TODO fix dc iFUNCTION
 
 // resolveRelative ( token_addr token_addr -- token_addr token_addr )
 : resolveRelative dup v_dictionaryMask  & v_dictionaryRelative == [ over v_dictionaryMask & swap v_dictionaryMask ~ & | ]
 
 : _dc5 s'  ERROR[ ' .str . s' ] ' .str
 // _dc4 ( dict index  --  0 -1 | dict index 0)
 : _dc4 dup dictInvalid == [ 2drop 0 -1 ][ 0 ]
 // _dc3 ( dict index -- -1 -1 )
 : _dc3 space .entryName drop -1 -1
 // _dc2 ( dict index -- dict index token)
 : _dc2 nextEntry 2dup entryToken
 
 // dcwName ( token_addr token -- )
 : dcwName resolveRelative >c 0 dictInvalid { _dc2 c> dup >c  == [ _dc3 ][ _dc4 ] } c> swap 0== [ _dc5 ][ drop ]
 
 // dcwa ( token_addr token -- token_addr t/f )
 : dcwa dup v_string != [ -1 ][ drop 1 + dup s'  s\' ' .str .rawStr 0x27 emit dup strlen 2 / + 0 ]
 // dcw8 ( token_addr token -- token_addr t/f )
 : dcw8 dup v_else != [ dup v_then != [ dcwa ][ drop s'  ]' .str 0 ] ][ drop 1 + s'  ][' .str 0 ]
 // dcw6 ( token_addr token -- token_addr t/f )
 : dcw6 dup v_until != [ dup v_if != [ dcw8 ][ drop 1 + s'  [' .str 0 ] ][ drop 1 + s'  }' .str 0 ]
 // dcw4 ( token_addr token -- token_addr t/f )
 : dcw4 dup v_nint16 != [ dup v_begin != [ dcw6 ][ drop s'  {' .str 0 ] ][ drop 1 + dup fetchToken 0xFFFF0000 + space .n 0 ]
 // dcw3 ( token_addr token -- token_addr t/f )
 : dcw3 dup v_uint32 != [ dcw4 ][ drop 1 + dup fetchToken swap 1 + tuck fetchToken 16 << + space .n 0 ]
 // dcw1 ( token_addr token -- token_addr t/f )
 : dcw1 dup v_return != [ dup v_uint16 != [ dcw3 ][ drop 1 + dup fetchToken space .n 0 ] ][ 0 ]
 // dcw ( token_addr -- token_addr )
 : dcw dup fetchToken dcw1 [ dcwName ]
 
 // _dc0 ( token_addr -- )
 : _dc0 { dup fetchToken v_return == [ drop -1 ][ dcw 1 + 0 ] }
 // dc ( string -- )
 : dc s' : ' .str dup .str find dup 0xFFFF != [ _dc0 ][ _dc5 ] cr
 
 // duw2 ( token_addr+256 token_addr -- token_addr+256 token_addr t/f )
 : duw2 dup fetchToken dup .wx v_return == >r 1 + 2dup <= r> |
 // duw1 ( token_addr+256 token_addr -- token_addr+256 token_addr t/f )
 : duw1 dup isFunction [ s' iFUNCTION' .str ][ { duw2 } ]
 // duw ( string -- )
 : duw find dup 0xFFFF != [ dup .wx s' : ' .str dup 0x100 + swap duw1 2drop ][ drop ] cr
 
 // _dca ( dict index token --  dict index )
 : _dca dup isFunction [ s' // ' .str drop 2dup .entryName drop cr ][ s' : ' .str >c 2dup .entryName drop c> _dc0  cr ]
 
 //dAT ( n -- dict entry)
 : dAT1 rot 1 + dup >c rot rot c> c> dup >c ==
 : dAT >c 0 0 dictInvalid { nextEntry dup dictInvalid == [ -1 ][ dAT1 ] } c> drop rot drop
 
 // dcCount( -- count)
 : dcCount 0 0 dictInvalid { nextEntry dup dictInvalid == [ -1 ][ rot 1 + rot rot  0 ] } 2drop
 
 // dcAll ( -- )
 : dcAll dcCount { dup dAT 2dup entryToken _dca 2drop 1 - dup 0 == }
 
 // key ( -- c )
 : key { key? [ -1 ] }
 
 // esc?
 : esc? key? [ 0x1B == ]
 
 compileInterpreterDictionary
 
 
 YR00> : ps? s' PARAMETER STACK: ' .str psd? 0!= [ psd? { dup ps@ . 1 - dup 0== } drop ] cr
 
 YR00> : rs? s'    RETURN STACK: ' .str rsd? 0!= [ rsd? 1 - { dup rs@ . 1 - dup -1 == } drop ] cr
 
 YR00> : cs? s'   COMPILE STACK: ' .str csd? 0!= [ csd? 1 - { dup cs@ . 1 - dup -1 == } drop ] cr
 
 YR00> : st? ps? rs? cs?
 
 YR00>
 
 YR00> : _cd0 s' \rstatic const uint16_t ' .str
 
 YR00> : _cxc dup 0x20 < over 0x7E > | [ drop 0x2e ] emit
 
 YR00> : _cxb dup 0xFF & _cxc 8 >> _cxc 1 +
 
 YR00> : _cxa dup getCurrentDictionary _cxb
 
 YR00> : _cx0 _cxa _cxa _cxa _cxa _cxa _cxa _cxa _cxa drop
 
 YR00> : _cd1 _cd0 s' compiledDictionaryData[] = {\r' .str
 
 YR00> : _cx1 _cd0 s' compiledExtensionDictionaryData[] = {\r' .str
 
 YR00> : _cd2 dup 0x10 - 0xF & 0== [ s' // ' .str dup 0x10 - dup .wx space _cx0 cr ]
 
 YR00> : _cd3 0 { dup getCurrentDictionary .wx s' , ' .str 1 + _cd2 dup getCurrentDictionaryEnd >= } drop
 
 YR00> : _cd4 getCurrentDictionaryEnd 0!= [ _cd3 crlf ] s' };\r' .str
 
 YR00> : _cd5 s' static const CompiledDictionary ' .str
 
 YR00> : _cx5 s' static CompiledDictionary ' .str
 
 YR00>
 
 YR00> : _cd6 s' interpreterCompiledDictionary( compiledDictionaryData, ' .str
 
 YR00> : _cx6 s' compiledExtensionDictionary( compiledExtensionDictionaryData, ' .str
 
 YR00> : _cd7 getCurrentDictionaryLastWord .wx s' , ' .str getCurrentDictionaryEnd .wx
 
 YR00> : _cd8 _cd1 _cd4 _cd5 _cd6 _cd7  s' , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);\r' .str
 
 YR00> : _cx8 _cx1 _cd4 _cd5 _cx6 _cd7 s' , YRSHELL_DICTIONARY_EXTENSION_COMPILED);\r' .str
 
 YR00> : compileInterpreterDictionary _cd8
 
 YR00> : compileExtensionDictionary _cx8
 
 YR00>
 
 YR00> : spaces dup [ { space 1 - dup 0== } ] drop
 
 YR00> : dictInvalid 0xFFFF
 
 YR00> : _wl2 rot dup [ c> drop cr ][ c> spaces ] 0== rot rot dup dictInvalid ==
 
 YR00> : _wl3 .entryName 40 swap - >c
 
 YR00> : wl 0 0 dictInvalid { nextEntry over .bx dup .wx 2dup entryToken .wx 2dup _wl3 _wl2 } 2drop drop cr
 
 YR00>
 
 YR00> : _inf0 s'      DICTIONARY_SIZE: ' .str dictionarySize . cr
 
 YR00> : _inf1 s'             PAD_SIZE: ' .str padSize . cr
 
 YR00> : _inf2 s'        NUM_REGISTERS: ' .str numRegisters . cr
 
 YR00> : _inf3 s' PARAMETER_STACK_SIZE: ' .str parameterStackSize . cr
 
 YR00> : _inf4 s'    RETURN_STACK_SIZE: ' .str returnStackSize . cr
 
 YR00> : _inf5 s'   COMPILE_STACK_SIZE: ' .str compileStackSize . cr
 
 YR00> : _inf6 s'             INQ_SIZE: ' .str inqSize . cr
 
 YR00>
 
 YR00> : _inf7 s'         AUX_INQ_SIZE: ' .str auxInqSize . cr
 
 YR00> : _inf8 s'            OUTQ_SIZE: ' .str outqSize . cr
 
 YR00> : _inf9 s'        AUX_OUTQ_SIZE: ' .str auxOutqSize . cr
 
 YR00> : _infa s'     LAST_BUFFER_SIZE: ' .str lastBufferSize . cr
 
 YR00> : _infb s'           Shell Size: ' .str shellSize . cr
 
 YR00> : _infc s' Dictionary used free: ' .str dictionarySize getCurrentDictionaryEnd dup . - . cr
 
 YR00> : _inf _inf0 _inf1 _inf2 _inf3 _inf4 _inf5 _inf6 _inf7 _inf8 _inf9 _infa _infb _infc
 
 YR00> : info cr printShellClass 4 spaces printMainFileName cr cr _inf
 
 YR00>
 
 YR00> : _ts0 s'       COUNT' .str
 
 YR00> : _ts1 s'          MIN' .str
 
 YR00> : _ts2 s'         MAX' .str
 
 YR00> : _ts3 s'       AVERAGE' .str
 
 YR00> : _ts4 _ts0 _ts1 _ts2 _ts3 cr cr
 
 YR00> : _ts5 dup sliceStats dup >r [ . . . . ] r> dup [ over printSliceName ] cr
 
 YR00> : runStats _ts4 0 { _ts5 [ 1 + 0 ][ -1 ] } drop
 
 YR00> : resetStats 0 { dup clearStats [ 1 + 0 ][ -1 ] } drop
 
 YR00>
 
 YR00> : _dr0  dup numRegisters < [  dup @ . 1 + ]
 
 YR00> : _dr1 dup . s' : ' .str  dup @ . _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 cr
 
 YR00> : regs 0 { _dr1 dup numRegisters >= } drop
 
 YR00>
 
 YR00>
 
 YR00>
 
 YR00>
 
 YR00> //TODO fix dc iFUNCTION
 
 YR00>
 
 YR00> // resolveRelative ( token_addr token_addr -- token_addr token_addr )
 
 YR00> : resolveRelative dup v_dictionaryMask  & v_dictionaryRelative == [ over v_dictionaryMask & swap v_dictionaryMask ~ & | ]
 
 YR00>
 
 YR00> : _dc5 s'  ERROR[ ' .str . s' ] ' .str
 
 YR00> // _dc4 ( dict index  --  0 -1 | dict index 0)
 
 YR00> : _dc4 dup dictInvalid == [ 2drop 0 -1 ][ 0 ]
 
 YR00> // _dc3 ( dict index -- -1 -1 )
 
 YR00> : _dc3 space .entryName drop -1 -1
 
 YR00> // _dc2 ( dict index -- dict index token)
 
 YR00> : _dc2 nextEntry 2dup entryToken
 
 YR00>
 
 YR00> // dcwName ( token_addr token -- )
 
 YR00> : dcwName resolveRelative >c 0 dictInvalid { _dc2 c> dup >c  == [ _dc3 ][ _dc4 ] } c> swap 0== [ _dc5 ][ drop ]
 
 YR00>
 
 YR00> // dcwa ( token_addr token -- token_addr t/f )
 
 YR00> : dcwa dup v_string != [ -1 ][ drop 1 + dup s'  s\' ' .str .rawStr 0x27 emit dup strlen 2 / + 0 ]
 
 YR00> // dcw8 ( token_addr token -- token_addr t/f )
 
 YR00> : dcw8 dup v_else != [ dup v_then != [ dcwa ][ drop s'  ]' .str 0 ] ][ drop 1 + s'  ][' .str 0 ]
 
 YR00> // dcw6 ( token_addr token -- token_addr t/f )
 
 YR00> : dcw6 dup v_until != [ dup v_if != [ dcw8 ][ drop 1 + s'  [' .str 0 ] ][ drop 1 + s'  }' .str 0 ]
 
 YR00> // dcw4 ( token_addr token -- token_addr t/f )
 
 YR00> : dcw4 dup v_nint16 != [ dup v_begin != [ dcw6 ][ drop s'  {' .str 0 ] ][ drop 1 + dup fetchToken 0xFFFF0000 + space .n 0 ]
 
 YR00> // dcw3 ( token_addr token -- token_addr t/f )
 
 YR00> : dcw3 dup v_uint32 != [ dcw4 ][ drop 1 + dup fetchToken swap 1 + tuck fetchToken 16 << + space .n 0 ]
 
 YR00> // dcw1 ( token_addr token -- token_addr t/f )
 
 YR00> : dcw1 dup v_return != [ dup v_uint16 != [ dcw3 ][ drop 1 + dup fetchToken space .n 0 ] ][ 0 ]
 
 YR00> // dcw ( token_addr -- token_addr )
 
 YR00> : dcw dup fetchToken dcw1 [ dcwName ]
 
 YR00>
 
 YR00> // _dc0 ( token_addr -- )
 
 YR00> : _dc0 { dup fetchToken v_return == [ drop -1 ][ dcw 1 + 0 ] }
 
 YR00> // dc ( string -- )
 
 YR00> : dc s' : ' .str dup .str find dup 0xFFFF != [ _dc0 ][ _dc5 ] cr
 
 YR00>
 
 YR00> // duw2 ( token_addr+256 token_addr -- token_addr+256 token_addr t/f )
 
 YR00> : duw2 dup fetchToken dup .wx v_return == >r 1 + 2dup <= r> |
 
 YR00> // duw1 ( token_addr+256 token_addr -- token_addr+256 token_addr t/f )
 
 YR00> : duw1 dup isFunction [ s' iFUNCTION' .str ][ { duw2 } ]
 
 YR00> // duw ( string -- )
 
 YR00> : duw find dup 0xFFFF != [ dup .wx s' : ' .str dup 0x100 + swap duw1 2drop ][ drop ] cr
 
 YR00>
 
 YR00> // _dca ( dict index token --  dict index )
 
 YR00> : _dca dup isFunction [ s' // ' .str drop 2dup .entryName drop cr ][ s' : ' .str >c 2dup .entryName drop c> _dc0  cr ]
 
 YR00>
 
 YR00> //dAT ( n -- dict entry)
 
 YR00> : dAT1 rot 1 + dup >c rot rot c> c> dup >c ==
 
 YR00> : dAT >c 0 0 dictInvalid { nextEntry dup dictInvalid == [ -1 ][ dAT1 ] } c> drop rot drop
 
 YR00>
 
 YR00> // dcCount( -- count)
 
 YR00> : dcCount 0 0 dictInvalid { nextEntry dup dictInvalid == [ -1 ][ rot 1 + rot rot  0 ] } 2drop
 
 YR00>
 
 YR00> // dcAll ( -- )
 
 YR00> : dcAll dcCount { dup dAT 2dup entryToken _dca 2drop 1 - dup 0 == }
 
 YR00>
 
 YR00> // key ( -- c )
 
 YR00> : key { key? [ -1 ] }
 
 YR00>
 
 YR00> // esc?
 
 YR00> : esc? key? [ 0x1B == ]
 
 YR00>
 
 YR00> compileInterpreterDictionary
 
 static const uint16_t compiledDictionaryData[] = {
 0xFFFF , 0x7370 , 0x003F , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , 0x2052 , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC037 , 0xC03F , // 0x0000  ..ps?...PARAMETE
 0xC073 , 0xE020 , 0xC037 , 0xC076 , 0xC01E , 0xC03A , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC077 , 0xE014 , 0xC022 , 0xC075 , // 0x0010  s. .7.v...:.....
 0xC003 , 0xC001 , 0x0000 , 0x7372 , 0x003F , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 0x204E , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , // 0x0020  ......rs?...   R
 0xC038 , 0xC03F , 0xC073 , 0xE047 , 0xC038 , 0xC009 , 0x0001 , 0xC028 , 0xC076 , 0xC01E , 0xC03B , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , // 0x0030  8.?.s.G.8.....(.
 0xC00B , 0xFFFF , 0xC031 , 0xC077 , 0xE039 , 0xC022 , 0xC075 , 0xC003 , 0xC001 , 0x0022 , 0x7363 , 0x003F , 0xC016 , 0x2020 , 0x4F43 , 0x504D , // 0x0040  ....1.w.9.".u...
 0x4C49 , 0x2045 , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC039 , 0xC03F , 0xC073 , 0xE06E , 0xC039 , 0xC009 , 0x0001 , 0xC028 , 0xC076 , // 0x0050  ILE STACK: ...9.
 0xC01E , 0xC03C , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC00B , 0xFFFF , 0xC031 , 0xC077 , 0xE060 , 0xC022 , 0xC075 , 0xC003 , 0xC001 , // 0x0060  ..<.......(.....
 0x0049 , 0x7473 , 0x003F , 0xE003 , 0xE025 , 0xE04C , 0xC001 , 0x0070 , 0x635F , 0x3064 , 0x0000 , 0xC016 , 0x730D , 0x6174 , 0x6974 , 0x2063 , // 0x0070  I.st?...%.L...p.
 0x6F63 , 0x736E , 0x2074 , 0x6975 , 0x746E , 0x3631 , 0x745F , 0x0020 , 0xC012 , 0xC001 , 0x0077 , 0x635F , 0x6378 , 0x0000 , 0xC01E , 0xC009 , // 0x0080  const uint16_t .
 0x0020 , 0xC02F , 0xC040 , 0xC009 , 0x007E , 0xC032 , 0xC02D , 0xC073 , 0xE09D , 0xC022 , 0xC009 , 0x002E , 0xC075 , 0xC04F , 0xC001 , 0x008A , // 0x0090   ./.@...~.2.-.s.
 0x635F , 0x6278 , 0x0000 , 0xC01E , 0xC009 , 0x00FF , 0xC02C , 0xE08E , 0xC009 , 0x0008 , 0xC06D , 0xE08E , 0xC009 , 0x0001 , 0xC027 , 0xC001 , // 0x00A0  _cxb........,...
 0x009F , 0x635F , 0x6178 , 0x0000 , 0xC01E , 0xC044 , 0xE0A3 , 0xC001 , 0x00B0 , 0x635F , 0x3078 , 0x0000 , 0xE0B4 , 0xE0B4 , 0xE0B4 , 0xE0B4 , // 0x00B0  .._cxa....D.....
 0xE0B4 , 0xE0B4 , 0xE0B4 , 0xE0B4 , 0xC022 , 0xC001 , 0x00B8 , 0x635F , 0x3164 , 0x0000 , 0xE07B , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , // 0x00C0  ........"....._c
 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x6144 , 0x6174 , 0x5D5B , 0x3D20 , 0x7B20 , 0x000D , 0xC012 , 0xC001 , 0x00C6 , 0x635F , 0x3178 , // 0x00D0  DictionaryData[]
 0x0000 , 0xE07B , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 0x446E , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , // 0x00E0  ..{...compiledEx
 0x4479 , 0x7461 , 0x5B61 , 0x205D , 0x203D , 0x0D7B , 0x0000 , 0xC012 , 0xC001 , 0x00DD , 0x635F , 0x3264 , 0x0000 , 0xC01E , 0xC009 , 0x0010 , // 0x00F0  yData[] = {.....
 0xC028 , 0xC009 , 0x000F , 0xC02C , 0xC03E , 0xC073 , 0xE115 , 0xC016 , 0x2F2F , 0x0020 , 0xC012 , 0xC01E , 0xC009 , 0x0010 , 0xC028 , 0xC01E , // 0x0100  (.....,.>.s.....
 0xC011 , 0xC002 , 0xE0BC , 0xC003 , 0xC075 , 0xC001 , 0x00F9 , 0x635F , 0x3364 , 0x0000 , 0xC009 , 0x0000 , 0xC076 , 0xC01E , 0xC044 , 0xC011 , // 0x0110  ........u....._c
 0xC016 , 0x202C , 0x0000 , 0xC012 , 0xC009 , 0x0001 , 0xC027 , 0xE0FD , 0xC01E , 0xC045 , 0xC033 , 0xC077 , 0xE11D , 0xC022 , 0xC001 , 0x0116 , // 0x0120  .., ........'...
 0x635F , 0x3464 , 0x0000 , 0xC045 , 0xC03F , 0xC073 , 0xE13A , 0xE11A , 0xC005 , 0xC075 , 0xC016 , 0x3B7D , 0x000D , 0xC012 , 0xC001 , 0x012F , // 0x0130  _cd4..E.?.s.:...
 0x635F , 0x3564 , 0x0000 , 0xC016 , 0x7473 , 0x7461 , 0x6369 , 0x6320 , 0x6E6F , 0x7473 , 0x4320 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , // 0x0140  _cd5....static c
 0x6974 , 0x6E6F , 0x7261 , 0x2079 , 0x0000 , 0xC012 , 0xC001 , 0x013F , 0x635F , 0x3578 , 0x0000 , 0xC016 , 0x7473 , 0x7461 , 0x6369 , 0x4320 , // 0x0150  tionary ......?.
 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x2079 , 0x0000 , 0xC012 , 0xC001 , 0x0157 , 0x635F , 0x3664 , 0x0000 , // 0x0160  ompiledDictionar
 0xC016 , 0x6E69 , 0x6574 , 0x7072 , 0x6572 , 0x6574 , 0x4372 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x2879 , // 0x0170  ..interpreterCom
 0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x4479 , 0x7461 , 0x2C61 , 0x0020 , 0xC012 , 0xC001 , 0x016C , // 0x0180   compiledDiction
 0x635F , 0x3678 , 0x0000 , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 0x446E , 0x6369 , 0x6974 , 0x6E6F , // 0x0190  _cx6....compiled
 0x7261 , 0x2879 , 0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4564 , 0x7478 , 0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , // 0x01A0  ary( compiledExt
 0x6144 , 0x6174 , 0x202C , 0x0000 , 0xC012 , 0xC001 , 0x018F , 0x635F , 0x3764 , 0x0000 , 0xC046 , 0xC011 , 0xC016 , 0x202C , 0x0000 , 0xC012 , // 0x01B0  Data, ........_c
 0xC045 , 0xC011 , 0xC001 , 0x01B6 , 0x635F , 0x3864 , 0x0000 , 0xE0CA , 0xE133 , 0xE143 , 0xE170 , 0xE1BA , 0xC016 , 0x202C , 0x5259 , 0x4853 , // 0x01C0  E......._cd8....
 0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x495F , 0x544E , 0x5245 , 0x5250 , 0x5445 , 0x5245 , 0x435F , 0x4D4F , 0x4950 , // 0x01D0  ELL_DICTIONARY_I
 0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x01C3 , 0x635F , 0x3878 , 0x0000 , 0xE0E1 , 0xE133 , 0xE143 , 0xE193 , 0xE1BA , 0xC016 , // 0x01E0  LED);........._c
 0x202C , 0x5259 , 0x4853 , 0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x455F , 0x5458 , 0x4E45 , 0x4953 , 0x4E4F , 0x435F , // 0x01F0  , YRSHELL_DICTIO
 0x4D4F , 0x4950 , 0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x01E6 , 0x6F63 , 0x706D , 0x6C69 , 0x4965 , 0x746E , 0x7265 , 0x7270 , // 0x0200  OMPILED);.......
 0x7465 , 0x7265 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE1C7 , 0xC001 , 0x0208 , 0x6F63 , 0x706D , 0x6C69 , 0x4565 , 0x7478 , // 0x0210  eterDictionary..
 0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE1EA , 0xC001 , 0x021A , 0x7073 , 0x6361 , 0x7365 , 0x0000 , // 0x0220  ensionDictionary
 0xC01E , 0xC073 , 0xE23D , 0xC076 , 0xC002 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC077 , 0xE234 , 0xC075 , 0xC022 , 0xC001 , 0x022B , // 0x0230  ..s.=.v.......(.
 0x6964 , 0x7463 , 0x6E49 , 0x6176 , 0x696C , 0x0064 , 0xC009 , 0xFFFF , 0xC001 , 0x023F , 0x775F , 0x326C , 0x0000 , 0xC023 , 0xC01E , 0xC073 , // 0x0240  dictInvalid.....
 0xE256 , 0xC036 , 0xC022 , 0xC003 , 0xC074 , 0xE259 , 0xC036 , 0xE230 , 0xC075 , 0xC03E , 0xC023 , 0xC023 , 0xC01E , 0xE246 , 0xC031 , 0xC001 , // 0x0250  V.6."...t.Y.6.0.
 0x0249 , 0x775F , 0x336C , 0x0000 , 0xC049 , 0xC009 , 0x0028 , 0xC01F , 0xC028 , 0xC035 , 0xC001 , 0x0260 , 0x6C77 , 0x0000 , 0xC009 , 0x0000 , // 0x0260  I._wl3..I...(...
 0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xC048 , 0xC040 , 0xC010 , 0xC01E , 0xC011 , 0xC041 , 0xC04A , 0xC011 , 0xC041 , 0xE264 , 0xE24D , 0xC077 , // 0x0270  ....F.v.H.@.....
 0xE274 , 0xC042 , 0xC022 , 0xC003 , 0xC001 , 0x026B , 0x695F , 0x666E , 0x0030 , 0xC016 , 0x2020 , 0x2020 , 0x4420 , 0x4349 , 0x4954 , 0x4E4F , // 0x0280  t.B.".....k._inf
 0x5241 , 0x5F59 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC053 , 0xC00C , 0xC003 , 0xC001 , 0x0285 , 0x695F , 0x666E , 0x0031 , 0xC016 , // 0x0290  ARY_SIZE: ....S.
 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4150 , 0x5F44 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC054 , 0xC00C , 0xC003 , // 0x02A0              PAD_
 0xC001 , 0x029B , 0x695F , 0x666E , 0x0032 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4E20 , 0x4D55 , 0x525F , 0x4745 , 0x5349 , 0x4554 , 0x5352 , // 0x02B0  ...._inf2...
 0x203A , 0x0000 , 0xC012 , 0xC055 , 0xC00C , 0xC003 , 0xC001 , 0x02B1 , 0x695F , 0x666E , 0x0033 , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , // 0x02C0  : ....U.........
 0x5F52 , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC056 , 0xC00C , 0xC003 , 0xC001 , 0x02C7 , 0x695F , 0x666E , // 0x02D0  R_STACK_SIZE: ..
 0x0034 , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 0x5F4E , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC057 , // 0x02E0  4...   RETURN_ST
 0xC00C , 0xC003 , 0xC001 , 0x02DD , 0x695F , 0x666E , 0x0035 , 0xC016 , 0x2020 , 0x4F43 , 0x504D , 0x4C49 , 0x5F45 , 0x5453 , 0x4341 , 0x5F4B , // 0x02F0  ........_inf5...
 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC058 , 0xC00C , 0xC003 , 0xC001 , 0x02F3 , 0x695F , 0x666E , 0x0036 , 0xC016 , 0x2020 , 0x2020 , // 0x0300  SIZE: ....X.....
 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4E49 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC059 , 0xC00C , 0xC003 , 0xC001 , 0x0309 , // 0x0310          INQ_SIZE
 0x695F , 0x666E , 0x0037 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x5541 , 0x5F58 , 0x4E49 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , // 0x0320  _inf7...
 0xC012 , 0xC05A , 0xC00C , 0xC003 , 0xC001 , 0x031F , 0x695F , 0x666E , 0x0038 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4F20 , // 0x0330  ..Z........._inf
 0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05B , 0xC00C , 0xC003 , 0xC001 , 0x0335 , 0x695F , 0x666E , 0x0039 , 0xC016 , // 0x0340  UTQ_SIZE: ....[.
 0x2020 , 0x2020 , 0x2020 , 0x4120 , 0x5855 , 0x4F5F , 0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05C , 0xC00C , 0xC003 , // 0x0350         AUX_OUTQ_
 0xC001 , 0x034B , 0x695F , 0x666E , 0x0061 , 0xC016 , 0x2020 , 0x2020 , 0x414C , 0x5453 , 0x425F , 0x4655 , 0x4546 , 0x5F52 , 0x4953 , 0x455A , // 0x0360  ..K._infa...
 0x203A , 0x0000 , 0xC012 , 0xC086 , 0xC00C , 0xC003 , 0xC001 , 0x0361 , 0x695F , 0x666E , 0x0062 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , // 0x0370  : ............a.
 0x2020 , 0x6853 , 0x6C65 , 0x206C , 0x6953 , 0x657A , 0x203A , 0x0000 , 0xC012 , 0xC051 , 0xC00C , 0xC003 , 0xC001 , 0x0377 , 0x695F , 0x666E , // 0x0380    Shell Size: ..
 0x0063 , 0xC016 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x7520 , 0x6573 , 0x2064 , 0x7266 , 0x6565 , 0x203A , 0x0000 , 0xC012 , 0xC053 , // 0x0390  c...Dictionary u
 0xC045 , 0xC01E , 0xC00C , 0xC028 , 0xC00C , 0xC003 , 0xC001 , 0x038D , 0x695F , 0x666E , 0x0000 , 0xE289 , 0xE29F , 0xE2B5 , 0xE2CB , 0xE2E1 , // 0x03A0  E.....(.........
 0xE2F7 , 0xE30D , 0xE323 , 0xE339 , 0xE34F , 0xE365 , 0xE37B , 0xE391 , 0xC001 , 0x03A7 , 0x6E69 , 0x6F66 , 0x0000 , 0xC003 , 0xC052 , 0xC009 , // 0x03B0  ....#.9.O.e.{...
 0x0004 , 0xE230 , 0xC087 , 0xC003 , 0xC003 , 0xE3AB , 0xC001 , 0x03B9 , 0x745F , 0x3073 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4F43 , // 0x03C0  ..0.............
 0x4E55 , 0x0054 , 0xC012 , 0xC001 , 0x03C7 , 0x745F , 0x3173 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4D20 , 0x4E49 , 0x0000 , // 0x03D0  UNT......._ts1..
 0xC012 , 0xC001 , 0x03D4 , 0x745F , 0x3273 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x414D , 0x0058 , 0xC012 , 0xC001 , 0x03E2 , // 0x03E0  ......_ts2....
 0x745F , 0x3373 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x5641 , 0x5245 , 0x4741 , 0x0045 , 0xC012 , 0xC001 , 0x03EF , 0x745F , 0x3473 , // 0x03F0  _ts3....      AV
 0x0000 , 0xE3CB , 0xE3D8 , 0xE3E6 , 0xE3F3 , 0xC003 , 0xC003 , 0xC001 , 0x03FD , 0x745F , 0x3573 , 0x0000 , 0xC01E , 0xC066 , 0xC01E , 0xC024 , // 0x0400  ................
 0xC073 , 0xE417 , 0xC00C , 0xC00C , 0xC00C , 0xC00C , 0xC075 , 0xC025 , 0xC01E , 0xC073 , 0xE41E , 0xC040 , 0xC067 , 0xC075 , 0xC003 , 0xC001 , // 0x0410  s...........u.%.
 0x0408 , 0x7572 , 0x536E , 0x6174 , 0x7374 , 0x0000 , 0xE401 , 0xC009 , 0x0000 , 0xC076 , 0xE40C , 0xC073 , 0xE434 , 0xC009 , 0x0001 , 0xC027 , // 0x0420  ..runStats......
 0xC009 , 0x0000 , 0xC074 , 0xE437 , 0xC00B , 0xFFFF , 0xC075 , 0xC077 , 0xE42A , 0xC022 , 0xC001 , 0x0420 , 0x6572 , 0x6573 , 0x5374 , 0x6174 , // 0x0430  ....t.7.....u.w.
 0x7374 , 0x0000 , 0xC009 , 0x0000 , 0xC076 , 0xC01E , 0xC065 , 0xC073 , 0xE450 , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 0xC074 , 0xE453 , // 0x0440  ts......v...e.s.
 0xC00B , 0xFFFF , 0xC075 , 0xC077 , 0xE445 , 0xC022 , 0xC001 , 0x043B , 0x645F , 0x3072 , 0x0000 , 0xC01E , 0xC055 , 0xC02F , 0xC073 , 0xE467 , // 0x0450  ....u.w.E."...;.
 0xC01E , 0xC064 , 0xC00C , 0xC009 , 0x0001 , 0xC027 , 0xC075 , 0xC001 , 0x0457 , 0x645F , 0x3172 , 0x0000 , 0xC01E , 0xC00C , 0xC016 , 0x203A , // 0x0460  ..d.......'.u...
 0x0000 , 0xC012 , 0xC01E , 0xC064 , 0xC00C , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xC003 , 0xC001 , 0x0468 , // 0x0470  ......d...[.[.[.
 0x6572 , 0x7367 , 0x0000 , 0xC009 , 0x0000 , 0xC076 , 0xE46C , 0xC01E , 0xC055 , 0xC033 , 0xC077 , 0xE486 , 0xC022 , 0xC001 , 0x047F , 0x6572 , // 0x0480  regs......v.l...
 0x6F73 , 0x766C , 0x5265 , 0x6C65 , 0x7461 , 0x7669 , 0x0065 , 0xC01E , 0xC07F , 0xC02C , 0xC080 , 0xC031 , 0xC073 , 0xE4A7 , 0xC040 , 0xC07F , // 0x0490  solveRelative...
 0xC02C , 0xC01F , 0xC07F , 0xC081 , 0xC02C , 0xC02D , 0xC075 , 0xC001 , 0x048E , 0x645F , 0x3563 , 0x0000 , 0xC016 , 0x4520 , 0x5252 , 0x524F , // 0x04A0  ,.......,.-.u...
 0x205B , 0x0000 , 0xC012 , 0xC00C , 0xC016 , 0x205D , 0x0000 , 0xC012 , 0xC001 , 0x04A8 , 0x645F , 0x3463 , 0x0000 , 0xC01E , 0xE246 , 0xC031 , // 0x04B0  [ ........] ....
 0xC073 , 0xE4C9 , 0xC042 , 0xC009 , 0x0000 , 0xC00B , 0xFFFF , 0xC074 , 0xE4CC , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x04B9 , 0x645F , 0x3363 , // 0x04C0  s...B.........t.
 0x0000 , 0xC002 , 0xC049 , 0xC022 , 0xC00B , 0xFFFF , 0xC00B , 0xFFFF , 0xC001 , 0x04CD , 0x645F , 0x3263 , 0x0000 , 0xC048 , 0xC041 , 0xC04A , // 0x04D0  ....I.".........
 0xC001 , 0x04D9 , 0x6364 , 0x4E77 , 0x6D61 , 0x0065 , 0xE497 , 0xC035 , 0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xE4DD , 0xC036 , 0xC01E , 0xC035 , // 0x04E0  ....dcwName...5.
 0xC031 , 0xC073 , 0xE4F6 , 0xE4D1 , 0xC074 , 0xE4F8 , 0xE4BD , 0xC075 , 0xC077 , 0xE4EC , 0xC036 , 0xC01F , 0xC03E , 0xC073 , 0xE502 , 0xE4AC , // 0x04F0  1.s.....t.....u.
 0xC074 , 0xE504 , 0xC022 , 0xC075 , 0xC001 , 0x04E1 , 0x6364 , 0x6177 , 0x0000 , 0xC01E , 0xC07D , 0xC03D , 0xC073 , 0xE512 , 0xC00B , 0xFFFF , // 0x0500  t...".u.....dcwa
 0xC074 , 0xE529 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC016 , 0x7320 , 0x2027 , 0x0000 , 0xC012 , 0xC082 , 0xC009 , 0x0027 , 0xC04F , // 0x0510  t.).".....'.....
 0xC01E , 0xC043 , 0xC009 , 0x0002 , 0xC02A , 0xC027 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x0505 , 0x6364 , 0x3877 , 0x0000 , 0xC01E , 0xC079 , // 0x0520  ..C.....*.'.....
 0xC03D , 0xC073 , 0xE545 , 0xC01E , 0xC07A , 0xC03D , 0xC073 , 0xE53B , 0xE509 , 0xC074 , 0xE543 , 0xC022 , 0xC016 , 0x5D20 , 0x0000 , 0xC012 , // 0x0530  =.s.E...z.=.s.;.
 0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE550 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC016 , 0x5D20 , 0x005B , 0xC012 , 0xC009 , 0x0000 , 0xC075 , // 0x0540  ....u.t.P.".....
 0xC001 , 0x052A , 0x6364 , 0x3677 , 0x0000 , 0xC01E , 0xC07C , 0xC03D , 0xC073 , 0xE56F , 0xC01E , 0xC078 , 0xC03D , 0xC073 , 0xE562 , 0xE52E , // 0x0550  ..*.dcw6....|.=.
 0xC074 , 0xE56D , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC016 , 0x5B20 , 0x0000 , 0xC012 , 0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE57A , 0xC022 , // 0x0560  t.m.".....'... [
 0xC009 , 0x0001 , 0xC027 , 0xC016 , 0x7D20 , 0x0000 , 0xC012 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x0551 , 0x6364 , 0x3477 , 0x0000 , 0xC01E , // 0x0570  ....'... }......
 0xC071 , 0xC03D , 0xC073 , 0xE596 , 0xC01E , 0xC07B , 0xC03D , 0xC073 , 0xE58C , 0xE555 , 0xC074 , 0xE594 , 0xC022 , 0xC016 , 0x7B20 , 0x0000 , // 0x0580  q.=.s.....{.=.s.
 0xC012 , 0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE5A4 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC06A , 0xC00B , 0x0000 , 0xC027 , 0xC002 , // 0x0590  ......u.t..."...
 0xC083 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x057B , 0x6364 , 0x3377 , 0x0000 , 0xC01E , 0xC070 , 0xC03D , 0xC073 , 0xE5B1 , 0xE57F , 0xC074 , // 0x05A0  ......u...{.dcw3
 0xE5C6 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC06A , 0xC01F , 0xC009 , 0x0001 , 0xC027 , 0xC021 , 0xC06A , 0xC009 , 0x0010 , 0xC06B , // 0x05B0  ..".....'...j...
 0xC027 , 0xC002 , 0xC083 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x05A5 , 0x6364 , 0x3177 , 0x0000 , 0xC01E , 0xC06E , 0xC03D , 0xC073 , 0xE5E5 , // 0x05C0  '.........u.....
 0xC01E , 0xC06F , 0xC03D , 0xC073 , 0xE5D8 , 0xE5A9 , 0xC074 , 0xE5E3 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC06A , 0xC002 , 0xC083 , // 0x05D0  ..o.=.s.....t...
 0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE5E8 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x05C7 , 0x6364 , 0x0077 , 0xC01E , 0xC06A , 0xE5CB , 0xC073 , // 0x05E0  ....u.t.......u.
 0xE5F3 , 0xE4E6 , 0xC075 , 0xC001 , 0x05E9 , 0x645F , 0x3063 , 0x0000 , 0xC076 , 0xC01E , 0xC06A , 0xC06E , 0xC031 , 0xC073 , 0xE604 , 0xC022 , // 0x05F0  ....u....._dc0..
 0xC00B , 0xFFFF , 0xC074 , 0xE60B , 0xE5EC , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 0xC075 , 0xC077 , 0xE5F9 , 0xC001 , 0x05F4 , 0x6364 , // 0x0600  ....t.........'.
 0x0000 , 0xC016 , 0x203A , 0x0000 , 0xC012 , 0xC01E , 0xC012 , 0xC068 , 0xC01E , 0xC009 , 0xFFFF , 0xC03D , 0xC073 , 0xE621 , 0xE5F8 , 0xC074 , // 0x0610  ....: ........h.
 0xE623 , 0xE4AC , 0xC075 , 0xC003 , 0xC001 , 0x060E , 0x7564 , 0x3277 , 0x0000 , 0xC01E , 0xC06A , 0xC01E , 0xC011 , 0xC06E , 0xC031 , 0xC024 , // 0x0620  #...u.......duw2
 0xC009 , 0x0001 , 0xC027 , 0xC041 , 0xC030 , 0xC025 , 0xC02D , 0xC001 , 0x0625 , 0x7564 , 0x3177 , 0x0000 , 0xC01E , 0xC07E , 0xC073 , 0xE649 , // 0x0630  ....'.A.0.%.-...
 0xC016 , 0x4669 , 0x4E55 , 0x5443 , 0x4F49 , 0x004E , 0xC012 , 0xC074 , 0xE64E , 0xC076 , 0xE629 , 0xC077 , 0xE64A , 0xC075 , 0xC001 , 0x0638 , // 0x0640  ..iFUNCTION...t.
 0x7564 , 0x0077 , 0xC068 , 0xC01E , 0xC009 , 0xFFFF , 0xC03D , 0xC073 , 0xE668 , 0xC01E , 0xC011 , 0xC016 , 0x203A , 0x0000 , 0xC012 , 0xC01E , // 0x0650  duw.h.......=.s.
 0xC009 , 0x0100 , 0xC027 , 0xC01F , 0xE63C , 0xC042 , 0xC074 , 0xE66A , 0xC022 , 0xC075 , 0xC003 , 0xC001 , 0x064F , 0x645F , 0x6163 , 0x0000 , // 0x0660  ....'...<.B.t.j.
 0xC01E , 0xC07E , 0xC073 , 0xE67F , 0xC016 , 0x2F2F , 0x0020 , 0xC012 , 0xC022 , 0xC041 , 0xC049 , 0xC022 , 0xC003 , 0xC074 , 0xE68B , 0xC016 , // 0x0670  ..~.s.....// ...
 0x203A , 0x0000 , 0xC012 , 0xC035 , 0xC041 , 0xC049 , 0xC022 , 0xC036 , 0xE5F8 , 0xC003 , 0xC075 , 0xC001 , 0x066C , 0x4164 , 0x3154 , 0x0000 , // 0x0680  : ....5.A.I.".6.
 0xC023 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC035 , 0xC023 , 0xC023 , 0xC036 , 0xC036 , 0xC01E , 0xC035 , 0xC031 , 0xC001 , 0x068C , 0x4164 , // 0x0690  #.....'...5.#.#.
 0x0054 , 0xC035 , 0xC009 , 0x0000 , 0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xC048 , 0xC01E , 0xE246 , 0xC031 , 0xC073 , 0xE6B2 , 0xC00B , 0xFFFF , // 0x06A0  T.5.........F.v.
 0xC074 , 0xE6B4 , 0xE690 , 0xC075 , 0xC077 , 0xE6A8 , 0xC036 , 0xC022 , 0xC023 , 0xC022 , 0xC001 , 0x069E , 0x6364 , 0x6F43 , 0x6E75 , 0x0074 , // 0x06B0  t.....u.w...6.".
 0xC009 , 0x0000 , 0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xC048 , 0xC01E , 0xE246 , 0xC031 , 0xC073 , 0xE6D0 , 0xC00B , 0xFFFF , 0xC074 , 0xE6D9 , // 0x06C0  ........F.v.H...
 0xC023 , 0xC009 , 0x0001 , 0xC027 , 0xC023 , 0xC023 , 0xC009 , 0x0000 , 0xC075 , 0xC077 , 0xE6C6 , 0xC042 , 0xC001 , 0x06BB , 0x6364 , 0x6C41 , // 0x06D0  #.....'.#.#.....
 0x006C , 0xE6C0 , 0xC076 , 0xC01E , 0xE6A1 , 0xC041 , 0xC04A , 0xE670 , 0xC042 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC009 , 0x0000 , 0xC031 , // 0x06E0  l...v.....A.J.p.
 0xC077 , 0xE6E3 , 0xC001 , 0x06DD , 0x656B , 0x0079 , 0xC076 , 0xC04B , 0xC073 , 0xE6FD , 0xC00B , 0xFFFF , 0xC075 , 0xC077 , 0xE6F7 , 0xC001 , // 0x06F0  w.......key.v.K.
 0x06F3 , 0x7365 , 0x3F63 , 0x0000 , 0xC04B , 0xC073 , 0xE70B , 0xC009 , 0x001B , 0xC031 , 0xC075 , 0xC001 ,
 
 };
 static const CompiledDictionary interpreterCompiledDictionary( compiledDictionaryData, 0x0700 , 0x070C , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);
 


 */

/*
static uint16_t compiledDictionaryData[] = {
};
YRShellCompiledDictionary compiledDictionary( compiledDictionaryData, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_COMPILED);
*/

static const uint16_t compiledDictionaryData[] = {
    0xFFFF , 0x7370 , 0x003F , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , 0x2052 , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC037 , 0xC03F , // 0x0000  ..ps?...PARAMETE
    0xC073 , 0xE020 , 0xC037 , 0xC076 , 0xC01E , 0xC03A , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC077 , 0xE014 , 0xC022 , 0xC075 , // 0x0010  s. .7.v...:.....
    0xC003 , 0xC001 , 0x0000 , 0x7372 , 0x003F , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 0x204E , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , // 0x0020  ......rs?...   R
    0xC038 , 0xC03F , 0xC073 , 0xE047 , 0xC038 , 0xC009 , 0x0001 , 0xC028 , 0xC076 , 0xC01E , 0xC03B , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , // 0x0030  8.?.s.G.8.....(.
    0xC00B , 0xFFFF , 0xC031 , 0xC077 , 0xE039 , 0xC022 , 0xC075 , 0xC003 , 0xC001 , 0x0022 , 0x7363 , 0x003F , 0xC016 , 0x2020 , 0x4F43 , 0x504D , // 0x0040  ....1.w.9.".u...
    0x4C49 , 0x2045 , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC039 , 0xC03F , 0xC073 , 0xE06E , 0xC039 , 0xC009 , 0x0001 , 0xC028 , 0xC076 , // 0x0050  ILE STACK: ...9.
    0xC01E , 0xC03C , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC00B , 0xFFFF , 0xC031 , 0xC077 , 0xE060 , 0xC022 , 0xC075 , 0xC003 , 0xC001 , // 0x0060  ..<.......(.....
    0x0049 , 0x7473 , 0x003F , 0xE003 , 0xE025 , 0xE04C , 0xC001 , 0x0070 , 0x635F , 0x3064 , 0x0000 , 0xC016 , 0x730D , 0x6174 , 0x6974 , 0x2063 , // 0x0070  I.st?...%.L...p.
    0x6F63 , 0x736E , 0x2074 , 0x6975 , 0x746E , 0x3631 , 0x745F , 0x0020 , 0xC012 , 0xC001 , 0x0077 , 0x635F , 0x6378 , 0x0000 , 0xC01E , 0xC009 , // 0x0080  const uint16_t .
    0x0020 , 0xC02F , 0xC040 , 0xC009 , 0x007E , 0xC032 , 0xC02D , 0xC073 , 0xE09D , 0xC022 , 0xC009 , 0x002E , 0xC075 , 0xC04F , 0xC001 , 0x008A , // 0x0090   ./.@...~.2.-.s.
    0x635F , 0x6278 , 0x0000 , 0xC01E , 0xC009 , 0x00FF , 0xC02C , 0xE08E , 0xC009 , 0x0008 , 0xC06D , 0xE08E , 0xC009 , 0x0001 , 0xC027 , 0xC001 , // 0x00A0  _cxb........,...
    0x009F , 0x635F , 0x6178 , 0x0000 , 0xC01E , 0xC044 , 0xE0A3 , 0xC001 , 0x00B0 , 0x635F , 0x3078 , 0x0000 , 0xE0B4 , 0xE0B4 , 0xE0B4 , 0xE0B4 , // 0x00B0  .._cxa....D.....
    0xE0B4 , 0xE0B4 , 0xE0B4 , 0xE0B4 , 0xC022 , 0xC001 , 0x00B8 , 0x635F , 0x3164 , 0x0000 , 0xE07B , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , // 0x00C0  ........"....._c
    0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x6144 , 0x6174 , 0x5D5B , 0x3D20 , 0x7B20 , 0x000D , 0xC012 , 0xC001 , 0x00C6 , 0x635F , 0x3178 , // 0x00D0  DictionaryData[]
    0x0000 , 0xE07B , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 0x446E , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , // 0x00E0  ..{...compiledEx
    0x4479 , 0x7461 , 0x5B61 , 0x205D , 0x203D , 0x0D7B , 0x0000 , 0xC012 , 0xC001 , 0x00DD , 0x635F , 0x3264 , 0x0000 , 0xC01E , 0xC009 , 0x0010 , // 0x00F0  yData[] = {.....
    0xC028 , 0xC009 , 0x000F , 0xC02C , 0xC03E , 0xC073 , 0xE115 , 0xC016 , 0x2F2F , 0x0020 , 0xC012 , 0xC01E , 0xC009 , 0x0010 , 0xC028 , 0xC01E , // 0x0100  (.....,.>.s.....
    0xC011 , 0xC002 , 0xE0BC , 0xC003 , 0xC075 , 0xC001 , 0x00F9 , 0x635F , 0x3364 , 0x0000 , 0xC009 , 0x0000 , 0xC076 , 0xC01E , 0xC044 , 0xC011 , // 0x0110  ........u....._c
    0xC016 , 0x202C , 0x0000 , 0xC012 , 0xC009 , 0x0001 , 0xC027 , 0xE0FD , 0xC01E , 0xC045 , 0xC033 , 0xC077 , 0xE11D , 0xC022 , 0xC001 , 0x0116 , // 0x0120  .., ........'...
    0x635F , 0x3464 , 0x0000 , 0xC045 , 0xC03F , 0xC073 , 0xE13A , 0xE11A , 0xC005 , 0xC075 , 0xC016 , 0x3B7D , 0x000D , 0xC012 , 0xC001 , 0x012F , // 0x0130  _cd4..E.?.s.:...
    0x635F , 0x3564 , 0x0000 , 0xC016 , 0x7473 , 0x7461 , 0x6369 , 0x6320 , 0x6E6F , 0x7473 , 0x4320 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , // 0x0140  _cd5....static c
    0x6974 , 0x6E6F , 0x7261 , 0x2079 , 0x0000 , 0xC012 , 0xC001 , 0x013F , 0x635F , 0x3578 , 0x0000 , 0xC016 , 0x7473 , 0x7461 , 0x6369 , 0x4320 , // 0x0150  tionary ......?.
    0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x2079 , 0x0000 , 0xC012 , 0xC001 , 0x0157 , 0x635F , 0x3664 , 0x0000 , // 0x0160  ompiledDictionar
    0xC016 , 0x6E69 , 0x6574 , 0x7072 , 0x6572 , 0x6574 , 0x4372 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x2879 , // 0x0170  ..interpreterCom
    0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x4479 , 0x7461 , 0x2C61 , 0x0020 , 0xC012 , 0xC001 , 0x016C , // 0x0180   compiledDiction
    0x635F , 0x3678 , 0x0000 , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 0x446E , 0x6369 , 0x6974 , 0x6E6F , // 0x0190  _cx6....compiled
    0x7261 , 0x2879 , 0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4564 , 0x7478 , 0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , // 0x01A0  ary( compiledExt
    0x6144 , 0x6174 , 0x202C , 0x0000 , 0xC012 , 0xC001 , 0x018F , 0x635F , 0x3764 , 0x0000 , 0xC046 , 0xC011 , 0xC016 , 0x202C , 0x0000 , 0xC012 , // 0x01B0  Data, ........_c
    0xC045 , 0xC011 , 0xC001 , 0x01B6 , 0x635F , 0x3864 , 0x0000 , 0xE0CA , 0xE133 , 0xE143 , 0xE170 , 0xE1BA , 0xC016 , 0x202C , 0x5259 , 0x4853 , // 0x01C0  E......._cd8....
    0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x495F , 0x544E , 0x5245 , 0x5250 , 0x5445 , 0x5245 , 0x435F , 0x4D4F , 0x4950 , // 0x01D0  ELL_DICTIONARY_I
    0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x01C3 , 0x635F , 0x3878 , 0x0000 , 0xE0E1 , 0xE133 , 0xE143 , 0xE193 , 0xE1BA , 0xC016 , // 0x01E0  LED);........._c
    0x202C , 0x5259 , 0x4853 , 0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x455F , 0x5458 , 0x4E45 , 0x4953 , 0x4E4F , 0x435F , // 0x01F0  , YRSHELL_DICTIO
    0x4D4F , 0x4950 , 0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x01E6 , 0x6F63 , 0x706D , 0x6C69 , 0x4965 , 0x746E , 0x7265 , 0x7270 , // 0x0200  OMPILED);.......
    0x7465 , 0x7265 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE1C7 , 0xC001 , 0x0208 , 0x6F63 , 0x706D , 0x6C69 , 0x4565 , 0x7478 , // 0x0210  eterDictionary..
    0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE1EA , 0xC001 , 0x021A , 0x7073 , 0x6361 , 0x7365 , 0x0000 , // 0x0220  ensionDictionary
    0xC01E , 0xC073 , 0xE23D , 0xC076 , 0xC002 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC077 , 0xE234 , 0xC075 , 0xC022 , 0xC001 , 0x022B , // 0x0230  ..s.=.v.......(.
    0x6964 , 0x7463 , 0x6E49 , 0x6176 , 0x696C , 0x0064 , 0xC009 , 0xFFFF , 0xC001 , 0x023F , 0x775F , 0x326C , 0x0000 , 0xC023 , 0xC01E , 0xC073 , // 0x0240  dictInvalid.....
    0xE256 , 0xC036 , 0xC022 , 0xC003 , 0xC074 , 0xE259 , 0xC036 , 0xE230 , 0xC075 , 0xC03E , 0xC023 , 0xC023 , 0xC01E , 0xE246 , 0xC031 , 0xC001 , // 0x0250  V.6."...t.Y.6.0.
    0x0249 , 0x775F , 0x336C , 0x0000 , 0xC049 , 0xC009 , 0x0028 , 0xC01F , 0xC028 , 0xC035 , 0xC001 , 0x0260 , 0x6C77 , 0x0000 , 0xC009 , 0x0000 , // 0x0260  I._wl3..I...(...
    0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xC048 , 0xC040 , 0xC010 , 0xC01E , 0xC011 , 0xC041 , 0xC04A , 0xC011 , 0xC041 , 0xE264 , 0xE24D , 0xC077 , // 0x0270  ....F.v.H.@.....
    0xE274 , 0xC042 , 0xC022 , 0xC003 , 0xC001 , 0x026B , 0x695F , 0x666E , 0x0030 , 0xC016 , 0x2020 , 0x2020 , 0x4420 , 0x4349 , 0x4954 , 0x4E4F , // 0x0280  t.B.".....k._inf
    0x5241 , 0x5F59 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC053 , 0xC00C , 0xC003 , 0xC001 , 0x0285 , 0x695F , 0x666E , 0x0031 , 0xC016 , // 0x0290  ARY_SIZE: ....S.
    0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4150 , 0x5F44 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC054 , 0xC00C , 0xC003 , // 0x02A0              PAD_
    0xC001 , 0x029B , 0x695F , 0x666E , 0x0032 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4E20 , 0x4D55 , 0x525F , 0x4745 , 0x5349 , 0x4554 , 0x5352 , // 0x02B0  ...._inf2...
    0x203A , 0x0000 , 0xC012 , 0xC055 , 0xC00C , 0xC003 , 0xC001 , 0x02B1 , 0x695F , 0x666E , 0x0033 , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , // 0x02C0  : ....U.........
    0x5F52 , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC056 , 0xC00C , 0xC003 , 0xC001 , 0x02C7 , 0x695F , 0x666E , // 0x02D0  R_STACK_SIZE: ..
    0x0034 , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 0x5F4E , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC057 , // 0x02E0  4...   RETURN_ST
    0xC00C , 0xC003 , 0xC001 , 0x02DD , 0x695F , 0x666E , 0x0035 , 0xC016 , 0x2020 , 0x4F43 , 0x504D , 0x4C49 , 0x5F45 , 0x5453 , 0x4341 , 0x5F4B , // 0x02F0  ........_inf5...
    0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC058 , 0xC00C , 0xC003 , 0xC001 , 0x02F3 , 0x695F , 0x666E , 0x0036 , 0xC016 , 0x2020 , 0x2020 , // 0x0300  SIZE: ....X.....
    0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4E49 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC059 , 0xC00C , 0xC003 , 0xC001 , 0x0309 , // 0x0310          INQ_SIZE
    0x695F , 0x666E , 0x0037 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x5541 , 0x5F58 , 0x4E49 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , // 0x0320  _inf7...
    0xC012 , 0xC05A , 0xC00C , 0xC003 , 0xC001 , 0x031F , 0x695F , 0x666E , 0x0038 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4F20 , // 0x0330  ..Z........._inf
    0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05B , 0xC00C , 0xC003 , 0xC001 , 0x0335 , 0x695F , 0x666E , 0x0039 , 0xC016 , // 0x0340  UTQ_SIZE: ....[.
    0x2020 , 0x2020 , 0x2020 , 0x4120 , 0x5855 , 0x4F5F , 0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05C , 0xC00C , 0xC003 , // 0x0350         AUX_OUTQ_
    0xC001 , 0x034B , 0x695F , 0x666E , 0x0061 , 0xC016 , 0x2020 , 0x2020 , 0x414C , 0x5453 , 0x425F , 0x4655 , 0x4546 , 0x5F52 , 0x4953 , 0x455A , // 0x0360  ..K._infa...
    0x203A , 0x0000 , 0xC012 , 0xC086 , 0xC00C , 0xC003 , 0xC001 , 0x0361 , 0x695F , 0x666E , 0x0062 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , // 0x0370  : ............a.
    0x2020 , 0x6853 , 0x6C65 , 0x206C , 0x6953 , 0x657A , 0x203A , 0x0000 , 0xC012 , 0xC051 , 0xC00C , 0xC003 , 0xC001 , 0x0377 , 0x695F , 0x666E , // 0x0380    Shell Size: ..
    0x0063 , 0xC016 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x7520 , 0x6573 , 0x2064 , 0x7266 , 0x6565 , 0x203A , 0x0000 , 0xC012 , 0xC053 , // 0x0390  c...Dictionary u
    0xC045 , 0xC01E , 0xC00C , 0xC028 , 0xC00C , 0xC003 , 0xC001 , 0x038D , 0x695F , 0x666E , 0x0000 , 0xE289 , 0xE29F , 0xE2B5 , 0xE2CB , 0xE2E1 , // 0x03A0  E.....(.........
    0xE2F7 , 0xE30D , 0xE323 , 0xE339 , 0xE34F , 0xE365 , 0xE37B , 0xE391 , 0xC001 , 0x03A7 , 0x6E69 , 0x6F66 , 0x0000 , 0xC003 , 0xC052 , 0xC009 , // 0x03B0  ....#.9.O.e.{...
    0x0004 , 0xE230 , 0xC087 , 0xC003 , 0xC003 , 0xE3AB , 0xC001 , 0x03B9 , 0x745F , 0x3073 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4F43 , // 0x03C0  ..0.............
    0x4E55 , 0x0054 , 0xC012 , 0xC001 , 0x03C7 , 0x745F , 0x3173 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4D20 , 0x4E49 , 0x0000 , // 0x03D0  UNT......._ts1..
    0xC012 , 0xC001 , 0x03D4 , 0x745F , 0x3273 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x414D , 0x0058 , 0xC012 , 0xC001 , 0x03E2 , // 0x03E0  ......_ts2....
    0x745F , 0x3373 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x5641 , 0x5245 , 0x4741 , 0x0045 , 0xC012 , 0xC001 , 0x03EF , 0x745F , 0x3473 , // 0x03F0  _ts3....      AV
    0x0000 , 0xE3CB , 0xE3D8 , 0xE3E6 , 0xE3F3 , 0xC003 , 0xC003 , 0xC001 , 0x03FD , 0x745F , 0x3573 , 0x0000 , 0xC01E , 0xC066 , 0xC01E , 0xC024 , // 0x0400  ................
    0xC073 , 0xE417 , 0xC00C , 0xC00C , 0xC00C , 0xC00C , 0xC075 , 0xC025 , 0xC01E , 0xC073 , 0xE41E , 0xC040 , 0xC067 , 0xC075 , 0xC003 , 0xC001 , // 0x0410  s...........u.%.
    0x0408 , 0x7572 , 0x536E , 0x6174 , 0x7374 , 0x0000 , 0xE401 , 0xC009 , 0x0000 , 0xC076 , 0xE40C , 0xC073 , 0xE434 , 0xC009 , 0x0001 , 0xC027 , // 0x0420  ..runStats......
    0xC009 , 0x0000 , 0xC074 , 0xE437 , 0xC00B , 0xFFFF , 0xC075 , 0xC077 , 0xE42A , 0xC022 , 0xC001 , 0x0420 , 0x6572 , 0x6573 , 0x5374 , 0x6174 , // 0x0430  ....t.7.....u.w.
    0x7374 , 0x0000 , 0xC009 , 0x0000 , 0xC076 , 0xC01E , 0xC065 , 0xC073 , 0xE450 , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 0xC074 , 0xE453 , // 0x0440  ts......v...e.s.
    0xC00B , 0xFFFF , 0xC075 , 0xC077 , 0xE445 , 0xC022 , 0xC001 , 0x043B , 0x645F , 0x3072 , 0x0000 , 0xC01E , 0xC055 , 0xC02F , 0xC073 , 0xE467 , // 0x0450  ....u.w.E."...;.
    0xC01E , 0xC064 , 0xC00C , 0xC009 , 0x0001 , 0xC027 , 0xC075 , 0xC001 , 0x0457 , 0x645F , 0x3172 , 0x0000 , 0xC01E , 0xC00C , 0xC016 , 0x203A , // 0x0460  ..d.......'.u...
    0x0000 , 0xC012 , 0xC01E , 0xC064 , 0xC00C , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xE45B , 0xC003 , 0xC001 , 0x0468 , // 0x0470  ......d...[.[.[.
    0x6572 , 0x7367 , 0x0000 , 0xC009 , 0x0000 , 0xC076 , 0xE46C , 0xC01E , 0xC055 , 0xC033 , 0xC077 , 0xE486 , 0xC022 , 0xC001 , 0x047F , 0x6572 , // 0x0480  regs......v.l...
    0x6F73 , 0x766C , 0x5265 , 0x6C65 , 0x7461 , 0x7669 , 0x0065 , 0xC01E , 0xC07F , 0xC02C , 0xC080 , 0xC031 , 0xC073 , 0xE4A7 , 0xC040 , 0xC07F , // 0x0490  solveRelative...
    0xC02C , 0xC01F , 0xC07F , 0xC081 , 0xC02C , 0xC02D , 0xC075 , 0xC001 , 0x048E , 0x645F , 0x3563 , 0x0000 , 0xC016 , 0x4520 , 0x5252 , 0x524F , // 0x04A0  ,.......,.-.u...
    0x205B , 0x0000 , 0xC012 , 0xC00C , 0xC016 , 0x205D , 0x0000 , 0xC012 , 0xC001 , 0x04A8 , 0x645F , 0x3463 , 0x0000 , 0xC01E , 0xE246 , 0xC031 , // 0x04B0  [ ........] ....
    0xC073 , 0xE4C9 , 0xC042 , 0xC009 , 0x0000 , 0xC00B , 0xFFFF , 0xC074 , 0xE4CC , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x04B9 , 0x645F , 0x3363 , // 0x04C0  s...B.........t.
    0x0000 , 0xC002 , 0xC049 , 0xC022 , 0xC00B , 0xFFFF , 0xC00B , 0xFFFF , 0xC001 , 0x04CD , 0x645F , 0x3263 , 0x0000 , 0xC048 , 0xC041 , 0xC04A , // 0x04D0  ....I.".........
    0xC001 , 0x04D9 , 0x6364 , 0x4E77 , 0x6D61 , 0x0065 , 0xE497 , 0xC035 , 0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xE4DD , 0xC036 , 0xC01E , 0xC035 , // 0x04E0  ....dcwName...5.
    0xC031 , 0xC073 , 0xE4F6 , 0xE4D1 , 0xC074 , 0xE4F8 , 0xE4BD , 0xC075 , 0xC077 , 0xE4EC , 0xC036 , 0xC01F , 0xC03E , 0xC073 , 0xE502 , 0xE4AC , // 0x04F0  1.s.....t.....u.
    0xC074 , 0xE504 , 0xC022 , 0xC075 , 0xC001 , 0x04E1 , 0x6364 , 0x6177 , 0x0000 , 0xC01E , 0xC07D , 0xC03D , 0xC073 , 0xE512 , 0xC00B , 0xFFFF , // 0x0500  t...".u.....dcwa
    0xC074 , 0xE529 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC016 , 0x7320 , 0x2027 , 0x0000 , 0xC012 , 0xC082 , 0xC009 , 0x0027 , 0xC04F , // 0x0510  t.).".....'.....
    0xC01E , 0xC043 , 0xC009 , 0x0002 , 0xC02A , 0xC027 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x0505 , 0x6364 , 0x3877 , 0x0000 , 0xC01E , 0xC079 , // 0x0520  ..C.....*.'.....
    0xC03D , 0xC073 , 0xE545 , 0xC01E , 0xC07A , 0xC03D , 0xC073 , 0xE53B , 0xE509 , 0xC074 , 0xE543 , 0xC022 , 0xC016 , 0x5D20 , 0x0000 , 0xC012 , // 0x0530  =.s.E...z.=.s.;.
    0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE550 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC016 , 0x5D20 , 0x005B , 0xC012 , 0xC009 , 0x0000 , 0xC075 , // 0x0540  ....u.t.P.".....
    0xC001 , 0x052A , 0x6364 , 0x3677 , 0x0000 , 0xC01E , 0xC07C , 0xC03D , 0xC073 , 0xE56F , 0xC01E , 0xC078 , 0xC03D , 0xC073 , 0xE562 , 0xE52E , // 0x0550  ..*.dcw6....|.=.
    0xC074 , 0xE56D , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC016 , 0x5B20 , 0x0000 , 0xC012 , 0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE57A , 0xC022 , // 0x0560  t.m.".....'... [
    0xC009 , 0x0001 , 0xC027 , 0xC016 , 0x7D20 , 0x0000 , 0xC012 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x0551 , 0x6364 , 0x3477 , 0x0000 , 0xC01E , // 0x0570  ....'... }......
    0xC071 , 0xC03D , 0xC073 , 0xE596 , 0xC01E , 0xC07B , 0xC03D , 0xC073 , 0xE58C , 0xE555 , 0xC074 , 0xE594 , 0xC022 , 0xC016 , 0x7B20 , 0x0000 , // 0x0580  q.=.s.....{.=.s.
    0xC012 , 0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE5A4 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC06A , 0xC00B , 0x0000 , 0xC027 , 0xC002 , // 0x0590  ......u.t..."...
    0xC083 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x057B , 0x6364 , 0x3377 , 0x0000 , 0xC01E , 0xC070 , 0xC03D , 0xC073 , 0xE5B1 , 0xE57F , 0xC074 , // 0x05A0  ......u...{.dcw3
    0xE5C6 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC06A , 0xC01F , 0xC009 , 0x0001 , 0xC027 , 0xC021 , 0xC06A , 0xC009 , 0x0010 , 0xC06B , // 0x05B0  ..".....'...j...
    0xC027 , 0xC002 , 0xC083 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x05A5 , 0x6364 , 0x3177 , 0x0000 , 0xC01E , 0xC06E , 0xC03D , 0xC073 , 0xE5E5 , // 0x05C0  '.........u.....
    0xC01E , 0xC06F , 0xC03D , 0xC073 , 0xE5D8 , 0xE5A9 , 0xC074 , 0xE5E3 , 0xC022 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC06A , 0xC002 , 0xC083 , // 0x05D0  ..o.=.s.....t...
    0xC009 , 0x0000 , 0xC075 , 0xC074 , 0xE5E8 , 0xC009 , 0x0000 , 0xC075 , 0xC001 , 0x05C7 , 0x6364 , 0x0077 , 0xC01E , 0xC06A , 0xE5CB , 0xC073 , // 0x05E0  ....u.t.......u.
    0xE5F3 , 0xE4E6 , 0xC075 , 0xC001 , 0x05E9 , 0x645F , 0x3063 , 0x0000 , 0xC076 , 0xC01E , 0xC06A , 0xC06E , 0xC031 , 0xC073 , 0xE604 , 0xC022 , // 0x05F0  ....u....._dc0..
    0xC00B , 0xFFFF , 0xC074 , 0xE60B , 0xE5EC , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 0xC075 , 0xC077 , 0xE5F9 , 0xC001 , 0x05F4 , 0x6364 , // 0x0600  ....t.........'.
    0x0000 , 0xC016 , 0x203A , 0x0000 , 0xC012 , 0xC01E , 0xC012 , 0xC068 , 0xC01E , 0xC009 , 0xFFFF , 0xC03D , 0xC073 , 0xE621 , 0xE5F8 , 0xC074 , // 0x0610  ....: ........h.
    0xE623 , 0xE4AC , 0xC075 , 0xC003 , 0xC001 , 0x060E , 0x7564 , 0x3277 , 0x0000 , 0xC01E , 0xC06A , 0xC01E , 0xC011 , 0xC06E , 0xC031 , 0xC024 , // 0x0620  #...u.......duw2
    0xC009 , 0x0001 , 0xC027 , 0xC041 , 0xC030 , 0xC025 , 0xC02D , 0xC001 , 0x0625 , 0x7564 , 0x3177 , 0x0000 , 0xC01E , 0xC07E , 0xC073 , 0xE649 , // 0x0630  ....'.A.0.%.-...
    0xC016 , 0x4669 , 0x4E55 , 0x5443 , 0x4F49 , 0x004E , 0xC012 , 0xC074 , 0xE64E , 0xC076 , 0xE629 , 0xC077 , 0xE64A , 0xC075 , 0xC001 , 0x0638 , // 0x0640  ..iFUNCTION...t.
    0x7564 , 0x0077 , 0xC068 , 0xC01E , 0xC009 , 0xFFFF , 0xC03D , 0xC073 , 0xE668 , 0xC01E , 0xC011 , 0xC016 , 0x203A , 0x0000 , 0xC012 , 0xC01E , // 0x0650  duw.h.......=.s.
    0xC009 , 0x0100 , 0xC027 , 0xC01F , 0xE63C , 0xC042 , 0xC074 , 0xE66A , 0xC022 , 0xC075 , 0xC003 , 0xC001 , 0x064F , 0x645F , 0x6163 , 0x0000 , // 0x0660  ....'...<.B.t.j.
    0xC01E , 0xC07E , 0xC073 , 0xE67F , 0xC016 , 0x2F2F , 0x0020 , 0xC012 , 0xC022 , 0xC041 , 0xC049 , 0xC022 , 0xC003 , 0xC074 , 0xE68B , 0xC016 , // 0x0670  ..~.s.....// ...
    0x203A , 0x0000 , 0xC012 , 0xC035 , 0xC041 , 0xC049 , 0xC022 , 0xC036 , 0xE5F8 , 0xC003 , 0xC075 , 0xC001 , 0x066C , 0x4164 , 0x3154 , 0x0000 , // 0x0680  : ....5.A.I.".6.
    0xC023 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC035 , 0xC023 , 0xC023 , 0xC036 , 0xC036 , 0xC01E , 0xC035 , 0xC031 , 0xC001 , 0x068C , 0x4164 , // 0x0690  #.....'...5.#.#.
    0x0054 , 0xC035 , 0xC009 , 0x0000 , 0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xC048 , 0xC01E , 0xE246 , 0xC031 , 0xC073 , 0xE6B2 , 0xC00B , 0xFFFF , // 0x06A0  T.5.........F.v.
    0xC074 , 0xE6B4 , 0xE690 , 0xC075 , 0xC077 , 0xE6A8 , 0xC036 , 0xC022 , 0xC023 , 0xC022 , 0xC001 , 0x069E , 0x6364 , 0x6F43 , 0x6E75 , 0x0074 , // 0x06B0  t.....u.w...6.".
    0xC009 , 0x0000 , 0xC009 , 0x0000 , 0xE246 , 0xC076 , 0xC048 , 0xC01E , 0xE246 , 0xC031 , 0xC073 , 0xE6D0 , 0xC00B , 0xFFFF , 0xC074 , 0xE6D9 , // 0x06C0  ........F.v.H...
    0xC023 , 0xC009 , 0x0001 , 0xC027 , 0xC023 , 0xC023 , 0xC009 , 0x0000 , 0xC075 , 0xC077 , 0xE6C6 , 0xC042 , 0xC001 , 0x06BB , 0x6364 , 0x6C41 , // 0x06D0  #.....'.#.#.....
    0x006C , 0xE6C0 , 0xC076 , 0xC01E , 0xE6A1 , 0xC041 , 0xC04A , 0xE670 , 0xC042 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC009 , 0x0000 , 0xC031 , // 0x06E0  l...v.....A.J.p.
    0xC077 , 0xE6E3 , 0xC001 , 0x06DD , 0x656B , 0x0079 , 0xC076 , 0xC04B , 0xC073 , 0xE6FD , 0xC00B , 0xFFFF , 0xC075 , 0xC077 , 0xE6F7 , 0xC001 , // 0x06F0  w.......key.v.K.
    0x06F3 , 0x7365 , 0x3F63 , 0x0000 , 0xC04B , 0xC073 , 0xE70B , 0xC009 , 0x001B , 0xC031 , 0xC075 , 0xC001 ,
    
};
static const CompiledDictionary interpreterCompiledDictionary( compiledDictionaryData, 0x0700 , 0x070C , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);


static const FunctionEntry interpreterFunctions[] = {
    { (uint16_t)YRShellInterpreter::SI_CC_return,     ""},
    { (uint16_t)YRShellInterpreter::SI_CC_space,      "space"},
    { (uint16_t)YRShellInterpreter::SI_CC_cr,         "cr"},
    { (uint16_t)YRShellInterpreter::SI_CC_lf,         "lf"},
    { (uint16_t)YRShellInterpreter::SI_CC_crlf,       "crlf"},
    { (uint16_t)YRShellInterpreter::SI_CC_reset,      "reset"},
    { (uint16_t)YRShellInterpreter::SI_CC_prompt,     "prompt"},
    { (uint16_t)YRShellInterpreter::SI_CC_clearPad,   "clearPad"},
    { (uint16_t)YRShellInterpreter::SI_CC_dot,        "."},
    { (uint16_t)YRShellInterpreter::SI_CC_dotByte,    ".b"},
    { (uint16_t)YRShellInterpreter::SI_CC_dotWord,    ".w"},
    { (uint16_t)YRShellInterpreter::SI_CC_dotX,       ".x"},
    { (uint16_t)YRShellInterpreter::SI_CC_dotByteX,   ".bx"},
    { (uint16_t)YRShellInterpreter::SI_CC_dotWordX,   ".wx"},
    { (uint16_t)YRShellInterpreter::SI_CC_dotStr,     ".str"},
    { (uint16_t)YRShellInterpreter::SI_CC_hex,        "hex"},
    { (uint16_t)YRShellInterpreter::SI_CC_decimal,    "decimal"},
    { (uint16_t)YRShellInterpreter::SI_CC_stringDef,  "s'"},
    { (uint16_t)YRShellInterpreter::SI_CC_if,         "["},
    { (uint16_t)YRShellInterpreter::SI_CC_else,       "]["},
    { (uint16_t)YRShellInterpreter::SI_CC_then,       "]"},
    { (uint16_t)YRShellInterpreter::SI_CC_begin,      "{"},
    { (uint16_t)YRShellInterpreter::SI_CC_until,      "}"},
    { (uint16_t)YRShellInterpreter::SI_CC_dup,        "dup"},
    { (uint16_t)YRShellInterpreter::SI_CC_swap,       "swap"},
    { (uint16_t)YRShellInterpreter::SI_CC_nip,        "nip"},
    { (uint16_t)YRShellInterpreter::SI_CC_tuck,       "tuck"},
    { (uint16_t)YRShellInterpreter::SI_CC_drop,       "drop"},
    { (uint16_t)YRShellInterpreter::SI_CC_rot,        "rot"},
    { (uint16_t)YRShellInterpreter::SI_CC_toR,        ">r"},
    { (uint16_t)YRShellInterpreter::SI_CC_fromR,      "r>"},
    { (uint16_t)YRShellInterpreter::SI_CC_negate,     "neg"},
    { (uint16_t)YRShellInterpreter::SI_CC_plus,       "+"},
    { (uint16_t)YRShellInterpreter::SI_CC_minus,      "-"},
    { (uint16_t)YRShellInterpreter::SI_CC_multiply,   "*"},
    { (uint16_t)YRShellInterpreter::SI_CC_divide,     "/"},
    { (uint16_t)YRShellInterpreter::SI_CC_mod,        "%"},
    { (uint16_t)YRShellInterpreter::SI_CC_and,        "&"},
    { (uint16_t)YRShellInterpreter::SI_CC_or,         "|"},
    { (uint16_t)YRShellInterpreter::SI_CC_xor,        "^"},
    { (uint16_t)YRShellInterpreter::SI_CC_lessThan,   "<"},
    { (uint16_t)YRShellInterpreter::SI_CC_lessEqual,  "<="},
    { (uint16_t)YRShellInterpreter::SI_CC_equal,      "=="},
    { (uint16_t)YRShellInterpreter::SI_CC_greaterThan,        ">"},
    { (uint16_t)YRShellInterpreter::SI_CC_greaterThanEqual,   ">="},
    { (uint16_t)YRShellInterpreter::SI_CC_setDebug,   "setDebug"},
    { (uint16_t)YRShellInterpreter::SI_CC_toC,        ">c"},
    { (uint16_t)YRShellInterpreter::SI_CC_fromC,      "c>"},
    { (uint16_t)YRShellInterpreter::SI_CC_parameterStackDepth,    "psd?"},
    { (uint16_t)YRShellInterpreter::SI_CC_returnStackDepth,       "rsd?"},
    { (uint16_t)YRShellInterpreter::SI_CC_compileStackDepth,      "csd?"},
    { (uint16_t)YRShellInterpreter::SI_CC_parameterStackAt,       "ps@" },
    { (uint16_t)YRShellInterpreter::SI_CC_returnStackAt,          "rs@" },
    { (uint16_t)YRShellInterpreter::SI_CC_compileStackAt,         "cs@" },
    { (uint16_t)YRShellInterpreter::SI_CC_notEqual,               "!=" },
    { (uint16_t)YRShellInterpreter::SI_CC_zeroEqual,              "0==" },
    { (uint16_t)YRShellInterpreter::SI_CC_zeroNotEqual,           "0!=" },
    { (uint16_t)YRShellInterpreter::SI_CC_over,                   "over" },
    { (uint16_t)YRShellInterpreter::SI_CC_2dup,                   "2dup" },
    { (uint16_t)YRShellInterpreter::SI_CC_2drop,                  "2drop" },
    { (uint16_t)YRShellInterpreter::SI_CC_strlen,                 "strlen" },
    { (uint16_t)YRShellInterpreter::SI_CC_getCurrentDictionary,                   "getCurrentDictionary" },
    { (uint16_t)YRShellInterpreter::SI_CC_getCurrentDictionaryEnd,                "getCurrentDictionaryEnd" },
    { (uint16_t)YRShellInterpreter::SI_CC_getCurrentDictionaryLastWord,           "getCurrentDictionaryLastWord" },
    { (uint16_t)YRShellInterpreter::SI_CC_delay,                                  "delay" },
    
    { (uint16_t)YRShellInterpreter::SI_CC_nextEntry,                              "nextEntry" },
    { (uint16_t)YRShellInterpreter::SI_CC_dotEntryName,                           ".entryName" },
    { (uint16_t)YRShellInterpreter::SI_CC_entryToken,                             "entryToken" },
    { (uint16_t)YRShellInterpreter::SI_CC_keyQ,                                   "key?" },
    { (uint16_t)YRShellInterpreter::SI_CC_auxKeyQ,                                "auxKey?" },
    { (uint16_t)YRShellInterpreter::SI_CC_auxIO,                                  "auxIO" },
    { (uint16_t)YRShellInterpreter::SI_CC_mainIO,                                 "mainIO" },
    { (uint16_t)YRShellInterpreter::SI_CC_emit,                                   "emit" },
    { (uint16_t)YRShellInterpreter::SI_CC_auxEmit,                                "auxEmit" },

    { (uint16_t)YRShellInterpreter::SI_CC_shellSize,                              "shellSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_printShellClass,                        "printShellClass" },
    
    { (uint16_t)YRShellInterpreter::SI_CC_dictionarySize,                        "dictionarySize" },
    { (uint16_t)YRShellInterpreter::SI_CC_padSize,                               "padSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_numRegisters,                          "numRegisters" },
    { (uint16_t)YRShellInterpreter::SI_CC_parameterStackSize,                    "parameterStackSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_returnStackSize,                       "returnStackSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_compileStackSize,                      "compileStackSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_inqSize,                               "inqSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_auxInqSize,                            "auxInqSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_outqSize,                              "outqSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_auxOutqSize,                           "auxOutqSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_dictionaryClear,                       "dictClear" },
    { (uint16_t)YRShellInterpreter::SI_CC_setCommandEcho,                        "setCommandEcho" },
    { (uint16_t)YRShellInterpreter::SI_CC_setExpandCR,                           "setExpandCR" },
    { (uint16_t)YRShellInterpreter::SI_CC_systicks,                              "systicks" },
    { (uint16_t)YRShellInterpreter::SI_CC_micros,                                "micros" },
    { (uint16_t)YRShellInterpreter::SI_CC_millis,                                "millis" },

    { (uint16_t)YRShellInterpreter::SI_CC_bang,                                	 "!" },
    { (uint16_t)YRShellInterpreter::SI_CC_at,                                    "@" },
    { (uint16_t)YRShellInterpreter::SI_CC_clearStats,                            "clearStats" },
    { (uint16_t)YRShellInterpreter::SI_CC_sliceStats,                            "sliceStats" },
    { (uint16_t)YRShellInterpreter::SI_CC_printSliceName,                        "printSliceName" },
    
    { (uint16_t)YRShellInterpreter::SI_CC_find,                                  "find" },
    { (uint16_t)YRShellInterpreter::SI_CC_findEntry,                             "findEntry" },
    { (uint16_t)YRShellInterpreter::SI_CC_fetchToken,                            "fetchToken" },

    { (uint16_t)YRShellInterpreter::SI_CC_lshift,                                "<<" },
    { (uint16_t)YRShellInterpreter::SI_CC_irshift,                               "i>>" },
    { (uint16_t)YRShellInterpreter::SI_CC_rshift,                                ">>" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_return,                              "v_return" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_uint16,                              "v_uint16" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_uint32,                              "v_uint32" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_nint16,                              "v_nint16" },

    { (uint16_t)YRShellInterpreter::SI_CC_noop,                                  "noop" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_if,                                  "v_if" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_else,                                "v_else" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_then,                                "v_then" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_begin,                               "v_begin" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_until,                               "v_until" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_string,                              "v_string" },
    { (uint16_t)YRShellInterpreter::SI_CC_isFunction,                            "isFunction" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_dictionaryMask,                      "v_dictionaryMask" },
    { (uint16_t)YRShellInterpreter::SI_CC_v_dictionaryRelative,                  "v_dictionaryRelative" },

    { (uint16_t)YRShellInterpreter::SI_CC_not,                                   "~" },
    { (uint16_t)YRShellInterpreter::SI_CC_dotRawStr,                             ".rawStr" },
    { (uint16_t)YRShellInterpreter::SI_CC_dotN,                                  ".n" },

    { (uint16_t)YRShellInterpreter::SI_CC_setBaud,                                "setBaud" },
    { (uint16_t)YRShellInterpreter::SI_CC_sysTicksPerSecond,                      "sysTicksPerSecond" },

    { (uint16_t)YRShellInterpreter::SI_CC_lastBufferSize,                         "lastBufferSize" },
    { (uint16_t)YRShellInterpreter::SI_CC_printMainFileName,                      "printMainFileName" },

    { (uint16_t)YRShellInterpreter::SI_CC_setPromptEnable,                        "setPromptEnable" },

    { (uint16_t)YRShellInterpreter::SI_CC_strBang,                                "str!" },
    { (uint16_t)YRShellInterpreter::SI_CC_charBang,                               "c!" },
    { (uint16_t)YRShellInterpreter::SI_CC_charAt,                                 "c@" },
    { (uint16_t)YRShellInterpreter::SI_CC_textIO,                                 "textIO" },

    { (uint16_t)YRShellInterpreter::SI_CC_nextDelay,                              "nextDelay" },
    
    { (uint16_t)YRShellInterpreter::SI_CC_isEntryMatch,                           "isEntryMatch" },


#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
    { (uint16_t)YRShellInterpreter::SI_CC_dotf,                                   ".f" },
    { (uint16_t)YRShellInterpreter::SI_CC_dote,                                   ".e" },
    { (uint16_t)YRShellInterpreter::SI_CC_fLessThan,                              "f<" },
    { (uint16_t)YRShellInterpreter::SI_CC_fLessEqual,                             "f<="},
    { (uint16_t)YRShellInterpreter::SI_CC_fEqual,                                 "f=="},
    { (uint16_t)YRShellInterpreter::SI_CC_fGreaterThan,                           "f>"},
    { (uint16_t)YRShellInterpreter::SI_CC_fGreaterThanEqual,                      "f>="},
    
    { (uint16_t)YRShellInterpreter::SI_CC_fPlus,                                  "f+" },
    { (uint16_t)YRShellInterpreter::SI_CC_fMinus,                                 "f-" },
    { (uint16_t)YRShellInterpreter::SI_CC_fMultiply,                              "f*" },
    { (uint16_t)YRShellInterpreter::SI_CC_fDivide,                                "f/" },
    { (uint16_t)YRShellInterpreter::SI_CC_fPI,                                    "fPI" },
    { (uint16_t)YRShellInterpreter::SI_CC_fSin,                                   "fsin" },
    { (uint16_t)YRShellInterpreter::SI_CC_fCos,                                   "fcos" },
    { (uint16_t)YRShellInterpreter::SI_CC_fTan,                                   "ftan" },
    { (uint16_t)YRShellInterpreter::SI_CC_faSin,                                  "fasin" },
    { (uint16_t)YRShellInterpreter::SI_CC_faCos,                                  "facos" },
    { (uint16_t)YRShellInterpreter::SI_CC_faTan,                                  "fatan" },
    { (uint16_t)YRShellInterpreter::SI_CC_fExp,                                   "fexp" },
    { (uint16_t)YRShellInterpreter::SI_CC_fLog,                                   "flog" },
    { (uint16_t)YRShellInterpreter::SI_CC_fPow,                                   "fpow" },
    { (uint16_t)YRShellInterpreter::SI_CC_fLog10,                                 "flog10" },
    { (uint16_t)YRShellInterpreter::SI_CC_fRound,                                 "fround" },
    { (uint16_t)YRShellInterpreter::SI_CC_floatToInt,                             "f>i" },
    { (uint16_t)YRShellInterpreter::SI_CC_intToFloat,                             "i>f" },
 
#endif
    { (uint16_t)0, NULL}
};




static FunctionDictionary dictionaryInterpreterFunction( interpreterFunctions, YRSHELL_DICTIONARY_INTERPRETER_FUNCTION );

#ifdef YRSHELL_DEBUG
const char *stateDebugStrings[] = {
    "YRSHELL_INVALID_STATE",
	"YRSHELL_NOT_INITIALIZED",
    "YRSHELL_BEGINNING",
    "YRSHELL_INRESET",
    "YRSHELL_BEGIN_IDLE",
    "YRSHELL_IDLE",
    "YRSHELL_FILLING_PAD",
    "YRSHELL_FILLING_AUXPAD",
    "YRSHELL_BEGIN_PARSING",
    "YRSHELL_PARSING",
    "YRSHELL_COMPILING",
    "YRSHELL_EXECUTING",
    "YRSHELL_WAIT_FOR_OUTPUT_SPACE",
    "YRSHELL_WAIT_DELAY",
    "YRSHELL_OUTPUT",
    "YRSHELL_OUTPUT_STR",
    "YRSHELL_LAST_INVALID_STATE"
};
const char *SIDebugStrings[] = {
    "SI_CC_first",
    "SI_CC_return",
    "SI_CC_space",
    "SI_CC_cr",
    "SI_CC_lf",
    "SI_CC_crlf",
    "SI_CC_reset",
    "SI_CC_prompt",
    "SI_CC_clearPad",
    "SI_CC_uint16",
    "SI_CC_uint32",
    "SI_CC_nint16",
    "SI_CC_dot",
    "SI_CC_dotByte",
    "SI_CC_dotWord",
    "SI_CC_dotX",
    "SI_CC_dotByteX",
    "SI_CC_dotWordX",
    "SI_CC_dotStr",
    "SI_CC_hex",
    "SI_CC_decimal",
    "SI_CC_stringDef",
    "SI_CC_string",
    "SI_CC_begin",
    "SI_CC_else",
    "SI_CC_then",
    "SI_CC_begin",
    "SI_CC_until",
    "SI_CC_jmp",
    "SI_CC_jmpz",
    "SI_CC_dup",
    "SI_CC_swap",
    "SI_CC_nip",
    "SI_CC_tuck",
    "SI_CC_drop",
    "SI_CC_rot",
    "SI_CC_toR",
    "SI_CC_fromR",
    "SI_CC_negate",
    "SI_CC_plus",
    "SI_CC_minus",
    "SI_CC_multiply",
    "SI_CC_divide",
    "SI_CC_mod",
    "SI_CC_and",
    "SI_CC_or",
    "SI_CC_xor",
    "SI_CC_lessThan",
    "SI_CC_lessEqual",
    "SI_CC_equal",
    "SI_CC_greaterThan",
    "SI_CC_greaterThanEqual",
    "SI_CC_setDebug",
    "SI_CC_toC",
    "SI_CC_fromC",
    "SI_CC_parameterStackDepth",
    "SI_CC_returnStackDepth",
    "SI_CC_compileStackDepth",
    "SI_CC_parameterStackAt",
    "SI_CC_returnStackAt",
    "SI_CC_compileStackAt",
    "SI_CC_notEqual",
    "SI_CC_zeroEqual",
    "SI_CC_zeroNotEqual",
    "SI_CC_over",
    "SI_CC_2dup",
    "SI_CC_2drop",
    "SI_CC_strlen",
    "SI_CC_getCurrentDictionary",
    "SI_CC_getCurrentDictionaryEnd",
    "SI_CC_getCurrentDictionaryLastWord",
    "SI_CC_delay",
    "SI_CC_nextEntry",
    "SI_CC_dotEntryName",
    "SI_CC_entryToken",
    "SI_CC_keyQ",
    "SI_CC_auxKeyQ",
    "SI_CC_auxIO",
    "SI_CC_mainIO",
    "SI_CC_emit",
    "SI_CC_auxEmit",

    "SI_CC_shellSize",
    "SI_CC_printShellClass",

    "SI_CC_dictionarySize",
    "SI_CC_padSize",
    "SI_CC_numRegisters",
    "SI_CC_parameterStackSize",
    "SI_CC_returnStackSize",
    "SI_CC_compileStackSize",
    "SI_CC_inqSize",
    "SI_CC_auxInqSize",
    "SI_CC_outqSize",
    "SI_CC_auxOutqSize",
 
    "SI_CC_dictionaryClear",
    "SI_CC_setCommandEcho",
    "SI_CC_setExpandCR",

    "SI_CC_systicks",
    "SI_CC_micros",
    "SI_CC_millis"

   	"SI_CC_bang",
    "SI_CC_at",

    "SI_CC_clearStats",
    "SI_CC_sliceStats",
    "SI_CC_printSliceName",

    "SI_CC_find",
    "SI_CC_findEntry",
    "SI_CC_fetchToken",
    
    "SI_CC_lshift",
    "SI_CC_irshift",
    "SI_CC_rshift",
    "SI_CC_v_return",
    "SI_CC_v_uint16",
    "SI_CC_v_uint32",
    "SI_CC_v_nint16",
 
    "SI_CC_noop",
    "SI_CC_x_if",
    "SI_CC_x_else",
    "SI_CC_x_then",
    "SI_CC_x_begin",
    "SI_CC_x_until",
    "SI_CC_v_if",
    "SI_CC_v_else",
    "SI_CC_v_then",
    "SI_CC_v_begin",
    "SI_CC_v_until",
    "SI_CC_v_string",
    "SI_CC_isFunction",
    "SI_CC_v_dictionaryMask",
    "SI_CC_v_dictionaryRelative",
    "SI_CC_not",
    
    "SI_CC_dotRawStr",
    "SI_CC_dotN",

    "SI_CC_setBaud",
    "SI_CC_sysTicksPerSecond",

    "SI_CC_lastBufferSize",
    "SI_CC_printMainFileName",
    
    "SI_CC_setPromptEnable",
    
    "SI_CC_strBang",
    "SI_CC_charBang",
    "SI_CC_charAt",
    "SI_CC_textIO",

    "SI_CC_nextDelay",
    
    "SI_CC_isEntryMatch",

#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
    "SI_CC_dotf",
    "SI_CC_dote",
    "SI_CC_fLessThan",
    "SI_CC_fLessEqual",
    "SI_CC_fEqual",
    "SI_CC_fGreaterThan",
    "SI_CC_fGreaterThanEqual",

    "SI_CC_fPlus",
    "SI_CC_fMinus",
    "SI_CC_fMultiply",
    "SI_CC_fDivide",
    "SI_CC_fPI",
    "SI_CC_fSin",
    "SI_CC_fCos",
    "SI_CC_fTan",
    "SI_CC_faSin",
    "SI_CC_faCos",
    "SI_CC_faTan",
    "SI_CC_fExp",
    "SI_CC_fLog",
    "SI_CC_fPow",
    "SI_CC_fLog10",
    "SI_CC_fRound",
    "SI_CC_floatToInt",
    "SI_CC_intToFloat",
#endif
    
    "SI_CC_last"
};
#endif

CompiledDictionary emptyDictionary;

void YRShellInterpreter::init( ) {
    reset();
    
    m_dictionaryList[ YRSHELL_DICTIONARY_DUMMY_INDEX] = &emptyDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_CURRENT_INDEX] = m_DictionaryCurrent;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_COMPILED_INDEX] = NULL;
    m_dictionaryList[ YRSHELL_DICTIONARY_EXTENSION_FUNCTION_INDEX] = NULL;
    m_dictionaryList[ YRSHELL_DICTIONARY_INTERPRETER_COMPILED_INDEX] = (Dictionary*)&interpreterCompiledDictionary;
    m_dictionaryList[ YRSHELL_DICTIONARY_COMMON_FUNCTION_INDEX] = NULL;
    m_dictionaryList[ YRSHELL_DICTIONARY_INTERPRETER_FUNCTION_INDEX] =  &dictionaryInterpreterFunction;
}
YRShellInterpreter::YRShellInterpreter() {
    m_DictionaryCurrent = NULL;
    m_Inq = m_AuxInq = m_Outq = m_AuxOutq = NULL;
    m_ParameterStack = m_ReturnStack = m_CompileStack = NULL;
    m_parameterStackSize = m_returnStackSize = m_compileStackSize = 0;
    m_commandEcho = m_expandCR = true;

    m_Pad = NULL;
    m_Registers = NULL;
    m_numRegisters = 0;
	m_compileTopOfStack = 0;
#ifdef YRSHELL_DEBUG
	m_debugFlags = 0;
#endif
	m_hexMode = false;
	m_outputTimeoutInMilliseconds = 1000;
	m_padCount = 0;
    m_padSize = 0;
	m_PC = YRSHELL_DICTIONARY_INVALID;
	m_returnTopOfStack = 0;
	m_saveptr = NULL;
	m_state = YRSHELL_NOT_INITIALIZED;
	m_token = NULL;
	m_topOfStack = 0;
	m_useAuxQueues = false;
    m_useTextOutput = false;
#ifdef  INPUT_BUFFER_EDITING
    m_LastBuffer = NULL;
    if( m_lastBufferSize != 0) {
        memset(m_LastBuffer, 0, m_lastBufferSize);
        m_lastBufferFree = 0;
        m_lastBufferIndex = 0;
    }
#endif
	strcpy( m_autoPrompt, "\rYR_");
	unsignedToStringZero(s_shellNumber++, 2, &m_autoPrompt[3]);
	strcat( m_autoPrompt, ">");
	setPrompt( m_autoPrompt);
    m_outputStrPtr  = NULL;
    m_outputUint = 0;
    m_outputStr0 = NULL;
    m_outputStr1 = NULL;
    m_outputStr2 = NULL;
    m_outputStr3 = NULL;
    m_stateTopOfStack = 0;
    m_promptEnable = true;
}
YRShellInterpreter::~YRShellInterpreter() {
}
bool YRShellInterpreter::isCompileToken() {
    return m_token != NULL && (!strcmp( m_token, "s'") || !strcmp( m_token, "[") || !strcmp( m_token, "][") || !strcmp( m_token, "]") || !strcmp( m_token, "{") || !strcmp( m_token, "}") );
}
uint16_t YRShellInterpreter::find( const char* name) {
    uint16_t rc = YRSHELL_DICTIONARY_INVALID;
    for( uint8_t i = 0; rc == YRSHELL_DICTIONARY_INVALID && i < YRSHELL_DICTIONARY_LAST_INDEX; i++) {
        if( m_dictionaryList[ i] != NULL) {
            rc = m_dictionaryList[ i]->find( name);
        }
    }
    return rc;
}
uint16_t YRShellInterpreter::findEntry( const char* name) {
    uint16_t rc = YRSHELL_DICTIONARY_INVALID;
    for( uint8_t i = 0; rc == YRSHELL_DICTIONARY_INVALID && i < YRSHELL_DICTIONARY_LAST_INDEX; i++) {
        if( m_dictionaryList[ i] != NULL) {
            rc = m_dictionaryList[ i]->findEntry( name);
        }
    }
    return rc;
}
const char* YRShellInterpreter::getFileName( const char* P) {
    const char* R = P;
    while( *P++ != '\0') {
        if( *P == '/' || *P == '\'') {
            R = ++P;
        }
    }
    return R;
}

void YRShellInterpreter::shellERROR( const char* file, unsigned line) {
    reset( file, line);
}
void YRShellInterpreter::shellERROR( const char* file, unsigned line, const char* message) {
    reset( file, line, message);
}
void YRShellInterpreter::CC_nextEntry( ) {
    uint32_t v1 = popParameterStack();
    uint32_t v2 = popParameterStack();
    uint32_t res = YRSHELL_DICTIONARY_INVALID;
    if( v1 == YRSHELL_DICTIONARY_INVALID && v2 == 0) {
        if( m_dictionaryList[ v2] != NULL) {
            res = m_dictionaryList[ v2]->getFirstEntry();
        }
    } else {
        if( v1 != YRSHELL_DICTIONARY_INVALID && v2 < YRSHELL_DICTIONARY_LAST_INDEX && m_dictionaryList[ v2] != NULL) {
            res = m_dictionaryList[ v2]->getNextEntry(v1);
        }
    }
    if( res == YRSHELL_DICTIONARY_INVALID) {
        v2++;
    }
    while( res == YRSHELL_DICTIONARY_INVALID && v2 < YRSHELL_DICTIONARY_LAST_INDEX ) {
        if(m_dictionaryList[ v2] != NULL ) {
            res = m_dictionaryList[ v2]->getFirstEntry();
            if( res == YRSHELL_DICTIONARY_INVALID ) {
                v2++;
            }
        } else {
            v2++;
        }
    }
    if( v2 > YRSHELL_DICTIONARY_LAST_INDEX) {
        v2 = YRSHELL_DICTIONARY_LAST_INDEX;
    }
    pushParameterStack( v2);
    pushParameterStack( res);
}

void YRShellInterpreter:: prompt( ) {
	if( m_promptEnable) {
		outString( m_prompt);
	}
}
void YRShellInterpreter::executeFunction( uint16_t n) {
    bool b;
    int32_t i;
    uint32_t v1, v2, v3;
    float f1, f2;
    const char *P, *M;
    Sliceable *S;

#ifdef YRSHELL_DEBUG
    if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
        outString("[");
        if(n >= SI_CC_first && n <= SI_CC_last) {
            outString( SIDebugStrings[n - SI_CC_first]);
        }
        outString("]");
    }
#endif
    switch( n) {
        case SI_CC_return:
            m_PC = popReturnStack();
            break;
        case SI_CC_space:
            outChar( ' ');
            break;
        case SI_CC_cr:
            outChar( '\r');
            break;
        case SI_CC_prompt:
            prompt();
            break;
        case SI_CC_lf:
            outChar( '\n');
            break;
        case SI_CC_crlf:
            outChar( '\r');
            outChar( '\n');
            break;
        case SI_CC_reset:
            m_DictionaryCurrent->reset();
            reset( __FILE__, __LINE__, "COMMAND RESET");
            break;
        case SI_CC_uint16:
            v1 = fetchCurrentValueToken();
            pushParameterStack( v1);
            break;
        case SI_CC_uint32:
            v1 = fetchCurrentValueToken();
            v1 |= ((uint32_t)fetchCurrentValueToken()) << 16;
            pushParameterStack( v1);
            break;
        case SI_CC_nint16:
            v1 = fetchCurrentValueToken();
            v1 |= 0xFFFF0000;
            pushParameterStack( v1);
            break;
        case SI_CC_dot:
            if( m_hexMode) {
                outUint32X( popParameterStack());
            } else {
                outInt32( popParameterStack());
            }
            outChar( ' ');
            break;
        case SI_CC_dotByte:
            if( m_hexMode) {
                outUint8X( popParameterStack());
            } else {
                outInt8( popParameterStack());
            }
            outChar( ' ');
            break;
        case SI_CC_dotWord:
            if( m_hexMode) {
                outUint16X( popParameterStack());
            } else {
                outInt16( popParameterStack());
            }
            outChar( ' ');
            break;
        case SI_CC_dotX:
            outUint32X( popParameterStack());
            outChar( ' ');
            break;
        case SI_CC_dotByteX:
            outUint8X( popParameterStack());
            outChar( ' ');
            break;
        case SI_CC_dotWordX:
            outUint16X( popParameterStack());
            outChar( ' ');
            break;
        case SI_CC_dotStr:
            m_outputStrPtr = getAddressFromToken( popParameterStack());
            pushState(YRSHELL_OUTPUT_STR);
            break;
        case SI_CC_hex:
            m_hexMode = true;
            break;
        case SI_CC_decimal:
            m_hexMode = false;
            break;
        case SI_CC_stringDef:
            CC_stringDef();
            break;
        case SI_CC_string:
            pushParameterStack( getAbsoluteAddressToken( ));
            P = getAddressFromToken( m_PC);
            m_PC += Dictionary::nameLength(P);
            break;
        case SI_CC_if:
            CC_if();
            break;
        case SI_CC_else:
            CC_else();
            break;
        case SI_CC_then:
            CC_then();
            break;
        case SI_CC_begin:
            CC_begin();
            break;
        case SI_CC_until:
            CC_until();
            break;
        case SI_CC_x_else:
        case SI_CC_jmp:
            v1 = fetchCurrentToken();
            m_PC &= YRSHELL_DICTIONARY_RELATIVE_ALL_MASK;
            m_PC |= v1 & ~YRSHELL_DICTIONARY_RELATIVE_ALL_MASK;
            break;
        case SI_CC_x_if:
        case SI_CC_x_until:
        case SI_CC_jmpz:
            if( !popParameterStack()) {
                v1 = fetchCurrentToken();
                m_PC &= YRSHELL_DICTIONARY_RELATIVE_ALL_MASK;
                m_PC |= v1 & ~YRSHELL_DICTIONARY_RELATIVE_ALL_MASK;
            } else {
                m_PC++;
            }
            break;
        case SI_CC_dup:
            v1 = popParameterStack();
            pushParameterStack(v1);
            pushParameterStack(v1);
            break;
        case SI_CC_swap:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(v1);
            pushParameterStack(v2);
            break;
        case SI_CC_nip:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(v1);
            break;
        case SI_CC_tuck:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(v1);
            pushParameterStack(v2);
            pushParameterStack(v1);
            break;
        case SI_CC_drop:
            v1 = popParameterStack();
            break;
        case SI_CC_rot:
            v1 = popParameterStack();
            v2 = popParameterStack();
            v3 = popParameterStack();
            pushParameterStack(v2);
            pushParameterStack(v1);
            pushParameterStack(v3);
            break;
        case SI_CC_toR:
            v1 = popParameterStack();
            pushReturnStack(v1);
            break;
        case SI_CC_fromR:
            v1 = popReturnStack();
            pushParameterStack(v1);
            break;
        case SI_CC_negate:
            v1 = popParameterStack();
            pushParameterStack( 0 - v1);
            break;
        case SI_CC_plus:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v1 + v2);
            break;
        case SI_CC_minus:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v2 - v1);
            break;
        case SI_CC_multiply:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v1 * v2);
            break;
        case SI_CC_divide:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v2 / v1);
            break;
        case SI_CC_mod:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v2 % v1);
            break;
        case SI_CC_and:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v1 & v2);
            break;
        case SI_CC_or:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v1 | v2);
            break;
        case SI_CC_xor:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v1 ^ v2);
            break;
        case SI_CC_lessThan:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( ((int32_t) v2) < ((int32_t) v1));
            break;
        case SI_CC_lessEqual:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(  ((int32_t) v2)  <= ((int32_t) v1));
            break;
        case SI_CC_equal:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v2 == v1);
            break;
        case SI_CC_greaterThan:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(  ((int32_t) v2)  > ((int32_t) v1));
            break;
        case SI_CC_greaterThanEqual:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(  ((int32_t) v2)  >= ((int32_t) v1));
            break;
        case SI_CC_setDebug:
            v1 = popParameterStack();
#ifdef YRSHELL_DEBUG
            m_debugFlags = v1;
#endif
            break;
        case SI_CC_toC:
            v1 = popParameterStack();
            pushCompileStack(v1);
            break;
        case SI_CC_fromC:
            v1 = popCompileStack();
            pushParameterStack(v1);
            break;
        case SI_CC_parameterStackDepth:
            pushParameterStack( m_topOfStack);
            break;
        case SI_CC_returnStackDepth:
            pushParameterStack( m_returnTopOfStack);
            break;
        case SI_CC_compileStackDepth:
            pushParameterStack( m_compileTopOfStack);
            break;
        case SI_CC_parameterStackAt:
            v1 = m_topOfStack - 2 - popParameterStack();
            if( v1 >= m_topOfStack) {
                shellERROR( __FILE__, __LINE__, "BAD STACK ACCESS");
            } else {
                pushParameterStack( m_ParameterStack[ v1]);
            }
            break;
            
        case SI_CC_returnStackAt:
            v1 = m_returnTopOfStack - 1 - popParameterStack();
            if( v1 >= m_returnTopOfStack) {
                shellERROR( __FILE__, __LINE__, "BAD RETURN STACK ACCESS");
            } else {
                pushParameterStack( m_ReturnStack[ v1]);
            }
            break;
        case SI_CC_compileStackAt:
            v1 = m_compileTopOfStack - 1 - popParameterStack();
            if( v1 >= m_compileTopOfStack) {
                shellERROR( __FILE__, __LINE__, "BAD COMPILE STACK ACCESS");
            } else {
                pushParameterStack( m_CompileStack[ v1]);
            }
            break;
        case SI_CC_notEqual:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack( v2 != v1);
            break;
        case SI_CC_zeroEqual:
            v1 = popParameterStack();
            pushParameterStack( 0 == v1);
            break;
        case SI_CC_zeroNotEqual:
            v1 = popParameterStack();
            pushParameterStack( 0 != v1);
            break;
        case SI_CC_over:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(v2);
            pushParameterStack(v1);
            pushParameterStack(v2);
            break;
        case SI_CC_2dup:
            v1 = popParameterStack();
            v2 = popParameterStack();
            pushParameterStack(v2);
            pushParameterStack(v1);
            pushParameterStack(v2);
            pushParameterStack(v1);
            break;
        case SI_CC_2drop:
            v1 = popParameterStack();
            v2 = popParameterStack();
            break;
        case SI_CC_strlen:
            P = getAddressFromToken(popParameterStack());
            if( P == NULL) {
                pushParameterStack(0);
            } else {
                pushParameterStack((uint32_t) strlen(P));
            }
            break;
        case SI_CC_getCurrentDictionary:
            pushParameterStack( m_DictionaryCurrent->getWord(popParameterStack()));
            break;
        case SI_CC_getCurrentDictionaryEnd:
            pushParameterStack(m_DictionaryCurrent->getBackupWordEnd());
            break;
        case SI_CC_getCurrentDictionaryLastWord:
            pushParameterStack(m_DictionaryCurrent->getBackupLastWord());
            break;
            
        case SI_CC_delay:
            m_delayTimer.setInterval(popParameterStack());
            pushState(YRSHELL_WAIT_DELAY);
            break;
		case SI_CC_nextDelay:
			m_delayTimer.isNextInterval();
            pushState(YRSHELL_WAIT_DELAY);
			break;
       	case SI_CC_nextEntry:
            CC_nextEntry( );
            break;
        case SI_CC_dotEntryName:
            v1 = popParameterStack();
            v2 = popParameterStack();
            if( v1 != YRSHELL_DICTIONARY_INVALID && v2 < YRSHELL_DICTIONARY_LAST_INDEX && m_dictionaryList[ v2] != NULL) {
                v1 = m_dictionaryList[ v2]->getNameAddressToken(v1);
                P = getAddressFromToken( v1);
                if( P == NULL || strlen(P) == 0) {
                    pushParameterStack( 0);
                } else {
                    pushParameterStack((uint32_t) strlen( P));
                    m_outputStrPtr = P;
                    pushState( YRSHELL_OUTPUT_STR);
                }
            } else {
                pushParameterStack(0);
            }
            break;
        case SI_CC_isEntryMatch:
            v1 = popParameterStack();
            v2 = popParameterStack();
            v3 = popParameterStack();
            pushParameterStack(v3);
            pushParameterStack(v2);
            pushParameterStack(v1);
            if( v1 != YRSHELL_DICTIONARY_INVALID && v2 < YRSHELL_DICTIONARY_LAST_INDEX && m_dictionaryList[ v2] != NULL) {
                v1 = m_dictionaryList[ v2]->getNameAddressToken(v1);
                P = getAddressFromToken( v1);
                M = getAddressFromToken( v3);
                v1 = (uint32_t) strlen( P);
                v2 = (uint32_t) strlen( M);
                if( P == NULL || v1 == 0 || M == NULL || v2 == 0 || v1 < v2) {
                    pushParameterStack( 0);
                } else {
                    v3 = v1 - v2;
                    v1 = 0;
                    for( i = 0; i <= v3; i++) {
                        if( !strncmp(P+i, M, v2) ) {
                            v1 = -1;
                            break;
                        }
                    }
                    pushParameterStack(v1);
                }
            } else {
                pushParameterStack( 0);
            }
            break;
            
        case SI_CC_entryToken:
            v1 = popParameterStack();
            v2 = popParameterStack();
            if( v1 != YRSHELL_DICTIONARY_INVALID && v2 < YRSHELL_DICTIONARY_LAST_INDEX && m_dictionaryList[ v2] != NULL) {
                v1 = m_dictionaryList[ v2]->getToken(v1);
                pushParameterStack( v1);
            } else {
                pushParameterStack(YRSHELL_DICTIONARY_INVALID);
            }
            break;
        case SI_CC_keyQ:
            if( m_Inq->valueAvailable()) {
                pushParameterStack( m_Inq->get());
                pushParameterStack( -1);
            } else {
                pushParameterStack( 0);
                pushParameterStack( 0);
            }
            break;
            
        case SI_CC_auxKeyQ:
            if( m_AuxInq->valueAvailable()) {
                pushParameterStack( m_AuxInq->get());
                pushParameterStack( -1);
            } else {
                pushParameterStack( 0);
                pushParameterStack( 0);
            }
            break;
        case SI_CC_auxIO:
            m_useAuxQueues = true;
            m_useTextOutput = false;
            break;
        case SI_CC_mainIO:
            m_useAuxQueues = false;
            m_useTextOutput = false;
            break;
        case SI_CC_emit:
            outChar( popParameterStack());
            break;
        case SI_CC_auxEmit:
            b = m_useAuxQueues;
            m_useAuxQueues = true;
            outChar( popParameterStack());
            m_useAuxQueues = b;
            break;
            
        case SI_CC_shellSize:
            pushParameterStack( shellSize());
            break;
        case SI_CC_printShellClass:
            m_outputStrPtr = shellClass();
            pushState( YRSHELL_OUTPUT_STR);
            break;

        case SI_CC_dictionarySize:
            pushParameterStack( m_DictionaryCurrent->getSize());
            break;
        case SI_CC_padSize:
            pushParameterStack( m_padSize);
            break;
        case SI_CC_numRegisters:
            pushParameterStack( m_numRegisters);
            break;
        case SI_CC_parameterStackSize:
            pushParameterStack( m_parameterStackSize);
            break;
        case SI_CC_returnStackSize:
            pushParameterStack( m_returnStackSize);
            break;
        case SI_CC_compileStackSize:
            pushParameterStack( m_compileStackSize);
            break;
        case SI_CC_inqSize:
            pushParameterStack( m_Inq->size());
            break;
        case SI_CC_auxInqSize:
            pushParameterStack( m_AuxInq->size());
            break;
        case SI_CC_outqSize:
            pushParameterStack( m_Outq->size());
            break;
        case SI_CC_auxOutqSize:
            pushParameterStack( m_AuxOutq->size());
            break;
 
        case SI_CC_dictionaryClear:
            m_DictionaryCurrent->reset();
            nextState( YRSHELL_BEGIN_IDLE);
            break;
 
        case SI_CC_setCommandEcho:
            m_commandEcho = popParameterStack() != 0;
            break;
        case SI_CC_setExpandCR:
            m_expandCR = popParameterStack() != 0;
            break;

        case SI_CC_systicks:
            pushParameterStack( HiResTimer::getSysticks());
            break;

        case SI_CC_micros:
            pushParameterStack( HiResTimer::getMicros());
            break;

        case SI_CC_millis:
            pushParameterStack( HiResTimer::getMillis());
            break;

        case SI_CC_bang:
        	v1 = popParameterStack();
        	v2 = popParameterStack();
        	if( v1 < m_numRegisters) {
        		m_Registers[ v1] = v2;
        	} else {
            	reset( __FILE__, __LINE__, "INVALID REGISTER");
	       	}
            break;
        case SI_CC_at:
        	v1 = popParameterStack();
        	if( v1 < m_numRegisters) {
        		pushParameterStack(m_Registers[ v1]);
        	} else {
            	reset( __FILE__, __LINE__, "INVALID REGISTER");
	       	}
            break;
         case SI_CC_clearStats:
        	v1 = popParameterStack();
        	S = Sliceable::getSlicePointer( v1);
        	if( S != NULL) {
        		S->resetTimer();
        		pushParameterStack( -1);
         	} else {
         		pushParameterStack( 0);
         	}
            break;
       case SI_CC_sliceStats:
        	v1 = popParameterStack();
        	S = Sliceable::getSlicePointer( v1);
        	if( S != NULL) {
        		pushParameterStack( S->getTimerAverage());
        		pushParameterStack( S->getTimerMax());
        		pushParameterStack( S->getTimerMin());
        		pushParameterStack( S->getTimerCount());
        		pushParameterStack( -1);
        	} else {
        		pushParameterStack( 0);
        	}
            break;
        case SI_CC_printSliceName:
        	v1 = popParameterStack();
        	S = Sliceable::getSlicePointer( v1);
        	if( S != NULL) {
                m_outputStrPtr = S->sliceName();
                pushState( YRSHELL_OUTPUT_STR);
         	}
            break;
            
        case SI_CC_find:
            P = getAddressFromToken(popParameterStack());
            pushParameterStack( find( P));
            break;
        case SI_CC_findEntry:
            P = getAddressFromToken(popParameterStack());
            pushParameterStack( findEntry( P));
            break;
        case SI_CC_fetchToken:
            v1 = popParameterStack( );
            v2 = fetchValueToken( v1);
            pushParameterStack( v2);
            break;

        case SI_CC_lshift:
            v1 = popParameterStack( );
            v2 = popParameterStack( );
            pushParameterStack( v2 << v1);
            break;
        case SI_CC_irshift:
            v1 = popParameterStack( );
            v2 = popParameterStack( );
            pushParameterStack( ((int32_t) v2) << ((int32_t) v1) );
            break;
        case SI_CC_rshift:
            v1 = popParameterStack( );
            v2 = popParameterStack( );
            pushParameterStack( v2 >> v1);
            break;
        case SI_CC_v_return:
            pushParameterStack( YRShellInterpreter::SI_CC_return);
            break;
        case SI_CC_v_uint16:
            pushParameterStack( YRShellInterpreter::SI_CC_uint16);
            break;
        case SI_CC_v_uint32:
            pushParameterStack( YRShellInterpreter::SI_CC_uint32);
            break;
        case SI_CC_v_nint16:
            pushParameterStack(YRShellInterpreter::SI_CC_nint16);
            break;
            
        case SI_CC_x_begin:
        case SI_CC_x_then:
        case SI_CC_noop:
            break;
            
        case SI_CC_v_if:
            pushParameterStack(YRShellInterpreter::SI_CC_x_if);
            break;
        case SI_CC_v_else:
            pushParameterStack(YRShellInterpreter::SI_CC_x_else);
            break;
        case SI_CC_v_then:
            pushParameterStack(YRShellInterpreter::SI_CC_x_then);
            break;
        case SI_CC_v_begin:
            pushParameterStack(YRShellInterpreter::SI_CC_x_begin);
            break;
        case SI_CC_v_until:
            pushParameterStack(YRShellInterpreter::SI_CC_x_until);
            break;
        case SI_CC_v_string:
            pushParameterStack(YRShellInterpreter::SI_CC_string);
            break;
        case SI_CC_isFunction:
            v1 = popParameterStack();
            v1 &= YRSHELL_DICTIONARY_MASK;
            pushParameterStack( v1 == YRSHELL_DICTIONARY_INTERPRETER_FUNCTION ||  v1 == YRSHELL_DICTIONARY_COMMON_FUNCTION || v1 == YRSHELL_DICTIONARY_EXTENSION_FUNCTION );
            break;
        case SI_CC_v_dictionaryMask:
            pushParameterStack(YRSHELL_DICTIONARY_MASK);
            break;
        case SI_CC_v_dictionaryRelative:
            pushParameterStack(YRSHELL_DICTIONARY_RELATIVE);
            break;

        case SI_CC_not:
            v1 = popParameterStack();
            v2 = ~ v1;
            pushParameterStack(v2);
            break;

        case SI_CC_dotRawStr:
            P = getAddressFromToken( popParameterStack());
            outRawString( P );
            break;

        case SI_CC_dotN:
            if( m_hexMode) {
                outUint32Xn( popParameterStack());
            } else {
                outInt32n( popParameterStack());
            }
            break;

        case SI_CC_setBaud:
            v1 = popParameterStack();
            v2 = popParameterStack();
            HW_setSerialBaud(v1, v2);
            break;
        case SI_CC_sysTicksPerSecond:
            v1 = HW_getSysTicksPerSecond();
            pushParameterStack( v1);
            break;

        case SI_CC_lastBufferSize:
            pushParameterStack( m_lastBufferSize);
            break;
        case SI_CC_printMainFileName:
            m_outputStrPtr = mainFileName();
            pushState( YRSHELL_OUTPUT_STR);
            break;
        
        case SI_CC_setPromptEnable:
            m_promptEnable = popParameterStack( );
            break;
            
        case SI_CC_strBang:
            v1 = popParameterStack();
            v2 = popParameterStack();
            v1 =  v1 > ((uint32_t) (m_textBufferSize - 1)) ? m_textBufferSize - 1 : v1;
            v3 = m_textBufferSize - v1;
            if( m_textBufferSize > 0 && v3 > 1) {
                strncpy( &m_TextBuffer[ v1], getAddressFromToken(v2), v3);
                m_TextBuffer[ m_textBufferSize - 1] = '\0';
            }
            break;
        case SI_CC_charBang:
            v1 = popParameterStack();
            v2 = popParameterStack();
            if( v1 < m_textBufferSize) {
                m_TextBuffer[ v1 ] = v2;
                m_TextBuffer[ m_textBufferSize - 1] = '\0';
            }
            break;
        case SI_CC_charAt:
            v1 = popParameterStack();
            if( v1 < m_textBufferSize) {
                pushParameterStack( m_TextBuffer[ v1 ]);
            } else {
                pushParameterStack( m_TextBuffer[ v1 ]);
            }
            break;
        case SI_CC_textIO:
             m_useTextOutput = true;
            break;

#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
        case SI_CC_dotf:
            outFloat(popFloat());
            break;
            
        case SI_CC_dote:
            outEngFloat(popFloat());
            break;
            
        case SI_CC_fLessThan:
            f1 = popFloat();
            f2 = popFloat();
            pushParameterStack(f2 < f1);
            break;
            
        case SI_CC_fLessEqual:
            f1 = popFloat();
            f2 = popFloat();
            pushParameterStack(f2 <= f1);
            break;
            
        case SI_CC_fEqual:
            f1 = popFloat();
            f2 = popFloat();
            pushParameterStack(f2 == f1);
            break;
            
        case SI_CC_fGreaterThan:
            f1 = popFloat();
            f2 = popFloat();
            pushParameterStack(f2 > f1);
            break;
            
        case SI_CC_fGreaterThanEqual:
            f1 = popFloat();
            f2 = popFloat();
            pushParameterStack(f2 >= f1);
            break;
            
        case SI_CC_fPlus:
            f1 = popFloat();
            f2 = popFloat();
            pushFloat(f2 + f1);
            break;
            
        case SI_CC_fMinus:
            f1 = popFloat();
            f2 = popFloat();
            pushFloat(f2 - f1);
            break;
            
        case SI_CC_fMultiply:
            f1 = popFloat();
            f2 = popFloat();
            pushFloat(f2 * f1);
            break;
            
        case SI_CC_fDivide:
            f1 = popFloat();
            f2 = popFloat();
            pushFloat(f2 / f1);
            break;
            
        case SI_CC_fSin:
            pushFloat( sinf(popFloat()));
            break;
            
        case SI_CC_fPI:
            pushFloat( 3.1415927f);
            break;
            
        case SI_CC_fCos:
            pushFloat( cosf(popFloat()));
            break;
            
        case SI_CC_fTan:
            pushFloat( tanf(popFloat()));
            break;
            
        case SI_CC_faSin:
            pushFloat( asinf(popFloat()));
            break;
            
        case SI_CC_faCos:
            pushFloat( acosf(popFloat()));
            break;
            
        case SI_CC_faTan:
            pushFloat( atanf(popFloat()));
            break;
            
        case SI_CC_fExp:
            pushFloat( expf(popFloat()));
            break;
            
        case SI_CC_fLog:
            pushFloat( logf(popFloat()));
            break;
            
        case SI_CC_fPow:
            f1 = popFloat();
            f2 = popFloat();
            pushFloat( powf(f2, f1) );
            break;
            
        case SI_CC_fLog10:
            pushFloat( log10f(popFloat()));
            break;
            
        case SI_CC_fRound:
            pushFloat( roundf(popFloat()));
            break;
            
        case SI_CC_floatToInt:
            v1 = ((int32_t) popFloat());
            pushParameterStack(v1);
            break;
            
        case SI_CC_intToFloat:
            v1 = popParameterStack();
            i = (int32_t) v1;
            pushFloat( i);
            break;
            
#endif
        default:
            reset( __FILE__, __LINE__);
            break;
    }
}
void YRShellInterpreter::CC_stringDef( ) {
    char c;
    uint16_t token = 0;
    bool tokenEmpty = true;
    m_DictionaryCurrent->addToken( SI_CC_string);
    while( 1) {
        c =  *m_saveptr++;
        if( c == '\\') {
            c = *m_saveptr++;
            switch( c) {
                case '\\':
                    c = '\\';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
                case '\'':
                    c = '\'';
                    break;
                case 'x':
                    c = charToHex(*m_saveptr++) << 4;
                    c |= charToHex(*m_saveptr++);
                    break;
                default:
                    c = '0';
                    break;
            }
        } else if( c == '\'' || c == '\0' ) {
            if( tokenEmpty) {
                m_DictionaryCurrent->addToken( 0);
            } else {
                m_DictionaryCurrent->addToken( token);
            }
            break;
        }
        if( tokenEmpty) {
            token = (uint16_t) c;
            tokenEmpty = false;
        } else {
            token |= ((uint16_t) c) << 8;
            tokenEmpty = true;
            m_DictionaryCurrent->addToken( token);
        }
    }
}
void YRShellInterpreter::CC_if( ) {
    m_DictionaryCurrent->addToken( SI_CC_x_if);
    //m_DictionaryCurrent->addToken( SI_CC_jmpz);
    pushCompileStack(0x10000000 | YRSHELL_DICTIONARY_RELATIVE| m_DictionaryCurrent->getWordEnd());
    m_DictionaryCurrent->addToken( 0);
}
void YRShellInterpreter::CC_else( ) {
    uint32_t v = popCompileStack();
    if( (v & 0xF0000000) != 0x10000000) {
        shellERROR( __FILE__, __LINE__, "MISSING [");
    } else {
        m_DictionaryCurrent->addToken( SI_CC_x_else);
        //m_DictionaryCurrent->addToken( SI_CC_jmp);
        pushCompileStack(0x10000000 | YRSHELL_DICTIONARY_RELATIVE | m_DictionaryCurrent->getWordEnd());
        m_DictionaryCurrent->addToken( 0);
        m_DictionaryCurrent->setToken( v & YRSHELL_DICTIONARY_ADDRESS_MASK, YRSHELL_DICTIONARY_RELATIVE| m_DictionaryCurrent->getWordEnd());
    }
}
void YRShellInterpreter::CC_then( ) {
    uint32_t v = popCompileStack();
    if( (v & 0xF0000000) != 0x10000000) {
        shellERROR( __FILE__, __LINE__, "MISSING [");
    } else {
        m_DictionaryCurrent->addToken( SI_CC_x_then);
        m_DictionaryCurrent->setToken( v & YRSHELL_DICTIONARY_ADDRESS_MASK, YRSHELL_DICTIONARY_RELATIVE | m_DictionaryCurrent->getWordEnd());
    }
}
void YRShellInterpreter::CC_begin( ) {
    m_DictionaryCurrent->addToken( SI_CC_x_begin);
    pushCompileStack(0x20000000 | YRSHELL_DICTIONARY_RELATIVE | m_DictionaryCurrent->getWordEnd());
}
void YRShellInterpreter::CC_until( ) {
    uint32_t v = popCompileStack();
    if( (v & 0xF0000000) != 0x20000000) {
        shellERROR( __FILE__, __LINE__, "MISSING {");
    } else {
        m_DictionaryCurrent->addToken( SI_CC_x_until);
        m_DictionaryCurrent->addToken(YRSHELL_DICTIONARY_RELATIVE | (v & YRSHELL_DICTIONARY_ADDRESS_MASK));
    }
}
void YRShellInterpreter::interpretReset( ) {
    nextState( YRSHELL_INRESET);
    m_padCount = 0;
    m_token = NULL;
    m_saveptr = NULL;
    m_DictionaryCurrent->rollBack();
}
void YRShellInterpreter::reset( ) {
    if( m_state != YRSHELL_INRESET) {
        m_Inq->reset();
        m_AuxInq->reset();
        m_Outq->reset();
        m_AuxOutq->reset();
    
#ifdef YRSHELL_DEBUG
        //m_debugFlags = YRSHELL_DEBUG_STATE | YRSHELL_DEBUG_INPUT | YRSHELL_DEBUG_TOKEN;
        //m_debugFlags = YRSHELL_DEBUG_STATE | YRSHELL_DEBUG_TOKEN | YRSHELL_DEBUG_EXECUTE;
        m_debugFlags = 0;
#endif
        nextState( YRSHELL_INRESET);
        m_padCount = 0;
        m_token = NULL;
        m_saveptr = NULL;
    
        m_topOfStack = 0;
        m_returnTopOfStack = 0;
        m_compileTopOfStack = 0;
        m_stateTopOfStack = 0;
        m_hexMode = false;
        m_useAuxQueues = false;
    
        m_PC = 0;
        //m_outputTimeoutInMilliseconds = 1000;
    
        m_outputStr0 = "\r" YRSHELL_VERSION " Commit: " GIT_COMMIT_HASH " " GIT_COMMIT_SHORT_HASH "\rCompile Time: " __DATE__ " " __TIME__ " Main File: ";
        m_outputStr1 = mainFileName();
    }
}
void YRShellInterpreter::reset( const char* file, unsigned line) {
    reset();
    m_outputStr2 = getFileName( file);
    m_outputUint = line;
    m_outputUintValid = true;
}
void YRShellInterpreter::reset( const char* file, unsigned line, const char* message) {
    reset();
    m_outputStr2 = message;
    m_outputStr3 = getFileName( file);
    m_outputUint = line;
    m_outputUintValid = true;
}
void YRShellInterpreter::nextState(YRShellState n) {
#ifdef YRSHELL_DEBUG
    if( m_debugFlags & YRSHELL_DEBUG_STATE) {
        outString( "[[");
        outString(  stateDebugStrings[ m_state]);
        outString( "]->[");
        outString(stateDebugStrings[ n]);
        outString( "]]");
    }
#endif
    m_state = n;
    if( m_state <= YRSHELL_INVALID_STATE || m_state >= YRSHELL_LAST_INVALID_STATE) {
        shellERROR( __FILE__, __LINE__);
    }
}
void YRShellInterpreter::pushState( YRShellState n) {
    if( m_stateTopOfStack >= (sizeof(m_stateStack)/sizeof(m_stateStack[0])) ) {
        shellERROR( __FILE__, __LINE__);
    }
    m_stateStack[ m_stateTopOfStack++] = m_state;
    nextState( n);
}
void YRShellInterpreter::popState() {
    if( m_stateTopOfStack == 0 ) {
        shellERROR( __FILE__, __LINE__);
    }
    m_state = m_stateStack[ --m_stateTopOfStack];
    if( m_state <= YRSHELL_INVALID_STATE || m_state >= YRSHELL_LAST_INVALID_STATE) {
        shellERROR( __FILE__, __LINE__);
    }
}
void YRShellInterpreter::fillPadInternal( char c) {
    uint16_t i, j;
    if( c == '\r' || c == '\n') {
        for( i = 0; i < m_padCount; i++ ) {
            if( m_Pad[ i] != ' ' && m_Pad[ i] != '\t') {
                break;
            }
        }
        if( m_Pad[ i] == '/' && m_Pad[ i + 1] == '/') {
            m_padCount = 0;
            m_Pad[ 0] = '\0';
        }
#ifdef INPUT_BUFFER_EDITING
        if( m_lastBufferSize != 0) {
            m_lastBufferIndex = m_lastBufferFree;
            for( i = 0; i < m_padCount + 1; i++) {
                m_LastBuffer[ m_lastBufferFree++ ] = m_Pad[ i];
                if( m_lastBufferFree >= m_lastBufferSize) {
                    m_lastBufferFree = 0;
                }
            }
            j = m_lastBufferFree;
            for( i = 0; i < m_lastBufferSize + 1 && m_LastBuffer[ j] != '\0'; i++) {
                m_LastBuffer[  j++] = '\0';
                if( j >= m_lastBufferSize) {
                    j = 0;
                }
            }
        }
        nextState( YRSHELL_BEGIN_PARSING);
    } else if( c == '\x08' ) {
        if( m_padCount > 0) {
            m_Pad[ --m_padCount] = '\0';
            if( m_commandEcho) {
                outChar( ' ');
                outChar( '\x08');
            }
        }
    } else if( c == '\x05') {
        if( m_lastBufferSize != 0) {
            for( m_padCount = 0, i = 0, j = m_lastBufferIndex; i < (m_padSize - 1)  &&  m_LastBuffer[j] != '\0'; m_padCount++, i++) {
                m_Pad[ m_padCount] = m_LastBuffer[ j];
                j = (j < m_lastBufferSize - 1) ? j + 1 : 0;
            }
            m_Pad[ m_padCount] = '\0';
            m_lastBufferIndex = (m_lastBufferIndex == 0) ? m_lastBufferSize - 1 : m_lastBufferIndex - 1;
            for( i = 0; i < m_lastBufferSize &&  m_LastBuffer[ m_lastBufferIndex] == '\0'; i++ ) {
                m_lastBufferIndex = (m_lastBufferIndex == 0) ? m_lastBufferSize - 1 : m_lastBufferIndex - 1;
            }
            for( i = 0; i < m_lastBufferSize &&  m_LastBuffer[ m_lastBufferIndex] != '\0'; i++ ) {
                m_lastBufferIndex = (m_lastBufferIndex == 0) ? m_lastBufferSize - 1 : m_lastBufferIndex - 1;
            }
            m_lastBufferIndex = ((m_lastBufferIndex + 1) >= m_lastBufferSize) ? 0 : m_lastBufferIndex + 1;
            m_padCount = strlen( m_Pad);
            prompt();
            m_outputStrPtr = m_Pad;
            pushState( YRSHELL_OUTPUT_STR);
        }
    } else {
        m_Pad[ m_padCount++] = c;
        m_Pad[ m_padCount] = '\0';
    }
#else
    nextState( YRSHELL_BEGIN_PARSING);
    } else {
        m_Pad[ m_padCount++] = c;
        m_Pad[ m_padCount] = '\0';
    }
#endif
}
void YRShellInterpreter::fillPad( char c) {
#ifdef YRSHELL_DEBUG
    if( m_debugFlags & YRSHELL_DEBUG_INPUT) {
        outString("[YRShellInterpreter::fillPad ");
        outUint8X( c);
        outChar( ']');
    }
#endif
    if( m_commandEcho) {
        outChar( c);
    }
    if( m_padCount > (m_padSize - 4)) {
        reset( __FILE__, __LINE__, "INPUT BUFFER OVERFLOW");
    } else {
        fillPadInternal( c);
    }
}
#ifdef YRSHELL_DEBUG
void YRShellInterpreter::debugToken() {
    if( m_debugFlags &  YRSHELL_DEBUG_TOKEN) {
        outString("TOKEN: [");
        outString(m_token);
        outString( "]\r\n");
    }
}
#endif
void YRShellInterpreter::beginParsing(void) {
    m_saveptr = NULL;
    m_token = strtok_r(m_Pad, "\t ", &m_saveptr);
    if( m_token == NULL) {
        nextState( YRSHELL_BEGIN_IDLE  );
    } else {
        if( strcmp( m_token, ":")) {
            if( !m_DictionaryCurrent->newCompile("_")) {
                shellERROR( __FILE__, __LINE__);
            }
            if( m_compileTopOfStack) {
                shellERROR(__FILE__, __LINE__);
            }
            pushReturnStack( 0);
            m_PC = YRSHELL_DICTIONARY_CURRENT_RELATIVE | YRSHELL_DICTIONARY_CURRENT | m_DictionaryCurrent->getWordEnd();
#ifdef YRSHELL_DEBUG
            debugToken();
#endif
            if( processToken()) {
            	nextState( YRSHELL_PARSING);
            }
        } else {
            m_token = strtok_r( NULL, "\t ", &m_saveptr);
            if( m_token == NULL) {
                shellERROR( __FILE__, __LINE__, "NULL DEFINITION");
            } else {
                if( !m_DictionaryCurrent->newCompile(m_token)) {
                    shellERROR( __FILE__, __LINE__, "DICTIONARY FULL");
                } else {
#ifdef YRSHELL_DEBUG
                    if( m_debugFlags & YRSHELL_DEBUG_TOKEN) {
                        outString("\r\nCOMPILING ");
                    }
                    debugToken();
#endif
                    nextState( YRSHELL_COMPILING);
                }
            }
        }
    }
}
void YRShellInterpreter::setOutputTimeout( uint32_t t) {
    m_outputTimeoutInMilliseconds = t;
}

uint16_t YRShellInterpreter::outputSpace( ) {
    return  m_useTextOutput ? m_textBufferSize - strlen(m_TextBuffer): (m_useAuxQueues ? m_AuxOutq->free() : m_Outq->free());
}
void YRShellInterpreter::slice(void) {
    char c;

    if( ( (m_Outq->free() < (m_Outq->size()/2)) || (m_AuxOutq->free() < (m_AuxOutq->size()/2)) ) && m_state != YRSHELL_WAIT_FOR_OUTPUT_SPACE && m_state != YRSHELL_OUTPUT&& m_state != YRSHELL_OUTPUT_STR) {
        pushState( YRSHELL_WAIT_FOR_OUTPUT_SPACE);
        m_outputTimeout.setInterval(m_outputTimeoutInMilliseconds);
    }
    switch( m_state) {
    	case YRSHELL_NOT_INITIALIZED:
    		init();
    		break;
        case YRSHELL_BEGIN_IDLE:
            prompt( );
            CC_clearPad();
            m_padCount = 0;
            nextState( YRSHELL_IDLE);
            break;
        case YRSHELL_IDLE:
            if( m_useAuxQueues) {
                if( m_AuxInq->valueAvailable()) {
                    nextState( YRSHELL_FILLING_AUXPAD);
                }
            } else {
                if( m_Inq->valueAvailable()) {
                    nextState( YRSHELL_FILLING_PAD);
                }
            }
            break;
        case YRSHELL_FILLING_PAD:
            if( m_Inq->valueAvailable() ) {
                c = m_Inq->get();
                fillPad( c);
            }
            break;
        case YRSHELL_FILLING_AUXPAD:
            if( m_AuxInq->valueAvailable() ) {
                c = m_AuxInq->get();
                fillPad( c);
            }
            break;
        case YRSHELL_BEGIN_PARSING:
            beginParsing();
            break;
        case YRSHELL_PARSING:
            m_token = strtok_r(NULL, "\t ", &m_saveptr);
            processToken();
#ifdef YRSHELL_DEBUG
            debugToken();
#endif
            if( m_token == NULL) {
                nextState( YRSHELL_EXECUTING);
            }
            break;
        case YRSHELL_COMPILING:
            m_token = strtok_r(NULL, "\t ", &m_saveptr);
            processToken();
#ifdef YRSHELL_DEBUG
            debugToken();
#endif
            if( m_token == NULL) {
                if( m_compileTopOfStack) {
                    m_DictionaryCurrent->rollBack();
                    shellERROR(__FILE__, __LINE__, "INCOMPLETE CONTROL STRUCTURE");
                } else {
                    m_DictionaryCurrent->newCompileDone();
                    if( m_state != YRSHELL_OUTPUT_STR) {
                        nextState( YRSHELL_BEGIN_IDLE);
                    }
                }
            }
            break;
        case YRSHELL_EXECUTING:
            executing();
            break;
        case YRSHELL_WAIT_FOR_OUTPUT_SPACE:
            if( m_Outq->used() < (m_Outq->size()/2) && m_AuxOutq->used() < (m_AuxOutq->size()/2) ) {
                popState( );
                //nextState( m_lastState);
            } else if( m_outputTimeout.hasIntervalElapsed()) {
                if( m_outputTimeoutInMilliseconds != 0) {
                    shellERROR( __FILE__, __LINE__, "OUTPUT WAIT FOR SPACE TIMEOUT");
                } else {
                    m_Outq->drop(  m_Outq->size()/2 - m_Outq->free() );
                    m_AuxOutq->drop( m_Outq->size()/2 - m_AuxOutq->free());
                }
            }
            break;
        case YRSHELL_WAIT_DELAY:
            if( m_delayTimer.hasIntervalElapsed()) {
                popState( );
            }
            break;
        case YRSHELL_OUTPUT_STR:
            if( outputSpace() < 6) {
                if( m_outputTimeoutInMilliseconds == 0) {
                    m_Outq->drop(  m_Outq->size()/2 - m_Outq->free() );
                    m_AuxOutq->drop( m_Outq->size()/2 - m_AuxOutq->free());
                }
            } else {
                while( m_outputStrPtr != NULL && outputSpace() >= 4 ) {
                    outChar( *m_outputStrPtr++);
                    if( *m_outputStrPtr == '\0') {
                        m_outputStrPtr = NULL;
                    }
                }
                if( m_outputStrPtr == NULL) {
                    popState( );
                }
            }
            break;

            
        case YRSHELL_OUTPUT:
            if(m_outputStr0 != NULL) {
                m_outputStrPtr = m_outputStr0;
                m_outputStr0 = NULL;
                pushState( YRSHELL_OUTPUT_STR);
            } else if(m_outputStr1 != NULL) {
                m_outputStrPtr = m_outputStr1;
                m_outputStr1 = NULL;
                pushState( YRSHELL_OUTPUT_STR);
           } else if( m_outputStr2 != NULL) {
                outChar( '\r');
                m_outputStrPtr = m_outputStr2;
                m_outputStr2 = NULL;
                pushState( YRSHELL_OUTPUT_STR);
            } else if( m_outputStr3 != NULL) {
                outChar( '\r');
                m_outputStrPtr = m_outputStr3;
                m_outputStr3 = NULL;
                pushState( YRSHELL_OUTPUT_STR);
            } else if( m_outputUintValid ) {
                m_outputUintValid = false;
                outInt32( m_outputUint);
            } else {
                outChar( '\r');
                popState( );
            }
            break;
        default:
            reset( __FILE__, __LINE__);
            break;
            
    }
    if( m_state == YRSHELL_INRESET) {
        nextState( YRSHELL_BEGIN_IDLE);
        pushState( YRSHELL_OUTPUT);
    }
}
bool YRShellInterpreter::processLiteralToken( ){
    bool rc = false;
    uint32_t value;
    if( stringToUnsigned( m_token, &value) || stringToUnsignedX( m_token, &value)
#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
                || stringToFloat( m_token, &value)
#endif
            ) {
        rc = true;
        if( (value & 0xFFFF0000) == 0xFFFF0000) {
            if( !m_DictionaryCurrent->addToken( SI_CC_nint16)) {
                shellERROR( __FILE__, __LINE__);
            }
            if( !m_DictionaryCurrent->addToken( (uint16_t) value)) {
                shellERROR( __FILE__, __LINE__);
            }
        } else if( (value & 0xFFFF0000) == 0) {
            if( !m_DictionaryCurrent->addToken( SI_CC_uint16)) {
                shellERROR( __FILE__, __LINE__);
            }
            if( !m_DictionaryCurrent->addToken( (uint16_t) value)) {
                shellERROR( __FILE__, __LINE__);
            }
        } else {
            if( !m_DictionaryCurrent->addToken( SI_CC_uint32)) {
                shellERROR( __FILE__, __LINE__);
            }
            if( !m_DictionaryCurrent->addToken( (uint16_t) (0xFFFF & value))) {
                shellERROR( __FILE__, __LINE__);
            }
            if( !m_DictionaryCurrent->addToken( (uint16_t) (value >> 16))) {
                shellERROR( __FILE__, __LINE__);
            }
        }
    }
    return rc;
}
bool YRShellInterpreter::processToken( ){
	bool ret = true;
    if( m_token == NULL) {
        if( !m_DictionaryCurrent->addToken( SI_CC_return)) {
            shellERROR( __FILE__, __LINE__,  "DICTIONARY FULL");
        }
    } else {
        uint16_t rc = find( m_token);
        if( isCompileToken()) {
            executeToken(rc);
        } else {
            if( rc == YRSHELL_DICTIONARY_INVALID) {
                if( !processLiteralToken()) {
                    outString( "\rUNDEFINED: ");
                    m_outputStrPtr = m_token;
                    interpretReset();
                    nextState( YRSHELL_BEGIN_IDLE);
                    pushState( YRSHELL_OUTPUT_STR);
                    m_returnTopOfStack = 0;
                    m_compileTopOfStack = 0;
                    ret = false;
                }
            } else {
                if( (rc & YRSHELL_DICTIONARY_MASK) == YRSHELL_DICTIONARY_CURRENT) {
                    rc &= YRSHELL_DICTIONARY_ADDRESS_MASK;
                    rc |= YRSHELL_DICTIONARY_CURRENT_RELATIVE | YRSHELL_DICTIONARY_RELATIVE;
                }
                if( !m_DictionaryCurrent->addToken( rc)) {
                    shellERROR( __FILE__, __LINE__, "DICTIONARY FULL");
                }
            }
        }
    }
    return ret;
}
void YRShellInterpreter::executing( ) {
    if( (m_PC & YRSHELL_DICTIONARY_ADDRESS_MASK) == 0) {
        m_DictionaryCurrent->rollBack();
        nextState( YRSHELL_BEGIN_IDLE);
    } else {
#ifdef YRSHELL_DEBUG
        if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
            outString("[");
            outUint16X(m_PC);
            outString( ":");
        }
#endif
        uint16_t token = fetchCurrentToken();
#ifdef YRSHELL_DEBUG
        if( m_debugFlags & YRSHELL_DEBUG_EXECUTE) {
            outUint16X(token);
            outString("]");
        }
#endif
        executeToken( token);
    }
}
const char* YRShellInterpreter::getAddressFromToken( uint16_t token) {
    const char *rc = NULL;
    uint16_t address = YRSHELL_DICTIONARY_ADDRESS_MASK & token;
    uint16_t mask = YRSHELL_DICTIONARY_MASK & token;
    uint16_t index = (mask >> YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS) & YRSHELL_DICTIONARY_LAST_INDEX;
    
    if( m_textBufferSize > 0 && mask == 0 && address < m_textBufferSize) {
        rc = &m_TextBuffer[ address];
    } else {
        if( index < YRSHELL_DICTIONARY_LAST_INDEX) {
            rc = m_dictionaryList[ index]->getAddress( address);
        } else {
            rc = m_dictionaryList[ (m_PC >> YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS) & YRSHELL_DICTIONARY_LAST_INDEX ]->getAddress( address);
        }
    }
    return rc;
}
uint16_t YRShellInterpreter::getAbsoluteAddressToken( ) {
    return m_dictionaryList[ (m_PC >> YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS) & YRSHELL_DICTIONARY_LAST_INDEX ]->getMask() | (m_PC & YRSHELL_DICTIONARY_ADDRESS_MASK);
}

uint16_t YRShellInterpreter::fetchValueToken( uint16_t tok) {
    uint16_t address = YRSHELL_DICTIONARY_ADDRESS_MASK & tok;
    uint16_t mask = YRSHELL_DICTIONARY_MASK & tok;
    uint16_t index = (mask >> YRSHELL_DICTIONARY_NUMBER_ADDRESS_BITS) & YRSHELL_DICTIONARY_LAST_INDEX;
    uint16_t token = YRSHELL_DICTIONARY_INVALID;

    if( index < YRSHELL_DICTIONARY_LAST_INDEX) {
        token = m_dictionaryList[ index]->getWord( address);
    } else {
        token = m_dictionaryList[ (m_PC >> YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS) & YRSHELL_DICTIONARY_LAST_INDEX ]->getWord( address);
    }
    return token;
}
uint16_t YRShellInterpreter::fetchCurrentValueToken( ) {
    uint16_t token = fetchValueToken( m_PC);
    m_PC++;
    return token;
}

uint16_t YRShellInterpreter::fetchCurrentToken( ) {
    uint16_t token = fetchCurrentValueToken();
    if( token == YRSHELL_DICTIONARY_INVALID) {
        shellERROR( __FILE__, __LINE__);
    }
    return token;
}

void YRShellInterpreter::executeToken( uint16_t token ) {
    uint16_t mask = YRSHELL_DICTIONARY_MASK & token;
    switch( mask) {
        case YRSHELL_DICTIONARY_INTERPRETER_COMPILED:
            pushReturnStack( m_PC);
            m_PC = token | YRSHELL_DICTIONARY_INTERPRETER_COMPILED_RELATIVE;
            break;
        case YRSHELL_DICTIONARY_EXTENSION_COMPILED:
            pushReturnStack( m_PC);
            m_PC = token | YRSHELL_DICTIONARY_EXTENSION_COMPILED_RELATIVE;
            break;
        case YRSHELL_DICTIONARY_CURRENT:
            pushReturnStack( m_PC);
            m_PC = token | YRSHELL_DICTIONARY_CURRENT_RELATIVE;
            break;
        case YRSHELL_DICTIONARY_RELATIVE:
            switch( m_PC & YRSHELL_DICTIONARY_RELATIVE_MASK){
                case YRSHELL_DICTIONARY_CURRENT_RELATIVE:
                    pushReturnStack( m_PC);
                    m_PC = token | YRSHELL_DICTIONARY_CURRENT_RELATIVE;
                    break;
                case YRSHELL_DICTIONARY_INTERPRETER_COMPILED_RELATIVE:
                    pushReturnStack( m_PC);
                    m_PC = token | YRSHELL_DICTIONARY_INTERPRETER_COMPILED_RELATIVE;
                    break;
                case YRSHELL_DICTIONARY_EXTENSION_COMPILED_RELATIVE:
                    pushReturnStack( m_PC);
                    m_PC = token | YRSHELL_DICTIONARY_EXTENSION_COMPILED_RELATIVE;
                    break;
                    
            }
            break;
            
        case YRSHELL_DICTIONARY_INTERPRETER_FUNCTION:
            executeFunction(token);
            break;
        case YRSHELL_DICTIONARY_COMMON_FUNCTION:
            executeFunction(token);
            break;
        case YRSHELL_DICTIONARY_EXTENSION_FUNCTION:
            executeFunction(token);
            break;
        default:
            shellERROR( __FILE__, __LINE__);
            break;
    }
}
CircularQBase<char>& YRShellInterpreter::getInq() {
    return *m_Inq;
}
CircularQBase<char>& YRShellInterpreter::getAuxInq() {
    return *m_AuxInq;
}
CircularQBase<char>& YRShellInterpreter::getOutq() {
    return *m_Outq;
}
CircularQBase<char>& YRShellInterpreter::getAuxOutq() {
    return *m_AuxOutq;
}

void YRShellInterpreter::outCharRaw( const char c) {
    if( m_useTextOutput) {
        uint32_t s = (uint32_t) strlen( m_TextBuffer);
        if( s < ((uint32_t) (m_textBufferSize - 1)) ) {
            m_TextBuffer[ s++] = c;
            m_TextBuffer[ s] = '\0';
        } else {
            if(  m_state != YRSHELL_INRESET) {
                shellERROR( __FILE__, __LINE__, "TEXT OUTPUT BUFFER OVERFLOW");
            }
        }
    } else {
        if( m_useAuxQueues) {
            if( !m_AuxOutq->put( c) && m_state != YRSHELL_INRESET) {
                shellERROR( __FILE__, __LINE__, "AUX OUTPUT BUFFER OVERFLOW");
            }
        } else {
            if( !m_Outq->put( c) && m_state != YRSHELL_INRESET) {
                shellERROR( __FILE__, __LINE__, "OUTPUT BUFFER OVERFLOW");
            }
        }
    }
}
void YRShellInterpreter::outChar( const char c) {
    outCharRaw( c);
    if( m_expandCR && c == '\x0D') {
        outCharRaw( '\n');
    }
}
void YRShellInterpreter::outString( const char* P) {
    if( P != NULL) {
        while( *P != '\0') {
            outChar( *P++);
        }
    }
}
void YRShellInterpreter::outRawString( const char* P) {
    if( P != NULL) {
        while( *P != '\0') {
            if( *P == '\r') {
                outChar( '\\');
                outChar( 'r');
            } else if( *P == '\n') {
                outChar( '\\');
                outChar( 'n');
            } else if( *P == '\t') {
                outChar( '\\');
                outChar( 't');
            } else {
                outChar( *P);
            }
            P++;
        }
    }
}
const char* YRShellInterpreter::stringToUnsignedInternal( const char* P, uint32_t* V) {
    const char* rc = P;
    bool negative = false;
    uint32_t value = 0, numDigits = 0;
    if( *P == '-') {
        negative = -1;
        P++;
    }
    while( *P >= '0' && *P <= '9' && numDigits++ < 16 ) {
        value *= 10;
        value += *P - '0';
        P++;
    }
    rc = P;
    if( negative) {
        *V = 0-value;
    } else {
        *V = value;
    }
    return rc;
}
bool YRShellInterpreter::stringToUnsigned( const char* P, uint32_t* V){
    bool rc = false;
    if( *P == '\0') {
        rc = 0;
    } else {
        P = stringToUnsignedInternal(P, V);
        if( *P == '\0') {
            rc = true;
        }
    }
    return rc;
}
#ifdef YRSHELL_INTERPRETER_FLOATING_POINT
float YRShellInterpreter::popFloat( ) {
    float rc;
    uint32_t* P = (uint32_t*) &rc;
    uint32_t v = popParameterStack();
    *P = v;
    return rc;
}
void  YRShellInterpreter::pushFloat( float f) {
    uint32_t* P = (uint32_t*) &f;
    pushParameterStack(*P);
}
bool YRShellInterpreter::stringToFloat( const char* P, uint32_t* V) {
    bool rc = false;
    float fv = 0.0f;
    int32_t bd = 0, nd = 0, ad = 0, exp = 0;
    const char *ep;
    if( *P != '\0') {
        P = stringToUnsignedInternal(P, (uint32_t*) &bd);
        if( *P++ == '.') {
            ep = stringToUnsignedInternal(P, (uint32_t*) &ad);
            nd = (uint32_t) (ep - P);
            if( *ep == '\0') {
                rc = true;
            } else if( *ep == 'e' || *ep == 'E') {
                ep++;
                ep = stringToUnsignedInternal( ep, (uint32_t*) &exp);
                if( *ep == '\0') {
                    rc = -1;
                }
            }
        }
    }
    if( rc) {
        fv = (powf(10.0f, nd) * ((float) bd) + ((float) ad)) * powf( 10.0f, exp - nd);
    }
    uint32_t* tp = ((uint32_t*) &fv);
    *V = *tp;
    return rc;
}
void YRShellInterpreter::floatToString( float num, char *s) {
    char *P = s;
    float d, fex;
    uint8_t i;
    int ex;
    
    if( num < 0) {
        *P++ = '-';
        num = 0.0f - num;
    } else {
        *P++ = ' ';
    }
    fex =log10f( num);
    ex = (int) fex;
    d = powf(10.f, (float) ex);
    num /= d;
    if( num < 1.0f) {
        num *= 10.0f;
        ex--;
    }
    if( num >= 1.0f) {
        i = (uint8_t) num;
        *P++ = 0x30 + i;
        num -= (float) i;
    } else {
        *P++ = '0';
    }
    *P++ = '.';
    unsignedToStringZero((uint32_t) (num * 1000000.0f), 6, P);
    P += 6;
    *P++ = 'e';
    if( ex < 0){
        *P++ = '-';
        ex = 0 - ex;
    } else {
        *P++ = '+';
    }
    unsignedToStringZero((uint32_t) ex, 2, P);
}
void YRShellInterpreter::floatEngToString( float num, char *s) {
    char sign, *P = s, *P1;
    float d, fex;
    int eex, ex, dc;
    
    if( num < 0) {
        sign = '-';
        num = 0.0f - num;
    } else {
        sign = ' ';
    }
    fex =log10f(num);
    eex = (int) fex;
    ex = 3* (eex/3);
    if( eex < 0){
        ex -= 3;
    }
    d = powf(10.f, (float) ex);
    dc = eex - ex;
    if(eex < 0 ) {
        dc--;
    }
    
    num /= d;
    unsignedToStringZero((uint32_t) num , 4, P);
    P1 = P;
    while( *P1 =='0') {
        *P1++ = ' ';
    }
    if( *P1 == '\0') {
        *--P1 = '0';
    }
    *--P1 = sign;
    P+= 4;
    *P++ = '.';
    unsignedToStringZero((uint32_t) (num * 1000000.0f), 6, P);
    P += 6;
    P1 = P;
    while( dc-- > 0) {
        *--P1 = '0';
    }
    *P++ = 'e';
    if( ex < 0) {
        *P++ = '-';
        ex = 0 - ex;
    } else {
        *P++ = '+';
    }
    unsignedToStringZero((uint32_t) ex, 2, P);
}
void YRShellInterpreter::outFloat( float num) {
    char buf[ 16];
    floatToString(num, buf);
    outString( buf);
}
void YRShellInterpreter::outEngFloat( float num) {
    char buf[ 16];
    floatEngToString(num, buf);
    outString( buf);
}
#endif
char YRShellInterpreter::charToHex( char c) {
    char value = '\0';
    if(  c >= '0' && c <= '9' ) {
        value |= c - '0';
    } else if(  c >= 'a' && c <= 'f' ) {
        value |= c - 'a' + 10;
    } else if(  c >= 'A' && c <= 'F') {
        value |= c - 'A' + 10;
    }
    return value;
}
bool YRShellInterpreter::stringToUnsignedX( const char* P, uint32_t* V){
    bool rc = false;
    uint32_t value = 0, numDigits = 0;
    if( (*P != '0') || (*(P+1) != 'x')) {
        rc = false;
    } else {
        P +=2;
        while( ((*P >= '0' && *P <= '9') ||  (*P >= 'a' && *P <= 'f')  ||  (*P >= 'A' && *P <= 'F'))  && numDigits++ < 16 ) {
            value <<= 4;
            value |= charToHex( *P++);
        }
        if( *P == '\0') {
            rc = true;
        }
        *V = value;
    }
    return rc;
    
}
void YRShellInterpreter::unsignedToStringZero(uint32_t num, uint8_t numDigits, char *s) {
    char *P = s + numDigits;
    *P-- = '\0';
    while( P >= s) {
        *P-- = '0' + (num % 10);
        num /= 10;
    }
}
void YRShellInterpreter::unsignedToString(uint32_t num, uint8_t numDigits, char *s) {
    unsignedToStringZero( num, numDigits, s);
    while( *s == '0' && numDigits-- > 1) {
        *s++ = ' ';
    }
}
void YRShellInterpreter::unsignedToStringX(uint32_t num, uint8_t numDigits, char *s){
    *s++ = '0';
    *s++ = 'x';
    char *P = s + numDigits;
    char c;
    *P-- = '\0';
    while( P >= s) {
        c = num & 0xF;
        if( c > 9) {
            c += '7';
        } else {
            c += '0';
        }
        *P-- =  c;
        num >>= 4;
    }
}
void YRShellInterpreter::intToString(int32_t num, uint8_t numDigits, char *s) {
    *s++ = ' ';
    if( num < 0) {
        unsignedToString( (unsigned) (0 - num), numDigits, s);
        while( *s == ' ') {
            s++;
        }
        s--;
        *s = '-';
    } else {
        unsignedToString( (unsigned) num, numDigits, s);
    }
}
void YRShellInterpreter::outUint8( uint8_t v) {
    char buf[ 4];
    unsignedToString(v, 3, buf);
    outString( buf);
}
void YRShellInterpreter::outUint16( uint16_t v) {
    char buf[ 6];
    unsignedToString(v, 5, buf);
    outString( buf);
}
void YRShellInterpreter::outUint32( uint32_t v) {
    char buf[ 10];
    unsignedToString(v, 9, buf);
    outString( buf);
}
void YRShellInterpreter::outUint8X( uint8_t v) {
    char buf[ 5];
    unsignedToStringX(v, 2, buf);
    outString( buf);
}
void YRShellInterpreter::outUint16X( uint16_t v) {
    char buf[ 7];
    unsignedToStringX(v, 4, buf);
    outString( buf);
}
void YRShellInterpreter::outUint32X( uint32_t v) {
    char buf[ 11];
    unsignedToStringX(v, 8, buf);
    outString( buf);
    
}
void YRShellInterpreter::outUint32Xn( uint32_t v) {
    char *P, buf[ 11];
    unsignedToStringX(v, 8, buf);
    P = &buf[2];
    while( *P == '0') {
        P++;
    }
    if( *P == '\0') {
        P--;
    }
    *--P = 'x';
    *--P = '0';
    outString( P);
    
}
void YRShellInterpreter::outInt8( int8_t v) {
    char buf[ 5];
    intToString(v, 3, buf);
    outString( buf);
}
void YRShellInterpreter::outInt16( int16_t v) {
    char buf[ 7];
    intToString(v, 5, buf);
    outString( buf);
}
void YRShellInterpreter::outInt32( int32_t v) {
    char buf[ 12];
    intToString(v, 10, buf);
    outString( buf);
}
void YRShellInterpreter::outInt32n( int32_t v) {
    char *P, buf[ 12];
    intToString(v, 10, buf);
    P = buf;
    while( *P == ' ') {
        P++;
    }
    outString( P);
}
uint32_t YRShellInterpreter::popParameterStack( ) {
    uint32_t rc = 0;
    if(m_topOfStack > 0) {
        rc = m_ParameterStack[ --m_topOfStack];
    } else {
        shellERROR( __FILE__, __LINE__, "STACK UNDERFLOW\r\n");
    }
    return rc;
}
uint32_t YRShellInterpreter::popReturnStack( ) {
    uint32_t rc = 0;
    if(m_returnTopOfStack > 0) {
        rc = m_ReturnStack[ --m_returnTopOfStack];
    } else {
        shellERROR( __FILE__, __LINE__, "RETURN STACK UNDERFLOW\r\n");
    }
    return rc;
}
uint32_t YRShellInterpreter::popCompileStack( ) {
    uint32_t rc = 0;
    if(m_compileTopOfStack > 0) {
        rc = m_CompileStack[ --m_compileTopOfStack];
    } else {
        shellERROR( __FILE__, __LINE__, "COMPILE STACK UNDERFLOW\r\n");
    }
    return rc;
}
void YRShellInterpreter::pushParameterStack( uint32_t v) {
    if( m_topOfStack < m_parameterStackSize){
        m_ParameterStack[ m_topOfStack++] = v;
    } else {
        shellERROR( __FILE__, __LINE__, "STACK OVERFLOW\r\n");
    }
}
void YRShellInterpreter::pushReturnStack( uint32_t v) {
    if( m_returnTopOfStack < m_returnStackSize){
        m_ReturnStack[ m_returnTopOfStack++] = v;
    } else {
        shellERROR( __FILE__, __LINE__, "RETURN STACK OVERFLOW\r\n");
    }
}
void YRShellInterpreter::pushCompileStack( uint32_t v) {
    if( m_compileTopOfStack < m_compileStackSize){
        m_CompileStack[ m_compileTopOfStack++] = v;
    } else {
        shellERROR( __FILE__, __LINE__, "RETURN STACK OVERFLOW\r\n");
    }
}
void YRShellInterpreter::CC_clearPad() {
    memset(m_Pad, '\0',  m_padSize);
}
void YRShellInterpreter::setPrompt( const char* prompt ) {
    m_prompt = prompt;
}

