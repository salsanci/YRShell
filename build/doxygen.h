/** \mainpage Shell
 
# This project provide an interactive shell on an Ardiuno Arm based system

- \subpage userDocumentation "Shell user documentation"
- \subpage customizingDocumentation "Shell customizing documnetation
- \subpage developerDocumentation "Shell developer documentation"
- \subpage currentStatus "Current Project Status"
- \subpage examples "Examples"
 
 */ 

/** \page userDocumentation Shell user documentation

MarkDown here.

 */

/*! \page customizingDocumentation Shell customizing documnetation

MarkDown here.

 */

/** \page developerDocumentation Shell developer documentation

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