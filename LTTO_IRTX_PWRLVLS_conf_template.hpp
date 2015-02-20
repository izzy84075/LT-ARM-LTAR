#include "stm32l051xx.h"
#include "stm32l0xx_hal_gpio.h"

#define LEDSOFF GPIOB->BRR |= GPIO_PIN_5 | GPIO_PIN_4;
#define LOW_LEDON GPIOB->BSRR |= GPIO_PIN_4;
#define HIGH_LEDON GPIOB->BSRR |= GPIO_PIN_5;
