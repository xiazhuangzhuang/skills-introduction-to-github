#include "USART.h"
#include <string.h>
#include "stdio.h"

USART_RecData USART1_RecData;
USART_RecData USART2_RecData;

void USART1_Init(uint32_t BaudRate)
{
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA和USART1的时钟

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);  //发送
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);  //接收
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //发送模式和接受模式同时开启
	USART_InitStruct.USART_BaudRate = BaudRate; //波特率 
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Parity = USART_Parity_No;  //校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1; //停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b; //字长 无校验就选择8位即可
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE); //开启使能
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, ENABLE);	//开启串口中断

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}

void USART2_Init(uint32_t Baud)
{
    RCC_APB2PeriphClockCmd(USART2_PeriphPort,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    GPIO_InitTypeDef GPIO_Struct;
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;      //复用推挽
    GPIO_Struct.GPIO_Pin = USART2_TXPin;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART2_Port,&GPIO_Struct);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Struct.GPIO_Pin = USART2_RXPin;
    GPIO_Init(USART2_Port,&GPIO_Struct);

    USART_InitTypeDef USART_STruct;
    USART_STruct.USART_BaudRate = Baud;
    USART_STruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁用硬件流
    USART_STruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //开启接收和发送
    USART_STruct.USART_Parity = USART_Parity_No; //无校验
    USART_STruct.USART_StopBits = USART_StopBits_1; //停止位为1
    USART_STruct.USART_WordLength = USART_WordLength_8b; //数据位8
    USART_Init(USART2,&USART_STruct);
    USART_Cmd(USART2,ENABLE); //USART2使能

    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); 
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//开启接收数据寄存器不为空中断和空闲线路检测中断
    //配置NVIC
    NVIC_InitTypeDef NVIC_Struct;
    NVIC_Struct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_Struct.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
    NVIC_Struct.NVIC_IRQChannelSubPriority = 0;        //子优先级
	NVIC_Init(&NVIC_Struct);
}


/**
 * @description: USART串口发送字符串 
 * @param {USART_TypeDef} *USARTx 选择要发送字符串的串口
 * @param {unsigned char} *str 要发送的字符串
 * @return {*}
 */
void USART_SendString(USART_TypeDef *USARTx, char *str)
{
   	while(*str != '\0') //一直循环 直到遇到字符串的结尾'\0'
    {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}


/**
 * @description: USART串口发送字符串,可以发送'\0' 
 * @param {USART_TypeDef} *USARTx 选择要发送字符串的串口
 * @param {unsigned char} *str 要发送的字符串
 * @param {uint8_t} len 要发送的字符串的长度
 * @return {*}
 */
void USART_SendNString(USART_TypeDef *USARTx, char *str,uint8_t len)
{
   	while(len--)
    {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

/**
 * @description: USART串口发送字符串数组 
 * @param {USART_TypeDef} *USARTx 选择要发送字符串的串口
 * @param {unsigned char} *str 要发送的字符串
 * @param {uint8_t} len 要发送的字符串数组的长度
 * @return {*}
 */
void USART_SendAString(USART_TypeDef *USARTx, char *arr,uint8_t len)
{
   	for (uint8_t i = 0; i < len; i++)
    {
        USART_SendData(USARTx, arr[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }  
}
/**
 * printf的重定义，printf函数内的内容会被发送到串口1
*/
int fputc(int ch,FILE *stream)
{

	while( !(USART1->SR & (1<<6)));	//等待数据发送完成
	USART1->DR = ch;

	return 0;
}



void USART1_IRQHandler()
{
    if(USART1->SR & (1 << 5)) //判断RXNE是否为1 以此来判断是否进入接收中断
    {
        USART1->SR &= ~(1 << 5);//清中断 //清除RXNE中断标志位
        USART1_RecData.USART_RXBUF[USART1_RecData.Count] = USART1->DR;
        USART1_RecData.Count++;
    }
    if(USART1->SR & (1 << 4)) //判断IDLE是否为1 以此来判断是否进入空闲状态
    {
		USART1->SR;
		USART1->DR; //清除标志位
        USART1_RecData.Flag = 1;
        memcpy(USART1_RecData.USART_RxData,USART1_RecData.USART_RXBUF,255);
        memset(USART1_RecData.USART_RXBUF,0,255);
		USART1_RecData.Count = 0;
    }
}

void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //判断RXNE是否为1 以此来判断是否进入接收中断
    {
        USART2_RecData.USART_RXBUF[USART2_RecData.Count] = USART2->DR;
        USART2_RecData.Count++;
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);//清除RXNE中断标志位
    }
	else if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //判断IDLE是否为1 以此来判断是否进入空闲状态
    {
        USART2->SR;
        USART2->DR;//清除空闲中断标志位
        USART2_RecData.Flag = 1;
//		memcpy(USART2_RecData.USART_RxData,USART2_RecData.USART_RXBUF,255);
//		memset(USART2_RecData.USART_RXBUF,0,255);
		USART2_RecData.Count = 0;
    }
}


