#ifndef USART1_H
#define USART1_H

#include "stm32f10x.h"
#include "stdio.h"

typedef struct 
{
	uint8_t buf[32];
	uint8_t flag;
	uint32_t count;
}_USART1GetData;

extern _USART1GetData USART1_GetData;

void USART1_Config(uint32_t BrrVal);

void USART1_SendByte(uint8_t byte);

void USART1_SendString(char * Str);

void USART1_SendBuf(char * Str, uint8_t len);

uint8_t USART1_GetByte(void);
	
#endif
