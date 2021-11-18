#include "stm32f4xx.h"
#include "gpio.h"
#include "timer.h"
#include "usart.h"
#include "dma.h"
#include "dac.h"
#include "adc.h"
#include "buffer.h"

#define ARM_MATH_CM4
#include "arm_math.h"

int16_t spectrum[FFT_SPECTRUM_RES];
float fft_input[FFT_RESOLUTION];
float fft_output[FFT_RESOLUTION];

extern BUFFER TxData;
extern uint8_t click_flag;
extern uint8_t fft_buffer_process_flag;
extern int16_t data_adc_buff[2][FFT_RESOLUTION];

arm_rfft_fast_instance_f32 fft_handler;

void LedIndication_init() {
	TIM7_init();
	TIM4_init();
}

void SoundIndication_init() {
	DAC2_init();
	DMA1_init();
	TIM6_init();
}

void Recording_init() {
	TIM3_init();
	ADC1_init();
	DMA2_init();
}

void UART_Transiver_init() {
	TIM5_init();
	UART4_init();
}

void Recording_process();
void FFT_process();

int main(void) {
	GPIOD_init();
	LedIndication_init();
	UART_Transiver_init();
	SoundIndication_init();
	Recording_init();

	arm_rfft_fast_init_f32(&fft_handler, FFT_RESOLUTION);

    while(1)
    {
    	USART_Transive();
    	Recording_process();
    }
}

void Recording_process()
{
	if(fft_buffer_process_flag == FFT_BUFFER_FULL)
	{
		fft_buffer_process_flag = FFT_BUFFER_RESET;
		uint8_t current_buffer_indx = DMA2_Stream0->CR & DMA_SxCR_CT ? 0 : 1;
		for(int i = 0; i < FFT_RESOLUTION; i++)
		{
			fft_input[i] = (float)(data_adc_buff[current_buffer_indx][i]);
		}
		FFT_process();
	}
}

float complexABS(float real, float imag) {
	return sqrtf(real*real+imag*imag);
}

void FFT_process()
{
	arm_rfft_fast_f32(&fft_handler, fft_input, fft_output, 0);

	uint16_t spectrum_point = 0;
	uint8_t noise_offset = 100; //variable noise floor offset / default value 100

	for (int i = 2; i < FFT_RESOLUTION; i = i + 2) {
		spectrum[spectrum_point] = (int)(20*log10f(complexABS(fft_output[i], fft_output[i+1]))) - noise_offset;
		if (spectrum[spectrum_point] < 0)
			spectrum[spectrum_point] = 0;
		if (spectrum[spectrum_point] > 13 && click_flag == 1 && TxData.Length == 0) {
			TIM5->CR1 &= ~TIM_CR1_CEN;
			GPIOD_TOGGLE_PIN(GPIOD_PIN_15);
			UART_Latency_Send();
			click_flag = 0;
		}
		spectrum_point++;
	}
}
