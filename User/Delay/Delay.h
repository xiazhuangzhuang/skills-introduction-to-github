#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h"

extern uint32_t LED_Run[2];
extern uint32_t BEEP_Run[2];

#define Delay_nopnms(n) Delay_nopnus(n * 1000);

void Delay_nopnus(uint32_t timers);

#endif
