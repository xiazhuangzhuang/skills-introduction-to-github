#ifndef __USART__H_
#define __USART__H_
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#define USART2_PeriphPort RCC_APB2Periph_GPIOA
#define USART2_Port GPIOA
#define USART2_TXPin GPIO_Pin_2
#define USART2_RXPin GPIO_Pin_3

typedef struct{
    char USART_RXBUF[255];
    uint8_t Count;
    uint8_t Flag;
    char USART_RxData[255];
}USART_RecData;

//创建结构体对象
extern USART_RecData USART1_RecData;
extern USART_RecData USART2_RecData;

void USART1_Init(uint32_t Baud);
void USART_SendString(USART_TypeDef *USARTx, char *str);
void USART_SendNString(USART_TypeDef *USARTx, char *str,uint8_t len);
void USART_SendAString(USART_TypeDef *USARTx, char *arr,uint8_t len);
void USART2_Init(uint32_t Baud);

#endif // !__USART__H_

