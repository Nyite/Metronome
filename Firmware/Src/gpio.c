#include "gpio.h"

/**
  * @brief   Metronome sound indication button interrupt handler
  */
void EXTI0_IRQHandler() {
	EXTI->PR |= EXTI_PR_PR0;
	TIM2->CR1 |= TIM_CR1_CEN;
}

/**
  * @brief   Metronome sound indication button GPIO initialization
  */
void GPIOA_Button_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}

/**
  * @brief   Metronome LED indication initialization
  */
void GPIOD_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD->MODER |= 0x55 << 24;				/* Output mode and pull-down */
	GPIOD->PUPDR |= 0xAA << 24;				/* on pins 12, 13, 14, 15	 */
}

/**
  * @brief   Metronome sound indication button interrupt initialization
  */
void EXTI_Button_init() {
	RCC->AHB2ENR |= RCC_APB2ENR_SYSCFGEN;
	EXTI->IMR 	 |= EXTI_IMR_MR0;
	EXTI->RTSR   |= EXTI_RTSR_TR0;
	NVIC_SetPriority(EXTI0_IRQn, 3);
	NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
  * @brief   USART transceiver initialization
  */
void GPIOC_UART4_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;	/* Alternative function */
	GPIOC->AFR[1] |= 0x88 << 8;	// on pins 10, 11 //
}

/**
  * @brief   Metronome sound indication initialization
  */
void GPIOA_DAC2_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER5_0 | GPIO_MODER_MODER5_1;
}

/**
  * @brief   Metronome microphone ADC initialization
  */
void GPIOA_ADC1_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER6_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_1;
}
