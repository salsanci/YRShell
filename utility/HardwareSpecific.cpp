#include "HardwareSpecific.h"

#ifdef PLATFORM_AC6
#include "STMSerialLib/STMSerial.h"
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
void HW_setSerialFlowControl( uint32_t port, uint32_t control) {
	#ifdef PLATFORM_AC6
            	STMSerial::setUartFlowControl( port, control);
#endif
#ifdef PLATFORM_ARDUINO
				switch( port) {
#ifdef ENABLE_SERIAL
					case 0:
//						BSerial.setBaud( baud);
					break;
#endif
#ifdef ENABLE_SERIAL1
					case 1:
//						BSerial1.setBaud( baud);
					break;
#endif
#ifdef ENABLE_SERIAL2
					case 2:
//						BSerial2.setBaud( baud);
					break;
#endif
#ifdef ENABLE_SERIAL3
					case 3:
//						BSerial3.setBaud( baud);
					break;
#endif

					default:
					break;
				}

#endif
}

#ifdef PLATFORM_LUA
#include <time.h>
// TODO CHECK RESOLUTION IS APPROPRIATE 2017-11-28 SAL
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
uint32_t HW_getSysTicksPerSecond( ) {
	return 1000000000;
}
#endif

#ifdef PLATFORM_AC6

uint32_t HW_getSysticks() {
	// int irqMask = __disable_irq();
	uint32_t m0 = HAL_GetTick();
	uint32_t u0 = SysTick->LOAD - SysTick->VAL;
	uint32_t m1 = HAL_GetTick();
	uint32_t u1 = SysTick->LOAD - SysTick->VAL;
	//if( irqMask) {
	//	__enable_irq( );
	//}

	if (m1 > m0) {
		return m1 * SysTick->LOAD + u1;
	} else {
		return m0 * SysTick->LOAD + u0;
	}
}
uint32_t HW_getMicros() {
	//int irqMask = __disable_irq();
	uint32_t m0 = HAL_GetTick();
	uint32_t u0 = SysTick->LOAD - SysTick->VAL;
	uint32_t m1 = HAL_GetTick();
	uint32_t u1 = SysTick->LOAD - SysTick->VAL;
	//if( irqMask) {
	//	__enable_irq( );
	//}

	if (m1 > m0) {
		return ( m1 * 1000 + (u1 * 1000) / SysTick->LOAD);
	} else {
		return ( m0 * 1000 + (u0 * 1000) / SysTick->LOAD);
	}
}
uint32_t HW_getMillis() {
    return HAL_GetTick();
}
uint32_t HW_getSysTicksPerSecond( ) {
	return HAL_RCC_GetHCLKFreq();
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
uint32_t HW_getSysTicksPerSecond( ) {
	return 1000000;
}
#endif


