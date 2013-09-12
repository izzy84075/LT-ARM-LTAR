#include "LTAR_Ser_Block.hpp"

#ifndef LTAR_Ser_Tx_H
#define LTAR_Ser_Tx_H

class LTAR_Ser_Tx {
	public:
		LTAR_Ser_Tx( void (*SERON)(void), void (*SEROFF)(void));
		void reset(void);
		bool queue(LTAR_Ser_Block block);
		bool blockInProgress(void);
		void tick2xActiveFreq(void);
	private:
		LTAR_Ser_Block_t workingBuffer;
		unsigned int step;
		unsigned int bitCounter;
		unsigned int byteCounter;
		unsigned int substep;
		bool busy;
		bool nextStep;
		bool readyToTX;
		
		bool sendInactive(void);
		bool sendActive(void);
		
		void (*ON)(void);
		void (*OFF)(void);
};

#endif
