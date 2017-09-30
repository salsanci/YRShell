/** \mainpage YrShell
 
# This project provides an interactive shell on an Ardiuno Arm based system

- \subpage introDocumentation "YrShell introduction"
- \subpage userDocumentation "YrShell user documentation"
- \subpage customizingDocumentation "YrShell customizing documnetation
- \subpage developerDocumentation "YrShell developer documentation"
- \subpage currentStatus "Current Project Status"
- \subpage examples "Examples"
 
 */ 

/** \page introDocumentation YrShell Introduction to YrShell

YrShell is an interactive command line shell providing access to the Arduino ARM processor simillar to the way the Linux BASH shell provides access to the OS functionality. YrShell provides scripting capabilities.  YrShell allows us to add commands to the YrShell dictionary. The idea is that YrShell allows us to manipulate the Arduino ARM processor (and any installed programs) from the command line. 

This is a very powerful and dangerous tool. (Muhuhahaha!) On a good day, one of the biggest issues in microcontroller porgramming is one routine stepping on the memory of another routine.  And here comes YrShell which can allow us to touch (through the command line) ANY part of the microcontroller! So to protect ourselves, YrShell is specifically made to NOT allow us to step on other routines' memory UNLESS WE PLAN TO. So by default, we generally can't touch memory used by another routine.  But IF WE DEFINE that we wish to touch another routine, we can. Muhuhahaha! This is power with control.  

YrShell is simillar to, but not the same as, FORTH. That is, the YrShell command line works the same as a FORTH command line.  Under the hood its all different, but we don't care, because we never need to look at that part. (The source code and testing interfaces are provided in case you do want to go there, but then you must be crazy and you're on your own. If this does not discourage you, then maybe you can help with development and testing).

Back to FORTH, find the Leo Brode tutorial HERE:  http://www.forth.org/tutorials.html

End of introduction

 */
/** \page userDocumentation YrShell user documentation


YrShell is designed and tested on ST-Microelectronics STM32Nucleo-L476RG micocontroller board. 
This hardware should be more or ess compatible with ANY Arduino board, 
but we will not be sure until user confirm or deny specific boards.
For the purpose of this document we will only discuss one hardware board, the STM32Nucleo-K476RG.

Setup-Hardware

Setup-STM32Nucleo-L476RG

  Remove from package.
  Find the USB mini cable.
  Notice there are header pins exposed on both the top and bottom, take precautions so these do not get bent or shorted.
  I found some stanoffs and mounted it in a cookie tin.
  You might set it on a clean folder shop towel for the time being.
  Aside from physically secring the board, nothing more needs to be done until we are ready to program.

Setup-PC-hardware

  PC Hardware needs at least available USB port to use as virtual serial port.
  Old type RS232 serial port is possible but not covered here until requested.

PC-Operating Systems Supported   

  The YrShell development environment is tested on Mac OSX, Linux Mint 18.2, and Windows 10.
  Other versions of MacOSX, Linux (Ubuntu, debian), and Windows should work as expected.
  Windows XP is possible but not covered here until requested.   

  Plug in USB cable to available USB port on PC.  
  Notice that the USB cable prvides power to the Board.
  Plug other end into Nucleo-L476RG board;
     You should see a directory window open for 
     /media/<user>/NODE_L476RG (linux)
        containing two files, DETAILS.TXT and MBED.HTM
NOTE: Linux Mint displays the directory, LED1 COM is solid RED
NOTE: Windows 10 gives an error, LED1 COM flashes RED

FIRST TIME ONLY:

  The STM32Nucleo-L476RG ships with a demo software image.
  Press Button B1 (the blue one) and observe the blink pattern changes.
  Congratulations! Your board is working correctly.
      

Setup-PC-Software

Setup-Arduino-IDE

  Start with the instructions https://github.com/stm32duino/wiki/wiki/Getting-Started

  STEP 1:

    Download recent Arduino IDE (1.8.4 as of this writing) from:
      Mac OSX used https://www.arduino.cc/download_handler.php?f=/arduino-1.8.4-macosx.zip
      Linux (Ubuntu, Mint) used https://www.arduino.cc/download_handler.php?f=/arduino-1.8.4-linux64.tar.xz
    [Follow the istruction until you the Arduino IDE icon on your desktop]
    Extract the contents of the archive (I did "extract here" in Downloads)
    Find Install.sh (linux)  Execute this script, it puts the icon on the desktop,
          but does not yet put an Arduino directory in your home folder
    

  STEP 2:

    The second step of the instructions we are following take us to the Board Manager
      https://github.com/stm32duino/wiki/wiki/Boards-Manager
      The board manager adds definitions for the STM32L476RG board we will be using.
    Start the Arduino IDE
      Arduino IDE > File > Preferences > Settings > Additional Boards Manager URL: 
        https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json
     Tools > Board Manager > Select "STM32 Cores" from the bottom of the list
     OK
     Close

   Step 3: 

      Select the Nucleo-64 L476RG:
        Arduino IDE > Tools > Board: > Nucleo-64 (at the bottom of the list)
        Arduino IDE > Tools > Board number: Nucleo-L476RRG
        Arduino IDE > Tools > Upload Method: Mass Storage (STLink gives an error)
        Arduino IDE > Tools > USB Interface: None (default)
        Arduino IDE > Tools > Other Serial instance: All (default)
        Arduino IDE > Tools > Port: 
                                    /dev/ttyACM0 (on Linux) 
                                    /dev/ttyzzz (on MACOS) 
                                    COM1 (on Windows)
If PORT is greyed out, plug in the USB cable


AT THIS POINT, CHECK GROUPS FOR DIALOUT (FOR OSX and Linux)
  In linux, we need to grant permission for the user to access the communications port(s).
  https://askubuntu.com/questions/207999/how-to-add-dialup-group
  enter this command at the linux command prompt:

  sudo adduser $USER dialout

You must logout and log in again for this to take affect.

**********************************************************
  

TEST: Verify the Arduino IDE is correctly configured for the STM32-L476RG
   (at the bottom of the Getting-Started page, folllow the link  to the Blink Example)
   https://github.com/stm32duino/wiki/wiki/Blink-example
   Verify the blink and button behavior (different blink, button has no effect)

At this point, the general Arduino IDE is setup for the STM32Nucleo-L476RG.
The IDE has been verified as working by successful compilaction and load of an example program.
Now we procedue to the YrShell module.

=========================================================

Software Setup - YrShell

Setup-YrShell
   Download Fshell from GitHub  https://github.com/salsanci/YRShell
   copy the directory 
to 
   /home/<yourusername>/Arduino/libraries/
   The result should look like this:
   /home/<yourusername>/Arduino/libraries/YRShell

The YRShell directory should contain several files and subdirectories, such as:
   /home/<yourusername>/Arduino/libraries/YRShell/examples
   /home/<yourusername>/Arduino/libraries/YRShell/utility


Plug in the USB cable to the Nucleo-L476 board if it is not already.
Start the ARduino IDE
  Sketch > Include Library > YRShell (way at the bottom)
Navigate to the Fshell arduino sketch:
  File > OPen > /home/<yourusername>/Arduino/libraries/YRShell/examples/multipleShells/ multipleShells.ino

The Arduino IDE should display the Fshell sketch on your screen if it is set upcorrectly.
Notice line 129 - Serial.begin(115200);

Verify and upload the sketch to the Nucleo-L476 using the icons at the top of the arduino IDE.
  The LD1 COMM LED on by the USB jack should flash from green to red a couple times. 
  The green LED LD2 should flash ON for about a half second and OFF for about a second and a half.
  Notice Lines 143 and 146.  You can very the blink pattern by changing these intervals.
  Notice that you must upload after each edit for the change to affect the blink pattern.
  If you can change the blink pattern for example 10 milliseconds on and 1500 millseconds off, you are set up correctly.

Test the Fshell Command line.
  In the arduino IDE select
  Tools > Serial Monitor
  At the bootom of the window, select:
     Carriage Return
     115200 baud

At the serial monitor input field (at the top of the window) type in a couple numbers separated by spaces, and hit enter.

in the input field, type in 
st?
and hit enter. 
Type in 
wl
and hit enter

YRShell is now set up and ready for action.

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
