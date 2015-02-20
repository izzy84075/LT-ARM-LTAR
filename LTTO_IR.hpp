#ifndef LTTO_IR_H
#define LTTO_IR_H

#include "stdint.h"

#include "LTTO_IR_SIGNATURE.hpp"
#include "LTTO_IR_MULTIBYTE.hpp"

enum class eLTTO_IR_HEADERTYPE {
	NORMAL = 0x00,
	BEACON = 0x01
};

enum class eLTTO_IR_BITCOUNT {
	FIVE = 0x00,
	SEVEN = 0x01,
	EIGHT = 0x02,
	NINE = 0x03
};

enum class eLTTO_IR_SFP {
	SHORT = 0x00,
	MEDIUM = 0x01,
	LONG = 0x02
};

typedef struct LTTO_IR {
	eLTTO_IR_HEADERTYPE headerType;
	eLTTO_IR_BITCOUNT bitCount;
	eLTTO_IR_SFP SFP;
	uint16_t data;

	void reset(void);

	LTTO_IR& operator= (const LTTO_IR_SIGNATURE& from);
	bool convertTo(LTTO_IR_SIGNATURE *output);
} LTTO_IR;

#endif
