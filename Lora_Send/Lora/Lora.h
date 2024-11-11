#ifndef _LORA_H
#define _LORA_H

#include "stm32f10x.h"
#include "USART.h"
#include "Delay.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define LoraMOD0_PORT GPIOA
#define LoraMOD1_PORT GPIOA
#define LoraMOD0_PIN  GPIO_Pin_0
#define LoraMOD1_PIN  GPIO_Pin_1


//定义LORA的工作状态
typedef enum LORAMODE
{
    NORMALMODE,	//正常工作模式
    ECOMODE,	//省电模式 :串口接收关闭，无线处于空中唤醒模式，收到无线数据后，打开串口发送数据
    WAKEMODE,	//唤醒工作模式 :和正常工作模式的唯一区别: 数据包发射前自动增加唤醒码，这样才能唤醒工作在省电工作状态下的接收方
    SLEEPMODE	//休眠模式 :需要使用AT指令配置Lora模块的时候必须为休眠模式
}LORAMODE;

//定义LORA的发送模式
typedef enum LORASENDMODE
{
	P2PMODE,	//点对点模式，定点传输
	TRANSMODE	//透明广播
}SENDMODE;


void Lora_Init(void);
void Lora_Mode(LORAMODE mode);
uint8_t Lora_SetUp(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, SENDMODE mode);
void Lora_P2PSend(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, char* str,uint8_t len);
void Lora_TransSend(char* str,uint8_t len);

#endif
