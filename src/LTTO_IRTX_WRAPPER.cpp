#include "LTTO_IRTX_WRAPPER.hpp"

bool LTTO_IRTX_WRAPPER::queueSingleSignature(eLTTO_IR_SIGNATURETYPE sigType, uint8_t data) {
	uint16_t tempdata = data & 0x00FF;
	if(!signatureInProgress()) {
		LTTO_IR temp;
		
		switch(sigType) {
			case eLTTO_IR_SIGNATURETYPE_LTTO_BEACON:
				temp.headerType = eLTTO_IR_HEADERTYPE_BEACON;
				temp.bitCount = eLTTO_IR_BITCOUNT_5;
				temp.SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_LTAR_BEACON:
				temp.headerType = eLTTO_IR_HEADERTYPE_BEACON;
				temp.bitCount = eLTTO_IR_BITCOUNT_9;
				temp.SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_TAG:
				temp.headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				temp.bitCount = eLTTO_IR_BITCOUNT_7;
				temp.SFP = eLTTO_IR_SFP_MEDIUM;
				break;
			case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_PTYPE:
				temp.headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				temp.bitCount = eLTTO_IR_BITCOUNT_9;
				temp.SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_DATA:
				temp.headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				temp.bitCount = eLTTO_IR_BITCOUNT_8;
				temp.SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_CSUM:
				temp.headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				temp.bitCount = eLTTO_IR_BITCOUNT_9;
				temp.SFP = eLTTO_IR_SFP_LONG;
				tempdata |= 0x0100;
				break;
		}
		
		temp.data = tempdata;
		
		TX.queueSignature(temp);
		
		return true;
	}
	return false;
}

bool LTTO_IRTX_WRAPPER::queueMultibyteMessage(LTTO_IR_MULTIBYTE data) {
	if(storage.size == 0 && data.size) {
		storage = data;
		advanceMultibyte();
		return true;
	}
	return false;
}

bool LTTO_IRTX_WRAPPER::signatureInProgress(void) {
	if(TX.signatureInProgress() || storage.size) {
		return true;
	}
	return false;
}

bool LTTO_IRTX_WRAPPER::multibyteInProgress(void) {
	if(storage.size) {
		return true;
	}
	return false;
}

void LTTO_IRTX_WRAPPER::LTTO_IRTX_1msTick(void) {
	if(!TX.signatureInProgress() && storage.size) {
		advanceMultibyte();
	}
	TX.LTTO_IRTX_1msTick();
}

void LTTO_IRTX_WRAPPER::reset(void) {
	storage.reset();
	TX.reset();
}

void LTTO_IRTX_WRAPPER::advanceMultibyte(void) {
	if(!TX.signatureInProgress()) {
		if(storage_currentposition == 0) {
			queueSingleSignature(eLTTO_IR_SIGNATURETYPE_MULTIBYTE_PTYPE, storage.data[storage_currentposition++]);
		} else if(storage_currentposition < storage.size) {
			queueSingleSignature(eLTTO_IR_SIGNATURETYPE_MULTIBYTE_DATA, storage.data[storage_currentposition++]);
		} else {
			queueSingleSignature(eLTTO_IR_SIGNATURETYPE_MULTIBYTE_CSUM, storage.runningChecksum);
			storage.size = 0;
			storage_currentposition = 0;
		}
	}
}