#ifndef USART2_H
#define USART2_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

typedef struct 
{
	uint8_t buf[256];
	uint8_t flag;
	uint32_t count;
	uint8_t data[256];
	
}_USART2GetData;

extern _USART2GetData USART2_GetData;

void USART2_Config(uint32_t BrrVal);

void USART2_SendByte(uint8_t byte);

void USART2_SendString(char * Str);

void USART2_SendBuf(char * Str, uint8_t len);

uint8_t USART2_GetByte(void);
	
#endif
