#ifndef LTTO_IR_H
#define LTTO_IR_H

typedef enum {
	eLTTO_IR_SIGNATURETYPE_LTTO_BEACON,
	eLTTO_IR_SIGNATURETYPE_LTAR_BEACON,
	eLTTO_IR_SIGNATURETYPE_TAG,
	eLTTO_IR_SIGNATURETYPE_MULTIBYTE_PTYPE,
	eLTTO_IR_SIGNATURETYPE_MULTIBYTE_DATA,
	eLTTO_IR_SIGNATURETYPE_MULTIBYTE_CSUM
} eLTTO_IR_SIGNATURETYPE;

typedef enum {
	eLTTO_IR_HEADERTYPE_NORMAL = 0x00,
	eLTTO_IR_HEADERTYPE_BEACON = 0x01
} eLTTO_IR_HEADERTYPE;

typedef enum {
	eLTTO_IR_BITCOUNT_5 = 0x00,
	eLTTO_IR_BITCOUNT_7 = 0x01,
	eLTTO_IR_BITCOUNT_8 = 0x02,
	eLTTO_IR_BITCOUNT_9 = 0x03
} eLTTO_IR_BITCOUNT;

typedef enum {
	eLTTO_IR_SFP_SHORT = 0x00,
	eLTTO_IR_SFP_MEDIUM = 0x01,
	eLTTO_IR_SFP_LONG = 0x02
} eLTTO_IR_SFP;

typedef struct {
	eLTTO_IR_HEADERTYPE headerType;
	eLTTO_IR_BITCOUNT bitCount;
	eLTTO_IR_SFP SFP;
	unsigned int data;
	
	void reset(void) {
		headerType = eLTTO_IR_HEADERTYPE_NORMAL;
		bitCount = eLTTO_IR_BITCOUNT_5;
		SFP = eLTTO_IR_SFP_SHORT;
		data = 0;
	}
} LTTO_IR;

typedef struct {
	uint8_t data[22];
	uint8_t runningChecksum;
	uint8_t size;
	
	void reset(void) {
		size = 0;
	}
	
	bool appendByte(uint8_t what) {
		if(size < 23) {
			data[size] = what;
			runningChecksum += what;
			size++
			return true;
		}
		return false;
	}
} LTTO_IR_MULTIBYTE;

#endif
