#include "dma.h"

int16_t data_adc_buff[2][FFT_RESOLUTION];
uint8_t fft_buffer_process_flag;

extern int16_t spectrum[FFT_SPECTRUM_RES];
extern uint16_t waveform[DAC_RESOLUTION];

void DMA2_Stream0_IRQHandler()
{
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	fft_buffer_process_flag = FFT_BUFFER_FULL;
}

// DAC_OUT2 is on stream 6 of channel 7 of DMA1 //
void DMA1_init() {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;

	DMA1_Stream6->PAR = (uint32_t)(&DAC->DHR12R2);
	DMA1_Stream6->M0AR = (uint32_t)(waveform);
	DMA1_Stream6->NDTR = DAC_RESOLUTION;

	DMA1_Stream6->CR |= DMA_SxCR_CHSEL_0 | DMA_SxCR_CHSEL_1 | DMA_SxCR_CHSEL_2; // Channel 7 selected
	DMA1_Stream6->CR |= DMA_SxCR_MINC;
	DMA1_Stream6->CR |= DMA_SxCR_MSIZE_0;
	DMA1_Stream6->CR |= DMA_SxCR_PSIZE_0;
	DMA1_Stream6->CR |= DMA_SxCR_CIRC;
	DMA1_Stream6->CR |= DMA_SxCR_DIR_0;

	DMA1_Stream6->CR |= DMA_SxCR_EN;
}

// ADC1 is on stream 0 of channel 0 of DMA2 //
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
