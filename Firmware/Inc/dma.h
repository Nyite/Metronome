#ifndef _DMA_H_
#define _DMA_H_

#include "stm32f4xx.h"
#include "global.h"
#include "dac.h"
#include "adc.h"

/* Microphone ADC buffer states */
#define FFT_BUFFER_RESET	0	/* Reset ADC buffer state */
#define FFT_BUFFER_FULL		1	/* ADC buffer full and ready to be processed state */

/* Initialization functions */
void DMA1_init();
void DMA2_init();

#endif /* _DMA_H_ */
