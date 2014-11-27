#include "LTTO_IRRXTX.hpp"

LTTO_IRRXTX::LTTO_IRRXTX( void (*IRLEDON)(void), void (*IRLEDOFF)(void)) : TX(IRLEDON, IRLEDOFF) {
	reset();
}

void LTTO_IRRXTX::reset(void) {
	RX.reset();
	TX.reset();
}

void LTTO_IRRXTX::LTTO_IRRXTX_1msTick(void) {
	RX.LTTO_IRRX_1msTick();
	TX.LTTO_IRTX_1msTick();
}