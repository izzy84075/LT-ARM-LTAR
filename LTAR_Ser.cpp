#include "LTAR_Ser.hpp"

bool LTAR_Ser::queueBlock(LTAR_Ser_Block_t block, bool highPriority) {
	bool succeeded = false;
	
	if(synced()) {
		if(highPriority) {
			succeeded = TX_HighPriority_buffer.insert(block);
			if(!succeeded) {
				status.flags.TX_HighPriority_buffer_overrun = true;
			}
		} else {
			succeeded = TX_Normal_buffer.insert(block);
			if(!succeeded) {
				status.flags.TX_Normal_buffer_overrun = true;
			}
		}
	}
	
	return succeeded;
}

void LTAR_Ser::tick2xActiveFreq(void) {
	LTAR_Ser_Block_t temp;
	
	if(TX_HighPriority_buffer.getData(temp)) {
		if(TX.queue(temp)) {
			TX_HighPriority_buffer.advance();
		}
	} else if(TX_Normal_buffer.getData(temp)) {
		if(TX.queue(temp)) {
			TX_Normal_buffer.advance();
		}
	}
	
	TX.tick2xActiveFreq();
}

bool LTAR_Ser::blockWaiting(LTAR_Ser_Block_t &waitingBlock) {
	return RX_buffer.getDataAndAdvance(waitingBlock);
}

bool LTAR_Ser::newSample(unsigned int frequency) {
	bool somethingHappened = RX.newSample(frequency);
	
	if(synced()) {
		TX.enable = true;
		
		LTAR_Ser_Block_t temp;
		if(RX.getBlock(temp)) {
			if(!RX_buffer.insert(temp)) {
				status.flags.RX_buffer_overrun = true;
			}
		}
	}
	
	return somethingHappened;
}

bool LTAR_Ser::tick1ms(void) {
	bool lostSync = RX.tick1ms();
	
	if(lostSync) {
		TX.reset();
		RX_buffer.reset();
		TX_Normal_buffer.reset();
		TX_HighPriority_buffer.reset();
	}
	
	return lostSync;
}

bool LTAR_Ser::synced(void) {
	return RX.getStatus().flags.synced;
}
