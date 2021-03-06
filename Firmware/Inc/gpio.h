#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f4xx.h"
#include "global.h"

/* GPIOD LED indication process macros */
#define GPIOD_TOGGLE_PIN(pin_msk)	GPIOD->ODR ^= 0x1 << pin_msk
#define GPIOD_SET_PIN(pin_msk)		GPIOD->BSRR = 0x1 << pin_msk
#define GPIOD_RESET_PIN(pin_msk)	GPIOD->BSRR = 0x1 << (pin_msk + 16)

/* Initialization functions */
void GPIOA_Button_init();
void GPIOD_init();
void EXTI_Button_init();
void GPIOC_UART4_init();
void GPIOA_DAC2_init();
void GPIOA_ADC1_init();

/* LED indicators PIN encoding */
enum GPIOD_PIN_MSK {
	GPIOD_PIN_12 = 12,
	GPIOD_PIN_13 = 13,
	GPIOD_PIN_14 = 14,
	GPIOD_PIN_15 = 15,
};

#endif /* _GPIO_H_ */
