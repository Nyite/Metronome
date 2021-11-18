#ifndef _DMA_H_
#define _DMA_H_

#include "stm32f4xx.h"
#include "dac.h"
#include "adc.h"

#define FFT_BUFFER_RESET	0
#define FFT_BUFFER_FULL		1

void DMA1_init();
void DMA2_init();

#endif
