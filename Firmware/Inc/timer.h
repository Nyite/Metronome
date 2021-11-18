#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"
#include "gpio.h"
#include "dac.h"
#include "usart.h"

#define APB1CLOCK 	16000000

#define LED_FLAG_RESET	0
#define LED_FLAG_SET	1
#define DEFAULT_TEMPO	120
#define DEFAULT_RHYTHM	0xF0000000

void set_Tempo(uint16_t tempo);
void set_default_Rhythm();
void TIM3_init();
void TIM4_init();
void TIM5_init();
void TIM6_init();
void TIM7_init();

#define	ONE_FORTH_MASK			0xF0000000
#define	ONE_EIGHTH_MASK			0xFF000000
#define	ONE_SIXTEENTH_MASK	 	0xFFFF0000
#define	ONE_THIRTY_SECOND_MASK 	0xFFFFFFFF

enum NOTE_LENGTH
{
	ONE_FORTH 			= 1,
	ONE_EIGHTH 			= 2,
	ONE_SIXTEENTH	 	= 4,
	ONE_THIRTY_SECOND 	= 8,
};

#endif
