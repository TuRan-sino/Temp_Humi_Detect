/****************************************************************************************************
@author: TuRan
@data: 
@des: 	EXTI外部中断相关代码
****************************************************************************************************/
#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

void EXTI_Init_Mine(void)
{
	EXTI_InitTypeDef EI;		// 设置EXTI外部中断初始化的结构体

	NVIC_InitTypeDef NI;		// 设置NVIC中断优先级初始化的结构体

	KEY_Init();				// 初始化IO口为输入模式

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		// 打开GPIO端口复用时钟, 为了使得GPIO (GPIO_EXTILineConfig函数使用了EXTICR), 使用了该寄存器就必须开启AFIO

	EI.EXTI_Line = EXTI_Line0;
	EI.EXTI_LineCmd = ENABLE;
	EI.EXTI_Mode = EXTI_Mode_Interrupt;
	EI.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EI);

	// GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_Pin_0);

	NI.NVIC_IRQChannel = EXTI0_IRQn;
	NI.NVIC_IRQChannelCmd = ENABLE;
	NI.NVIC_IRQChannelPreemptionPriority = 1;
	NI.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NI);	
}