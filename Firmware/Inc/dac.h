#ifndef _DAC_H_
#define _DAC_H_

#include "stm32f4xx.h"
#include "global.h"
#include "gpio.h"
#include "math.h"

#define DAC_RESOLUTION	256	/* Beep waveform resolution */

/* Initialization functions */
void DAC2_init();
void SetWaveform();

#endif /* _DAC_H_ */
