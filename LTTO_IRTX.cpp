#include "LTTO_IRTX.hpp"

LTTO_IRTX::LTTO_IRTX( void (*IRLEDON)(void), void (*IRLEDOFF)(void)) {
	LEDON = IRLEDON;
	LEDOFF = IRLEDOFF;
	reset();
}

bool LTTO_IRTX::queueSignature(LTTO_IR signature) {
	if(!busy) {
		workingBuffer = signature;
		step = 0;
		busy = true;
		return true;
	} else {
		return false;
	}
}

bool LTTO_IRTX::signatureInProgress(void) {
	return busy;
}

void LTTO_IRTX::LTTO_IRTX_1msTick(void) {
	if(busy) {
		if(timer > 0) {
			timer--;
		}
		if(timer == 0) {
			switch(step) {
				case 0:
					//PRE-SYNC
					//3ms on
					(*LEDON)();
					timer = 3;
				
					step++;
					break;
				case 1:
					//PRE-SYNC PAUSE
					//6ms off
					(*LEDOFF)();
					timer = 6;
				
					step++;
					break;
				case 2:
					//SYNC
					//3ms or 6ms on
					(*LEDON)();
					switch(workingBuffer.headerType) {
						case eLTTO_IR_HEADERTYPE_NORMAL:
							timer = 3;
							break;
						case eLTTO_IR_HEADERTYPE_BEACON:
							timer = 6;
							break;
					}
					
					//Set up the counter for how many data bits we have to send
					switch(workingBuffer.bitCount) {
						case eLTTO_IR_BITCOUNT_5:
							dataCounter = 5;
							break;
						case eLTTO_IR_BITCOUNT_7:
							dataCounter = 7;
							break;
						case eLTTO_IR_BITCOUNT_8:
							dataCounter = 8;
							break;
						case eLTTO_IR_BITCOUNT_9:
							dataCounter = 9;
							break;
					}
					
					step++;
					break;
				case 3:
					//Data bit pause
					//2ms off
					(*LEDOFF)();
					timer = 2;
				
					step++;
					break;
				case 4:
					//Data bit
					//1ms or 2ms on
					(*LEDON)();
					
					if(workingBuffer.data & 0x01) {
						//This is a "One" bit
						timer = 2;
					} else {
						//This is a "Zero" bit
						timer = 1;
					}
					
					//Figure out whether we're done, shift the data right one if not
					dataCounter--;
					if(dataCounter != 0) {
						//Not done yet
						//Shift the data right one and go back to the data bit pause
						workingBuffer.data >>= 1;
						step = 3;
					} else {
						//Done. On to the SFP!
						step++;
					}
				
					break;
				case 5:
					//Special Format Pause
					//25ms, 56ms or 80ms off
					(*LEDOFF)();
					switch(workingBuffer.SFP) {
						case eLTTO_IR_SFP_SHORT:
							timer = 25;
							break;
						case eLTTO_IR_SFP_MEDIUM:
							timer = 56;
							break;
						case eLTTO_IR_SFP_LONG:
							timer = 80;
							break;
					}
					
					step++;
					break;
				default:
					//Either we just finished a packet or something went wrong. Turn the LED off(Just to be safe) and clean up.
					(*LEDOFF)();
					busy = false;
					workingBuffer.reset();
					step = 0;
					break;
			}
		}
	}
}

void LTTO_IRTX::reset(void) {
	timer = 0;
	workingBuffer.reset();
	busy = false;
	step = 0;
	dataCounter = 0;
}
