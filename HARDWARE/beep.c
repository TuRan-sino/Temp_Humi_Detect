/****************************************************************************************************
@author: TuRan
@data: 
@des: 	蜂鸣器相关代码
****************************************************************************************************/

#include "beep.h"

extern u32 threshold_temp;
extern u32 threshold_humi;

/**
 * @brief 初始化蜂鸣器相关寄存器
 * @retval void
*/


void Beep_Init()
{
	// 使能GPIOB的时钟
	RCC->APB2ENR = 1<<3;

	// 设置输出模式为 	"推挽模式"
	// 设置最大速率为: 	"50MHZ"
	// 设置PIN口为:		"8"
	GPIOB->CRH |= 0x0000000f;
	GPIOB->CRH &= 0xfffffff3;
}

void Beep_Alarm(u32 temp, u32 humi)
{
	if(temp > threshold_temp){
		BEEP = BEEP_ON;
		delay_ms(50);
		BEEP = BEEP_OFF;
	}

	if(humi < threshold_humi){
		BEEP = BEEP_ON;
		delay_ms(50);
		BEEP = BEEP_OFF;
	}
}