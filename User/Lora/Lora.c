#include "Lora.h"

//Lora模块的串口用到了USART2

/**
 * @brief Lora模块MOD0和MOD1引脚GPIO初始化
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
 * @brief Lora 工作模式选择
 * @param mode : NORMALMODE 正常模式 ECOMODE 省电模式 WAKEMODE 唤醒模式 SLEEPMODE 睡眠模式
 * @return 无返回值
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
 * @brief Lora 定点传输模式设置
 * @param csmode : 想要设置的传输模式
 * @param ADDH : 模块地址高八位
 * @param ADDL : 模块地址低八位
 * @param Channel : 想要设置的信道
 * @return 成功返回1，失败返回0
 */
uint8_t Lora_CSMode(LORACSMODE csmode, uint8_t ADDH, uint8_t ADDL, uint8_t Channel)
{
	uint8_t SetBuf[6] = {0}; 
	
	SetBuf[0] = 0xC0; //固定格式,配置掉电可保存
	SetBuf[1] = ADDH; //模块地址高八位
	SetBuf[2] = ADDL; //模块地址低八位
	SetBuf[3] = 0x1A; //SPEED通信配置寄存器：00011010 串口格式8N1(默认 N-无校验位) 波特率9600(默认) 空中速率2.4k(默认)
	SetBuf[4] = Channel; //CHAN信道寄存器：默认0x17(433MHz) 0x00(410MHz) 0x1F(441MHz)
	if (csmode == TM)
		SetBuf[5] = 0x40; //OPTION特殊寄存器：01000000 透明传输(默认) TXD、AUX推挽输出，RXD上拉输入(默认) 无限唤醒时间250ms(默认) 发射功率20dBm(默认)
	else if (csmode == P2P)
		SetBuf[5] = 0xC0; //OPTION特殊寄存器：11000000 定点传输 TXD、AUX推挽输出，RXD上拉输入(默认) 无限唤醒时间250ms(默认) 发射功率20dBm(默认)
	
	Lora_WorkMode(SLEEPMODE); //指令设置之前需要切换为睡眠模式
	
	uint8_t count = 10;
	while (count--)
	{
		USART2_SendBuf((char *)SetBuf, 6);
		if (strstr((const char *)USART2_GetData.data, "OK") != NULL)
		{
			Lora_WorkMode(NORMALMODE); //设置成功切换为正常工作模式
			memset(USART2_GetData.data,0,255);
			
			return 1;
		}
		
		Delay_nopnms(100);
	}
	
	return 0;
}

/**
 * @brief Lora 定点传输模式发送
 * @param ADDH : 模块地址高八位
 * @param ADDL : 模块地址低八位
 * @param Channel : 想要设置的信道
 * @param str : 想要发送的数据
 * @return 成功返回1，失败返回0
 */
uint8_t Lora_P2PSend(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, char *str)
{
	char SendBuf[3] = {0};
	SendBuf[0] = ADDH;
	SendBuf[1] = ADDL;
	SendBuf[2] = Channel;
	
//	USART2_SendBuf(SendBuf, 3); //发送目标地址、信道
//	USART2_SendBuf(str, strlen(str)); //发送数据
//	
//	return 1;
	
	while (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET) //发送前判断AUX引脚状态，AUX处于低电平，繁忙
	{
		Delay_nopnms(2);
		if (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET)//AUX处于高电平2ms
		{
			USART2_SendBuf(SendBuf, 3); //发送目标地址、信道
			USART2_SendBuf(str, strlen(str)); //发送数据
			
			while (GPIO_ReadInputDataBit(Lora_AUX_Port, Lora_AUX_Pin) == SET) //发送后判断AUX引脚状态，AUX处于高电平，发送完成
				return 1;
		}
	}
}

/**
 * @brief Lora 透明传输模式发送
 * @param str : 想要发送的数据
 * @return 成功返回1，失败返回0
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