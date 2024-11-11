#include "stm32f10x.h"
#include "Usart2.h"
#include "Usart1.h"
#include "Lora.h"
#include "Delay.h"

int main(void)
{
	NVIC_SetPriorityGrouping(5);
	USART1_Config(115200);
	USART2_Config(9600);
	
	printf("Ready\r\n");
	
#if 0 //���㴫��ģʽ������ģ����¼�Ĵ���
	if (Lora_CSMode(P2P, 0x11, 0x11, 0x17))
		printf("P2PMode Send OK\r\n");
#else //���㴫��ģʽ������ģ����¼�Ĵ���
	if (Lora_CSMode(P2P, 0x22, 0x22, 0x17))
		printf("P2PMode Recv OK\r\n");
#endif
	
	while(1)
	{
		#if 0 //���㴫��ģʽ������ģ����¼�Ĵ���
			Lora_P2PSend(0x22, 0x22, 0x17, "hello world~\r\n"); 
			Delay_nopnms(3000);
			printf("111111111111111\r\n");
		#else //���㴫��ģʽ������ģ����¼�Ĵ���
			if (USART2_GetData.flag)
			{
				printf("2222222222222\r\n");
				printf("USART2 GetData : %s\r\n", USART2_GetData.data);
				memset(USART2_GetData.data, 0, 256);
				USART2_GetData.flag = 0;
			}
		#endif
	}
}
