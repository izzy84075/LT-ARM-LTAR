#include "LTTO_IRTX.hpp"

#ifndef LTTO_IRTX_PWRLVLS_H
#define LTTO_IRTX_PWRLVLS_H

typedef enum {
	OFF,
	LOW,
	MEDIUM,
	HIGH
} eLTTO_IRTX_PWRLVLS_LEVELS;

class LTTO_IRTX_PWRLVLS : public LTTO_IRTX {
	public:
		LTTO_IRTX_PWRLVLS( void (*HIGH_IRLEDON)(void), void (*LOW_IRLEDON)(void), void (*IRLEDSOFF)(void)) : LTTO_IRTX (IRLEDON_WRAPPER, IRLEDSOFF);
		
		void setPower(eLTTO_IRTX_PWRLVLS_LEVELS power);
		eLTTO_IRTX_PWRLVLS_LEVELS getPower(void);
	private:
		eLTTO_IRTX_PWRLVLS_LEVELS currentPower;
		
		void (*HIGH_LEDON)(void);
		void (*LOW_LEDON)(void);
		
		void IRLEDON_WRAPPER(void);
};

#endif