#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f4xx.h"
#include "global.h"
#include "gpio.h"

#define FFT_SPECTRUM_RES	FFT_RESOLUTION/2	/* FFT spectrum pool */

/* Initialization functions */
void ADC1_init();

#endif /* _ADC_H_ */
