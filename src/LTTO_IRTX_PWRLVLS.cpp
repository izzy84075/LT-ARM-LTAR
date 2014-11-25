#include "LTTO_IRTX_PWRLVLS.hpp"

LTTO_IRTX_PWRLVLS::LTTO_IRTX_PWRLVLS ( void (*HIGH_IRLEDON)(void), void (*LOW_IRLEDON)(void), void (*IRLEDSOFF)(void)) : LTTO_IRTX (IRLEDON_WRAPPER, IRLEDSOFF) {
	HIGH_LEDON = HIGH_IRLEDON;
	LOW_LEDON = LOW_IRLEDON;
	currentPower = LOW;
}

void LTTO_IRTX_PWRLVLS::setPower(eLTTO_IRTX_PWRLVLS_LEVELS power) {
	currentPower = power;
}

eLTTO_IRTX_PWRLVLS_LEVELS LTTO_IRTX_PWRLVLS::getPower(void) {
	return currentPower;
}

void LTTO_IRTX_PWRLVLS::IRLEDON_WRAPPER(void) {
	switch(currentPower) {
		case OFF:
			(*IRLEDOFF)();
			break;
		case LOW:
			(*LOW_LEDON)();
			break;
		case MEDIUM:
			(*HIGH_LEDON)();
			break;
		case HIGH:
			(*LOW_LEDON)();
			(*HIGH_LEDON)();
			break;
	}
}