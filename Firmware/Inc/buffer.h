#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "stm32f4xx.h"
#include "global.h"
#include "adc.h"

/* Buffer states */
#define BUFFER_OK		0	/* Buffer operation completed successfully */
#define BUFFER_FULL		1	/* Buffer is full and no data can inserted in it */
#define BUFFER_EMPTY	1	/* Buffer is empty and no data can be extracted from it */
#define BUFFER_NES 		1	/* Not enough space is current buffer in order to store all given data */

#define SPECTRUM_PKG_SIZE	FFT_SPECTRUM_RES + 1	/* FFT_SPECTRUM_RES/4 */

/* Buffer sizes */
#define RX_PACKAGE_SIZE		8
#define TX_PACKAGE_SIZE		SPECTRUM_PKG_SIZE

/**
  * @brief   Circular buffer typedef
  */
typedef struct {
	volatile uint8_t* DataBuff;
	uint16_t iStart;
	uint16_t iEnd;
	uint16_t Length;
	uint16_t Size;
} BUFFER;

/* Buffer initialization functions */
void RTxBuffer_Init();
void Buffer_Init(BUFFER* B, uint8_t* pArray, uint16_t BuffSize);

/* Buffer process functions */
void Buffer_Flush(BUFFER* B);
uint8_t Buffer_AddToEnd(BUFFER* B, uint8_t dataIn);
uint8_t Buffer_GetFromFront(BUFFER* B, uint8_t* dataOut);

/* Buffer overwrite functions */
uint8_t Buffer_StringOverwrite(BUFFER* B, char str[], uint8_t str_len);
uint8_t Buffer_LatencyOverwrite(BUFFER* B, uint16_t ltc);
uint8_t Buffer_SpectrumOverwrite(BUFFER* B, uint8_t* spectrum, uint16_t start);

#endif /* _BUFFER_H_ */
