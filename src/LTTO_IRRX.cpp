#include "LTTO_IRRX.hpp"

LTTO_IRRX::LTTO_IRRX(void) {
	reset();
}

void LTTO_IRRX::LTTO_IRRX_1msTick(void) {
	if(multibyteBuffer.size && !multibyteComplete) {
		//If we've started receiving a multibyte packet and it's not complete, increment the age.
		multibyteAge++;
	}
	
	if(multibyteAge > 90) {
		//It's been too long since we last received a multibyte component, and it's still not finished.
		multibyteReset(); //Reset the multibyte buffer.
	}
}

void LTTO_IRRX::reset(void) {
	RX.reset();
	signatureBuffer.reset();
	multibyteReset();
}

eLTTO_IRRX_PACKETREADY LTTO_IRRX::newSample(unsigned int qmsOn, unsigned int qmsOff) {
	if(RX.newSample(qmsOn, qmsOff)) {
		//New raw packet
		LTTO_IR_SIGNATURE temp;
		if(RX.getSignature().convert(temp)) {
			//Valid signature
			switch(temp.sigType) {
				case eLTTO_IR_SIGNATURETYPE_LTTO_BEACON:
				case eLTTO_IR_SIGNATURETYPE_LTAR_BEACON:
				case eLTTO_IR_SIGNATURETYPE_TAG:
					if(signatureBuffer.sigType == eLTTO_IR_SIGNATURETYPE_INVALID) {
						//There's not already a signature in the buffer
						signatureBuffer = temp;
					}
					break;
				case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_PTYPE:
					//The start of a multibyte packet
					if(multibyteBuffer.size && !multibyteComplete) {
						//We're already in the middle of receiving a multibyte packet. Something's wrong.
						multibyteReset(); //Abort!
					} else if(!multibyteComplete) {
						//We're not in the middle of receiving something, and we don't already have a complete packet in the buffer.
						multibyteAge = 0;
						multibyteBuffer.appendByte((uint8_t)(temp.data));
					}
					break;
				case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_DATA:
					//Data in a multibyte packet
					if(multibyteBuffer.size && !multibyteComplete) {
						//We've received the start of a multibyte packet, and it's not marked complete yet.
						multibyteAge = 0;
						multibyteBuffer.appendByte((uint8_t)(temp.data)); //Add it to the buffer!
					}
					break;
				case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_CSUM:
					//Multibyte Checksum
					if(multibyteBuffer.size && !multibyteComplete) {
						//We've received the start of a multibyte packet, and it's not marked complete yet.
						if(multibyteBuffer.runningChecksum = (uint8_t)(temp.data)) {
							//The checksum matches what we've calculated for the received data
							multibyteComplete = true; //Mark the packet complete!
						} else {
							//Checksum didn't match.
							multibyteReset(); //Abort!
						}
					}
					break;
			}
		}
	}
	
	return signaturesWaiting();
}

eLTTO_IRRX_PACKETREADY LTTO_IRRX::signaturesWaiting(void) {
	bool signature = (signatureBuffer.sigType != eLTTO_IR_SIGNATURETYPE_INVALID);
	
	if(signature && multibyteComplete) {
		return BOTH;
	} else if(signature) {
		return SINGLE;
	} else if(multibyteComplete) {
		return MULTIBYTE;
	} else {
		return NONE;
	}
}

bool LTTO_IRRX::getSignature(LTTO_IR_SIGNATURE *output) {
	if(signatureBuffer.sigType != eLTTO_IR_SIGNATURETYPE_INVALID) {
		output = signatureBuffer;
		signatureBuffer.reset();
		return true;
	}
	
	return false;
}

bool LTTO_IRRX::getMultibyte(LTTO_IR_MULTIBYTE *output) {
	if(multibyteComplete) {
		output = multibyteBuffer;
		multibyteReset();
		return true;
	}
	
	return false;
}

void LTTO_IRRX::multibyteReset(void) {
	multibyteBuffer.reset();
	multibyteAge = 0;
	multibyteComplete = false;
}