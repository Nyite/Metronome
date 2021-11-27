#include "dac.h"

uint16_t waveform[DAC_RESOLUTION];	/* Beep waveform buffer */

/**
  * @brief   Sound indication initialization function
  */
void DAC2_init() {

	SetWaveform();
	GPIOA_DAC2_init();

	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	// Conversion trigger on TIM6 TRGO //
	DAC->CR |= DAC_CR_TEN2;
	DAC->CR &= ~DAC_CR_TSEL2;

	DAC->CR |= DAC_CR_DMAEN2;
	DAC->CR |= DAC_CR_EN2;
}

/**
  * @brief   Beep waveform buffer initialization function
  */
void SetWaveform() {
	for (int i = 0; i < DAC_RESOLUTION; i++)
		waveform[i] = 2047.5*(sin(2*M_PI*i/(float)DAC_RESOLUTION) + 1);
}
