#include "Lora.h"

//Loraģ��Ĵ����õ���USART2

/**
 * @brief Loraģ��MOD0��MOD1����GPIO��ʼ��
 */
void Lora_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = Lora_MOD0_Pin | Lora_MOD1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Lora_MOD0_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = Lora_AUX_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Lora_AUX_Port, &GPIO_InitStructure);
}

/**
 * @brief Lora ����ģʽѡ��
 * @param mode : NORMALMODE ����ģʽ ECOMODE ʡ��ģʽ WAKEMODE ����ģʽ SLEEPMODE ˯��ģʽ
 * @return �޷���ֵ
 */
void Lora_WorkMode(LORAMODE mode)
{
	Lora_Init();
	switch (mode)
	{
		case NORMALMODE : 
			GPIO_ResetBits(Lora_MOD0_Port, Lora_MOD0_Pin);
		  GPIO_ResetBits(Lora_MOD1_Port, Lora_MOD1_Pin);
			break;
		case ECOMODE : 
			GPIO_ResetBits(Lora_MOD0_Port, Lora_MOD0_Pin);
		 	GPIO_SetBits(Lora_MOD1_Port, Lora_MOD1_Pin);
			break;
		case WAKEMODE : 
			GPIO_SetBits(Lora_MOD0_Port, Lora_MOD0_Pin);
		 	GPIO_ResetBits(Lora_MOD1_Port, Lora_MOD1_Pin);
			break;
		case SLEEPMODE : 
			GPIO_SetBits(Lora_MOD0_Port, Lora_MOD0_Pin);
		 	GPIO_SetBits(Lora_MOD1_Port, Lora_MOD1_Pin);
			break;
	}
}

/**
 * @brief Lora ���㴫��ģʽ����
 * @param csmode : ��Ҫ���õĴ���ģʽ
 * @param ADDH : ģ���ַ�߰�λ
 * @param ADDL : ģ���ַ�Ͱ�λ
 * @param Channel : ��Ҫ���õ��ŵ�
 * @return �ɹ�����1��ʧ�ܷ���0
 */
uint8_t Lora_CSMode(LORACSMODE csmode, uint8_t ADDH, uint8_t ADDL, uint8_t Channel)
{
	uint8_t SetBuf[6] = {0}; 
	
	SetBuf[0] = 0xC0; //�̶���ʽ,���õ���ɱ���
	SetBuf[1] = ADDH; //ģ���ַ�߰�λ
	SetBuf[2] = ADDL; //ģ���ַ�Ͱ�λ
	SetBuf[3] = 0x1A; //SPEEDͨ�����üĴ�����00011010 ���ڸ�ʽ8N1(Ĭ�� N-��У��λ) ������9600(Ĭ��) ��������2.4k(Ĭ��)
	SetBuf[4] = Channel; //CHAN�ŵ��Ĵ�����Ĭ��0x17(433MHz) 0x00(410MHz) 0x1F(441MHz)
	if (csmode == TM)
		SetBuf[5] = 0x40; //OPTION����Ĵ�����01000000 ͸������(Ĭ��) TXD��AUX���������RXD��������(Ĭ��) ���޻���ʱ��250ms(Ĭ��) ���书��20dBm(Ĭ��)
	else if (csmode == P2P)
		SetBuf[5] = 0xC0; //OPTION����Ĵ�����11000000 ���㴫�� TXD��AUX���������RXD��������(Ĭ��) ���޻���ʱ��250ms(Ĭ��) ���书��20dBm(Ĭ��)
	
	Lora_WorkMode(SLEEPMODE); //ָ������֮ǰ��Ҫ�л�Ϊ˯��ģʽ
	
	uint8_t count = 10;
	while (count--)
	{
		USART2_SendBuf((char *)SetBuf, 6);
		if (strstr((const char *)USART2_GetData.data, "OK") != NULL)
		{
			Lora_WorkMode(NORMALMODE); //���óɹ��л�Ϊ��������ģʽ
			memset(USART2_GetData.data,0,255);
			
			return 1;
		}
		
		Delay_nopnms(100);
	}
	
	return 0;
}

/**
 * @brief Lora ���㴫��ģʽ����
 * @param ADDH : ģ���ַ�߰�λ
 * @param ADDL : ģ���ַ�Ͱ�λ
 * @param Channel : ��Ҫ���õ��ŵ�
 * @param str : ��Ҫ���͵�����
 * @return �ɹ�����1��ʧ�ܷ���0
 */
uint8_t Lora_P2PSend(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, char *str)
{
	char SendBuf[3] = {0};
	SendBuf[0] = ADDH;
	SendBuf[1] = ADDL;
	SendBuf[2] = Channel;
	
//	USART2_SendBuf(SendBuf, 3); //����Ŀ���ַ���ŵ�
//	USART2_SendBuf(str, strlen(str)); //��������
//	
//	return 1;
	
	while (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET) //����ǰ�ж�AUX����״̬��AUX���ڵ͵�ƽ����æ
	{
		Delay_nopnms(2);
		if (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET)//AUX���ڸߵ�ƽ2ms
		{
			USART2_SendBuf(SendBuf, 3); //����Ŀ���ַ���ŵ�
			USART2_SendBuf(str, strlen(str)); //��������
			
			while (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET) //���ͺ��ж�AUX����״̬��AUX���ڸߵ�ƽ���������
				return 1;
		}
	}
}

/**
 * @brief Lora ͸������ģʽ����
 * @param str : ��Ҫ���͵�����
 * @return �ɹ�����1��ʧ�ܷ���0
 */
uint8_t Lora_TMSend(char *str)
{
	while (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET)
	{
		Delay_nopnms(2);
		if (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET)
		{
			USART2_SendBuf(str, sizeof(str));
			
			if (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET)
				return 1;
		}
	}
}