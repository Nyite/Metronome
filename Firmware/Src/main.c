#include "main.h"

#define ARM_MATH_CM4							/* CMSIS FFT lib define */
#include "arm_math.h"							/* CMSIS FFT lib include */

uint8_t spectrum_active_buffer;					/* Spectrum active buffer index */
uint8_t spectrum[2][FFT_SPECTRUM_RES];			/* Spectrum [dB] of incoming microphone signal */
float transient_level[FFT_SPECTRUM_RES];		/* Threshold for transient detection with frequency adaptation */
float fft_input[FFT_RESOLUTION];				/* Raw ADC data to be processed in FFT */
float fft_output[FFT_RESOLUTION];				/* Raw FFT processed data: complex values is form of [real1; imag1; real2; imag2; ...] */

extern uint8_t send_flag;						/* @usart.c */
extern uint8_t btn_flag;						/* @timer.c */
extern BUFFER TxData;							/* @buffer.c */
extern uint8_t fft_buffer_process_flag;			/* @dma.c */
extern int16_t data_adc_buff[2][FFT_RESOLUTION];/* @dma.c */

arm_rfft_fast_instance_f32 fft_handler;			/* CMSIS lib FFT handler declaration */

int main(void) {
	/* Program initialization */
	LedIndication_init();
	SoundIndication_init();
	UART_Transiver_init();
	Recording_init();
	Button_init();
	arm_rfft_fast_init_f32(&fft_handler, FFT_RESOLUTION);	/* CMSIS lib FFT handler initialization */

	/* Main process loop */
    while(1) {
    	USART_Transive();
    	Recording_process();
    }
}

/**
  * @brief   Sound indication switch initialization
  */
void Button_init() {
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
	spectrum_active_buffer = 0;
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
  * @brief   Spectrum frequency attenuation function
  */
void Spectrum_attenuation(uint16_t pos, uint8_t buff_indx, uint8_t DB) {
	int16_t tmp = spectrum[buff_indx][pos] - DB;
	spectrum[buff_indx][pos] = tmp < 0 ? 0 : tmp;
}

/**
  * @brief   Spectrum buzzer beep attenuation function
  * @note	 Buzzer adds high frequency noises to indicator sound wave
  * 			thus we are attenuating  noise frequencies
  * 			lying in range of 1.7 to 5.7kHz
  */
void beep_filtering(uint16_t pos, uint8_t buff_indx) {
	if ((pos == BEEP_FREQ1_POS) || (pos == BEEP_FREQ2_POS)) /* 440Hz & 880Hz 5 dB attenuation */
		Spectrum_attenuation(pos, spectrum_active_buffer, 5);
	if ((pos >= 89) && (pos <= 292)) 						/* HF buzzer noise 10 dB attenuation */
		Spectrum_attenuation(pos, spectrum_active_buffer, 10);
	spectrum[buff_indx][FFT_SPECTRUM_RES-1] = 0;
}

/**
  * @brief   Spectrum calculation function
  */
void FFT_process()
{
	arm_rfft_fast_f32(&fft_handler, fft_input, fft_output, 0);
	uint8_t spectrum_inactive_buffer = spectrum_active_buffer;
	uint16_t sp = 0;	/* Additional iterator for spectrum array */

	spectrum_active_buffer ^= 0x1;	/* Swap buffers */

	for (int i = 0; i < FFT_RESOLUTION; i = i + 2) {
		int16_t tmp = (int)(20*log10f(complexABS(fft_output[i], fft_output[i+1]))) - NOISE_FLOOR;
		spectrum[spectrum_active_buffer][sp] = (tmp < 0) || (sp > LOW_PASS_CUTOFF_FREQ_POS) ? 0 : tmp; /* Spectrum [dB] of incoming microphone signal */

		beep_filtering(sp, spectrum_active_buffer);

		if (spectrum[spectrum_active_buffer][sp] - spectrum[spectrum_inactive_buffer][sp] >= TRANSIENT_THR && (TIM5->CR1 & TIM_CR1_CEN) && TxData.Length == 0) {		/* Sound transient detector */
			TIM5->CR1 &= ~TIM_CR1_CEN;
#ifndef SPECTRUM_DEBUG
			UART_Latency_Send();
#else
			send_flag = READY_TO_SEND;
#endif
		}
		sp++;
	}
}
