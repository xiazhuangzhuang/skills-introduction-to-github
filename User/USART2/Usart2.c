#include "Usart2.h"

_USART2GetData USART2_GetData = {.flag = 0};

void USART2_IRQHandler(void)
{
	//�����ж�
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE))//�յ����ݴ����ж�Ϊ1
	{
		USART_ClearFlag(USART2,USART_FLAG_RXNE);//���ж�
		USART2_GetData.buf[USART2_GetData.count++] = USART_ReceiveData(USART2);
	}
	
	//�����ж�
	if (USART_GetFlagStatus(USART2, USART_FLAG_IDLE))//�����ж�Ϊ1
	{
		//M3�⺯��û���ṩ�����жϵ��������
//    USART_ClearFlag(USART2, USART_FLAG_IDLE);
			USART2->SR;
			USART2->DR;
		
		//USART2_GetData�ڵ�buf�н��յ�count���ֽ�����
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
	
  //1��ʱ��ʹ��--GPIOA(PA2-TX,PA3-RX)��USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//2������GPIO
  //PA2--�����������
  //PA3--��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//3��USART2 ģʽ����
	USART_InitStructure.USART_BaudRate = BrrVal;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2, &USART_InitStructure);
	
	//USART2���ж�����
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�򿪽����ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�򿪿����ж�
	
	NVIC_SetPriority(USART2_IRQn, 0);//ռ��=0���μ�=0(������ 0000)��
	NVIC_EnableIRQ(USART2_IRQn);
	
	//4��USART2 ʹ��
	USART_Cmd(USART2, ENABLE);
}

//���͵����ֽں���
void USART2_SendByte(uint8_t byte)
{	
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //���ϸ����ݷ���
	USART_SendData(USART2, byte); //�� byte ���� DR �Ĵ���
}

//�����ַ�������
void USART2_SendString(char * Str)
{
	while (*Str != '\0')
		USART2_SendByte(*Str++);
}

//���ͻ���������
void USART2_SendBuf(char * Str, uint8_t len)
{
	while (len--)
	{
		USART2_SendByte(*Str++);
	}
}

//�����Խ���
uint8_t USART2_GetByte(void)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == 0);
	return USART1->DR;
}
