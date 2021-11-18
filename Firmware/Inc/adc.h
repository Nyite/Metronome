#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f4xx.h"
#include "gpio.h"

#define FFT_RESOLUTION		2048
#define FFT_SPECTRUM_RES	FFT_RESOLUTION/2

void ADC1_init();

#endif
