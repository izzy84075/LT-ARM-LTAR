#include "stdint.h"

#ifndef LTTO_IR_SIGNATURE_H
#define LTTO_IR_SIGNATURE_H

enum class eLTTO_IR_SIGNATURETYPE {
	LTTO_BEACON,
	LTAR_BEACON,
	TAG,
	MULTIBYTE_PTYPE,
	MULTIBYTE_DATA,
	MULTIBYTE_CSUM,
	INVALID
};

typedef struct LTTO_IR_SIGNATURE {
	eLTTO_IR_SIGNATURETYPE sigType;
	uint16_t data;

	void reset(void);
} LTTO_IR_SIGNATURE;

#endif
