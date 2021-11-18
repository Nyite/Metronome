#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"
#include "gpio.h"
#include "buffer.h"
#include "timer.h"

/*
 * Control package size in bytes
 * General package series: 	package type | 7 data bytes
 * Tempo package series:	T (0x54) | tempo (2 bytes MSB to LSB) | note duration | 4 bytes of rhythm setup
 * Note duration variants are listed in NOTE_LENGTH enum in timer.h
 */

#define BAUD 		115200
#define APB2CLOCK 	16000000

#define PKG_OK 		0
/*
 * Tempo package errors
 */
#define TEMPO_OOR			1	// Tempo out of range
#define LENGTH_RHYTHM_ERR	1	// Note length out of range | not supported rhythm sequence

#define USARTDIV_MANTISA_POS 		0x00000004

// Configures BRR register // Use ONLY AFTER OVER8 bit configuration //
#define UART4_SET_BRR(req_baud) \
	uint8_t over_state = (UART4->CR1 & USART_CR1_OVER8) >> USART_CR1_OVER8_Pos; \
	float div = (float)APB2CLOCK / (8 * (2 - over_state)*req_baud); \
	uint16_t mantissa = (uint16_t)div; \
	float fraction = 8 * (2 - over_state) * (div - mantissa); \
	uint16_t fraction_mantissa = (uint16_t)fraction; \
	if ((fraction - fraction_mantissa) > 0.5) { \
		++fraction_mantissa; \
		if (fraction_mantissa == 8 * (2 - over_state)) { \
			fraction_mantissa = 0; \
			++mantissa; \
		} \
	} \
	UART4->BRR = (mantissa << USARTDIV_MANTISA_POS) | fraction_mantissa;

void UART4_init();
void pkg_Process();
uint8_t TempoPkg_Validate();
void USART_Transive();
void UART_Latency_Send();

#endif
