#ifndef HardwareSpecific_h
#define HardwareSpecific_h

#ifdef __linux__
	#define PLATFORM_LA
    #define PLATFORM_LINUX
#endif
#ifdef __APPLE__
	#define PLATFORM_LA
    #define PLATFORM_APPLE
#endif

#ifndef PLATFORM_LA
	#ifndef PLATFORM_AC6
			#define PLATFORM_ARDUINO
	#endif

	#ifdef PLATFORM_AC6
		#include "processorGlobal.h"
	#endif

	#ifdef PLATFORM_ARDUINO
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

#endif
