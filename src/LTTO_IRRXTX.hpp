#include "LTTO_IRRX.hpp"
#include "LTTO_IRTX.hpp"

#ifndef LTTO_IRRXTX_H
#define LTTO_IRRXTX_H

class LTTO_IRRXTX {
	public:
		LTTO_IRRX RX;
		LTTO_IRTX TX;
		
		LTTO_IRRXTX( void (*IRLEDON)(void), void (*IRLEDOFF)(void)) : TX(IRLEDON, IRLEDOFF);
		void reset(void);
		void LTTO_IRRXTX_1msTick(void);
	private:
		
};

#endif