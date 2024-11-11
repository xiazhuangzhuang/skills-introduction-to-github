#include "Usart2.h"

_USART2GetData USART2_GetData = {.flag = 0};

void USART2_IRQHandler(void)
{
	//接收中断
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE))//收到数据触发中断为1
	{
		USART_ClearFlag(USART2,USART_FLAG_RXNE);//清中断
		USART2_GetData.buf[USART2_GetData.count++] = USART_ReceiveData(USART2);
	}
	
	//空闲中断
	if (USART_GetFlagStatus(USART2, USART_FLAG_IDLE))//产生中断为1
	{
		//M3库函数没有提供空闲中断的清除函数
//    USART_ClearFlag(USART2, USART_FLAG_IDLE);
			USART2->SR;
			USART2->DR;
		
		//USART2_GetData内的buf有接收到count个字节数据
		USART2_GetData.flag = 1;
		
		memcpy(USART2_GetData.data,USART2_GetData.buf,256);
		memset(USART2_GetData.buf,0,256);
		USART2_GetData.count = 0;
	}
}

void USART2_Config(uint32_t BrrVal)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
  //1）时钟使能--GPIOA(PA2-TX,PA3-RX)、USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//2）配置GPIO
  //PA2--复用推挽输出
  //PA3--浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//3）USART2 模式配置
	USART_InitStructure.USART_BaudRate = BrrVal;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2, &USART_InitStructure);
	
	//USART2的中断配置
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//打开接收中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//打开空闲中断
	
	NVIC_SetPriority(USART2_IRQn, 0);//占先=0，次级=0(二进制 0000)；
	NVIC_EnableIRQ(USART2_IRQn);
	
	//4）USART2 使能
	USART_Cmd(USART2, ENABLE);
}

//发送单个字节函数
void USART2_SendByte(uint8_t byte)
{	
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //等上个数据发完
	USART_SendData(USART2, byte); //将 byte 放入 DR 寄存器
}

//发送字符串函数
void USART2_SendString(char * Str)
{
	while (*Str != '\0')
		USART2_SendByte(*Str++);
}

//发送缓冲区函数
void USART2_SendBuf(char * Str, uint8_t len)
{
	while (len--)
	{
		USART2_SendByte(*Str++);
	}
}

//阻塞性接收
uint8_t USART2_GetByte(void)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == 0);
	return USART1->DR;
}
