#include "Usart1.h"

_USART1GetData USART1_GetData;

void USART1_IRQHandler(void)
{
	uint8_t temp = '\0';
	
	if (USART1->SR & (0X1 << 5)) //�յ�����
	{
		USART1->SR &= ~(0X1 << 5); // ���ж�
		USART1_GetData.buf[USART1_GetData.count++] = USART1->DR;
	}
	
	if (USART1->SR & (0X1 << 4)) //��⵽��������
	{
		//���ж�
		temp = temp;
		temp = USART1->SR;
		temp = USART1->DR;
		
		USART1_GetData.flag = 1;
	}
}

void USART1_Config(uint32_t BrrVal)
{
	//1��ʱ��ʹ��
	RCC->APB2ENR |= (0x1 << 2); //GPIOA
	RCC->APB2ENR |= (0x1 << 14); //USART1
	
	//2��GPIOģʽ����, PA9 �� USART1_TX �����������, PA10 USART1_RX ��������
	GPIOA->CRH &= ~(0xff << 4);
	GPIOA->CRH |= (0x4b << 4);
	
	//3��USART1 ģʽ����
		//8-0-1 Э��
//	USART1->CR1 &= ~(0x1 << 12);
//	USART1->CR1 &= ~(0x1 << 10);
//	USART1->CR2 &= ~(0x3 << 12);
//		//ȫ˫��ģʽ
//	USART1->CR1 |= (0x1 << 3);
//	USART1->CR1 |= (0x1 << 2);
//		//������
//	USART1->BRR = 72000000 / BrrVal;
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BrrVal;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStructure);
	
	//USART1���ж�����
	USART1->CR1 |= (0X1 << 5);//�򿪽����ж�--�ж�Դ��USART1���յ�1���ֽ�����
	USART1->CR1 |= (0X1 << 4);//�򿪿����ж�
	
	NVIC_SetPriority(USART1_IRQn, 5);//ռ��=1���μ�=1(������ 0101)��
	NVIC_EnableIRQ(USART1_IRQn);
	
	//4��USART1 ʹ��
	USART1->CR1 |= (0X1 << 13);
}

//���͵����ֽں���
void USART1_SendByte(uint8_t byte)
{
	while ((USART1->SR & (0X1 << 6)) == 0); //���ϸ����ݷ���
	USART1->DR = byte; //�� byte ���� DR �Ĵ���
}

//�����ַ�������
void USART1_SendString(char * Str)
{
	while (*Str != '\0')
		USART1_SendByte(*Str++);
}

//���ͻ���������
void USART1_SendBuf(char * Str, uint8_t len)
{
	while (len--)
		USART1_SendByte(*Str++);
}

//printf �ض���
int fputc(int c, FILE * stream)
{
	USART1_SendByte(c);
	return c;
}

//�����Խ���
uint8_t USART1_GetByte(void)
{
	while ((USART1->SR & (0X1 << 5)) == 0);
	return USART1->DR;
}
