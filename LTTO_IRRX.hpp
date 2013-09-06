#include "LTTO_IR.hpp"

#ifndef LTTO_IRRX_H
#define LTTO_IRRX_H

class LTTO_IRRX {
	public:
		LTTO_IRRX(void);
		bool newSample(unsigned int qmsOn, unsigned int qmsOff);
		bool isSignatureWaiting(void);
		LTTO_IR getSignature(void);
		void reset(void);
	private:
		unsigned int raw;
		unsigned int step;
		LTTO_IR workingBuffer;
		bool signatureWaiting;
};

#endif
