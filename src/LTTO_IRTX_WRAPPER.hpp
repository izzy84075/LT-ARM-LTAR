#include "LTTO_IR.hpp"
#include "LTTO_IRTX.hpp"

#ifndef LTTO_IRTX_WRAPPER_H
#define LTTO_IRTX_WRAPPER_H

class LTTO_IRTX_WRAPPER {
	public:
		LTTO_IRTX_WRAPPER( void (*IRLEDON)(void), void (*IRLEDOFF)(void)) : TX(IRLEDON, IRLEDOFF) {};
		bool queueSingleSignature(eLTTO_IR_SIGNATURETYPE sigType, uint16_t data);
		bool queueMultibyteMessage(LTTO_IR_MULTIBYTE data);
		bool signatureInProgress(void);
		bool multibyteInProgress(void);
		void LTTO_IRTX_1msTick(void);
		void reset(void);
	private:
		LTTO_IRTX TX;
		
		LTTO_IR_MULTIBYTE storage;
		uint8_t storage_currentposition;
		
		bool advanceMultibyte(void);
};

#endif