#ifndef LTTO_IR_H
#define LTTO_IR_H

typedef enum {
	eLTTO_IR_SIGNATURETYPE_LTTO_BEACON,
	eLTTO_IR_SIGNATURETYPE_LTAR_BEACON,
	eLTTO_IR_SIGNATURETYPE_TAG,
	eLTTO_IR_SIGNATURETYPE_MULTIBYTE_PTYPE,
	eLTTO_IR_SIGNATURETYPE_MULTIBYTE_DATA,
	eLTTO_IR_SIGNATURETYPE_MULTIBYTE_CSUM,
	eLTTO_IR_SIGNATURETYPE_INVALID
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
	uint16_t data;
	
	void reset(void) {
		headerType = eLTTO_IR_HEADERTYPE_NORMAL;
		bitCount = eLTTO_IR_BITCOUNT_5;
		SFP = eLTTO_IR_SFP_SHORT;
		data = 0;
	}
	
	bool convert(LTTO_IR_SIGNATURE *output) {
		switch(headerType) {
			case eLTTO_IR_HEADERTYPE_NORMAL:
				//Tag, multibyte PType, multibyte data, or multibyte CSum
				switch(bitCount) {
					case eLTTO_IR_BITCOUNT_7:
						//Tag
						output->sigType = eLTTO_IR_SIGNATURETYPE_TAG;
						break;
					case eLTTO_IR_BITCOUNT_8:
						//Multibyte data
						output->sigType = eLTTO_IR_SIGNATURETYPE_MULTIBYTE_DATA;
						break;
					case eLTTO_IR_BITCOUNT_9:
						//Multibyte PType or multibyte CSum
						if(data & 0x0100) {
							//Multibyte CSum
							output->sigType = eLTTO_IR_SIGNATURETYPE_MULTIBYTE_CSUM;
						} else {
							//Multibyte PType
							eLTTO_IR_SIGNATURETYPE_MULTIBYTE_PTYPE;
						}
						break;
					default:
						//Invalid
						output->sigType = eLTTO_IR_SIGNATURETYPE_INVALID;
						return false;
				}
				break;
			case eLTTO_IR_HEADERTYPE_BEACON:
				//LTTO beacon or LTAR beacon
				switch(bitCount) {
					case eLTTO_IR_BITCOUNT_5:
						//LTTO beacon
						output->sigType = eLTTO_IR_SIGNATURETYPE_LTTO_BEACON;
						break;
					case eLTTO_IR_BITCOUNT_9:
						//LTAR beacon
						output->sigType = eLTTO_IR_SIGNATURETYPE_LTAR_BEACON;
						break;
					default:
						output->sigType = eLTTO_IR_SIGNATURETYPE_INVALID;
						return false;
				}
				break;
		}
		
		output->data = data;
		
		return true;
	}
} LTTO_IR;

typedef struct {
	eLTTO_IR_SIGNATURETYPE sigType;
	uint16_t data;
	
	void convert(LTTO_IR *output) {
		output->data = 0;
		switch(sigType) {
			case eLTTO_IR_SIGNATURETYPE_LTTO_BEACON:
				output->headerType = eLTTO_IR_HEADERTYPE_BEACON;
				output->bitCount = eLTTO_IR_BITCOUNT_5;
				output->SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_LTAR_BEACON:
				output->headerType = eLTTO_IR_HEADERTYPE_BEACON;
				output->bitCount = eLTTO_IR_BITCOUNT_9;
				output->SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_TAG:
				output->headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				output->bitCount = eLTTO_IR_BITCOUNT_7;
				output->SFP = eLTTO_IR_SFP_MEDIUM;
				break;
			case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_PTYPE:
				output->headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				output->bitCount = eLTTO_IR_BITCOUNT_9;
				output->SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_DATA:
				output->headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				output->bitCount = eLTTO_IR_BITCOUNT_8;
				output->SFP = eLTTO_IR_SFP_SHORT;
				break;
			case eLTTO_IR_SIGNATURETYPE_MULTIBYTE_CSUM:
				output->headerType = eLTTO_IR_HEADERTYPE_NORMAL;
				output->bitCount = eLTTO_IR_BITCOUNT_9;
				output->SFP = eLTTO_IR_SFP_LONG;
				output->data |= 0x0100;
				break;
		}
		
		output->data |= data;
	}
	
	void reset(void) {
		sigType = eLTTO_IR_SIGNATURETYPE_INVALID;
		data = 0;
	}
} LTTO_IR_SIGNATURE;

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
