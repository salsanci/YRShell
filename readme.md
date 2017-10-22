# YrShell


- test --> build
	-
- utility --> src

- fix __APPLE_CC__ **

 –lpthread for linux **  

 - fix Makefile **  

 - yrshell  
 
YrShell : An interactive command line shell for arduino arm based systems

YrShell is an interactive command line shell providing access to the Arduino ARM processor simillar to the way the Linux BASH shell provides access to the OS functionality. YrShell provides scripting capabilities.  YrShell allows us to add commands to the YrShell dictionary. The idea is that YrShell allows us to manipulate the Arduino ARM processor (and any installed programs) from the command line.   

This is a very powerful and dangerous tool. (Muhuhahaha!) On a good day, one of the biggest issues in microcontroller porgramming is one routine stepping on the memory of another routine.  And here comes YrShell which can allow us to touch (through the command line) ANY part of the microcontroller! So to protect ourselves, YrShell is specifically made to NOT allow us to step on other routines' memory UNLESS WE PLAN TO. So by default, we generally can't touch memory used by another routine.  But IF WE DEFINE that we wish to touch another routine, we can. Muhuhahaha! This is power with control.   

YrShell is simillar to, but not the same as, FORTH. That is, the YrShell command line works the same as a FORTH command line.  Under the hood its all different, but we don't care, because we never need to look at that part. (The source code and testing interfaces are provided in case you do want to go there, but then you must be crazy and you're on your own. If this does not discourage you, then maybe you can help with development and testing).
Back to FORTH, find the Leo Brode tutorial HERE:  http://www.forth.org/tutorials.html  
 

# Setup-Hardware
YrShell is designed and tested on ST-Microelectronics STM32Nucleo-L476RG micocontroller board. This hardware should be more or ess compatible with ANY Arduino board, but we will not be sure until user confirm or deny specific boards. For the purpose of this document we will only discuss one hardware board, the STM32Nucleo-L476RG.  
Note: As of this writing, YrShell has been brought up on:
- STM32Nucleo-L476RG
- Arduino Due (ARM Cortex-M3)
- Arduino Mega 2560 (ATMEGA2560 8 bit controler. It runs, its just slower) 

## Setup micro controller: STM32Nucleo-L476RG  
  1. Remove from package.  
  1.   Find the USB mini cable.  
  1. Notice there are header pins exposed on both the top and bottom, take precautions so these do not get bent or shorted.  
  - I found some stanoffs and mounted it in a cookie tin.  
  - You might simply set it on a clean shop towel for the time being.  
  - Aside from physically securing the board, nothing more needs to be done until we are ready to program.  
  
## Setup: PC hardware  
PC Hardware needs at least available USB port to use as virtual serial port.  
Old type RS232 serial port is possible but not covered here until requested.  

### PC Operating Systems Supported    
The YrShell development environment is tested on Mac OSX, Linux Mint 18.2, and Windows 10. 
Other versions of MacOSX, Linux (Ubuntu, debian), and Windows should work as expected. 
Windows XP is possible but not covered here until requested.   
1. Plug in USB cable to available USB port on PC.   
   - Notice that the USB cable prvides power to the Board.
1. Plug other end into Nucleo-L476RG board    
   - You should see a directory window open for   
   `/media/<user>/NODE_L476RG`   (linux)  
   containing two files, DETAILS.TXT and MBED.HTM  
1. Windows 10: the directory is displayed in File Explorer, but does not automatically open in a window.
   
NOTE: Linux Mint displays the directory, LED1 COM is solid RED  
NOTE: Windows 10 gives an error, LED1 COM flashes RED  

FIRST TIME ONLY:  
  The STM32Nucleo-L476RG ships with a demo software image.  
  Press Button B1 (the blue one) and observe the blink pattern changes.  
  Congratulations! Your board is working correctly.  
      
# Setup-PC-Software  

There are two parts to the PC software.  These are the general PC build tools, and the arduino IDE.  

## Setup PC tools  

The build tools are the (gnu) build-essential (compiler) build tools and the doxygen documentation tools.  

### PC Tools: build essential  

We need the build tools for compilation and and testing.  Your installation may or may not need these tools for the Arduino IDE. If you wish to perform automatic testing on the PC side, you will likely need the build tools.  

#### For Linux and OSX:  

`sudo apt-get update`  
`sudo apt-get install build-essential`  

#### For Windows: 

There are a couple alternatives for compiling YRShell.cpp for the Windows coomand line.  One is Cygwin, another is WSL, the windows Subsystem for Linux (bash for Windows).  

__We need to figure how we want to handle this__


### PC Tools: doxygen

Sal is using doxygen (document generator tool) to generate the documentation for the YrShell source code.  If you want to use the same pro-tools as Sal, and/or wish to contribute to this project (optional) you can download and install from the author at the following link.  

http://www.stack.nl/~dimitri/doxygen/download.html  

NOTE: you are not required to install or use doxygen, to use YRShell.  If you plan to look at or modify the YRShell,cpp source code, doxygen may be handy. The install instructions the development team uses are included here for completeness.  

## Setup-Arduino-IDE  
Start with the instructions https://github.com/stm32duino/wiki/wiki/Getting-Started  

STEP 1:   
Download recent Arduino IDE (1.8.4 as of this writing):

- Choose the link for your operation system
   - Mac OSX used  https://www.arduino.cc/download_handler.php?f=/arduino-1.8.4-macosx.zip  
   - Linux (Ubuntu, Mint) used  https://www.arduino.cc/download_handler.php?f=/arduino-1.8.4-linux64.tar.xz  
Follow the istruction until you the Arduino IDE icon on your desktop.  Hints:
  - Extract the contents of the archive (I did "extract here" in Downloads)  
  - Find Install.sh (linux)  Execute this script, it puts the icon on the desktop,  but does not yet put an Arduino directory in your home folder  
    
STEP 2:   
The second step of the instructions we are following take us to the Board Manager  
https://github.com/stm32duino/wiki/wiki/Boards-Manager  
The board manager adds definitions for the STM32L476RG board we will be using.  Follow the numbered instructions all the way to the bottom of the page to just before the "troubleshooting" section.  Summary of the main actions:
- Start the Arduino IDE  
      Arduino IDE > File > Preferences > Settings > Additional Boards Manager URL:  
        https://github.com/stm32duino/BoardManagerFiles/raw/master/STM32/package_stm_index.json  
     `Tools > Board Manager > Select "STM32 Cores" ` from the bottom of the list  
     OK  
     Close    
     
     Notice that the first time we launch the Arduino IDE, it creates an Arduino directory folder for our program source code.  
     OSX/Linux: `~/Arduino`  
     Windows 10: `~/Documents/Arudino`  
     The auto-created Arduino directory will be important later when we add the YRShell source code.

STEP 3:  
Select the Nucleo-64 L476RG:
- Arduino IDE > Tools > Board: > Nucleo-64 (at the bottom of the list)
- Arduino IDE > Tools > Board number: Nucleo-L476RRG
- Arduino IDE > Tools > Upload Method: Mass Storage (STLink gives an error)
- Arduino IDE > Tools > USB Interface: None (default)
- Arduino IDE > Tools > Other Serial instance: All (default)
- Arduino IDE > Tools > Port:  
` /dev/ttyACM0` (on Linux)  
`/dev/ttyzzz` (on MACOS)  
`COMn` (on Windows) 

If PORT is greyed out, plug in the USB cable  
AT THIS POINT, CHECK GROUPS FOR DIALOUT (FOR OSX and Linux)  
  In linux, we need to grant permission for the user to access the communications port(s).  
  https://askubuntu.com/questions/207999/how-to-add-dialup-group  
  enter this command at the linux command prompt:  
  `sudo adduser $USER dialout`  
You must logout and log in again for this to take affect.   

***
# Test to confirm Arduino IDE is setup correctly 
Verify the Arduino IDE is correctly configured for the STM32-L476RG.  Use the following instructions to compile and load the generic Arudino Blinky LED routine.  Demonstrate that the Blinky rate of the LED can be changed by loading new software.  
 - At the bottom of the Getting-Started page, folllow the link  to the Blink Example  
   https://github.com/stm32duino/wiki/wiki/Blink-example  
 - Verify the blink and button behavior (different blink, button has no effect).  The blink on and blink off time are in milliseconds.   Change the values to long on time (e.g. 1000) and short off time (e.g. 50); compile and load; see that the flashes are as expects.  Switch the short and long times, and verify the blink behavior changes as expected.  
 
At this point, the general Arduino IDE is setup for the STM32Nucleo-L476RG. The IDE has been verified as working by successful compilaction and load of an example program.  

***
Now we procede to the YrShell module.  
***
# Software Setup - YrShell
**Here we finally get to loading the YrShell firmware on to the Nucleo-L476RG**

## Setup-YrShell
Clone or Download Fshell from GitHub  `https://github.com/salsanci/YRShell`  
- copy the directory  
    - Linux/OSX `/home/<yourusername>/YrShell`
    - Windows 10 `c:/YrShell`
to  
   `/home/<yourusername>/Arduino/libraries/`  
The result should look like this:  
   `/home/<yourusername>/Arduino/libraries/YRShell`
   
   Notice that the directory folder `Arduino` was created automaticaly buy the Arduino IDE when it started up the first time.
   
The YRShell directory should contain several files and subdirectories, such as:  
   `/home/<yourusername>/Arduino/libraries/YRShell/examples`  
   `/home/<yourusername>/Arduino/libraries/YRShell/utility`  
   
Plug in the USB cable to the Nucleo-L476 board if it is not already.  
- Start the Arduino IDE  
  `Sketch > Include Library > YRShell` (way at the bottom)
- Navigate to the Fshell arduino sketch:  
  `File > OPen > /home/<yourusername>/Arduino/libraries/YRShell/examples/multipleShells/multipleShells.ino`  
  
The Arduino IDE should display the Fshell sketch on your screen if it is set upcorrectly.  

__Notice line 129 - Serial.begin(115200);__  

Verify and upload the sketch to the Nucleo-L476 using the icons at the top of the arduino IDE.  
  - The LD1 COMM LED on by the USB jack should flash from green to red a couple times. 
  - The green LED LD2 should flash ON for about a half second and OFF for about a second and a half.
  - Notice Lines 182 and 185 in YRShell/multipleShells.ino.  You can very the blink pattern by changing these intervals.
  - Notice that you must upload after each edit for the change to affect the blink pattern.
  - If you can change the blink pattern for example 10 milliseconds on and 1500 millseconds off, you are set up correctly.  
  
Test the Fshell Command line.  
  - In the arduino IDE select  
  `Tools > Serial Monitor`  
  - At the bootom of the window, select:  
     `Carriage Return`  
     `115200 baud`  

At the serial monitor input field (at the top of the window) type in a couple numbers separated by spaces, and hit enter.   

in the input field, type in  
 `st?`  
and hit enter.   

**You should see the current stack items displayed.**`/  Use the period **.** followed by the enter key to take items off the stack, and type numbers separated by spaces and terminated with the enter key to put items on the stack.  Notice how you can cause stack underflow messages when you remove more items than are  present.

Type in  
`wl`  
and hit enter  

**You should see a display off all currently defined command words.**  Try the various commands such as addition `+` subtraction `-` multipliplication `*` and divdision `/` to get a feel for how they function.  Try more commands until something blows up.  Hit the reset button to start fresh.  Notice that not all command have a meanings out of context. Avoid executing commands out of context as they often cause undefined behavior.

YRShell is now set up and ready for action.  


# End of user documentation for initial setup    

# Full YRShell source code documentation is generated using DOXYGEN

The documentation beyond inital setup will be generated using `make doxygen` in the build directory. The generated doxygen documentation will be found in the created directory `~/build/html/index.html`  

To Generate the documentation switch to the build directory `cd ~/YRShell/build`  
run the MAKE utility for doxygen: `make doxygen`  
Doxygen will create a directory `html`  
Switch to the html directory:

`cd ~/YRShell/build/html`  

find the file `index.html`

Open index.html in your favorite browser to find the generated documentation for the YRShell source code.

#END
