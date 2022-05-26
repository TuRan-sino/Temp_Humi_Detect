/****************************************************************************************************
@author: TuRan
@data: 
@des: 	LED相关代码
		LED0 --> 
****************************************************************************************************/
#include "led.h"
#include "stm32f10x.h"
#include "delay.h"

/**
 * @brief LED初始化函数
 * @param void
 * @retval void
*/
void LED_Init(void)
{
	RCC->APB2ENR |= 1<<3;
	RCC->APB2ENR |= 1<<6;

	// 初始化B， 清零
	GPIOB->CRL &= 0xff0fffff;
	// 设置推挽输出
	GPIOB->CRL |= 0x00300000;
	// 设置端口5输出高电平
	GPIOB->ODR |= 1<<5;


	// 初始化E组， 清零
	GPIOE->CRL &= 0xff0fffff;
	// 设置推挽输出
	GPIOE->CRL |= 0x00300000;
	// 设置端口5输出高电平
	GPIOE->ODR |= 1<<5;
}