#include "LTTO_IRRX.hpp"
#include "LTTO_IRTX_PWRLVLS.hpp"

#ifndef LTTO_IRRXTX_PWRLVLS_H
#define LTTO_IRRXTX_PWRLVLS_H

class LTTO_IRRXTX_PWRLVLS {
	public:
		LTTO_IRRX RX;
		LTTO_IRTX_PWRLVLS TX;

		LTTO_IRRXTX_PWRLVLS( void (*HIGH_IRLEDON)(void), void (*LOW_IRLEDON)(void), void (*IRLEDSOFF)(void));
		void reset(void);
		void LTTO_IRRXTX_1msTick(void);
	private:

};

#endif
