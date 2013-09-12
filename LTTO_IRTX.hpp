#include "LTTO_IR.hpp"

#ifndef LTTO_IRTX_H
#define LTTO_IRTX_H

class LTTO_IRTX {
	public:
		LTTO_IRTX( void (*IRLEDON)(void), void (*IRLEDOFF)(void));
		bool queueSignature(LTTO_IR signature);
		bool signatureInProgress(void);
		void LTTO_IRTX_1msTick(void);
		void reset(void);
	private:
		LTTO_IR workingBuffer;
		unsigned int timer;
		unsigned int step;
		unsigned int dataCounter;
		bool busy;
		void (*LEDON)(void);
		void (*LEDOFF)(void);
};

#endif
