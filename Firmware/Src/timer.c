#include "timer.h"

extern uint16_t waveform[DAC_RESOLUTION];

uint32_t rhythm_code;
uint8_t  bar_len;
uint8_t click_flag;

void TIM7_IRQHandler() {
	TIM7->SR &= ~TIM_SR_UIF;	// End LED beep   //
	TIM6->CR1 &= ~TIM_CR1_CEN;	// End sound beep //
	GPIOD_RESET_PIN(GPIOD_PIN_12);
}

//void TIM5_IRQHandler() {
//	TIM5->SR &= ~TIM_SR_UIF;	// End LED beep   //
//	GPIOD_TOGGLE_PIN(GPIOD_PIN_14);
//}

void TIM4_IRQHandler() {
	static uint8_t cnt_skip = 0;
	static uint8_t bar_indx = 0;
	static uint32_t rhythm_buff = DEFAULT_RHYTHM;

	TIM4->SR &= ~TIM_SR_UIF;
	if (++cnt_skip >= 60) {
		click_flag = 1;
		TIM5->CNT = 0;
		TIM5->CR1 |= TIM_CR1_CEN;
		if (rhythm_buff & 0x80000000) {
			if (bar_indx == 0)
				TIM6->PSC = 1-1;
			else
				TIM6->PSC = 2-1;
			GPIOD_SET_PIN(GPIOD_PIN_12);
			TIM7->CR1 |= TIM_CR1_CEN;	// Start LED beep 	//
			TIM6->CR1 |= TIM_CR1_CEN;	// Start sound beep //
		}
		rhythm_buff = rhythm_buff << 1;
		if (++bar_indx >= bar_len) {
			bar_indx = 0;
			rhythm_buff = rhythm_code;
		}
		cnt_skip = 0;
	}
}

void set_Tempo(uint16_t tempo) {
	float div = (float)APB1CLOCK/(TIM4->PSC+1)/tempo/(bar_len/4);
	uint16_t tmp = (uint16_t)div;
	TIM4->ARR = (div-tmp) > 0.5 ? (tmp+1) : tmp;
}

void set_default_Rhythm() {
	bar_len = ONE_FORTH*4;
	rhythm_code = DEFAULT_RHYTHM;
}

// ADC conversion timer //
void TIM3_init()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 |= TIM_CR1_ARPE;
	TIM3->PSC = 20-1;
	TIM3->ARR = 30-1;
	TIM3->CR2 |= TIM_CR2_MMS_1;

	TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM4_init() {
	click_flag = 0;

	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->PSC = 10-1;
	set_default_Rhythm();
	set_Tempo(DEFAULT_TEMPO);
	TIM4->DIER |= TIM_DIER_UIE;
	NVIC_SetPriority(TIM4_IRQn, 5);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4->CR1 |= TIM_CR1_CEN;
}

// Latency timer //
void TIM5_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->CR1 |= TIM_CR1_ARPE;
	TIM5->PSC = 16000-1;
	TIM5->EGR |= TIM_EGR_UG;
//	TIM5->CR1 |= TIM_CR1_CEN;
}

// DAC conversion timer //
void TIM6_init() {
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->CR1 |= TIM_CR1_ARPE;
	// 440 Hz //
	TIM6->PSC = 2-1;
	TIM6->ARR = 71-1;
	TIM6->CR2 |= TIM_CR2_MMS_1;
//	TIM6->CR1 |= TIM_CR1_CEN;	// Start sound beep //
}

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
