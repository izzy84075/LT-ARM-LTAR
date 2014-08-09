#include "LTAR_Ser.hpp"

void LTAR_Ser::reset(void) {
	RX.reset();
	TX.reset();
	RX_buffer.reset();
	TX_Normal_buffer.reset();
	TX_HighPriority_buffer.reset();
}

LTAR_Ser_Status_t LTAR_Ser::getStatus(void) {
	return status;
}

bool LTAR_Ser::queueBlock(LTAR_Ser_Block_t block, bool highPriority) {
	bool succeeded = false;
	
	if(synced()) {
		if(highPriority) {
			succeeded = TX_HighPriority_buffer.insert(block);
			status.flags.TX_HighPriority_buffer_overrun = !succeeded;
		} else {
			succeeded = TX_Normal_buffer.insert(block);
			status.flags.TX_Normal_buffer_overrun = !succeeded;
		}
	}
	
	return succeeded;
}

void LTAR_Ser::tick2xActiveFreq(void) {
	LTAR_Ser_Block_t temp;
	
	TX.tick2xActiveFreq();
	
	if(!TX.isBusy()) {
		if(TX_HighPriority_buffer.getData(temp)) {
			if(TX.queue(temp)) {
				TX_HighPriority_buffer.advance();
			}
		} else if(TX_Normal_buffer.getData(temp)) {
			if(TX.queue(temp)) {
				TX_Normal_buffer.advance();
			}
		}
	}
}

bool LTAR_Ser::blockWaiting(LTAR_Ser_Block_t &waitingBlock) {
	return RX_buffer.getDataAndAdvance(waitingBlock);
}

bool LTAR_Ser::newSample(unsigned int frequency) {
	bool somethingHappened = RX.newSample(frequency);
	
	if(synced()) {
		status.flags.Synced = true;
		TX.enable = true;
		
		LTAR_Ser_Block_t temp;
		if(RX.getBlock(temp)) {
			status.flags.RX_buffer_overrun = !RX_buffer.insert(temp);
		}
	}
	
	return somethingHappened;
}

bool LTAR_Ser::tick1ms(void) {
	bool lostSync = RX.tick1ms();
	
	if(lostSync) {
		reset();
		status.data = 0;
	}
	
	return lostSync;
}

bool LTAR_Ser::synced(void) {
	return RX.getStatus().flags.synced;
}
