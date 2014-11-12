#ifndef LTAR_SER_BLOCK_H
#define LTAR_SER_BLOCK_H

typedef class LTAR_Ser_Block {
	public:
		LTAR_Ser_Block(void);
		void reset(void);
		bool appendByte(unsigned char input);
		bool appendChecksum(void);
		unsigned char getLength(void);
		unsigned char readByte(unsigned int which);
		bool checkChecksum(void);
	private:
		unsigned char length;
		unsigned char runningChecksum;
		unsigned char data[22];
} LTAR_Ser_Block_t;

#endif
