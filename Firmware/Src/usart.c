#include "usart.h"

extern BUFFER RxData;
extern BUFFER TxData;

extern uint32_t rhythm_code;
extern uint8_t  bar_len;

void UART4_IRQHandler()
{
	if (UART4->SR & USART_SR_RXNE)
	{
		Buffer_AddToEnd(&RxData, UART4->DR);
		return;
	}
	if (UART4->SR & USART_SR_TXE)
		if (Buffer_GetFromFront(&TxData, &(UART4->DR)) == BUFFER_EMPTY)
		{
			UART4->CR1 &= ~USART_CR1_TXEIE;
			Buffer_Flush(&TxData);
		}
}

void UART4_init()
{
	RTxBuffer_Init();
	Buffer_Flush(&RxData);
	GPIOC_UART4_init();
	RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
	UART4_SET_BRR(BAUD);
	UART4->CR1 |= USART_CR1_TE;
	UART4->CR1 |= USART_CR1_RE;
	UART4->CR1 |= USART_CR1_RXNEIE;
	UART4->CR1 |= USART_CR1_UE;

	NVIC_SetPriority(UART4_IRQn, 3);
	NVIC_EnableIRQ(UART4_IRQn);
}

uint8_t TempoPkg_Validate(uint16_t *tempo)
{
	uint8_t val_flag = 0;
	// Tempo range check //
	*tempo = RxData.DataBuff[1] << 8 | RxData.DataBuff[2];
		if (*tempo < 30 || *tempo > 500)
			return TEMPO_OOR;
	bar_len = RxData.DataBuff[3]*4;
	rhythm_code = RxData.DataBuff[4] << 24 | RxData.DataBuff[5] << 16 | RxData.DataBuff[6] << 8 | RxData.DataBuff[7];
	// Note division & rhythm check //
	switch (RxData.DataBuff[3])
	{
		case ONE_FORTH:
			val_flag = rhythm_code & ONE_FORTH_MASK ? 1 : 0;
			break;
		case ONE_EIGHTH:
			val_flag = rhythm_code & ONE_EIGHTH_MASK ? 1 : 0;
			break;
		case ONE_SIXTEENTH:
			val_flag = rhythm_code & ONE_SIXTEENTH_MASK ? 1 : 0;
			break;
		case ONE_THIRTY_SECOND:
			val_flag = rhythm_code & ONE_THIRTY_SECOND_MASK ? 1 : 0;
			break;
	}
	if (!val_flag)
		return LENGTH_RHYTHM_ERR;
	return PKG_OK;
}

void pkg_Process()
{
	uint16_t tempo;
	switch (RxData.DataBuff[0])
	{
		case 'A':
			Buffer_StringOverwrite(&TxData, "Command", 7);
			break;
		case 'D':
			Buffer_StringOverwrite(&TxData, "Data", 5);
			break;
		case 'T':
			if (TempoPkg_Validate(&tempo) == PKG_OK)
			{
				set_Tempo(tempo);
				Buffer_StringOverwrite(&TxData, "Tmp_set", 7);
			}
			else
				Buffer_StringOverwrite(&TxData, "Tmp_err", 7);
			break;
		default:
			Buffer_StringOverwrite(&TxData, "Unknown", 7);
	}
}

void USART_Transive()
{
	// Process received package //
	if (RxData.Length == RxData.Size)
	{
		pkg_Process();
		Buffer_Flush(&RxData);
		UART4->CR1 |= USART_CR1_TXEIE;
	}
}

void UART_Latency_Send()
{
	Buffer_LatencyOverwrite(&TxData, TIM5->CNT);
	UART4->CR1 |= USART_CR1_TXEIE;
}
