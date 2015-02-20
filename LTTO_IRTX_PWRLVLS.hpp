#include "LTTO_IRTX.hpp"

#include "../pinstate/pinstate.hpp"

#ifndef LTTO_IRTX_PWRLVLS_H
#define LTTO_IRTX_PWRLVLS_H

enum class eLTTO_IRTX_PWRLVLS_LEVELS {
	OFF,
	LOW,
	MEDIUM,
	HIGH
};

class LTTO_IRTX_PWRLVLS : public LTTO_IRTX {
	public:
		LTTO_IRTX_PWRLVLS( void (*HIGH_IRLEDON)(void), void (*LOW_IRLEDON)(void), void (*IRLEDSOFF)(void));

		void setPower(eLTTO_IRTX_PWRLVLS_LEVELS power);
		eLTTO_IRTX_PWRLVLS_LEVELS getPower(void);
    private:
		eLTTO_IRTX_PWRLVLS_LEVELS currentPower;

		void (*HIGH_LEDON)(void);
		void (*LOW_LEDON)(void);
        void (*LEDSOFF)(void);

		void update_IRLED(ePIN_STATE state);
};

#endif
