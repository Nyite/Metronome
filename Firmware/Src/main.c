#include "main.h"

#define ARM_MATH_CM4							/* CMSIS FFT lib define */
#include "arm_math.h"							/* CMSIS FFT lib include */

uint8_t spectrum[FFT_SPECTRUM_RES];				/* Spectrum [dB] of incoming microphone signal */
float fft_input[FFT_RESOLUTION];				/* Raw ADC data to be processed in FFT */
float fft_output[FFT_RESOLUTION];				/* Raw FFT processed data: complex values is form of [real1; imag1; real2; imag2; ...] */

extern uint8_t btn_flag;						/* @timer.c */
extern BUFFER TxData;							/* @buffer.c */
extern uint8_t fft_buffer_process_flag;			/* @dma.c */
extern int16_t data_adc_buff[2][FFT_RESOLUTION];/* @dma.c */

arm_rfft_fast_instance_f32 fft_handler;			/* CMSIS lib FFT handler declaration */

int main(void) {
	/* Program initialization */
#ifndef SPECTRUM_DEBUG
	LedIndication_init();
	SoundIndication_init();
#endif
	UART_Transiver_init();
	Recording_init();
	Button_init();
	arm_rfft_fast_init_f32(&fft_handler, FFT_RESOLUTION);	/* CMSIS lib FFT handler initialization */

	/* Main process loop */
    while(1)
    {
    	USART_Transive();
    	Recording_process();
    }
}

/**
  * @brief   Sound indication switch initialization
  */
void Button_init() {
#ifdef SPECTRUM_DEBUG
	btn_flag = BUTTON_RESET;
#endif
	GPIOA_Button_init();
	EXTI_Button_init();
	TIM2_init();
}

/**
  * @brief   Sound indication switch initialization
  */
void LedIndication_init() {
	GPIOD_init();
	TIM7_init();
	TIM4_init();
}

/**
  * @brief   Sound indication initialization
  */
void SoundIndication_init() {
	DAC2_init();
	DMA1_init();
	TIM6_init();
}

/**
  * @brief   Microphone recording initialization
  */
void Recording_init() {
	TIM3_init();
	ADC1_init();
	DMA2_init();
}

/**
  * @brief   UART transceiver initialization
  */
void UART_Transiver_init() {
	TIM5_init();
	UART4_init();
}

/**
  * @brief   Recorded ADC data process
  * @note		DMA is configured in double buffer mode
  * 			In this function only a full buffer is processed
  */
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

/**
  * @brief   Complex absolute value function
  */
float complexABS(float real, float imag) {
	return sqrtf(real*real+imag*imag);
}

/**
  * @brief   Spectrum calculation function
  */
void FFT_process()
{
	arm_rfft_fast_f32(&fft_handler, fft_input, fft_output, 0);

	uint16_t spectrum_point = 0;	/* Additional iterator for spectrum array */

	for (int i = 2; i < FFT_RESOLUTION; i = i + 2) {
		int16_t tmp = (int)(20*log10f(complexABS(fft_output[i], fft_output[i+1]))) - NOISE_FLOOR;
		spectrum[spectrum_point] = tmp > 0 ? tmp : 0; /* Spectrum [dB] of incoming microphone signal */
#ifndef SPECTRUM_DEBUG
		if (spectrum[spectrum_point] >= TRANSIENT_THR && (TIM5->CR1 & TIM_CR1_CEN) && TxData.Length == 0) {		/* Sound transient detector */
			TIM5->CR1 &= ~TIM_CR1_CEN;
			GPIOD_TOGGLE_PIN(GPIOD_PIN_15);
			UART_Latency_Send();

		}
#endif
		spectrum_point++;
	}
}
