#include "Delay.h"

uint32_t LED_Run[2] = {0, 5000};
uint32_t BEEP_Run[2] = {0, 1};

//此中断服务函数将会0.1ms进入一次，并且不受while循环的影响
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
