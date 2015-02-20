#include "stdint.h"

#ifndef LTTO_IR_MULTIBYTE_H
#define LTTO_IR_MULTIBYTE_H

struct LTTO_IR_MULTIBYTE {
	uint8_t data[22];
	uint8_t runningChecksum;
	uint8_t size;

	void reset(void);

	bool appendByte(uint8_t what);
};

#endif
