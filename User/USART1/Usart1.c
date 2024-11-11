#include "Usart1.h"

_USART1GetData USART1_GetData;

void USART1_IRQHandler(void)
{
	uint8_t temp = '\0';
	
	if (USART1->SR & (0X1 << 5)) //收到数据
	{
		USART1->SR &= ~(0X1 << 5); // 清中断
		USART1_GetData.buf[USART1_GetData.count++] = USART1->DR;
	}
	
	if (USART1->SR & (0X1 << 4)) //检测到空闲总线
	{
		//清中断
		temp = temp;
		temp = USART1->SR;
		temp = USART1->DR;
		
		USART1_GetData.flag = 1;
	}
}

void USART1_Config(uint32_t BrrVal)
{
	//1）时钟使能
	RCC->APB2ENR |= (0x1 << 2); //GPIOA
	RCC->APB2ENR |= (0x1 << 14); //USART1
	
	//2）GPIO模式配置, PA9 接 USART1_TX 复用推挽输出, PA10 USART1_RX 浮空输入
	GPIOA->CRH &= ~(0xff << 4);
	GPIOA->CRH |= (0x4b << 4);
	
	//3）USART1 模式配置
		//8-0-1 协议
//	USART1->CR1 &= ~(0x1 << 12);
//	USART1->CR1 &= ~(0x1 << 10);
//	USART1->CR2 &= ~(0x3 << 12);
//		//全双工模式
//	USART1->CR1 |= (0x1 << 3);
//	USART1->CR1 |= (0x1 << 2);
//		//波特率
//	USART1->BRR = 72000000 / BrrVal;
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BrrVal;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStructure);
	
	//USART1的中断配置
	USART1->CR1 |= (0X1 << 5);//打开接收中断--中断源：USART1接收到1个字节数据
	USART1->CR1 |= (0X1 << 4);//打开空闲中断
	
	NVIC_SetPriority(USART1_IRQn, 5);//占先=1，次级=1(二进制 0101)；
	NVIC_EnableIRQ(USART1_IRQn);
	
	//4）USART1 使能
	USART1->CR1 |= (0X1 << 13);
}

//发送单个字节函数
void USART1_SendByte(uint8_t byte)
{
	while ((USART1->SR & (0X1 << 6)) == 0); //等上个数据发完
	USART1->DR = byte; //将 byte 放入 DR 寄存器
}

//发送字符串函数
void USART1_SendString(char * Str)
{
	while (*Str != '\0')
		USART1_SendByte(*Str++);
}

//发送缓冲区函数
void USART1_SendBuf(char * Str, uint8_t len)
{
	while (len--)
		USART1_SendByte(*Str++);
}

//printf 重定向
int fputc(int c, FILE * stream)
{
	USART1_SendByte(c);
	return c;
}

//阻塞性接收
uint8_t USART1_GetByte(void)
{
	while ((USART1->SR & (0X1 << 5)) == 0);
	return USART1->DR;
}
