#include "LTTO_IR.hpp"
#include "LTTO_IR_RAW.hpp"

#ifndef LTTO_IRRX_H
#define LTTO_IRRX_H

typedef enum {
	NONE,
	SINGLE,
	MULTIBYTE,
	BOTH
} eLTTO_IRRX_PACKETREADY;

class LTTO_IRRX {
	public:
		LTTO_IRRX(void);
		void LTTO_IRRX_1msTick(void);
		void reset(void);
		
		eLTTO_IRRX_PACKETREADY newSample(unsigned int qmsOn, unsigned int qmsOff);
		
		eLTTO_IRRX_PACKETREADY signaturesWaiting(void);
		bool getSignature(LTTO_IR_SIGNATURE *output);
		bool getMultibyte(LTTO_IR_MULTIBYTE *output);
	private:
		LTTO_IRRX_RAW RX;
		
		LTTO_IR_SIGNATURE signatureBuffer;
		
		LTTO_IR_MULTIBYTE multibyteBuffer;
		uint8_t multibyteAge;
		bool multibyteComplete;
		
		void multibyteReset(void);
}

#endif