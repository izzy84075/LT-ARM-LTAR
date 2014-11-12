#include <stm32f30x.h>

#include "LTAR_Ser.hpp"
#include "LTAR_Ser_Block.hpp"

int main(void);
void SERON(void);
void SEROFF(void);

unsigned int delay;
bool fire;

GPIO_InitTypeDef GPIO_InitStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;
RCC_ClocksTypeDef RCC_Clocks;

LTAR_Ser Ser(&SERON, &SEROFF);

extern "C" {
	void SysTick_Handler(void) {
		Ser.tick2xActiveFreq();
	}

	void TIM2_IRQHandler(void)
	{
		RCC_GetClocksFreq(&RCC_Clocks);

		/* Clear TIM2 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

		/* Get the Input Capture value */
		IC2Value = TIM_GetCapture2(TIM2);

		if (IC2Value != 0)
		{
			/* Duty cycle computation */
			DutyCycle = (TIM_GetCapture1(TIM2) * 100) / IC2Value;

			/* Frequency computation
			   TIM2 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */

			Frequency = RCC_Clocks.HCLK_Frequency / IC2Value;
		}
		else
		{
			DutyCycle = 0;
			Frequency = 0;
		}
		if(Frequency > 1500 && Frequency < 2500) {
			GPIOB->BSRR |= GPIO_Pin_8;
		} else if(Frequency > 3500 && Frequency < 4500) {
			GPIOB->BRR |= GPIO_Pin_8;
		}
		//SerRx.newSample(Frequency);
	}
}

int main(void) {
	//SystemInit();
	SysTick_Config(SystemCoreClock / 8800);

	//Turn on the GPIO clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* GPIOA clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	//Set up our GPIO pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 + GPIO_Pin_8 + GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* TIM2 chennel2 configuration : PA.01 */
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect TIM pin to AF1 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1);
	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;

	TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

	/* Select the TIM2 Input Trigger: TI2FP2 */
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);

	/* Select the slave Mode: Reset Mode */
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);

	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);

	/* Enable the CC2 Interrupt Request */
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);

	LTAR_Ser_Block_t RXblock;

	LTAR_Ser_Block_t block;
	block.appendByte(0x01);
	block.appendByte(0x02);
	block.appendByte(0x03);
	block.appendByte(0x04);
	block.appendByte(0x05);
	block.appendChecksum();

	for(int i = 30; i != 0; i--) {
		Ser.newSample(2000);
	}

	//Start bit
	Ser.newSample(4000);
	//Data bits
	Ser.newSample(4000);
	Ser.newSample(2000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	//Stop bits
	Ser.newSample(2000);
	Ser.newSample(2000);

	//Start bit
	Ser.newSample(4000);
	//Data bits
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	Ser.newSample(4000);
	//Stop bits
	Ser.newSample(2000);
	Ser.newSample(2000);

	while(1) {
		Ser.queueBlock(block);

		Ser.blockWaiting(RXblock);
	}
}

void SERON(void) {
	GPIOB->BSRR = GPIO_Pin_15;
}

void SEROFF(void) {
	GPIOB->BRR = GPIO_Pin_15;
}
