#ifndef HardwareSpecific_h
#define HardwareSpecific_h

//#define COMPILE_DEBUG

#ifdef __unix__
	#define PLATFORM_LUA
    #define PLATFORM_UNIX
	#ifdef COMPILE_DEBUG
		#warning "COMPILEDEBUG 0: __unix__ DEFINED"
	#endif
#endif
#ifdef __linux__
	#define PLATFORM_LUA
    #define PLATFORM_LINUX
	#ifdef COMPILE_DEBUG
		#warning "COMPILEDEBUG 1: __linux__ DEFINED"
	#endif
#endif
#ifdef __APPLE__
	#define PLATFORM_LUA
    #define PLATFORM_APPLE
	#ifdef COMPILE_DEBUG
		#warning "COMPILEDEBUG 2: __APPLE__ DEFINED"
	#endif
#endif

#ifdef __WIN32
	#define PLATFORM_WINDOWS
	#define PLATFORM_LUA
 	#ifdef COMPILE_DEBUG
		#warning "COMPILEDEBUG 3: __WIN32 DEFINED"
	#endif
#endif

#ifndef PLATFORM_LUA
	#ifdef COMPILE_DEBUG
		#warning "COMPILEDEBUG 3: PLATFORM_LA NOT DEFINED"
	#endif
	#ifndef PLATFORM_AC6
		#ifdef COMPILE_DEBUG
			#warning "COMPILEDEBUG 4: PLATFORM_AC6 NOT DEFINED"
		#endif
		#define PLATFORM_ARDUINO
	#endif

	#ifdef PLATFORM_AC6
		#ifdef COMPILE_DEBUG
			#warning "COMPILEDEBUG 5: PLATFORM_AC6 DEFINED"
		#endif
		#include "processorGlobal.h"
	#endif

	#ifdef PLATFORM_ARDUINO
		#ifdef COMPILE_DEBUG
			#warning "COMPILEDEBUG 6: PLATFORM_ARDUINO DEFINED"
		#endif
		#ifdef ARDUINO_SAM_DUE
			#define ENABLE_SERIAL1
			#define ENABLE_SERIAL2
			#define ENABLE_SERIAL3
		#endif
		#ifdef ARDUINO_AVR_MEGA2560
			#define ENABLE_SERIAL1
			#define ENABLE_SERIAL2
			#define ENABLE_SERIAL3
		#endif
		#include "Arduino.h"
	#endif
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SERIAL_FLOW_CONTROL_NONE 0
#define SERIAL_FLOW_CONTROL_XON_XOFF 1
#define SERIAL_FLOW_CONTROL_RTS_CTS 2

void HW_setSerialBaud( uint32_t port, uint32_t baud);
void HW_setSerialFlowControl( uint32_t port, uint32_t control);

uint32_t HW_getSysticks( void);
uint32_t HW_getMicros( void);
uint32_t HW_getMillis( void);

uint32_t HW_getSysTicksPerSecond( void);

#endif
