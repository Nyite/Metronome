#ifndef _UART_H_
#define _UART_H_

#include "stm32f4xx.h"
#include "global.h"
#include "math.h"
#include "gpio.h"
#include "buffer.h"
#include "timer.h"

/**
  * @brief   General UART package series: 	package type | upto 7 data bytes
  * @note	 Tempo package series:	T (0x54) | tempo (2 bytes MSB to LSB) | note duration | 4 bytes of rhythm register
  * 			Note duration variants are listed in NOTE_LENGTH enum in timer.h
  * @note	 Latency package series: L(0x4C) | Latency (2 bytes MSB to LSB)
  */

/* Rhythm encoding mask used to check for rhythm existence */
#define	ONE_FORTH_MASK			0xF0000000
#define	ONE_EIGHTH_MASK			0xFF000000
#define	ONE_SIXTEENTH_MASK	 	0xFFFF0000
#define	ONE_THIRTY_SECOND_MASK 	0xFFFFFFFF

/* Received tempo package states */
#define PKG_OK 				0	/* Package received correctly */
#define TEMPO_OOR			1	/* Received tempo is out of range */
#define LENGTH_RHYTHM_ERR	1	/* Received note length out of range or rhythm sequence is not supported */

#define USARTDIV_MANTISA_POS 		0x00000004

/* BRR register macro // Use ONLY AFTER OVER8 bit configuration */
#define UART_SET_BRR(src, req_baud) \
	uint8_t over_state = (src->CR1 & USART_CR1_OVER8) >> USART_CR1_OVER8_Pos; \
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
	src->BRR = (mantissa << USARTDIV_MANTISA_POS) | fraction_mantissa;

/* UART initialization functions */
void UART4_init();

/* UART transceiver functions */
void pkg_Process();
uint8_t TempoPkg_Validate();
void USART_Transive();
void UART_Latency_Send();

#endif /* _UART_H_ */
