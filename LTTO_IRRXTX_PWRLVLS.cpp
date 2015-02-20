#include "LTTO_IRRXTX_PWRLVLS.hpp"

LTTO_IRRXTX_PWRLVLS::LTTO_IRRXTX_PWRLVLS() {
	reset();
}

void LTTO_IRRXTX_PWRLVLS::reset(void) {
	RX.reset();
	TX.reset();
}

void LTTO_IRRXTX_PWRLVLS::LTTO_IRRXTX_1msTick(void) {
	RX.LTTO_IRRX_1msTick();
	TX.LTTO_IRTX_1msTick();
}
