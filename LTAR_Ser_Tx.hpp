#include "LTAR_Ser_Block.hpp"

#ifndef LTAR_SER_TX_H
#define LTAR_SER_TX_H

class LTAR_Ser_Tx {
	public:
		bool enable;
		
		LTAR_Ser_Tx( void (*SERON)(void), void (*SEROFF)(void));
		void reset(void);
		bool queue(LTAR_Ser_Block block);
		void tick2xActiveFreq(void);
		bool isBusy(void);
	private:
		LTAR_Ser_Block_t workingBuffer;
		unsigned int step;
		unsigned int bitCounter;
		unsigned int byteCounter;
		unsigned int substep;
		bool nextState;
		bool busy;
		bool nextStep;
		bool readyToTX;
		
		bool sendInactive(void);
		bool sendActive(void);
		void clear(void);
		
		void (*ON)(void);
		void (*OFF)(void);
};

#endif
