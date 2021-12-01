#include "timer.h"

uint32_t rhythm_code;					/* rhythm register representing bool state for each note in a bar */
uint8_t  bar_len;						/* Musical bar length */

extern uint8_t uart_sp_cnt;				/* @uart.c */
/**
  * @brief   OPM timer interrupt handler to generate
  * 		 short beep and LED blink on metronome tick
  */
void TIM7_IRQHandler() {
	TIM7->SR &= ~TIM_SR_UIF;			/* End LED beep   */
	TIM6->CR1 &= ~TIM_CR1_CEN;			/* End sound beep */
	GPIOD_RESET_PIN(GPIOD_PIN_12);
}

/**
  * @brief   Main metronome timer interrupt handler
  * @note	 rhythm_buff & rhythm_code - rhythm registers
  * 			representing bool state for each note in a bar
  */
void TIM4_IRQHandler() {
	static uint8_t cnt_skip = 0;		/* Timer frequency divide counter */
	static uint8_t bar_indx = 0;		/* Current note position in bar */
	static uint32_t rhythm_buff = DEFAULT_RHYTHM;

	TIM4->SR &= ~TIM_SR_UIF;
	if (++cnt_skip >= 60) {				/* Conversion from beats per second to beats per minute */
		if (rhythm_buff & 0x80000000) {
			if (bar_indx == 0)			/* First note of the bar is set to 2x higher frequency */
				TIM6->PSC = 1-1;
			else
				TIM6->PSC = 2-1;
			GPIOD_SET_PIN(GPIOD_PIN_12);/* Start LED blink 	*/
			TIM7->CR1 |= TIM_CR1_CEN;	/* Start OPM timer to end beep and blink */
			TIM6->CR1 |= TIM_CR1_CEN;	/* Start sound beep conversion */
		}
		rhythm_buff = rhythm_buff << 1; /* Bar loop */
		if (++bar_indx >= bar_len) {
			bar_indx = 0;
			rhythm_buff = rhythm_code;
		}
		cnt_skip = 0;
		TIM5->CNT = 0;					/* Latency timer restart */
		TIM5->CR1 |= TIM_CR1_CEN;
	}
}

/**
  * @brief   Metronome button handler
  */
void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;
	if (GPIOA->IDR & GPIO_IDR_IDR_0) {
		if (DAC->CR & DAC_CR_EN2)
			DAC->CR &= ~DAC_CR_EN2;
		else
			DAC->CR |= DAC_CR_EN2;
	}
}

/**
  * @brief   Metronome tempo set function
  * 			calculates appropriate ARR value for choosen bpm
  */
void set_Tempo(uint16_t tempo) {
	TIM4->ARR = (uint32_t)(round((float)APB1CLOCK/(TIM4->PSC+1)/tempo/(bar_len/4))-1);
}

/**
  * @brief   Metronome sound indication contact bounce timer initialization
  */
void TIM2_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CR1 |= TIM_CR1_OPM;
	TIM2->PSC = 1600-1;					/* 100ms delay */
	TIM2->ARR = 1000-1;
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM2_IRQn, 7);
	NVIC_EnableIRQ(TIM2_IRQn);
}

/**
  * @brief   ADC conversion master timer
  * @note	 Sample frequency here is set to SAMPLE_FREQ
  */
void TIM3_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->PSC = 20-1;
	TIM3->ARR = (uint32_t)(round((float)APB1CLOCK/(TIM3->PSC+1)/SAMPLE_FREQ)-1);
	TIM3->CR2 |= TIM_CR2_MMS_1;
	TIM3->CR1 |= TIM_CR1_CEN;
}

/**
  * @brief   Main metronome timer
  * @note	 TIM4 frequency is 60x beats per minute  (i.e. beats per second)
  */
void TIM4_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->PSC = 10-1;
	bar_len = DEFAULT_BAR_LEN;
	rhythm_code = DEFAULT_RHYTHM;
	set_Tempo(DEFAULT_TEMPO);
	TIM4->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM4_IRQn, 5);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4->CR1 |= TIM_CR1_CEN;
}

/**
  * @brief   Latency timer
  * @note	 Latency here is counted in microseconds
  */
void TIM5_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->CR1 |= TIM_CR1_ARPE;
	TIM5->PSC = 16000-1;
	TIM5->EGR |= TIM_EGR_UG;
}

/**
  * @brief   DAC conversion master timer
  * @note	 Beep frequency here is set to BEEP_FREQ
  */
void TIM6_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->CR1 |= TIM_CR1_ARPE;
	TIM6->PSC = 2-1;
	TIM6->ARR = (uint32_t)(round((float)APB1CLOCK/(TIM6->PSC+1)/DAC_RESOLUTION/BEEP_FREQ)-1);
	TIM6->CR2 |= TIM_CR2_MMS_1;
}

/**
  * @brief   OPM timer to generate
  * 		 short beep and LED blink on metronome tick
  */
void TIM7_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	TIM7->CR1 |= TIM_CR1_ARPE;
	TIM7->CR1 |= TIM_CR1_OPM;
	TIM7->PSC = 160-1;
	TIM7->ARR = 5000-1;
	TIM7->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM7_IRQn, 7);
	NVIC_EnableIRQ(TIM7_IRQn);
}
