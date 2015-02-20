#include "LTTO_IRTX_NORMAL.hpp"

LTTO_IRTX_NORMAL::LTTO_IRTX_NORMAL( void (*IRLEDON)(void), void (*IRLEDOFF)(void)) {
    LEDON = IRLEDON;
    LEDOFF = IRLEDOFF;
}

void LTTO_IRTX_NORMAL::update_IRLED(ePIN_STATE state) {
    if(state == HIGH) {
        (*LEDON)();
    } else {
        (*LEDOFF)();
    }
}