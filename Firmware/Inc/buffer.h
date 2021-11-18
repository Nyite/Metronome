#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "stm32f4xx.h"

#define BUFFER_OK		0
// Error cases //
#define BUFFER_FULL		1
#define BUFFER_EMPTY	1
#define BUFFER_NES 		1	// Not enough space

#define RX_PACKAGE_SIZE	8
#define TX_PACKAGE_SIZE	8

typedef struct {
	volatile uint8_t* DataBuff;
	uint8_t iStart;
	uint8_t iEnd;
	uint8_t Length;
	uint8_t Size;
} BUFFER;

void RTxBuffer_Init();
void Buffer_Flush(BUFFER* B);
void Buffer_Init(BUFFER* B, uint8_t* pArray, uint8_t BuffSize);
uint8_t Buffer_AddToEnd(BUFFER* B, uint8_t dataIn);
uint8_t Buffer_GetFromFront(BUFFER* B, uint8_t* dataOut);
uint8_t Buffer_StringOverwrite(BUFFER* B, char str[], uint8_t str_len);
uint8_t Buffer_LatencyOverwrite(BUFFER* B, uint16_t ltc);

#endif
