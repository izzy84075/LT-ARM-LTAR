#include "LTTO_IR.hpp"

#include "../pinstate/pinstate.hpp"

#ifndef LTTO_IRTX_H
#define LTTO_IRTX_H

class LTTO_IRTX {
	public:
		LTTO_IRTX();
		void reset(void);
		void LTTO_IRTX_1msTick(void);

		bool enqueue(eLTTO_IR_SIGNATURETYPE sigType, uint16_t data);
		bool enqueue(LTTO_IR_SIGNATURE &signature);
		bool enqueue(LTTO_IR_MULTIBYTE &structure);
		bool enqueue(LTTO_IR &signature);

		bool isBusy(void);
	private:
		LTTO_IR workingBuffer;
		LTTO_IR_MULTIBYTE multibyteBuffer;

		unsigned int timer;
		unsigned int step;
		unsigned int dataCounter;
		unsigned int multibytePosition;
		bool busy;

        virtual void update_IRLED(ePIN_STATE state);

		bool tryAdvancingMultibyte(void);
};

#endif
