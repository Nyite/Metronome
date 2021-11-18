#include "gpio.h"

void GPIOD_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x55 << 24;	// Output mode and pull-down //
	GPIOD->PUPDR |= 0xAA << 24;	// on pins 12, 13, 14, 15	//
}

void GPIOC_UART4_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;		// Alternative function //
	GPIOC->AFR[1] |= 0x88 << 8;	// on pins 10, 11 //
}

void GPIOA_DAC2_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_0 | GPIO_MODER_MODER5_1;
}

void GPIOA_ADC1_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER6_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_1;
}