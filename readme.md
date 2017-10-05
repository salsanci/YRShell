# Shell

- test --> build
	-
- utility --> src

- fix __APPLE_CC__ **

 –lpthread for linux **

 - fix Makefile **

 - yrshell
 
 /** \page introDocumentation YrShell Introduction to YrShell  

YrShell is an interactive command line shell providing access to the Arduino ARM processor simillar to the way the Linux BASH shell provides access to the OS functionality. YrShell provides scripting capabilities.  YrShell allows us to add commands to the YrShell dictionary. The idea is that YrShell allows us to manipulate the Arduino ARM processor (and any installed programs) from the command line.   

This is a very powerful and dangerous tool. (Muhuhahaha!) On a good day, one of the biggest issues in microcontroller porgramming is one routine stepping on the memory of another routine.  And here comes YrShell which can allow us to touch (through the command line) ANY part of the microcontroller! So to protect ourselves, YrShell is specifically made to NOT allow us to step on other routines' memory UNLESS WE PLAN TO. So by default, we generally can't touch memory used by another routine.  But IF WE DEFINE that we wish to touch another routine, we can. Muhuhahaha! This is power with control.   

YrShell is simillar to, but not the same as, FORTH. That is, the YrShell command line works the same as a FORTH command line.  Under the hood its all different, but we don't care, because we never need to look at that part. (The source code and testing interfaces are provided in case you do want to go there, but then you must be crazy and you're on your own. If this does not discourage you, then maybe you can help with development and testing).
Back to FORTH, find the Leo Brode tutorial HERE:  http://www.forth.org/tutorials.html
End of introduction
 */
