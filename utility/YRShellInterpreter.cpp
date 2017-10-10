
#include "YRShellInterpreter.h"
/*
 

 : ps? s' PARAMETER STACK: ' .str psd? 0!= [ psd? { dup ps@ . 1 - dup 0== } drop ] cr
 : rs? s'    RETURN STACK: ' .str rsd? 0!= [ rsd? 1 - { dup rs@ . 1 - dup -1 == } drop ] cr
 : cs? s'   COMPILE STACK: ' .str csd? 0!= [ csd? 1 - { dup cs@ . 1 - dup -1 == } drop ] cr
 : st? ps? rs? cs?
 
 : _cd0 s' \rstatic uint16_t ' .str
 : _cd1 _cd0 s' compiledDictionaryData[] = {\r' .str
 : _cx1 _cd0 s' compiledExtensionDictionaryData[] = {\r' .str
 : _cd2 dup 0xF & 0== [ cr s' // ' .str dup .wx cr ]
 : _cd3 0 { _cd2 dup getCurrentDictionary .wx s' , ' .str 1 + dup getCurrentDictionaryEnd >= } drop
 : _cd4 getCurrentDictionaryEnd 0!= [ _cd3 crlf ] s' };\r' .str
 : _cd5 s' CompiledDictionary ' .str
 
 : _cd6 s' interpreterCompiledDictionary( compiledDictionaryData, ' .str
 : _cx6 s' compiledExtensionDictionary( compiledExtensionDictionaryData, ' .str
 : _cd7 getCurrentDictionaryLastWord .wx s' , ' .str getCurrentDictionaryEnd .wx
 : _cd8 _cd1 _cd4 _cd5 _cd6 _cd7  s' , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);\r' .str
 : _cx8 _cx1 _cd4 _cd5 _cx6 _cd7 s' , YRSHELL_DICTIONARY_EXTENSION_COMPILED);\r' .str
 : compileInterpreterDictionary _cd8
 : compileExtensionDictionary _cx8
 
 : spaces dup [ { space 1 - dup 0== } ] drop 
 : dictInvalid 0xFFFF
 : _wl2 rot dup [ cr ] 0== rot rot dup dictInvalid ==
 : _wl3 .entryName 40 swap - spaces
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
 : _infa s'           Shell Size: ' .str shellSize . cr
 : _infb s' Dictionary used free: ' .str dictionarySize getCurrentDictionaryEnd dup . - . cr
 : info printShellClass cr  _inf0 _inf1 _inf2 _inf3 _inf4 _inf5 _inf6 _inf7 _inf8 _inf9 _infa _infb
 
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




YRShell Version 0.1
bShell>
 : ps? s' PARAMETER STACK: ' .str psd? 0!= [ psd? { dup ps@ . 1 - dup 0== } drop ] cr
 : rs? s'    RETURN STACK: ' .str rsd? 0!= [ rsd? 1 - { dup rs@ . 1 - dup -1 == } drop ] cr
 : cs? s'   COMPILE STACK: ' .str csd? 0!= [ csd? 1 - { dup cs@ . 1 - dup -1 == } drop ] cr
 : st? ps? rs? cs?
 
 : _cd0 s' \rstatic uint16_t ' .str
 : _cd1 _cd0 s' compiledDictionaryData[] = {\r' .str
 : _cx1 _cd0 s' compiledExtensionDictionaryData[] = {\r' .str
 : _cd2 dup 0xF & 0== [ cr s' // ' .str dup .wx cr ]
 : _cd3 0 { _cd2 dup getCurrentDictionary .wx s' , ' .str 1 + dup getCurrentDictionaryEnd >= } drop
 : _cd4 getCurrentDictionaryEnd 0!= [ _cd3 crlf ] s' };\r' .str
 : _cd5 s' CompiledDictionary ' .str
 
 : _cd6 s' interpreterCompiledDictionary( compiledDictionaryData, ' .str
 : _cx6 s' compiledExtensionDictionary( compiledExtensionDictionaryData, ' .str
 : _cd7 getCurrentDictionaryLastWord .wx s' , ' .str getCurrentDictionaryEnd .wx
 : _cd8 _cd1 _cd4 _cd5 _cd6 _cd7  s' , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);\r' .str
 : _cx8 _cx1 _cd4 _cd5 _cx6 _cd7 s' , YRSHELL_DICTIONARY_EXTENSION_COMPILED);\r' .str
 : compileInterpreterDictionary _cd8
 : compileExtensionDictionary _cx8
 
 : spaces dup [ { space 1 - dup 0== } ] drop 
 : dictInvalid 0xFFFF
 : _wl2 rot dup [ cr ] 0== rot rot dup dictInvalid ==
 : _wl3 .entryName 40 swap - spaces
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
 : _infa s'           Shell Size: ' .str shellSize . cr
 : _infb s' Dictionary used free: ' .str dictionarySize getCurrentDictionaryEnd dup . - . cr
 : info printShellClass cr  _inf0 _inf1 _inf2 _inf3 _inf4 _inf5 _inf6 _inf7 _inf8 _inf9 _infa _infb
 
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

bShell> : ps? s' PARAMETER STACK: ' .str psd? 0!= [ psd? { dup ps@ . 1 - dup 0== } drop ] cr
bShell> : rs? s'    RETURN STACK: ' .str rsd? 0!= [ rsd? 1 - { dup rs@ . 1 - dup -1 == } drop ] cr
bShell> : cs? s'   COMPILE STACK: ' .str csd? 0!= [ csd? 1 - { dup cs@ . 1 - dup -1 == } drop ] cr
bShell> : st? ps? rs? cs?
bShell> 
bShell> : _cd0 s' \rstatic uint16_t ' .str
bShell> : _cd1 _cd0 s' compiledDictionaryData[] = {\r' .str
bShell> : _cx1 _cd0 s' compiledExtensionDictionaryData[] = {\r' .str
bShell> : _cd2 dup 0xF & 0== [ cr s' // ' .str dup .wx cr ]
bShell> : _cd3 0 { _cd2 dup getCurrentDictionary .wx s' , ' .str 1 + dup getCurrentDictionaryEnd >= } drop
bShell> : _cd4 getCurrentDictionaryEnd 0!= [ _cd3 crlf ] s' };\r' .str
bShell> : _cd5 s' CompiledDictionary ' .str
bShell> 
bShell> : _cd6 s' interpreterCompiledDictionary( compiledDictionaryData, ' .str
bShell> : _cx6 s' compiledExtensionDictionary( compiledExtensionDictionaryData, ' .str
bShell> : _cd7 getCurrentDictionaryLastWord .wx s' , ' .str getCurrentDictionaryEnd .wx
bShell> : _cd8 _cd1 _cd4 _cd5 _cd6 _cd7  s' , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);\r' .str
bShell> : _cx8 _cx1 _cd4 _cd5 _cx6 _cd7 s' , YRSHELL_DICTIONARY_EXTENSION_COMPILED);\r' .str
bShell> : compileInterpreterDictionary _cd8
bShell> : compileExtensionDictionary _cx8
bShell> 
bShell> : spaces dup [ { space 1 - dup 0== } ] drop 
bShell> : dictInvalid 0xFFFF
bShell> : _wl2 rot dup [ cr ] 0== rot rot dup dictInvalid ==
bShell> : _wl3 .entryName 40 swap - spaces
bShell> : wl 0 0 dictInvalid { nextEntry over .bx dup .wx 2dup entryToken .wx 2dup _wl3 _wl2 } 2drop drop cr
bShell> 
bShell> : _inf0 s'      DICTIONARY_SIZE: ' .str dictionarySize . cr
bShell> : _inf1 s'             PAD_SIZE: ' .str padSize . cr
bShell> : _inf2 s'        NUM_REGISTERS: ' .str numRegisters . cr
bShell> : _inf3 s' PARAMETER_STACK_SIZE: ' .str parameterStackSize . cr
bShell> : _inf4 s'    RETURN_STACK_SIZE: ' .str returnStackSize . cr
bShell> : _inf5 s'   COMPILE_STACK_SIZE: ' .str compileStackSize . cr
bShell> : _inf6 s'             INQ_SIZE: ' .str inqSize . cr
bShell>
bShell> : _inf7 s'         AUX_INQ_SIZE: ' .str auxInqSize . cr
bShell> : _inf8 s'            OUTQ_SIZE: ' .str outqSize . cr
bShell> : _inf9 s'        AUX_OUTQ_SIZE: ' .str auxOutqSize . cr
bShell> : _infa s'           Shell Size: ' .str shellSize . cr
bShell> : _infb s' Dictionary used free: ' .str dictionarySize getCurrentDictionaryEnd dup . - . cr
bShell> : info printShellClass cr  _inf0 _inf1 _inf2 _inf3 _inf4 _inf5 _inf6 _inf7 _inf8 _inf9 _infa _infb
bShell> 
bShell> : _ts0 s'       COUNT' .str
bShell> : _ts1 s'          MIN' .str
bShell> : _ts2 s'         MAX' .str
bShell> : _ts3 s'       AVERAGE' .str
bShell> : _ts4 _ts0 _ts1 _ts2 _ts3 cr cr
bShell> : _ts5 dup sliceStats dup >r [ . . . . ] r> dup [ over printSliceName ] cr
bShell> : runStats _ts4 0 { _ts5 [ 1 + 0 ][ -1 ] } drop
bShell> : resetStats 0 { dup clearStats [ 1 + 0 ][ -1 ] } drop
bShell>
bShell> : _dr0  dup numRegisters < [  dup @ . 1 + ]
bShell> : _dr1 dup . s' : ' .str  dup @ . _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 _dr0 cr
bShell> : regs 0 { _dr1 dup numRegisters >= } drop
bShell>compileInterpreterDictionary
compileInterpreterDictionary

static uint16_t compiledDictionaryData[] = {

// 0x0000 
0xFFFF , 0x7370 , 0x003F , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , 0x2052 , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC037 , 0xC03F , 
// 0x0010 
0xC01D , 0xE01E , 0xC037 , 0xC01E , 0xC03A , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC01D , 0xE013 , 0xC022 , 0xC003 , 0xC001 , 
// 0x0020 
0x0000 , 0x7372 , 0x003F , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 0x204E , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC038 , 0xC03F , 
// 0x0030 
0xC01D , 0xE043 , 0xC038 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03B , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC00B , 0xFFFF , 0xC031 , 
// 0x0040 
0xC01D , 0xE036 , 0xC022 , 0xC003 , 0xC001 , 0x0020 , 0x7363 , 0x003F , 0xC016 , 0x2020 , 0x4F43 , 0x504D , 0x4C49 , 0x2045 , 0x5453 , 0x4341 , 
// 0x0050 
0x3A4B , 0x0020 , 0xC012 , 0xC039 , 0xC03F , 0xC01D , 0xE068 , 0xC039 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03C , 0xC00C , 0xC009 , 0x0001 , 
// 0x0060 
0xC028 , 0xC01E , 0xC00B , 0xFFFF , 0xC031 , 0xC01D , 0xE05B , 0xC022 , 0xC003 , 0xC001 , 0x0045 , 0x7473 , 0x003F , 0xE003 , 0xE023 , 0xE048 , 
// 0x0070 
0xC001 , 0x006A , 0x635F , 0x3064 , 0x0000 , 0xC016 , 0x730D , 0x6174 , 0x6974 , 0x2063 , 0x6975 , 0x746E , 0x3631 , 0x745F , 0x0020 , 0xC012 , 
// 0x0080 
0xC001 , 0x0071 , 0x635F , 0x3164 , 0x0000 , 0xE075 , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 
// 0x0090 
0x6144 , 0x6174 , 0x5D5B , 0x3D20 , 0x7B20 , 0x000D , 0xC012 , 0xC001 , 0x0081 , 0x635F , 0x3178 , 0x0000 , 0xE075 , 0xC016 , 0x6F63 , 0x706D , 
// 0x00A0 
0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 0x446E , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x4479 , 0x7461 , 0x5B61 , 0x205D , 0x203D , 
// 0x00B0 
0x0D7B , 0x0000 , 0xC012 , 0xC001 , 0x0098 , 0x635F , 0x3264 , 0x0000 , 0xC01E , 0xC009 , 0x000F , 0xC02C , 0xC03E , 0xC01D , 0xE0C7 , 0xC003 , 
// 0x00C0 
0xC016 , 0x2F2F , 0x0020 , 0xC012 , 0xC01E , 0xC011 , 0xC003 , 0xC001 , 0x00B4 , 0x635F , 0x3364 , 0x0000 , 0xC009 , 0x0000 , 0xE0B8 , 0xC01E , 
// 0x00D0 
0xC044 , 0xC011 , 0xC016 , 0x202C , 0x0000 , 0xC012 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC045 , 0xC033 , 0xC01D , 0xE0CE , 0xC022 , 0xC001 , 
// 0x00E0 
0x00C8 , 0x635F , 0x3464 , 0x0000 , 0xC045 , 0xC03F , 0xC01D , 0xE0EA , 0xE0CC , 0xC005 , 0xC016 , 0x3B7D , 0x000D , 0xC012 , 0xC001 , 0x00E0 , 
// 0x00F0 
0x635F , 0x3564 , 0x0000 , 0xC016 , 0x6F43 , 0x706D , 0x6C69 , 0x6465 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0020 , 0xC012 , 0xC001 , 
// 0x0100 
0x00EF , 0x635F , 0x3664 , 0x0000 , 0xC016 , 0x6E69 , 0x6574 , 0x7072 , 0x6572 , 0x6574 , 0x4372 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 
// 0x0110 
0x6974 , 0x6E6F , 0x7261 , 0x2879 , 0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x4479 , 0x7461 , 0x2C61 , 
// 0x0120 
0x0020 , 0xC012 , 0xC001 , 0x0100 , 0x635F , 0x3678 , 0x0000 , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 
// 0x0130 
0x446E , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x2879 , 0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4564 , 0x7478 , 0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 
// 0x0140 
0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x6144 , 0x6174 , 0x202C , 0x0000 , 0xC012 , 0xC001 , 0x0123 , 0x635F , 0x3764 , 0x0000 , 0xC046 , 0xC011 , 
// 0x0150 
0xC016 , 0x202C , 0x0000 , 0xC012 , 0xC045 , 0xC011 , 0xC001 , 0x014A , 0x635F , 0x3864 , 0x0000 , 0xE085 , 0xE0E4 , 0xE0F3 , 0xE104 , 0xE14E , 
// 0x0160 
0xC016 , 0x202C , 0x5259 , 0x4853 , 0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x495F , 0x544E , 0x5245 , 0x5250 , 0x5445 , 
// 0x0170 
0x5245 , 0x435F , 0x4D4F , 0x4950 , 0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x0157 , 0x635F , 0x3878 , 0x0000 , 0xE09C , 0xE0E4 , 
// 0x0180 
0xE0F3 , 0xE127 , 0xE14E , 0xC016 , 0x202C , 0x5259 , 0x4853 , 0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x455F , 0x5458 , 
// 0x0190 
0x4E45 , 0x4953 , 0x4E4F , 0x435F , 0x4D4F , 0x4950 , 0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x017A , 0x6F63 , 0x706D , 0x6C69 , 
// 0x01A0 
0x4965 , 0x746E , 0x7265 , 0x7270 , 0x7465 , 0x7265 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE15B , 0xC001 , 0x019C , 0x6F63 , 
// 0x01B0 
0x706D , 0x6C69 , 0x4565 , 0x7478 , 0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE17E , 0xC001 , 0x01AE , 
// 0x01C0 
0x7073 , 0x6361 , 0x7365 , 0x0000 , 0xC01E , 0xC01D , 0xE1CF , 0xC002 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC01D , 0xE1C7 , 0xC022 , 
// 0x01D0 
0xC001 , 0x01BF , 0x6964 , 0x7463 , 0x6E49 , 0x6176 , 0x696C , 0x0064 , 0xC009 , 0xFFFF , 0xC001 , 0x01D1 , 0x775F , 0x326C , 0x0000 , 0xC023 , 
// 0x01E0 
0xC01E , 0xC01D , 0xE1E4 , 0xC003 , 0xC03E , 0xC023 , 0xC023 , 0xC01E , 0xE1D8 , 0xC031 , 0xC001 , 0x01DB , 0x775F , 0x336C , 0x0000 , 0xC049 , 
// 0x01F0 
0xC009 , 0x0028 , 0xC01F , 0xC028 , 0xE1C4 , 0xC001 , 0x01EB , 0x6C77 , 0x0000 , 0xC009 , 0x0000 , 0xC009 , 0x0000 , 0xE1D8 , 0xC048 , 0xC040 , 
// 0x0200 
0xC010 , 0xC01E , 0xC011 , 0xC041 , 0xC04A , 0xC011 , 0xC041 , 0xE1EF , 0xE1DF , 0xC01D , 0xE1FE , 0xC042 , 0xC022 , 0xC003 , 0xC001 , 0x01F6 , 
// 0x0210 
0x695F , 0x666E , 0x0030 , 0xC016 , 0x2020 , 0x2020 , 0x4420 , 0x4349 , 0x4954 , 0x4E4F , 0x5241 , 0x5F59 , 0x4953 , 0x455A , 0x203A , 0x0000 , 
// 0x0220 
0xC012 , 0xC053 , 0xC00C , 0xC003 , 0xC001 , 0x020F , 0x695F , 0x666E , 0x0031 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 
// 0x0230 
0x4150 , 0x5F44 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC054 , 0xC00C , 0xC003 , 0xC001 , 0x0225 , 0x695F , 0x666E , 0x0032 , 0xC016 , 
// 0x0240 
0x2020 , 0x2020 , 0x2020 , 0x4E20 , 0x4D55 , 0x525F , 0x4745 , 0x5349 , 0x4554 , 0x5352 , 0x203A , 0x0000 , 0xC012 , 0xC055 , 0xC00C , 0xC003 , 
// 0x0250 
0xC001 , 0x023B , 0x695F , 0x666E , 0x0033 , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , 0x5F52 , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 
// 0x0260 
0x203A , 0x0000 , 0xC012 , 0xC056 , 0xC00C , 0xC003 , 0xC001 , 0x0251 , 0x695F , 0x666E , 0x0034 , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 
// 0x0270 
0x5F4E , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC057 , 0xC00C , 0xC003 , 0xC001 , 0x0267 , 0x695F , 0x666E , 
// 0x0280 
0x0035 , 0xC016 , 0x2020 , 0x4F43 , 0x504D , 0x4C49 , 0x5F45 , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC058 , 
// 0x0290 
0xC00C , 0xC003 , 0xC001 , 0x027D , 0x695F , 0x666E , 0x0036 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4E49 , 0x5F51 , 
// 0x02A0 
0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC059 , 0xC00C , 0xC003 , 0xC001 , 0x0293 , 0x695F , 0x666E , 0x0037 , 0xC016 , 0x2020 , 0x2020 , 
// 0x02B0 
0x2020 , 0x2020 , 0x5541 , 0x5F58 , 0x4E49 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05A , 0xC00C , 0xC003 , 0xC001 , 0x02A9 , 
// 0x02C0 
0x695F , 0x666E , 0x0038 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4F20 , 0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 
// 0x02D0 
0xC012 , 0xC05B , 0xC00C , 0xC003 , 0xC001 , 0x02BF , 0x695F , 0x666E , 0x0039 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4120 , 0x5855 , 0x4F5F , 
// 0x02E0 
0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05C , 0xC00C , 0xC003 , 0xC001 , 0x02D5 , 0x695F , 0x666E , 0x0061 , 0xC016 , 
// 0x02F0 
0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x6853 , 0x6C65 , 0x206C , 0x6953 , 0x657A , 0x203A , 0x0000 , 0xC012 , 0xC051 , 0xC00C , 0xC003 , 
// 0x0300 
0xC001 , 0x02EB , 0x695F , 0x666E , 0x0062 , 0xC016 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x7520 , 0x6573 , 0x2064 , 0x7266 , 0x6565 , 
// 0x0310 
0x203A , 0x0000 , 0xC012 , 0xC053 , 0xC045 , 0xC01E , 0xC00C , 0xC028 , 0xC00C , 0xC003 , 0xC001 , 0x0301 , 0x6E69 , 0x6F66 , 0x0000 , 0xC052 , 
// 0x0320 
0xC003 , 0xE213 , 0xE229 , 0xE23F , 0xE255 , 0xE26B , 0xE281 , 0xE297 , 0xE2AD , 0xE2C3 , 0xE2D9 , 0xE2EF , 0xE305 , 0xC001 , 0x031B , 0x745F , 
// 0x0330 
0x3073 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4F43 , 0x4E55 , 0x0054 , 0xC012 , 0xC001 , 0x032E , 0x745F , 0x3173 , 0x0000 , 0xC016 , 
// 0x0340 
0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4D20 , 0x4E49 , 0x0000 , 0xC012 , 0xC001 , 0x033B , 0x745F , 0x3273 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 
// 0x0350 
0x2020 , 0x2020 , 0x414D , 0x0058 , 0xC012 , 0xC001 , 0x0349 , 0x745F , 0x3373 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x5641 , 0x5245 , 
// 0x0360 
0x4741 , 0x0045 , 0xC012 , 0xC001 , 0x0356 , 0x745F , 0x3473 , 0x0000 , 0xE332 , 0xE33F , 0xE34D , 0xE35A , 0xC003 , 0xC003 , 0xC001 , 0x0364 , 
// 0x0370 
0x745F , 0x3573 , 0x0000 , 0xC01E , 0xC066 , 0xC01E , 0xC024 , 0xC01D , 0xE37D , 0xC00C , 0xC00C , 0xC00C , 0xC00C , 0xC025 , 0xC01E , 0xC01D , 
// 0x0380 
0xE383 , 0xC040 , 0xC067 , 0xC003 , 0xC001 , 0x036F , 0x7572 , 0x536E , 0x6174 , 0x7374 , 0x0000 , 0xE368 , 0xC009 , 0x0000 , 0xE373 , 0xC01D , 
// 0x0390 
0xE398 , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 0xC01C , 0xE39A , 0xC00B , 0xFFFF , 0xC01D , 0xE38E , 0xC022 , 0xC001 , 0x0385 , 0x6572 , 
// 0x03A0 
0x6573 , 0x5374 , 0x6174 , 0x7374 , 0x0000 , 0xC009 , 0x0000 , 0xC01E , 0xC065 , 0xC01D , 0xE3B2 , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 
// 0x03B0 
0xC01C , 0xE3B4 , 0xC00B , 0xFFFF , 0xC01D , 0xE3A7 , 0xC022 , 0xC001 , 0x039E , 0x645F , 0x3072 , 0x0000 , 0xC01E , 0xC055 , 0xC02F , 0xC01D , 
// 0x03C0 
0xE3C7 , 0xC01E , 0xC064 , 0xC00C , 0xC009 , 0x0001 , 0xC027 , 0xC001 , 0x03B8 , 0x645F , 0x3172 , 0x0000 , 0xC01E , 0xC00C , 0xC016 , 0x203A , 
// 0x03D0 
0x0000 , 0xC012 , 0xC01E , 0xC064 , 0xC00C , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xC003 , 0xC001 , 0x03C8 , 
// 0x03E0 
0x6572 , 0x7367 , 0x0000 , 0xC009 , 0x0000 , 0xE3CC , 0xC01E , 0xC055 , 0xC033 , 0xC01D , 0xE3E5 , 0xC022 , 0xC001 , 

};
CompiledDictionary interpreterCompiledDictionary( compiledDictionaryData, 0x03DF , 0x03ED , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);
bShell>


 */

/*
static uint16_t compiledDictionaryData[] = {
};
YRShellCompiledDictionary compiledDictionary( compiledDictionaryData, 0xFFFF , 0x0000 , YRSHELL_DICTIONARY_COMPILED);
*/

static uint16_t compiledDictionaryData[] = {

// 0x0000 
0xFFFF , 0x7370 , 0x003F , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , 0x2052 , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC037 , 0xC03F , 
// 0x0010 
0xC01D , 0xE01E , 0xC037 , 0xC01E , 0xC03A , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC01D , 0xE013 , 0xC022 , 0xC003 , 0xC001 , 
// 0x0020 
0x0000 , 0x7372 , 0x003F , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 0x204E , 0x5453 , 0x4341 , 0x3A4B , 0x0020 , 0xC012 , 0xC038 , 0xC03F , 
// 0x0030 
0xC01D , 0xE043 , 0xC038 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03B , 0xC00C , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC00B , 0xFFFF , 0xC031 , 
// 0x0040 
0xC01D , 0xE036 , 0xC022 , 0xC003 , 0xC001 , 0x0020 , 0x7363 , 0x003F , 0xC016 , 0x2020 , 0x4F43 , 0x504D , 0x4C49 , 0x2045 , 0x5453 , 0x4341 , 
// 0x0050 
0x3A4B , 0x0020 , 0xC012 , 0xC039 , 0xC03F , 0xC01D , 0xE068 , 0xC039 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03C , 0xC00C , 0xC009 , 0x0001 , 
// 0x0060 
0xC028 , 0xC01E , 0xC00B , 0xFFFF , 0xC031 , 0xC01D , 0xE05B , 0xC022 , 0xC003 , 0xC001 , 0x0045 , 0x7473 , 0x003F , 0xE003 , 0xE023 , 0xE048 , 
// 0x0070 
0xC001 , 0x006A , 0x635F , 0x3064 , 0x0000 , 0xC016 , 0x730D , 0x6174 , 0x6974 , 0x2063 , 0x6975 , 0x746E , 0x3631 , 0x745F , 0x0020 , 0xC012 , 
// 0x0080 
0xC001 , 0x0071 , 0x635F , 0x3164 , 0x0000 , 0xE075 , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 
// 0x0090 
0x6144 , 0x6174 , 0x5D5B , 0x3D20 , 0x7B20 , 0x000D , 0xC012 , 0xC001 , 0x0081 , 0x635F , 0x3178 , 0x0000 , 0xE075 , 0xC016 , 0x6F63 , 0x706D , 
// 0x00A0 
0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 0x446E , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x4479 , 0x7461 , 0x5B61 , 0x205D , 0x203D , 
// 0x00B0 
0x0D7B , 0x0000 , 0xC012 , 0xC001 , 0x0098 , 0x635F , 0x3264 , 0x0000 , 0xC01E , 0xC009 , 0x000F , 0xC02C , 0xC03E , 0xC01D , 0xE0C7 , 0xC003 , 
// 0x00C0 
0xC016 , 0x2F2F , 0x0020 , 0xC012 , 0xC01E , 0xC011 , 0xC003 , 0xC001 , 0x00B4 , 0x635F , 0x3364 , 0x0000 , 0xC009 , 0x0000 , 0xE0B8 , 0xC01E , 
// 0x00D0 
0xC044 , 0xC011 , 0xC016 , 0x202C , 0x0000 , 0xC012 , 0xC009 , 0x0001 , 0xC027 , 0xC01E , 0xC045 , 0xC033 , 0xC01D , 0xE0CE , 0xC022 , 0xC001 , 
// 0x00E0 
0x00C8 , 0x635F , 0x3464 , 0x0000 , 0xC045 , 0xC03F , 0xC01D , 0xE0EA , 0xE0CC , 0xC005 , 0xC016 , 0x3B7D , 0x000D , 0xC012 , 0xC001 , 0x00E0 , 
// 0x00F0 
0x635F , 0x3564 , 0x0000 , 0xC016 , 0x6F43 , 0x706D , 0x6C69 , 0x6465 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0020 , 0xC012 , 0xC001 , 
// 0x0100 
0x00EF , 0x635F , 0x3664 , 0x0000 , 0xC016 , 0x6E69 , 0x6574 , 0x7072 , 0x6572 , 0x6574 , 0x4372 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 
// 0x0110 
0x6974 , 0x6E6F , 0x7261 , 0x2879 , 0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4464 , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x4479 , 0x7461 , 0x2C61 , 
// 0x0120 
0x0020 , 0xC012 , 0xC001 , 0x0100 , 0x635F , 0x3678 , 0x0000 , 0xC016 , 0x6F63 , 0x706D , 0x6C69 , 0x6465 , 0x7845 , 0x6574 , 0x736E , 0x6F69 , 
// 0x0130 
0x446E , 0x6369 , 0x6974 , 0x6E6F , 0x7261 , 0x2879 , 0x6320 , 0x6D6F , 0x6970 , 0x656C , 0x4564 , 0x7478 , 0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 
// 0x0140 
0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x6144 , 0x6174 , 0x202C , 0x0000 , 0xC012 , 0xC001 , 0x0123 , 0x635F , 0x3764 , 0x0000 , 0xC046 , 0xC011 , 
// 0x0150 
0xC016 , 0x202C , 0x0000 , 0xC012 , 0xC045 , 0xC011 , 0xC001 , 0x014A , 0x635F , 0x3864 , 0x0000 , 0xE085 , 0xE0E4 , 0xE0F3 , 0xE104 , 0xE14E , 
// 0x0160 
0xC016 , 0x202C , 0x5259 , 0x4853 , 0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x495F , 0x544E , 0x5245 , 0x5250 , 0x5445 , 
// 0x0170 
0x5245 , 0x435F , 0x4D4F , 0x4950 , 0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x0157 , 0x635F , 0x3878 , 0x0000 , 0xE09C , 0xE0E4 , 
// 0x0180 
0xE0F3 , 0xE127 , 0xE14E , 0xC016 , 0x202C , 0x5259 , 0x4853 , 0x4C45 , 0x5F4C , 0x4944 , 0x5443 , 0x4F49 , 0x414E , 0x5952 , 0x455F , 0x5458 , 
// 0x0190 
0x4E45 , 0x4953 , 0x4E4F , 0x435F , 0x4D4F , 0x4950 , 0x454C , 0x2944 , 0x0D3B , 0x0000 , 0xC012 , 0xC001 , 0x017A , 0x6F63 , 0x706D , 0x6C69 , 
// 0x01A0 
0x4965 , 0x746E , 0x7265 , 0x7270 , 0x7465 , 0x7265 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE15B , 0xC001 , 0x019C , 0x6F63 , 
// 0x01B0 
0x706D , 0x6C69 , 0x4565 , 0x7478 , 0x6E65 , 0x6973 , 0x6E6F , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x0000 , 0xE17E , 0xC001 , 0x01AE , 
// 0x01C0 
0x7073 , 0x6361 , 0x7365 , 0x0000 , 0xC01E , 0xC01D , 0xE1CF , 0xC002 , 0xC009 , 0x0001 , 0xC028 , 0xC01E , 0xC03E , 0xC01D , 0xE1C7 , 0xC022 , 
// 0x01D0 
0xC001 , 0x01BF , 0x6964 , 0x7463 , 0x6E49 , 0x6176 , 0x696C , 0x0064 , 0xC009 , 0xFFFF , 0xC001 , 0x01D1 , 0x775F , 0x326C , 0x0000 , 0xC023 , 
// 0x01E0 
0xC01E , 0xC01D , 0xE1E4 , 0xC003 , 0xC03E , 0xC023 , 0xC023 , 0xC01E , 0xE1D8 , 0xC031 , 0xC001 , 0x01DB , 0x775F , 0x336C , 0x0000 , 0xC049 , 
// 0x01F0 
0xC009 , 0x0028 , 0xC01F , 0xC028 , 0xE1C4 , 0xC001 , 0x01EB , 0x6C77 , 0x0000 , 0xC009 , 0x0000 , 0xC009 , 0x0000 , 0xE1D8 , 0xC048 , 0xC040 , 
// 0x0200 
0xC010 , 0xC01E , 0xC011 , 0xC041 , 0xC04A , 0xC011 , 0xC041 , 0xE1EF , 0xE1DF , 0xC01D , 0xE1FE , 0xC042 , 0xC022 , 0xC003 , 0xC001 , 0x01F6 , 
// 0x0210 
0x695F , 0x666E , 0x0030 , 0xC016 , 0x2020 , 0x2020 , 0x4420 , 0x4349 , 0x4954 , 0x4E4F , 0x5241 , 0x5F59 , 0x4953 , 0x455A , 0x203A , 0x0000 , 
// 0x0220 
0xC012 , 0xC053 , 0xC00C , 0xC003 , 0xC001 , 0x020F , 0x695F , 0x666E , 0x0031 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 
// 0x0230 
0x4150 , 0x5F44 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC054 , 0xC00C , 0xC003 , 0xC001 , 0x0225 , 0x695F , 0x666E , 0x0032 , 0xC016 , 
// 0x0240 
0x2020 , 0x2020 , 0x2020 , 0x4E20 , 0x4D55 , 0x525F , 0x4745 , 0x5349 , 0x4554 , 0x5352 , 0x203A , 0x0000 , 0xC012 , 0xC055 , 0xC00C , 0xC003 , 
// 0x0250 
0xC001 , 0x023B , 0x695F , 0x666E , 0x0033 , 0xC016 , 0x4150 , 0x4152 , 0x454D , 0x4554 , 0x5F52 , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 
// 0x0260 
0x203A , 0x0000 , 0xC012 , 0xC056 , 0xC00C , 0xC003 , 0xC001 , 0x0251 , 0x695F , 0x666E , 0x0034 , 0xC016 , 0x2020 , 0x5220 , 0x5445 , 0x5255 , 
// 0x0270 
0x5F4E , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC057 , 0xC00C , 0xC003 , 0xC001 , 0x0267 , 0x695F , 0x666E , 
// 0x0280 
0x0035 , 0xC016 , 0x2020 , 0x4F43 , 0x504D , 0x4C49 , 0x5F45 , 0x5453 , 0x4341 , 0x5F4B , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC058 , 
// 0x0290 
0xC00C , 0xC003 , 0xC001 , 0x027D , 0x695F , 0x666E , 0x0036 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4E49 , 0x5F51 , 
// 0x02A0 
0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC059 , 0xC00C , 0xC003 , 0xC001 , 0x0293 , 0x695F , 0x666E , 0x0037 , 0xC016 , 0x2020 , 0x2020 , 
// 0x02B0 
0x2020 , 0x2020 , 0x5541 , 0x5F58 , 0x4E49 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05A , 0xC00C , 0xC003 , 0xC001 , 0x02A9 , 
// 0x02C0 
0x695F , 0x666E , 0x0038 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4F20 , 0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 
// 0x02D0 
0xC012 , 0xC05B , 0xC00C , 0xC003 , 0xC001 , 0x02BF , 0x695F , 0x666E , 0x0039 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4120 , 0x5855 , 0x4F5F , 
// 0x02E0 
0x5455 , 0x5F51 , 0x4953 , 0x455A , 0x203A , 0x0000 , 0xC012 , 0xC05C , 0xC00C , 0xC003 , 0xC001 , 0x02D5 , 0x695F , 0x666E , 0x0061 , 0xC016 , 
// 0x02F0 
0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x6853 , 0x6C65 , 0x206C , 0x6953 , 0x657A , 0x203A , 0x0000 , 0xC012 , 0xC051 , 0xC00C , 0xC003 , 
// 0x0300 
0xC001 , 0x02EB , 0x695F , 0x666E , 0x0062 , 0xC016 , 0x6944 , 0x7463 , 0x6F69 , 0x616E , 0x7972 , 0x7520 , 0x6573 , 0x2064 , 0x7266 , 0x6565 , 
// 0x0310 
0x203A , 0x0000 , 0xC012 , 0xC053 , 0xC045 , 0xC01E , 0xC00C , 0xC028 , 0xC00C , 0xC003 , 0xC001 , 0x0301 , 0x6E69 , 0x6F66 , 0x0000 , 0xC052 , 
// 0x0320 
0xC003 , 0xE213 , 0xE229 , 0xE23F , 0xE255 , 0xE26B , 0xE281 , 0xE297 , 0xE2AD , 0xE2C3 , 0xE2D9 , 0xE2EF , 0xE305 , 0xC001 , 0x031B , 0x745F , 
// 0x0330 
0x3073 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x4F43 , 0x4E55 , 0x0054 , 0xC012 , 0xC001 , 0x032E , 0x745F , 0x3173 , 0x0000 , 0xC016 , 
// 0x0340 
0x2020 , 0x2020 , 0x2020 , 0x2020 , 0x4D20 , 0x4E49 , 0x0000 , 0xC012 , 0xC001 , 0x033B , 0x745F , 0x3273 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 
// 0x0350 
0x2020 , 0x2020 , 0x414D , 0x0058 , 0xC012 , 0xC001 , 0x0349 , 0x745F , 0x3373 , 0x0000 , 0xC016 , 0x2020 , 0x2020 , 0x2020 , 0x5641 , 0x5245 , 
// 0x0360 
0x4741 , 0x0045 , 0xC012 , 0xC001 , 0x0356 , 0x745F , 0x3473 , 0x0000 , 0xE332 , 0xE33F , 0xE34D , 0xE35A , 0xC003 , 0xC003 , 0xC001 , 0x0364 , 
// 0x0370 
0x745F , 0x3573 , 0x0000 , 0xC01E , 0xC066 , 0xC01E , 0xC024 , 0xC01D , 0xE37D , 0xC00C , 0xC00C , 0xC00C , 0xC00C , 0xC025 , 0xC01E , 0xC01D , 
// 0x0380 
0xE383 , 0xC040 , 0xC067 , 0xC003 , 0xC001 , 0x036F , 0x7572 , 0x536E , 0x6174 , 0x7374 , 0x0000 , 0xE368 , 0xC009 , 0x0000 , 0xE373 , 0xC01D , 
// 0x0390 
0xE398 , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 0xC01C , 0xE39A , 0xC00B , 0xFFFF , 0xC01D , 0xE38E , 0xC022 , 0xC001 , 0x0385 , 0x6572 , 
// 0x03A0 
0x6573 , 0x5374 , 0x6174 , 0x7374 , 0x0000 , 0xC009 , 0x0000 , 0xC01E , 0xC065 , 0xC01D , 0xE3B2 , 0xC009 , 0x0001 , 0xC027 , 0xC009 , 0x0000 , 
// 0x03B0 
0xC01C , 0xE3B4 , 0xC00B , 0xFFFF , 0xC01D , 0xE3A7 , 0xC022 , 0xC001 , 0x039E , 0x645F , 0x3072 , 0x0000 , 0xC01E , 0xC055 , 0xC02F , 0xC01D , 
// 0x03C0 
0xE3C7 , 0xC01E , 0xC064 , 0xC00C , 0xC009 , 0x0001 , 0xC027 , 0xC001 , 0x03B8 , 0x645F , 0x3172 , 0x0000 , 0xC01E , 0xC00C , 0xC016 , 0x203A , 
// 0x03D0 
0x0000 , 0xC012 , 0xC01E , 0xC064 , 0xC00C , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xE3BC , 0xC003 , 0xC001 , 0x03C8 , 
// 0x03E0 
0x6572 , 0x7367 , 0x0000 , 0xC009 , 0x0000 , 0xE3CC , 0xC01E , 0xC055 , 0xC033 , 0xC01D , 0xE3E5 , 0xC022 , 0xC001 , 

};
CompiledDictionary interpreterCompiledDictionary( compiledDictionaryData, 0x03DF , 0x03ED , YRSHELL_DICTIONARY_INTERPRETER_COMPILED);


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
    { (uint16_t)YRShellInterpreter::SI_CC_v_until,                               "v_until" },

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
    "INVALID",
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
    "YRSHELL_WAIT_DELAY"
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
    m_dictionaryList[ YRSHELL_DICTIONARY_INTERPRETER_COMPILED_INDEX] = &interpreterCompiledDictionary;
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
	m_debugFlags = 0;
	m_hexMode = false;
	m_lastState = YRSHELL_INVALID_STATE;
	m_outputTimeoutInMilliseconds = 1000;
	m_padCount = 0;
    m_padSize = 0;
	m_PC = YRSHELL_DICTIONARY_INVALID;
	m_returnTopOfStack = 0;
	m_saveptr = NULL;
	m_state = YRSHELL_INVALID_STATE;
	m_token = NULL;
	m_topOfStack = 0;
	m_useAuxQueues = false;
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
    reset( getFileName(file), line);
}
void YRShellInterpreter::shellERROR( const char* file, unsigned line, const char* message) {
    reset( getFileName(file), line, message);
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

void YRShellInterpreter::executeFunction( uint16_t n) {
    bool b;
    int32_t i;
    uint32_t v1, v2, v3;
    float f1, f2;
    const char *P;
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
            outString(m_prompt);
            break;
        case SI_CC_lf:
            outChar( '\n');
            break;
        case SI_CC_crlf:
            outChar( '\r');
            outChar( '\n');
            break;
        case SI_CC_reset:
            reset( __FILE__, __LINE__, "COMMAND RESET\r\n");
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
            P = getAddressFromToken( popParameterStack());
            outString( P );
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
            nextState(YRSHELL_WAIT_DELAY);
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
                outString( P);
                pushParameterStack((uint32_t) strlen( P));
            } else {
                pushParameterStack(0);
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
            break;
        case SI_CC_mainIO:
            m_useAuxQueues = false;
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
            outString( shellClass());
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
        		outString( S->sliceName() );
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
                case 'x':
                    c = charToHex(*m_saveptr++) << 4;
                    c |= charToHex(*m_saveptr++);
                    break;
                default:
                    c = '0';
                    break;
            }
        }
        if( c == '\'' || c == '\0' ) {
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
    m_DictionaryCurrent->addToken( SI_CC_jmpz);
    pushCompileStack(0x10000000 | YRSHELL_DICTIONARY_RELATIVE| m_DictionaryCurrent->getWordEnd());
    m_DictionaryCurrent->addToken( 0);
}
void YRShellInterpreter::CC_else( ) {
    uint32_t v = popCompileStack();
    if( (v & 0xF0000000) != 0x10000000) {
        shellERROR( __FILE__, __LINE__, "MISSING [");
    } else {
        m_DictionaryCurrent->addToken( SI_CC_jmp);
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
        m_DictionaryCurrent->setToken( v & YRSHELL_DICTIONARY_ADDRESS_MASK, YRSHELL_DICTIONARY_RELATIVE | m_DictionaryCurrent->getWordEnd());
    }
}
void YRShellInterpreter::CC_begin( ) {
    pushCompileStack(0x20000000 | YRSHELL_DICTIONARY_RELATIVE | m_DictionaryCurrent->getWordEnd());
}
void YRShellInterpreter::CC_until( ) {
    uint32_t v = popCompileStack();
    if( (v & 0xF0000000) != 0x20000000) {
        shellERROR( __FILE__, __LINE__, "MISSING {");
    } else {
        m_DictionaryCurrent->addToken( SI_CC_jmpz);
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
        m_hexMode = false;

        m_useAuxQueues = false;
    
        m_PC = 0;
        m_outputTimeoutInMilliseconds = 1000;
    
        outChar( '\r');
        outString( YRSHELL_VERSION);
    }
}
void YRShellInterpreter::reset( const char* file, unsigned line) {
    reset();
    outString( file);
    outUint32(line);
    outChar( '\r');
   
}
void YRShellInterpreter::reset( const char* file, unsigned line, const char* message) {
    reset();
    outString( file);
    outUint32(line);
    outString( " ");
    outString( message);
    outChar( '\r');
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
    m_lastState = m_state;
    m_state = n;
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
    if( m_padCount > (m_padSize - 2)) {
        reset( __FILE__, __LINE__, "INPUT BUFFER OVERFLOW");
    } else {
        if( c == '\r' || c == '\n') {
            if( m_Pad[0] == '/' && m_Pad[1] == '/') {
                m_padCount = 0;
                m_Pad[ 0] = '\0';
            }
            nextState( YRSHELL_BEGIN_PARSING);
        } else {
            m_Pad[ m_padCount++] = c;
            m_Pad[ m_padCount] = '\0';
        }
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

void YRShellInterpreter::slice(void) {
    char c;
    if( ( (m_Outq->free() < (m_Outq->size()/2)) || (m_AuxOutq->free() < (m_AuxOutq->size()/2)) ) && m_state != YRSHELL_WAIT_FOR_OUTPUT_SPACE) {
        nextState( YRSHELL_WAIT_FOR_OUTPUT_SPACE);
        m_outputTimeout.setInterval(m_outputTimeoutInMilliseconds);
    }
    switch( m_state) {
        case YRSHELL_BEGIN_IDLE:
            outString( m_prompt);
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
                    nextState( YRSHELL_BEGIN_IDLE);
                }
            }
            break;
        case YRSHELL_EXECUTING:
            executing();
            break;
        case YRSHELL_WAIT_FOR_OUTPUT_SPACE:
            if( m_Outq->used() < (m_Outq->size()/2) && m_AuxOutq->used() < (m_AuxOutq->size()/2) ) {
                nextState( m_lastState);
            } else if( m_outputTimeout.hasIntervalElapsed()) {
                shellERROR( __FILE__, __LINE__, "OUTPUT WAIT FOR SPACE TIMEOUT");
            }
            break;
        case YRSHELL_WAIT_DELAY:
            if( m_delayTimer.hasIntervalElapsed()) {
                nextState( m_lastState);
            }
            break;
        default:
            break;
            
    }
    if( m_state == YRSHELL_INRESET) {
        nextState( YRSHELL_BEGIN_IDLE);
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
                    outString( "\rUNDEFINED [");
                    outString( m_token);
                    outString( "]\r");
                    interpretReset();
                    nextState( YRSHELL_BEGIN_IDLE);
                    m_returnTopOfStack = 0;
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
    
    if( index < YRSHELL_DICTIONARY_LAST_INDEX) {
        rc = m_dictionaryList[ index]->getAddress( address);
    } else {
        rc = m_dictionaryList[ (m_PC >> YRSHELL_DICTIONARY_NUMBER_TOKEN_BITS) & YRSHELL_DICTIONARY_LAST_INDEX ]->getAddress( address);
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

void YRShellInterpreter::outChar( const char c) {
    if( m_useAuxQueues) {
        if( !m_AuxOutq->put( c) && m_state != YRSHELL_INRESET) {
            shellERROR( __FILE__, __LINE__, "OUTPUT BUFFER OVERFLOW");
        }
    } else {
        if( !m_Outq->put( c) && m_state != YRSHELL_INRESET) {
            shellERROR( __FILE__, __LINE__, "OUTPUT BUFFER OVERFLOW");
        }
    }
    if( m_expandCR && c == '\x0D') {
        outChar( '\n');
    }
}

void YRShellInterpreter::outString( const char* P) {
    if( P != NULL) {
        while( *P != '\0') {
            outChar( *P++);
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

