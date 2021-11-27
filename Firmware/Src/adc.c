#include "adc.h"

/**
  * @brief   Microphone ADC initialization function
  */
void ADC1_init()
{
	GPIOA_ADC1_init();
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->SQR3 |= ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_1;

	ADC1->CR1 |= ADC_CR1_DISCEN;
	ADC1->CR2 |= ADC_CR2_EXTEN_0;	/* Rising edge of */
	ADC1->CR2 |= ADC_CR2_EXTSEL_3;	/* TIM3_TRGO master trigger event */

	ADC1->CR2 |= ADC_CR2_DDS;		/* DMA Enable */
	ADC1->CR2 |= ADC_CR2_DMA;

	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}
