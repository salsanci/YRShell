/** \mainpage YrShell
 
# This project provides an interactive shell on an Ardiuno Arm based system

- \subpage introDocumentation "YrShell introduction"
- \subpage userDocumentation "YrShell user documentation"
- \subpage customizingDocumentation "YrShell customizing documnetation
- \subpage developerDocumentation "YrShell developer documentation"
- \subpage currentStatus "Current Project Status"
- \subpage examples "Examples" 
- \subpage gitnotes "Notes on using GIT"
 
 */ 

/** \page introDocumentation YrShell Introduction to YrShell

YrShell is an interactive command line shell providing access to the Arduino ARM processor.
The result is similar to the way the Linux BASH shell provides access to OS functionality. 
YrShell allows us to manipulate the Arduino ARM processor (and any installed programs) from the command line.  

YrShell is simillar to, but not the same as, FORTH. That is, the YrShell command line works the same as a FORTH command line.  Under the hood its all different, but we don't care, because we never need to look at that part. (The source code and testing interfaces are provided in case you do want to go there, but then you must be crazy and you're on your own. If this does not discourage you, then maybe you can help with development and testing).

Back to FORTH, find the Leo Brode tutorial HERE:  http://www.forth.org/tutorials.html

End of introduction

 */
/** \page userDocumentation YrShell user documentation

The initial setup instructions for the PC software tool chain and installing the YRShell firmware are in the [ReadMe.md] (https://github.com/salsanci/YRShell/blob/doc/readme.md) for this Github project.

After YRShell is installed using the instructions in ReadMe.md,  continue with these instructions here.

# Compile YRShell for the PC command line 

YRShell is written in the C programming languge.  This means the code can be made to run on any suitable processor that hasa C compiler.  The PC also has a C compiler.  

The same YRShell code that runs on the Nucleo-L476RG and Ardunio boards (Due, Mega2560) can also run on the PC.

## To build YRShell for the PC command line:

Move to the YRShell directoy:  
`cd /YRShell/build`  

Compile and build the YRShell executable:  
`make`  

After the complier finishes, the YRShell executable should be in the directory.

Execute that YRShell program from the command line:

`./yrshell`

You should see the same YRShell command prompt as was displayed on the arduiino IDE serial monitor.

Try various commands such as word list `wl`  stack display `st?` interger math functions `+` `-` `*` `/`

# Built-in IEEE floating point math

Try the floating point math functions such as:

`5.432 3.217 f+ .f`

Here's something that is a little bit interesting, and reveals how floating point is done on the command line.
Paste the following into the YRShell command line:  

`4.444 1.111 st? f- st? .f`

Notice that is you enter float numbers and pop them off the stack using the interger stack pop `.` that the float number is display as an interger in IEEE float format.
The converse is also true.  This means we have to take care if we wish to manipulate both float and interger number on the same stack.

At this writing, there is no command word to perform mixed math (float with interger).

# Compiler stack

In addition to the usual parameter stack and the usual return stack, YRShell has a third stack, the compiler stack.  
At compile time, this stack is used internally.  At run time this stack is available for the user.  
Now we can use another stack, without having to worry about the crossing definition boundraies as we would with the return stak. 

The >c word take the value on the top of the parameter stack, and puts it on the compiler stack.

The c> work takes the top value off the compiler stack, and puts it on the parameter stack.

# Build the example Test script

Notice that the `make command builds the YRShell command line shell on the pc, and the test script(s).

[continue here]

End of user documentation

 */

/*! \page customizingDocumentation YrShell customizing documnetation

MarkDown here.

 */

/** \page developerDocumentation YrShell developer documentation

MarkDown here.

 */

/** \page currentStatus Current Project Status


- 20170905 - docker set up, running and testes on Due & L476, osx and linux

*/

/** \page examples

: cr? key? [ 0x0D == ] 

: esc? key? [ 0x1B == ] 

: ck? key? [ 0x0B == ] 

: k. { key? [ cr .bx cr -1 ] }

: term s' Hit CTL-k to exit' .str crlf { auxKey? [ emit ][ drop ] key? [ dup 0x0B == [ drop -1 ][ auxEmit 0 ] ] }


: tlpin 10

: tlsynch tlpin pinOut tlpin pinHi tlpin pinLo tlpin pinHi

: at auxIO  s' AT' .str mainIO 1000 delay  { auxKey? [ emit 0 ][ drop -1 ] } crlf

: ath auxIO s' AT+HELP' .str crlf mainIO 1000 delay { auxKey? [ emit 0 ][ drop -1 ] } crlf


: ssb setSerial1Baud

: wuls s' 12345678901234567890123456789012345678901234567890' .str

: wu auxIO wuls wuls mainIO 1000 delay { auxKey? [ emit 0 ][ drop -1 ] } crlf

: hmt1 dup auxEmit 0x0D == [ 0x0A auxEmit ]

: hmterm s' Hit CTL-k to exit' .str crlf 1 setTerm



: hc05Send auxIO .str crlf mainIO 1000 delay { auxKey? [ emit 0 ][ drop -1 ] } crlf

: hc05_1 s' AT+VERSION' hc05Send

: hc05_2 s' AT+ROLE?' hc05Send

: hc05_3 s' AT+UART?' hc05Send

: hc05_4 s' AT+NAME?' hc05Send

: hc05_5 s' AT+PSWD?' hc05Send

: hc05_6 s' AT+PSWD=1111' hc05Send s' AT+UART=230400,0,0' hc05Send

: hc05q hc05_1 hc05_2 hc05_3 hc05_4 hc05_5

: hca 38400 ssb hc05q s' AT+NAME=SanciHc05-A' hc05Send  hc05_6 hc05q
: hcb 38400 ssb hc05q s' AT+NAME=SanciHc05-B' hc05Send  hc05_6 hc05q
: hcc 38400 ssb hc05q s' AT+NAME=SanciHc05-C' hc05Send  hc05_6 hc05q
: hcd 38400 ssb hc05q s' AT+NAME=SanciHc05-D' hc05Send  hc05_6 hc05q
: hce 38400 ssb hc05q s' AT+NAME=SanciHc05-E' hc05Send  hc05_6 hc05q
: hcf 38400 ssb hc05q s' AT+NAME=SanciHc05-F' hc05Send  hc05_6 hc05q
: hcg 38400 ssb hc05q s' AT+NAME=SanciHc05-G' hc05Send  hc05_6 hc05q
: hch 38400 ssb hc05q s' AT+NAME=SanciHc05-H' hc05Send  hc05_6 hc05q
: hci 38400 ssb hc05q s' AT+NAME=SanciHc05-I' hc05Send  hc05_6 hc05q
: hcj 38400 ssb hc05q s' AT+NAME=SanciHc05-J' hc05Send  hc05_6 hc05q



: ble_2 s' AT+ROLE' hc05Send

: ble_3 s' AT+BAUD' hc05Send

: ble_4 s' AT+NAME' hc05Send

: ble_5 s' AT+PASS' hc05Send

: ble_6 s' AT+PASS112358' hc05Send 
: ble_7 s' AT+BAUD9' hc05Send


: bleq hc05_1 ble_2 ble_3 ble_4 ble_5

: ble1 9600 ssb bleq  s' AT+NAMESanciBLE01' hc05Send ble_6 ble_7
: ble2 9600 ssb bleq  s' AT+NAMESanciBLE02' hc05Send ble_6 ble_7
: ble3 9600 ssb bleq  s' AT+NAMESanciBLE03' hc05Send ble_6 ble_7
: ble4 9600 ssb bleq  s' AT+NAMESanciBLE04' hc05Send ble_6 ble_7
: ble5 9600 ssb bleq  s' AT+NAMESanciBLE05' hc05Send ble_6 ble_7
: ble6 9600 ssb bleq  s' AT+NAMESanciBLE06' hc05Send ble_6 ble_7
: ble7 9600 ssb bleq  s' AT+NAMESanciBLE07' hc05Send ble_6 ble_7
: ble8 9600 ssb bleq  s' AT+NAMESanciBLE08' hc05Send ble_6 ble_7
: ble9 9600 ssb bleq  s' AT+NAMESanciBLE09' hc05Send ble_6 ble_7

: b? 230400 ssb bleq



0x44 serial2Write 100 delay serial3Read .
0x45 serial3Write 100 delay serial2Read .





\ for the HC06

\ green wire
27 wconstant hcProgPin

\ white/orange wire
26 wconstant hcRx

\ white/green wire
25 wconstant hcTx

9600 4/ wconstant hcBaud

0 wconstant hcSerialCog

hcSerialCog cogreset 100 delms
c" hcRx hcTx hcBaud serial" hcSerialCog cogx 100 delms


: hcSend
	cogid hcSerialCog iolink .cstr 1000 delms cogid iounlink cr
;

hcProgPin pinhi hcProgPin pinout

c" AT" hcSend
c" AT+VERSION" hcSend
c" AT+NAMESanciHC06-1" hcSend
c" AT+PIN1111" hcSend
c" AT+PN" hcSend
c" AT+BAUD9" hcSend

hcProgPin pinlo 

1000 delms

hcSerialCog cogreset 100 delms
230400 4/ wconstant hcBaud
c" hcRx hcTx hcBaud serial" hcSerialCog cogx 100 delms

hcProgPin pinhi hcProgPin pinout

c" AT" hcSend
c" AT+VERSION" hcSend
c" AT+NAMESanciHC06-2" hcSend
c" AT+PIN1111" hcSend
c" AT+PN" hcSend
\ c" AT+BAUD4" hcSend \ sets back to 9600 baud

hcProgPin pinlo



fl

\ for the HC05

\ green wire
27 wconstant hcProgPin

\ white/orange wire
26 wconstant hcRx

\ white/green wire
25 wconstant hcTx

9600 4/ wconstant hcBaud

0 wconstant hcSerialCog

hcSerialCog cogreset 100 delms
c" hcRx hcTx hcBaud serial" hcSerialCog cogx 100 delms
1 hcSerialCog sersetflags


: hcSend
	cogid hcSerialCog iolink .cstr 1000 delms cogid iounlink cr
;

hcProgPin pinhi hcProgPin pinout

c" AT+VERSION~h0D~h0A" hcSend
c" AT+ROLE?~h0D~h0A" hcSend
c" AT+UART?~h0D~h0A" hcSend
c" AT+NAME?~h0D~h0A" hcSend
c" AT+PSWD?~h0D~h0A" hcSend
c" AT+NAME=SanciHc05-1~h0D~h0A" hcSend
c" AT+PSWD=1111~h0D~h0A" hcSend
c" AT+UART=230400,0,0~h0D~h0A" hcSend
c" AT+VERSION~h0D~h0A" hcSend
c" AT+ROLE?~h0D~h0A" hcSend
c" AT+UART?~h0D~h0A" hcSend
c" AT+NAME?~h0D~h0A" hcSend
c" AT+PSWD?~h0D~h0A" hcSend

hcProgPin pinlo 

\ after the hc05 is power cycled, the baud rate will change, not like the hc06

********************************************************************
* Command             Description			           *
* ---------------------------------------------------------------- *
* AT                  Check if the command terminal work normally  *
* AT+RESET            Software reboot				   *
* AT+VERSION          Get firmware, bluetooth, HCI and LMP version *
* AT+HELP             List all the commands		           *
* AT+NAME             Get/Set local device name                    *
* AT+PIN              Get/Set pin code for pairing                 *
* AT+PASS             Get/Set pin code for pairing                 *
* AT+BAUD             Get/Set baud rate		                   *
* AT+LADDR            Get local bluetooth address		   *
* AT+ADDR             Get local bluetooth address		   *
* AT+DEFAULT          Restore factory default			   *
* AT+RENEW            Restore factory default			   *
* AT+STATE            Get current state				   *
* AT+PWRM             Get/Set power on mode(low power) 		   *
* AT+POWE             Get/Set RF transmit power 		   *
* AT+SLEEP            Sleep mode 		                   *
* AT+ROLE             Get/Set current role.	                   *
* AT+PARI             Get/Set UART parity bit.                     *
* AT+STOP             Get/Set UART stop bit.                       *
* AT+START            System start working.			   *
* AT+IMME             System wait for command when power on.	   *
* AT+IBEA             Switch iBeacon mode.	                   *
* AT+IBE0             Set iBeacon UUID 0.            	           *
* AT+IBE1             Set iBeacon UUID 1.            	           *
* AT+IBE2             Set iBeacon UUID 2.            	           *
* AT+IBE3             Set iBeacon UUID 3.            	           *
* AT+MARJ             Set iBeacon MARJ .            	           *
* AT+MINO             Set iBeacon MINO .            	           *
* AT+MEA              Set iBeacon MEA .            	           *
* AT+NOTI             Notify connection event .                    *
* AT+UUID             Get/Set system SERVER_UUID .            	   *
* AT+CHAR             Get/Set system CHAR_UUID .            	   *
* -----------------------------------------------------------------*
* Note: (M) = The command support slave mode only. 		   *
* For more information, please visit http://www.bolutek.com        *
* Copyright@2013 www.bolutek.com. All rights reserved.		   *
********************************************************************



*/

/** \page gitnotes

Notes for commonly used git commands

To set fresh with the project (you have no YRShell directory on your PC and want a fresh one)

git clone https://github.com/salsanci/YRShell.git

Move to the YRShell directory (created by git)

`cd YRShell`

To see the current branch

`git branch`

To create a new branch (to keep your changes separate from master)

`git checkout -b` `[name_of_your_new_branch]`

To change to a different branch (one that already exists in the project)

`git checkout doc`

To update your local PC with this week's changes (from the development team on Sunday afternoon)

  - Get Sal's changes to MASTER
  
`git pull master`

  - get Braino's documentation changes to DOC  
  
`git pull doc`

  - push changes up to the doc branch
  
  `git pull origin doc`  (pulls anybody else's changes to me)
  `git push origin doc`  (pushes my changes out for everybody else)
  
  Sal merges our changes (if he accept them) into MASTER

End git notes

*/
