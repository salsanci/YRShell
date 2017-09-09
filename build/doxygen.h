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

: sl { s' UUUU' .str crlf cr? }

: k. { key? [ cr .bx cr -1 ] }

: slp dup pinOut { dup pinHi dup pinLo cr? } pinIn

: pulse dup pinOut dup pinHi 2000 delay dup pinLo pinIn

: p dup pulse pulse

: sla { auxIO  s' UUUU' .str crlf mainIO cr? } 

: slb { auxIO  s' UUUU' .str crlf mainIO s' UUUU' .str crlf cr? } 

: sl1 { auxIO  s' UUUU' .str crlf mainIO s' AAAA' .str crlf cr? } 

: term s' Hit CTL-k to exit\r\n' .str { auxKey? [ emit ][ drop ] key? [ dup 0x0B == [ drop -1 ][ auxEmit 0 ] ] }




57600 setSerial1Baud



*/