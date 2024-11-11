#include "Lora.h"

//Lora模块的串口用到了USART2

/**
 * @brief Lora模块MOD0和MOD1引脚GPIO初始化
 * 
 */
void Lora_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitTypeDef GPIO_Struct;
    GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Struct.GPIO_Pin = LoraMOD0_PIN | LoraMOD1_PIN;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LoraMOD0_PORT,&GPIO_Struct);
}

/**
 * @brief 配置Lora模块的工作模式
 * 
 * @param mode 枚举变量,选择需要的工作状态
 */
void Lora_Mode(LORAMODE mode)
{
    Lora_Init();
    switch (mode)
    {
    case NORMALMODE: //正常工作模式
        GPIO_ResetBits(LoraMOD0_PORT,LoraMOD0_PIN);
        GPIO_ResetBits(LoraMOD1_PORT,LoraMOD1_PIN);
        break;
    case ECOMODE:    //省电模式
        GPIO_ResetBits(LoraMOD0_PORT,LoraMOD0_PIN);
        GPIO_SetBits(LoraMOD1_PORT,LoraMOD1_PIN);
        break;
    case WAKEMODE:  //唤醒工作模式
        GPIO_SetBits(LoraMOD0_PORT,LoraMOD0_PIN);
        GPIO_ResetBits(LoraMOD1_PORT,LoraMOD1_PIN);
        break;
    case SLEEPMODE: //睡眠模式
        GPIO_SetBits(LoraMOD0_PORT,LoraMOD0_PIN);
        GPIO_SetBits(LoraMOD1_PORT,LoraMOD1_PIN);
        break;
    }
}


/**
 * @brief 定点传输模式配置
 * 
 * @param ADDH 地址高位
 * @param ADDL 地址低位
 * @param Channel 信道
 * @param mode 设备需要配置的模式 P2PMODE 点对点模式 TRANSMODE 透明广播模式
 * @return uint8_t 配置成功返回1 失败返回0
 */
uint8_t Lora_SetUp(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, SENDMODE mode)
{
    char LoraArrSetUp[6];
    LoraArrSetUp[0] = 0xC0;		//设置模块参数指令，该指令设置的参数断电后可保存
    LoraArrSetUp[1] = ADDH;		//模块地址高 8 位
    LoraArrSetUp[2] = ADDL;		//模块地址低 8 位
    LoraArrSetUp[3] = 0x1A; 	//波特率设置
    LoraArrSetUp[4] = Channel;	//信道配置默认0x17(433MHz), 0x00(Min:410MHz), 0xFF(Max:441MHz)
	if(mode == P2PMODE)
		LoraArrSetUp[5] = 0xC0; 	//特殊功能寄存器,定点传输模式
    else if(mode == TRANSMODE)
		LoraArrSetUp[5] = 0x40; 	//特殊功能寄存器,透明传输模式
	else
		return 0;	
    uint16_t count = 10;
    Lora_Mode(SLEEPMODE); 			//切换为睡眠模式以接收控制指令
    while (count--)
    {
		USART_SendAString(USART2,LoraArrSetUp,6); //设置模块参数命令
        if(strstr((const char *)USART2_RecData.USART_RXBUF,"OK") != NULL)
        {
			Lora_Mode(NORMALMODE); //设置完之后切换为正常工作模式
            memset(USART2_RecData.USART_RXBUF,0,255);
            return 1;
        }
        Delay_ms(100);   
    }
    return 0;
}


/**
 * @brief 定点模式发送，发送方直接把数据发送到指定的地址和信道
 * 
 * @param ADDH 发送给指定的模块的地址高位
 * @param ADDL 发送给指定的模块的地址低位
 * @param Channel 选择和接收模块相同的信道
 * @param str 需要发送的内容
 * @param len 发送的内容长度
 */
void Lora_P2PSend(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, char* str,uint8_t len)
{
    char SendSetUp[3];
    SendSetUp[0] = ADDH;
    SendSetUp[1] = ADDL;
    SendSetUp[2] = Channel;
    USART_SendAString(USART2,SendSetUp,3);
    USART_SendNString(USART2,str,len);
}



/**
 * @brief 透明传输模式发送,需要保证全部模块的地址和信道相同
 * 
 * @param str 需要发送的内容
 * @param len 发送内容的长度
 */
void Lora_TransSend(char* str,uint8_t len)
{
    USART_SendNString(USART2,str,len);
}


