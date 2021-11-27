#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f4xx.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "dma.h"
#include "dac.h"
#include "adc.h"
#include "buffer.h"

/* Initialization functions */
void Button_init();
void LedIndication_init();
void SoundIndication_init();
void Recording_init();
void UART_Transiver_init();

/* Process functions */
void Recording_process();
void FFT_process();

/* Auxiliary functions */
float complexABS(float real, float imag);

#endif /* _MAIN_H_ */
