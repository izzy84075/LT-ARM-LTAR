#include "LTAR_Ser_Rx.hpp"

LTAR_Ser_Rx::LTAR_Ser_Rx(unsigned int activeLower, unsigned int activeUpper, unsigned int inactiveLower, unsigned int inactiveUpper) {
	reset();
	activeMin = activeLower;
	activeMax = activeUpper;
	inactiveMin = inactiveLower;
	inactiveMax = inactiveUpper;
}

void LTAR_Ser_Rx::reset(void) {
	status.data = 0;
	timeout = 0;
	clearBuffer();
}

bool LTAR_Ser_Rx::newSample(unsigned int frequency) {
	if(status.flags.synced) {
		return processAudio(frequency);
	} else {
		return waitForSync(frequency);
	}
}

LTAR_Ser_Rx_Status_t LTAR_Ser_Rx::getStatus(void) {
	return status;
}

bool LTAR_Ser_Rx::tick1ms(void) {
	if(timeout) {
		timeout -= 1;
		if(timeout == 0) {
			reset();
			status.flags.carrierLost = true;
			return true;
		}
	}
	return false;
}

bool LTAR_Ser_Rx::getBlock(LTAR_Ser_Block &block) {
	if(status.flags.blockReady) {
		block = captureBuffer;
		clearBuffer();
		return true;
	} else {
		return false;
	}
}

void LTAR_Ser_Rx::clearBuffer(void) {
	captureByte = 0;
	inactiveCounter = 0;
	bitsCaptured = 0;
	startBitDetected = false;
	status.flags.blockReady = false;
	captureBuffer.reset();
}

bool LTAR_Ser_Rx::waitForSync(unsigned int frequency) {
	if(frequency >= inactiveMin && frequency < inactiveMax) {
		inactiveCounter++;
		if(inactiveCounter > 14) {
			status.data = 0;
			status.flags.synced = true;
			timeout = 100;
			inactiveCounter = 0;
			return true;
		}
	} else {
		inactiveCounter = 0;
	}
	return false;
}

bool LTAR_Ser_Rx::processAudio(unsigned int frequency) {
	unsigned char data_bit;
	
	if(frequency >= activeMin && frequency < activeMax) {
		//"Active" cycle. A "0".
		data_bit = 0;
		inactiveCounter = 0;
	} else if(frequency >= inactiveMin && frequency < inactiveMax) {
		//"Inactive" cycle. A "1".
		data_bit = 1;
		inactiveCounter += 1;
	} else {
		//Noise of some sort. Don't do anything.
		return false;
	}
	
	timeout = 100;
	
	if(!startBitDetected) {
		if(!data_bit) {
			//This is the start bit
			startBitDetected = true;
			bitsCaptured = 0;
			captureByte = 0;
			return false;
		}
	} else {
		if(bitsCaptured < 8) {
			//Data bit
			captureByte >>= 1;
			if(data_bit) {
				captureByte |= 0x80;
			} else {
				captureByte &= 0x7F;
			}
			bitsCaptured += 1;
			return false;
		} else {
			startBitDetected = false;
			if(data_bit) {
				//Stop bit
				if(!captureBuffer.appendByte(captureByte)) {
					//Block too long to be valid.
					status.data = 0;
					status.flags.bufferOverrun = true;
					captureBuffer.reset();
					return true;
				}
			} else {
				//Incorrectly framed byte
				captureBuffer.reset();
				status.data = 0;
				status.flags.framingError = true;
				return true;
			}
		}
	}
	
	if(captureBuffer.getLength() && (inactiveCounter > 14)) {
		if(captureBuffer.getLength() > 1) {
			//Block finished
			
			status.flags.blockReady = true;
			return true;
		} else {
			//Not a long enough block to be valid.
			//Reset for next block.
			captureBuffer.reset();
		}
	}
	return false;
}
