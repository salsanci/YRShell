This "doc" folder was added to YRshell to align with the doc folder in YVshell

## Command Line Monitor Shell

YRshell and YVshell are both a command line interface monitor shell for micro controller.  YRshell is the impementation on Arduino comaptible microcontrolers.  YVshell is the same shell (or a very similar shell) on a verilog definition of a minimal CPU (to be programend into FPGA).  YRshell and YVshell should provide the same user experience. 

A command line interface provides similar fucntioality to xterm on linux, or powershell on Windows. We can type in characters at the prompt to run diagnostics or execute simple functions.  Scripting allow string together many functions tofor a powerful result. 

YRshell (and YVshell) also support defining new fucntion of the fly. The command line interpreter supports definiting new functions, which are COMPILED into the shell. This means these execute with about the same speed as any other assembler program. the intent is to support very fast deterministic program execution. 

