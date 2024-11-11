#ifndef LORA_H
#define LORA_H

#include "stm32f10x.h"
#include "Usart2.h"
#include "String.h"
#include "Delay.h"

#define Lora_MOD0_Port   GPIOA
#define Lora_MOD1_Port   GPIOA
#define Lora_MOD0_Pin    GPIO_Pin_0
#define Lora_MOD1_Pin    GPIO_Pin_1
#define Lora_AUX_Port    GPIOA
#define Lora_AUX_Pin     GPIO_Pin_4

typedef enum LoraMode
{
	NORMALMODE, //正常模式
	ECOMODE,    //省电模式 :串口接收关闭，无线处于空中唤醒模式，收到无线数据后，打开串口发送数据
	WAKEMODE,   //唤醒模式 :和正常工作模式的唯一区别: 数据包发射前自动增加唤醒码，这样才能唤醒工作在省电工作状态下的接收方
	SLEEPMODE   //睡眠模式 :需要使用AT指令配置Lora模块的时候必须为休眠模式
	
}LORAMODE;

typedef enum LoraCSMode
{
	TM = 0,   //透明传输(默认)
	P2P,      //定点传输 
	
}LORACSMODE;

void Lora_Init(void);
void Lora_WorkMode(LORAMODE mode);
uint8_t Lora_CSMode(LORACSMODE csmode, uint8_t ADDH, uint8_t ADDL, uint8_t Channel);
uint8_t Lora_P2PSend(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, char *str);
uint8_t Lora_TMSend(char *str);
	
#endif
