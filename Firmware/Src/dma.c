#include "dma.h"

int16_t data_adc_buff[2][FFT_RESOLUTION];			/* Double buffer for ADC microphone recording and process */
uint8_t fft_buffer_process_flag;					/* Microphone ADC buffer flag */

extern int16_t spectrum[FFT_SPECTRUM_RES];			/* @main.c */
extern uint16_t waveform[DAC_RESOLUTION];			/* @dac.c */

/**
  * @brief   DMA stream for ADC recording buffering interrupt handler
  */
void DMA2_Stream0_IRQHandler()
{
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	fft_buffer_process_flag = FFT_BUFFER_FULL;
}

/**
  * @brief   DMA stream initialization for DAC conversion
  * @note		DAC_OUT2 is on channel 7 of DMA1_Stream6
  */
void DMA1_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	DMA1_Stream6->PAR = (uint32_t)(&DAC->DHR12R2);
	DMA1_Stream6->M0AR = (uint32_t)(waveform);
	DMA1_Stream6->NDTR = DAC_RESOLUTION;

	DMA1_Stream6->CR |= DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 | DMA_SxCR_CHSEL_2;	/* Channel 7 selected */
	DMA1_Stream6->CR |= DMA_SxCR_MINC;
	DMA1_Stream6->CR |= DMA_SxCR_MSIZE_0;
	DMA1_Stream6->CR |= DMA_SxCR_PSIZE_0;
	DMA1_Stream6->CR |= DMA_SxCR_CIRC;
	DMA1_Stream6->CR |= DMA_SxCR_DIR_0;

	DMA1_Stream6->CR |= DMA_SxCR_EN;
}

/**
  * @brief   DMA stream initialization for ADC recording buffering
  * @note		ФВС1 is on channel 0 of DMA2_Stream0
  */
void DMA2_init() {
	fft_buffer_process_flag = FFT_BUFFER_RESET;
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	DMA2_Stream0->CR |= DMA_SxCR_DBM;	// Double buffer node
	DMA2_Stream0->PAR = (uint32_t)(&ADC1->DR);
	DMA2_Stream0->M0AR = (uint32_t)(&data_adc_buff[0][0]);
	DMA2_Stream0->M1AR = (uint32_t)(&data_adc_buff[1][0]);
	DMA2_Stream0->NDTR = FFT_RESOLUTION;

	DMA2_Stream0->CR |= DMA_SxCR_TCIE;
	DMA2_Stream0->CR |= DMA_SxCR_MINC;
	DMA2_Stream0->CR |= DMA_SxCR_MSIZE_0;
	DMA2_Stream0->CR |= DMA_SxCR_PSIZE_0;

	DMA2_Stream0->CR |= DMA_SxCR_EN;
	NVIC_SetPriority(DMA2_Stream0_IRQn, 3);
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}
