#include "LTAR_Ser_Block.hpp"

LTAR_Ser_Block::LTAR_Ser_Block(void) {
	reset();
}

void LTAR_Ser_Block::reset(void) {
	length = 0;
	runningChecksum = 0xFF;
	for(int i = 22;i != 0;i--) {
		data[i-1] = 0;
	}
}

bool LTAR_Ser_Block::appendByte(unsigned char input) {
	if(length < 22) {
		data[length] = input;
		length++;
		runningChecksum -= input;
		return true;
	}
	return false;
}

bool LTAR_Ser_Block::appendChecksum(void) {
	if(length < 22) {
		data[length] = runningChecksum;
		length++;
		runningChecksum -= runningChecksum;
		return true;
	}
	return false;
}

unsigned char LTAR_Ser_Block::getLength(void) {
	return length;
}

unsigned char LTAR_Ser_Block::readByte(unsigned int which) {
	if(which < length) {
		return data[which];
	}
	return 0;
}

bool LTAR_Ser_Block::checkChecksum(void) {
	if(runningChecksum == 0) {
		return true;
	}
	return false;
}
