#include "LTAR_Ser_Rx.hpp"
#include "LTAR_Ser_Tx.hpp"
#include "LTAR_Ser_Block.hpp"
#include "circular-buffer/CircularBuffer.hpp"

#ifndef LTAR_SER_H
#define LTAR_SER_H

typedef union {
	struct {
		unsigned char RX_buffer_overrun:1;
		unsigned char TX_Normal_buffer_overrun:1;
		unsigned char TX_HighPriority_buffer_overrun:1;
		unsigned char Synced:1;
	} flags;
	
	unsigned char data;
} LTAR_Ser_Status_t;

class LTAR_Ser {
	public:
		LTAR_Ser( void (*SERON)(void), void (*SEROFF)(void), unsigned int activeLower = 3800, unsigned int activeUpper = 4200, unsigned int inactiveLower = 1900, unsigned int inactiveUpper = 2100) : TX(SERON, SEROFF), RX(activeLower, activeUpper, inactiveLower, inactiveUpper) {};
		
		//Manager functions
		void reset(void);
		LTAR_Ser_Status_t getStatus(void);
		
		//TX functions
		bool queueBlock(LTAR_Ser_Block_t block, bool highPriority = false);
		void tick2xActiveFreq(void);
		
		//RX functions
		bool blockWaiting(LTAR_Ser_Block_t &waitingBlock);
		bool newSample(unsigned int frequency);
		bool tick1ms(void);
		
	private:
		LTAR_Ser_Tx TX;
		LTAR_Ser_Rx RX;
		LTAR_Ser_Status_t status;
		
		CircularBuffer<LTAR_Ser_Block_t, 10> RX_buffer;
		CircularBuffer<LTAR_Ser_Block_t, 5> TX_Normal_buffer;
		CircularBuffer<LTAR_Ser_Block_t, 5> TX_HighPriority_buffer;
		
		bool synced(void);
};

#endif
