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
	NORMALMODE, //����ģʽ
	ECOMODE,    //ʡ��ģʽ :���ڽ��չرգ����ߴ��ڿ��л���ģʽ���յ��������ݺ󣬴򿪴��ڷ�������
	WAKEMODE,   //����ģʽ :����������ģʽ��Ψһ����: ���ݰ�����ǰ�Զ����ӻ����룬�������ܻ��ѹ�����ʡ�繤��״̬�µĽ��շ�
	SLEEPMODE   //˯��ģʽ :��Ҫʹ��ATָ������Loraģ���ʱ�����Ϊ����ģʽ
	
}LORAMODE;

typedef enum LoraCSMode
{
	TM = 0,   //͸������(Ĭ��)
	P2P,      //���㴫�� 
	
}LORACSMODE;

void Lora_Init(void);
void Lora_WorkMode(LORAMODE mode);
uint8_t Lora_CSMode(LORACSMODE csmode, uint8_t ADDH, uint8_t ADDL, uint8_t Channel);
uint8_t Lora_P2PSend(uint8_t ADDH, uint8_t ADDL, uint8_t Channel, char *str);
uint8_t Lora_TMSend(char *str);
	
#endif
