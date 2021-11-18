#include "buffer.h"

BUFFER RxData;
BUFFER TxData;

uint8_t RxDataArray[RX_PACKAGE_SIZE];
uint8_t TxDataArray[TX_PACKAGE_SIZE];

void RTxBuffer_Init()
{
	Buffer_Init(&RxData, RxDataArray, RX_PACKAGE_SIZE);
	Buffer_Init(&TxData, TxDataArray, TX_PACKAGE_SIZE);
}

void Buffer_Flush(BUFFER* B)
{
	B->iStart 	= 0;
	B->iEnd 	= 0;
	B->Length 	= 0;
}

void Buffer_Init(BUFFER* B, uint8_t* pArray, uint8_t BuffSize)
{
	B->DataBuff = pArray;
	B->iStart 	= 0;
	B->iEnd 	= 0;
	B->Length 	= 0;
	B->Size 	= BuffSize;
}

uint8_t Buffer_AddToEnd(BUFFER* B, uint8_t dataIn)
{
	if(B->Length >= B->Size)
		return BUFFER_FULL;
	B->DataBuff[B->iEnd] = dataIn;
	++B->iEnd;
	++B->Length;
	if(B->iEnd > B->Size-1)
		B->iEnd = 0;
	return BUFFER_OK;
}

uint8_t Buffer_GetFromFront(BUFFER* B, uint8_t* dataOut)
{
	if(B->Length == 0)
		return BUFFER_EMPTY;
	*dataOut = B->DataBuff[B->iStart];
	++B->iStart;
	--B->Length;
	if(B->iStart > B->Size)
		B->iStart = 0;
	return BUFFER_OK;
}

uint8_t Buffer_StringOverwrite(BUFFER* B, char str[], uint8_t str_len)
{
	if (str_len > B->Size)
		return BUFFER_NES;
	for (int i = 0; i < str_len; i++)
		B->DataBuff[i] = str[i];
	B->iEnd 	= str_len;
	B->Length 	= str_len;
	return BUFFER_OK;
}

uint8_t Buffer_LatencyOverwrite(BUFFER* B, uint16_t ltc)
{
	B->DataBuff[0] = 'L';
	B->DataBuff[1] = ltc >> 8;
	B->DataBuff[2] = ltc;
	B->iEnd 	= 3;
	B->Length 	= 3;
	return BUFFER_OK;
}
