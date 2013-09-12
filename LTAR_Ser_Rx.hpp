#include "LTAR_Ser_Block.hpp"

#ifndef LTAR_SER_RX_H
#define LTAR_SER_RX_H

typedef union {
	struct {
		unsigned char synced:1;
		unsigned char framingError:1;
		unsigned char bufferOverrun:1;
		unsigned char carrierLost:1;
		unsigned char blockReady:1;
	} flags;
	
	unsigned char data;
} LTAR_Ser_Rx_Status_t;

class LTAR_Ser_Rx {
	public:
		LTAR_Ser_Rx(unsigned int activeLower, unsigned int activeUpper, unsigned int inactiveLower, unsigned int inactiveUpper);
		void reset(void);
		bool newSample(unsigned int frequency);
		LTAR_Ser_Rx_Status_t getStatus(void);
		bool tick1ms(void);
		LTAR_Ser_Block getBlock(void);
		void clearBuffer(void);
	private:
		LTAR_Ser_Block_t captureBuffer;
		
		LTAR_Ser_Rx_Status_t status;
		
		unsigned int activeMin;
		unsigned int activeMax;
		unsigned int inactiveMin;
		unsigned int inactiveMax;
		
		unsigned int timeout;
		
		unsigned char captureByte;
		unsigned int inactiveCounter;
		unsigned int bitsCaptured;
		bool startBitDetected;
		
		bool waitForSync(unsigned int frequency);
		bool processAudio(unsigned int frequency);
};

#endif
