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
#include "math.h"

/* Macro to find frequency position in spectrum array */
#define FIND_FREQ_SPECTRUM_POS(freq)	round(freq*FFT_RESOLUTION/SAMPLE_FREQ)

/* Spectrum array positions of filtered frequencies */
#define LOW_PASS_CUTOFF_FREQ_POS	FIND_FREQ_SPECTRUM_POS(LOW_PASS_CUTOFF_FREQ) + 1/* Low pass filter cutoff frequency spectrum array position */
#define BEEP_FREQ1_POS				FIND_FREQ_SPECTRUM_POS(BEEP_FREQ)				/* Beep base frequency spectrum array position */
#define BEEP_FREQ2_POS				FIND_FREQ_SPECTRUM_POS(BEEP_FREQ*2)				/* Beep twice frequency spectrum array position */

/* Initialization functions */
void Button_init();
void LedIndication_init();
void SoundIndication_init();
void Detection_level_init();
void Recording_init();
void UART_Transiver_init();

/* Process functions */
void Recording_process();
void Spectrum_attenuation(uint16_t pos, uint8_t buff_indx, uint8_t DB);
void beep_filtering(uint16_t pos, uint8_t buff_indx);
void FFT_process();

/* Auxiliary functions */
float complexABS(float real, float imag);

#endif /* _MAIN_H_ */
