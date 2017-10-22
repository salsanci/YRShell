#ifndef HardwareSpecific_h
#define HardwareSpecific_h

#ifdef __linux__
	#define PLATFORM_LA
    #define PLATFORM_LINUX
	#warning "COMPILEDEBUG 1: __linux__ DEFINED"
#endif
#ifdef __APPLE__
	#define PLATFORM_LA
    #define PLATFORM_APPLE
	#warning "COMPILEDEBUG 2: __APPLE__ DEFINED"
#endif

#ifndef PLATFORM_LA
	#warning "COMPILEDEBUG 3: PLATFORM_LA NOT DEFINED"
	#ifndef PLATFORM_AC6
	#warning "COMPILEDEBUG 4: PLATFORM_AC6 NOT DEFINED"
		#define PLATFORM_ARDUINO
	#endif

	#ifdef PLATFORM_AC6
		#warning "COMPILEDEBUG 5: PLATFORM_AC6 DEFINED"
		#include "processorGlobal.h"
	#endif

	#ifdef PLATFORM_ARDUINO
		#warning "COMPILEDEBUG 6: PLATFORM_ARDUINO DEFINED"
		#include "Arduino.h"
	#endif
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void HW_setSerialBaud( uint32_t port, uint32_t baud);

uint32_t HW_getSysticks( void);
uint32_t HW_getMicros( void);
uint32_t HW_getMillis( void);

uint32_t HW_getSysTicksPerSecond( void);

#endif
