#include "LTTO_IRTX_PWRLVLS.hpp"

#include "LTTO_IRTX_PWRLVLS_conf.hpp"

LTTO_IRTX_PWRLVLS::LTTO_IRTX_PWRLVLS () {
	currentPower = eLTTO_IRTX_PWRLVLS_LEVELS::LOW;
    update_IRLED(ePIN_STATE::LOW);
}

void LTTO_IRTX_PWRLVLS::setPower(eLTTO_IRTX_PWRLVLS_LEVELS power) {
	currentPower = power;
}

eLTTO_IRTX_PWRLVLS_LEVELS LTTO_IRTX_PWRLVLS::getPower(void) {
	return currentPower;
}

void LTTO_IRTX_PWRLVLS::update_IRLED(ePIN_STATE state) {
    if(state == ePIN_STATE::HIGH) {
        switch(currentPower) {
            case eLTTO_IRTX_PWRLVLS_LEVELS::OFF:
                LEDSOFF
                break;
            case eLTTO_IRTX_PWRLVLS_LEVELS::LOW:
                LOW_LEDON
                break;
            case eLTTO_IRTX_PWRLVLS_LEVELS::MEDIUM:
                HIGH_LEDON
                break;
            case eLTTO_IRTX_PWRLVLS_LEVELS::HIGH:
                LOW_LEDON
                HIGH_LEDON
                break;
        }
    } else {
        LEDSOFF
    }
}
