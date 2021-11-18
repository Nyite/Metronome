#ifndef _DAC_H
#define _DAC_H

#define DAC_RESOLUTION	256

#include "stm32f4xx.h"
#include "gpio.h"
#include "math.h"

void DAC2_init();
void SetWaveform();

#endif
