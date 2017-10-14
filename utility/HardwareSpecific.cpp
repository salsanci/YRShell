#include "HardwareSpecific.h"

#ifdef PLATFORM_AC6
#include "STMlib2/STMSerial.h"
#endif

#ifdef PLATFORM_ARDUINO
#include "BufferedSerial.h"
#endif

void HW_setSerialBaud( uint32_t port, uint32_t baud) {
	#ifdef PLATFORM_AC6
            	STMSerial::setUartBaudRate( port, baud);
#endif
#ifdef PLATFORM_ARDUINO
				switch( port) {
#ifdef ENABLE_SERIAL
					case 0:
						BSerial.setBaud( baud);
					break;
#endif
#ifdef ENABLE_SERIAL1
					case 1:
						BSerial1.setBaud( baud);
					break;
#endif
#ifdef ENABLE_SERIAL2
					case 2:
						BSerial2.setBaud( baud);
					break;
#endif
#ifdef ENABLE_SERIAL3
					case 3:
						BSerial3.setBaud( baud);
					break;
#endif

					default:
					break;
				}

#endif

}


uint32_t HW_getSysticks( void);
uint32_t HW_getMicros( void);
uint32_t HW_getMillis( void);

#ifdef PLATFORM_LA
#include <time.h>
uint32_t HW_getSysticks() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000000000.0;
    return (unsigned) end; 
}
uint32_t HW_getMicros() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000000.0;
    return (unsigned) end; 
}
uint32_t HW_getMillis() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    double end = (t.tv_sec + ((double) t.tv_nsec) / 1000000000.0) * 1000.0;
    return (unsigned) end; 
}
#endif

#ifdef PLATFORM_AC6

uint32_t HW_getSysticks() {
    return HW_getMicros();
}
uint32_t HW_getMicros() {
	uint32_t m0 = HAL_GetTick();
	uint32_t u0 = SysTick->LOAD - SysTick->VAL;
	uint32_t m1 = HAL_GetTick();
	uint32_t u1 = SysTick->LOAD - SysTick->VAL;

	if (m1 > m0) {
		return ( m1 * 1000 + (u1 * 1000) / SysTick->LOAD);
	} else {
		return ( m0 * 1000 + (u0 * 1000) / SysTick->LOAD);
	}
}
uint32_t HW_getMillis() {
    return HAL_GetTick();
}
#endif

#ifdef PLATFORM_ARDUINO
uint32_t HW_getSysticks() {
    return micros(); 
}
uint32_t HW_getMicros() {
    return micros(); 
}
uint32_t HW_getMillis() {
    return millis(); 
} 
#endif