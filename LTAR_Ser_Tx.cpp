#include "LTAR_Ser_Tx.hpp"

LTAR_Ser_Tx::LTAR_Ser_Tx( void (*SERON)(void), void (*SEROFF)(void)) {
	ON = SERON;
	OFF = SEROFF;
	
	
	reset();
	
	(*OFF)();
}

void LTAR_Ser_Tx::reset(void) {
	enable = false;
	clear();
}

void LTAR_Ser_Tx::clear(void) {
	workingBuffer.reset();
	step = 0;
	bitCounter = 0;
	byteCounter = 0;
	substep = 0;
	readyToTX = false;
	busy = false;
}

bool LTAR_Ser_Tx::queue(LTAR_Ser_Block block) {
	if(!busy && enable) {
		workingBuffer = block;
		busy = true;
		return true;
	} else {
		return false;
	}
}

void LTAR_Ser_Tx::tick2xActiveFreq(void) {
	if(busy && readyToTX && enable) {
		switch(step) {
			case 0:
				//Need to send a start bit
				if(sendActive()) {
					step += 1;
				}
				break;
			case 1:
				//Sending data bits for a byte
				if((workingBuffer.readByte(byteCounter) >> bitCounter) & 0x01) {
					//Current bit is a 1, send an inactive cycle
					nextStep = sendInactive();
				} else {
					//Current bit is a 0, send an active cycle
					nextStep = sendActive();
				}
				
				if(nextStep) {
					//Done with a bit
					bitCounter += 1;
					if(bitCounter == 8) {
						//Done with a byte
						bitCounter = 0;
						byteCounter += 1;
						step += 1;
					}
				}
				break;
			case 2:
				//Sending stop bits, possibly looping back to the start bit if there's more data
				if(sendInactive()) {
					//Done with a stop bit
					bitCounter += 1;
					if(bitCounter == 2) {
						//Done sending stop bits
						bitCounter = 0;
						if(byteCounter == workingBuffer.getLength()) {
							//No more data to send
							step += 1;
						} else {
							//Still have data to send
							//Back to the start bit!
							step = 0;
						}
					}
				}
				break;
			case 3:
				//Idling to indicate end of block
				if(sendInactive()) {
					//Done with an idle bit
					bitCounter += 1;
					if(bitCounter > 15) {
						//Done idling. Clear everything for the next block!
						clear();
					}
				}
				break;
		}
	} else {
		readyToTX = sendInactive();
	}
}

bool LTAR_Ser_Tx::sendInactive(void) {
	bool isDone = false;

	switch(substep) {
		case 0:
			//Start the high side of the bit
			(*ON)();
			break;
		case 2:
			//Start the low side of the bit
			(*OFF)();
			break;
		case 3:
			//End of a bit
			isDone = true;
			break;
	}
	
	substep += 1;
	
	if(isDone) {
		substep = 0;
	}
	
	return isDone;
}

bool LTAR_Ser_Tx::sendActive(void) {
	bool isDone = false;
	
	switch(substep) {
		case 0:
			//Start the high side of the bit
			(*ON)();
			break;
		case 1:
			//Start the low side of the bit
			(*OFF)();
			isDone = true;
			break;
	}
	
	substep += 1;
	
	if(isDone) {
		substep = 0;
	}
	
	return isDone;
}
