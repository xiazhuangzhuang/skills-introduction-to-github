#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Lora.h"

int main(void)
{
	NVIC_SetPriorityGrouping(5);
	USART1_Init(115200);
	USART2_Init(9600);
	printf("Ready\r\n");
	if(Lora_SetUp(0x11,0x11,0x17,P2PMODE)) //定点模式
		printf("SET DONE\r\n");
	//接收模块需要烧录的代码
//    if(Lora_SetUp(0x22,0x22,0x17,P2PMODE)) //定点模式
//		printf("SET DONE\r\n");


	while (1)
	{
		Lora_P2PSend(0x22,0x22,0x17,"Hello World~\r\n",13);
		Delay_ms(500);
		//接收模块需要烧录的代码
		//接收发送模块发来的数据并打印
//		if(USART2_RecData.Flag)
//		{
////			printf("%s",USART2_RecData.USART_RxData);
////			USART2_RecData.Flag = 0;
////			memset(USART2_RecData.USART_RxData,0,255);
//			
//			printf("%s",USART2_RecData.USART_RXBUF);
//			USART2_RecData.Flag = 0;
//			memset(USART2_RecData.USART_RXBUF,0,255);
//		}

	}
}
