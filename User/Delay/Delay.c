#include "Delay.h"

uint32_t LED_Run[2] = {0, 5000};
uint32_t BEEP_Run[2] = {0, 1};

//���жϷ���������0.1ms����һ�Σ����Ҳ���whileѭ����Ӱ��
//void SysTick_Handler(void)
//{
//	LED_Run[0]++;
//	BEEP_Run[0]++;
//}

void Delay_nop1us(void)
{
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

void Delay_nopnus(uint32_t timers)
{
	while(timers--) 
		Delay_nop1us();
}
