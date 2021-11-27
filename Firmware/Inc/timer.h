#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f4xx.h"
#include "global.h"
#include "math.h"
#include "uart.h"
#include "gpio.h"
#include "dac.h"

#define BUTTON_SET		1
#define BUTTON_RESET	0

/* Set functions */
void set_Tempo(uint16_t tempo);

/* Timer initialization functions */
void TIM2_init();
void TIM3_init();
void TIM4_init();
void TIM5_init();
void TIM6_init();
void TIM7_init();

#endif /* _TIMER_H_ */
