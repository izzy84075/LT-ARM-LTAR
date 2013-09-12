#include "stm32f30x.h"

#include "LTAR_Ser_Rx.hpp"
#include "LTAR_Ser_Tx.hpp"
#include "LTAR_Ser_Block.hpp"

int main(void);
void SERON(void);
void SEROFF(void);

unsigned int delay;
bool fire;

GPIO_InitTypeDef GPIO_InitStructure;

LTAR_Ser_Rx SerRx(4800, 3200, 2400, 1600);
LTAR_Ser_Tx SerTx(&SERON, &SEROFF);

extern "C" {
	void SysTick_Handler(void) {
		SerTx.tick2xActiveFreq();
	}
}

int main(void) {
	//SystemInit();
	SysTick_Config(SystemCoreClock / 8800);
	
	//Turn on the GPIO clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	
	//Set up our GPIO pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	LTAR_Ser_Block_t block;
	block.appendByte(0x01);
	block.appendByte(0x02);
	block.appendByte(0x03);
	block.appendByte(0x04);
	block.appendByte(0x05);
	block.appendChecksum();
	
	while(1) {
		if(!SerTx.blockInProgress()) {
			SerTx.queue(block);
		}
	}
}

void SERON(void) {
	GPIOB->BSRR = GPIO_Pin_15;
}

void SEROFF(void) {
	GPIOB->BRR = GPIO_Pin_15;
}
